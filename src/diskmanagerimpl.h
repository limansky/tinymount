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

#ifndef DISKMANAGERIMPL_H
#define DISKMANAGERIMPL_H

#include "common.h"

#include <QString>

class DiskManagerImpl
{
public:
    virtual void mountDevice(const QString& path) = 0;
    virtual void unmountDevice(const QString& path, bool force) = 0;
    virtual void detachDevice(const QString& path) = 0;
    virtual bool isReady() const = 0;
    virtual Tinymount::Devices devices() const = 0;
    virtual ~DiskManagerImpl() {}
};

class DiskManagerImplListener
{
public:
    virtual void onDeviceAdded(const Tinymount::DeviceInfo& deviceInfo) = 0;
    virtual void onDeviceRemoved(const Tinymount::DeviceInfo& deviceInfo) = 0;
    virtual void onDeviceChanged(const Tinymount::DeviceInfo& deviceInfo) = 0;
    virtual void onDeviceMounted(const Tinymount::DeviceInfo& deviceInfo, const QString& path, Tinymount::ErrorCode errorCode) = 0;
    virtual void onDeviceUnmounted(const Tinymount::DeviceInfo& deviceInfo, Tinymount::ErrorCode errorCode) = 0;
};

#endif // DISKMANAGERIMPL_H
