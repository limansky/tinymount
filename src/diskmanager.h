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

#ifndef DISKMANAGER_H
#define DISKMANAGER_H

#include <QObject>
#include <QMap>
//#include <QSharedPointer>
#include <tr1/memory>

class UDisksInterface;
class QDBusObjectPath;

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
    bool mounted;
    QString udisksPath;
};

typedef std::tr1::shared_ptr<DeviceInfo> DeviceInfoPtr;

class DiskManager : public QObject
{
    Q_OBJECT
public:
    struct MountResult
    {
        MountResult(int error, const QString& path)
            : error(error)
            , path(path)
        {
        }

        int error;
        QString path;
    };

    explicit DiskManager(QObject *parent = 0);

    typedef QList<DeviceInfoPtr> Devices;
    typedef QMap<QString, DeviceInfoPtr> DeviceMap;

    Devices devices() const { return deviceCache.values(); }

    MountResult mountDevice(const QString& path);
    bool unmountDevice(const QString& path);

    const DeviceInfoPtr deviceByPath(const QString& path) const { return deviceCache.value(path); }

signals:
    void deviceAdded(const DeviceInfo& device);
    void deviceRemoved(const DeviceInfoPtr device);
    void deviceChanged(const DeviceInfo& device);

private slots:
    void onDeviceAdded(const QDBusObjectPath& path);
    void onDeviceRemoved(const QDBusObjectPath& path);
    void onDeviceChanged(const QDBusObjectPath& path);

private:
    DeviceInfoPtr deviceForPath(const QDBusObjectPath& path);

private:
    UDisksInterface* udisks;
    DeviceMap deviceCache;
};

#endif // DISKMANAGER_H
