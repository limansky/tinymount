#ifndef TINYMOUNTTRAY_H
#define TINYMOUNTTRAY_H

#include <QObject>

class QSystemTrayIcon;
class QMenu;
class UDisksInterface;
class QDBusObjectPath;

class TinyMountTray : public QObject
{
    Q_OBJECT
public:
    explicit TinyMountTray(QObject *parent = 0);

signals:

public slots:

private slots:
    void onDeviceAdded(const QDBusObjectPath& path);
    void onDeviceRemoved(const QDBusObjectPath& path);

private:
    QSystemTrayIcon* tray;
    QMenu* trayMenu;
    UDisksInterface* udisks;
};

#endif // TINYMOUNTTRAY_H
