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

namespace {
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

    typedef QList<Option> Options;

    void showUsage(const Options& options, const QString& usageOption, const QString& usagePrefix)
    {
        QTextStream cout(stdout);

        if (!usagePrefix.isEmpty())
        {
            cout << usagePrefix << "\n";
        }

        cout << qApp->translate("Arguments", "Parameters:\n");
        QStringList args;
        int arglen = 0;

        foreach (const Option& o, options)
        {
            QString arg = "--" + o.name;
            if (o.hasValue) arg += "=VALUE";
            if (arg.length() > arglen) arglen = arg.length();
            args << arg;
        }

        if (usageOption.length() + 2 > arglen) arglen = usageOption.length() + 2;

        Q_ASSERT(args.length() == options.length());

        Options::const_iterator op = options.constBegin();
        for (QStringList::const_iterator a = args.constBegin(); a != args.constEnd(); ++a)
        {
            cout << "\t" << *a << QString(arglen - a->length(), ' ') << "\t" << (op++)->desc << "\n";
        }

        cout << "\t--" << usageOption << QString(arglen - usageOption.length(), ' ')
             << "\t" << qApp->translate("Arguments", "Show this information\n");

        cout.flush();

    }

    void processArgs(const QList<Option>& options, const QString& usageOption="", const QString& usagePrefix = "")
    {
        if (!usageOption.isEmpty())
        {
            const QString usageParameter = "--" + usageOption;
            foreach (const QString a, QApplication::arguments())
            {
                if (a == usageParameter)
                {
                    showUsage(options, usageOption, usagePrefix);
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

    processArgs(SUPPORTED_OPTIONS, "help", qApp->translate("Arguments", "TinyMount, version %1").arg(TINYMOUNT_VERSION));

    QApplication::setWindowIcon(QIcon(":/icons/tinymount.png"));
    TinyMountTray tmt;

    return app.exec();
}
