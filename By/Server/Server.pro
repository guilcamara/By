TEMPLATE = app
TARGET = SERVER

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

SOURCES += \
    main.cpp \
    server.cpp

HEADERS += \
    server.h
