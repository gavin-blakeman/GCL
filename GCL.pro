#**********************************************************************************************************************************
#
# PROJECT:            General Class Library (GCL)
# FILE:               GCL Project File
# SUBSYSTEM:					Project File
# LANGUAGE:						C++
# TARGET OS:					All (Standard C++)
# LIBRARY DEPENDANCE:	None.
# NAMESPACE:					GCL
# AUTHOR:							Gavin Blakeman.
# LICENSE:            GPLv2
#
#                     Copyright 2013-2019 Gavin Blakeman.
#                     This file is part of the General Class Library (GCL)
#
#                     GCL is free software: you can redistribute it and/or modify it under the terms of the GNU General
#                     Public License as published by the Free Software Foundation, either version 2 of the License, or
#                     (at your option) any later version.
#
#                     GCL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the
#                     implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
#                     for more details.
#
#                     You should have received a copy of the GNU General Public License along with GCL.  If not,
#                     see <http://www.gnu.org/licenses/>.
#
# OVERVIEW:           The .pro file for the GCL library.
#
# CLASSES INCLUDED:   None
#
# HISTORY:            2013-06-16 GGB - File created.
//
#**********************************************************************************************************************************

TARGET = GCL
TEMPLATE = lib
CONFIG += staticlib

QT -= core gui

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
  "../MCL" \
  "../SCL" \
  "/home/gavin/Documents/Projects/software/Library/Boost/boost_1_71_0"

SOURCES += \
    source/string.cpp \
    source/logger/LoggerCore.cpp \
    source/logger/FileSink.cpp \
    source/logger/StreamSink.cpp \
    source/Resource.cpp \
    source/Functions.cpp \
    source/common.cpp \
    source/SQLWriter.cpp \
    source/dateTime.cpp \
    source/filesystem.cpp \
    source/alarm/alarmCore.cpp \
    source/alarm/alarmDaily.cpp \
    source/alarm/alarmWeekly.cpp \
    source/error.cpp

HEADERS += \
    GCL \
    include/logger/LoggerCore.h \
    include/logger/FileSink.h \
    include/logger/StreamSink.h \
    include/Resource.h \
    include/Functions.h \
    include/common.h \
    include/SQLWriter.h \
    include/dateTime.h \
    include/config.h \
    include/filesystem.h \
    include/alarm/alarmCore.h \
    include/alarm/alarmDaily.h \
    include/alarm/alarmWeekly.h \
    include/error.h

OTHER_FILES += \
    changelog.txt

