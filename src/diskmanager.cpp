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

#include "diskmanager.h"
#include "udisks/udisksinterface.h"

static const char* UDISKS_SERVICE = "org.freedesktop.UDisks";
static const char* UDISKS_PATH = "/org/freedesktop/UDisks";

DiskManager::DiskManager(QObject *parent) :
    QObject(parent)
{
    udisks = new UDisksInterface(UDISKS_SERVICE,
                                 UDISKS_PATH,
                                 QDBusConnection::systemBus(),
                                 this);

    connect(udisks, SIGNAL(DeviceAdded(QDBusObjectPath)), this, SLOT(onDeviceAdded(QDBusObjectPath)));


    QDBusPendingReply<QList<QDBusObjectPath> > devs =  udisks->EnumerateDevices();

    devs.waitForFinished();

    foreach(const QDBusObjectPath& d, devs.value())
    {
        DeviceInfo* info = deviceForPath(d);
        if (0 != info)
        {
            deviceCache.insert(d.path(), info);
        }
    }
}

DeviceInfo* DiskManager::deviceForPath(const QDBusObjectPath &path)
{
    DeviceInfo* d = new DeviceInfo;

    d->name = path.path();
    d->type = DeviceInfo::Other;
    d->mounted = false;

    return d;
}

void DiskManager::onDeviceAdded(const QDBusObjectPath &path)
{
    qDebug() << "Device added:" << path.path();

    DeviceInfo* d = deviceForPath(path);

    if (0 != d)
    {
        deviceCache.insert(path.path(), d);
        emit deviceAdded(*d);
    }
}

void DiskManager::onDeviceRemoved(const QDBusObjectPath &path)
{
    qDebug() << "Device removed:" << path.path();
    deviceCache.remove(path.path());
}
