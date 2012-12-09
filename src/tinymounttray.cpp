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
#include "settings.h"
#include "settingsdialog.h"

#ifdef WITH_LIBNOTIFY
#include "libnotifier.h"
#endif

#include <QSystemTrayIcon>
#include <QIcon>
#include <QtDebug>
#include <QMenu>
#include <QApplication>
#include <QMessageBox>
#include <QTimer>

namespace {
    QString iconNameForType (DeviceInfo::DeviceType type)
    {
        switch (type)
        {
        case DeviceInfo::CD:
            return "media-optical";
        case DeviceInfo::Flash:
        case DeviceInfo::Other:
            return "media-flash";
        case DeviceInfo::Floppy:
            return "media-floppy";
        case DeviceInfo::HDD:
            return "drive-harddisk";
        }

        return QString();
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
            string = qApp->translate("Errors", "DBus error");
            break;
        case DiskManager::NotAuthorized:
            string = qApp->translate("Errors", "User is not authorized");
            break;
        case DiskManager::Busy:
            string = qApp->translate("Errors", "Device is busy");
            break;
        case DiskManager::Failed:
            string = qApp->translate("Errors", "Operation is failed");
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

    connect(manager, SIGNAL(deviceMounted(DeviceInfo,QString,int)), this, SLOT(onMountDone(DeviceInfo,QString,int)));
    connect(manager, SIGNAL(deviceUnmounted(DeviceInfo,int)), this, SLOT(onUnmountDone(DeviceInfo,int)));

    trayMenu = new QMenu();

    tray = new QSystemTrayIcon(QApplication::windowIcon(), this);
    tray->setContextMenu(trayMenu);

    connect(tray, SIGNAL(messageClicked()), trayMenu, SLOT(show()));

#ifdef WITH_LIBNOTIFY
    notifier = new LibNotifier("tinymount");
#endif

    QTimer::singleShot(0, this, SLOT(reloadDevices()));
}

TinyMountTray::~TinyMountTray()
{
#ifdef WITH_LIBNOTIFY
    delete notifier;
#endif
}

void TinyMountTray::reloadDevices()
{
    trayMenu->clear();

    bool showSystem = SettingsManager::instance().getSettings().showSystemDisks;
    bool hasDevices = false;

    foreach (const DeviceInfoPtr d, manager->devices())
    {
        if (!showSystem && d->isSystem) continue;

        hasDevices = true;

        QIcon icon;
        QString text = SettingsManager::instance().getSettings().itemFormat;
        text.replace("%name%", d->name).replace("%fs%", d->fileSystem).replace("%size%", formatFileSize(d->size));

        if (d->isMounted)
        {
            icon = QIcon::fromTheme("media-eject");
            text.replace("%mounted%", d->mountPoint);
        }
        else
        {
            icon = QIcon::fromTheme(iconNameForType(d->type));
            text.replace("%mounted%", "");
        }

        QAction* action = new QAction(icon, text, trayMenu);
        action->setData(d->udisksPath);

        if (d->isMounted)
        {
            connect(action, SIGNAL(triggered()), this, SLOT(onUnmount()));
        }
        else
        {
            connect(action, SIGNAL(triggered()), this, SLOT(onMount()));
        }

        trayMenu->addAction(action);
    }

    trayMenu->addSeparator();
    trayMenu->addAction(tr("Settings"), this, SLOT(showSettings()));
    trayMenu->addAction(tr("About..."), this, SLOT(showAbout()));
    trayMenu->addAction(QIcon::fromTheme("exit"), tr("Quit"), qApp, SLOT(quit()));

    tray->setVisible(!SettingsManager::instance().getSettings().hideIcon || hasDevices);
}

void TinyMountTray::onDeviceAdded(const DeviceInfo &device)
{
    qDebug() << "Device added:" << device.name;

    if (SettingsManager::instance().getSettings().deviceNotifications)
        showNotification(tr("Device is added"),
                         tr("Device %1 is added").arg(device.name),
                         iconNameForType(device.type));

    if (SettingsManager::instance().getSettings().mountAutomaticaly)
        manager->mountDevice(device.udisksPath);

    reloadDevices();
}

void TinyMountTray::onDeviceRemoved(const DeviceInfo& device)
{
    qDebug() << "Device removed:" << device.name;

    if (SettingsManager::instance().getSettings().deviceNotifications)
        showNotification(tr("Device is removed"),
                         tr("Device %1 is removed").arg(device.name),
                         iconNameForType(device.type));

    reloadDevices();
}

void TinyMountTray::onMount()
{
    QAction* a = qobject_cast<QAction*>(sender());
    Q_ASSERT(a);

    const QString& path = a->data().toString();

    manager->mountDevice(path);
}

void TinyMountTray::onUnmount()
{
    QAction* a = qobject_cast<QAction*>(sender());
    Q_ASSERT(a);

    const QString& path = a->data().toString();

    manager->unmountDevice(path);
}

void TinyMountTray::onMountDone(const DeviceInfo &device, const QString &mountPath, int status)
{
    qDebug() << "Device" << device.name << "is mounted to" << mountPath << ", status =" << status;

    if (DiskManager::OK == status)
    {
        if (SettingsManager::instance().getSettings().mountNotifications)
            showNotification(tr("Device is mounted"),
                             tr("%1 is mounted to %2.").arg(device.name).arg(mountPath),
                             iconNameForType(device.type));
    }
    else
    {
        showNotification(tr("Mount failed"),
                         tr("%1 mounting error. %2.").arg(device.name).arg(errorToString(status)),
                         "dialog-error");
    }
}

void TinyMountTray::onUnmountDone(const DeviceInfo &device, int status)
{
    qDebug() << "Device" << device.name << "is unmounted, status =" << status;

    if (DiskManager::OK == status)
    {
        const Settings& settings = SettingsManager::instance().getSettings();
        if (settings.mountNotifications)
            showNotification(tr("Device is unmounted"),
                             tr("%1 is unmounted successfuly.").arg(device.name),
                             iconNameForType(device.type));

        if (settings.detachRemovable)
            manager->detachDevice(device.udisksPath);

    }
    else
    {
        showNotification(tr("Unmount failed"),
                         tr("Failed to unmount %1. %2.").arg(device.name).arg(errorToString(status)),
                         "dialog-error");
    }
}

void TinyMountTray::showAbout()
{
    QMessageBox::about(0, tr("TinyMount, version %1").arg(TINYMOUNT_VERSION),
                          tr("Copyright (c) 2012 Mike Limansky\n\n"
                             "Use and redistribute under terms of the GNU General Public License Version 2."));
}

void TinyMountTray::showSettings()
{
    SettingsDialog dlg(SettingsManager::instance().getSettings());

    if (QDialog::Accepted == dlg.exec())
    {
        const Settings& settings = dlg.getSettings();
        const Settings& oldSettings = SettingsManager::instance().getSettings();
        bool refreshList = settings.showSystemDisks != oldSettings.showSystemDisks
                || settings.itemFormat != oldSettings.itemFormat
                || settings.hideIcon != oldSettings.hideIcon;
        SettingsManager::instance().save(settings);

        if (refreshList)
        {
            reloadDevices();
        }
    }
}

void TinyMountTray::showNotification(const QString& title, const QString &message, const QString& icon)
{
#ifdef WITH_LIBNOTIFY
    if (SettingsManager::instance().getSettings().useLibnotify)
    {
        notifier->showNotification(title, message, icon);
    }
    else
    {
        tray->showMessage(title, message);
    }
#else
    Q_UNUSED(icon);
    tray->showMessage(title, message);
#endif
}
