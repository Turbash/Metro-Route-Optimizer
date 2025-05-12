QT += core gui widgets
TARGET = MetroRoute
TEMPLATE = app
CONFIG += c++11

SOURCES += \
    main.cpp \
    MetroData.cpp \
    RouteCalculator.cpp \
    MetroMapView.cpp \
    MetroPlannerWindow.cpp \
    Visualization.cpp

HEADERS += \
    MetroData.h \
    RouteCalculator.h \
    MetroMapView.h \
    MetroPlannerWindow.h \
    Visualization.h
