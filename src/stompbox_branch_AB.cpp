/****************************************************************************
**
** Copyright (C) 2020 Colin Willcocks.
** All rights reserved.
** This file is part of "VG-99 FloorBoard".
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License along
** with this program; if not, write to the Free Software Foundation, Inc.,
** 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA. 
**
****************************************************************************/

#include "stompbox_branch_AB.h"
#include "Preferences.h"
#include "SysxIO.h"

stompbox_branch_AB::stompbox_branch_AB(QWidget *parent)
    : stompBox(parent)
{
    SysxIO *sysxIO = SysxIO::Instance();
    sysxIO->deBug("build branch_AB");

    /* branch_AB */
    setOnImage(":/images/path_thru.png");
    setOffImage(":/images/path_thru.png");
    setEditPages();
}

void stompbox_branch_AB::updateSignal()
{

}

void stompbox_branch_AB::setEditPages()
{


}
