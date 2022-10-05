#**********************************************************************************************************************************
#
# PROJECT:            General Class Library (GCL)
# FILE:               GCL Project File
# SUBSYSTEM:					Project File
# LANGUAGE:						C++17
# TARGET OS:					All (Standard C++)
# LIBRARY DEPENDANCE:	None.
# NAMESPACE:					GCL
# AUTHOR:							Gavin Blakeman.
# LICENSE:            GPLv2
#
#                     Copyright 2013-2022 Gavin Blakeman.
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
DEFINES += BOOST_THREAD_USE_LIB

QMAKE_CXXFLAGS += -std=c++20

DESTDIR = ""
OBJECTS_DIR = "objects"


INCLUDEPATH += \
  "../MCL" \
  "../SCL" \

SOURCES += \
    source/dataParser/dataDelimited.cpp \
    source/pluginManager/pluginManager.cpp \
    source/string.cpp \
    source/Resource.cpp \
    source/Functions.cpp \
    source/common.cpp \
    source/SQLWriter.cpp \
    source/dateTime.cpp \
    source/filesystem.cpp \
    source/alarm/alarmCore.cpp \
    source/alarm/alarmDaily.cpp \
    source/alarm/alarmWeekly.cpp \
    source/error.cpp \
    source/logger/loggerCore.cpp \
    source/logger/fileSink.cpp \
    source/logger/streamSink.cpp

HEADERS += \
    GCL \
    include/Resource.h \
    include/Functions.h \
    include/any.hpp \
    include/bitValue.h \
    include/common.h \
    include/SQLWriter.h \
    include/dataParser/dataDelimited.h \
    include/dataParser/dataParserCore.h \
    include/dateTime.h \
    include/config.h \
    include/filesystem.h \
    include/alarm/alarmCore.h \
    include/alarm/alarmDaily.h \
    include/alarm/alarmWeekly.h \
    include/error.h \
    include/logger/loggerCore.h \
    include/logger/fileSink.h \
    include/logger/streamSink.h \
    include/configurationReader/readerSections.hpp \
    include/configurationReader/readerVanilla.hpp \
    include/configurationReader/readerCore.hpp \
    include/GCLError.h \
    include/pluginManager/pluginManager.h \
    include/traits.h

OTHER_FILES += \
    changelog.txt

DISTFILES += \
    README \
    changelog.txt \
    include/sqlWriter_typedef.inc \
    include/sqlWriter_variables.inc

