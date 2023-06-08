//*************************************************************
//  History:    Date       Author               Comment
//              1996       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
// pagerid.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\pps.h"
#include "..\..\ppsdoc.h"
#include "..\..\ppsview.h" 
#include "..\..\mainfrm.h"
#include "..\..\cpager\cpager.h"
#include "..\..\cpager\codeplug.h" 
#include "pagerid.h"

extern CPpsApp    theApp;

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPagerId

IMPLEMENT_DYNCREATE(CPagerId, CPpsFormView)

CPagerId::CPagerId()
	: CPpsFormView(CPagerId::IDD)
{
    bReflex50Enabled = FALSE;
	//{{AFX_DATA_INIT(CPagerId)
	m_byCodeplugVersion = 0;
	m_byOtaRevision = 0;
	m_szPowerUpPrompt = "";
	m_szSerialNum = "";
	m_szIdBlockInfo = "";
	m_szInventoryControlNum = "";
	m_byDspPatchLevel = 0;
	m_byHostPatchLevel = 0;
	m_szCapCode = "";
	//}}AFX_DATA_INIT
}

CPagerId::~CPagerId()
{
}

void CPagerId::DoDataExchange(CDataExchange* pDX)
{
	CPpsFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPagerId)
	DDX_Text(pDX, IDC_CP_VER_EDIT, m_byCodeplugVersion);
	DDV_MinMaxByte(pDX, m_byCodeplugVersion, 0, 255);
	DDX_Text(pDX, IDC_OTA_REV_EDIT, m_byOtaRevision);
	DDV_MinMaxByte(pDX, m_byOtaRevision, 0, 255);
	DDX_Text(pDX, IDC_PU_PROMPT_EDIT, m_szPowerUpPrompt);
	DDV_MaxChars(pDX, m_szPowerUpPrompt, 20);
	DDX_Text(pDX, IDC_SN_EDIT, m_szSerialNum);
	DDX_Text(pDX, IDC_IDBLK_INFO_EDIT, m_szIdBlockInfo);
	DDV_MaxChars(pDX, m_szIdBlockInfo, 300);
	DDX_Text(pDX, IDC_ICN_EDIT, m_szInventoryControlNum);
	DDV_MaxChars(pDX, m_szInventoryControlNum, 10);
	DDX_Text(pDX, IDC_DSP_PATCH_EDIT, m_byDspPatchLevel);
	DDV_MinMaxByte(pDX, m_byDspPatchLevel, 0, 255);
	DDX_Text(pDX, IDC_HOST_PATCH_EDIT, m_byHostPatchLevel);
	DDV_MinMaxByte(pDX, m_byHostPatchLevel, 0, 255);
	DDX_Text(pDX, IDC_CAPCODE_EDIT, m_szCapCode);
	DDV_MaxChars(pDX, m_szCapCode, 15);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPagerId, CPpsFormView)
	//{{AFX_MSG_MAP(CPagerId)
	ON_BN_CLICKED(IDC_DEFAULTS_BTN, OnAcceptDefaultsBtn)
	ON_BN_CLICKED(IDC_UNDO_BTN, OnUndoBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPagerId diagnostics

#ifdef _DEBUG
void CPagerId::AssertValid() const
{
	CPpsFormView::AssertValid();
}

void CPagerId::Dump(CDumpContext& dc) const
{
	CPpsFormView::Dump(dc);
}
#endif //_DEBUG



////////////////////////////////////////////////////////////////////////////
//	OnInitialUpdate()
//  
//  Update DDX variables from document and resize frame around view.
//
void CPagerId::OnInitialUpdate(void)
{
  TRACE("CPagerId::OnInitialUpdate : nThisViewID=%X\n", CPagerId::IDD);
  
  CEdit	*pEdit = NULL;
  if (((CMainFrame*)(theApp.m_pMainWnd))->IsAccessRestricted())   // if not lab-use only PPS
  {
    pEdit = (CEdit *)GetDlgItem(IDC_SN_EDIT);   // disable Serial Number Modification
    ASSERT(pEdit != NULL);
    pEdit->EnableWindow(FALSE);
    pEdit = (CEdit *)GetDlgItem(IDC_ICN_EDIT);   // disable Inventory Control Number Modification
    ASSERT(pEdit != NULL);
    pEdit->EnableWindow(FALSE);
  }
  CPpsFormView::OnInitialUpdate();
  return;   
}




/////////////////////////////////////////////////////////////////////////////
//	OnUpdate()
//
void CPagerId::OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint)
{
  TRACE("\nCPagerId::OnUpdate : nViewID=%X  LHintHI=%X  lHintLO=%X\n", 
  		CPagerId::IDD,  HIWORD(lHint), LOWORD(lHint));
  
  if (HIWORD(lHint))        		// user switched views, now active
    ResizeParentToFit(FALSE);

  // check for Reflex50 enabled
  CP_reflex50_enable.bSetFromDoc(&bReflex50Enabled);
  // make screen element visible accordingly 
  CStatic *pStatic = (CStatic *)GetDlgItem(IDC_CAPCODE_STATIC);
  ASSERT(pStatic != NULL);
  CEdit *pEdit = (CEdit*)GetDlgItem(IDC_CAPCODE_EDIT);
  ASSERT(pEdit != NULL);
  if (bReflex50Enabled)
  {
    pStatic->ShowWindow(SW_SHOW);
    pEdit->ShowWindow(SW_SHOW);
  }
  else
  {
    pStatic->ShowWindow(SW_HIDE);
    pEdit->ShowWindow(SW_HIDE);
  }
  
  SetFromDoc();
  CPpsFormView::OnUpdate(pSender, lHint, pHint);
}	


////////////////////////////////////////////////////////////////////////////
//  SetFromDoc()
//
void CPagerId::SetFromDoc(void)
{
  TRACE("\nCPagerId::SetFromDoc\n");
  CPpsDoc	*pDoc = GetDocument();
  
  // set this view's member data
                                                                         
  // build the capcode string from various fields - as performed in Tango  
  if (bReflex50Enabled == TRUE)  
  {
    BYTE	byPagerFrame, byPersonalCollapse;
    DWORD 	dwPersonalAddrs;
    char	szBuff[20];
    CP_PagerFrame.bySetFromDoc(&byPagerFrame);
    CP_PersonalCollapse.bySetFromDoc(&byPersonalCollapse);
    CP_Personal_addrs.dwSetFromDoc(&dwPersonalAddrs); 
    sprintf(szBuff, "%03d%01d%010lu", byPagerFrame, byPersonalCollapse, 
            (unsigned long)dwPersonalAddrs);
    m_szCapCode = szBuff;
  } 
  else
    m_szCapCode = "";   
  
  // CP_Serial_Number
  pDoc->Field(CP_Serial_Number.Offset(), CP_Serial_Number.Length(), 
              (BYTE *)m_szSerialNum.GetBuffer(CP_Serial_Number.Length()));
  m_szSerialNum.ReleaseBuffer(CP_Serial_Number.Length());
  
  // CP_Version
  pDoc->Field(CP_Version.Offset(), CP_Version.Length(), &m_byCodeplugVersion);
  
  // CP_OTA_Subheader
  pDoc->Field(CP_OTA_Subheader.Offset() + 4, 1, &m_byOtaRevision);
  
  // CP_Inventory_Control_Number
  pDoc->Field(CP_Inventory_Control_Number.Offset(), 
              CP_Inventory_Control_Number.Length(),
              (BYTE *)m_szInventoryControlNum.GetBuffer(CP_Inventory_Control_Number.Length()));
  m_szInventoryControlNum.ReleaseBuffer(CP_Inventory_Control_Number.Length());
  
  // CP_AP_Powerup
  pDoc->Field(CP_AP_Powerup.Offset(), 
              CP_AP_Powerup.Length(), 
              (BYTE *)m_szPowerUpPrompt.GetBuffer(CP_AP_Powerup.Length()));
  m_szPowerUpPrompt.ReleaseBuffer(CP_AP_Powerup.Length());
  
  // CP_HostPatchLevel
  pDoc->Field(CP_HostPatchLevel.Offset(), 1, &m_byHostPatchLevel);
  
  // CP_SPPatchLevel
  pDoc->Field(CP_SPPatchLevel.Offset(), 1, &m_byDspPatchLevel); 
  
  // info block - the document path indicates whether the document
  // is an opened S-Record File or if the document is from an attached pager.
  if (pDoc->GetPathName().IsEmpty() == TRUE)
    theApp.GetThePager()->GetPagerIdBlockInfo(m_szIdBlockInfo);
  else
    m_szIdBlockInfo.Empty();
  
  UpdateData(FALSE);	// set controls data
}


////////////////////////////////////////////////////////////////////////////
//	SetToDoc()
//
BOOL CPagerId::SetToDoc(void)
{
  TRACE("\nCPagerId::SetToDoc\n");
  CPpsDoc	*pDoc = GetDocument();
  CEdit *pEdit = NULL;
  
  if (!(UpdateData(TRUE)))  // get controls data
    return FALSE;
  
  pEdit = (CEdit *)GetDlgItem(IDC_SN_EDIT);
  ASSERT(pEdit != NULL);
  if (pEdit->GetModify()) 
    CP_Serial_Number.szSetToDoc((LPCTSTR)m_szSerialNum, 10);

  pEdit = (CEdit *)GetDlgItem(IDC_ICN_EDIT);
  ASSERT(pEdit != NULL);
  if (pEdit->GetModify()) 
    CP_Inventory_Control_Number.szSetToDoc((LPCTSTR)m_szInventoryControlNum, 10);
   
  pEdit = (CEdit *)GetDlgItem(IDC_PU_PROMPT_EDIT);
  ASSERT(pEdit != NULL);
  if (pEdit->GetModify()) 
    CP_AP_Powerup.szSetToDoc((LPCTSTR)m_szPowerUpPrompt, 20);
  
  return(TRUE);
}





/////////////////////////////////////////////////////////////////////////////
// CPagerId message handlers




////////////////////////////////////////////////////////////////////////
//  OnAcceptDefaultsBtn()
//
//  This function initiates a move of data from select fields of the
//  CPpsDoc.m_byDefaultCodePlugArray to the CPpsDoc.m_byCodePlugArray.
//  The m_byDefaultCodePlugArray is loaded from file 'default.s19' upon
//  application execution.  If the file has not been found, then no 
//  default values are available.  
//
void CPagerId::OnAcceptDefaultsBtn()
{
  TRACE("\nCPagerId::OnAcceptDefaultsBtn\n"); 
  CPpsDoc	*pDoc = GetDocument(); 
  
  if (pDoc->SetFieldFromDefault(CP_AP_Powerup.Offset(), CP_AP_Powerup.Length()))
    return;

  SetFromDoc();   // update this view
}


////////////////////////////////////////////////////////////////////////
//  OnArchived()
//
//  This function initiates a move of data from select fields of the
//  CPpsDoc.m_byArchivedCodePlugArray to the CPpsDoc.m_byCodePlugArray.
//
void CPagerId::OnArchived(void)
{
  CPpsDoc	*pDoc = GetDocument(); 
   
  if (pDoc->SetFieldFromArchived(CP_AP_Powerup.Offset(), CP_AP_Powerup.Length()))
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
void CPagerId::OnUndoBtn()
{
  TRACE("\nCPagerId::OnUndoBtn\n"); 
  CPpsDoc	*pDoc = GetDocument(); 
  
  if (pDoc->SetFieldFromOriginal(CP_AP_Powerup.Offset(), CP_AP_Powerup.Length()))
    return;

  SetFromDoc();   // update this view
}



int CPagerId::OnPrintHelper(CStringArray &rStringArray, UINT nPageNumber, int nAvailableLines)
{
   // if not enough lines available, wait for next page...
   const int nNumLinesRequired = 6;
   if (nAvailableLines < nNumLinesRequired)
     return(-1);    
   int nNumLinesBegin = rStringArray.GetSize();

   // check for Reflex50 enabled
   CP_reflex50_enable.bSetFromDoc(&bReflex50Enabled);

   CString	szLocal;
   
   rStringArray.Add("INVENTORY");

   szLocal =  "    Serial Number:  ";  
   szLocal += m_szSerialNum;
   rStringArray.Add(szLocal);
   
   szLocal =  "    Inventory Control Number:  ";  
   szLocal += m_szInventoryControlNum;
   rStringArray.Add(szLocal);  
   
   szLocal =  "    Power-Up Prompt:  ";  
   szLocal += m_szPowerUpPrompt;
   rStringArray.Add(szLocal);
   
   if (bReflex50Enabled)
   {
     szLocal =  "    Capcode:  ";
     szLocal += m_szCapCode;
     rStringArray.Add(szLocal);
   }      
   
   rStringArray.Add("");  // newline
   return(rStringArray.GetSize() - nNumLinesBegin);
}




