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
class DeviceInfo;
class DiskManager;

class TinyMountTray : public QObject
{
    Q_OBJECT
public:
    explicit TinyMountTray(QObject *parent = 0);

signals:

private slots:
    void onDeviceAdded(const DeviceInfo& device);
    void onDeviceRemoved(const DeviceInfo& device);

private:
    QSystemTrayIcon* tray;
    QMenu* trayMenu;
    DiskManager* manager;
};

#endif // TINYMOUNTTRAY_H
