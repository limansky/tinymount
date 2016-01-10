/*
 * TinyMount -- simple disks mounter
 * Copyright (C) 2012-2016 Mike Limansky
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

#include "common.h"
#include "diskmanagerimpl.h"

#include <QObject>
#include <tr1/memory>

class DiskManagerImpl;
class QDBusObjectPath;
class QDBusPendingCallWatcher;

class DiskManager : public QObject, public DiskManagerImplListener
{
    Q_OBJECT
public:
    explicit DiskManager(QObject *parent = 0);

    bool isReady() const;

    typedef QList<Tinymount::DeviceInfoPtr> Devices;

    Devices devices() const;

    void mountDevice(const QString& path);
    void unmountDevice(const QString& path, bool force = false);
    void detachDevice(const QString& path);

signals:
    void deviceAdded(const Tinymount::DeviceInfo& device);
    void deviceRemoved(const Tinymount::DeviceInfo& device);
    void deviceChanged(const Tinymount::DeviceInfo& device);
    void deviceMounted(const Tinymount::DeviceInfo& device, const QString& path, int status);
    void deviceUnmounted(const Tinymount::DeviceInfo& device, int status);

private:
    virtual void onDeviceAdded(const Tinymount::DeviceInfo& deviceInfo);
    virtual void onDeviceChanged(const Tinymount::DeviceInfo& deviceInfo);
    virtual void onDeviceRemoved(const Tinymount::DeviceInfo& deviceInfo);
    virtual void onDeviceMounted(const Tinymount::DeviceInfo& deviceInfo, const QString& path, Tinymount::ErrorCode errorCode);
    virtual void onDeviceUnmounted(const Tinymount::DeviceInfo& deviceInfo, Tinymount::ErrorCode errorCode);

private:
    DiskManagerImpl* impl;
};

#endif // DISKMANAGER_H
