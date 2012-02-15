#include "diskmanager.h"
#include "udisks/udisksinterface.h"

static const char* UDISKS_SERVICE = "org.freedesktop.UDisks";
static const char* UDISKS_PATH = "/org/freedesktop/UDisks";

DiskManager::DiskManager(QObject *parent) :
    QObject(parent)
{
    udisks = new UDisksInterface(UDISKS_SERVICE,
                                 UDISKS_PATH,
                                 QDBusConnection::systemBus(),
                                 this);

    connect(udisks, SIGNAL(DeviceAdded(QDBusObjectPath)), this, SLOT(onDeviceAdded(QDBusObjectPath)));


    QDBusPendingReply<QList<QDBusObjectPath> > devs =  udisks->EnumerateDevices();

    devs.waitForFinished();

    foreach(const QDBusObjectPath& d, devs.value())
    {
        DeviceInfo* info = deviceForPath(d);
        if (0 != info)
        {
            deviceCache.insert(d.path(), info);
        }
    }
}

DeviceInfo* DiskManager::deviceForPath(const QDBusObjectPath &path)
{
    DeviceInfo* d = new DeviceInfo;

    d->name = path.path();
    d->type = DeviceInfo::Other;
    d->mounted = false;

    return d;
}

void DiskManager::onDeviceAdded(const QDBusObjectPath &path)
{
    qDebug() << "Device added:" << path.path();

    DeviceInfo* d = deviceForPath(path);

    if (0 != d)
    {
        deviceCache.insert(path.path(), d);
        emit deviceAdded(*d);
    }
}

void DiskManager::onDeviceRemoved(const QDBusObjectPath &path)
{
    qDebug() << "Device removed:" << path.path();
    deviceCache.remove(path.path());
}
