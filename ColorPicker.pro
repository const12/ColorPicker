#-------------------------------------------------
#
# Project created by QtCreator 2020-05-21T16:20:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ColorPicker
TEMPLATE = app
RC_ICONS = $$PWD/resource/MyIcon.ico

SOURCES += main.cpp \
    colorpicker.cpp \
    toollabel.cpp

HEADERS  += \
    colorpicker.h \
    toollabel.h

FORMS    += \
    colorpicker.ui

RESOURCES += \
    resource.qrc
