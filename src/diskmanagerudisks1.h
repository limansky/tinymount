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

#ifndef DISKMANAGERUDISKS1_H
#define DISKMANAGERUDISKS1_H

#include "diskmanagerimpl.h"
#include <QObject>

class UDisksInterface;
class QDBusObjectPath;
class QDBusPendingCallWatcher;

class DiskManagerUDisks1 : public QObject, public DiskManagerImpl
{
    Q_OBJECT
public:
    DiskManagerUDisks1(QObject* parent = 0);
    virtual ~DiskManagerUDisks1();

    virtual void mountDevice(const QString& path);
    virtual void unmountDevice(const QString& path);

public slots:
    void onDeviceAdded(const QDBusObjectPath& path);
    void onDeviceRemoved(const QDBusObjectPath& path);
    void onDeviceChanged(const QDBusObjectPath& path);

    void onMountComplete(QDBusPendingCallWatcher* call);
    void onUnmountComplete(QDBusPendingCallWatcher* call);

private:
    UDisksInterface* udisks;
};

#endif // DISKMANAGERUDISKS1_H
