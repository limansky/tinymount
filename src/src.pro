######################################################################
# This file is a part of tinymount
######################################################################

TEMPLATE = app
TARGET = tinymount
DEPENDPATH += .
INCLUDEPATH += .

QT += dbus

# Input
SOURCES += main.cpp \
    diskmanager.cpp \
    tinymounttray.cpp

HEADERS += udisks/udisksinterface.h \
    diskmanager.h \
    tinymounttray.h
SOURCES += udisks/udisksinterface.cpp




