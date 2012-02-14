#include "tinymounttray.h"
#include "udisks/udisksinterface.h"

#include <QSystemTrayIcon>
#include <QIcon>
#include <QtDebug>
#include <QMenu>
#include <QApplication>

static const char* UDISKS_SERVICE = "org.freedesktop.UDisks";
static const char* UDISKS_PATH = "/org/freedesktop/UDisks";

TinyMountTray::TinyMountTray(QObject *parent) :
    QObject(parent)
{
    qDebug() << "Creating UI";

    // HACK!!! always hicolor on my PC
    QIcon::setThemeName("oxygen");

    udisks = new UDisksInterface(UDISKS_SERVICE,
                                 UDISKS_PATH,
                                 QDBusConnection::systemBus(),
                                 this);

    connect(udisks, SIGNAL(DeviceAdded(QDBusObjectPath)), this, SLOT(onDeviceAdded(QDBusObjectPath)));

    tray = new QSystemTrayIcon(this);
    tray->setIcon(QIcon::fromTheme("drive-removable-media"));
    tray->show();

    trayMenu = new QMenu();
    trayMenu->addAction(tr("Quit"), qApp, SLOT(quit()));

    tray->setContextMenu(trayMenu);
}

void TinyMountTray::onDeviceAdded(const QDBusObjectPath &path)
{
    qDebug() << "Device added:" << path.path();
    tray->showMessage(tr("Device is added"), path.path());
}

void TinyMountTray::onDeviceRemoved(const QDBusObjectPath &path)
{
    qDebug() << "Device removed:" << path.path();
    tray->showMessage(tr("Device is removed"), path.path());
}
