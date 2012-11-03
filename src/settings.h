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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>

struct Settings
{
    bool showSystemDisks;
    bool hideIcon;
    bool deviceNotifications;
    bool mountNotifications;
    bool mountAutomaticaly;
    bool detachRemovable;
#ifdef WITH_LIBNOTIFY
    bool useLibnotify;
#endif
    QString itemFormat;
};

class SettingsManager
{
public:
    static SettingsManager& instance();

    static QString defaultItemFormat();

    const Settings& getSettings() const { return settings; }
    void save(const Settings& newSettings);

private:
    SettingsManager();
    SettingsManager(const SettingsManager&);

    void readSettings();

private:
    Settings settings;
};

#endif // SETTINGS_H
