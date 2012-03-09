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
VERSION = 0.0.1

DEPENDPATH += .
INCLUDEPATH += .

CONFIG += debug

QT += dbus

# Input
SOURCES = \
    main.cpp \
    diskmanager.cpp \
    tinymounttray.cpp

HEADERS = \
    diskmanager.h \
    tinymounttray.h

HEADERS += \
    udisks/udisksinterface.h \
    udisks/udisksdeviceinterface.h

SOURCES += \
    udisks/udisksinterface.cpp \
    udisks/udisksdeviceinterface.cpp

RESOURCES += tinymount.qrc

TRANSLATIONS = translations/tinymount_ru.ts

updateqm.input = TRANSLATIONS
updateqm.output = ${QMAKE_FILE_BASE}.qm
updateqm.commands = lrelease -silent ${QMAKE_FILE_IN} -qm ${QMAKE_FILE_BASE}.qm
updateqm.CONFIG += no_link target_predeps
QMAKE_EXTRA_COMPILERS += updateqm

PRE_TARGETDEP=updateqm

DEFINES += TINYMOUNT_VERSION=\\\"$${VERSION}\\\"
