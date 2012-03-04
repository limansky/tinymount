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

#include <QApplication>
#include <QSystemTrayIcon>
#include <QFile>
#include <QDebug>

#include "tinymounttray.h"

struct Option {
    typedef void (*Handler)(const QString&);
    Option(const QString& paramName, bool withValue, Handler paramHandler = 0, const QString& description = "")
        : name(paramName)
        , hasValue(withValue)
        , handler(paramHandler)
        ,desc(description)
    {}

    QString name;
    bool hasValue;
    Handler handler;
    QString desc;
};

void processArgs(const QList<Option>& options, const QString& usageOption="", const QString& usagePrefix = "")
{
    if (!usageOption.isEmpty())
    {
        foreach (const QString a, QApplication::arguments())
        {
            if (a == usageOption)
            {
                QTextStream cout(stdout);

                if (!usagePrefix.isEmpty())
                {
                    cout << usagePrefix << "\n";
                }

                cout << qApp->translate("Arguments", "Parameters:\n");
                foreach (const Option o, options)
                {
                    cout << "\t" << o.name << "\t" << o.desc << "\n";
                }

                cout.flush();

                ::exit(0);
            }
        }
    }

    foreach (const Option& o, options)
    {
        QString patternString = "^--" + o.name;

        if (o.hasValue) patternString += "=(\\S+)";

        QRegExp pattern(patternString);

        foreach (const QString& a, QApplication::arguments())
        {

            if (pattern.exactMatch(a))
            {
                qDebug() << "Found param:" << a << "processing as" << o.desc;
                if (0 != o.handler)
                {
                    o.handler(pattern.cap(1));
                }
            }
        }
    }
}

static const QList<Option> SUPPORTED_OPTIONS = QList<Option>()
        << Option(("iconTheme"), true, QIcon::setThemeName, QT_TR_NOOP("Set icon theme to be used"));


int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    if (!QSystemTrayIcon::isSystemTrayAvailable())
    {
        qFatal("No system tray available");
    }

    processArgs(SUPPORTED_OPTIONS, "--help", "tinymount, version 0.0.1");

    QApplication::setWindowIcon(QIcon(":/icons/tinymount.png"));
    TinyMountTray tmt;

    return app.exec();
}
