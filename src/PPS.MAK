# Microsoft Visual C++ generated build script - Do not modify

PROJ = PPS
DEBUG = 0
PROGTYPE = 0
CALLER = 
ARGS = 
DLLS = 
D_RCDEFINES = /d_DEBUG 
R_RCDEFINES = /dNDEBUG 
ORIGIN = MSVC
ORIGIN_VER = 1.00
PROJPATH = C:\VOBDEV\PPS16\
USEMFC = 1
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = /YcSTDAFX.H
CUSEPCHFLAG = 
CPPUSEPCHFLAG = /YuSTDAFX.H
FIRSTC =             
FIRSTCPP = STDAFX.CPP  
RC = rc
CFLAGS_D_WEXE = /nologo /G3 /W3 /Zi /AL /Gx- /Od /D "_DEBUG" /D "TBX_FRAME" /D "DISABLE_TUNING" /D "DISABLE_HEXEDIT" /GA /Fd"PPS.PDB" 
CFLAGS_R_WEXE = /nologo /Gs /G3 /W3 /AL /D "NDEBUG" /D "TBX_FRAME" /GA 
LFLAGS_D_WEXE = /NOLOGO /NOD /NOI /PACKC:61440 /STACK:8000 /ALIGN:16 /ONERROR:NOEXE /CO  
LFLAGS_R_WEXE = /NOLOGO /NOD /NOI /PACKC:61440 /STACK:10240 /ALIGN:16 /ONERROR:NOEXE /MAP  
LIBS_D_WEXE = lafxcwd oldnames libw llibcew gpib commdlg.lib mmsystem.lib shell.lib 
LIBS_R_WEXE = lafxcw oldnames libw llibcew gpib commdlg.lib mmsystem.lib shell.lib 
RCFLAGS = /nologo /z 
RESFLAGS = /nologo /t /k
RUNFLAGS = 
DEFFILE = PPS.DEF
OBJS_EXT = 
LIBS_EXT = 
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS_D_WEXE)
LFLAGS = $(LFLAGS_D_WEXE)
LIBS = $(LIBS_D_WEXE)
MAPFILE = nul
RCDEFINES = $(D_RCDEFINES)
!else
CFLAGS = $(CFLAGS_R_WEXE)
LFLAGS = $(LFLAGS_R_WEXE)
LIBS = $(LIBS_R_WEXE)
MAPFILE = nul
RCDEFINES = $(R_RCDEFINES)
!endif
!if [if exist MSVC.BND del MSVC.BND]
!endif
SBRS = STDAFX.SBR \
		PPS.SBR \
		MAINFRM.SBR \
		PPSDOC.SBR \
		PPSVIEW.SBR \
		W16COMM.SBR \
		CPAGER.SBR \
		CIOOPER.SBR \
		PEGASUS.SBR \
		HEXEDIT.SBR \
		DWNLDPWD.SBR \
		PAGERID.SBR \
		NUMEDIT.SBR \
		ADDRESS.SBR \
		INFOSVCS.SBR \
		TOOLBARX.SBR \
		SECURITY.SBR \
		CODEPLUG.SBR \
		PROTOCOL.SBR \
		SPINBTN.SBR \
		MESSAGES.SBR \
		ALERTS.SBR \
		OPTIONS.SBR \
		PROTO50.SBR \
		R50REG.SBR \
		CANMSGS.SBR \
		BSOUND.SBR \
		PROGALRT.SBR \
		COMMVIEW.SBR \
		TUNE.SBR \
		HPMDA.SBR \
		W16GPIB.SBR \
		COMPORTS.SBR \
		R25ADDR.SBR \
		PROTO25.SBR \
		OPTS25.SBR


PPS_RCDEP = c:\vobdev\pps16\res\pps.ico \
	c:\vobdev\pps16\res\icon1.ico \
	c:\vobdev\pps16\res\icon2.ico \
	c:\vobdev\pps16\res\icon3.ico \
	c:\vobdev\pps16\res\dnsd.bmp \
	c:\vobdev\pps16\res\dnsf.bmp \
	c:\vobdev\pps16\res\dnsu.bmp \
	c:\vobdev\pps16\res\dnsx.bmp \
	c:\vobdev\pps16\res\upsd.bmp \
	c:\vobdev\pps16\res\upsf.bmp \
	c:\vobdev\pps16\res\upsu.bmp \
	c:\vobdev\pps16\res\upsx.bmp \
	c:\vobdev\pps16\res\ampdlogo.bmp \
	c:\vobdev\pps16\res\pps.rc2


STDAFX_DEP = c:\vobdev\pps16\stdafx.h \
	c:\vobdev\pps16\toolbarx\toolbarx.h


PPS_DEP = c:\vobdev\pps16\stdafx.h \
	c:\vobdev\pps16\toolbarx\toolbarx.h \
	c:\vobdev\pps16\pps.h \
	c:\vobdev\pps16\cpager\cpager.h \
	c:\vobdev\pps16\cpager\ciooper.h \
	c:\vobdev\pps16\w16comm\w16comm.h \
	c:\vobdev\pps16\w16comm\comports.h \
	c:\vobdev\pps16\w16gpib\w16gpib.h \
	c:\vobdev\pps16\w16gpib\gpib\windecl.h \
	c:\vobdev\pps16\mainfrm.h \
	c:\vobdev\pps16\ppsdoc.h \
	c:\vobdev\pps16\ppsview.h \
	c:\vobdev\pps16\cpager\pegasus.h \
	c:\vobdev\pps16\cpager\codeplug.h \
	c:\vobdev\pps16\dialogs\tuning\hpmda.h \
	c:\vobdev\pps16\views\hexedit\hexedit.h \
	c:\vobdev\pps16\views\pagerid\pagerid.h \
	c:\vobdev\pps16\views\address\address.h \
	c:\vobdev\pps16\misc\numedit.h \
	c:\vobdev\pps16\views\address\r25addr.h \
	c:\vobdev\pps16\views\security\security.h \
	c:\vobdev\pps16\views\protocol\protocol.h \
	c:\vobdev\pps16\misc\spinbtn\spinbtn.h \
	c:\vobdev\pps16\views\proto50\proto50.h \
	c:\vobdev\pps16\views\messages\messages.h \
	c:\vobdev\pps16\views\alerts\alerts.h \
	c:\vobdev\pps16\views\options\options.h \
	c:\vobdev\pps16\views\r50reg\r50reg.h \
	c:\vobdev\pps16\views\canmsgs\canmsgs.h \
	c:\vobdev\pps16\views\commview\commview.h \
	c:\vobdev\pps16\views\protocol\proto25.h \
	c:\vobdev\pps16\views\options\opts25.h


MAINFRM_DEP = c:\vobdev\pps16\stdafx.h \
	c:\vobdev\pps16\toolbarx\toolbarx.h \
	c:\vobdev\pps16\pps.h \
	c:\vobdev\pps16\cpager\cpager.h \
	c:\vobdev\pps16\cpager\ciooper.h \
	c:\vobdev\pps16\w16comm\w16comm.h \
	c:\vobdev\pps16\w16comm\comports.h \
	c:\vobdev\pps16\w16gpib\w16gpib.h \
	c:\vobdev\pps16\w16gpib\gpib\windecl.h \
	c:\vobdev\pps16\mainfrm.h \
	c:\vobdev\pps16\cpager\pegasus.h \
	c:\vobdev\pps16\ppsdoc.h \
	c:\vobdev\pps16\ppsview.h \
	c:\vobdev\pps16\cpager\codeplug.h \
	c:\vobdev\pps16\dialogs\dwnldpwd\dwnldpwd.h \
	c:\vobdev\pps16\dialogs\tuning\tune.h \
	c:\vobdev\pps16\misc\spinbtn\spinbtn.h \
	c:\vobdev\pps16\misc\numedit.h


PPSDOC_DEP = c:\vobdev\pps16\stdafx.h \
	c:\vobdev\pps16\toolbarx\toolbarx.h \
	c:\vobdev\pps16\pps.h \
	c:\vobdev\pps16\cpager\cpager.h \
	c:\vobdev\pps16\cpager\ciooper.h \
	c:\vobdev\pps16\w16comm\w16comm.h \
	c:\vobdev\pps16\w16comm\comports.h \
	c:\vobdev\pps16\w16gpib\w16gpib.h \
	c:\vobdev\pps16\w16gpib\gpib\windecl.h \
	c:\vobdev\pps16\ppsdoc.h \
	c:\vobdev\pps16\mainfrm.h \
	c:\vobdev\pps16\cpager\codeplug.h


PPSVIEW_DEP = c:\vobdev\pps16\stdafx.h \
	c:\vobdev\pps16\toolbarx\toolbarx.h \
	c:\vobdev\pps16\pps.h \
	c:\vobdev\pps16\cpager\cpager.h \
	c:\vobdev\pps16\cpager\ciooper.h \
	c:\vobdev\pps16\w16comm\w16comm.h \
	c:\vobdev\pps16\w16comm\comports.h \
	c:\vobdev\pps16\w16gpib\w16gpib.h \
	c:\vobdev\pps16\w16gpib\gpib\windecl.h \
	c:\vobdev\pps16\ppsdoc.h \
	c:\vobdev\pps16\ppsview.h


W16COMM_DEP = c:\vobdev\pps16\pps.h \
	c:\vobdev\pps16\cpager\cpager.h \
	c:\vobdev\pps16\cpager\ciooper.h \
	c:\vobdev\pps16\w16comm\w16comm.h \
	c:\vobdev\pps16\w16comm\comports.h \
	c:\vobdev\pps16\w16gpib\w16gpib.h \
	c:\vobdev\pps16\w16gpib\gpib\windecl.h


CPAGER_DEP = c:\vobdev\pps16\pps.h \
	c:\vobdev\pps16\cpager\cpager.h \
	c:\vobdev\pps16\cpager\ciooper.h \
	c:\vobdev\pps16\w16comm\w16comm.h \
	c:\vobdev\pps16\w16comm\comports.h \
	c:\vobdev\pps16\w16gpib\w16gpib.h \
	c:\vobdev\pps16\w16gpib\gpib\windecl.h \
	c:\vobdev\pps16\mainfrm.h


CIOOPER_DEP = c:\vobdev\pps16\pps.h \
	c:\vobdev\pps16\cpager\cpager.h \
	c:\vobdev\pps16\cpager\ciooper.h \
	c:\vobdev\pps16\w16comm\w16comm.h \
	c:\vobdev\pps16\w16comm\comports.h \
	c:\vobdev\pps16\w16gpib\w16gpib.h \
	c:\vobdev\pps16\w16gpib\gpib\windecl.h


PEGASUS_DEP = c:\vobdev\pps16\pps.h \
	c:\vobdev\pps16\cpager\cpager.h \
	c:\vobdev\pps16\cpager\ciooper.h \
	c:\vobdev\pps16\w16comm\w16comm.h \
	c:\vobdev\pps16\w16comm\comports.h \
	c:\vobdev\pps16\w16gpib\w16gpib.h \
	c:\vobdev\pps16\w16gpib\gpib\windecl.h \
	c:\vobdev\pps16\mainfrm.h \
	c:\vobdev\pps16\ppsdoc.h \
	c:\vobdev\pps16\cpager\pegasus.h \
	c:\vobdev\pps16\cpager\codeplug.h


HEXEDIT_DEP = c:\vobdev\pps16\pps.h \
	c:\vobdev\pps16\cpager\cpager.h \
	c:\vobdev\pps16\cpager\ciooper.h \
	c:\vobdev\pps16\w16comm\w16comm.h \
	c:\vobdev\pps16\w16comm\comports.h \
	c:\vobdev\pps16\w16gpib\w16gpib.h \
	c:\vobdev\pps16\w16gpib\gpib\windecl.h \
	c:\vobdev\pps16\ppsdoc.h \
	c:\vobdev\pps16\ppsview.h \
	c:\vobdev\pps16\mainfrm.h \
	c:\vobdev\pps16\views\hexedit\hexedit.h


DWNLDPWD_DEP = c:\vobdev\pps16\pps.h \
	c:\vobdev\pps16\cpager\cpager.h \
	c:\vobdev\pps16\cpager\ciooper.h \
	c:\vobdev\pps16\w16comm\w16comm.h \
	c:\vobdev\pps16\w16comm\comports.h \
	c:\vobdev\pps16\w16gpib\w16gpib.h \
	c:\vobdev\pps16\w16gpib\gpib\windecl.h \
	c:\vobdev\pps16\dialogs\dwnldpwd\dwnldpwd.h


PAGERID_DEP = c:\vobdev\pps16\pps.h \
	c:\vobdev\pps16\cpager\cpager.h \
	c:\vobdev\pps16\cpager\ciooper.h \
	c:\vobdev\pps16\w16comm\w16comm.h \
	c:\vobdev\pps16\w16comm\comports.h \
	c:\vobdev\pps16\w16gpib\w16gpib.h \
	c:\vobdev\pps16\w16gpib\gpib\windecl.h \
	c:\vobdev\pps16\ppsdoc.h \
	c:\vobdev\pps16\ppsview.h \
	c:\vobdev\pps16\mainfrm.h \
	c:\vobdev\pps16\cpager\codeplug.h \
	c:\vobdev\pps16\views\pagerid\pagerid.h


NUMEDIT_DEP = c:\vobdev\pps16\misc\numedit.h


ADDRESS_DEP = c:\vobdev\pps16\pps.h \
	c:\vobdev\pps16\cpager\cpager.h \
	c:\vobdev\pps16\cpager\ciooper.h \
	c:\vobdev\pps16\w16comm\w16comm.h \
	c:\vobdev\pps16\w16comm\comports.h \
	c:\vobdev\pps16\w16gpib\w16gpib.h \
	c:\vobdev\pps16\w16gpib\gpib\windecl.h \
	c:\vobdev\pps16\ppsdoc.h \
	c:\vobdev\pps16\ppsview.h \
	c:\vobdev\pps16\mainfrm.h \
	c:\vobdev\pps16\cpager\codeplug.h \
	c:\vobdev\pps16\views\address\address.h \
	c:\vobdev\pps16\misc\numedit.h \
	c:\vobdev\pps16\views\address\infosvcs.h


INFOSVCS_DEP = c:\vobdev\pps16\pps.h \
	c:\vobdev\pps16\cpager\cpager.h \
	c:\vobdev\pps16\cpager\ciooper.h \
	c:\vobdev\pps16\w16comm\w16comm.h \
	c:\vobdev\pps16\w16comm\comports.h \
	c:\vobdev\pps16\w16gpib\w16gpib.h \
	c:\vobdev\pps16\w16gpib\gpib\windecl.h \
	c:\vobdev\pps16\views\address\infosvcs.h


SECURITY_DEP = c:\vobdev\pps16\pps.h


SECURITY_DEP = c:\vobdev\pps16\cpager\cpager.h \
	c:\vobdev\pps16\cpager\ciooper.h \
	c:\vobdev\pps16\w16comm\w16comm.h \
	c:\vobdev\pps16\w16comm\comports.h \
	c:\vobdev\pps16\w16gpib\w16gpib.h \
	c:\vobdev\pps16\w16gpib\gpib\windecl.h \
	c:\vobdev\pps16\ppsdoc.h \
	c:\vobdev\pps16\ppsview.h \
	c:\vobdev\pps16\mainfrm.h \
	c:\vobdev\pps16\cpager\codeplug.h \
	c:\vobdev\pps16\dialogs\dwnldpwd\dwnldpwd.h \
	c:\vobdev\pps16\views\security\security.h


CODEPLUG_DEP = c:\vobdev\pps16\pps.h \
	c:\vobdev\pps16\cpager\cpager.h \
	c:\vobdev\pps16\cpager\ciooper.h \
	c:\vobdev\pps16\w16comm\w16comm.h \
	c:\vobdev\pps16\w16comm\comports.h \
	c:\vobdev\pps16\w16gpib\w16gpib.h \
	c:\vobdev\pps16\w16gpib\gpib\windecl.h \
	c:\vobdev\pps16\ppsdoc.h \
	c:\vobdev\pps16\cpager\codeplug.h


PROTOCOL_DEP = c:\vobdev\pps16\pps.h \
	c:\vobdev\pps16\cpager\cpager.h \
	c:\vobdev\pps16\cpager\ciooper.h \
	c:\vobdev\pps16\w16comm\w16comm.h \
	c:\vobdev\pps16\w16comm\comports.h \
	c:\vobdev\pps16\w16gpib\w16gpib.h \
	c:\vobdev\pps16\w16gpib\gpib\windecl.h \
	c:\vobdev\pps16\ppsdoc.h \
	c:\vobdev\pps16\ppsview.h \
	c:\vobdev\pps16\cpager\codeplug.h \
	c:\vobdev\pps16\views\protocol\protocol.h \
	c:\vobdev\pps16\misc\spinbtn\spinbtn.h \
	c:\vobdev\pps16\misc\numedit.h


SPINBTN_DEP = c:\vobdev\pps16\misc\spinbtn\spinbtn.h \
	c:\vobdev\pps16\pps.h \
	c:\vobdev\pps16\cpager\cpager.h \
	c:\vobdev\pps16\cpager\ciooper.h \
	c:\vobdev\pps16\w16comm\w16comm.h \
	c:\vobdev\pps16\w16comm\comports.h \
	c:\vobdev\pps16\w16gpib\w16gpib.h \
	c:\vobdev\pps16\w16gpib\gpib\windecl.h


MESSAGES_DEP = c:\vobdev\pps16\pps.h \
	c:\vobdev\pps16\cpager\cpager.h \
	c:\vobdev\pps16\cpager\ciooper.h \
	c:\vobdev\pps16\w16comm\w16comm.h \
	c:\vobdev\pps16\w16comm\comports.h \
	c:\vobdev\pps16\w16gpib\w16gpib.h \
	c:\vobdev\pps16\w16gpib\gpib\windecl.h \
	c:\vobdev\pps16\ppsdoc.h \
	c:\vobdev\pps16\ppsview.h \
	c:\vobdev\pps16\cpager\codeplug.h \
	c:\vobdev\pps16\views\messages\messages.h


ALERTS_DEP = c:\vobdev\pps16\pps.h \
	c:\vobdev\pps16\cpager\cpager.h \
	c:\vobdev\pps16\cpager\ciooper.h \
	c:\vobdev\pps16\w16comm\w16comm.h \
	c:\vobdev\pps16\w16comm\comports.h \
	c:\vobdev\pps16\w16gpib\w16gpib.h \
	c:\vobdev\pps16\w16gpib\gpib\windecl.h \
	c:\vobdev\pps16\ppsdoc.h \
	c:\vobdev\pps16\ppsview.h \
	c:\vobdev\pps16\cpager\codeplug.h \
	c:\vobdev\pps16\misc\spinbtn\spinbtn.h \
	c:\vobdev\pps16\views\alerts\alerts.h \
	c:\vobdev\pps16\views\alerts\progalrt.h \
	c:\vobdev\pps16\misc\bsound.h


OPTIONS_DEP = c:\vobdev\pps16\pps.h \
	c:\vobdev\pps16\cpager\cpager.h \
	c:\vobdev\pps16\cpager\ciooper.h \
	c:\vobdev\pps16\w16comm\w16comm.h \
	c:\vobdev\pps16\w16comm\comports.h \
	c:\vobdev\pps16\w16gpib\w16gpib.h \
	c:\vobdev\pps16\w16gpib\gpib\windecl.h \
	c:\vobdev\pps16\ppsdoc.h \
	c:\vobdev\pps16\ppsview.h \
	c:\vobdev\pps16\cpager\codeplug.h \
	c:\vobdev\pps16\misc\spinbtn\spinbtn.h \
	c:\vobdev\pps16\views\options\options.h


PROTO50_DEP = c:\vobdev\pps16\pps.h \
	c:\vobdev\pps16\cpager\cpager.h \
	c:\vobdev\pps16\cpager\ciooper.h \
	c:\vobdev\pps16\w16comm\w16comm.h \
	c:\vobdev\pps16\w16comm\comports.h \
	c:\vobdev\pps16\w16gpib\w16gpib.h \
	c:\vobdev\pps16\w16gpib\gpib\windecl.h \
	c:\vobdev\pps16\ppsdoc.h \
	c:\vobdev\pps16\ppsview.h \
	c:\vobdev\pps16\cpager\codeplug.h \
	c:\vobdev\pps16\views\proto50\proto50.h \
	c:\vobdev\pps16\misc\spinbtn\spinbtn.h \
	c:\vobdev\pps16\misc\numedit.h


R50REG_DEP = c:\vobdev\pps16\pps.h \
	c:\vobdev\pps16\cpager\cpager.h \
	c:\vobdev\pps16\cpager\ciooper.h \
	c:\vobdev\pps16\w16comm\w16comm.h \
	c:\vobdev\pps16\w16comm\comports.h \
	c:\vobdev\pps16\w16gpib\w16gpib.h \
	c:\vobdev\pps16\w16gpib\gpib\windecl.h \
	c:\vobdev\pps16\ppsdoc.h \
	c:\vobdev\pps16\ppsview.h \
	c:\vobdev\pps16\cpager\codeplug.h \
	c:\vobdev\pps16\views\r50reg\r50reg.h \
	c:\vobdev\pps16\misc\spinbtn\spinbtn.h


CANMSGS_DEP = c:\vobdev\pps16\pps.h \
	c:\vobdev\pps16\cpager\cpager.h \
	c:\vobdev\pps16\cpager\ciooper.h \
	c:\vobdev\pps16\w16comm\w16comm.h \
	c:\vobdev\pps16\w16comm\comports.h \
	c:\vobdev\pps16\w16gpib\w16gpib.h \
	c:\vobdev\pps16\w16gpib\gpib\windecl.h \
	c:\vobdev\pps16\ppsdoc.h \
	c:\vobdev\pps16\ppsview.h \
	c:\vobdev\pps16\cpager\codeplug.h \
	c:\vobdev\pps16\mainfrm.h \
	c:\vobdev\pps16\views\canmsgs\canmsgs.h


BSOUND_DEP = c:\vobdev\pps16\misc\bsound.h


PROGALRT_DEP = c:\vobdev\pps16\pps.h \
	c:\vobdev\pps16\cpager\cpager.h \
	c:\vobdev\pps16\cpager\ciooper.h \
	c:\vobdev\pps16\w16comm\w16comm.h \
	c:\vobdev\pps16\w16comm\comports.h \
	c:\vobdev\pps16\w16gpib\w16gpib.h \
	c:\vobdev\pps16\w16gpib\gpib\windecl.h \
	c:\vobdev\pps16\views\alerts\progalrt.h \
	c:\vobdev\pps16\misc\bsound.h


COMMVIEW_DEP = c:\vobdev\pps16\pps.h \
	c:\vobdev\pps16\cpager\cpager.h \
	c:\vobdev\pps16\cpager\ciooper.h \
	c:\vobdev\pps16\w16comm\w16comm.h \
	c:\vobdev\pps16\w16comm\comports.h \
	c:\vobdev\pps16\w16gpib\w16gpib.h \
	c:\vobdev\pps16\w16gpib\gpib\windecl.h \
	c:\vobdev\pps16\ppsdoc.h \
	c:\vobdev\pps16\ppsview.h \
	c:\vobdev\pps16\mainfrm.h \
	c:\vobdev\pps16\cpager\codeplug.h \
	c:\vobdev\pps16\views\commview\commview.h


TUNE_DEP = c:\vobdev\pps16\pps.h \
	c:\vobdev\pps16\cpager\cpager.h \
	c:\vobdev\pps16\cpager\ciooper.h \
	c:\vobdev\pps16\w16comm\w16comm.h \
	c:\vobdev\pps16\w16comm\comports.h \
	c:\vobdev\pps16\w16gpib\w16gpib.h \
	c:\vobdev\pps16\w16gpib\gpib\windecl.h \
	c:\vobdev\pps16\ppsdoc.h \
	c:\vobdev\pps16\ppsview.h \
	c:\vobdev\pps16\cpager\pegasus.h \
	c:\vobdev\pps16\cpager\codeplug.h \
	c:\vobdev\pps16\mainfrm.h \
	c:\vobdev\pps16\dialogs\tuning\tune.h \
	c:\vobdev\pps16\misc\spinbtn\spinbtn.h \
	c:\vobdev\pps16\misc\numedit.h \
	c:\vobdev\pps16\dialogs\tuning\hpmda.h \
	c:\vobdev\pps16\views\protocol\protocol.h \
	c:\vobdev\pps16\views\proto50\proto50.h


HPMDA_DEP = c:\vobdev\pps16\pps.h \
	c:\vobdev\pps16\cpager\cpager.h \
	c:\vobdev\pps16\cpager\ciooper.h \
	c:\vobdev\pps16\w16comm\w16comm.h \
	c:\vobdev\pps16\w16comm\comports.h \
	c:\vobdev\pps16\w16gpib\w16gpib.h \
	c:\vobdev\pps16\w16gpib\gpib\windecl.h \
	c:\vobdev\pps16\dialogs\tuning\hpmda.h \
	c:\vobdev\pps16\dialogs\tuning\tune.h \
	c:\vobdev\pps16\misc\spinbtn\spinbtn.h \
	c:\vobdev\pps16\misc\numedit.h \
	c:\vobdev\pps16\cpager\pegasus.h


W16GPIB_DEP = c:\vobdev\pps16\pps.h \
	c:\vobdev\pps16\cpager\cpager.h \
	c:\vobdev\pps16\cpager\ciooper.h \
	c:\vobdev\pps16\w16comm\w16comm.h \
	c:\vobdev\pps16\w16comm\comports.h \
	c:\vobdev\pps16\w16gpib\w16gpib.h \
	c:\vobdev\pps16\w16gpib\gpib\windecl.h


COMPORTS_DEP = c:\vobdev\pps16\pps.h \
	c:\vobdev\pps16\cpager\cpager.h \
	c:\vobdev\pps16\cpager\ciooper.h \
	c:\vobdev\pps16\w16comm\w16comm.h \
	c:\vobdev\pps16\w16comm\comports.h \
	c:\vobdev\pps16\w16gpib\w16gpib.h \
	c:\vobdev\pps16\w16gpib\gpib\windecl.h


R25ADDR_DEP = c:\vobdev\pps16\pps.h \
	c:\vobdev\pps16\cpager\cpager.h \
	c:\vobdev\pps16\cpager\ciooper.h \
	c:\vobdev\pps16\w16comm\w16comm.h \
	c:\vobdev\pps16\w16comm\comports.h \
	c:\vobdev\pps16\w16gpib\w16gpib.h \
	c:\vobdev\pps16\w16gpib\gpib\windecl.h \
	c:\vobdev\pps16\ppsdoc.h \
	c:\vobdev\pps16\ppsview.h \
	c:\vobdev\pps16\mainfrm.h \
	c:\vobdev\pps16\cpager\codeplug.h \
	c:\vobdev\pps16\views\address\r25addr.h \
	c:\vobdev\pps16\misc\numedit.h


PROTO25_DEP = c:\vobdev\pps16\pps.h \
	c:\vobdev\pps16\cpager\cpager.h \
	c:\vobdev\pps16\cpager\ciooper.h \
	c:\vobdev\pps16\w16comm\w16comm.h \
	c:\vobdev\pps16\w16comm\comports.h \
	c:\vobdev\pps16\w16gpib\w16gpib.h \
	c:\vobdev\pps16\w16gpib\gpib\windecl.h \
	c:\vobdev\pps16\ppsdoc.h \
	c:\vobdev\pps16\ppsview.h \
	c:\vobdev\pps16\cpager\codeplug.h \
	c:\vobdev\pps16\views\protocol\protocol.h \
	c:\vobdev\pps16\misc\spinbtn\spinbtn.h \
	c:\vobdev\pps16\misc\numedit.h \
	c:\vobdev\pps16\views\protocol\proto25.h


OPTS25_DEP = c:\vobdev\pps16\pps.h \
	c:\vobdev\pps16\cpager\cpager.h \
	c:\vobdev\pps16\cpager\ciooper.h \
	c:\vobdev\pps16\w16comm\w16comm.h \
	c:\vobdev\pps16\w16comm\comports.h \
	c:\vobdev\pps16\w16gpib\w16gpib.h \
	c:\vobdev\pps16\w16gpib\gpib\windecl.h \
	c:\vobdev\pps16\ppsdoc.h \
	c:\vobdev\pps16\ppsview.h \
	c:\vobdev\pps16\cpager\codeplug.h \
	c:\vobdev\pps16\misc\spinbtn\spinbtn.h \
	c:\vobdev\pps16\views\options\opts25.h


all:	$(PROJ).EXE

PPS.RES:	PPS.RC $(PPS_RCDEP)
	$(RC) $(RCFLAGS) $(RCDEFINES) -r PPS.RC

STDAFX.OBJ:	STDAFX.CPP $(STDAFX_DEP)
	$(CPP) $(CFLAGS) $(CPPCREATEPCHFLAG) /c STDAFX.CPP

PPS.OBJ:	PPS.CPP $(PPS_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c PPS.CPP

MAINFRM.OBJ:	MAINFRM.CPP $(MAINFRM_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c MAINFRM.CPP

PPSDOC.OBJ:	PPSDOC.CPP $(PPSDOC_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c PPSDOC.CPP

PPSVIEW.OBJ:	PPSVIEW.CPP $(PPSVIEW_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c PPSVIEW.CPP

W16COMM.OBJ:	W16COMM\W16COMM.CPP $(W16COMM_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c W16COMM\W16COMM.CPP

CPAGER.OBJ:	CPAGER\CPAGER.CPP $(CPAGER_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c CPAGER\CPAGER.CPP

CIOOPER.OBJ:	CPAGER\CIOOPER.CPP $(CIOOPER_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c CPAGER\CIOOPER.CPP

PEGASUS.OBJ:	CPAGER\PEGASUS.CPP $(PEGASUS_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c CPAGER\PEGASUS.CPP

HEXEDIT.OBJ:	VIEWS\HEXEDIT\HEXEDIT.CPP $(HEXEDIT_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c VIEWS\HEXEDIT\HEXEDIT.CPP

DWNLDPWD.OBJ:	DIALOGS\DWNLDPWD\DWNLDPWD.CPP $(DWNLDPWD_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c DIALOGS\DWNLDPWD\DWNLDPWD.CPP

PAGERID.OBJ:	VIEWS\PAGERID\PAGERID.CPP $(PAGERID_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c VIEWS\PAGERID\PAGERID.CPP

NUMEDIT.OBJ:	MISC\NUMEDIT.CPP $(NUMEDIT_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c MISC\NUMEDIT.CPP

ADDRESS.OBJ:	VIEWS\ADDRESS\ADDRESS.CPP $(ADDRESS_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c VIEWS\ADDRESS\ADDRESS.CPP

INFOSVCS.OBJ:	VIEWS\ADDRESS\INFOSVCS.CPP $(INFOSVCS_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c VIEWS\ADDRESS\INFOSVCS.CPP

TOOLBARX.OBJ:	TOOLBARX\TOOLBARX.CPP $(TOOLBARX_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c TOOLBARX\TOOLBARX.CPP

SECURITY.OBJ:	VIEWS\SECURITY\SECURITY.CPP $(SECURITY_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c VIEWS\SECURITY\SECURITY.CPP

CODEPLUG.OBJ:	CPAGER\CODEPLUG.CPP $(CODEPLUG_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c CPAGER\CODEPLUG.CPP

PROTOCOL.OBJ:	VIEWS\PROTOCOL\PROTOCOL.CPP $(PROTOCOL_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c VIEWS\PROTOCOL\PROTOCOL.CPP

SPINBTN.OBJ:	MISC\SPINBTN\SPINBTN.CPP $(SPINBTN_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c MISC\SPINBTN\SPINBTN.CPP

MESSAGES.OBJ:	VIEWS\MESSAGES\MESSAGES.CPP $(MESSAGES_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c VIEWS\MESSAGES\MESSAGES.CPP

ALERTS.OBJ:	VIEWS\ALERTS\ALERTS.CPP $(ALERTS_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c VIEWS\ALERTS\ALERTS.CPP

OPTIONS.OBJ:	VIEWS\OPTIONS\OPTIONS.CPP $(OPTIONS_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c VIEWS\OPTIONS\OPTIONS.CPP

PROTO50.OBJ:	VIEWS\PROTO50\PROTO50.CPP $(PROTO50_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c VIEWS\PROTO50\PROTO50.CPP

R50REG.OBJ:	VIEWS\R50REG\R50REG.CPP $(R50REG_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c VIEWS\R50REG\R50REG.CPP

CANMSGS.OBJ:	VIEWS\CANMSGS\CANMSGS.CPP $(CANMSGS_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c VIEWS\CANMSGS\CANMSGS.CPP

BSOUND.OBJ:	MISC\BSOUND.CPP $(BSOUND_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c MISC\BSOUND.CPP

PROGALRT.OBJ:	VIEWS\ALERTS\PROGALRT.CPP $(PROGALRT_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c VIEWS\ALERTS\PROGALRT.CPP

COMMVIEW.OBJ:	VIEWS\COMMVIEW\COMMVIEW.CPP $(COMMVIEW_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c VIEWS\COMMVIEW\COMMVIEW.CPP

TUNE.OBJ:	DIALOGS\TUNING\TUNE.CPP $(TUNE_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c DIALOGS\TUNING\TUNE.CPP

HPMDA.OBJ:	DIALOGS\TUNING\HPMDA.CPP $(HPMDA_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c DIALOGS\TUNING\HPMDA.CPP

W16GPIB.OBJ:	W16GPIB\W16GPIB.CPP $(W16GPIB_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c W16GPIB\W16GPIB.CPP

COMPORTS.OBJ:	W16COMM\COMPORTS.CPP $(COMPORTS_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c W16COMM\COMPORTS.CPP

R25ADDR.OBJ:	VIEWS\ADDRESS\R25ADDR.CPP $(R25ADDR_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c VIEWS\ADDRESS\R25ADDR.CPP

PROTO25.OBJ:	VIEWS\PROTOCOL\PROTO25.CPP $(PROTO25_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c VIEWS\PROTOCOL\PROTO25.CPP

OPTS25.OBJ:	VIEWS\OPTIONS\OPTS25.CPP $(OPTS25_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c VIEWS\OPTIONS\OPTS25.CPP


$(PROJ).EXE::	PPS.RES

$(PROJ).EXE::	STDAFX.OBJ PPS.OBJ MAINFRM.OBJ PPSDOC.OBJ PPSVIEW.OBJ W16COMM.OBJ CPAGER.OBJ \
	CIOOPER.OBJ PEGASUS.OBJ HEXEDIT.OBJ DWNLDPWD.OBJ PAGERID.OBJ NUMEDIT.OBJ ADDRESS.OBJ \
	INFOSVCS.OBJ TOOLBARX.OBJ SECURITY.OBJ CODEPLUG.OBJ PROTOCOL.OBJ SPINBTN.OBJ MESSAGES.OBJ \
	ALERTS.OBJ OPTIONS.OBJ PROTO50.OBJ R50REG.OBJ CANMSGS.OBJ BSOUND.OBJ PROGALRT.OBJ \
	COMMVIEW.OBJ TUNE.OBJ HPMDA.OBJ W16GPIB.OBJ COMPORTS.OBJ R25ADDR.OBJ PROTO25.OBJ \
	OPTS25.OBJ $(OBJS_EXT) $(DEFFILE)
	echo >NUL @<<$(PROJ).CRF
STDAFX.OBJ +
PPS.OBJ +
MAINFRM.OBJ +
PPSDOC.OBJ +
PPSVIEW.OBJ +
W16COMM.OBJ +
CPAGER.OBJ +
CIOOPER.OBJ +
PEGASUS.OBJ +
HEXEDIT.OBJ +
DWNLDPWD.OBJ +
PAGERID.OBJ +
NUMEDIT.OBJ +
ADDRESS.OBJ +
INFOSVCS.OBJ +
TOOLBARX.OBJ +
SECURITY.OBJ +
CODEPLUG.OBJ +
PROTOCOL.OBJ +
SPINBTN.OBJ +
MESSAGES.OBJ +
ALERTS.OBJ +
OPTIONS.OBJ +
PROTO50.OBJ +
R50REG.OBJ +
CANMSGS.OBJ +
BSOUND.OBJ +
PROGALRT.OBJ +
COMMVIEW.OBJ +
TUNE.OBJ +
HPMDA.OBJ +
W16GPIB.OBJ +
COMPORTS.OBJ +
R25ADDR.OBJ +
PROTO25.OBJ +
OPTS25.OBJ +
$(OBJS_EXT)
$(PROJ).EXE
$(MAPFILE)
c:\msvc\lib\+
c:\msvc\mfc\lib\+
$(LIBS)
$(DEFFILE);
<<
	link $(LFLAGS) @$(PROJ).CRF
	$(RC) $(RESFLAGS) PPS.RES $@
	@copy $(PROJ).CRF MSVC.BND

$(PROJ).EXE::	PPS.RES
	if not exist MSVC.BND 	$(RC) $(RESFLAGS) PPS.RES $@

run: $(PROJ).EXE
	$(PROJ) $(RUNFLAGS)


$(PROJ).BSC: $(SBRS)
	bscmake @<<
/o$@ $(SBRS)
<<
