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

#include "libnotifier.h"
#include <libnotify/notify.h>
#include <QString>
#include <QDebug>

LibNotifier::LibNotifier(const QString &appname)
{
    notify_init(appname.toUtf8().constData());
}

LibNotifier::~LibNotifier()
{
    notify_uninit();
}

void LibNotifier::showNotification(const QString& title, const QString& message, const QString& icon)
{

#ifdef NOTIFY_CHECK_VERSION
#if NOTIFY_CHECK_VERSION (0, 7, 0)
    NotifyNotification* n = notify_notification_new(title.toUtf8().constData(),
                                                    message.toUtf8().constData(),
                                                    icon.toUtf8().constData());
#else
    NotifyNotification* n = notify_notification_new(title.toUtf8().constData(),
                                                    message.toUtf8().constData(),
                                                    icon.toUtf8().constData(),
                                                    NULL);
#endif
#else
    NotifyNotification* n = notify_notification_new(title.toUtf8().constData(),
                                                    message.toUtf8().constData(),
                                                    icon.toUtf8().constData(),
                                                    NULL);
#endif

    GError* err = 0;
    notify_notification_show(n, &err);

    if (err)
    {
        qCritical() << "Unable to show notification:" << err->message;
        g_error_free(err);
    }

    g_object_unref(n);
}
