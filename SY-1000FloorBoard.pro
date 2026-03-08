#############################################################################
##
## Copyright (C) 2007~2025 Colin Willcocks.
## Copyright (C) 2005~2007 Uco Mesdag. 
## All rights reserved.
##
## This file is part of "SY-1000 FloorBoard".
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License along
## with this program; if not, write to the Free Software Foundation, Inc.,
## 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA. 
##
#############################################################################

TEMPLATE = app
CONFIG += c++17
#CONFIG += release
TARGET = "SY-1000FloorBoard"
VERSION = 2026.03.05
DESTDIR = ./packager
OBJECTS_DIR += release
UI_DIR += ./generatedfiles
MOC_DIR += ./generatedfiles/release
INCLUDEPATH += ./generatedfiles \
./generatedfiles/release \
.

# Require Qt 6
versionAtLeast(QT_VERSION, 6.0.0)|error("Qt 6.0 or later is required.")

TRANSLATIONS = language_fr.ts \
               language_ge.ts \
               language_ch.ts \
               language_es.ts \
               language_jp.ts \
               language_pt.ts \
               language_pl.ts

DEPENDPATH += .
QT += xml widgets printsupport

#Platform dependent file(s)
win32-g++:contains(QMAKE_HOST.arch, x86):{
       message("x86 build")
       ## Windows x86 (32bit) specific build here
       exists("C:\Qt\SY-1000 - Qt5-15\windows\WinMM.Lib")
               {	# <-- Change the path to WinMM.Lib here!
                   LIBS += C:\Qt\SY-1000 - Qt5-15\windows\WinMM.Lib	# <-- Change the path here also!
               }
       message(Including Windows 32 specific headers and sources...)
}
win32:contains(QMAKE_HOST.arch, x86_64):{
       message("x86_64 build")
       ## Windows x86_64 (64bit) specific build here
       exists("c:\Qt\SY-1000\win64\WinMM.Lib")
               {	# <-- Change the path to WinMM.Lib here!
                   LIBS += c:\Qt\SY-1000\win64\WinMM.Lib	# <-- Change the path here also!
               }
       message(Including Windows 64 specific headers and sources...)
}
linux-g++{
        LIBS += -lasound
	message("ALSA LIBRARIES SHOULD BE INSTALLED or ERROR will Occur") 
	message("Please install the ALSA Audio System packages if not present") 	
	message(Including Linux specific headers and sources...)
}
linux-g++-64{
        LIBS += -lasound
	message("ALSA LIBRARIES SHOULD BE INSTALLED or ERROR will Occur") 
	message("Please install the ALSA Audio System packages if not present") 	
	message(Including Linux specific headers and sources...)
}
macx{
        QMAKE_APPLE_DEVICE_ARCHS="arm64"
	LIBS += -framework CoreMidi -framework CoreAudio -framework CoreFoundation
	message("X-Code LIBRARIES SHOULD BE INSTALLED or ERROR will Occur") 
	message("Please install the X-Code Audio System packages if not present") 
	ICON = SY-1000FloorBoard.icns
        QMAKE_INFO_PLIST = MyInfo.plist
	message(Including Mac OS X specific headers and sources...)
}
ios{
        LIBS += -framework CoreMidi -framework CoreAudio -framework CoreFoundation
        message("X-Code LIBRARIES SHOULD BE INSTALLED or ERROR will Occur")
        message("Please install the X-Code Audio System packages if not present")
        ICON = SY-1000FloorBoard.icns
        message(Including Mac OS X specific headers and sources...)
}

#Include file(s)
include(SY-1000FloorBoard.pri)

#Windows resource file
win32:RC_FILE = SY-1000FloorBoard.rc

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

DISTFILES += \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/AndroidManifest.xml \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat

FORMS += \
    consoletoolbar.ui

HEADERS += \
    consoletoolbar.h

SOURCES += \
    consoletoolbar.cpp


