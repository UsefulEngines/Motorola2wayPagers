//*************************************************************
//  History:    Date       Author               Comment
//              3/96       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
// pps.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"

#include "pps.h"
#include "mainfrm.h"
#include "ppsdoc.h"
#include "ppsview.h"
#include "w16comm\w16comm.h" 
#include "cpager\pegasus.h" 
#include "cpager\codeplug.h"
#include "cpager\ciooper.h"
#include "w16gpib\w16gpib.h"
#include "dialogs\tuning\hpmda.h"
#include "views\hexedit\hexedit.h"
#include "views\pagerid\pagerid.h"
#include "views\address\address.h"
#include "views\address\r25addr.h"
#include "views\security\security.h"
#include "views\protocol\protocol.h"
#include "views\proto50\proto50.h"
#include "views\messages\messages.h"
#include "views\alerts\alerts.h"
#include "views\options\options.h"
#include "views\r50reg\r50reg.h"
#include "views\canmsgs\canmsgs.h" 
#include "views\commview\commview.h"
#include "views\protocol\proto25.h"
#include "views\options\opts25.h" 
// ...
// ...
// ...
// *** ADD NEW VIEW HEADER FILE INCLUDES HERE.


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CPpsApp

BEGIN_MESSAGE_MAP(CPpsApp, CWinApp)
	//{{AFX_MSG_MAP(CPpsApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_UTILITY_ALLDEFAULT, OnUtilitySetAllDefault)
	ON_UPDATE_COMMAND_UI(ID_UTILITY_ALLDEFAULT, OnUpdateUtilitySetAllDefault)
	ON_COMMAND(ID_SETALL_DEFAULT, OnSetAllDefault)
	ON_COMMAND(ID_UTILITY_SETALLARCHIVE, OnUtilitySetAllArchive)
	ON_UPDATE_COMMAND_UI(ID_UTILITY_SETALLARCHIVE, OnUpdateUtilitySetAllArchive)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// Globally scoped static objects 

/////////////////////////////////////////////////////////////////////////////
// The one and only CPpsApp CWinApp object
//
CPpsApp theApp; 


// this single instance of the CIoController class encapsulates the
// hardware input/output operations of the application.  See files
// 'ciooper.h', 'ciooper.cpp'.
CIoController theIoController;

// This single static instance of the CWin16Comm class encapsulates the 
// Win3.1 communications API.  The class declaration and definition are
// located in files 'w16comm.h' and 'w16comm.cpp'.  The CMainFrame window
// class manages events from the comm port since it is the only window
// class guaranteed to be instantiated for the duration of the program.
// That is, a CWND handle must be associated with the enabled comm port. 
CWin16Comm theSerialPort;        // default to "com1:9600,n,8,1"


// Static instances of the CPager derived classes encapsulates the
// application-pager interface protocol.  The base class declaration and
// definition are located in files 'cpager.h' and 'cpager.cpp'.  The
// other files in that directory (e.g. pegasus.h/.cpp) contain CPager
// derived class declarations and definitions.  
CPegasus thePegasusPager;  
//CVail theVailPager;
/*** ADD NEW CPAGER DERIVED OBJECTS HERE **/ 

#ifndef DISABLE_TUNING
// single static instance of the CWin16Gpib class encapsulates the 
// gpib.dll communications API.  The class declaration and definition
// located in files 'w16gpib.h' and 'w16gpib.cpp'.  See the declaration
// and definition of the CHpMda class in 'hpmda.h' and 'hpmda.cpp'. 
  CWin16Gpib theGpibPort;
  CHpMda	 theHpMda; 
#endif 

  

////////////////////////////////////////////////////////////////////////////
//  The views associated with the Pegasus/Centaur/PageFinder pager.
//	
//  Each device type or class may have a corresponding ViewInfo array.
//  The m_pViewInfo pointer allows these arrays to be selected when the
//  pager is read or when a configuration file is opened. 
//
//  Note:  You may with to use the IsPresentable() view class member 
//  function mechanism to selectively disable selection of a view
//  within a view set at run-time.  For example, the Reflex25 and Reflex50
//  protocol configuration view classes are both a member of the 
//  PegasusViews view set, but one is always determined to be not
//  presentable based upon run-time criteria.  The determination
//  is made by the view class upon a call to IsPresentable().  Such
//  use of InPresentable allows for fewer view sets to be defined.
//
//
static ViewInfo PegasusViews[] =   // older units Reflex25/50 Mix
{   
  { RUNTIME_CLASS(CPpsView), 		ID_VIEW_BANNER, 	5, &CPpsFormView::IsPresentable }, 
  { RUNTIME_CLASS(CPagerId), 		ID_VIEW_PAGERID, 	6, &CPpsFormView::IsPresentable }, 
  { RUNTIME_CLASS(CSecurity),		ID_VIEW_SECURITY, 	7, &CPpsFormView::IsPresentable }, 
  { RUNTIME_CLASS(CAddressInfo), 	ID_VIEW_ADDRESSES, 	8, &CPpsFormView::IsPresentable }, 
  { RUNTIME_CLASS(CReflex25Protocol), ID_VIEW_PROTOCOL, 9, &CReflex25Protocol::IsPresentable }, 
  { RUNTIME_CLASS(CReflex50Protocol), ID_VIEW_PROTOCOL, 9, &CReflex50Protocol::IsPresentable }, 
  { RUNTIME_CLASS(CReflex50Registration), ID_VIEW_REGISTRATION, 10, &CReflex50Registration::IsPresentable }, 
  { RUNTIME_CLASS(CMessages), 		ID_VIEW_MESSAGES, 	11, &CPpsFormView::IsPresentable }, 
  { RUNTIME_CLASS(CCannedMessages), ID_VIEW_CANNEDMSGS, 12, &CCannedMessages::IsPresentable }, 
  { RUNTIME_CLASS(CAlerts), 		ID_VIEW_ALERTS, 	13, &CPpsFormView::IsPresentable }, 
  { RUNTIME_CLASS(COptions), 		ID_VIEW_OPTIONS, 	14, &CPpsFormView::IsPresentable }, 
  #ifndef DISABLE_HEXEDIT
  { RUNTIME_CLASS(CCommViewer), 	ID_VIEW_COMMVIEW, 	17, &CCommViewer::IsPresentable }, 
  { RUNTIME_CLASS(CHexEdit), 		ID_VIEW_HEXEDIT, 	18, &CHexEdit::IsPresentable },  
  #endif // DISABLE_HEXEDIT
};



// *** Views for Reflex25 PageFinder ***
//
static ViewInfo R50PageFinderViews[] =
{   
  { RUNTIME_CLASS(CPpsView), 		ID_VIEW_BANNER, 	5, &CPpsFormView::IsPresentable }, 
  { RUNTIME_CLASS(CPagerId), 		ID_VIEW_PAGERID, 	6, &CPpsFormView::IsPresentable }, 
  #ifndef MTEL_AFFILIATE_PPS
  { RUNTIME_CLASS(CSecurity),		ID_VIEW_SECURITY, 	7, &CPpsFormView::IsPresentable }, 
  #endif // MTEL_AFFILIATE_PPS
  { RUNTIME_CLASS(CAddressInfo), 	ID_VIEW_ADDRESSES, 	8, &CPpsFormView::IsPresentable }, 
  { RUNTIME_CLASS(CReflex50Protocol), ID_VIEW_PROTOCOL, 9, &CReflex50Protocol::IsPresentable }, 
  { RUNTIME_CLASS(CReflex50Registration), ID_VIEW_REGISTRATION, 10, &CReflex50Registration::IsPresentable }, 
  { RUNTIME_CLASS(CMessages), 		ID_VIEW_MESSAGES, 	11, &CPpsFormView::IsPresentable }, 
  { RUNTIME_CLASS(CCannedMessages), ID_VIEW_CANNEDMSGS, 12, &CCannedMessages::IsPresentable }, 
  { RUNTIME_CLASS(CAlerts), 		ID_VIEW_ALERTS, 	13, &CPpsFormView::IsPresentable }, 
  { RUNTIME_CLASS(COptions), 		ID_VIEW_OPTIONS, 	14, &CPpsFormView::IsPresentable }, 
  #ifndef DISABLE_HEXEDIT
  { RUNTIME_CLASS(CCommViewer), 	ID_VIEW_COMMVIEW, 	17, &CCommViewer::IsPresentable }, 
  { RUNTIME_CLASS(CHexEdit), 		ID_VIEW_HEXEDIT, 	18, &CHexEdit::IsPresentable },  
  #endif // DISABLE_HEXEDIT
};



// *** Views for Reflex25 PageFinder with CP_Version >= 0x80 ***
//
static ViewInfo R25PageFinderViews[] = 
{   
  { RUNTIME_CLASS(CPpsView), 		ID_VIEW_BANNER, 	5, &CPpsFormView::IsPresentable }, 
  { RUNTIME_CLASS(CPagerId), 		ID_VIEW_PAGERID, 	6, &CPpsFormView::IsPresentable }, 
  { RUNTIME_CLASS(CSecurity),		ID_VIEW_SECURITY, 	7, &CPpsFormView::IsPresentable }, 
  { RUNTIME_CLASS(CR25Address),     ID_VIEW_ADDRESSES, 	8, &CPpsFormView::IsPresentable }, 
  { RUNTIME_CLASS(CProto25View),    ID_VIEW_PROTOCOL,    9, &CPpsFormView::IsPresentable }, 
  { RUNTIME_CLASS(CMessages), 		ID_VIEW_MESSAGES, 	11, &CPpsFormView::IsPresentable }, 
  { RUNTIME_CLASS(CCannedMessages), ID_VIEW_CANNEDMSGS, 12, &CCannedMessages::IsPresentable }, 
  { RUNTIME_CLASS(CAlerts), 		ID_VIEW_ALERTS, 	13, &CPpsFormView::IsPresentable }, 
  { RUNTIME_CLASS(COptionsR25), 	ID_VIEW_OPTIONS, 	14, &CPpsFormView::IsPresentable }, 
  #ifndef DISABLE_HEXEDIT
  { RUNTIME_CLASS(CCommViewer), 	ID_VIEW_COMMVIEW, 	17, &CCommViewer::IsPresentable }, 
  { RUNTIME_CLASS(CHexEdit), 		ID_VIEW_HEXEDIT, 	18, &CHexEdit::IsPresentable },  
  #endif // DISABLE_HEXEDIT
};



//  *** ADD NEW DEVICE DEPENDENT VIEWINFO STRUCT ARRAYS HERE ...



/////////////////////////////////////////////////////////////////////////////////
//  The pagers supported by this application.
//
//  The following structure array serves to list all devices supported by
//  this application.  The PagerId #defines are coded in file "codeplug.h".
//  Set the pre-processor flags as indicated to build the PPS with restricted
//  support.  See file PPS_VOB/releas16/releas16.txt for PPS release
//  building procedure details.

#define R25	FALSE
#define R50	TRUE

static PagerInfo SupportedPagers[] = 
{ 
#ifdef MTEL_AFFILIATE_PPS
  { RUNTIME_CLASS(CPegasus), PAGEFINDER,  0x07, R50, "R508207.S19" }, 
  { RUNTIME_CLASS(CPegasus), PAGEFINDER,  0x08, R50, "R508208.S19" }, 
  { RUNTIME_CLASS(CPegasus), PAGEFINDER,  0x09, R50, "R508209.S19" },
  { RUNTIME_CLASS(CPegasus), PAGEFINDER,  0x0A, R50, "R50820A.S19" },
  { RUNTIME_CLASS(CPegasus), PAGEFINDER,  0x0B, R50, "R50820B.S19" }, 
  { RUNTIME_CLASS(CPegasus), PAGEFINDER,  0x0C, R50, "R50820C.S19" }, 
  { RUNTIME_CLASS(CPegasus), ASPEN,       0x09, R50, "R508209.S19" },
  { RUNTIME_CLASS(CPegasus), EXPANDAUR,   0x01, R50, "R508501.S19" },
  { RUNTIME_CLASS(CPegasus), LYNX,   	  0x01, R50, "R508601.S19" }, 
// 
#elif REFLEX50_ONLY
  { RUNTIME_CLASS(CPegasus), PEGASUS,     0x02, R50, "R50V48.S19" }, 
  { RUNTIME_CLASS(CPegasus), PEGASUS,     0x03, R50, "R50V411.S19" }, 
  { RUNTIME_CLASS(CPegasus), PEGASUS,     0x04, R50, "R50V412.S19" }, 
  { RUNTIME_CLASS(CPegasus), PEGASUS,     0x05, R50, "R50V412.S19" }, 
  { RUNTIME_CLASS(CPegasus), CENTAUR, 	  0x05, R50, "R50V412.S19" }, 
  { RUNTIME_CLASS(CPegasus), PAGEFINDER,  0x05, R50, "R508205.S19" }, 
  { RUNTIME_CLASS(CPegasus), PAGEFINDER,  0x06, R50, "R508206.S19" }, 
  { RUNTIME_CLASS(CPegasus), PAGEFINDER,  0x07, R50, "R508207.S19" }, 
  { RUNTIME_CLASS(CPegasus), PAGEFINDER,  0x08, R50, "R508208.S19" }, 
  { RUNTIME_CLASS(CPegasus), PAGEFINDER,  0x09, R50, "R508209.S19" },
  { RUNTIME_CLASS(CPegasus), PAGEFINDER,  0x0A, R50, "R50820A.S19" },
  { RUNTIME_CLASS(CPegasus), PAGEFINDER,  0x0B, R50, "R50820B.S19" }, 
  { RUNTIME_CLASS(CPegasus), PAGEFINDER,  0x0C, R50, "R50820C.S19" }, 
  { RUNTIME_CLASS(CPegasus), ASPEN,       0x09, R50, "R508209.S19" },
  { RUNTIME_CLASS(CPegasus), EXPANDAUR,   0x01, R50, "R508501.S19" }, 
  { RUNTIME_CLASS(CPegasus), LYNX,   	  0x01, R50, "R508601.S19" }, 
// 
#elif REFLEX25_ONLY
  { RUNTIME_CLASS(CPegasus), PEGASUS, 	  0x01, R25, "R25V42.S19" }, 
  { RUNTIME_CLASS(CPegasus), PEGASUS,     0x02, R25, "R25V48.S19" }, 
  { RUNTIME_CLASS(CPegasus), PEGASUS,     0x03, R25, "R25V411.S19" }, 
  { RUNTIME_CLASS(CPegasus), PEGASUS,     0x04, R25, "R25V412.S19" }, 
  { RUNTIME_CLASS(CPegasus), PEGASUS,     0x05, R25, "R25V412.S19" }, 
  { RUNTIME_CLASS(CPegasus), CENTAUR, 	  0x05, R25, "R25V412.S19" }, 
  { RUNTIME_CLASS(CPegasus), PEGASUS_ACK, 0x05, R25,  NULL }, 
  { RUNTIME_CLASS(CPegasus), PAGEFINDER,  0x05, R25, "R258205.S19" }, 
  { RUNTIME_CLASS(CPegasus), PAGEFINDER,  0x06, R25, "R258206.S19" }, 
  { RUNTIME_CLASS(CPegasus), PAGEFINDER,  0x07, R25, "R258207.S19" }, 
  { RUNTIME_CLASS(CPegasus), PAGEFINDER,  0x08, R25, "R258208.S19" }, 
  { RUNTIME_CLASS(CPegasus), PAGEFINDER,  0x09, R25, "R258209.S19" }, 
  { RUNTIME_CLASS(CPegasus), PAGEFINDER,  0x80, R25, "R258280.S19" },
  { RUNTIME_CLASS(CPegasus), EXPANDAUR,   0x80, R25, "R258580.S19" }, 
  { RUNTIME_CLASS(CPegasus), EXPANDAUR,   0x81, R25, "R258581.S19" }, 
//
#else 
  { RUNTIME_CLASS(CPegasus), PEGASUS, 	  0x01, R25, "R25V42.S19" }, 
  { RUNTIME_CLASS(CPegasus), PEGASUS,     0x02, R25, "R25V48.S19" }, 
  { RUNTIME_CLASS(CPegasus), PEGASUS,     0x02, R50, "R50V48.S19" }, 
  { RUNTIME_CLASS(CPegasus), PEGASUS,     0x03, R25, "R25V411.S19" }, 
  { RUNTIME_CLASS(CPegasus), PEGASUS,     0x03, R50, "R50V411.S19" }, 
  { RUNTIME_CLASS(CPegasus), PEGASUS,     0x04, R25, "R25V412.S19" }, 
  { RUNTIME_CLASS(CPegasus), PEGASUS,     0x04, R50, "R50V412.S19" }, 
  { RUNTIME_CLASS(CPegasus), PEGASUS,     0x05, R25, "R25V412.S19" }, 
  { RUNTIME_CLASS(CPegasus), PEGASUS,     0x05, R50, "R50V412.S19" }, 
  { RUNTIME_CLASS(CPegasus), CENTAUR, 	  0x05, R25, "R25V412.S19" }, 
  { RUNTIME_CLASS(CPegasus), CENTAUR, 	  0x05, R50, "R50V412.S19" }, 
  { RUNTIME_CLASS(CPegasus), PEGASUS_ACK, 0x05, R25,  NULL }, 
  { RUNTIME_CLASS(CPegasus), PAGEFINDER,  0x05, R25, "R258205.S19" }, 
  { RUNTIME_CLASS(CPegasus), PAGEFINDER,  0x05, R50, "R508205.S19" }, 
  { RUNTIME_CLASS(CPegasus), PAGEFINDER,  0x06, R25, "R258206.S19" }, 
  { RUNTIME_CLASS(CPegasus), PAGEFINDER,  0x06, R50, "R508206.S19" }, 
  { RUNTIME_CLASS(CPegasus), PAGEFINDER,  0x07, R25, "R258207.S19" }, 
  { RUNTIME_CLASS(CPegasus), PAGEFINDER,  0x07, R50, "R508207.S19" }, 
  { RUNTIME_CLASS(CPegasus), PAGEFINDER,  0x08, R25, "R258208.S19" }, 
  { RUNTIME_CLASS(CPegasus), PAGEFINDER,  0x08, R50, "R508208.S19" }, 
  { RUNTIME_CLASS(CPegasus), PAGEFINDER,  0x09, R25, "R258209.S19" }, 
  { RUNTIME_CLASS(CPegasus), PAGEFINDER,  0x09, R50, "R508209.S19" },
  { RUNTIME_CLASS(CPegasus), PAGEFINDER,  0x0A, R50, "R50820A.S19" },
  { RUNTIME_CLASS(CPegasus), PAGEFINDER,  0x0B, R50, "R50820B.S19" }, 
  { RUNTIME_CLASS(CPegasus), PAGEFINDER,  0x0C, R50, "R50820C.S19" }, 
  { RUNTIME_CLASS(CPegasus), ASPEN,       0x09, R50, "R508209.S19" }, 
  { RUNTIME_CLASS(CPegasus), PAGEFINDER,  0x80, R25, "R258280.S19" }, 
  { RUNTIME_CLASS(CPegasus), EXPANDAUR,   0x01, R50, "R508501.S19" }, 
  { RUNTIME_CLASS(CPegasus), EXPANDAUR,   0x80, R25, "R258580.S19" }, 
  { RUNTIME_CLASS(CPegasus), EXPANDAUR,   0x81, R25, "R258581.S19" }, 
  { RUNTIME_CLASS(CPegasus), LYNX,   	  0x01, R50, "R508601.S19" }, 
#endif  
  // *** ADD NEW PAGER SUPPORT INFO HERE
};

static int nNumSupportedPagers = (sizeof(SupportedPagers)) / (sizeof(struct tagPagerInfo));

/////////////////////////////////////////////////////////////////////////////
//  IsPagerSupported()
// 
//  This function searches the SupportedPagers PagerInfo struct array above
//  to determine if the indicated byPagerId is a valid identifier.  The
//  function is used primarily upon a device handshake operation.
//
BOOL CPpsApp::IsPagerSupported(BYTE byPagerId)
{  
  TRACE("\nCPpsApp::IsPagerSupported : byPagerId=%02X\n", byPagerId);
  
  for (int lcv = nNumSupportedPagers; lcv >= 0; lcv--)
  {
    if (SupportedPagers[lcv].byPagerId == byPagerId)
      return(TRUE);
  }
  return(FALSE);
}


/////////////////////////////////////////////////////////////////////////////
//  IsPagerSupported()
// 
//  This function searches the SupportedPagers PagerInfo struct array above
//  to determine if the indicated byPagerId is a valid identifier.  The
//  function is used primarily upon a Device-Read or File-Open operation.
//
BOOL CPpsApp::IsPagerSupported(BYTE byPagerId, BYTE byCodeplugVersion, BOOL bR50Enabled)
{
  TRACE("\nCPpsApp::IsPagerSupported : byPagerId=%02X : byCpVer=%02X : bR50Enable=%d\n", 
        byPagerId, byCodeplugVersion, bR50Enabled);

  for (int lcv = nNumSupportedPagers; lcv >= 0; lcv--)
  {
    if ((SupportedPagers[lcv].byPagerId == byPagerId) &&
        (SupportedPagers[lcv].byCodeplugVersion == byCodeplugVersion) &&
        (SupportedPagers[lcv].bReflex50 == bR50Enabled))
      return(TRUE);
  }
  return(FALSE);
}


/////////////////////////////////////////////////////////////////////////////
//  GetDefaultS19FileName()
// 
//  This function searches the SupportedPagers PagerInfo struct array above
//  to obtain a default codeplug .s19 file name associated with the indicated
//  pager id, codeplug version, and protocol.  The function is used by the
//  CPpsDoc class upon a Device-Read or File-Open operation.
//
const char *CPpsApp::GetDefaultS19FileName(BYTE byPagerId, BYTE byCodeplugVersion, 
                                            BOOL bR50Enabled)
{
  TRACE("\nCPpsApp::GetDefaultS19FileName : byPagerId=%02X : byCpVer=%02X : bR50Enable=%d\n", 
        byPagerId, byCodeplugVersion, bR50Enabled);

  for (int lcv = nNumSupportedPagers; lcv >= 0; lcv--)
  {
    if ((SupportedPagers[lcv].byPagerId == byPagerId) &&
        (SupportedPagers[lcv].byCodeplugVersion == byCodeplugVersion) &&
        (SupportedPagers[lcv].bReflex50 == bR50Enabled))
      return(SupportedPagers[lcv].pszDefaultS19File);
  }
  return(NULL);
}



/////////////////////////////////////////////////////////////////////////////
//  SelectThePager()
//
//  This function is called from the CPager base class upon completion of
//  an initiate read function.  The attached device will respond to the
//  AA ping with a device id.  If the device id is valid, then it is used
//  to select the appropriate CPager derived class to complete the codeplug
//  read operation. 
//
void CPpsApp::SelectThePager(BYTE byPagerId)
{
  TRACE("\nCPpsApp::SelectThePager : byPagerId=%02X\n", byPagerId);
   
  switch(byPagerId)
  { 
    case PEGASUS :
    case CENTAUR :
    case PEGASUS_ACK :
    case PAGEFINDER :
    case ASPEN :
    case EXPANDAUR : 
      m_pThePager = (CPager *)&thePegasusPager; 
      break;
    
    /********** VAIL FAMILY NOT SUPPORTED
    case VAIL :
      m_pThePager = (CPager *)&theVailPager;
      break;
    ***********/
    
    // Add new view set selection cases here...
    
    default:
      m_pThePager = (CPager *)&thePegasusPager; 
      break;  
  } 
  return;
}



/////////////////////////////////////////////////////////////////////////////
//  SelectViewSet()
//
//  This function is called from the CPpsDoc class upon a file open or 
//  device read operation.  The configuration device id is used to select
//  the set of views associated with that device.  
//
void CPpsApp::SelectViewSet(BYTE byDeviceId, BYTE byCodePlugVersion, BOOL bR50Enabled)
{ 
  TRACE("\nCPpsApp::SelectViewSet : byDeviceId=%02X : byCPVer=%02X\n",
         byDeviceId, byCodePlugVersion);
          
  ViewInfo *pCurrentViewSet = m_pViewInfo;
  
  // note:  Use the codeplug version parameter for finer grain selection
  // if required...
  
  switch (byDeviceId)
  {
    case PEGASUS :
    case CENTAUR :
    case PEGASUS_ACK :
      m_pViewInfo = PegasusViews;
      m_nNumViews = ((sizeof(PegasusViews)) / (sizeof(struct tagViewInfo)));
    break;
    //
    case PAGEFINDER :
      if (bR50Enabled)   // Reflex50 only PageFinder
      {
         m_pViewInfo = R50PageFinderViews;
         m_nNumViews = ((sizeof(R50PageFinderViews)) / (sizeof(struct tagViewInfo))); 
      }
      else if (byCodePlugVersion < 0x80)  // older Reflex25 PageFinder units
      {
         m_pViewInfo = PegasusViews;
         m_nNumViews = ((sizeof(PegasusViews)) / (sizeof(struct tagViewInfo)));
      }
      else  //  (byCodeplugVersion >= 0x80)  newer Reflex25 PageFinder units
      {
         m_pViewInfo = R25PageFinderViews;
         m_nNumViews = ((sizeof(R25PageFinderViews)) / (sizeof(struct tagViewInfo)));
      }
    break;
    //
    case ASPEN :
    case LYNX :
      m_pViewInfo = R50PageFinderViews;
      m_nNumViews = ((sizeof(R50PageFinderViews)) / (sizeof(struct tagViewInfo))); 
    break;
    //
    case EXPANDAUR :
      if (bR50Enabled)   // Reflex50 only Expandaur
      {
        m_pViewInfo = R50PageFinderViews;
        m_nNumViews = ((sizeof(R50PageFinderViews)) / (sizeof(struct tagViewInfo)));
      }
      else               // Reflex25 only Expandaur 
      {
        m_pViewInfo = R25PageFinderViews;
        m_nNumViews = ((sizeof(R25PageFinderViews)) / (sizeof(struct tagViewInfo)));
      }
    break;
    //
    // *** ADD NEW VIEW-SET SELECTION CASES HERE
    //
    default :
      m_pViewInfo = PegasusViews;
      m_nNumViews = ((sizeof(PegasusViews)) / (sizeof(struct tagViewInfo)));
    break;
  }
  
  // if new view set different from old switch to the common view.
  if (pCurrentViewSet != m_pViewInfo)
    SwitchView(0, FALSE);
    
  return;
} 




/////////////////////////////////////////////////////////////////////////////
// CPpsApp construction

CPpsApp::CPpsApp()
{ 
  // select the initial CPager derived object.
  m_pThePager = (CPager *)&thePegasusPager; 
  // set default view info data.
  m_nCurView = 0;                                   
  m_pViewInfo = PegasusViews;
  m_nNumViews = ((sizeof(PegasusViews)) / (sizeof(struct tagViewInfo)));
}





CPager *CPpsApp::GetThePager(void)
{
  return(m_pThePager);   // points to currently selected CPager derived object.
}



#include <direct.h>  // to get the current working directory path

/////////////////////////////////////////////////////////////////////////////
// CPpsApp initialization

BOOL CPpsApp::InitInstance()
{ 
	TRACE("\nCPpsApp::InitInstance\n");   
	
	SetDialogBkColor();        // Set dialog background color to gray
	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CPpsDoc),        // single application document
		RUNTIME_CLASS(CMainFrame),     // main SDI frame window
		RUNTIME_CLASS(CPpsView));      // initial application view
	AddDocTemplate(pDocTemplate);

    // create a new (empty) document
    OnFileNew();

    // if the following asserts trigger, then ensure that the static
    // ViewInfo data is properly pointed to in the constructor above.
    ASSERT(m_pViewInfo);
    ASSERT(m_nCurView == 0);
    ASSERT(m_nNumViews);
    
	// enable file manager drag/drop and DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes();
	m_pMainWnd->DragAcceptFiles();
    
    // establish the initial working directory path 
    char cBuff[200];
    m_strAppPathName = ::_getcwd(cBuff, sizeof(cBuff));

    if (m_lpCmdLine[0] != '\0')
    {    
		// open an existing document
		OpenDocumentFile(m_lpCmdLine);
    }   
	return TRUE;
}


////////////////////////////////////////////////////////////////////////////
//  GetViewIndex()
//  
//  If the class is a member of the current m_pViewInfo array, then the 
//  index into the array is returned.  Otherwise, a -1 indicates that the
//  view class is not available.
//
int CPpsApp::GetViewIndex(CRuntimeClass *pViewClass)
{
  for (int lcv = 0; lcv < m_nNumViews; ++lcv)
  {
    if (m_pViewInfo[lcv].pViewClass == pViewClass)
      return(lcv);
  }
  return(-1);  // no match
}



////////////////////////////////////////////////////////////////////////////
//  IsViewPresentable()
//  
//  See the corresponding description below.
//
BOOL CPpsApp::IsViewPresentable(CRuntimeClass *pViewClass)
{ 
  int nIndex = GetViewIndex(pViewClass);
  
  if (ValidIndex(nIndex) == FALSE) return(FALSE);
  return(m_pViewInfo[nIndex].pfnIsPresentable());
}


////////////////////////////////////////////////////////////////////////////
//  IsViewPresentable()
//  
//  Certain application views may not be presentable based upon current
//  pager id, codeplug version, labuse only, or any other constraints.
//  Views which may have a run-time viewability restriction must implement a static
//  member function which may be called without instantiating the view
//  class.  
//
BOOL CPpsApp::IsViewPresentable(int nIndex)
{
  if (ValidIndex(nIndex) == FALSE) return(FALSE);
  return(m_pViewInfo[nIndex].pfnIsPresentable());
}




/////////////////////////////////////////////////////////////////////////////
//  SwitchViewOnCommand()
//
//  Primary view switching mechanism for user view switching from a toolbar
//  or view menu action.  If the current view info contains a view class 
//  with an associated nCommandID, then the view is switched.  Else a NULL
//  pointer returns.
//
CView *CPpsApp::SwitchViewOnCommand(UINT nCommandID, BOOL bValidateAndSave)
{
  TRACE("\nCPpsApp::SwitchViewOnCommand : nCommandID = %04X\n", nCommandID);
  
  for (int lcv = 0; lcv < m_nNumViews; ++lcv)
  {
    if ((m_pViewInfo[lcv].nCommandId == nCommandID) && 
        (m_pViewInfo[lcv].pfnIsPresentable() == TRUE))
      return(SwitchView(m_pViewInfo[lcv].pViewClass, bValidateAndSave));
  }
  return(NULL);
}


 

/////////////////////////////////////////////////////////////////////////////
//	SwitchView()
//
//	Switch from one view to another by ViewInfo struct array index.  
//
CView *CPpsApp::SwitchView(int nIndex, BOOL bValidateAndSave)
{
  TRACE("\nPpsApp::SwitchView : nIndex=%d\n", nIndex);
  ASSERT((nIndex >= 0) && (nIndex < m_nNumViews));
 
  if (!((nIndex >= 0) && (nIndex < m_nNumViews)))
    return NULL;  
  return(SwitchView(m_pViewInfo[nIndex].pViewClass, bValidateAndSave));
}
  				  


/////////////////////////////////////////////////////////////////////////////
//  SwitchView()
//
//  View switching mechanism.  The currently active view class is replaced
//  by the selected view class.  The CRuntimeClass mechanism is used to 
//  ensure view creation and destruction follows the same path as in the
//  document template mechanism.  This design might be improved upon by
//  inheriting a CMultiViewSingleDocumentTemplate class from the 
//  CSingleDocTemplate class which currently supports only one view.
// 
CView *CPpsApp::SwitchView(CRuntimeClass *pViewClass, BOOL bValidateAndSave)
{
  TRACE("\nCPpsApp::SwitchView\n");
  CView *pView = (((CFrameWnd *)m_pMainWnd)->GetActiveView());    
  
  // If no active view, no need to switch.
  if (pView == NULL)
    return(NULL);
  
  // If we're already displaying this kind of view, no need to go further. 
  if ((pView->IsKindOf(pViewClass)) == TRUE)
     return(pView);

  // save active view's data, return on error                                  
  if (bValidateAndSave)
  {
    if (!(SaveActiveViewsData()))
      return(pView);
  }
  
  CView *pNewView = CreateNewView(pViewClass);
  if (pNewView == NULL)
    return(NULL);
  
  // set the new current ViewInfo array index.
  m_nCurView = GetViewIndex(pViewClass);

  // exchange view window ID's so RecalcLayout() works
  WORD temp = ::GetWindowWord(pView->m_hWnd, GWW_ID);
  ::SetWindowWord(pView->m_hWnd, GWW_ID, ::GetWindowWord(pNewView->m_hWnd, GWW_ID));
  ::SetWindowWord(pNewView->m_hWnd, GWW_ID, temp);
  
  // display and update the new current view - hide the old one 
  pView->ShowWindow(SW_HIDE);
  pNewView->SendMessage(WM_INITIALUPDATE, 0, 0);   
  pNewView->ShowWindow(SW_SHOW);
  ((CFrameWnd*)m_pMainWnd)->SetActiveView(pNewView);
  ((CFrameWnd*)m_pMainWnd)->RecalcLayout();
  // all views in this application must be derived from CPpsFormView - see 'ppsview.h'
  ((CPpsFormView*)pNewView)->OnUpdate(NULL, MAKELONG(0, 1), NULL); 
  
  // Destroy the old view.   
  DestroyOldView(pView);
  return(pNewView);
}
   


/////////////////////////////////////////////////////////////////////////
//  CreateNewView()
//
//  This function orchestrates both the view class object instantiation
//  as well as the Windows window object creation.  It is called from the
//  SwitchView function above as well as the OnPrintHelper function below.
//  In the case of the OnPrintHelper creation, the new class is intended
//  to exist only long enough to get run-time print strings.  The print
//  strings cannot be acquired if the view object is not instantiated.
//
CView *CPpsApp::CreateNewView(CRuntimeClass *pViewClass)
{ 
  TRACE("\nCPpsApp::CreateNewView\n");
  // Get pointer to CDocument object so that it can be used in the creation 
  // process of the new view
  CPpsDoc *pDoc = (CPpsDoc*)((CFrameWnd *)m_pMainWnd)->GetActiveDocument();

  // Instantiate the new view class.  The new view must be created and
  // in context before the current view is destroyed.  Otherwise, the 
  // document will be deleted automatically.                           
  CView *pNewView = (CView *)pViewClass->CreateObject();
  if (pNewView == NULL)
  {
   	TRACE("Warning: Dynamic create of view object %Fs failed.\n",
			pViewClass->m_lpszClassName);
	return(NULL);
  }

  // Initialize a CCreateContext to point to the active document.
  // With this context, the new view is added to the document
  // when the view is created in CView::OnCreate().
  CCreateContext newContext;
  newContext.m_pNewViewClass = pViewClass;
  newContext.m_pNewDocTemplate = NULL;
  newContext.m_pLastView = NULL;
  newContext.m_pCurrentFrame = (CFrameWnd*)m_pMainWnd;
  newContext.m_pCurrentDoc = pDoc;
    
  // create the Windows window.  The AFX_IDW_PANE_FIRST+1 value will be
  // replaced by AFX_IDW_PANE_FIRST when the window ID's are switched
  // below.
  if (!pNewView->Create(NULL, "UnimportantString", WS_CHILD,
      CRect(0,0,0,0), m_pMainWnd, AFX_IDW_PANE_FIRST+1, &newContext))
  {
     TRACE("Warning: couldn't create Windows window for view %Fs.\n",
             pViewClass->m_lpszClassName);
	 return(NULL);  
  }                                      
  return(pNewView);
}
  

////////////////////////////////////////////////////////////////////////////
//  DestroyOldView()
//
//  This function orchestrates both the view class object destruction
//  as well as the Windows window object destruction.  It is called from the
//  SwitchView function above as well as the OnPrintHelper function below.
//  In the case of the OnPrintHelper creation, the old class is intended
//  to exist only long enough to get run-time print strings.  The print
//  strings cannot be acquired if the view object has not been instantiated.
//  
void CPpsApp::DestroyOldView(CView *pView)
{                                 
  TRACE("\nCPpsApp::DestroyOldView\n");
  ASSERT(pView);
  // Destroy the old view.  Be sure to use DestroyWindow so that the Windows window
  // memory will be properly deleted.  Set flag so that document will not be deleted 
  // when the view is destroyed
  CPpsDoc *pDoc = (CPpsDoc*)((CFrameWnd *)m_pMainWnd)->GetActiveDocument();  
  BOOL bAutoDelete=pDoc->m_bAutoDelete;
  pDoc->m_bAutoDelete=FALSE; 
  pView->DestroyWindow();
  pDoc->m_bAutoDelete=bAutoDelete;
  return;
}



/////////////////////////////////////////////////////////////////////////////
//	SaveActiveViewsData()
//
//	Save and validate active views data if necessary.  Fail if 
//	validation fails.
//
BOOL CPpsApp::SaveActiveViewsData(void)
{ 
  TRACE("\nCPpsApp::SaveActiveViewsData\n");  
  CView	*pActiveView = (((CFrameWnd *)m_pMainWnd)->GetActiveView());
  
  if (!(pActiveView))
    return TRUE;		// no active view...
  
  // All views must be derived CPpsFormView, see file ppsview.h  
  return(((CPpsFormView*)pActiveView)->SetToDoc());
}                                              



/////////////////////////////////////////////////////////////////////////
//  ProcessMessageFilter()
//
//  Use this procedure to bypass default message processing as
//  necessary.  An example may be the interception of system keys
//  in order to bypass or enhance default key processing as in the
//  CHexEdit view. 
//
BOOL CPpsApp::ProcessMessageFilter(int code, LPMSG lpMsg)
{  
  #ifndef DISABLE_HEXEDIT
  CView	*pActiveView = (((CFrameWnd *)m_pMainWnd)->GetActiveView());
  
  if (lpMsg->message == WM_KEYDOWN)
  {
    if (pActiveView->IsKindOf(RUNTIME_CLASS(CHexEdit)))
    {                          
      if (((CHexEdit*)pActiveView)->ProcessMessageFilter(code, lpMsg) == TRUE)
        return TRUE;  
      // else continue with default message processing
    } 
  }
  #endif
  // Default processing of the message.
  return CWinApp::ProcessMessageFilter(code, lpMsg);
}
   



////////////////////////////////////////////////////////////////////////////
// OnPrintHelper()
//
// Print requests may be initiated from any view in the application.  Since
// MFC print functionality is directly tied to the CView class, only the active
// view receives the print command.  The CPpsFormView base class overrides the
// CView::OnPrint function which calls this function in order to iterate through
// each view obtaining print strings.
//
void CPpsApp::OnPrintHelper(CStringArray &rStringArray, UINT nPageNumber)
{
  TRACE("CPpsApp::OnPrintHelper : nPageNumber=%d\n", nPageNumber);
  
  CView	*pView = NULL;
  int nNumLinesAvailable = 55;
  int nNumLinesUsed = 0; 
  static int nNextView = 0;
  
  for (int lcv = nNextView; lcv < m_nNumViews; lcv++)
  {  
     // no need to call view if it is not presentable.
     if (IsViewPresentable(lcv) == FALSE) continue;
     // is the view currently being displayed? no need to create it. 
     if (lcv == m_nCurView)
     {
       pView = (((CFrameWnd *)m_pMainWnd)->GetActiveView()); 
     }
     else  // temporarily create hidden view in order to get print strings.
     {
       pView = CreateNewView(m_pViewInfo[lcv].pViewClass);
       ASSERT(pView);
       pView->OnInitialUpdate();   
     }    
     if (pView != NULL)
     {
        nNumLinesUsed = ((CPpsFormView *)pView)->OnPrintHelper(rStringArray, nPageNumber, nNumLinesAvailable);
        if (nNumLinesUsed > 0)
          nNumLinesAvailable -= nNumLinesUsed;
     }
     if (lcv != m_nCurView)   // destroy the temporarily created view.
     {
       DestroyOldView(pView);
       pView = NULL;
     }
     if (nNumLinesUsed < 0) 
     {
       nNextView = lcv;  
       return;    // not enough lines available on this page, print on next page.
     }
  }
  
  // reset after all views given print opportunity.
  if (lcv >= m_nNumViews)
    nNextView = 0;
  return;
}



///////////////////////////////////////////////////////////////////////////////
//  Set All to Default command handlers
//
//  Iterate through each view and call the 'UseDefaults' member function in 
//  order to globally set each field to the default setting.
//
static char *pszOnSetDefaults = "Set all modifiable configuration fields to the \n"
                                "default content?  Press F1 for Help."; 
static const DWORD dwOnDefaultsHelpID = HID_BASE_RESOURCE + IDP_SETALLDEFAULTS;
// 
void CPpsApp::OnUtilitySetAllDefault()
{ 
  TRACE("\nCPpsApp::OnUtilitySetAllDefault : dwOnDefaultsHelpID = %04X\n", (UINT)dwOnDefaultsHelpID);
  if (AfxMessageBox(pszOnSetDefaults, MB_OKCANCEL, (UINT)dwOnDefaultsHelpID) == IDOK)  
   OnSetAllDefault();
  return;
}

void CPpsApp::OnUpdateUtilitySetAllDefault(CCmdUI* pCmdUI)
{
  CPpsDoc *pCPpsDoc = (CPpsDoc*)((CFrameWnd *)m_pMainWnd)->GetActiveDocument();  
  ASSERT(pCPpsDoc->IsKindOf(RUNTIME_CLASS(CPpsDoc)));

  BOOL bEnable = ((!(GetThePager()->IsOperationInProgress())) && (pCPpsDoc->GetDocSize()));
  pCmdUI->Enable(bEnable);
  return;
}


//  This command handler is directly initiated by the CTRL+D key sequence. The
//  intent is to by-pass the confirmation message box which is presented
//  in the menu handler above. 
static char *pszNoDefaultData = "Default configuration data is not\n"
                                "available for this device.";
void CPpsApp::OnSetAllDefault()
{
  CPpsDoc *pDoc = (CPpsDoc*)((CFrameWnd *)m_pMainWnd)->GetActiveDocument();  
  ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(CPpsDoc)));

  if (pDoc->IsDefaultDataAvailable() == FALSE)
  {
    AfxMessageBox(pszNoDefaultData, MB_OK); 
    return;
  }
  
  // ensure current view's data is saved first.
  if (!(SaveActiveViewsData()))
    return;

  // iterate through all presentable views to set each view's fields to
  // the default content.
  CView	*pView = NULL;
  for (int lcv = 0; lcv < m_nNumViews; lcv++)
  {  
     // no need to call view if it is not presentable.
     if (IsViewPresentable(lcv) == FALSE) continue;
     // is the view currently being displayed? no need to create it. 
     if (lcv == m_nCurView)
     {
       pView = (((CFrameWnd *)m_pMainWnd)->GetActiveView()); 
     }
     else  // temporarily create hidden view in order to call member function.
     {
       pView = CreateNewView(m_pViewInfo[lcv].pViewClass);
       ASSERT(pView);
       pView->OnInitialUpdate();   
     }    
     if (pView != NULL)  // call the view member function
     {
        ((CPpsFormView *)pView)->OnDefaults();
     }
     if (lcv != m_nCurView)   // destroy the temporarily created view.
     {
       DestroyOldView(pView);
       pView = NULL;
     }
  }
  ::MessageBeep(MB_OK);      // give user a completion indicator   
  return;
}




///////////////////////////////////////////////////////////////////////
//  OnUtilitySetAllArchive()
//
//  Iterate through each view and call the 'UseArchived' member function in 
//  order to globally set each field to the user specified archive
//  file settings.
//
//  
void CPpsApp::OnUtilitySetAllArchive()
{
  CPpsDoc *pDoc = (CPpsDoc*)((CFrameWnd *)m_pMainWnd)->GetActiveDocument();  
  ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(CPpsDoc)));

  // ensure current view's data is saved first.
  if (!(SaveActiveViewsData()))
    return;

  if (pDoc->LoadArchivedConfiguration())
    return;	   // abort on error or user cancel
    
  // iterate through all presentable views to set each view's fields to
  // the archived content.
  CView	*pView = NULL;
  for (int lcv = 0; lcv < m_nNumViews; lcv++)
  {  
     // no need to call view if it is not presentable.
     if (IsViewPresentable(lcv) == FALSE) continue;
     // is the view currently being displayed? no need to create it. 
     if (lcv == m_nCurView)
     {
       pView = (((CFrameWnd *)m_pMainWnd)->GetActiveView()); 
     }
     else  // temporarily create hidden view in order to call member function.
     {
       pView = CreateNewView(m_pViewInfo[lcv].pViewClass);
       ASSERT(pView);
       pView->OnInitialUpdate();   
     }    
     if (pView != NULL)  // call the view member function
     {
        ((CPpsFormView *)pView)->OnArchived();
     }
     if (lcv != m_nCurView)   // destroy the temporarily created view.
     {
       DestroyOldView(pView);
       pView = NULL;
     }
  }
  ::MessageBeep(MB_OK);      // give user a completion indicator   
  return;  
}

void CPpsApp::OnUpdateUtilitySetAllArchive(CCmdUI* pCmdUI)
{
  CPpsDoc *pCPpsDoc = (CPpsDoc*)((CFrameWnd *)m_pMainWnd)->GetActiveDocument();  
  ASSERT(pCPpsDoc->IsKindOf(RUNTIME_CLASS(CPpsDoc)));

  BOOL bEnable = ((!(GetThePager()->IsOperationInProgress())) && (pCPpsDoc->GetDocSize()));
  pCmdUI->Enable(bEnable);
}


//////////////////////////////////////////////////////////////////////
//  StatusMsg()
//
void CPpsApp::StatusMsg(const CString &rstrMsg)
{
  ((CMainFrame *)m_pMainWnd)->StatusBarMsg((LPCTSTR)rstrMsg);
  return;
}



//////////////////////////////////////////////////////////////////////
//  StatusMsg()
//
void CPpsApp::StatusMsg(UINT nStringResourceId)
{ 
  CString	strTemp = "\0";
  VERIFY(strTemp.LoadString(nStringResourceId));  // fail on resource not found
  ((CMainFrame *)m_pMainWnd)->StatusBarMsg((LPCTSTR)strTemp);
  return;
}


/////////////////////////////////////////////////////////////////////
//  StatusMsgClear()
//
void CPpsApp::StatusMsgClear(void)
{
  StatusMsg(AFX_IDS_IDLEMESSAGE);
  return;
}



BOOL CPpsApp::IsLabUseOnly(void)
{
  return((((CMainFrame *)m_pMainWnd)->IsAccessRestricted() == FALSE));
}



/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{ 
  public:
	CAboutDlg();

  // Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

  // Implementation
  protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}



void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}



BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();    
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// App command to run the dialog
void CPpsApp::OnAppAbout()
{
  if (theIoController.IsOperationInProgress())
    return;
     
  CAboutDlg aboutDlg;
  aboutDlg.DoModal();
}


