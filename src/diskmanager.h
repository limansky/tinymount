#ifndef DISKMANAGER_H
#define DISKMANAGER_H

#include <QObject>
#include <QMap>

class UDisksInterface;
class QDBusObjectPath;

struct DeviceInfo
{
    enum DeviceType
    {
        HardDisk,
        CD,
        Floppy,
        SD,
        Other
    };

    QString name;
    DeviceType type;
    bool mounted;
};

class DiskManager : public QObject
{
    Q_OBJECT
public:
    explicit DiskManager(QObject *parent = 0);

    typedef QList<DeviceInfo*> Devices;
    typedef QMap<QString, DeviceInfo*> DeviceMap;

    Devices devices() const { return deviceCache.values(); }

signals:
    void deviceAdded(const DeviceInfo& device);
    void deviceRemoved(const DeviceInfo& device);

private slots:
    void onDeviceAdded(const QDBusObjectPath& path);
    void onDeviceRemoved(const QDBusObjectPath& path);

private:
    DeviceInfo* deviceForPath(const QDBusObjectPath& path);

private:
    UDisksInterface* udisks;
    DeviceMap deviceCache;
};

#endif // DISKMANAGER_H
