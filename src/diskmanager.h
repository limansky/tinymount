/*
 * TinyMount -- simple disks mounter
 * Copyright (C) 2012-2014 Mike Limansky
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

#ifndef DISKMANAGER_H
#define DISKMANAGER_H

#include "diskmanagerimpl.h"

#include <QObject>
#include <QMap>
#include <tr1/memory>

class UDisksInterface;
class QDBusObjectPath;
class QDBusPendingCallWatcher;

class DiskManager : public QObject
{
    Q_OBJECT
public:
    enum ErrorCode
    {
        OK,
        DBusError,
        NotAuthorized,
        Busy,
        Failed,
        Cancelled,
        InvalidRequest,
        UnknownFileSystem
    };

    explicit DiskManager(QObject *parent = 0);

    bool isReady() const { return ready; }

    typedef QList<DeviceInfoPtr> Devices;
    typedef QMap<QString, DeviceInfoPtr> DeviceMap;

    Devices devices() const { return deviceCache.values(); }

    void mountDevice(const QString& path);
    void unmountDevice(const QString& path, bool force = false);
    void detachDevice(const QString& path);

    const DeviceInfoPtr deviceByPath(const QString& path) const { return deviceCache.value(path); }

signals:
    void deviceAdded(const DeviceInfo& device);
    void deviceRemoved(const DeviceInfo& device);
    void deviceChanged(const DeviceInfo& device);
    void deviceMounted(const DeviceInfo& device, const QString& path, int status);
    void deviceUnmounted(const DeviceInfo& device, int status);

private slots:
    void onDeviceAdded(const QDBusObjectPath& path);
    void onDeviceRemoved(const QDBusObjectPath& path);
    void onDeviceChanged(const QDBusObjectPath& path);

    void onMountComplete(QDBusPendingCallWatcher* call);
    void onUnmountComplete(QDBusPendingCallWatcher* call);

private:
    DeviceInfoPtr deviceForPath(const QDBusObjectPath& path);

private:
    DiskManagerImpl* impl;
    UDisksInterface* udisks;
    DeviceMap deviceCache;
    bool ready;
};

#endif // DISKMANAGER_H
