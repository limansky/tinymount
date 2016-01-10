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

#include "diskmanager.h"
#ifdef USE_UDISKS2
#include "diskmanagerudisks2.h"
#else
#include "diskmanagerudisks1.h"
#include "udisks/udisksinterface.h"
#include "udisks/udisksdeviceinterface.h"
#endif

using namespace Tinymount;

DiskManager::DiskManager(QObject *parent) :
    QObject(parent)
{
#ifdef USE_UDISKS2
    impl = new DiskManagerUDisks2;
#else
    impl = new DiskManagerUDisks1(*this);
#endif
}

bool DiskManager::isReady() const
{
    return impl->isReady();
}

DiskManager::Devices DiskManager::devices() const
{
    return impl->devices();
}

void DiskManager::onDeviceAdded(const DeviceInfo &deviceInfo)
{
    emit deviceAdded(deviceInfo);
}

void DiskManager::onDeviceRemoved(const DeviceInfo &deviceInfo)
{
    emit deviceRemoved(deviceInfo);
}

void DiskManager::onDeviceMounted(const DeviceInfo& deviceInfo, const QString &path, ErrorCode errorCode)
{
    emit deviceMounted(deviceInfo, path, errorCode);
}

void DiskManager::onDeviceUnmounted(const DeviceInfo& deviceInfo, ErrorCode errorCode)
{
    emit deviceUnmounted(deviceInfo, errorCode);
}

void DiskManager::onDeviceChanged(const DeviceInfo &deviceInfo)
{
    emit deviceChanged(deviceInfo);
}

void DiskManager::mountDevice(const QString& path)
{
    impl->mountDevice(path);
}

void DiskManager::unmountDevice(const QString& path, bool force)
{
    impl->unmountDevice(path, force);
}

void DiskManager::detachDevice(const QString &path)
{
    impl->detachDevice(path);
}
