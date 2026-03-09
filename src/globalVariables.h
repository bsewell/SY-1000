/****************************************************************************
**
** Copyright (C) 2007~2025 Colin Willcocks.
** Copyright (C) 2005~2007 Uco Mesdag. 
** All rights reserved.
** This file is part of "SY-1000 FloorBoard".
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

#ifndef GLOBALVARIABLES_H
#define GLOBALVARIABLES_H

/****************************************************************************
* These are general settings keep in mind that number of fx and fx setting 
* are per item setting to change them you will need to modify the following 
* files:
*     -> floorBoard.cpp
*     -> stompBox.cpp
*     -> stompbox_{fx abbrv.}.cpp
*
****************************************************************************/

#include <QWidget>
#include <QtWidgets>

/* General Parameters */
const QString deviceType = "SY-1000";
const QString idRequestString = "F07E7F0601F7";		// Indentity Request
const QString idReplyPatern = "0602416903000000000000F7";			// F07E10 part of Returned device id message must contain/match this (QRegExp or a string without spaces and all caps).
const int buttonBlinkInterval = 250;				// The interval (ms) the led on buttons blink.

/* Sysex Message Parameters */
const int sysxAddressOffset = 8;	// Offset (starts at 0) where the address information starts in a sysx message.
const int sysxDataOffset = 12;		// Offset (starts at 0) where the data starts in a sysx message.
const int checksumOffset = 8;		  // Offset (starts at 0) where we start calculating the checksum (Normally this is the address offset).
const int sysxNameOffset = 12;		// Offset (starts at 0) where the data starts in a sysx message.
const int nameLength = 16;        // length of name string

/* Patches and Banks */
const int bankTotalUser = 50;		// Number of user (editable) banks.
const int bankTotalAll = 100;		// Number of total banks.
const int patchPerBank = 4;			// Number of patches in a bank.
const int bankSize = 128;       // Number of patches in a midi bank before bank change message is required.

/* Midi Send & Receive */
const int patchReplySize = 6290;       // size of bytes in a guitar patch before trimming.
const int patchReplySizeBass = 6158;   // size of bytes in a bass patch before trimming.
const int systemReplySize = 2174;      //718; //6821;      // size of bytes in system data before trimming.
const int patchSize = 6402;            // size of bytes in a patch after trimming.
const int patchSizeBass = 6270;        // size of bytes in a patch after trimming.
const int systemSize = 6947;           // size of bytes for system data.
const int bulkPatchNamesSize = 3423; //3550;

const int nameReplySize = 30;    // size of bytes for name data.
const QString patchRequestDataSize = "00020000";
const QString systemRequest = "F04110000000691100000000000B220053F7";
const QString bulkPatchNames = "F041100000006911520000000000187F17F7";
const QString bulkPatchNames_Bass = "F041100000006911530000000000187F16F7";

//const int maxWait = 40;			 // Maximum times we loop through the receive handle before we give up waiting.(for whole patch) tempory used by midi prefs
//const int minWait = 5;			 // Minimum times we loop through the receive handle before we give up waiting.(for small data)   "      "       "       "
const int maxRetry = 4;			   // Maximum times we retry to load a patch in case of a transfer error .

/* Patch Sellection (Used when copying patches) */
const int sellectionBlinks = 5;				// Times we blink to indicate we have sellected a patch before returning.
const int sellectionBlinkInterval = 500;	// Interval (ms) the item blinks.

/* Patch addresses */
const QString nameAddress = "00";
const QString tempBulkWrite = "10";
const QString tempDataWrite = "10";

#endif // GLOBALVARIABLES_H
