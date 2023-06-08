//*************************************************************
//  History:    Date       Author               Comment
//              1996       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
// security.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\pps.h"
#include "..\..\ppsdoc.h"
#include "..\..\ppsview.h"
#include "..\..\mainfrm.h" 
#include "..\..\cpager\codeplug.h" 
#include "..\..\dialogs\dwnldpwd\dwnldpwd.h"
#include "security.h"

extern CPpsApp    theApp;

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSecurity

IMPLEMENT_DYNCREATE(CSecurity, CPpsFormView)

CSecurity::CSecurity()
	: CPpsFormView(CSecurity::IDD)
{
	m_szOtaPassword = "";
	m_szReleasePassword = "";
	m_szSerialAccessPassword = "";
	//{{AFX_DATA_INIT(CSecurity)
	m_bOtaDisable = FALSE;
	m_bSerialAccessPwdRequired = FALSE;
	//}}AFX_DATA_INIT
}

CSecurity::~CSecurity()
{
}

void CSecurity::DoDataExchange(CDataExchange* pDX)
{
	CPpsFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSecurity)
	DDX_Check(pDX, IDC_OTA_CHECK, m_bOtaDisable);
	DDX_Check(pDX, IDC_SERIAL_CHECK, m_bSerialAccessPwdRequired);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSecurity, CPpsFormView)
	//{{AFX_MSG_MAP(CSecurity)
	ON_BN_CLICKED(IDC_SEC_UNDO_BTN, OnUndoBtn)
	ON_BN_CLICKED(IDC_CHG_OTA_PWD_BTN, OnChgOtaPwdBtn)
	ON_BN_CLICKED(IDC_CHG_OTAREL_PWD_BTN, OnChgOtaReleasePwdBtn)
	ON_BN_CLICKED(IDC_CHG_SER_PWD_BTN, OnChgSerialPwdBtn)
	ON_BN_CLICKED(IDC_SERIAL_CHECK, OnSerialCheck)
	//}}AFX_MSG_MAP
	//ON_BN_CLICKED(IDC_SEC_DEFAULTS_BTN, OnAcceptDefaultsBtn)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSecurity diagnostics

#ifdef _DEBUG
void CSecurity::AssertValid() const
{
	CPpsFormView::AssertValid();
}

void CSecurity::Dump(CDumpContext& dc) const
{
	CPpsFormView::Dump(dc);
}
#endif //_DEBUG



////////////////////////////////////////////////////////////////////////////
//	OnInitialUpdate()
//  
//  Update DDX variables from document and resize frame around view.
//
void CSecurity::OnInitialUpdate(void)
{
  TRACE("CSecurity::OnInitialUpdate : CSecurity::IDD=%X\n", CSecurity::IDD);
  
  CPpsFormView::OnInitialUpdate();
  return;
}




/////////////////////////////////////////////////////////////////////////////
//	OnUpdate()
//
void CSecurity::OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint)
{
  TRACE("\nCSecurity::OnUpdate : nViewID=%X  LHintHI=%X  lHintLO=%X\n", 
  		CSecurity::IDD,  HIWORD(lHint), LOWORD(lHint));

  if (HIWORD(lHint))        		// user switched views, now active
    ResizeParentToFit(FALSE);

  SetFromDoc();
  CPpsFormView::OnUpdate(pSender, lHint, pHint);
}	


////////////////////////////////////////////////////////////////////////////
//  SetFromDoc()
//
void CSecurity::SetFromDoc(void)
{
  TRACE("\nCSecurity::SetFromDoc\n");
  CPpsDoc	*pDoc = GetDocument();
  char cCurrentPwd[11];
  char cDecodedPwd[11];
  
  // get and decode the OTA Access Password
  pDoc->Field(CP_OTA_Password.Offset(), CP_OTA_Password.Length(), (BYTE *)cCurrentPwd);
  cCurrentPwd[10] = '\0';
  theApp.GetThePager()->DecodePassword(cCurrentPwd, cDecodedPwd);
  cDecodedPwd[10] = '\0'; 
  m_szOtaPassword = cDecodedPwd;
  
  // get and decode the OTA Release Features Access Password
  pDoc->Field(CP_Release_Password.Offset(), CP_Release_Password.Length(), (BYTE *)cCurrentPwd);
  cCurrentPwd[10] = '\0';
  theApp.GetThePager()->DecodePassword(cCurrentPwd, cDecodedPwd);
  cDecodedPwd[10] = '\0'; 
  m_szReleasePassword = cDecodedPwd; 
  
  // get and decode the Serial Port Access Password
  pDoc->Field(CP_DL_Password.Offset(), CP_DL_Password.Length(), (BYTE *)cCurrentPwd);
  cCurrentPwd[10] = '\0';
  theApp.GetThePager()->DecodePassword(cCurrentPwd, cDecodedPwd);
  cDecodedPwd[10] = '\0'; 
  m_szSerialAccessPassword = cDecodedPwd;
  
  // ota disable
  CP_rf_programming_disabled.bSetFromDoc(&m_bOtaDisable);
  
  // serial access password required
  CP_dl_security.bSetFromDoc(&m_bSerialAccessPwdRequired);
   
  UpdateData(FALSE);	// set controls data
}



////////////////////////////////////////////////////////////////////////////
//	SetToDoc()
//
BOOL CSecurity::SetToDoc(void)
{
  TRACE("\nCSecurity::SetToDoc\n");
  
  if (!(UpdateData(TRUE)))  // get controls data
    return FALSE;
  
  // ota disable
  CP_rf_programming_disabled.bSetToDoc(m_bOtaDisable);
  
  // serial access password required
  CP_dl_security.bSetToDoc(m_bSerialAccessPwdRequired);

  return(TRUE);
}




/////////////////////////////////////////////////////////////////////////////
// CSecurity message handlers




////////////////////////////////////////////////////////////////////////
//  OnAcceptDefaultsBtn()
//
//  This function initiates a move of data from select fields of the
//  CPpsDoc.m_byDefaultCodePlugArray to the CPpsDoc.m_byCodePlugArray.
//  The m_byDefaultCodePlugArray is loaded from file 'default.s19' upon
//  application execution.  If the file has not been found, then no 
//  default values are available.  
// 
/*
void CSecurity::OnAcceptDefaultsBtn()
{
  TRACE("\nCSecurity::OnAcceptDefaultsBtn\n"); 
  CPpsDoc	*pDoc = GetDocument(); 
  
  if (pDoc->SetFieldFromDefault(CP_OTA_Protected_L.Offset(), CP_OTA_Protected_L.Length()))
    return;  
  if (pDoc->SetFieldFromDefault(CP_System_Type_1_L.Offset(), CP_System_Type_1_L.Length()))
    return;    
  //if (pDoc->SetFieldFromDefault(CP_OTA_Password.Offset(), CP_OTA_Password.Length()))
  //  return;    
  //if (pDoc->SetFieldFromDefault(CP_Release_Password.Offset(), CP_Release_Password.Length()))
  //  return;
  //if (pDoc->SetFieldFromDefault(CP_DL_Password.Offset(), CP_DL_Password.Length()))
  //  return;
  SetFromDoc();   // update this view
  return;
}
*/


////////////////////////////////////////////////////////////////////////
//  OnArchived()
//
//  This function initiates a move of data from select fields of the
//  CPpsDoc.m_byArchivedCodePlugArray to the CPpsDoc.m_byCodePlugArray.
//
/*
void CSecurity::OnArchived(void)
{
  CPpsDoc	*pDoc = GetDocument(); 

  if (pDoc->SetFieldFromArchived(CP_OTA_Protected_L.Offset(), CP_OTA_Protected_L.Length()))
    return;  
  if (pDoc->SetFieldFromArchived(CP_System_Type_1_L.Offset(), CP_System_Type_1_L.Length()))
    return;    
  //if (pDoc->SetFieldFromArchived(CP_OTA_Password.Offset(), CP_OTA_Password.Length()))
  //  return;    
  //if (pDoc->SetFieldFromArchived(CP_Release_Password.Offset(), CP_Release_Password.Length()))
  //  return;
  //if (pDoc->SetFieldFromArchived(CP_DL_Password.Offset(), CP_DL_Password.Length()))
  //  return;
   
  SetFromDoc();   // update this view
}
*/


////////////////////////////////////////////////////////////////////////
//  OnUndoBtn()
//
//  This function initiates a move of data from select fields of the
//  CPpsDoc.m_byOriginalCodePlugArray to the CPpsDoc.m_byCodePlugArray.
//  The m_byOriginalCodePlugArray is loaded upon 'pager-read' or 
//  'file-open' menu or toolbar actions.
//
void CSecurity::OnUndoBtn()
{
  TRACE("\nCSecurity::OnUndoBtn\n"); 
  CPpsDoc	*pDoc = GetDocument(); 

  if (pDoc->SetFieldFromOriginal(CP_OTA_Protected_L.Offset(), CP_OTA_Protected_L.Length()))
    return;  
  if (pDoc->SetFieldFromOriginal(CP_System_Type_1_L.Offset(), CP_System_Type_1_L.Length()))
    return;    
  if (pDoc->SetFieldFromOriginal(CP_OTA_Password.Offset(), CP_OTA_Password.Length()))
    return;    
  if (pDoc->SetFieldFromOriginal(CP_Release_Password.Offset(), CP_Release_Password.Length()))
    return;
  if (pDoc->SetFieldFromOriginal(CP_DL_Password.Offset(), CP_DL_Password.Length()))
    return;
  SetFromDoc();   // update this view 
  return;
}



void CSecurity::OnChgOtaPwdBtn()
{
  if (OnChangePassword(m_szOtaPassword))
    ChangePassword(CP_OTA_Password.Offset(), CP_OTA_Password.Length(), m_szOtaPassword);
  return;  	
}


void CSecurity::OnChgOtaReleasePwdBtn()
{
  if (OnChangePassword(m_szReleasePassword))
    ChangePassword(CP_Release_Password.Offset(), CP_Release_Password.Length(), m_szReleasePassword);
  return;  	
}


void CSecurity::OnChgSerialPwdBtn()
{ 
  if (OnChangePassword(m_szSerialAccessPassword))
  {
    ChangePassword(CP_DL_Password.Offset(), CP_DL_Password.Length(), m_szSerialAccessPassword); 
    theApp.GetThePager()->SetPagerPassword((LPCTSTR)m_szSerialAccessPassword);
  } 
  return;  	
}


int CSecurity::OnChangePassword(CString &rCurrentPwd)
{
  CDwnLdPwd dlg;
  int ret;
  
  // return 1 on change or 0 on no_change
  do 
  {
    if (((CMainFrame*)(theApp.m_pMainWnd))->IsAccessRestricted())
      ret = dlg.DoModal();
    else
      ret = dlg.DoModal((LPCTSTR)rCurrentPwd);
    if (ret == IDCANCEL)
      return(0);
    TRACE("\nCSecurity::OnChangePassword : %s : %s\n",
          (LPCTSTR)dlg.m_Edit1, (LPCTSTR)dlg.m_Edit2);
    if (dlg.m_Edit1 != dlg.m_Edit2)
    {
      AfxMessageBox("The two password entries must compare exactly.", 
      			    MB_ICONINFORMATION | MB_APPLMODAL | MB_OK);      
      continue;
    } 
    if (dlg.m_Edit1.GetLength() > 0)
      rCurrentPwd = dlg.m_Edit1;
    break;
  } while(1); 
  return(1);  	
}


static char *szWarning = "Be sure to make a note of the current password.\n"
                         "Access to the device will be impossible if the "
                         "password is not known.";
void CSecurity::ChangePassword(int nField, int nFieldLen, CString& rCurrentPwd)
{ 
  TRACE("\nCSecurity::ChangePassword\n"); 
  CPpsDoc	*pDoc = GetDocument();
  char szEncodedPwd[11];

  // pad with spaces if necessary 
  while (rCurrentPwd.GetLength() < 10)
    rCurrentPwd += ' ';
  theApp.GetThePager()->EncodePassword((LPCTSTR)rCurrentPwd, szEncodedPwd);
  TRACE("\nCSecurity::ChangePassword : %s : %s\n", (LPCTSTR)rCurrentPwd, szEncodedPwd);
  pDoc->SetField(nField, nFieldLen, (BYTE *)szEncodedPwd);

  AfxMessageBox(szWarning);
  return;
}

 

int CSecurity::OnPrintHelper(CStringArray &rStringArray, UINT nPageNumber, int nAvailableLines)
{
   // if not enough lines available, wait for next page...
   const int nNumLinesRequired = 4;    // count the number of 'rStringArray.Add' calls
   if (nAvailableLines < nNumLinesRequired)
     return(-1);    
   int nNumLinesBegin = rStringArray.GetSize();
   
   char szTemp[100];
   
   rStringArray.Add("SECURITY");
     //
     sprintf(szTemp, "    OTA Programming:  %d", m_bOtaDisable);
     rStringArray.Add(szTemp);
     // 
     sprintf(szTemp, "    Serial Access Password:  %d", m_bSerialAccessPwdRequired);
     rStringArray.Add(szTemp);

   rStringArray.Add("");
   return(rStringArray.GetSize() - nNumLinesBegin);
}


 

void CSecurity::OnSerialCheck()
{ 
  AfxMessageBox(szWarning); 
  if (m_szSerialAccessPassword.GetLength() > 0)
    theApp.GetThePager()->SetPagerPassword((LPCTSTR)m_szSerialAccessPassword);
}
