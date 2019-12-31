@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by DRAFTCABLE.HPJ. >"hlp\DraftCable.hm"
echo. >>"hlp\DraftCable.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\DraftCable.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\DraftCable.hm"
echo. >>"hlp\DraftCable.hm"
echo // Prompts (IDP_*) >>"hlp\DraftCable.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\DraftCable.hm"
echo. >>"hlp\DraftCable.hm"
echo // Resources (IDR_*) >>"hlp\DraftCable.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\DraftCable.hm"
echo. >>"hlp\DraftCable.hm"
echo // Dialogs (IDD_*) >>"hlp\DraftCable.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\DraftCable.hm"
echo. >>"hlp\DraftCable.hm"
echo // Frame Controls (IDW_*) >>"hlp\DraftCable.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\DraftCable.hm"
REM -- Make help for Project DRAFTCABLE

rem 12/06/2005
rem From this point all lines for genrating old .hpj have been removed
rem (...)
