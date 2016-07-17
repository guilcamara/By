TEMPLATE = app
TARGET = CLIENT

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

SOURCES += \
    main.cpp \
    client.cpp

HEADERS += \
    client.h
