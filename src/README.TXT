========================================================================
       PPS - an MFC based Single Document Interface (SDI) application
       with multiple views.
========================================================================

AppWizard has generated this PPS application for you.  

This file contains a summary of what you will find in each of the files that
make up your PPS application.


PPS.MAK
    This project file is compatible with the Visual C++ Workbench.

    It is also compatible with the NMAKE program provided with the
    Professional Edition of Visual C++.

    To build a debug version of the program from the MS-DOS prompt, type
	nmake DEBUG=1 /f PPS.MAK
    or to build a release version of the program, type
	nmake DEBUG=0 /f PPS.MAK

PPS.H
    This is the main include file for the application.  It includes other
    project specific includes (including RESOURCE.H) and declares the
    CPpsApp application class.

PPS.CPP
    This is the main application source file that contains the application
    class CPpsApp.

PPS.RC
    This is a listing of all of the Microsoft Windows resources that the
    program uses.  It includes the icons, bitmaps, and cursors that are stored
    in the RES subdirectory.  This file can be directly edited with App Studio.

RES\PPS.ICO
    This is an icon file, which is used as the application's icon.  This
    icon is included by the main resource file PPS.RC.

RES\PPS.RC2
    This file contains resources that are not edited by App Studio.  Initially
    this contains a VERSIONINFO resource that you can customize for your
    application.  You should place other non-App Studio editable resources
    in this file.

PPS.DEF
    This file contains information about the application that must be
    provided to run with Microsoft Windows.  It defines parameters
    such as the name and description of the application, and the size
    of the initial local heap.  The numbers in this file are typical
    for applications developed with the Microsoft Foundation Class Library.
    The default stack size can be adjusted by editing the project file.

PPS.CLW
    This file contains information used by ClassWizard to edit existing
    classes or add new classes.  ClassWizard also uses this file to store
    information needed to generate and edit message maps and dialog data
    maps and to generate prototype member functions.

/////////////////////////////////////////////////////////////////////////////

For the main frame window:

MAINFRM.H, MAINFRM.CPP
    These files contain the frame class CMainFrame, which is derived from
    CFrameWnd and controls all SDI frame features.

RES\TOOLBAR.BMP
    This bitmap file is used to create tiled images for the toolbar.
    The initial toolbar and status bar are constructed in the
    CMainFrame class.  Edit this toolbar bitmap along with the
    array in MAINFRM.CPP to add more toolbar buttons.

/////////////////////////////////////////////////////////////////////////////

AppWizard creates one document type and one view:

PPSDOC.H, PPSDOC.CPP - the document
    These files contain your CPpsDoc class.  Edit these files to
    add your special document data and to implement file saving and loading
    (via CPpsDoc::Serialize).

PPSVIEW.H, PPSVIEW.CPP - the view of the document
    These files contain your CPpsView class.
    CPpsView objects are used to view CPpsDoc objects.

    

PPS.REG
    This is an example .REG file that shows you the kind of registration
    settings the framework will set for you.  You can use this as a .REG
    file to go along with your application or just delete it and rely
    on the default RegisterShellFileTypes registration.


/////////////////////////////////////////////////////////////////////////////

Help Support:

MAKEHELP.BAT
    Use this batch file to create your application's Help file, PPS.HLP.

PPS.HPJ
    This file is the Help Project file used by the Help compiler to create
    your application's Help file.

HLP\*.BMP
    These are bitmap files required by the standard Help file topics for
    Microsoft Foundation Class Library standard commands.

HLP\*.RTF
    This file contains the standard help topics for standard MFC
    commands and screen objects.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

STDAFX.H, STDAFX.CPP
    These files are used to build a precompiled header (PCH) file
    named STDAFX.PCH and a precompiled types (PCT) file named STDAFX.OBJ.

RESOURCE.H
    This is the standard header file, which defines new resource IDs.
    App Studio reads and updates this file.

/////////////////////////////////////////////////////////////////////////////
Other notes:

  AppWizard uses "TODO:" to indicate parts of the source code you
  should add to or customize.

///////////////////////////////////////////////////////////////////////////// 
PPS VIEWS:

    The sub-directories beneath /views contain view declaration and definition
    files for use with the PPS.  Each view is inhereted from a common 
    CPpsFormView class which is defined in ppsview.h,.cpp.  The /views/boiler/
    boiler.h,.cpp file contains details on how to add new views to the
    application.  Refer also to mainfrm.cpp and pps.cpp for comments on
    how to associate menu and toolbar items to view invocation.

/////////////////////////////////////////////////////////////////////////////
PAGER CLASSES:

    The /cpager sub-directory contains pager class definition files.  All
    pager classes are derived from class CPager as detailed in cpager.h,.cpp.
    See pegasus.h,.cpp as well as vail.h,.cpp.
    
////////////////////////////////////////////////////////////////////////////
SERIAL PORT SUPPORT:

   The /w16comm sub-directory contains files w16comm.h,.cpp which encapsulate
   support for 16-bit windows serial port access.  See also /views/commview/
   commview.h,.cpp.
   
////////////////////////////////////////////////////////////////////////////
GPIB PORT SUPPORT:

  The /w16gpib sub-directory contains files for 16-bit windows support of
  national instruments supplied GPIB cards.  You will need to ensure the
  file gpib.lib is copied from the /w16gpib/gpib directory to the /lib
  directory in the compiler installation directory.  
  
////////////////////////////////////////////////////////////////////////////
DEVICE TUNING SUPPORT:

  See /dialogs/tuning and associated files for reference.

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

