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
HEADERS += ./src/editPage.h \
    ./src/appservices.h \
    ./src/editWindow.h \
    ./src/globalVariables.h \
    ./src/mainWindow.h \
    ./src/bankTreeList.h \
    ./src/bulkEditDialog.h \
    ./src/bulkLoadDialog.h \
    ./src/bulkSaveDialog.h \
    ./src/customButton.h \
    ./src/customControlParaEQ.h \
    ./src/customControlKnob.h \
    ./src/customControlDataKnob.h \
    ./src/customControlRange.h \
    ./src/customControlLabel.h \
    ./src/customComboBox.h \
    ./src/customControlListMenu.h \
    ./src/customControlMidiTable.h \
    ./src/customControlSpinBox.h \
    ./src/customControlSwitch.h \
    ./src/customControlTabMenu.h \
    ./src/customControlTarget.h \
    ./src/customControlVU.h \
    ./src/customDial.h \
    ./src/customDisplay.h \
    ./src/customParaEQGraph.h \
    ./src/customKnob.h \
    ./src/customDataKnob.h \
    ./src/customKnobTarget.h \
    ./src/customKnobRange.h \
    ./src/customLabelDisplay.h \
    ./src/customLed.h \
    ./src/customLogo.h \
    ./src/customMeter.h \
    ./src/customMultiComboBox.h \
    ./src/customPanelButton.h \
    ./src/customRangeDial.h \
    ./src/customRenameDialog.h \
    ./src/customRenameWidget.h \
    ./src/customSlider.h \
    ./src/customSplashScreen.h \
    ./src/customStructure.h \
    ./src/customSwitch.h \ 
    ./src/customSwitch3way.h \ 
    ./src/customSystemOverride.h \
    ./src/customTargetDial.h \
    ./src/customTargetListMenu.h \
    ./src/customVU.h \
    ./src/dragBar.h \
    ./src/fileDialog.h \
    ./src/floorBoard.h \
    ./src/floorBoardDisplay.h \
    ./src/floorPanelBar.h \
    ./src/floorPanelBarButton.h \
    ./src/initPatchListMenu.h \
    ./src/menuPage.h \
    ./src/menuPage_assign1.h \
    ./src/menuPage_master.h \
    ./src/menuPage_pdl.h \
    ./src/menuPage_setup.h \
    ./src/menuPage_system.h \
    ./src/menuPage_tuner.h \
    ./src/notesDialog.h \
    ./src/patchWriteDialog.h \
    ./src/renameDialog.h \
    ./src/renameWidget.h \
    ./src/statusBarSymbol.h \
    ./src/statusBarWidget.h \
    ./src/soundSource_inst1.h \
    ./src/soundSource_inst2.h \
    ./src/soundSource_inst3.h \
    ./src/soundSource_normal.h \
    ./src/stompBox.h \
    ./src/stompbox_amp.h \
    ./src/stompbox_balancer1.h \
    ./src/stompbox_balancer2.h \
    ./src/stompbox_balancer3.h \
    ./src/stompbox_branch_AB.h \
    ./src/stompbox_ce.h \
    ./src/stompbox_cs.h \
    ./src/stompbox_dd1.h \
    ./src/stompbox_dd2.h \
    ./src/stompbox_dd3.h \
    ./src/stompbox_divider.h \
    ./src/stompbox_ds.h \
    ./src/stompbox_eq1.h \
    ./src/stompbox_eq2.h \
    ./src/stompbox_fx1.h \
    ./src/stompbox_fx2.h \
    ./src/stompbox_fx3.h \
    ./src/stompbox_fv1.h \
    ./src/stompbox_fv2.h \
    ./src/stompbox_lp.h \
    ./src/stompbox_main_out_left.h \
    ./src/stompbox_main_out_right.h \
    ./src/stompbox_main_out_stereo.h \
    ./src/stompbox_master.h \
    ./src/stompbox_mixer.h \
    ./src/stompbox_ns.h \   
    ./src/stompbox_rv.h \
    ./src/stompbox_split.h \
    ./src/stompbox_sub_out_left.h \
    ./src/stompbox_sub_out_right.h \
    ./src/stompbox_sub_out_stereo.h \
    ./src/midiIO.h \
    ./src/midiIODestroyer.h \
    ./src/MidiTable.h \
    ./src/MidiTableDestroyer.h \
    ./src/RtMidi.h \
    ./src/summaryDialog.h \
    ./src/summaryDialogPatchList.h \
    ./src/summaryDialogSystem.h \
    ./src/SysxIO.h \
    ./src/SysxIODestroyer.h \
    ./src/sysxWriter.h \
    ./src/tsl_SY300_translation.h \
    ./src/tsl_GP10_translation.h \
    ./src/tsl_GT1000_translation.h \
    ./src/tsl_VG99_VB99_translation.h \
    ./src/Preferences.h \
    ./src/PreferencesDestroyer.h \
    ./src/preferencesDialog.h \
    ./src/preferencesPages.h \
    ./src/xmlwriter/xmlwriter.h \
    ./src/xmlwriter/xmlwriter_p.h

#Source files
SOURCES += ./src/editPage.cpp \
    ./src/appservices.cpp \
    ./src/editWindow.cpp \
    ./src/main.cpp \
    ./src/mainWindow.cpp \
    ./src/bankTreeList.cpp \
    ./src/bulkEditDialog.cpp \
    ./src/bulkLoadDialog.cpp \
    ./src/bulkSaveDialog.cpp \
    ./src/customButton.cpp \
    ./src/customControlParaEQ.cpp \
    ./src/customControlKnob.cpp \
    ./src/customControlDataKnob.cpp \
    ./src/customControlLabel.cpp \
    ./src/customComboBox.cpp \
    ./src/customControlListMenu.cpp \
    ./src/customControlMidiTable.cpp \
    ./src/customControlRange.cpp \
    ./src/customControlSpinBox.cpp \
    ./src/customControlSwitch.cpp \
    ./src/customControlTabMenu.cpp \
    ./src/customControlTarget.cpp \
    ./src/customControlVU.cpp \
    ./src/customDial.cpp \
    ./src/customDisplay.cpp \
    ./src/customParaEQGraph.cpp \
    ./src/customKnob.cpp \
    ./src/customDataKnob.cpp \
    ./src/customKnobTarget.cpp \
    ./src/customKnobRange.cpp \
    ./src/customLabelDisplay.cpp \
    ./src/customLed.cpp \
    ./src/customLogo.cpp \
    ./src/customMeter.cpp \
    ./src/customMultiComboBox.cpp \
    ./src/customPanelButton.cpp \
    ./src/customRangeDial.cpp \
    ./src/customRenameDialog.cpp \
    ./src/customRenameWidget.cpp \
    ./src/customSlider.cpp \
    ./src/customSplashScreen.cpp \
    ./src/customStructure.cpp \
    ./src/customSwitch.cpp \
    ./src/customSwitch3way.cpp \
    ./src/customSystemOverride.cpp \
    ./src/customTargetDial.cpp \
    ./src/customTargetListMenu.cpp \
    ./src/customVU.cpp \
    ./src/dragBar.cpp \
    ./src/fileDialog.cpp \
    ./src/floorBoard.cpp \
    ./src/floorBoardDisplay.cpp \
    ./src/floorPanelBar.cpp \
    ./src/floorPanelBarButton.cpp \
    ./src/initPatchListMenu.cpp \
    ./src/menuPage.cpp \
    ./src/menuPage_assign1.cpp \
    ./src/menuPage_master.cpp \
    ./src/menuPage_pdl.cpp \
    ./src/menuPage_setup.cpp \
    ./src/menuPage_system.cpp \
    ./src/menuPage_tuner.cpp \
    ./src/patchWriteDialog.cpp \
    ./src/notesDialog.cpp \
    ./src/renameDialog.cpp \
    ./src/renameWidget.cpp \
    ./src/RtMidi.cpp \
    ./src/statusBarSymbol.cpp \
    ./src/statusBarWidget.cpp \
    ./src/soundSource_inst1.cpp \
    ./src/soundSource_inst2.cpp \
    ./src/soundSource_inst3.cpp \
    ./src/soundSource_normal.cpp \
    ./src/stompBox.cpp \
    ./src/stompbox_amp.cpp \
    ./src/stompbox_balancer1.cpp \
    ./src/stompbox_balancer2.cpp \
    ./src/stompbox_balancer3.cpp \
    ./src/stompbox_branch_AB.cpp \
    ./src/stompbox_ce.cpp \
    ./src/stompbox_cs.cpp \
    ./src/stompbox_dd1.cpp \
    ./src/stompbox_dd2.cpp \
    ./src/stompbox_dd3.cpp \
    ./src/stompbox_divider.cpp \
    ./src/stompbox_ds.cpp \
    ./src/stompbox_eq1.cpp \
    ./src/stompbox_eq2.cpp \
    ./src/stompbox_fx1.cpp \
    ./src/stompbox_fx2.cpp \
    ./src/stompbox_fx3.cpp \
    ./src/stompbox_fv1.cpp \
    ./src/stompbox_fv2.cpp \
    ./src/stompbox_lp.cpp \
    ./src/stompbox_main_out_left.cpp \
    ./src/stompbox_main_out_right.cpp \
    ./src/stompbox_main_out_stereo.cpp \
    ./src/stompbox_master.cpp \
    ./src/stompbox_mixer.cpp \
    ./src/stompbox_ns.cpp \
    ./src/stompbox_rv.cpp \
    ./src/stompbox_split.cpp \
    ./src/stompbox_sub_out_left.cpp \
    ./src/stompbox_sub_out_right.cpp \
    ./src/stompbox_sub_out_stereo.cpp \
    ./src/summaryDialog.cpp \
    ./src/summaryDialogPatchList.cpp \
    ./src/summaryDialogSystem.cpp \
    ./src/midiIO.cpp \
    ./src/midiIODestroyer.cpp \
    ./src/MidiTable.cpp \
    ./src/MidiTableDestroyer.cpp \
    ./src/Preferences.cpp \
    ./src/PreferencesDestroyer.cpp \
    ./src/preferencesDialog.cpp \
    ./src/preferencesPages.cpp \
    ./src/SysxIO.cpp \
    ./src/SysxIODestroyer.cpp \
    ./src/sysxWriter.cpp \
    ./src/tsl_SY300_translation.cpp \
    ./src/tsl_GP10_translation.cpp \
    ./src/tsl_GT1000_translation.cpp \
    ./src/tsl_VG99_VB99_translation.cpp \
    ./src/xmlwriter/xmlwriter.cpp    
    
#Resource file(s)
RESOURCES += SY-1000FloorBoard.qrc
