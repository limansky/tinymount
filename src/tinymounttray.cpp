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
}

TinyMountTray::TinyMountTray(QObject *parent) :
    QObject(parent)
{
    qDebug() << "Creating UI";

    //QIcon::setThemeName("gtk+");

    manager = new DiskManager(this);
    connect(manager, SIGNAL(deviceAdded(DeviceInfo)), this, SLOT(onDeviceAdded(DeviceInfo)));
    connect(manager, SIGNAL(deviceRemoved(DeviceInfo)), this, SLOT(onDeviceRemoved(DeviceInfo)));

    trayMenu = new QMenu();
    updateMenu();

    tray = new QSystemTrayIcon(QApplication::windowIcon(), this);
    tray->show();
    tray->setContextMenu(trayMenu);
}

void TinyMountTray::updateMenu()
{
    trayMenu->clear();

    foreach (const DeviceInfo* d, manager->devices())
    {
        EventHandler * h = 0;
        if (d->mounted)
            h = new UnmountHandler(d->udisksPath, *manager, this);
        else
            h = new MountHandler(d->udisksPath, *manager, this);

        trayMenu->addAction(iconForType(d->type),
                            QString("%1 (%2) %3").arg(d->name).arg(d->fileSystem).arg(formatFileSize(d->size)),
                            h, SLOT(onEventHandled()));
    }

    trayMenu->addSeparator();
    trayMenu->addAction(tr("Quit"), qApp, SLOT(quit()));
}

void TinyMountTray::onDeviceAdded(const DeviceInfo &device)
{
    qDebug() << "Device added:" << device.name;
    tray->showMessage(tr("Device is added"), device.name);
    updateMenu();
}

void TinyMountTray::onDeviceRemoved(const DeviceInfo &device)
{
    qDebug() << "Device removed:" << device.name;
    tray->showMessage(tr("Device is removed"), device.name);
    updateMenu();
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
    manager.mountDevice(deviceId);
}

UnmountHandler::UnmountHandler(const QString &id, DiskManager& diskManager, QObject *parent)
    : EventHandler(id, diskManager, parent)
{
}

void UnmountHandler::handleEvent()
{
    manager.unmountDevice(deviceId);
}
