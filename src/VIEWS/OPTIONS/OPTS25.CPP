//*************************************************************
//  History:    Date       Author               Comment
//              1997       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
// opts25.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\pps.h"
#include "..\..\ppsdoc.h"
#include "..\..\ppsview.h" 
#include "..\..\cpager\codeplug.h" 
#include "..\..\misc\spinbtn\spinbtn.h"                                  
#include "opts25.h"

extern CPpsApp    theApp;

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsR25

IMPLEMENT_DYNCREATE(COptionsR25, CPpsFormView)

COptionsR25::COptionsR25()
	: CPpsFormView(COptionsR25::IDD)
{
	//{{AFX_DATA_INIT(COptionsR25)
	m_bInternationalDate = FALSE;
	m_szMessageReadResponseMode = "";
	m_fAckMsgPctg = 0;
	m_bDisplayTimestamp = FALSE;
	m_bPowerOnRamCheck = FALSE;
	m_byRegFailRestTime = 0;
	m_byAsyncSleepFrames = 0;
	//}}AFX_DATA_INIT
}

COptionsR25::~COptionsR25()
{
}

void COptionsR25::DoDataExchange(CDataExchange* pDX)
{
	CPpsFormView::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(COptionsR25)
	DDX_Check(pDX, IDC_INTNATNL_DATE_CHK, m_bInternationalDate);
	DDX_Text(pDX, IDC_MSG_READ_RESPONSE, m_szMessageReadResponseMode);
	DDV_MaxChars(pDX, m_szMessageReadResponseMode, 9);
	DDX_Text(pDX, IDC_ACKMSGPCTG_EDIT, m_fAckMsgPctg);
	DDV_MinMaxFloat(pDX, m_fAckMsgPctg, 0.0, 10.0);
	DDX_Check(pDX, IDC_TIMESTAMP_CHECK, m_bDisplayTimestamp);
	DDX_Check(pDX, IDC_POWERONRAM_CHECK, m_bPowerOnRamCheck);
	DDX_Text(pDX, IDC_REGFAILRESTTIME_EDIT, m_byRegFailRestTime);
	DDV_MinMaxByte(pDX, m_byRegFailRestTime, 0, 255);
	DDX_Text(pDX, IDC_ASYNCSLEEPTIME_EDIT, m_byAsyncSleepFrames);
	DDV_MinMaxByte(pDX, m_byAsyncSleepFrames, 0, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsR25, CPpsFormView)
	//{{AFX_MSG_MAP(COptionsR25)
	ON_BN_CLICKED(IDC_OPTIONS_DEFAULTS_BTN, OnAcceptDefaultsBtn)
	ON_BN_CLICKED(IDC_OPTIONS_UNDO_BTN, OnUndoBtn)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SPIN_BUTTON, OnWmSpinButton)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COptionsR25 diagnostics

#ifdef _DEBUG
void COptionsR25::AssertValid() const
{
	CPpsFormView::AssertValid();
}

void COptionsR25::Dump(CDumpContext& dc) const
{
	CPpsFormView::Dump(dc);
}
#endif //_DEBUG



////////////////////////////////////////////////////////////////////////////
//	OnInitialUpdate()
//  
//  Update DDX variables from document and resize frame around view.
//
void COptionsR25::OnInitialUpdate(void)
{
  TRACE("COptionsR25::OnInitialUpdate : COptionsR25::IDD=%X\n", COptionsR25::IDD);
  
  // Load the Bitmap Button images
  VERIFY(m_MsgReadUpBtn.AutoLoad(IDC_MSGREAD_UP_BTN, this));
  VERIFY(m_MsgReadDnBtn.AutoLoad(IDC_MSGREAD_DWN_BTN, this));

  VERIFY(m_AckMsgPctgUpBtn.AutoLoad(IDC_ACKMSGPCTG_UP_BTN, this));
  VERIFY(m_AckMsgPctgDnBtn.AutoLoad(IDC_ACKMSGPCTG_DWN_BTN, this));
  
  CPpsFormView::OnInitialUpdate();
  return;
}




/////////////////////////////////////////////////////////////////////////////
//	OnUpdate()
//
void COptionsR25::OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint)
{
  TRACE("\nCOptionsR25::OnUpdate : IDD=%X  LHintHI=%X  lHintLO=%X\n", 
  		COptionsR25::IDD,  HIWORD(lHint), LOWORD(lHint));

  if (HIWORD(lHint))        		// user switched views, now active
    ResizeParentToFit(FALSE);

  SetFromDoc();
  CPpsFormView::OnUpdate(pSender, lHint, pHint);
}	



// ***	Static Data as per Codeplug description document							  
static char *szMessageReadByUser[] = { "None", "Automatic", "Manual", "Illegal" };
static const nNumMessageReadByUser = 2;   // manual not supported
static BYTE byIndexMessageReadByUser = 0;


////////////////////////////////////////////////////////////////////////////
//  SetFromDoc()
//
void COptionsR25::SetFromDoc(void)
{
  TRACE("\nCOptionsR25::SetFromDoc\n");
  CPpsDoc *pDoc = GetDocument();
  
  // CP_AckMsgPctg
  CP_AckMsgPctg.bySetFromDoc(&m_byAckMsgPctg);
  m_fAckMsgPctg = ((m_byAckMsgPctg / 200.0F) * 100);
  
  // CP_Function1_L
  CP_msg_read_by_user.bySetFromDoc(&byIndexMessageReadByUser);  
  if (byIndexMessageReadByUser < nNumMessageReadByUser)
    m_szMessageReadResponseMode = szMessageReadByUser[byIndexMessageReadByUser];
  
  // CP_Display_L
  CP_international_date.bSetFromDoc(&m_bInternationalDate);
  CP_display_timestamp.bSetFromDoc(&m_bDisplayTimestamp);
  
  // Memory_Opts_L
  CP_power_on_ram_check.bSetFromDoc(&m_bPowerOnRamCheck);
    
  // *** CP_RegFailRestTime 
  CP_RegFailRestTime.bySetFromDoc(&m_byRegFailRestTime);
  
  // CP_AsyncSleepTime
  CP_AsyncSleepTime.bySetFromDoc(&m_byAsyncSleepFrames);

  UpdateData(FALSE);	// set controls data
}



////////////////////////////////////////////////////////////////////////////
//	SetToDoc()
//
BOOL COptionsR25::SetToDoc(void)
{
  TRACE("\nCOptionsR25::SetToDoc\n");
  CPpsDoc *pDoc = GetDocument();
  
  if (!(UpdateData(TRUE)))  // get controls data
    return FALSE;
  
  // CP_AckMsgPctg
  CP_AckMsgPctg.bySetToDoc(m_byAckMsgPctg);

  // CP_Funciton1_L
  CP_msg_read_by_user.bySetToDoc(byIndexMessageReadByUser);  
  
  // CP_Display_L
  CP_international_date.bSetToDoc(m_bInternationalDate);
  CP_display_timestamp.bSetToDoc(m_bDisplayTimestamp);
  
  // Memory_Opts_L
  CP_power_on_ram_check.bSetToDoc(m_bPowerOnRamCheck);

  // *** CP_RegFailRestTime
  CP_RegFailRestTime.bySetToDoc(m_byRegFailRestTime);
  
  // CP_AsyncSleepTime
  CP_AsyncSleepTime.bySetToDoc(m_byAsyncSleepFrames); 

  return(TRUE);
}




/////////////////////////////////////////////////////////////////////////////
// COptionsR25 message handlers




////////////////////////////////////////////////////////////////////////
//  OnAcceptDefaultsBtn()
//
//  This function initiates a move of data from select fields of the
//  CPpsDoc.m_byDefaultCodePlugArray to the CPpsDoc.m_byCodePlugArray.
//  The m_byDefaultCodePlugArray is loaded from file 'default.s19' upon
//  application execution.  If the file has not been found, then no 
//  default values are available.  
//
void COptionsR25::OnAcceptDefaultsBtn()
{
  TRACE("\nCOptionsR25::OnAcceptDefaultsBtn\n"); 
  CPpsDoc	*pDoc = GetDocument(); 
   
  if (pDoc->SetFieldFromDefault(CP_Function1_L.Offset(), CP_Function1_L.Length()))
    return;
  if (pDoc->SetFieldFromDefault(CP_Display_L.Offset(), CP_Display_L.Length()))
    return;  
  if (pDoc->SetFieldFromDefault(CP_AckMsgPctg.Offset(), CP_AckMsgPctg.Length()))
    return; 
  if (pDoc->SetFieldFromDefault(CP_Memory_Opts_L.Offset(), CP_Memory_Opts_L.Length()))
    return; 
  if (pDoc->SetFieldFromDefault(CP_RegFailRestTime.Offset(), CP_RegFailRestTime.Length()))
    return;
  if (pDoc->SetFieldFromDefault(CP_AsyncSleepTime.Offset(), CP_AsyncSleepTime.Length()))
    return;

  SetFromDoc();   // update this view
}


////////////////////////////////////////////////////////////////////////
//  OnArchived()
//
//  This function initiates a move of data from select fields of the
//  CPpsDoc.m_byArchivedCodePlugArray to the CPpsDoc.m_byCodePlugArray.
//
void COptionsR25::OnArchived(void)
{
  CPpsDoc	*pDoc = GetDocument(); 
   
  if (pDoc->SetFieldFromArchived(CP_Function1_L.Offset(), CP_Function1_L.Length()))
    return;
  if (pDoc->SetFieldFromArchived(CP_Display_L.Offset(), CP_Display_L.Length()))
    return;  
  if (pDoc->SetFieldFromArchived(CP_AckMsgPctg.Offset(), CP_AckMsgPctg.Length()))
    return; 
  if (pDoc->SetFieldFromArchived(CP_Memory_Opts_L.Offset(), CP_Memory_Opts_L.Length()))
    return; 
  if (pDoc->SetFieldFromArchived(CP_RegFailRestTime.Offset(), CP_RegFailRestTime.Length()))
    return;
  if (pDoc->SetFieldFromArchived(CP_AsyncSleepTime.Offset(), CP_AsyncSleepTime.Length()))
    return;
    
  SetFromDoc();   // update this view
}


////////////////////////////////////////////////////////////////////////
//  OnUndoBtn()
//
//  This function initiates a move of data from select fields of the
//  CPpsDoc.m_byOriginalCodePlugArray to the CPpsDoc.m_byCodePlugArray.
//  The m_byOriginalCodePlugArray is loaded upon 'pager-read' or 
//  'file-open' menu or toolbar actions.
//
void COptionsR25::OnUndoBtn()
{
  TRACE("\nCOptionsR25::OnUndoBtn\n"); 
  CPpsDoc	*pDoc = GetDocument(); 
  
  if (pDoc->SetFieldFromOriginal(CP_Function1_L.Offset(), CP_Function1_L.Length()))
    return;
  if (pDoc->SetFieldFromOriginal(CP_Display_L.Offset(), CP_Display_L.Length()))
    return;
  if (pDoc->SetFieldFromOriginal(CP_AckMsgPctg.Offset(), CP_AckMsgPctg.Length()))
    return;  
  if (pDoc->SetFieldFromOriginal(CP_Memory_Opts_L.Offset(), CP_Memory_Opts_L.Length()))
    return; 
  if (pDoc->SetFieldFromOriginal(CP_RegFailRestTime.Offset(), CP_RegFailRestTime.Length()))
    return;
  if (pDoc->SetFieldFromOriginal(CP_AsyncSleepTime.Offset(), CP_AsyncSleepTime.Length()))
    return;

  SetFromDoc();   // update this view
}



/////////////////////////////////////////////////////////////////////////
//  OnWmSpinButton()
//
//  Calls to this function are generated from the CSpinButton::OnTimer()
//  function.  These calls result from the user holding down the left
//  mouse button.  The effect is to provide a miniature scroll button.
//
afx_msg LONG COptionsR25::OnWmSpinButton(UINT wParam, LONG lParam)
{
  TRACE("\nCAlerts::OnWmSpinButton : nBtnID=%04X\n", wParam);
  
  int nAcceleration = LOWORD(lParam);
  
  if (!UpdateData(TRUE))
    return(0L);

  switch(wParam)
  {
    case IDC_MSGREAD_UP_BTN :
        if (byIndexMessageReadByUser < (nNumMessageReadByUser-1))
        {
          byIndexMessageReadByUser++;
          m_szMessageReadResponseMode = szMessageReadByUser[byIndexMessageReadByUser];
        }  
        break; 

    case IDC_MSGREAD_DWN_BTN :
        if (byIndexMessageReadByUser)
        {
          byIndexMessageReadByUser--;
          m_szMessageReadResponseMode = szMessageReadByUser[byIndexMessageReadByUser];
        }  
        break; 
    
    case IDC_ACKMSGPCTG_UP_BTN:
        if ((m_byAckMsgPctg + 1) > MAX_ACKMSGPCTG) 
          break;
        m_byAckMsgPctg++;
        m_fAckMsgPctg = ((m_byAckMsgPctg / 200.0F) * 100);
        break;
        
    case IDC_ACKMSGPCTG_DWN_BTN:
        if ((m_byAckMsgPctg - 1) < MIN_ACKMSGPCTG) 
          break;
        m_byAckMsgPctg--;
        m_fAckMsgPctg = ((m_byAckMsgPctg / 200.0F) * 100);
        break;
    
    default:
        break;
  }

  UpdateData(FALSE);
  return(0L);
}


int COptionsR25::OnPrintHelper(CStringArray &rStringArray, UINT nPageNumber, int nAvailableLines)
{ 
   // if not enough lines available, wait for next page...
   const int nNumLinesRequired = 11;
   if (nAvailableLines < nNumLinesRequired)
     return(-1);    
   int nNumLinesBegin = rStringArray.GetSize();
     
   char    szTemp[200];
   
   rStringArray.Add("MISCELLANEOUS OPTIONS");
       rStringArray.Add("    Ack-Only:"); 
       sprintf(szTemp, "        First Message Read by User Response Mode:  %s", m_szMessageReadResponseMode);
       rStringArray.Add(szTemp);
       sprintf(szTemp, "        Acceptable Message Error-Content Percentage:  %f\n", m_fAckMsgPctg);
       rStringArray.Add(szTemp);
         
   rStringArray.Add("    Miscellaneous:"); 
       sprintf(szTemp, "        Enable International Date Format (d-m-y):  %d", m_bInternationalDate);
       rStringArray.Add(szTemp);
       //
       sprintf(szTemp, "        Display Time of Message Receipt:  %d", m_bDisplayTimestamp); 
       rStringArray.Add(szTemp);
       //
       sprintf(szTemp, "        Enable 'Power-On' RAM Check:  %d", m_bPowerOnRamCheck);
       rStringArray.Add(szTemp);
   
   sprintf(szTemp, "    Partial Coverage Registration Rest Timer:  %d", m_byRegFailRestTime);
   rStringArray.Add(szTemp);
   
   sprintf(szTemp, "      Asynchronous Sleep Cycle:  %d", m_byAsyncSleepFrames); 
   rStringArray.Add(szTemp);
   
   rStringArray.Add("");
   return(rStringArray.GetSize() - nNumLinesBegin);
}

