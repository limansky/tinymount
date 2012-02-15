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

#include "tinymounttray.h"
#include "diskmanager.h"

#include <QSystemTrayIcon>
#include <QIcon>
#include <QtDebug>
#include <QMenu>
#include <QApplication>

TinyMountTray::TinyMountTray(QObject *parent) :
    QObject(parent)
{
    qDebug() << "Creating UI";

    // HACK!!! always hicolor on my PC
    QIcon::setThemeName("oxygen");

    manager = new DiskManager(this);
    connect(manager, SIGNAL(deviceAdded(DeviceInfo)), this, SLOT(onDeviceAdded(DeviceInfo)));
    connect(manager, SIGNAL(deviceRemoved(DeviceInfo)), this, SLOT(onDeviceRemoved(DeviceInfo)));

    trayMenu = new QMenu();

    foreach (const DeviceInfo* d, manager->devices())
    {
        trayMenu->addAction(d->name);
    }

    trayMenu->addSeparator();
    trayMenu->addAction(tr("Quit"), qApp, SLOT(quit()));

    tray = new QSystemTrayIcon(this);
    tray->setIcon(QIcon::fromTheme("drive-removable-media"));
    tray->show();
    tray->setContextMenu(trayMenu);
}

void TinyMountTray::onDeviceAdded(const DeviceInfo &device)
{
    qDebug() << "Device added:" << device.name;
    tray->showMessage(tr("Device is added"), device.name);
}

void TinyMountTray::onDeviceRemoved(const DeviceInfo &device)
{
    qDebug() << "Device removed:" << device.name;
    tray->showMessage(tr("Device is removed"), device.name);
}
