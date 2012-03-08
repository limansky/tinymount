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
#include "udisks/udisksdeviceinterface.h"

static const char* UDISKS_SERVICE = "org.freedesktop.UDisks";
static const char* UDISKS_PATH = "/org/freedesktop/UDisks";

// Uncomment this to get verbose udisks log.
// #define LOG_DEVICE_INFO

namespace {
    bool containsFlashTypes(const QStringList& compatibility)
    {
        foreach (const QString& c, compatibility)
        {
            if (c.startsWith("flash"))
            {
                return true;
            }
        }
        return false;
    }

    DiskManager::ErrorCode mapErrorName(const QDBusError& error)
    {
        DiskManager::ErrorCode err = DiskManager::OK;

        if (error.isValid())
        {
            if (error.type() == QDBusError::Other)
            {
                if (error.name() == "org.freedesktop.UDisks.Error.NotAuthorized")
                {
                    err = DiskManager::NotAuthorized;
                }
                else if (error.name() == "org.freedesktop.UDisks.Error.Busy")
                {
                    err = DiskManager::Busy;
                }
                else if (error.name() == "org.freedesktop.UDisks.Error.Failed")
                {
                    err = DiskManager::Failed;
                }
                else if (error.name() == "org.freedesktop.UDisks.Error.Cancelled")
                {
                    err = DiskManager::Cancelled;
                }
                else if (error.name() == "org.freedesktop.UDisks.Error.FilesystemDriverMissing")
                {
                    err = DiskManager::UnknownFileSystem;
                }
                else
                {
                    err = DiskManager::InvalidRequest;
                }
            }
            else
            {
                err = DiskManager::DBusError;
            }
        }

        return err;
    }
}

DiskManager::DiskManager(QObject *parent) :
    QObject(parent)
{
    udisks = new UDisksInterface(UDISKS_SERVICE,
                                 UDISKS_PATH,
                                 QDBusConnection::systemBus(),
                                 this);

    connect(udisks, SIGNAL(DeviceAdded(QDBusObjectPath)), this, SLOT(onDeviceAdded(QDBusObjectPath)));
    connect(udisks, SIGNAL(DeviceRemoved(QDBusObjectPath)), this, SLOT(onDeviceRemoved(QDBusObjectPath)));
    connect(udisks, SIGNAL(DeviceChanged(QDBusObjectPath)), this, SLOT(onDeviceChanged(QDBusObjectPath)));

    QDBusPendingReply<QList<QDBusObjectPath> > devs =  udisks->EnumerateDevices();

    devs.waitForFinished();

    foreach(const QDBusObjectPath& d, devs.value())
    {
        DeviceInfoPtr info = deviceForPath(d);
        if (0 != info)
        {
            qDebug() << "Adding device to cache:" << d.path();
            deviceCache.insert(d.path(), info);
        }
    }
}

DeviceInfoPtr DiskManager::deviceForPath(const QDBusObjectPath &path)
{
    UDisksDeviceInterface dev(UDISKS_SERVICE, path.path(), QDBusConnection::systemBus());

#ifdef LOG_DEVICE_INFO
    qDebug() << "DiskManager::deviceForPath" << path.path();
    qDebug() << dev.nativePath();
    qDebug() << "\tsize =" << dev.deviceSize();
    qDebug() << "\tisDrive =" << dev.deviceIsDrive();
    qDebug() << "\tisVolume =" << dev.deviceIsPartition();
    qDebug() << "\tisParttable =" << dev.deviceIsPartitionTable();
    qDebug() << "\tisRemovable =" << dev.deviceIsRemovable();
    qDebug() << "\tisMounted =" << dev.deviceIsMounted();
    qDebug() << "\tisOptical =" << dev.deviceIsOpticalDisc();
    qDebug() << "\tisSysInt =" << dev.deviceIsSystemInternal();
    qDebug() << "\ttype and usage =" << dev.idType() << dev.idUsage();
    qDebug() << "\tLabal =" << dev.idLabel();
    qDebug() << "\tplabel =" << dev.partitionLabel();
    qDebug() << "\tdpname =" << dev.devicePresentationName() << ", " << dev.devicePresentationIconName();
    qDebug() << "\tcompatibility" << dev.driveMediaCompatibility();
#endif

    DeviceInfoPtr d;

    if (dev.deviceIsPartition() || dev.deviceIsOpticalDisc())
    {
        if (dev.partitionType() != "0x05"    // extended partition
            && dev.partitionType() != "0x0f"    // extended Win95 partition
            && dev.idType() != "swap"        // skip swap
           )
        {
            d = DeviceInfoPtr(new DeviceInfo);

            d->udisksPath = path.path();
            const QString& label = dev.idLabel();
            const QString& fn = dev.deviceFile();
            d->name = label.isEmpty() ? fn.mid(fn.lastIndexOf('/') + 1) : label;
            d->size = dev.deviceSize();
            d->fileSystem = dev.idType();
            d->type = dev.deviceIsSystemInternal() ? DeviceInfo::HDD :
                      dev.deviceIsOpticalDisc()    ? DeviceInfo::CD  :
                      dev.driveMediaCompatibility().contains("floppy") ? DeviceInfo::Floppy :
                      containsFlashTypes(dev.driveMediaCompatibility()) ? DeviceInfo::Flash :
                                                                          DeviceInfo::Other;
            d->mounted = dev.deviceIsMounted();
        }
    }

    return d;
}

void DiskManager::onDeviceAdded(const QDBusObjectPath &path)
{
    qDebug() << "Device added:" << path.path();

    DeviceInfoPtr d = deviceForPath(path);

    if (0 != d)
    {
        deviceCache.insert(path.path(), d);
        emit deviceAdded(*d);
    }
}

void DiskManager::onDeviceRemoved(const QDBusObjectPath &path)
{
    qDebug() << "Device removed:" << path.path();

    DeviceInfoPtr d = deviceCache.value(path.path());
    if (0 != d)
    {
        emit deviceRemoved(*d);
        deviceCache.remove(path.path());
    }
}

void DiskManager::onDeviceChanged(const QDBusObjectPath &path)
{
    qDebug() << "Device changed:" << path.path();

    DeviceInfoPtr d = deviceForPath(path);

    if (0 != d)
    {
        deviceCache.insert(path.path(), d);
        emit deviceChanged(*d);
    }
    else
    {
        deviceCache.remove(path.path());
    }
}

DiskManager::MountResult DiskManager::mountDevice(const QString& path)
{
    qDebug() << "mountDevice:" << path;
    DeviceMap::iterator it = deviceCache.find(path);

    if (it == deviceCache.end())
    {
        qWarning() << "Unknown device passed. Path = " << path;
        return MountResult(InvalidRequest, "");
    }

    UDisksDeviceInterface dev(UDISKS_SERVICE, path, QDBusConnection::systemBus());

    QDBusPendingReply<QString> r = dev.FilesystemMount("", QStringList());

    r.waitForFinished();

    const QString& mountPath = r.isValid() ? r.value() : "";
    return MountResult(mapErrorName(r.error()), mountPath);
}

int DiskManager::unmountDevice(const QString& path)
{
    qDebug() << "unmountDevice:" << path;
    DeviceMap::iterator it = deviceCache.find(path);

    if (it == deviceCache.end())
    {
        qWarning() << "Unknown device passed. Path = " << path;
        return false;
    }

    UDisksDeviceInterface dev(UDISKS_SERVICE, path, QDBusConnection::systemBus());

    QDBusPendingReply<> r = dev.FilesystemUnmount(QStringList());

    r.waitForFinished();

    return mapErrorName(r.error());
}
