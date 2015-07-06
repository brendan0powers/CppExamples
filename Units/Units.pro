TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

#enable debug symbols for release builds
#makes it easier to ensure constexpr is
#actually working
QMAKE_CXXFLAGS_RELEASE += -g
QMAKE_CFLAGS_RELEASE += -g
QMAKE_LFLAGS_RELEASE =

SOURCES += main.cpp

HEADERS += \
    units.h


