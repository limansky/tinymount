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
