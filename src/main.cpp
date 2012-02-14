#include <QApplication>
#include <QSystemTrayIcon>

#include "tinymounttray.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    if (!QSystemTrayIcon::isSystemTrayAvailable())
    {
        qFatal("No system tray available");
    }

    TinyMountTray tmt;

    return app.exec();
}
