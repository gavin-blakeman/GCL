#-------------------------------------------------
#
# Project created by QtCreator 2013-06-16T08:35:55
#
#-------------------------------------------------

QT -= core gui

TARGET = GCL
TEMPLATE = lib
CONFIG += staticlib

DEFINES += BOOST_CHRONO_DONT_PROVIDES_DEPRECATED_IO_SINCE_V2_0_0
QMAKE_CXXFLAGS += -std=c++17

win32 {
  CONFIG(release, debug|release) {
    DESTDIR = "../Library/win32/release"
    OBJECTS_DIR = "../Library/win32/release/object/GCL"
  }
  else {
    DESTDIR = "../Library/win32/debug"
    OBJECTS_DIR = "../Library/win32/debug/object/GCL"
  }
}

unix {
  CONFIG(release,debug|release) {
    DESTDIR = "../Library/unix/release"
    OBJECTS_DIR = "../Library/unix/release/object/GCL"
  }
  else {
    DESTDIR = "../Library/unix/debug"
    OBJECTS_DIR = "../Library/unix/debug/object/GCL"
  }
}

INCLUDEPATH += \
  "../Boost/boost 1.62" \
  "../ACL" \
  "../MCL" \
  "../SCL"

SOURCES += \
    Source/string.cpp \
    Source/logger/LoggerCore.cpp \
    Source/logger/FileSink.cpp \
    Source/logger/StreamSink.cpp \
    Source/Resource.cpp \
    Source/Functions.cpp \
    Source/common.cpp \
    Source/Error.cpp \
    Source/SQLWriter.cpp \
    Source/dateTime.cpp

HEADERS += \
    GCL \
    Include/logger/LoggerCore.h \
    Include/logger/FileSink.h \
    Include/logger/StreamSink.h \
    Include/Resource.h \
    Include/Functions.h \
    Include/common.h \
    Include/Error.h \
    Include/SQLWriter.h \
    Include/dateTime.h \
    Include/config.h

OTHER_FILES += \
    changelog.txt

