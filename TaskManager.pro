TEMPLATE = app
TARGET = TaskManager
QT = core gui charts
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
SOURCES += \
    main.cpp \
    systemtab.cpp \
    resourcestab.cpp

HEADERS += \
    systemtab.h \
    resourcestab.h
