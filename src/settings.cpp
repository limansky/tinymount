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

#include "settings.h"
#include <QSettings>

static const char* SHOW_SYSTEM_DISKS = "ShowSystemDisks";
static const char* MOUNT_AUTOMATICALY = "MountAutomaticaly";

static const char* NOTIFICATIONS = "Notifications";
static const char* NTF_DEVICES = "Devices";
static const char* NTF_MOUNT = "Mount";

SettingsManager::SettingsManager()
{
    readSettings();
}

SettingsManager& SettingsManager::instance()
{
    static SettingsManager settings;
    return settings;
}

void SettingsManager::readSettings()
{
    QSettings s("tinymount", "tinymount");
    settings.showSystemDisks = s.value(SHOW_SYSTEM_DISKS, true).toBool();
    settings.mountAutomaticaly = s.value(MOUNT_AUTOMATICALY, false).toBool();

    s.beginGroup(NOTIFICATIONS);
    settings.deviceNotifications = s.value(NTF_DEVICES, true).toBool();
    settings.mountNotifications= s.value(NTF_MOUNT, true).toBool();
}

void SettingsManager::save(const Settings& newSettings)
{
    settings = newSettings;

    QSettings s("tinymount", "tinymount");
    s.setValue(SHOW_SYSTEM_DISKS, settings.showSystemDisks);
    s.setValue(MOUNT_AUTOMATICALY, settings.mountAutomaticaly);
    s.beginGroup(NOTIFICATIONS);
    s.setValue(NTF_DEVICES, settings.deviceNotifications);
    s.setValue(NTF_MOUNT, settings.mountNotifications);
}
