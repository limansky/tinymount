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

#ifndef DISKMANAGERIMPL_H
#define DISKMANAGERIMPL_H

#include <QString>
#include <tr1/memory>

struct DeviceInfo
{
    enum DeviceType
    {
        HDD,
        CD,
        Floppy,
        Flash,
        Other
    };

    QString name;
    qulonglong size;
    QString fileSystem;
    DeviceType type;
    bool isMounted;
    QString mountPoint;
    QString udisksPath;
    bool isSystem;
};

typedef std::tr1::shared_ptr<DeviceInfo> DeviceInfoPtr;

class DiskManagerImpl
{
public:
    virtual void mountDevice(const QString& path) = 0;
    virtual void unmountDevice(const QString& path) = 0;
    virtual ~DiskManagerImpl() {}
};

class DiskManagerImplListener
{
public:
    virtual void onDeviceAdded(DeviceInfoPtr deviceInfo) = 0;
    virtual void onDeviceRemoved(DeviceInfoPtr deviceInfo) = 0;
    virtual void onDeviceChanged(DeviceInfoPtr deviceInfo) = 0;
};

#endif // DISKMANAGERIMPL_H
