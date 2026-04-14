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

# Xcode 26+ SDK treats implicit-function-declaration as an error;
# Qt 6.x qyieldcpu.h triggers this on ARM — suppress until Qt ships a fix.
QMAKE_CXXFLAGS += -Wno-error=implicit-function-declaration
TARGET = "SY-1000FloorBoard"
VERSION = 2026.04.14.189
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DESTDIR = ./build/packager
OBJECTS_DIR += build/release
UI_DIR += ./build/generatedfiles
MOC_DIR += ./build/generatedfiles/release
INCLUDEPATH += ./build/generatedfiles \
./build/generatedfiles/release \
./src \
.

# Require Qt 6
versionAtLeast(QT_VERSION, 6.0.0)|error("Qt 6.0 or later is required.")

TRANSLATIONS = i18n/language_fr.ts \
               i18n/language_ge.ts \
               i18n/language_ch.ts \
               i18n/language_es.ts \
               i18n/language_jp.ts \
               i18n/language_pt.ts \
               i18n/language_pl.ts

DEPENDPATH += . ./src
QT += xml widgets printsupport quick quickwidgets network

# Auto-stamp version in preferences.xml.dist from the .pro VERSION variable.
# This ensures the embedded version always matches and forces rcc to recompile.
# Colin (MinGW fix): stamp_version.sh requires a POSIX shell; MinGW on bare
# Windows has no sh.exe on PATH so the pre-target failed with "file not found".
# Wrapped in unix{} so it only runs on macOS/Linux where the script works.
# On Windows, bump the version manually or run stamp_version.sh via Git Bash/MSYS2.
unix {
    versionstamp.commands = $$PWD/tools/stamp_version.sh $$VERSION $$PWD/preferences.xml.dist
    versionstamp.depends = $$PWD/SY-1000FloorBoard.pro
    QMAKE_EXTRA_TARGETS += versionstamp
    PRE_TARGETDEPS += versionstamp
}

#Platform dependent file(s)
win32-g++:contains(QMAKE_HOST.arch, x86):{
       message("x86 build")
       ## Windows x86 (32bit) specific build here
       # Colin (MinGW fix): MinGW ignores #pragma comment(lib,...) so dbghelp
       # must be linked explicitly here.  MSVC handles it via the pragma in main.cpp.
       LIBS += -ldbghelp
       # Colin (MinGW fix): original paths were hardcoded to C:\Qt\SY-1000 - Qt5-15\...
       # $$PWD makes the path relative to the project root so any checkout location works.
       exists("$$PWD\windows\WinMM.Lib") {
           LIBS += $$PWD\windows\WinMM.Lib
       }
       message(Including Windows 32 specific headers and sources...)
}
win32:contains(QMAKE_HOST.arch, x86_64):{
       message("x86_64 build")
       ## Windows x86_64 (64bit) specific build here
       # Colin (MinGW fix): same as above — explicit -ldbghelp for MinGW.
       LIBS += -ldbghelp
       # Colin (MinGW fix): was hardcoded to c:\Qt\SY-1000\win64\...
       exists("$$PWD\win64\WinMM.Lib") {
           LIBS += $$PWD\win64\WinMM.Lib
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
	ICON = images/SY-1000FloorBoard.icns
        QMAKE_INFO_PLIST = MyInfo.plist
	message(Including Mac OS X specific headers and sources...)
}
ios{
        LIBS += -framework CoreMidi -framework CoreAudio -framework CoreFoundation
        message("X-Code LIBRARIES SHOULD BE INSTALLED or ERROR will Occur")
        message("Please install the X-Code Audio System packages if not present")
        ICON = images/SY-1000FloorBoard.icns
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
    src/consoletoolbar.ui

HEADERS += \
    src/consoletoolbar.h \
    src/parameterBridge.h \
    src/qmlHost.h \
    src/diagnosticServer.h \
    src/patchListModel.h \
    src/chainLayout.h

SOURCES += \
    src/consoletoolbar.cpp \
    src/parameterBridge.cpp \
    src/qmlHost.cpp \
    src/diagnosticServer.cpp \
    src/patchListModel.cpp \
    src/chainLayout.cpp

RESOURCES += \
    qml/qml.qrc

