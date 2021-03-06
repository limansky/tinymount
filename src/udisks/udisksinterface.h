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

/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -c UDisksInterface org.freedesktop.UDisks.xml -p udisksinterface.h:udisksinterface.cpp
 *
 * qdbusxml2cpp is Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef UDISKSINTERFACE_H_1329217546
#define UDISKSINTERFACE_H_1329217546

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

/*
 * Proxy class for interface org.freedesktop.UDisks
 */
class UDisksInterface: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "org.freedesktop.UDisks"; }

public:
    UDisksInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~UDisksInterface();

    Q_PROPERTY(bool DaemonIsInhibited READ daemonIsInhibited)
    inline bool daemonIsInhibited() const
    { return qvariant_cast< bool >(property("DaemonIsInhibited")); }

    Q_PROPERTY(QString DaemonVersion READ daemonVersion)
    inline QString daemonVersion() const
    { return qvariant_cast< QString >(property("DaemonVersion")); }

    Q_PROPERTY(bool SupportsLuksDevices READ supportsLuksDevices)
    inline bool supportsLuksDevices() const
    { return qvariant_cast< bool >(property("SupportsLuksDevices")); }

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<QString> DriveInhibitAllPolling(const QStringList &options)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(options);
        return asyncCallWithArgumentList(QLatin1String("DriveInhibitAllPolling"), argumentList);
    }

    inline QDBusPendingReply<QString> DriveSetAllSpindownTimeouts(int timeout_seconds, const QStringList &options)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(timeout_seconds) << qVariantFromValue(options);
        return asyncCallWithArgumentList(QLatin1String("DriveSetAllSpindownTimeouts"), argumentList);
    }

    inline QDBusPendingReply<> DriveUninhibitAllPolling(const QString &cookie)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(cookie);
        return asyncCallWithArgumentList(QLatin1String("DriveUninhibitAllPolling"), argumentList);
    }

    inline QDBusPendingReply<> DriveUnsetAllSpindownTimeouts(const QString &cookie)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(cookie);
        return asyncCallWithArgumentList(QLatin1String("DriveUnsetAllSpindownTimeouts"), argumentList);
    }

    inline QDBusPendingReply<QList<QDBusObjectPath> > EnumerateAdapters()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("EnumerateAdapters"), argumentList);
    }

    inline QDBusPendingReply<QStringList> EnumerateDeviceFiles()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("EnumerateDeviceFiles"), argumentList);
    }

    inline QDBusPendingReply<QList<QDBusObjectPath> > EnumerateDevices()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("EnumerateDevices"), argumentList);
    }

    inline QDBusPendingReply<QList<QDBusObjectPath> > EnumerateExpanders()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("EnumerateExpanders"), argumentList);
    }

    inline QDBusPendingReply<QList<QDBusObjectPath> > EnumeratePorts()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("EnumeratePorts"), argumentList);
    }

    inline QDBusPendingReply<QDBusObjectPath> FindDeviceByDeviceFile(const QString &device_file)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(device_file);
        return asyncCallWithArgumentList(QLatin1String("FindDeviceByDeviceFile"), argumentList);
    }

    inline QDBusPendingReply<QDBusObjectPath> FindDeviceByMajorMinor(qlonglong device_major, qlonglong device_minor)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(device_major) << qVariantFromValue(device_minor);
        return asyncCallWithArgumentList(QLatin1String("FindDeviceByMajorMinor"), argumentList);
    }

    inline QDBusPendingReply<QString> Inhibit()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("Inhibit"), argumentList);
    }

    inline QDBusPendingReply<QDBusObjectPath> LinuxLvm2LVCreate(const QString &group_uuid, const QString &name, qulonglong size, uint num_stripes, qulonglong stripe_size, uint num_mirrors, const QStringList &options, const QString &fstype, const QStringList &fsoptions)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(group_uuid) << qVariantFromValue(name) << qVariantFromValue(size) << qVariantFromValue(num_stripes) << qVariantFromValue(stripe_size) << qVariantFromValue(num_mirrors) << qVariantFromValue(options) << qVariantFromValue(fstype) << qVariantFromValue(fsoptions);
        return asyncCallWithArgumentList(QLatin1String("LinuxLvm2LVCreate"), argumentList);
    }

    inline QDBusPendingReply<> LinuxLvm2LVRemove(const QString &group_uuid, const QString &uuid, const QStringList &options)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(group_uuid) << qVariantFromValue(uuid) << qVariantFromValue(options);
        return asyncCallWithArgumentList(QLatin1String("LinuxLvm2LVRemove"), argumentList);
    }

    inline QDBusPendingReply<> LinuxLvm2LVSetName(const QString &group_uuid, const QString &uuid, const QString &name)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(group_uuid) << qVariantFromValue(uuid) << qVariantFromValue(name);
        return asyncCallWithArgumentList(QLatin1String("LinuxLvm2LVSetName"), argumentList);
    }

    inline QDBusPendingReply<> LinuxLvm2LVStart(const QString &group_uuid, const QString &uuid, const QStringList &options)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(group_uuid) << qVariantFromValue(uuid) << qVariantFromValue(options);
        return asyncCallWithArgumentList(QLatin1String("LinuxLvm2LVStart"), argumentList);
    }

    inline QDBusPendingReply<> LinuxLvm2VGAddPV(const QString &uuid, const QDBusObjectPath &physical_volume, const QStringList &options)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(uuid) << qVariantFromValue(physical_volume) << qVariantFromValue(options);
        return asyncCallWithArgumentList(QLatin1String("LinuxLvm2VGAddPV"), argumentList);
    }

    inline QDBusPendingReply<> LinuxLvm2VGRemovePV(const QString &vg_uuid, const QString &pv_uuid, const QStringList &options)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(vg_uuid) << qVariantFromValue(pv_uuid) << qVariantFromValue(options);
        return asyncCallWithArgumentList(QLatin1String("LinuxLvm2VGRemovePV"), argumentList);
    }

    inline QDBusPendingReply<> LinuxLvm2VGSetName(const QString &uuid, const QString &name)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(uuid) << qVariantFromValue(name);
        return asyncCallWithArgumentList(QLatin1String("LinuxLvm2VGSetName"), argumentList);
    }

    inline QDBusPendingReply<> LinuxLvm2VGStart(const QString &uuid, const QStringList &options)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(uuid) << qVariantFromValue(options);
        return asyncCallWithArgumentList(QLatin1String("LinuxLvm2VGStart"), argumentList);
    }

    inline QDBusPendingReply<> LinuxLvm2VGStop(const QString &uuid, const QStringList &options)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(uuid) << qVariantFromValue(options);
        return asyncCallWithArgumentList(QLatin1String("LinuxLvm2VGStop"), argumentList);
    }

    inline QDBusPendingReply<QDBusObjectPath> LinuxMdCreate(const QList<QDBusObjectPath> &components, const QString &level, qulonglong stripe_size, const QString &name, const QStringList &options)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(components) << qVariantFromValue(level) << qVariantFromValue(stripe_size) << qVariantFromValue(name) << qVariantFromValue(options);
        return asyncCallWithArgumentList(QLatin1String("LinuxMdCreate"), argumentList);
    }

    inline QDBusPendingReply<QDBusObjectPath> LinuxMdStart(const QList<QDBusObjectPath> &components, const QStringList &options)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(components) << qVariantFromValue(options);
        return asyncCallWithArgumentList(QLatin1String("LinuxMdStart"), argumentList);
    }

    inline QDBusPendingReply<> Uninhibit(const QString &cookie)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(cookie);
        return asyncCallWithArgumentList(QLatin1String("Uninhibit"), argumentList);
    }

Q_SIGNALS: // SIGNALS
    void AdapterAdded(const QDBusObjectPath &adapter);
    void AdapterChanged(const QDBusObjectPath &adapter);
    void AdapterRemoved(const QDBusObjectPath &adapter);
    void DeviceAdded(const QDBusObjectPath &device);
    void DeviceChanged(const QDBusObjectPath &device);
    void DeviceJobChanged(const QDBusObjectPath &device, bool job_in_progress, bool job_is_cancellable, const QString &job_id, int job_num_tasks, int job_cur_task, const QString &job_cur_task_id, double job_cur_task_percentage);
    void DeviceRemoved(const QDBusObjectPath &device);
    void ExpanderAdded(const QDBusObjectPath &expander);
    void ExpanderChanged(const QDBusObjectPath &expander);
    void ExpanderRemoved(const QDBusObjectPath &expander);
    void PortAdded(const QDBusObjectPath &port);
    void PortChanged(const QDBusObjectPath &port);
    void PortRemoved(const QDBusObjectPath &port);
};

namespace org {
  namespace freedesktop {
    typedef ::UDisksInterface UDisksI;
  }
}
#endif
