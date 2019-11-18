TEMPLATE = app
TARGET = TaskManager
QT = core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
SOURCES += \
    main.cpp \
    systemtab.cpp

HEADERS += \
    systemtab.h
