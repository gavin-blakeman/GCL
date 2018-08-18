#-------------------------------------------------
#
# Project created by QtCreator 2013-06-16T08:35:55
#
#-------------------------------------------------

TARGET = GCL
TEMPLATE = lib
CONFIG += staticlib

#QT -= core gui

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
  DESTDIR = ""
  OBJECTS_DIR = "objects"
  }
  else {
  DESTDIR = ""
  OBJECTS_DIR = "objects"
  }
}

INCLUDEPATH += \
  "../ACL" \
  "../boost 1.62" \
  "../MCL" \
  "../SCL"

SOURCES += \
    source/string.cpp \
    source/logger/LoggerCore.cpp \
    source/logger/FileSink.cpp \
    source/logger/StreamSink.cpp \
    source/Resource.cpp \
    source/Functions.cpp \
    source/common.cpp \
    source/Error.cpp \
    source/SQLWriter.cpp \
    source/dateTime.cpp \
    source/filesystem.cpp \
    source/alarm/alarmCore.cpp \
    source/alarm/alarmDaily.cpp \
    source/alarm/alarmWeekly.cpp

HEADERS += \
    GCL \
    include/logger/LoggerCore.h \
    include/logger/FileSink.h \
    include/logger/StreamSink.h \
    include/Resource.h \
    include/Functions.h \
    include/common.h \
    include/Error.h \
    include/SQLWriter.h \
    include/dateTime.h \
    include/config.h \
    include/filesystem.h \
    include/alarm/alarmCore.h \
    include/alarm/alarmDaily.h \
    include/alarm/alarmWeekly.h

OTHER_FILES += \
    changelog.txt

