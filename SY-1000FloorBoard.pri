#############################################################################
##
## Copyright (C) 2007~2025 Colin Willcocks. 
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
 
#Header files
HEADERS += ./editPage.h \
    ./appservices.h \
    ./editWindow.h \
    ./globalVariables.h \
    ./mainWindow.h \
    ./bankTreeList.h \
    ./bulkEditDialog.h \
    ./bulkLoadDialog.h \
    ./bulkSaveDialog.h \
    ./customButton.h \
    ./customControlParaEQ.h \
    ./customControlKnob.h \
    ./customControlDataKnob.h \
    ./customControlRange.h \
    ./customControlLabel.h \
    ./customControlListMenu.h \
    ./customControlMidiTable.h \
    ./customControlSpinBox.h \
    ./customControlSwitch.h \
    ./customControlTabMenu.h \
    ./customControlTarget.h \
    ./customControlVU.h \
    ./customDial.h \
    ./customDisplay.h \
    ./customParaEQGraph.h \
    ./customKnob.h \
    ./customDataKnob.h \
    ./customKnobTarget.h \
    ./customKnobRange.h \
    ./customLabelDisplay.h \
    ./customLed.h \
    ./customLogo.h \
    ./customMeter.h \
    ./customMultiComboBox.h \
    ./customPanelButton.h \
    ./customRangeDial.h \
    ./customRenameDialog.h \
    ./customRenameWidget.h \
    ./customSlider.h \
    ./customSplashScreen.h \
    ./customStructure.h \
    ./customSwitch.h \ 
    ./customSwitch3way.h \ 
    ./customSystemOverride.h \
    ./customTargetDial.h \
    ./customTargetListMenu.h \
    ./customVU.h \
    ./dragBar.h \
    ./fileDialog.h \
    ./floorBoard.h \
    ./floorBoardDisplay.h \
    ./floorPanelBar.h \
    ./floorPanelBarButton.h \
    ./initPatchListMenu.h \
    ./menuPage.h \
    ./menuPage_assign1.h \
    ./menuPage_master.h \
    ./menuPage_pdl.h \
    ./menuPage_setup.h \
    ./menuPage_system.h \
    ./menuPage_tuner.h \
    ./notesDialog.h \
    ./patchWriteDialog.h \
    ./renameDialog.h \
    ./renameWidget.h \
    ./statusBarSymbol.h \
    ./statusBarWidget.h \
    ./soundSource_inst1.h \
    ./soundSource_inst2.h \
    ./soundSource_inst3.h \
    ./soundSource_normal.h \
    ./stompBox.h \
    ./stompbox_amp.h \
    ./stompbox_balancer1.h \
    ./stompbox_balancer2.h \
    ./stompbox_balancer3.h \
    ./stompbox_branch_AB.h \
    ./stompbox_ce.h \
    ./stompbox_cs.h \
    ./stompbox_dd1.h \
    ./stompbox_dd2.h \
    ./stompbox_dd3.h \
    ./stompbox_divider.h \
    ./stompbox_ds.h \
    ./stompbox_eq1.h \
    ./stompbox_eq2.h \
    ./stompbox_fx1.h \
    ./stompbox_fx2.h \
    ./stompbox_fx3.h \
    ./stompbox_fv1.h \
    ./stompbox_fv2.h \
    ./stompbox_lp.h \
    ./stompbox_main_out_left.h \
    ./stompbox_main_out_right.h \
    ./stompbox_main_out_stereo.h \
    ./stompbox_master.h \
    ./stompbox_mixer.h \
    ./stompbox_ns.h \   
    ./stompbox_rv.h \
    ./stompbox_split.h \
    ./stompbox_sub_out_left.h \
    ./stompbox_sub_out_right.h \
    ./stompbox_sub_out_stereo.h \
    ./midiIO.h \
    ./midiIODestroyer.h \
    ./MidiTable.h \
    ./MidiTableDestroyer.h \
    ./RtMidi.h \
    ./summaryDialog.h \
    ./summaryDialogPatchList.h \
    ./summaryDialogSystem.h \
    ./SysxIO.h \
    ./SysxIODestroyer.h \
    ./sysxWriter.h \
    ./tsl_SY300_translation.h \
    ./tsl_GP10_translation.h \
    ./tsl_GT1000_translation.h \
    ./tsl_VG99_VB99_translation.h \
    ./Preferences.h \
    ./PreferencesDestroyer.h \
    ./preferencesDialog.h \
    ./preferencesPages.h \
    ./xmlwriter/xmlwriter.h \
    ./xmlwriter/xmlwriter_p.h

#Source files
SOURCES += ./editPage.cpp \
    ./appservices.cpp \
    ./editWindow.cpp \
    ./main.cpp \
    ./mainWindow.cpp \
    ./bankTreeList.cpp \
    ./bulkEditDialog.cpp \
    ./bulkLoadDialog.cpp \
    ./bulkSaveDialog.cpp \
    ./customButton.cpp \
    ./customControlParaEQ.cpp \
    ./customControlKnob.cpp \
    ./customControlDataKnob.cpp \
    ./customControlLabel.cpp \
    ./customControlListMenu.cpp \
    ./customControlMidiTable.cpp \
    ./customControlRange.cpp \
    ./customControlSpinBox.cpp \
    ./customControlSwitch.cpp \
    ./customControlTabMenu.cpp \
    ./customControlTarget.cpp \
    ./customControlVU.cpp \
    ./customDial.cpp \
    ./customDisplay.cpp \
    ./customParaEQGraph.cpp \
    ./customKnob.cpp \
    ./customDataKnob.cpp \
    ./customKnobTarget.cpp \
    ./customKnobRange.cpp \
    ./customLabelDisplay.cpp \
    ./customLed.cpp \
    ./customLogo.cpp \
    ./customMeter.cpp \
    ./customMultiComboBox.cpp \
    ./customPanelButton.cpp \
    ./customRangeDial.cpp \
    ./customRenameDialog.cpp \
    ./customRenameWidget.cpp \
    ./customSlider.cpp \
    ./customSplashScreen.cpp \
    ./customStructure.cpp \
    ./customSwitch.cpp \
    ./customSwitch3way.cpp \
    ./customSystemOverride.cpp \
    ./customTargetDial.cpp \
    ./customTargetListMenu.cpp \
    ./customVU.cpp \
    ./dragBar.cpp \
    ./fileDialog.cpp \
    ./floorBoard.cpp \
    ./floorBoardDisplay.cpp \
    ./floorPanelBar.cpp \
    ./floorPanelBarButton.cpp \
    ./initPatchListMenu.cpp \
    ./menuPage.cpp \
    ./menuPage_assign1.cpp \
    ./menuPage_master.cpp \
    ./menuPage_pdl.cpp \
    ./menuPage_setup.cpp \
    ./menuPage_system.cpp \
    ./menuPage_tuner.cpp \
    ./patchWriteDialog.cpp \
    ./notesDialog.cpp \
    ./renameDialog.cpp \
    ./renameWidget.cpp \
    ./RtMidi.cpp \
    ./statusBarSymbol.cpp \
    ./statusBarWidget.cpp \
    ./soundSource_inst1.cpp \
    ./soundSource_inst2.cpp \
    ./soundSource_inst3.cpp \
    ./soundSource_normal.cpp \
    ./stompBox.cpp \
    ./stompbox_amp.cpp \
    ./stompbox_balancer1.cpp \
    ./stompbox_balancer2.cpp \
    ./stompbox_balancer3.cpp \
    ./stompbox_branch_AB.cpp \
    ./stompbox_ce.cpp \
    ./stompbox_cs.cpp \
    ./stompbox_dd1.cpp \
    ./stompbox_dd2.cpp \
    ./stompbox_dd3.cpp \
    ./stompbox_divider.cpp \
    ./stompbox_ds.cpp \
    ./stompbox_eq1.cpp \
    ./stompbox_eq2.cpp \
    ./stompbox_fx1.cpp \
    ./stompbox_fx2.cpp \
    ./stompbox_fx3.cpp \
    ./stompbox_fv1.cpp \
    ./stompbox_fv2.cpp \
    ./stompbox_lp.cpp \
    ./stompbox_main_out_left.cpp \
    ./stompbox_main_out_right.cpp \
    ./stompbox_main_out_stereo.cpp \
    ./stompbox_master.cpp \
    ./stompbox_mixer.cpp \
    ./stompbox_ns.cpp \
    ./stompbox_rv.cpp \
    ./stompbox_split.cpp \
    ./stompbox_sub_out_left.cpp \
    ./stompbox_sub_out_right.cpp \
    ./stompbox_sub_out_stereo.cpp \
    ./summaryDialog.cpp \
    ./summaryDialogPatchList.cpp \
    ./summaryDialogSystem.cpp \
    ./midiIO.cpp \
    ./midiIODestroyer.cpp \
    ./MidiTable.cpp \
    ./MidiTableDestroyer.cpp \
    ./Preferences.cpp \
    ./PreferencesDestroyer.cpp \
    ./preferencesDialog.cpp \
    ./preferencesPages.cpp \
    ./SysxIO.cpp \
    ./SysxIODestroyer.cpp \
    ./sysxWriter.cpp \
    ./tsl_SY300_translation.cpp \
    ./tsl_GP10_translation.cpp \
    ./tsl_GT1000_translation.cpp \
    ./tsl_VG99_VB99_translation.cpp \
    ./xmlwriter/xmlwriter.cpp    
    
#Resource file(s)
RESOURCES += SY-1000FloorBoard.qrc
