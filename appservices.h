/****************************************************************************
**
** Copyright (C) 2007~2026 Colin Willcocks.
** All rights reserved.
** This file is part of "SY-1000 FloorBoard".
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
****************************************************************************/

#ifndef APPSERVICES_H
#define APPSERVICES_H

class Preferences;
class SysxIO;
class midiIO;
class MidiTable;

class AppServices
{
public:
    static AppServices& instance();

    Preferences& preferences() const;
    SysxIO& sysx() const;
    midiIO& midi() const;
    MidiTable& midiTable() const;

    double scaleRatio() const;
    double fontRatio() const;
    bool debugLoggingEnabled() const;

private:
    AppServices() = default;
};

#endif // APPSERVICES_H
