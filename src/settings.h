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

class Settings
{
public:
    static Settings& instance();

    bool showSystemDisks() const { return mShowSystemDisks; }
    void setShowSystemDisks(bool enabled) { mShowSystemDisks = enabled; }

    bool deviceNotifications() const { return mDeviceNotifications; }
    void setDeviceNotifications(bool enabled) { mDeviceNotifications = enabled; }

    bool mountNotifications() const { return mMountNotifications; }
    void setMountNotifications(bool enabled) { mMountNotifications = enabled; }

    void save();
private:
    Settings();
    Settings(const Settings&);

    void readSettings();

private:
    bool mShowSystemDisks;
    bool mDeviceNotifications;
    bool mMountNotifications;
};

#endif // SETTINGS_H
