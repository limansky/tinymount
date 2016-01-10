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

#ifndef TINYMOUNTTRAY_H
#define TINYMOUNTTRAY_H

#include <QObject>

class QSystemTrayIcon;
class QMenu;
class DiskManager;
class EventHandler;

namespace Tinymount
{
    struct DeviceInfo;
}

#ifdef WITH_LIBNOTIFY
class LibNotifier;
#endif

class TinyMountTray : public QObject
{
    Q_OBJECT
public:
    explicit TinyMountTray(QObject *parent = 0);
    virtual ~TinyMountTray();

public slots:
    void onDeviceAdded(const Tinymount::DeviceInfo& device);
    void onDeviceRemoved(const Tinymount::DeviceInfo& device);
    void reloadDevices();
    void showAbout();
    void showSettings();

    void onMount();
    void onUnmount();

    void onMountDone(const Tinymount::DeviceInfo& device, const QString& mountPath, int status);
    void onUnmountDone(const Tinymount::DeviceInfo& device, int status);

private:
    void showNotification(const QString& title, const QString &message, const QString &icon);

private:
    QSystemTrayIcon* tray;
    QMenu* trayMenu;
    DiskManager* manager;
#ifdef WITH_LIBNOTIFY
    LibNotifier* notifier;
#endif
};

#endif // TINYMOUNTTRAY_H
