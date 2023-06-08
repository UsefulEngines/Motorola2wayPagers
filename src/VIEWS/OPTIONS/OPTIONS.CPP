//*************************************************************
//  History:    Date       Author               Comment
//              1996       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
// options.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\pps.h"
#include "..\..\ppsdoc.h"
#include "..\..\ppsview.h" 
#include "..\..\cpager\codeplug.h" 
#include "..\..\misc\spinbtn\spinbtn.h"                                  
#include "options.h"

extern CPpsApp    theApp;

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptions

IMPLEMENT_DYNCREATE(COptions, CPpsFormView)

COptions::COptions()
	: CPpsFormView(COptions::IDD)
{
	//{{AFX_DATA_INIT(COptions)
	m_bInternationalDate = FALSE;
	m_szMessageReadResponseMode = "";
	m_fAckMsgPctg = 0;
	m_bDisplayTimestamp = FALSE;
	m_bPowerOnRamCheck = FALSE;
	//}}AFX_DATA_INIT     
	
	m_bR50Enabled = TRUE;
}

COptions::~COptions()
{
}

void COptions::DoDataExchange(CDataExchange* pDX)
{
	CPpsFormView::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(COptions)
	DDX_Check(pDX, IDC_INTNATNL_DATE_CHK, m_bInternationalDate);
	DDX_Text(pDX, IDC_MSG_READ_RESPONSE, m_szMessageReadResponseMode);
	DDV_MaxChars(pDX, m_szMessageReadResponseMode, 9);
	DDX_Check(pDX, IDC_TIMESTAMP_CHECK, m_bDisplayTimestamp);
	DDX_Check(pDX, IDC_POWERONRAM_CHECK, m_bPowerOnRamCheck);
	//}}AFX_DATA_MAP 
	
	if (m_bR50Enabled == FALSE)
	{
	  DDX_Text(pDX, IDC_ACKMSGPCTG_EDIT, m_fAckMsgPctg);
	  DDV_MinMaxFloat(pDX, m_fAckMsgPctg, 0.0, 10.0);
	}
}


BEGIN_MESSAGE_MAP(COptions, CPpsFormView)
	//{{AFX_MSG_MAP(COptions)
	ON_BN_CLICKED(IDC_OPTIONS_DEFAULTS_BTN, OnAcceptDefaultsBtn)
	ON_BN_CLICKED(IDC_OPTIONS_UNDO_BTN, OnUndoBtn)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SPIN_BUTTON, OnWmSpinButton)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COptions diagnostics

#ifdef _DEBUG
void COptions::AssertValid() const
{
	CPpsFormView::AssertValid();
}

void COptions::Dump(CDumpContext& dc) const
{
	CPpsFormView::Dump(dc);
}
#endif //_DEBUG



////////////////////////////////////////////////////////////////////////////
//	OnInitialUpdate()
//  
//  Update DDX variables from document and resize frame around view.
//
void COptions::OnInitialUpdate(void)
{
  TRACE("COptions::OnInitialUpdate : COptions::IDD=%X\n", COptions::IDD);
  
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
void COptions::OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint)
{
  TRACE("\nCOptions::OnUpdate : IDD=%X  LHintHI=%X  lHintLO=%X\n", 
  		COptions::IDD,  HIWORD(lHint), LOWORD(lHint));

  if (HIWORD(lHint))        		// user switched views, now active
    ResizeParentToFit(FALSE);
  
  CP_reflex50_enable.bSetFromDoc(&m_bR50Enabled);
  
  // disable % acceptable error field
  CStatic *pStatic = (CStatic*)GetDlgItem(IDC_MSG_ERR_STATIC);
  ASSERT(pStatic != NULL);
  pStatic->EnableWindow(!m_bR50Enabled);
  CEdit *pEdit = (CEdit*)GetDlgItem(IDC_ACKMSGPCTG_EDIT);
  ASSERT(pEdit != NULL);
  pEdit->EnableWindow(!m_bR50Enabled);
  CBitmapButton *pBtn = (CBitmapButton*)GetDlgItem(IDC_ACKMSGPCTG_UP_BTN);
  pBtn->EnableWindow(!m_bR50Enabled);
  pBtn = (CBitmapButton*)GetDlgItem(IDC_ACKMSGPCTG_DWN_BTN);
  ASSERT(pEdit != NULL);
  pBtn->EnableWindow(!m_bR50Enabled);

  // if the codeplug version is < 0x04, disable select display options
  CPpsDoc	*pDoc = GetDocument();
  BYTE		byThisDeviceId = pDoc->GetDeviceId(); 
  BYTE		byThisCPVersion = pDoc->GetCpVersion(); 
  BOOL		bEnable = ((byThisDeviceId >= PAGEFINDER) ||
                       ((byThisDeviceId <= PAGEFINDER) && (byThisCPVersion >= 0x04)));
  //
  m_bPowerOnRamCheck = FALSE;
  CButton   *pButton = (CButton*)GetDlgItem(IDC_POWERONRAM_CHECK);
  ASSERT(pButton != NULL);
  pButton->EnableWindow(bEnable);
  
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
void COptions::SetFromDoc(void)
{
  TRACE("\nCOptions::SetFromDoc\n");
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
  BYTE		byThisDeviceId = pDoc->GetDeviceId(); 
  BYTE		byThisCPVersion = pDoc->GetCpVersion(); 
  if ((byThisDeviceId >= PAGEFINDER) ||
      ((byThisDeviceId <= PAGEFINDER) && (byThisCPVersion >= 0x04)));
    CP_power_on_ram_check.bSetFromDoc(&m_bPowerOnRamCheck);
    
  UpdateData(FALSE);	// set controls data
}



////////////////////////////////////////////////////////////////////////////
//	SetToDoc()
//
BOOL COptions::SetToDoc(void)
{
  TRACE("\nCOptions::SetToDoc\n");
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
  BYTE		byThisDeviceId = pDoc->GetDeviceId(); 
  BYTE		byThisCPVersion = pDoc->GetCpVersion(); 
  if ((byThisDeviceId >= PAGEFINDER) ||
      ((byThisDeviceId <= PAGEFINDER) && (byThisCPVersion >= 0x04)));
    CP_power_on_ram_check.bSetToDoc(m_bPowerOnRamCheck);

  return(TRUE);
}




/////////////////////////////////////////////////////////////////////////////
// COptions message handlers




////////////////////////////////////////////////////////////////////////
//  OnAcceptDefaultsBtn()
//
//  This function initiates a move of data from select fields of the
//  CPpsDoc.m_byDefaultCodePlugArray to the CPpsDoc.m_byCodePlugArray.
//  The m_byDefaultCodePlugArray is loaded from file 'default.s19' upon
//  application execution.  If the file has not been found, then no 
//  default values are available.  
//
void COptions::OnAcceptDefaultsBtn()
{
  TRACE("\nCOptions::OnAcceptDefaultsBtn\n"); 
  CPpsDoc	*pDoc = GetDocument(); 
   
  if (pDoc->SetFieldFromDefault(CP_Function1_L.Offset(), CP_Function1_L.Length()))
    return;
  if (pDoc->SetFieldFromDefault(CP_Display_L.Offset(), CP_Display_L.Length()))
    return;  
  if (pDoc->SetFieldFromDefault(CP_AckMsgPctg.Offset(), CP_AckMsgPctg.Length()))
    return; 
  BYTE		byThisDeviceId = pDoc->GetDeviceId(); 
  BYTE		byThisCPVersion = pDoc->GetCpVersion(); 
  if ((byThisDeviceId >= PAGEFINDER) ||
      ((byThisDeviceId <= PAGEFINDER) && (byThisCPVersion >= 0x04)));
  {
    if (pDoc->SetFieldFromDefault(CP_Memory_Opts_L.Offset(), CP_Memory_Opts_L.Length()))
      return; 
  }
  
  SetFromDoc();   // update this view
}


////////////////////////////////////////////////////////////////////////
//  OnArchived()
//
//  This function initiates a move of data from select fields of the
//  CPpsDoc.m_byArchivedCodePlugArray to the CPpsDoc.m_byCodePlugArray.
//
void COptions::OnArchived(void)
{
  CPpsDoc	*pDoc = GetDocument(); 
   
  if (pDoc->SetFieldFromArchived(CP_Function1_L.Offset(), CP_Function1_L.Length()))
    return;
  if (pDoc->SetFieldFromArchived(CP_Display_L.Offset(), CP_Display_L.Length()))
    return;  
  if (pDoc->SetFieldFromArchived(CP_AckMsgPctg.Offset(), CP_AckMsgPctg.Length()))
    return; 
  BYTE		byThisDeviceId = pDoc->GetDeviceId(); 
  BYTE		byThisCPVersion = pDoc->GetCpVersion(); 
  if ((byThisDeviceId >= PAGEFINDER) ||
      ((byThisDeviceId <= PAGEFINDER) && (byThisCPVersion >= 0x04)));
  {
    if (pDoc->SetFieldFromArchived(CP_Memory_Opts_L.Offset(), CP_Memory_Opts_L.Length()))
      return; 
  }
    
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
void COptions::OnUndoBtn()
{
  TRACE("\nCOptions::OnUndoBtn\n"); 
  CPpsDoc	*pDoc = GetDocument(); 
  
  if (pDoc->SetFieldFromOriginal(CP_Function1_L.Offset(), CP_Function1_L.Length()))
    return;
  if (pDoc->SetFieldFromOriginal(CP_Display_L.Offset(), CP_Display_L.Length()))
    return;
  if (pDoc->SetFieldFromOriginal(CP_AckMsgPctg.Offset(), CP_AckMsgPctg.Length()))
    return;  
  BYTE		byThisDeviceId = pDoc->GetDeviceId(); 
  BYTE		byThisCPVersion = pDoc->GetCpVersion(); 
  if ((byThisDeviceId >= PAGEFINDER) ||
      ((byThisDeviceId <= PAGEFINDER) && (byThisCPVersion >= 0x04)));
  {
    if (pDoc->SetFieldFromOriginal(CP_Memory_Opts_L.Offset(), CP_Memory_Opts_L.Length()))
      return; 
  }

  SetFromDoc();   // update this view
}



/////////////////////////////////////////////////////////////////////////
//  OnWmSpinButton()
//
//  Calls to this function are generated from the CSpinButton::OnTimer()
//  function.  These calls result from the user holding down the left
//  mouse button.  The effect is to provide a miniature scroll button.
//
afx_msg LONG COptions::OnWmSpinButton(UINT wParam, LONG lParam)
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


int COptions::OnPrintHelper(CStringArray &rStringArray, UINT nPageNumber, int nAvailableLines)
{ 
   // if not enough lines available, wait for next page...
   const int nNumLinesRequired = 9;
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
   
   rStringArray.Add("");
   return(rStringArray.GetSize() - nNumLinesBegin);
}

