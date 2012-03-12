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

#ifndef TINYMOUNTTRAY_H
#define TINYMOUNTTRAY_H

#include <QObject>

class QSystemTrayIcon;
class QMenu;
class DeviceInfo;
class DiskManager;
class EventHandler;

class TinyMountTray : public QObject
{
    Q_OBJECT
public:
    explicit TinyMountTray(QObject *parent = 0);

public slots:
    void onDeviceAdded(const DeviceInfo& device);
    void onDeviceRemoved(const DeviceInfo& device);
    void reloadDevices();
    void showAbout();
    void showSettings();

    void onMountDone(const QString& devPath, const QString& mountPath, int status);
    void onUnmountDone(const QString& devPath, int status);

private:
    QSystemTrayIcon* tray;
    QMenu* trayMenu;
    DiskManager* manager;
    QList<EventHandler*> handers;
};

class EventHandler : public QObject
{
    Q_OBJECT
public:
    EventHandler(const QString& id, DiskManager& diskManager, QObject* parent = 0);

public slots:
    void onEventHandled();

protected:
    virtual void handleEvent() = 0;

protected:
    QString deviceId;
    DiskManager& manager;
};

class MountHandler : public EventHandler
{
    Q_OBJECT
public:
    MountHandler(const QString& id, DiskManager& diskManager, QObject* parent = 0);

signals:
    void mountDone(const QString& devPath, const QString& mountPath, int status);

protected:
    virtual void handleEvent();
};

class UnmountHandler : public EventHandler
{
    Q_OBJECT
public:
    UnmountHandler(const QString& id, DiskManager& diskManager, QObject* parent = 0);

signals:
    void unmountDone(const QString& path, int status);

public slots:
    virtual void handleEvent();
};

#endif // TINYMOUNTTRAY_H
