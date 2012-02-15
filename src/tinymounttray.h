#ifndef TINYMOUNTTRAY_H
#define TINYMOUNTTRAY_H

#include <QObject>

class QSystemTrayIcon;
class QMenu;
class DeviceInfo;
class DiskManager;

class TinyMountTray : public QObject
{
    Q_OBJECT
public:
    explicit TinyMountTray(QObject *parent = 0);

signals:

private slots:
    void onDeviceAdded(const DeviceInfo& device);
    void onDeviceRemoved(const DeviceInfo& device);

private:
    QSystemTrayIcon* tray;
    QMenu* trayMenu;
    DiskManager* manager;
};

#endif // TINYMOUNTTRAY_H
