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

#include "appservices.h"
#include "Preferences.h"
#include "SysxIO.h"
#include "midiIO.h"
#include "MidiTable.h"

AppServices& AppServices::instance()
{
    static AppServices services;
    return services;
}

Preferences& AppServices::preferences() const
{
    return *Preferences::Instance();
}

SysxIO& AppServices::sysx() const
{
    return *SysxIO::Instance();
}

midiIO& AppServices::midi() const
{
    return *midiIO::Instance();
}

MidiTable& AppServices::midiTable() const
{
    return *MidiTable::Instance();
}

double AppServices::scaleRatio() const
{
    bool ok = false;
    const double ratio = preferences().getPreferences("Window", "Scale", "ratio").toDouble(&ok);
    return ok && ratio > 0.0 ? ratio : 1.0;
}

double AppServices::fontRatio() const
{
    bool ok = false;
    const double ratio = preferences().getPreferences("Window", "Font", "ratio").toDouble(&ok);
    return ok && ratio > 0.0 ? ratio : 1.0;
}

bool AppServices::debugLoggingEnabled() const
{
    return preferences().getPreferences("Midi", "DBug", "bool") == "true";
}
