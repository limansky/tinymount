#
# TinyMount -- simple disks mounter
# Copyright (C) 2012 Mike Limansky
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#

TEMPLATE = app
TARGET = tinymount
VERSION = 0.2.6

DEPENDPATH += .
INCLUDEPATH += .

QMAKE_CXXFLAGS += -Wall -Werror

CONFIG += debug_and_release

isEmpty(PREFIX) {
    PREFIX = /usr/local
}

BINDIR = $$PREFIX/bin
DATADIR = $$PREFIX/share/tinymount

QT += dbus

# Input
SOURCES = \
    main.cpp \
    diskmanager.cpp \
    tinymounttray.cpp \
    settings.cpp \
    settingsdialog.cpp

HEADERS = \
    diskmanager.h \
    tinymounttray.h \
    settings.h \
    settingsdialog.h

HEADERS += \
    udisks/udisksinterface.h \
    udisks/udisksdeviceinterface.h

SOURCES += \
    udisks/udisksinterface.cpp \
    udisks/udisksdeviceinterface.cpp

RESOURCES += tinymount.qrc

TRANSLATIONS = translations/tinymount_ru.ts

with_libnotify {
    DEFINES += WITH_LIBNOTIFY
    CONFIG += link_pkgconfig
    !system(pkg-config --exists libnotify) {
        error(libnotify is not found)
    }
    PKGCONFIG = libnotify
    HEADERS += libnotifier.h
    SOURCES += libnotifier.cpp
}

updateqm.input = TRANSLATIONS
updateqm.output = ${QMAKE_FILE_BASE}.qm
updateqm.commands = lrelease -silent ${QMAKE_FILE_IN} -qm ${QMAKE_FILE_BASE}.qm
updateqm.CONFIG += no_link target_predeps
updateqm.variable_out = translations.files
QMAKE_EXTRA_COMPILERS += updateqm

translations.path = $$DATADIR
translations.CONFIG += no_check_exist

target.path = $$BINDIR

INSTALLS = target translations

DEFINES += TINYMOUNT_VERSION=\\\"$${VERSION}\\\" DATADIR=\\\"$${DATADIR}\\\"

FORMS += \
    settingsdialog.ui





