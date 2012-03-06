/*
 * TinyMount -- simple disks mounter
 * Copyright (C) 2012 Mike Limansky
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "tinymounttray.h"
#include "diskmanager.h"

#include <QSystemTrayIcon>
#include <QIcon>
#include <QtDebug>
#include <QMenu>
#include <QApplication>
#include <QMessageBox>

namespace {
    QIcon iconForType (DeviceInfo::DeviceType type)
    {
        switch (type)
        {
        case DeviceInfo::CD:
            return QIcon::fromTheme("media-optical");
        case DeviceInfo::Flash:
        case DeviceInfo::Other:
            return QIcon::fromTheme("media-flash");
        case DeviceInfo::Floppy:
            return QIcon::fromTheme("media-floppy");
        case DeviceInfo::HDD:
            return QIcon::fromTheme("drive-harddisk");
        }

        return QIcon();
    }

    QString formatFileSize(qulonglong size)
    {
        static const QStringList units = QStringList()
                << "B" << "KB" << "MB" << "GB" << "TB";
        int i = 0;
        double sizeD = size;
        while (sizeD > 1024 && i < units.size())
        {
            sizeD /= 1024;
            i++;
        }

        return QString::number(sizeD, 'f', 2) + " " + units[i];
    }

    QString errorToString(int error)
    {
        QString string;

        switch (error)
        {
        case DiskManager::DBusError:
            string = qApp->translate("Errors", "Communication error");
            break;
        case DiskManager::NotAuthorized:
            string = qApp->translate("Errors", "User not authorized");
            break;
        case DiskManager::Busy:
            string = qApp->translate("Errors", "Device is busy");
            break;
        case DiskManager::Failed:
            string = qApp->translate("Errors", "Operation is failed.");
            break;
        case DiskManager::Cancelled:
            string = qApp->translate("Errors", "Request is cancelled");
            break;
        case DiskManager::UnknownFileSystem:
            string = qApp->translate("Errors", "Unknown filesystem");
            break;
        default:
            string = qApp->translate("Errors", "Unknown error");
            break;
        };

        qDebug() << error << string;
        return string;
    }
}

TinyMountTray::TinyMountTray(QObject *parent) :
    QObject(parent)
{
    qDebug() << "Creating UI";

    manager = new DiskManager(this);
    connect(manager, SIGNAL(deviceAdded(DeviceInfo)), this, SLOT(onDeviceAdded(DeviceInfo)));
    connect(manager, SIGNAL(deviceRemoved(DeviceInfo)), this, SLOT(onDeviceRemoved(DeviceInfo)));
    connect(manager, SIGNAL(deviceChanged(DeviceInfo)), this, SLOT(reloadDevices()));

    trayMenu = new QMenu();
    reloadDevices();

    tray = new QSystemTrayIcon(QApplication::windowIcon(), this);
    tray->show();
    tray->setContextMenu(trayMenu);

    connect(tray, SIGNAL(messageClicked()), trayMenu, SLOT(show()));
}

void TinyMountTray::reloadDevices()
{
    trayMenu->clear();
    qDeleteAll(handers);
    handers.clear();

    foreach (const DeviceInfoPtr d, manager->devices())
    {
        EventHandler * h = 0;
        QIcon icon;
        if (d->mounted)
        {
            UnmountHandler* uh = new UnmountHandler(d->udisksPath, *manager, this);
            connect(uh, SIGNAL(unmountDone(QString,int)), this, SLOT(onUnmountDone(QString,int)));
            h = uh;
            icon = QIcon::fromTheme("media-eject");
        }
        else
        {
            MountHandler* mh = new MountHandler(d->udisksPath, *manager, this);
            connect(mh, SIGNAL(mountDone(QString,QString,int)), this, SLOT(onMountDone(QString,QString,int)));
            h = mh;
            icon = iconForType(d->type);
        }

        trayMenu->addAction(icon,
                            QString("%1 (%2) %3").arg(d->name).arg(d->fileSystem).arg(formatFileSize(d->size)),
                            h, SLOT(onEventHandled()));
        handers << h;
    }

    trayMenu->addSeparator();
    trayMenu->addAction(tr("About..."), this, SLOT(showAbout()));
    trayMenu->addAction(tr("Quit"), qApp, SLOT(quit()));
}

void TinyMountTray::onDeviceAdded(const DeviceInfo &device)
{
    qDebug() << "Device added:" << device.name;
    tray->showMessage(tr("Device is added"), device.name);
    reloadDevices();
}

void TinyMountTray::onDeviceRemoved(const DeviceInfo& device)
{
    qDebug() << "Device removed:" << device.name;
    tray->showMessage(tr("Device is removed"), device.name);
    reloadDevices();
}

void TinyMountTray::onMountDone(const QString &devPath, const QString &mountPath, int status)
{
    qDebug() << "Device" << devPath << "is mounted to" << mountPath << ", status =" << status;

    const DeviceInfoPtr d = manager->deviceByPath(devPath);
    Q_ASSERT(0 != d);

    if (DiskManager::OK == status)
    {
        tray->showMessage(tr("Device is mounted"), tr("%1 is mounted to %2").arg(d->name).arg(mountPath));
    }
    else
    {
        tray->showMessage(tr("Mount failed"), tr("%1 mounting error. %2").arg(d->name).arg(errorToString(status)));
    }
}

void TinyMountTray::onUnmountDone(const QString &devPath, int status)
{
    qDebug() << "Device" << devPath << "is unmounted, status =" << status;
    const DeviceInfoPtr d = manager->deviceByPath(devPath);

    Q_ASSERT(0 != d);

    if (DiskManager::OK == status)
    {
        tray->showMessage(tr("Device is unmounted"), tr("%1 is unmounted successfuly").arg(d->name));
    }
    else
    {
        tray->showMessage(tr("Unmount failed"), tr("Failed to unmount %1. %2").arg(d->name).arg(errorToString(status)));
    }
}

void TinyMountTray::showAbout()
{
    QMessageBox::about(0, tr("TinyMount, version %1").arg(TINYMOUNT_VERSION),
                          tr("Copyright (c) 2012 Mike Limansky\n\n"
                             "Use and redistribute under terms of the GNU General Public License Version 2."));
}

EventHandler::EventHandler(const QString &id, DiskManager &diskManager, QObject *parent)
    : QObject(parent)
    , deviceId(id)
    , manager(diskManager)
{
}

void EventHandler::onEventHandled()
{
    handleEvent();
}

MountHandler::MountHandler(const QString &id, DiskManager& diskManager, QObject *parent)
    : EventHandler(id, diskManager, parent)
{
}

void MountHandler::handleEvent()
{
    DiskManager::MountResult r = manager.mountDevice(deviceId);
    emit mountDone(deviceId, r.path, r.error);
}

UnmountHandler::UnmountHandler(const QString &id, DiskManager& diskManager, QObject *parent)
    : EventHandler(id, diskManager, parent)
{
}

void UnmountHandler::handleEvent()
{
    int err = manager.unmountDevice(deviceId);
    emit unmountDone(deviceId, err);
}
