@echo off
REM -- Ensure MSVC environment variables are set
call c:\msvc\bin\msvcvars.bat
REM
REM -- First make map file from App Studio generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by PPS.HPJ. >hlp\pps.hm
echo. >>hlp\pps.hm
echo // Commands (ID_* and IDM_*) >>hlp\pps.hm
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>hlp\pps.hm
echo. >>hlp\pps.hm
echo // Prompts (IDP_*) >>hlp\pps.hm
makehm IDP_,HIDP_,0x30000 resource.h >>hlp\pps.hm
echo. >>hlp\pps.hm
echo // Resources (IDR_*) >>hlp\pps.hm
makehm IDR_,HIDR_,0x20000 resource.h >>hlp\pps.hm
echo. >>hlp\pps.hm
echo // Dialogs (IDD_*) >>hlp\pps.hm
makehm IDD_,HIDD_,0x20000 resource.h >>hlp\pps.hm
echo. >>hlp\pps.hm
echo // Frame Controls (IDW_*) >>hlp\pps.hm
makehm IDW_,HIDW_,0x50000 resource.h >>hlp\pps.hm
REM -- Make help for Project PPS
call hc31 pps.hpj
echo.
