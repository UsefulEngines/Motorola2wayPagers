//*************************************************************
//  File name: MAINFRM.CPP
//
//  Description:  
//      Emplementation file for the CMainFrame class.
//      
//      
//
//  History:    Date       Author               Comment
//              03/96      P. Pennington        Created
//
// Written by Motorola AMSD, PMU Engineering.
// Legacy code for reference only.
//*************************************************************


#include "stdafx.h"
#include "pps.h"
#include "mainfrm.h"
#include "cpager\cpager.h"
#include "cpager\pegasus.h"
#include "w16comm\w16comm.h"
#include "ppsdoc.h"
#include "ppsview.h" 
#include "cpager\codeplug.h"
#include "dialogs\dwnldpwd\dwnldpwd.h"
#include "dialogs\tuning\tune.h" 
#include "cpager\ciooper.h"



extern CWin16Comm 		theSerialPort;
extern CPpsApp    		theApp;
extern CIoController	theIoController;



#ifdef _DEBUG 
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_COMMAND(ID_PAGER_PASSWD, OnPagerPasswd)
	ON_UPDATE_COMMAND_UI(ID_PAGER_PASSWD, OnUpdatePagerPasswd)
	ON_COMMAND(ID_TXTUNE_MODBAL_HIGH, OnTxtuneModbalHigh)
	ON_UPDATE_COMMAND_UI(ID_TXTUNE_MODBAL_HIGH, OnUpdateTxtuneModbalHigh)
	ON_COMMAND(ID_TXTUNE_MODBAL_LOW, OnTxtuneModbalLow)
	ON_UPDATE_COMMAND_UI(ID_TXTUNE_MODBAL_LOW, OnUpdateTxtuneModbalLow)
	ON_COMMAND(ID_TXTUNE_SCF, OnTxtuneScf)
	ON_UPDATE_COMMAND_UI(ID_TXTUNE_SCF, OnUpdateTxtuneScf)
	ON_WM_CLOSE()
	ON_COMMAND(ID_RXTUNE_REFOSC, OnRxtuneRefosc)
	ON_UPDATE_COMMAND_UI(ID_RXTUNE_REFOSC, OnUpdateRxtuneRefosc)
	ON_COMMAND(ID_PAGER_SW_RESET, OnPagerSwReset)
	ON_UPDATE_COMMAND_UI(ID_PAGER_SW_RESET, OnUpdatePagerSwReset)
	ON_COMMAND(ID_PAGER_HW_RESET, OnPagerHwReset)
	ON_UPDATE_COMMAND_UI(ID_PAGER_HW_RESET, OnUpdatePagerHwReset)
	ON_COMMAND(ID_TXTUNE_TXON_TEST, OnTxtuneTxonTest)
	ON_UPDATE_COMMAND_UI(ID_TXTUNE_TXON_TEST, OnUpdateTxtuneTxonTest)
	ON_COMMAND(ID_VIEW_BANNER, OnViewBanner)
	ON_UPDATE_COMMAND_UI(ID_VIEW_BANNER, OnUpdateViewBanner)
	ON_COMMAND(ID_RXTUNE_RCVRON_TEST, OnRxtuneRcvronTest)
	ON_UPDATE_COMMAND_UI(ID_RXTUNE_RCVRON_TEST, OnUpdateRxtuneRcvronTest)
	ON_COMMAND(ID_TXTUNE_SYMBOL_DEV, OnTxTuneSymbolDev)
	ON_UPDATE_COMMAND_UI(ID_TXTUNE_SYMBOL_DEV, OnUpdateTxTuneSymbolDev)
	ON_WM_ENTERIDLE()
	ON_COMMAND(ID_DEVICE_TUNING, OnDeviceTuning)
	ON_UPDATE_COMMAND_UI(ID_DEVICE_TUNING, OnUpdateDeviceTuning)
	ON_COMMAND(ID_VIEW_NEXT, OnViewNext)
	ON_COMMAND(ID_VIEW_PREVIOUS, OnViewPrevious)
	ON_COMMAND(ID_DEVICE_COMSETTINGS, OnDeviceComsettings)
	ON_UPDATE_COMMAND_UI(ID_DEVICE_COMSETTINGS, OnUpdateDeviceComsettings)
	//}}AFX_MSG_MAP
	// THE FOLLOWING MSG HANDLERS EDIT MANUALLY
	ON_MESSAGE(WM_COMMNOTIFY, OnCommNotify) 
	ON_MESSAGE(WM_IO_OPERATION, OnWmIoOperation)
	ON_MESSAGE(WM_READ_CP_DONE, OnWmReadCodeplugDone)
	ON_MESSAGE(WM_WRITE_CP_DONE, OnWmWriteCodeplugDone) 
	ON_MESSAGE(WM_IO_OPLIST_DONE, OnWmIoOpListDone)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_PORT, OnUpdateIndicatorPort) 
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_IOOP, OnUpdateIndicatorIoOp)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_PROGRESS, OnUpdateIndicatorProgress)  
   	// Global help commands
	ON_COMMAND(ID_HELP_INDEX, CFrameWnd::OnHelpIndex)
	ON_COMMAND(ID_HELP_USING, CFrameWnd::OnHelpUsing)
	ON_COMMAND(ID_HELP, CFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CFrameWnd::OnHelpIndex)
	// View sub-menu dynamic msg handlers
	ON_COMMAND(ID_VIEW_PAGERID, OnViewPagerid)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PAGERID, OnUpdateViewPagerid)
	ON_COMMAND(ID_VIEW_SECURITY, OnViewSecurity)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SECURITY, OnUpdateViewSecurity)
	ON_COMMAND(ID_VIEW_ADDRESSES, OnViewAddresses)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ADDRESSES, OnUpdateViewAddresses)
	ON_COMMAND(ID_VIEW_PROTOCOL, OnViewProtocol)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PROTOCOL, OnUpdateViewProtocol)
	ON_COMMAND(ID_VIEW_MESSAGES, OnViewMessages)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MESSAGES, OnUpdateViewMessages)
	ON_COMMAND(ID_VIEW_ALERTS, OnViewAlerts)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ALERTS, OnUpdateViewAlerts)
	ON_COMMAND(ID_VIEW_OPTIONS, OnViewOptions)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OPTIONS, OnUpdateViewOptions)
	ON_COMMAND(ID_VIEW_HEXEDIT, OnViewHexedit)
	ON_UPDATE_COMMAND_UI(ID_VIEW_HEXEDIT, OnUpdateViewHexedit)
	ON_COMMAND(ID_VIEW_PCLINK, OnViewPclink)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PCLINK, OnUpdateViewPclink)
	ON_COMMAND(ID_VIEW_REGISTRATION, OnViewRegistration)
	ON_UPDATE_COMMAND_UI(ID_VIEW_REGISTRATION, OnUpdateViewRegistration)
	ON_COMMAND(ID_VIEW_CANNEDMSGS, OnViewCannedmsgs)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CANNEDMSGS, OnUpdateViewCannedmsgs)
	ON_COMMAND(ID_VIEW_COMMVIEW, OnViewCommview)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COMMVIEW, OnUpdateViewCommview)
END_MESSAGE_MAP()


////////////////////////////////////////////////////////////////////////////
//  *** NOTE ON THE DYNAMICALLY CONSTRUCTED VIEW MENU ***
//  
//  The View sub-menu is constructed on the fly depending upon the 
//  currently attached pager type (see file pps.cpp).  In order for the
//  menu to be constructed and/or modified, the following resource entries
//  must be made.
//
//  (1)  Each view menu item must have a unique command ID (e.g. ID_VIEW_PAGERID).
//       This command ID cannot be inserted using Class Wizard.  To define a unique
//       command ID, open file 'resource.h' and find the contiguous ID_VIEW_... section.
//       Insert the new ID_VIEW_NEWCMD define into the list using the next sequential
//       command ID number.  Be sure to increment the corresponding NEXT_COMMAND_ID..
//       define at the bottom of the resource.h file.
//
//  (2)  Add a new entry to the IDR_MAIN_FRAME string table.  Assign the string the
//       same ID_VIEW_NEWCMD number as just created in step (1).  Type the new string
//       status bar prompt.  Place a '\n' character at the end of the string followed
//       by a tool-tip prompt.
//
//  (3)  Declare an 'OnViewNewView' member function entry in mainfrm.h and well as an
//       'OnUpdateViewNewView' member function.  See similar entries therein.
//
//  (4)  Define corresponding ON_COMMAND(ID_VIEW_NEWCMD,...) and ON_UPDATE_COMMAND_UI(...)
//       message map entries in this file.  See similar entries above.
//
//  (5)  Define the corresponding message map function handler below.  See similar entries.
//
//  (6)  Ensure that the ViewInfo struct array in file 'pps.cpp' lists the new command
//       identifier along with its corresponding view class name.
//
//  The view menu is dynamically constructed via the RefreshViewMenu() function below.
//  This mechanism might be improved by overriding the OnCmdMsg() member function.  However,
//  the best improvement might be to utilize the ON_COMMAND_RANGE / ON_UPDATE_COMMAND_UI_RANGE
//  macros (which, unfortunately are not available with 16-bit MFC).
//
//////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
// arrays of IDs used to initialize control bars

static UINT BASED_CODE buttons_tuning[] =
{
	// same order as in the bitmap 'toolbar.bmp'
	ID_RXTUNE_REFOSC,
	ID_TXTUNE_SCF,   
	ID_TXTUNE_MODBAL_LOW,   
	ID_TXTUNE_MODBAL_HIGH,
	ID_TXTUNE_SYMBOL_DEV,   
	  ID_SEPARATOR,
	  ID_SEPARATOR,
	ID_RXTUNE_RCVRON_TEST,
	ID_TXTUNE_TXON_TEST,   
};



static UINT BASED_CODE indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_PORT,
	ID_INDICATOR_IOOP,
	ID_INDICATOR_PROGRESS
};




/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG




/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// disable tuning via build environment variable  
	#ifdef DISABLE_TUNING
	  m_bEnableTuning = FALSE;
	#else
      m_bEnableTuning = TRUE;
    #endif

    // disable the HexEditor via build environment variable
    #ifdef DISABLE_HEXEDIT
      m_bEnableHexEdit = FALSE;
    #else
      m_bEnableHexEdit = TRUE;
    #endif 
    m_byVisibleToolBars = TBSHORT;
    
    // use these for reset toggle indicators
    nReadBmpIndex = 3;
    nWriteBmpIndex = 4;
}



CMainFrame::~CMainFrame()
{
}


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT &cs)
{ 
  // prevent re-sizable frame since all views are CFormView based and
  // simply would not look centered without extra coding to center each
  // view when drawn.
  cs.style &= ~(WS_THICKFRAME);     
                                  
  return(CFrameWnd::PreCreateWindow(cs));
}



int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadBitmap(IDR_MAINFRAME)) 
	{
		TRACE("Failed to create toolbar\n");
		return -1;      // fail to create
	}
    
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE("Failed to create status bar\n");
		return -1;      // fail to create
	}   
    
    ViewToolBarShort();
	return 0;
}

 

CMenu *CMainFrame::ClearTheViewMenu(void)
{
	// Locate the View submenu
	CMenu* pViewMenu = NULL;
	CMenu* pTopMenu = GetMenu();
	int iPos;
	for (iPos = pTopMenu->GetMenuItemCount()-1; iPos >= 0; iPos--)
	{
		CMenu* pMenu = pTopMenu->GetSubMenu(iPos);
		if (pMenu && pMenu->GetMenuItemID(0) == ID_VIEW_NEXT)
		{
			pViewMenu = pMenu;
			break;
		}
	}
	ASSERT(pViewMenu != NULL);

	// delete all items but ID_VIEW_BANNER at position 3 in the View sub-menu.
	for (iPos = pViewMenu->GetMenuItemCount()-1; iPos > 3; iPos--)
		pViewMenu->DeleteMenu(iPos, MF_BYPOSITION);
  
    // return the view menu pointer
    return(pViewMenu);
}   
 
 
void CMainFrame::ViewToolBarShort(void)
{ 
    TRACE("\nCMainFrame::ViewToolBarShort\n");
    
    // We may have gotten here on error, switch to the logo view
    if (theApp.GetCurViewIndex() != 0)
      theApp.SwitchView(0, FALSE);
      
	// ensure the View submenu is clear
	ClearTheViewMenu();

	// Set the toolbar to show only partial commmand list  
	m_wndToolBar.SetButtons(NULL, 4);
	m_wndToolBar.SetButtonInfo(0, ID_FILE_OPEN, TBBS_BUTTON, 0);
	m_wndToolBar.SetButtonInfo(1, ID_PAGER_READ, TBBS_BUTTON, nReadBmpIndex);
	m_wndToolBar.SetButtonInfo(2, ID_SEPARATOR, TBBS_SEPARATOR, 12);
	m_wndToolBar.SetButtonInfo(3, ID_CONTEXT_HELP, TBBS_BUTTON, 15);
	m_wndToolBar.Invalidate();
     
	// if Tuning is allowed and bar is visible, display the tuning toolbar as empty
	if ((m_bEnableTuning == TRUE) && (m_byVisibleToolBars & TBTUNING))  
	{ 
	  m_wndTuningToolBar.SetButtons(NULL, 1);
	  m_wndTuningToolBar.SetButtonInfo(0, ID_SEPARATOR, TBBS_SEPARATOR, 10);
	  m_wndTuningToolBar.Invalidate();
    }
    m_byVisibleToolBars = (BYTE)(TBSHORT | (m_byVisibleToolBars & TBTUNING)); 
}


void CMainFrame::ViewToolBarLong(void)
{ 
    int nPos, lcv, nNumViews;
    
    // if the current view is no longer visible, switch views 
    if (theApp.IsViewPresentable(theApp.GetCurViewIndex()) == FALSE)
      theApp.SwitchView(0, FALSE);
    
    // add the visible views to the View sub-menu
    RefreshViewMenu();
    
    // first, determine the number of visible toolbar view buttons
    nNumViews = 0; 
    for (lcv = 0; lcv < theApp.GetNumViews(); ++lcv)
    {  
       if (theApp.IsViewPresentable(lcv))
         nNumViews++;
    } 
    
    // add the tuning fork if allowed and separate tuning bar not visible
    if ((m_bEnableTuning) && !(m_byVisibleToolBars & TBTUNING))   
      nNumViews++;  
      
    // next, add the non-view button count to the number of views
    // and set the button count to the toolbar
	m_wndToolBar.SetButtons(NULL, (nNumViews + 13));  
    
    // next, begin setting button onto the toolbar 
    nPos = 0;   
	m_wndToolBar.SetButtonInfo(nPos, ID_FILE_OPEN, TBBS_BUTTON, 0);  nPos++;
	m_wndToolBar.SetButtonInfo(nPos, ID_FILE_SAVE, TBBS_BUTTON, 1);  nPos++;
	m_wndToolBar.SetButtonInfo(nPos, ID_FILE_PRINT, TBBS_BUTTON, 2);  nPos++;
	m_wndToolBar.SetButtonInfo(nPos, ID_SEPARATOR, TBBS_SEPARATOR, 12);  nPos++;
	m_wndToolBar.SetButtonInfo(nPos, ID_PAGER_READ, TBBS_BUTTON, nReadBmpIndex);  nPos++;
	m_wndToolBar.SetButtonInfo(nPos, ID_PAGER_WRITE, TBBS_BUTTON, nWriteBmpIndex);  nPos++;
	if ((m_bEnableTuning) && !(m_byVisibleToolBars & TBTUNING))
	{
	  m_wndToolBar.SetButtonInfo(nPos, ID_DEVICE_TUNING, TBBS_BUTTON, 19);  nPos++;
	}
	m_wndToolBar.SetButtonInfo(nPos, ID_SEPARATOR, TBBS_SEPARATOR, 12);  nPos++;
    // set the individual view buttons onto the toolbar
    for (lcv = 0; lcv < theApp.GetNumViews(); ++lcv)
    { 
       if (theApp.IsViewPresentable(lcv)) 
       {
         m_wndToolBar.SetButtonInfo(nPos, theApp.GetViewCommandId(lcv), TBBS_BUTTON,   
                                    theApp.GetViewToolbarBmp(lcv));
         nPos++;
       }
    }
 
    // set the modication recording, stop recording, play, and repeat buttons
	m_wndToolBar.SetButtonInfo(nPos, ID_SEPARATOR, TBBS_SEPARATOR, 12);  nPos++;
	m_wndToolBar.SetButtonInfo(nPos, ID_UTILITY_DELTA_ONOFF, TBBS_BUTTON, 23);  nPos++;
	m_wndToolBar.SetButtonInfo(nPos, ID_UTILITY_DELTA_PLAY, TBBS_BUTTON, 24);  nPos++;
	m_wndToolBar.SetButtonInfo(nPos, ID_UTILITY_DELTA_REPEAT, TBBS_BUTTON, 27);  nPos++;
    
    // set the help button onto the toolbar
	m_wndToolBar.SetButtonInfo(nPos, ID_SEPARATOR, TBBS_SEPARATOR, 12);  nPos++;
	m_wndToolBar.SetButtonInfo(nPos, ID_CONTEXT_HELP, TBBS_BUTTON, 15);  nPos++;
    
    // initiate paint of toolbar
	m_wndToolBar.Invalidate();
    
	// if Tuning is allowed and bar is already visible, display the tuning toolbar
	if ((m_bEnableTuning == TRUE) && (m_byVisibleToolBars & TBTUNING))
	{ 
	  if (!m_wndTuningToolBar.SetButtons(buttons_tuning,
		                           sizeof(buttons_tuning)/sizeof(UINT)))
	  {
		TRACE("CMainFrame::ViewToolBarLong : Failed to display m_wndTuningToolBar\n");
	  } 
	  m_wndTuningToolBar.Invalidate();
    }    

    m_byVisibleToolBars = (BYTE)(TBLONG | (m_byVisibleToolBars & TBTUNING)); 
    return;
}


void CMainFrame::ViewToolBarLabUseOnly(void)
{ 
    TRACE("\nCMainFrame::ViewToolBarLabUseOnly\n");
    
    // We may have gotten here on error, switch to the logo view
    if (theApp.GetCurViewIndex() != 0)
      theApp.SwitchView(0, FALSE);
      
	// ensure the View submenu is clear
	ClearTheViewMenu();

	// Set the toolbar to show only partial commmand list  
	m_wndToolBar.SetButtons(NULL, 11);
	m_wndToolBar.SetButtonInfo(0, ID_FILE_OPEN, TBBS_BUTTON, 0);
	m_wndToolBar.SetButtonInfo(1, ID_FILE_SAVE, TBBS_BUTTON, 1);
	m_wndToolBar.SetButtonInfo(2, ID_SEPARATOR, TBBS_SEPARATOR, 12);
	m_wndToolBar.SetButtonInfo(3, ID_PAGER_READ, TBBS_BUTTON, nReadBmpIndex);
	m_wndToolBar.SetButtonInfo(4, ID_PAGER_WRITE, TBBS_BUTTON, nWriteBmpIndex);
	m_wndToolBar.SetButtonInfo(5, ID_SEPARATOR, TBBS_SEPARATOR, 12);
	m_wndToolBar.SetButtonInfo(6, ID_VIEW_BANNER, TBBS_BUTTON, 5);
	m_wndToolBar.SetButtonInfo(7, ID_VIEW_COMMVIEW, TBBS_BUTTON, 17);
	m_wndToolBar.SetButtonInfo(8, ID_VIEW_HEXEDIT, TBBS_BUTTON, 18);
	m_wndToolBar.SetButtonInfo(9, ID_SEPARATOR, TBBS_SEPARATOR, 12);
	m_wndToolBar.SetButtonInfo(10, ID_CONTEXT_HELP, TBBS_BUTTON, 15);
	m_wndToolBar.Invalidate();
     
	// if Tuning is allowed and bar is visible, display the tuning toolbar as empty
	if ((m_bEnableTuning == TRUE) && (m_byVisibleToolBars & TBTUNING))  
	{ 
	  m_wndTuningToolBar.SetButtons(NULL, 1);
	  m_wndTuningToolBar.SetButtonInfo(0, ID_SEPARATOR, TBBS_SEPARATOR, 10);
	  m_wndTuningToolBar.Invalidate();
    } 
    m_byVisibleToolBars = (BYTE)(TBLABONLY | (m_byVisibleToolBars & TBTUNING)); 
    return; 
}


void CMainFrame::RefreshToolBar(void)
{
  if (m_byVisibleToolBars & TBLABONLY)
    ViewToolBarLabUseOnly();
  else if (m_byVisibleToolBars & TBSHORT)
    ViewToolBarShort();
  else if (m_byVisibleToolBars & TBLONG)
    ViewToolBarLong();
  return;
}

    
    
void CMainFrame::ToggleReadWriteBmps(BOOL bNormal)
{
  // toggle these bitmaps to indicate that resets have been
  // enabled/disabled.  See CPpsDoc::OnToggleResets()...  
  if (bNormal)
  {
    nReadBmpIndex = 3;
    nWriteBmpIndex = 4;
  }
  else
  {
    nReadBmpIndex = 20;
    nWriteBmpIndex = 21;
  }
  return;
}



//////////////////////////////////////////////////////////////////////////
//  RefreshViewMenu()
//
//  This function dynamically constructs the view menu contents.  Each
//  view indicates whether or not to be visible based upon device id,
//  codeplug version or other criteria.  The menu label is retrived
//  from the string table resource.
// 
void CMainFrame::RefreshViewMenu(void)
{
	// Update the view submenu to reflect the visible views
	// associated with this pager/codeplug version type.

	// First, clear the existing View submenu
	CMenu* pViewMenu = NULL;  
    pViewMenu = ClearTheViewMenu();
	ASSERT(pViewMenu != NULL);

    // Next, add all views which declare themselves visible.
    CString szMenuLabel; 
    for (int iPos = 1; iPos < theApp.GetNumViews(); ++iPos)
    {
       if (theApp.IsViewPresentable(iPos)) 
       {
          GetMenuStringFromStringTable(theApp.GetViewCommandId(iPos), szMenuLabel);
          pViewMenu->AppendMenu(MF_STRING, theApp.GetViewCommandId(iPos), (LPCTSTR)szMenuLabel);
       }
    } 
    // initiate paint of modified menu
    DrawMenuBar();
	return;
}

 
 
 
///////////////////////////////////////////////////////////////////////////////
//  GetMenuStringFromStringTable()
//
//  This function will retrieve a menu label for the corresponding nCommandId
//  from the string table.  All View menu command IDs must have a corresponding
//  entry in the string table.
//
void CMainFrame::GetMenuStringFromStringTable(UINT nCommandId, CString &rStr)
{
  // Strip the view menu string from the resource ID strings.  The
  // string must have a 'tool-tip' at the end delimited by an '\n' character.
  // See the string table resources via AppStudio.
  
  // load the resource string
  rStr.LoadString(nCommandId);
  ASSERT(!(rStr.IsEmpty()));
    
  // check for tool-tip at the end of the string
  int nNewLine = rStr.ReverseFind('\n');
  ASSERT(nNewLine != -1);
  
  // do we have a delimiter?
  if(nNewLine != -1)
  {
    // use the tool-tip as the menu label
    rStr = rStr.Right(rStr.GetLength() - nNewLine - 1);      
  }
  return;
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


///////////////////////////////////////////////////////////////////////////
//  OnTimer()
//
//  Message handler for the WM_TIMER message  
//  For CIoOperation derived classes, the nIDEvent is the current IoOpId.
//
void CMainFrame::OnTimer(UINT nIDEvent)
{
    TRACE("\nCMainFrame::OnTimer : nIDEvent=%d\n", nIDEvent);
       
    // An object returns 0L if it handled the event..
    if (theIoController.OnTimer(nIDEvent) != 0L)   
    { 
      TRACE("\nCMainFrame::OnTimer : Unknown Timer : nIDEvent=%d\n", nIDEvent);
      KillTimer(nIDEvent);      // kill unknown timer  
      #ifndef DISABLE_HEXEDIT 
      char szTemp[200];
      sprintf(szTemp, "CMainFrame::OnTimer : Unknown Timer Event : 0x%04X", nIDEvent);
      AfxMessageBox(szTemp); 
      #endif
    }

	CFrameWnd::OnTimer(nIDEvent);
	return;
}


/////////////////////////////////////////////////////////////////////////
//  OnCommNotify()
//
//  Communications event handler 
//   wParam = idDevice
//   LOWORD(lParam) = subset of (CN_EVENT, CN_RECEIVE, or CN_TRANSMIT or
//                               CN_POLL (which is PPS defined))
afx_msg LONG CMainFrame::OnCommNotify(WPARAM wParam, LPARAM lParam)
{ 
  TRACE("\nCMainFrame::OnCommNotify : %04X : %08X\n", wParam, lParam);
  
  if (theIoController.OnCommNotify(wParam, lParam) != 0L)
  { 
    // *** Note:  If the event is unhandled, then compare the LOWORD(lParam) value
    // with the valid event flags.  For serial operations, the value should be one
    // of the flags defined in windows.h (search for EV_RXCHAR).  Some of the events
    // can be ignored (e.g. EV_TXEMPTY). 
    /*
    #ifndef DISABLE_HEXEDIT
    char szTemp[100];
    sprintf(szTemp, "CMainFrame::OnCommNotify : Unhandled Event : wParam=%04X : lParam=%08X", wParam, lParam);
    AfxMessageBox(szTemp); 
    #endif
    */
  } 
  
  return(0L);   // Event handled
}
 

/////////////////////////////////////////////////////////////////////////
//  OnWmIoOperation()
//
//  CIoOperation initiated event handler.  Receives WM_IO_OPERATION messages
//  (see files 'ciooper.h' and 'ciooper.cpp') which indicate that an 
//  CIoOperation is completed.  
//    wParam = nStatus (0=IO_COMPLETED, 1=IO_FAILED, 2=IO_NOOPTIMEOUT)
//	  HIWORD(lParam) = nDeviceId
//    LOWORD(lParam) = CIoOperaiton ID
afx_msg LONG CMainFrame::OnWmIoOperation(WPARAM wParam, LPARAM lParam)
{
  TRACE("\nCMainFrame::OnWmIoOperation : wParam=%04X : lParamHi=%04X : lParamLo=%04X : ComId=%04X\n",
        wParam, HIWORD(lParam), LOWORD(lParam), theSerialPort.GetDeviceId());
  
  if (theIoController.OnIoOpCompletion(wParam, lParam) != 0L)  
  {
    #ifndef DISABLE_HEXEDIT
    AfxMessageBox("CMainFrame::OnWmIoOperation : Unknown IoOperation Event");
    #endif
  }
   
  return(0L);  // event handled
}
  

/////////////////////////////////////////////////////////////////////////
//  OnWmIoOpListDone()
//
//  CIoOperation::OnOpCompletion initiated event handler.  Receives WM_IO_OPLIST_DONE
//  messages.    
//    wParam = nStatus (0=IO_COMPLETED, 1=IO_FAILED)
//    LOWORD(lParam) = Last CIoOperation ID
afx_msg LONG CMainFrame::OnWmIoOpListDone(WPARAM wParam, LPARAM lParam)
{
  TRACE("\nCMainFrame::OnWmIoOpListDone\n");
  
  // notify the CPpsDoc class of the event
  CPpsDoc *pCPpsDoc = (CPpsDoc *)GetActiveDocument();
  ASSERT(pCPpsDoc->IsKindOf(RUNTIME_CLASS(CPpsDoc)));
  pCPpsDoc->OnWmIoOpListDone(wParam, lParam);  

  // Notify the active view   
  CView *pActiveView = GetActiveView();
  ASSERT(pActiveView != NULL);
  
  if (pActiveView)
    pActiveView->PostMessage(WM_IO_OPLIST_DONE, wParam, lParam); 

  return(0L);   // event handled
}
  




////////////////////////////////////////////////////////////////////////////
//  OnWmReadCodeplugDone()
//
//  Message handler for the WM_READ_CP_DONE message.
//    wParam = 0 if successful complete; 1 if failed complete
//    lParam = not used
//  Note:  it is necessary for the WM message to be processed
//  via the CMainFrame class since non-CWnd based classes
//  such as CPpsDoc cannot implement ON_MESSAGE macros in
//  their MESSAGE_MAP structure. 
afx_msg LONG CMainFrame::OnWmReadCodeplugDone(WPARAM wParam, LPARAM lParam)
{ 
  TRACE("\n CMainFrame::OnWmUserReadCodeplugDone : wParam=%04X "
          "lParam=%lX\n", wParam, lParam);
  
  // notify the CPpsDoc class of the event
  CPpsDoc *pCPpsDoc = (CPpsDoc *)GetActiveDocument();
  ASSERT(pCPpsDoc->IsKindOf(RUNTIME_CLASS(CPpsDoc)));

  pCPpsDoc->OnWmReadCodeplugDone(wParam, lParam);  

  return(0L);
}



////////////////////////////////////////////////////////////////////////////
//  OnWmWriteCodeplugDone()
//
//  Message handler for the WM_WRITE_CP_DONE message.
//    wParam = 0 if successful complete; 1 if failed complete
//    lParam = not used
//  Note:  it is necessary for the WM message to be processed
//  via the CMainFrame class since non-CWnd based classes
//  such as CPpsDoc cannot implement ON_MESSAGE macros in
//  their MESSAGE_MAP structure. 
afx_msg LONG CMainFrame::OnWmWriteCodeplugDone(WPARAM wParam, LPARAM lParam)
{
  TRACE("\n CMainFrame::OnWmUserWriteCodeplugDone : wParam=%04X "
          "lParam=%08lX\n", wParam, lParam);
   
  // notify the CPpsDoc class of the event
  CPpsDoc *pCPpsDoc = (CPpsDoc *)GetActiveDocument(); 
  ASSERT(pCPpsDoc->IsKindOf(RUNTIME_CLASS(CPpsDoc)));
  
  pCPpsDoc->OnWmWriteCodeplugDone(wParam, lParam);

  return(0L);
}



afx_msg void CMainFrame::OnUpdateIndicatorPort(CCmdUI * pCmdUI)
{ 
  if (theSerialPort.IsPortConfigured() == FALSE)
  {
     pCmdUI->Enable(FALSE);
     return;
  }
  m_wndStatusBar.SetPaneText(1, theSerialPort.pszGetPortBuildString());
  pCmdUI->Enable(TRUE);
}


afx_msg void CMainFrame::OnUpdateIndicatorIoOp(CCmdUI *pCmdUI)
{
   if (theApp.GetThePager()->IsOperationInProgress() == FALSE)
   {
     pCmdUI->Enable(FALSE);
     return;
   } 
   m_wndStatusBar.SetPaneText(2, theApp.GetThePager()->GetCurrentOpString());
   pCmdUI->Enable(TRUE);
}


static char *pszProgress = "||||||||||";
static char pszCurProgress[11];                 
afx_msg void CMainFrame::OnUpdateIndicatorProgress(CCmdUI *pCmdUI)
{  
   if (theApp.GetThePager()->IsOperationInProgress() == FALSE)
   {
     pCmdUI->Enable(FALSE);
     return;
   }
   
   int nPercent = theApp.GetThePager()->GetPercentComplete(); 
   
   if ((nPercent >= 0) && (nPercent <= 100))
   { 
     strncpy(pszCurProgress, pszProgress, (nPercent/10));
     pszCurProgress[(nPercent/10)+1] = '\0';
     m_wndStatusBar.SetPaneText(3, pszCurProgress);
   }
   pCmdUI->Enable(TRUE);
}


                 

void CMainFrame::OnPagerPasswd()
{
  CDwnLdPwd dlg;
  int ret;
  
  do 
  {
    ret = dlg.DoModal();
    if (ret == IDCANCEL)
      break;
    TRACE("\nCMainFrame::OnPagerPasswd : %s : %s\n",
          (LPCTSTR)dlg.m_Edit1, (LPCTSTR)dlg.m_Edit2);
    if (dlg.m_Edit1 != dlg.m_Edit2)
    {
      AfxMessageBox("The two password entries must compare exactly.", 
      			    MB_ICONINFORMATION | MB_APPLMODAL | MB_OK);      
      continue;
    } 
    if (dlg.m_Edit1.GetLength() > 0)
      theApp.GetThePager()->SetPagerPassword((LPCTSTR)dlg.m_Edit1);
    break;
  }
  while(1); 
  return;  	
}        



void CMainFrame::OnUpdatePagerPasswd(CCmdUI* pCmdUI)
{ 
  #ifdef MTEL_AFFILIATE_PPS
    pCmdUI->Enable(FALSE); // always disabled for this PPS version
  #else
    pCmdUI->Enable(!theApp.GetThePager()->IsOperationInProgress());
  #endif
}



//////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////
//  Tx Tuning Command Handlers
//
//  The tuning elements (toolbar and dialog window) work with the
//  CPegasus pager class.  If another pager is to be supported by
//  the PPS, and the tuning elements differ, then changes must be
//  made herein to invoke the appropriate tuning elements.
//
void CMainFrame::OnDeviceTuning()
{
  if (m_bEnableTuning == FALSE)
    return;
  
  // display the tuning toolbar for subsequent tuning short-cuts.
  if (!(m_byVisibleToolBars & TBTUNING))  
  {
    // create the tuning toolbar 
    if (!m_wndTuningToolBar.Create(this, WS_CHILD|WS_VISIBLE|CBRS_TOP) ||
    	!m_wndTuningToolBar.LoadBitmap(IDR_TUNING_TOOLS)) 
    {
  	  TRACE("Failed to create tuning toolbar\n");
		return;       // fail to create
	}                           
	// display the tuning toolbar
    if (!m_wndTuningToolBar.SetButtons(buttons_tuning,
  	                                   sizeof(buttons_tuning)/sizeof(UINT)))
	{
	  TRACE("Failed to display m_wndTuningToolBar\n");
	  return;
	}
	RecalcLayout();
	CPpsFormView *pView = (CPpsFormView *)GetActiveView();
	ASSERT(pView); 
	ASSERT(pView->IsKindOf(RUNTIME_CLASS(CPpsFormView)));
	pView->ResizeParentToFit(FALSE);  // adjust frame size to fit view
    m_byVisibleToolBars |= TBTUNING; 
    ViewToolBarLong();
  }
  
  return;
}

void CMainFrame::OnUpdateDeviceTuning(CCmdUI* pCmdUI)
{
  OnUpdatePagerTuning(pCmdUI);
}



void CMainFrame::OnPagerTuning(BYTE byTestId)
{
  #ifndef DISABLE_TUNING    // conditional compilation of tuning dialog
    CTheTuningDlg dlg;
    int ret;
  
    // The CTheTuningDlg is designed for the CPegasus class of
    // pagers.  Changes may be required, or a new tuning dialog
    // created, for new pager classes.
    ASSERT(theApp.GetThePager()->IsKindOf(RUNTIME_CLASS(CPegasus)));
    ret = dlg.DoModal(byTestId); 
  #endif
  return;	
}


void CMainFrame::OnUpdatePagerTuning(CCmdUI* pCmdUI)
{
  CPpsDoc *pCPpsDoc = (CPpsDoc *)GetActiveDocument(); 
  ASSERT(pCPpsDoc->IsKindOf(RUNTIME_CLASS(CPpsDoc)));

  if ((theApp.GetThePager()->IsOperationInProgress()) ||
      (!pCPpsDoc->GetDocSize()) ||
      (m_bEnableTuning == FALSE))
     pCmdUI->Enable(FALSE);
  else
     pCmdUI->Enable(TRUE);	
  return;	
}

void CMainFrame::OnTxtuneScf()
{
  OnPagerTuning(SILENT_CARRIER);
}

void CMainFrame::OnUpdateTxtuneScf(CCmdUI* pCmdUI)
{
  OnUpdatePagerTuning(pCmdUI);
}


void CMainFrame::OnTxtuneModbalHigh()
{
  OnPagerTuning(MODBALHIGH);
}

void CMainFrame::OnUpdateTxtuneModbalHigh(CCmdUI* pCmdUI)
{
  OnUpdatePagerTuning(pCmdUI);
}


void CMainFrame::OnTxtuneModbalLow()
{
  OnPagerTuning(MODBALLOW);
}

void CMainFrame::OnUpdateTxtuneModbalLow(CCmdUI* pCmdUI)
{
  OnUpdatePagerTuning(pCmdUI);
}


void CMainFrame::OnTxTuneSymbolDev()
{
  OnPagerTuning(TX_SPLT);
}

void CMainFrame::OnUpdateTxTuneSymbolDev(CCmdUI* pCmdUI)
{
  OnUpdatePagerTuning(pCmdUI);
}


void CMainFrame::OnRxtuneRefosc()
{
  OnPagerTuning(REF_OSC);
}

void CMainFrame::OnUpdateRxtuneRefosc(CCmdUI* pCmdUI)
{
  OnUpdatePagerTuning(pCmdUI);
}


void CMainFrame::OnRxtuneRcvronTest()
{
  OnPagerTuning(RCVR_ON);
}

void CMainFrame::OnUpdateRxtuneRcvronTest(CCmdUI* pCmdUI)
{
  OnUpdatePagerTuning(pCmdUI);	
}


void CMainFrame::OnTxtuneTxonTest()
{
  OnPagerTuning(TX_ON);
}

void CMainFrame::OnUpdateTxtuneTxonTest(CCmdUI* pCmdUI)
{
  OnUpdatePagerTuning(pCmdUI);
}



void CMainFrame::OnPagerSwReset()
{ 
  if (theApp.GetThePager()->DoSoftwareReset())
    AfxMessageBox ("Please wait until current operation is complete.\n",
                    MB_OK | MB_ICONHAND); 
}

void CMainFrame::OnUpdatePagerSwReset(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(!theApp.GetThePager()->IsOperationInProgress());
}

void CMainFrame::OnPagerHwReset()
{
  if (theApp.GetThePager()->DoHardwareReset())
    AfxMessageBox ("Please wait until current operation is complete.\n",
                    MB_OK | MB_ICONHAND); 
}

void CMainFrame::OnUpdatePagerHwReset(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(!theApp.GetThePager()->IsOperationInProgress());
}



///////////////////////////////////////////////////////////////////////////////
//  OnView... command handlers
//
//  Initiates a replacement of the currently active view class.  See 
//  file pps.cpp for implementation details.  Also read the notes above. 
//

void CMainFrame::OnViewPagerid()
{ 
  theApp.SwitchViewOnCommand(ID_VIEW_PAGERID);		// display the pager identification view
}

void CMainFrame::OnUpdateViewPagerid(CCmdUI* pCmdUI)
{
  CPpsDoc *pCPpsDoc = (CPpsDoc *)GetActiveDocument(); 
  ASSERT(pCPpsDoc->IsKindOf(RUNTIME_CLASS(CPpsDoc)));
  
  pCmdUI->SetCheck(theApp.GetCurViewCommandId() == ID_VIEW_PAGERID);
  pCmdUI->Enable(pCPpsDoc->GetDocSize() > 0);  
}



void CMainFrame::OnViewSecurity()
{
  theApp.SwitchViewOnCommand(ID_VIEW_SECURITY);		// display the pager passwords view
}

void CMainFrame::OnUpdateViewSecurity(CCmdUI* pCmdUI)
{
  CPpsDoc *pCPpsDoc = (CPpsDoc *)GetActiveDocument(); 
  ASSERT(pCPpsDoc->IsKindOf(RUNTIME_CLASS(CPpsDoc)));
  
  pCmdUI->SetCheck(theApp.GetCurViewCommandId() == ID_VIEW_SECURITY);
  pCmdUI->Enable(pCPpsDoc->GetDocSize() > 0);  
}


void CMainFrame::OnViewAddresses()
{
  theApp.SwitchViewOnCommand(ID_VIEW_ADDRESSES);      // addess configuration view
}

void CMainFrame::OnUpdateViewAddresses(CCmdUI* pCmdUI)
{
  CPpsDoc *pCPpsDoc = (CPpsDoc *)GetActiveDocument(); 
  ASSERT(pCPpsDoc->IsKindOf(RUNTIME_CLASS(CPpsDoc)));
  
  pCmdUI->SetCheck(theApp.GetCurViewCommandId() == ID_VIEW_ADDRESSES);
  pCmdUI->Enable(pCPpsDoc->GetDocSize() > 0);  
}


void CMainFrame::OnViewProtocol()
{
  theApp.SwitchViewOnCommand(ID_VIEW_PROTOCOL);  	 // protocol view
}

void CMainFrame::OnUpdateViewProtocol(CCmdUI* pCmdUI)
{
  CPpsDoc *pCPpsDoc = (CPpsDoc *)GetActiveDocument(); 
  ASSERT(pCPpsDoc->IsKindOf(RUNTIME_CLASS(CPpsDoc)));

  pCmdUI->SetCheck(theApp.GetCurViewCommandId() == ID_VIEW_PROTOCOL);
  pCmdUI->Enable(pCPpsDoc->GetDocSize() > 0);  
}


void CMainFrame::OnViewMessages()
{
  theApp.SwitchViewOnCommand(ID_VIEW_MESSAGES);		// message processing view	
}

void CMainFrame::OnUpdateViewMessages(CCmdUI* pCmdUI)
{
  CPpsDoc *pCPpsDoc = (CPpsDoc *)GetActiveDocument(); 
  ASSERT(pCPpsDoc->IsKindOf(RUNTIME_CLASS(CPpsDoc)));
  
  pCmdUI->SetCheck(theApp.GetCurViewCommandId() == ID_VIEW_MESSAGES);
  pCmdUI->Enable(pCPpsDoc->GetDocSize() > 0);  
}


void CMainFrame::OnViewAlerts()
{
  theApp.SwitchViewOnCommand(ID_VIEW_ALERTS);		// alerts configuration view	
}

void CMainFrame::OnUpdateViewAlerts(CCmdUI* pCmdUI)
{
  CPpsDoc *pCPpsDoc = (CPpsDoc *)GetActiveDocument(); 
  ASSERT(pCPpsDoc->IsKindOf(RUNTIME_CLASS(CPpsDoc)));
  
  pCmdUI->SetCheck(theApp.GetCurViewCommandId() == ID_VIEW_ALERTS);
  pCmdUI->Enable(pCPpsDoc->GetDocSize() > 0);  
}


void CMainFrame::OnViewOptions()
{
  theApp.SwitchViewOnCommand(ID_VIEW_OPTIONS);		// misc options view	
}

void CMainFrame::OnUpdateViewOptions(CCmdUI* pCmdUI)
{
  CPpsDoc *pCPpsDoc = (CPpsDoc *)GetActiveDocument(); 
  ASSERT(pCPpsDoc->IsKindOf(RUNTIME_CLASS(CPpsDoc)));
  
  pCmdUI->SetCheck(theApp.GetCurViewCommandId() == ID_VIEW_OPTIONS);
  pCmdUI->Enable(pCPpsDoc->GetDocSize() > 0);  
}


void CMainFrame::OnViewBanner()
{
  theApp.SwitchViewOnCommand(ID_VIEW_BANNER);		// initial view	
}

void CMainFrame::OnUpdateViewBanner(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(theApp.GetCurViewCommandId() == ID_VIEW_BANNER);
}



void CMainFrame::OnViewHexedit()
{
  #ifndef DISABLE_HEXEDIT
  CPpsDoc *pCPpsDoc = (CPpsDoc *)GetActiveDocument(); 
  ASSERT(pCPpsDoc->IsKindOf(RUNTIME_CLASS(CPpsDoc)));
  
  if (m_bEnableHexEdit == FALSE)
    return;
  if (pCPpsDoc->GetDocSize() > 0)
    theApp.SwitchViewOnCommand(ID_VIEW_HEXEDIT);		// hex editor view
  #endif	
}

void CMainFrame::OnUpdateViewHexedit(CCmdUI* pCmdUI)
{ 
  #ifndef DISABLE_HEXEDIT
  CPpsDoc *pCPpsDoc = (CPpsDoc *)GetActiveDocument(); 
  ASSERT(pCPpsDoc->IsKindOf(RUNTIME_CLASS(CPpsDoc)));
  
  pCmdUI->SetCheck(theApp.GetCurViewCommandId() == ID_VIEW_HEXEDIT);
  pCmdUI->Enable((pCPpsDoc->GetDocSize() > 0) && (m_bEnableHexEdit));
  #endif  
}


void CMainFrame::OnViewPclink()
{
  theApp.SwitchViewOnCommand(ID_VIEW_PCLINK);		// PC link view	
}

void CMainFrame::OnUpdateViewPclink(CCmdUI* pCmdUI)
{
  CPpsDoc *pCPpsDoc = (CPpsDoc *)GetActiveDocument(); 
  ASSERT(pCPpsDoc->IsKindOf(RUNTIME_CLASS(CPpsDoc)));
  
  pCmdUI->SetCheck(theApp.GetCurViewCommandId() == ID_VIEW_PCLINK);
  pCmdUI->Enable(pCPpsDoc->GetDocSize() > 0);  
}


void CMainFrame::OnViewRegistration()
{
  theApp.SwitchViewOnCommand(ID_VIEW_REGISTRATION); 	// Reflex50 Registration view
}

void CMainFrame::OnUpdateViewRegistration(CCmdUI* pCmdUI)
{
  CPpsDoc *pCPpsDoc = (CPpsDoc *)GetActiveDocument(); 
  ASSERT(pCPpsDoc->IsKindOf(RUNTIME_CLASS(CPpsDoc)));

  pCmdUI->SetCheck(theApp.GetCurViewCommandId() == ID_VIEW_REGISTRATION);
  pCmdUI->Enable((pCPpsDoc->GetDocSize() > 0));   
}



void CMainFrame::OnViewCannedmsgs()
{
  theApp.SwitchViewOnCommand(ID_VIEW_CANNEDMSGS);	// canned msgs configuration	
}

void CMainFrame::OnUpdateViewCannedmsgs(CCmdUI* pCmdUI)
{
  CPpsDoc *pCPpsDoc = (CPpsDoc *)GetActiveDocument(); 
  ASSERT(pCPpsDoc->IsKindOf(RUNTIME_CLASS(CPpsDoc)));
  
  pCmdUI->SetCheck(theApp.GetCurViewCommandId() == ID_VIEW_CANNEDMSGS);
  pCmdUI->Enable(pCPpsDoc->GetDocSize() > 0);  
}



void CMainFrame::OnViewCommview()
{ 
  #ifndef DISABLE_HEXEDIT
  if (!IsAccessRestricted())
    theApp.SwitchViewOnCommand(ID_VIEW_COMMVIEW);   // comm viewer view
  #endif	
}

void CMainFrame::OnUpdateViewCommview(CCmdUI* pCmdUI)
{ 
  #ifndef DISABLE_HEXEDIT
  pCmdUI->SetCheck(theApp.GetCurViewCommandId() == ID_VIEW_COMMVIEW);
  pCmdUI->Enable(!IsAccessRestricted());
  #endif  
}

///////////////////////////////////////////////////////////////




void CMainFrame::OnClose()
{ 
  theApp.GetThePager()->DisConnect();
  CFrameWnd::OnClose();
}

   

void CMainFrame::OnEnterIdle(UINT nWhy, CWnd* pWho)
{
  // Default Handling
  CFrameWnd::OnEnterIdle(nWhy, pWho);
	
  // Custom handling to update the status bar IoOperation and Progress 
  // indicators when a Dialog Box is active...	
  if (nWhy != MSGF_DIALOGBOX)
    return;
    
  UINT nIdOp, nStyleOp, nIdProgress, nStyleProgress;
  int  nWidthOp, nWidthProgress;
  
  nIdOp = m_wndStatusBar.GetItemID(2);
  nIdProgress = m_wndStatusBar.GetItemID(3);
  m_wndStatusBar.GetPaneInfo(2, nIdOp, nStyleOp, nWidthOp);
  m_wndStatusBar.GetPaneInfo(3, nIdProgress, nStyleProgress, nWidthProgress);
    
  if (theApp.GetThePager()->IsOperationInProgress() == FALSE)
  {
    nStyleOp |= SBPS_DISABLED;
    nStyleProgress |= SBPS_DISABLED; 
    m_wndStatusBar.SetPaneInfo(2, nIdOp, nStyleOp, nWidthOp);
    m_wndStatusBar.SetPaneInfo(3, nIdProgress, nStyleProgress, nWidthProgress);  
    return;
  }
  
  m_wndStatusBar.SetPaneText(2, theApp.GetThePager()->GetCurrentOpString());

  int nPercent = theApp.GetThePager()->GetPercentComplete();   
  if ((nPercent >= 0) && (nPercent <= 100))
  { 
    strncpy(pszCurProgress, pszProgress, (nPercent/10));
    pszCurProgress[(nPercent/10)+1] = '\0';
    m_wndStatusBar.SetPaneText(3, pszCurProgress);
  }  

  nStyleOp &= (~(SBPS_DISABLED));
  nStyleProgress &= (~(SBPS_DISABLED)); 
  m_wndStatusBar.SetPaneInfo(2, nIdOp, nStyleOp, nWidthOp);
  m_wndStatusBar.SetPaneInfo(3, nIdProgress, nStyleProgress, nWidthProgress);
  return;  
}


////////////////////////////////////////////////////////////////////
//  OnViewNext()
//
//  Short-cut to view switching.  User presses CTRL+RIGHT_ARROW
//
void CMainFrame::OnViewNext()
{  
   CPpsDoc *pDoc = (CPpsDoc *)GetActiveDocument(); 
   ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(CPpsDoc)));  
   if ((pDoc->GetDocSize() <= 0) || (!(m_byVisibleToolBars & TBLONG)))
     return;  

   int lcv;
   int nCurIndex = theApp.GetCurViewIndex();
   int nNumViews = theApp.GetNumViews();
   
   for (lcv = (nCurIndex+1); lcv < nNumViews; ++lcv)
   {
     if (theApp.IsViewPresentable(lcv))
       break;
   }
   if (lcv >= nNumViews) 
     theApp.SwitchView(0);
   else if (lcv != nCurIndex)
     theApp.SwitchView(lcv);
   return; 
}


////////////////////////////////////////////////////////////////////
//  OnViewPrevious()
//
//  Short-cut to view switching.  User presses CTRL+LEFT_ARROW
//
void CMainFrame::OnViewPrevious()
{
   CPpsDoc *pDoc = (CPpsDoc *)GetActiveDocument(); 
   ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(CPpsDoc)));  
   if ((pDoc->GetDocSize() <= 0) || (!(m_byVisibleToolBars & TBLONG)))
     return;  

   int lcv;
   int nCurIndex = theApp.GetCurViewIndex();
   int nNumViews = theApp.GetNumViews();
   
   for (lcv = (nCurIndex-1); lcv > 0; --lcv)
   {
     if (theApp.IsViewPresentable(lcv))
       break;
   } 
   
   if ((lcv > 0) && (lcv != nCurIndex))
     theApp.SwitchView(lcv);
   else if ((lcv == 0) && (nCurIndex != 0))
     theApp.SwitchView(0);
   else
   {
     for (lcv = (nNumViews-1); lcv > nCurIndex; --lcv)
     {
       if (theApp.IsViewPresentable(lcv))
         break;
     }
     if (lcv != nCurIndex)
       theApp.SwitchView(lcv);
   }
   
   return;	
}


/////////////////////////////////////////////////////////////////////////////
//  ToggleStatusIndicators()
//
//  This function may be used to toggle the appearance of the status bar
//  panes from sunken (default) to popped-out. 
//
void CMainFrame::ToggleStatusIndicators(void)
{   
  UINT nIdCommo, nStyleCommo;
  UINT nIdOp, nStyleOp;
  UINT nIdProgress, nStyleProgress;
  int  nWidthCommo, nWidthOp, nWidthProgress;
  
  nIdCommo = m_wndStatusBar.GetItemID(1);
  nIdOp = m_wndStatusBar.GetItemID(2);
  nIdProgress = m_wndStatusBar.GetItemID(3);
  m_wndStatusBar.GetPaneInfo(1, nIdCommo, nStyleCommo, nWidthCommo);
  m_wndStatusBar.GetPaneInfo(2, nIdOp, nStyleOp, nWidthOp);
  m_wndStatusBar.GetPaneInfo(3, nIdProgress, nStyleProgress, nWidthProgress);
   
  if (nStyleOp & SBPS_POPOUT)
  {
    nStyleCommo &= (~(SBPS_POPOUT));  
    nStyleOp &= (~(SBPS_POPOUT));
    nStyleProgress &= (~(SBPS_POPOUT)); 
  }
  else
  { 
    nStyleCommo |= SBPS_POPOUT; 
    nStyleOp |= SBPS_POPOUT;
    nStyleProgress |= SBPS_POPOUT; 
  }
  
  m_wndStatusBar.SetPaneInfo(1, nIdCommo, nStyleCommo, nWidthCommo);
  m_wndStatusBar.SetPaneInfo(2, nIdOp, nStyleOp, nWidthOp);
  m_wndStatusBar.SetPaneInfo(3, nIdProgress, nStyleProgress, nWidthProgress);
  return;  
}


//////////////////////////////////////////////////////////////////////
//  OnDeviceComsettings()
//
void CMainFrame::OnDeviceComsettings()
{ 
  theSerialPort.OnComPortSettings();
}

void CMainFrame::OnUpdateDeviceComsettings(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(!theApp.GetThePager()->IsOperationInProgress());
}


///////////////////////////////////////////////////////////////////////


