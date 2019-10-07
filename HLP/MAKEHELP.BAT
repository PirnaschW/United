@echo off
REM -- First make map file from App Studio generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by UNITED.HPJ. >hlp\united.hm
echo. >>hlp\united.hm
echo // Commands (ID_* and IDM_*) >>hlp\united.hm
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>hlp\united.hm
echo. >>hlp\united.hm
echo // Prompts (IDP_*) >>hlp\united.hm
makehm IDP_,HIDP_,0x30000 resource.h >>hlp\united.hm
echo. >>hlp\united.hm
echo // Resources (IDR_*) >>hlp\united.hm
makehm IDR_,HIDR_,0x20000 resource.h >>hlp\united.hm
echo. >>hlp\united.hm
echo // Dialogs (IDD_*) >>hlp\united.hm
makehm IDD_,HIDD_,0x20000 resource.h >>hlp\united.hm
echo. >>hlp\united.hm
echo // Frame Controls (IDW_*) >>hlp\united.hm
makehm IDW_,HIDW_,0x50000 resource.h >>hlp\united.hm
REM -- Make help for Project UNITED
call hc31 united.hpj
echo.
