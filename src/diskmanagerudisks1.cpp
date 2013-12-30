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

#include "diskmanagerudisks1.h"
#include "udisks/udisksinterface.h"
#include "udisks/udisksdeviceinterface.h"

static const char* UDISKS_SERVICE = "org.freedesktop.UDisks";
static const char* UDISKS_PATH = "/org/freedesktop/UDisks";

DiskManagerUDisks1::DiskManagerUDisks1(QObject *parent) : QObject(parent)
{
    udisks = new UDisksInterface(UDISKS_SERVICE,
                                 UDISKS_PATH,
                                 QDBusConnection::systemBus(),
                                 this);

    connect(udisks, SIGNAL(DeviceAdded(QDBusObjectPath)), this, SLOT(onDeviceAdded(QDBusObjectPath)));
    connect(udisks, SIGNAL(DeviceRemoved(QDBusObjectPath)), this, SLOT(onDeviceRemoved(QDBusObjectPath)));
    connect(udisks, SIGNAL(DeviceChanged(QDBusObjectPath)), this, SLOT(onDeviceChanged(QDBusObjectPath)));
}

DiskManagerUDisks1::~DiskManagerUDisks1()
{
    delete udisks;
}

void DiskManagerUDisks1::mountDevice(const QString& /*path*/)
{
}

void DiskManagerUDisks1::unmountDevice(const QString& /*path*/)
{
}

void DiskManagerUDisks1::onDeviceAdded(const QDBusObjectPath& /*path*/)
{

}

void DiskManagerUDisks1::onDeviceRemoved(const QDBusObjectPath& /*path*/)
{

}

void DiskManagerUDisks1::onDeviceChanged(const QDBusObjectPath& /*path*/)
{

}

void DiskManagerUDisks1::onMountComplete(QDBusPendingCallWatcher* /*call*/)
{

}

void DiskManagerUDisks1::onUnmountComplete(QDBusPendingCallWatcher* /*call*/) {

}
