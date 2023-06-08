//*************************************************************
//  History:    Date       Author               Comment
//              1996       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
// address.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\pps.h"
#include "..\..\ppsdoc.h"
#include "..\..\ppsview.h" 
#include "..\..\mainfrm.h"
#include "..\..\cpager\codeplug.h" 
#include "address.h"
#include "infosvcs.h"

extern CPpsApp    theApp;

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddressInfo

IMPLEMENT_DYNCREATE(CAddressInfo, CPpsFormView)

CAddressInfo::CAddressInfo()
	: CPpsFormView(CAddressInfo::IDD)
{
	//{{AFX_DATA_INIT(CAddressInfo)
	m_dwElectronicSerialNum = 1;
	m_dwPersonalAddress2 = 1;
	m_dwPersonalAddress1 = 1;
	m_dwInfoSvcAddr0 = 1;
	m_dwInfoSvcAddr1 = 1;
	m_dwInfoSvcAddr2 = 1;
	m_dwInfoSvcAddr3 = 1;
	m_dwInfoSvcAddr4 = 1;
	m_dwInfoSvcAddr5 = 1;
	m_bInfoSvc0Disable = FALSE;
	m_bInfoSvc1Disable = FALSE;
	m_bInfoSvc2Disable = FALSE;
	m_bInfoSvc3Disable = FALSE;
	m_bInfoSvc4Disable = FALSE;
	m_bInfoSvc5Disable = FALSE;
	//}}AFX_DATA_INIT
    bR50Enable = FALSE;
    dwOriginalPersAddr1 = 1;
    dwOriginalEsn = 1;
}

CAddressInfo::~CAddressInfo()
{
}


static const DWORD dwUnusedPersonalAddr = 0xFFFFFFFF;
static const char szEmptyString[] = "";  
#ifdef MTEL_AFFILIATE_PPS
  static const DWORD dwMinPersAddr = 1;
#else
  static const DWORD dwMinPersAddr = 0;
#endif

void CAddressInfo::DoDataExchange(CDataExchange* pDX)
{ 
	CPpsFormView::DoDataExchange(pDX);
	
	//{{AFX_DATA_MAP(CAddressInfo)
	DDX_Check(pDX, IDC_ISVC_DISABLE_CHK0, m_bInfoSvc0Disable);
	DDX_Check(pDX, IDC_ISVC_DISABLE_CHK1, m_bInfoSvc1Disable);
	DDX_Check(pDX, IDC_ISVC_DISABLE_CHK2, m_bInfoSvc2Disable);
	DDX_Check(pDX, IDC_ISVC_DISABLE_CHK3, m_bInfoSvc3Disable);
	DDX_Check(pDX, IDC_ISVC_DISABLE_CHK4, m_bInfoSvc4Disable);
	DDX_Check(pDX, IDC_ISVC_DISABLE_CHK5, m_bInfoSvc5Disable);
	//}}AFX_DATA_MAP   
	
    // Do DDX/DDV on ESN only if Reflex50 pager & LabUse PPS
	if (bR50Enable == TRUE)
	{
	  DDX_Text(pDX, IDC_ESN_EDIT, m_dwElectronicSerialNum);
	  DDV_MinMaxDWord(pDX, m_dwElectronicSerialNum, 1, 0x3FFFFFFF);
	}
	
	// if the codeplug personal address is unused, don't display
	// it.  If the user has typed a value therein, validate it.
	// 
	if (pDX->m_bSaveAndValidate == FALSE) // to the screen
	{
	  if (m_dwPersonalAddress1 == dwUnusedPersonalAddr)	 
	    PersAddrEdit1.SetWindowText(szEmptyString);
	  else
	  {
	    DDX_Text(pDX, IDC_PER_ADDR1_EDIT, m_dwPersonalAddress1);
	    DDV_MinMaxDWord(pDX, m_dwPersonalAddress1, dwMinPersAddr, 0x3FFFFFFF); 
	  }
	}
	else if (pDX->m_bSaveAndValidate == TRUE)  // from the screen
	{
	   if (PersAddrEdit1.GetWindowTextLength() > 0)
	   {
	     DDX_Text(pDX, IDC_PER_ADDR1_EDIT, m_dwPersonalAddress1);
	     DDV_MinMaxDWord(pDX, m_dwPersonalAddress1, dwMinPersAddr, 0x3FFFFFFF); 
	   }
	   else if (PersAddrEdit1.GetModify() == TRUE)
	     m_dwPersonalAddress1 = dwUnusedPersonalAddr;
    }
    
	//
	if (pDX->m_bSaveAndValidate == FALSE) // to the screen
	{
	  if (m_dwPersonalAddress2 == dwUnusedPersonalAddr)	 
	    PersAddrEdit2.SetWindowText(szEmptyString);
	  else 
	  {
	    DDX_Text(pDX, IDC_PER_ADDR2_EDIT, m_dwPersonalAddress2);
	    DDV_MinMaxDWord(pDX, m_dwPersonalAddress2, dwMinPersAddr, 0x3FFFFFFF);
	  } 
	}
	else if (pDX->m_bSaveAndValidate == TRUE)  // from the screen
	{
	   if (PersAddrEdit2.GetWindowTextLength() > 0)
	   {
	     DDX_Text(pDX, IDC_PER_ADDR2_EDIT, m_dwPersonalAddress2);
	     DDV_MinMaxDWord(pDX, m_dwPersonalAddress2, dwMinPersAddr, 0x3FFFFFFF); 
	   }
	   else if (PersAddrEdit2.GetModify() == TRUE)
	     m_dwPersonalAddress2 = dwUnusedPersonalAddr;
    }
	
	 
	// If the InfoService field is disabled, don't do xfer or
	// validation. 
	if (!m_bInfoSvc0Disable)
	{
	  DDX_Text(pDX, IDC_SVC_ADDR_EDIT0, m_dwInfoSvcAddr0); 
	  if (bR50Enable)
	   DDV_MinMaxDWord(pDX, m_dwInfoSvcAddr0, 0, 65534);  // Reflex50
	  else
	   DDV_MinMaxDWord(pDX, m_dwInfoSvcAddr0, 0x4000, 0xFFFFFF);  // Reflex25	   
    }
    else
      InfoSvcEdit0.SetWindowText(szEmptyString);
    //  
    if (!m_bInfoSvc1Disable)
    {
	  DDX_Text(pDX, IDC_SVC_ADDR_EDIT1, m_dwInfoSvcAddr1);
	  if (bR50Enable)
	    DDV_MinMaxDWord(pDX, m_dwInfoSvcAddr1, 0, 65534); // Reflex50
	  else
	    DDV_MinMaxDWord(pDX, m_dwInfoSvcAddr1, 0x4000, 0xFFFFFF);  // Reflex25	   
    } 
    else
      InfoSvcEdit1.SetWindowText(szEmptyString);
    //  
    if (!m_bInfoSvc2Disable)
    {
	  DDX_Text(pDX, IDC_SVC_ADDR_EDIT2, m_dwInfoSvcAddr2);
	  if (bR50Enable)
  	    DDV_MinMaxDWord(pDX, m_dwInfoSvcAddr2, 0, 65534); // Reflex50
  	  else
	    DDV_MinMaxDWord(pDX, m_dwInfoSvcAddr2, 0x4000, 0xFFFFFF);  // Reflex25	   
    }
    else
      InfoSvcEdit2.SetWindowText(szEmptyString);
    //  
    if (!m_bInfoSvc3Disable)
    {
	  DDX_Text(pDX, IDC_SVC_ADDR_EDIT3, m_dwInfoSvcAddr3);
	  if (bR50Enable)
	    DDV_MinMaxDWord(pDX, m_dwInfoSvcAddr3, 0, 65534);  // Reflex50
  	  else
	    DDV_MinMaxDWord(pDX, m_dwInfoSvcAddr3, 0x4000, 0xFFFFFF);  // Reflex25	   
    }
    else
      InfoSvcEdit3.SetWindowText(szEmptyString);
    //  
    if (!m_bInfoSvc4Disable)
    {
	  DDX_Text(pDX, IDC_SVC_ADDR_EDIT4, m_dwInfoSvcAddr4);
	  if (bR50Enable)
	    DDV_MinMaxDWord(pDX, m_dwInfoSvcAddr4, 0, 65534);    // Reflex50
  	  else
	    DDV_MinMaxDWord(pDX, m_dwInfoSvcAddr4, 0x4000, 0xFFFFFF);  // Reflex25	   
    } 
    else
      InfoSvcEdit4.SetWindowText(szEmptyString);
    //  
    if (!m_bInfoSvc5Disable)
    {
	  DDX_Text(pDX, IDC_SVC_ADDR_EDIT5, m_dwInfoSvcAddr5);
	  if (bR50Enable)
	    DDV_MinMaxDWord(pDX, m_dwInfoSvcAddr5, 0, 65534);     // Reflex50
  	  else
	    DDV_MinMaxDWord(pDX, m_dwInfoSvcAddr5, 0x4000, 0xFFFFFF);  // Reflex25	   
	}
    else
      InfoSvcEdit5.SetWindowText(szEmptyString);
     
  return;	
}


BEGIN_MESSAGE_MAP(CAddressInfo, CPpsFormView)
	//{{AFX_MSG_MAP(CAddressInfo)
	ON_BN_CLICKED(IDC_ADDR_DEFAULTS_BTN, OnAcceptDefaultsBtn)
	ON_BN_CLICKED(IDC_ADDR_UNDO_BTN, OnUndoBtn)
	ON_BN_CLICKED(IDC_SUBADDR_BTN0, OnSubaddrBtn0)
	ON_BN_CLICKED(IDC_SUBADDR_BTN1, OnSubaddrBtn1)
	ON_BN_CLICKED(IDC_SUBADDR_BTN2, OnSubaddrBtn2)
	ON_BN_CLICKED(IDC_SUBADDR_BTN3, OnSubaddrBtn3)
	ON_BN_CLICKED(IDC_SUBADDR_BTN4, OnSubaddrBtn4)
	ON_BN_CLICKED(IDC_SUBADDR_BTN5, OnSubaddrBtn5)
	ON_EN_KILLFOCUS(IDC_PER_ADDR1_EDIT, OnKillFocusPerAddr1Edit)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_ISVC_DISABLE_CHK0, OnInfoSvcDisableChk)
	ON_BN_CLICKED(IDC_ISVC_DISABLE_CHK1, OnInfoSvcDisableChk)
	ON_BN_CLICKED(IDC_ISVC_DISABLE_CHK2, OnInfoSvcDisableChk)
	ON_BN_CLICKED(IDC_ISVC_DISABLE_CHK3, OnInfoSvcDisableChk)
	ON_BN_CLICKED(IDC_ISVC_DISABLE_CHK4, OnInfoSvcDisableChk)
	ON_BN_CLICKED(IDC_ISVC_DISABLE_CHK5, OnInfoSvcDisableChk)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAddressInfo diagnostics

#ifdef _DEBUG
void CAddressInfo::AssertValid() const
{
	CPpsFormView::AssertValid();
}



void CAddressInfo::Dump(CDumpContext& dc) const
{
	CPpsFormView::Dump(dc);
}

#endif //_DEBUG





////////////////////////////////////////////////////////////////////////////
//	OnInitialUpdate()
//  
//  Update DDX variables from document and resize frame around view.
//
void CAddressInfo::OnInitialUpdate(void)
{
  TRACE("CAddressInfo::OnInitialUpdate : thisViewID=%X\n", CAddressInfo::IDD);

  // subclass the edit controls in order to force numeric only input.
  PersAddrEdit1.SubclassDlgItem(IDC_PER_ADDR1_EDIT, this);
  PersAddrEdit1.LimitText(10);
    
  PersAddrEdit2.SubclassDlgItem(IDC_PER_ADDR2_EDIT, this);
  PersAddrEdit2.LimitText(10);
    
  InfoSvcEdit0.SubclassDlgItem(IDC_SVC_ADDR_EDIT0, this);
  InfoSvcEdit0.LimitText(10);
  
  InfoSvcEdit1.SubclassDlgItem(IDC_SVC_ADDR_EDIT1, this);
  InfoSvcEdit1.LimitText(10);
    
  InfoSvcEdit2.SubclassDlgItem(IDC_SVC_ADDR_EDIT2, this);
  InfoSvcEdit2.LimitText(10);
  
  InfoSvcEdit3.SubclassDlgItem(IDC_SVC_ADDR_EDIT3, this);
  InfoSvcEdit3.LimitText(10);
    
  InfoSvcEdit4.SubclassDlgItem(IDC_SVC_ADDR_EDIT4, this);
  InfoSvcEdit4.LimitText(10);
    
  InfoSvcEdit5.SubclassDlgItem(IDC_SVC_ADDR_EDIT5, this);
  InfoSvcEdit5.LimitText(10); 
  
  CEdit	*pEdit = NULL;
  if (((CMainFrame*)(theApp.m_pMainWnd))->IsAccessRestricted())   // if not lab-use only PPS
  {
    pEdit = (CEdit *)GetDlgItem(IDC_ESN_EDIT); // disable Electronic Serial Number Modification
    ASSERT(pEdit != NULL);
    pEdit->EnableWindow(FALSE);
  }  
  
  #ifdef MTEL_AFFILIATE_PPS
    pEdit = (CEdit *)GetDlgItem(IDC_PER_ADDR2_EDIT); // make the 2nd personal address field read-only
    ASSERT(pEdit != NULL);
    pEdit->EnableWindow(FALSE);
  #endif
   
  CPpsFormView::OnInitialUpdate();  
  return;
}




/////////////////////////////////////////////////////////////////////////////
//	OnUpdate()
//
void CAddressInfo::OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint)
{
  TRACE("\nCAddressInfo::OnUpdate : nCAddressInfo::IDD=%X  LHintHI=%X  lHintLO=%X\n", 
  		CAddressInfo::IDD,  HIWORD(lHint), LOWORD(lHint));

  if (HIWORD(lHint))        		// user switched views, now active
    ResizeParentToFit(FALSE);

  // check for Reflex50 enabled, hide Reflex50 elements if this is Reflex25 unit
  // or an Mtel Affiliate PPS.
  CP_reflex50_enable.bSetFromDoc(&bR50Enable);
  CEdit *pEdit1 = (CEdit *)GetDlgItem(IDC_ESN_EDIT);
  ASSERT(pEdit1 != NULL);
  CStatic *pStatic1 = (CStatic *)GetDlgItem(IDC_R50ESN_STATIC);
  ASSERT(pStatic1 != NULL);
  CStatic *pStatic2 = (CStatic *)GetDlgItem(IDC_ESNTXT1_STATIC);
  ASSERT(pStatic2 != NULL);
  CStatic *pStatic3 = (CStatic *)GetDlgItem(IDC_ESNTXT2_STATIC);
  ASSERT(pStatic3 != NULL);
  CEdit *pEdit2 = (CEdit *)GetDlgItem(IDC_PER_ADDR2_EDIT);
  ASSERT(pEdit2 != NULL); 
  #ifdef MTEL_AFFILIATE_PPS
    pEdit1->ShowWindow(SW_HIDE); 
    pStatic1->ShowWindow(SW_HIDE);
    pStatic2->ShowWindow(SW_HIDE);
    pStatic3->ShowWindow(SW_HIDE);
  #else
  if (bR50Enable == FALSE)
  {
    pEdit1->ShowWindow(SW_HIDE); 
    pStatic1->ShowWindow(SW_HIDE);
    pStatic2->ShowWindow(SW_HIDE);
    pStatic3->ShowWindow(SW_HIDE);
    pEdit2->ShowWindow(SW_HIDE); 
  }
  else
  {
    pEdit1->ShowWindow(SW_SHOW); 
    pStatic1->ShowWindow(SW_SHOW);
    pStatic2->ShowWindow(SW_SHOW);
    pStatic3->ShowWindow(SW_SHOW);
    pEdit2->ShowWindow(SW_SHOW); 
  }  
  #endif
  
  // obtain original personal address in order to enforce Mtel Engineering
  // Use only PA/ESN range...  
  if (bR50Enable)
  {
    CPpsDoc	*pDoc = GetDocument();
    pDoc->GetOriginalField(CP_Personal_addrs.Offset(), CP_Personal_addrs.Length(), (BYTE*)&dwOriginalPersAddr1);
    dwOriginalPersAddr1 = CpField::dwByteSwap(dwOriginalPersAddr1);  
    pDoc->GetOriginalField(CP_Electronic_Serial_Number.Offset(), CP_Electronic_Serial_Number.Length(), (BYTE*)&dwOriginalEsn);
    dwOriginalEsn = CpField::dwByteSwap(dwOriginalEsn);  
    TRACE("\nCAddressInfo::OnUpdate : dwOriginalPersAddr1=%08lX : dwOriginalEsn\n", dwOriginalPersAddr1, dwOriginalEsn);
  }
  
  SetFromDoc();
  CPpsFormView::OnUpdate(pSender, lHint, pHint);
}	


////////////////////////////////////////////////////////////////////////////
//  SetFromDoc()
//
void CAddressInfo::SetFromDoc(void)
{
  TRACE("\nCAddressInfo::SetFromDoc\n");
  CPpsDoc	*pDoc = GetDocument();
  

  // CP_Electronic_Serial_Number
  if (bR50Enable == TRUE)
  { 
    CP_Electronic_Serial_Number.dwSetFromDoc(&m_dwElectronicSerialNum);
  }
  
  // *** Information Service Addresses 
  CP_infosvc_addrs.dwSetFromDoc(&m_dwInfoSvcAddr0, 0);
  CP_infosvc_addrs.dwSetFromDoc(&m_dwInfoSvcAddr1, 4);
  CP_infosvc_addrs.dwSetFromDoc(&m_dwInfoSvcAddr2, 8);
  CP_infosvc_addrs.dwSetFromDoc(&m_dwInfoSvcAddr3, 12);
  CP_infosvc_addrs.dwSetFromDoc(&m_dwInfoSvcAddr4, 16);
  CP_infosvc_addrs.dwSetFromDoc(&m_dwInfoSvcAddr5, 20);

  // *** bit 31 is a disable bit - not documented 
  CP_infosvc_addrs_disable.bSetFromDoc(&m_bInfoSvc0Disable, 0);
  CP_infosvc_addrs_disable.bSetFromDoc(&m_bInfoSvc1Disable, 4);
  CP_infosvc_addrs_disable.bSetFromDoc(&m_bInfoSvc2Disable, 8);
  CP_infosvc_addrs_disable.bSetFromDoc(&m_bInfoSvc3Disable, 12);
  CP_infosvc_addrs_disable.bSetFromDoc(&m_bInfoSvc4Disable, 16);
  CP_infosvc_addrs_disable.bSetFromDoc(&m_bInfoSvc5Disable, 20);

  // *** Active Information Service SubAddresses 
  CP_active_subaddrs.dwSetFromDoc(&dwSubAddr0, 0);   
  CP_active_subaddrs.dwSetFromDoc(&dwSubAddr1, 4);   
  CP_active_subaddrs.dwSetFromDoc(&dwSubAddr2, 8);   
  CP_active_subaddrs.dwSetFromDoc(&dwSubAddr3, 12);   
  CP_active_subaddrs.dwSetFromDoc(&dwSubAddr4, 16);   
  CP_active_subaddrs.dwSetFromDoc(&dwSubAddr5, 20);   
  
  // *** Personal Addresses 
  CP_Personal_addrs.dwSetFromDoc(&m_dwPersonalAddress1, 0);
  CP_Personal_addrs.dwSetFromDoc(&m_dwPersonalAddress2, 4);
  
  UpdateData(FALSE);	// set controls data
}


  

////////////////////////////////////////////////////////////////////////////
//	SetToDoc()
//
BOOL CAddressInfo::SetToDoc(void)
{
  TRACE("\nCAddressInfo::SetToDoc\n");
  CPpsDoc		*pDoc = GetDocument();
  
  if (!(UpdateData(TRUE)))  // get controls data
    return FALSE;

  // *** Information Service Addresses 
  CP_infosvc_addrs.dwSetToDoc(m_dwInfoSvcAddr0, 0);
  CP_infosvc_addrs.dwSetToDoc(m_dwInfoSvcAddr1, 4);
  CP_infosvc_addrs.dwSetToDoc(m_dwInfoSvcAddr2, 8);
  CP_infosvc_addrs.dwSetToDoc(m_dwInfoSvcAddr3, 12);
  CP_infosvc_addrs.dwSetToDoc(m_dwInfoSvcAddr4, 16);
  CP_infosvc_addrs.dwSetToDoc(m_dwInfoSvcAddr5, 20);

  // *** bit 31 is a disable bit - not documented 
  CP_infosvc_addrs_disable.bSetToDoc(m_bInfoSvc0Disable, 0);
  CP_infosvc_addrs_disable.bSetToDoc(m_bInfoSvc1Disable, 4);
  CP_infosvc_addrs_disable.bSetToDoc(m_bInfoSvc2Disable, 8);
  CP_infosvc_addrs_disable.bSetToDoc(m_bInfoSvc3Disable, 12);
  CP_infosvc_addrs_disable.bSetToDoc(m_bInfoSvc4Disable, 16);
  CP_infosvc_addrs_disable.bSetToDoc(m_bInfoSvc5Disable, 20);

  // *** Active Information Service SubAddresses 
  CP_active_subaddrs.dwSetToDoc(dwSubAddr0, 0);   
  CP_active_subaddrs.dwSetToDoc(dwSubAddr1, 4);   
  CP_active_subaddrs.dwSetToDoc(dwSubAddr2, 8);   
  CP_active_subaddrs.dwSetToDoc(dwSubAddr3, 12);   
  CP_active_subaddrs.dwSetToDoc(dwSubAddr4, 16);   
  CP_active_subaddrs.dwSetToDoc(dwSubAddr5, 20);   
  
  // *** Personal Addresses 
  CP_Personal_addrs.dwSetToDoc(m_dwPersonalAddress1, 0);
  CP_Personal_addrs.dwSetToDoc(m_dwPersonalAddress2, 4);

  // *** Automatic validation and generation of PA/ESN/BF/HC fields
  if (bR50Enable)
  { 
    if (PersAddrEdit1.GetModify())
    { 
      // automatically calculate new esn if LabUseOnly PPS or
      // if new personal address is within MtelEngineeringUse and
      // previous personal address is also within valid range.
      if ((theApp.IsLabUseOnly()) || ((IsMtelEngUseOnly(m_dwPersonalAddress1)))) 
        m_dwElectronicSerialNum = CPpsDoc::GenerateEsn(m_dwPersonalAddress1); 
      BYTE byPersonalBaseFrame = (BYTE)(m_dwPersonalAddress1 % 128);
      BYTE byHomeLogicalNum = (BYTE)(m_dwPersonalAddress1 % 12 + 1);
      CP_PagerFrame.bySetToDoc(byPersonalBaseFrame);
      CP_HomeLogicalNum.bySetToDoc(byHomeLogicalNum);
    }
    CP_Electronic_Serial_Number.dwSetToDoc(m_dwElectronicSerialNum);
  } 

  return(TRUE);
}


  

BOOL CAddressInfo::IsMtelEngUseOnly(DWORD dwPersAddr)
{   
   // original PA must be within range, original PA/ESN must match
   // and new PA must be within range in order for pager
   // to be considered valid Mtel Engineering Use pager.
   BOOL bOriginalPaInRange = ((dwOriginalPersAddr1 >= 0x43A0000) && (dwOriginalPersAddr1 <= 0x43A03FF));
   BOOL bOriginalPaEsnMatch = (CPpsDoc::GenerateEsn(dwOriginalPersAddr1) == dwOriginalEsn);
   BOOL bNewPaInRange = ((dwPersAddr >= 0x43A0000) && (dwPersAddr <= 0x43A03FF));
   return(bOriginalPaInRange && bOriginalPaEsnMatch && bNewPaInRange);
}


void CAddressInfo::OnKillFocusPerAddr1Edit()
{
  if (bR50Enable == FALSE)
    return;
  
  if (!(UpdateData(TRUE)))  // get controls data
    return;

  if (PersAddrEdit1.GetModify()) 
  {
    // automatically calculate new esn if LabUseOnly PPS or
    // if new personal address is within MtelEngineeringUse and
    // previous personal address is also within valid range. 
    if ((theApp.IsLabUseOnly()) || ((IsMtelEngUseOnly(m_dwPersonalAddress1))))
    { 
      m_dwElectronicSerialNum = CPpsDoc::GenerateEsn(m_dwPersonalAddress1);
    }
    BYTE byPersonalBaseFrame = (BYTE)(m_dwPersonalAddress1 % 128);
    BYTE byHomeLogicalNum = (BYTE)(m_dwPersonalAddress1 % 12 + 1);
    CP_PagerFrame.bySetToDoc(byPersonalBaseFrame);
    CP_HomeLogicalNum.bySetToDoc(byHomeLogicalNum);
  }

  UpdateData(FALSE);
  return;
}




/////////////////////////////////////////////////////////////////////////////
// CAddressInfo message handlers




////////////////////////////////////////////////////////////////////////
//  OnAcceptDefaultsBtn()
//
//  This function initiates a move of data from select fields of the
//  CPpsDoc.m_byDefaultCodePlugArray to the CPpsDoc.m_byCodePlugArray.
//  The m_byDefaultCodePlugArray is loaded from file 'default.s19' upon
//  application execution.  If the file has not been found, then no 
//  default values are available.  
//
void CAddressInfo::OnAcceptDefaultsBtn()
{
  TRACE("\nCAddressInfo::OnAcceptDefaultsBtn\n");
  CPpsDoc	*pDoc = GetDocument(); 
   
  if (pDoc->SetFieldFromDefault(CP_InfoSvc_Addrs.Offset(), CP_InfoSvc_Addrs.Length()))
    return;        
  if (pDoc->SetFieldFromDefault(CP_Active_SubAddrs.Offset(), CP_Active_SubAddrs.Length()))
    return;
    
  SetFromDoc();   // update this view
  return; 
}


 

////////////////////////////////////////////////////////////////////////
//  OnUndo()
//
//  This function initiates a move of data from select fields of the
//  CPpsDoc.m_byOriginalCodePlugArray to the CPpsDoc.m_byCodePlugArray.
//  The m_byOriginalCodePlugArray is loaded upon 'pager-read' or 
//  'file-open' menu or toolbar actions.
//
void CAddressInfo::OnUndoBtn(void)
{
  TRACE("\nCAddressInfo::OnUndoBtn\n");
  CPpsDoc	*pDoc = GetDocument(); 
  
  if (pDoc->SetFieldFromOriginal(CP_InfoSvc_Addrs.Offset(), CP_InfoSvc_Addrs.Length()))
    return;  
  if (pDoc->SetFieldFromOriginal(CP_Personal_Addrs.Offset(), CP_Personal_Addrs.Length()))
    return;
  if (pDoc->SetFieldFromOriginal(CP_Active_SubAddrs.Offset(), CP_Active_SubAddrs.Length()))
    return;
  if (bR50Enable == TRUE)
  { 
    if (pDoc->SetFieldFromOriginal(CP_Electronic_Serial_Number.Offset(), CP_Electronic_Serial_Number.Length()))
      return;  
  }
  
  SetFromDoc();   // update this view
  return;
}


////////////////////////////////////////////////////////////////////////
//  OnArchived()
//
//  This function initiates a move of data from select fields of the
//  CPpsDoc.m_byArchivedCodePlugArray to the CPpsDoc.m_byCodePlugArray.
//
void CAddressInfo::OnArchived(void)
{
  CPpsDoc	*pDoc = GetDocument(); 
   
  if (pDoc->SetFieldFromArchived(CP_InfoSvc_Addrs.Offset(), CP_InfoSvc_Addrs.Length()))
    return;  
  if (pDoc->SetFieldFromArchived(CP_Active_SubAddrs.Offset(), CP_Active_SubAddrs.Length()))
    return;
    
  SetFromDoc();   // update this view
}




void CAddressInfo::OnSubaddrBtn0()
{
	CInfoSvcSubAddrDlg	SubAddrDlg;
    SubAddrDlg.DoModal(&dwSubAddr0);
}


void CAddressInfo::OnSubaddrBtn1()
{
	CInfoSvcSubAddrDlg	SubAddrDlg;
    SubAddrDlg.DoModal(&dwSubAddr1);
}


void CAddressInfo::OnSubaddrBtn2()
{
	CInfoSvcSubAddrDlg	SubAddrDlg;
    SubAddrDlg.DoModal(&dwSubAddr2);
}


void CAddressInfo::OnSubaddrBtn3()
{
	CInfoSvcSubAddrDlg	SubAddrDlg;
    SubAddrDlg.DoModal(&dwSubAddr3);
}


void CAddressInfo::OnSubaddrBtn4()
{
	CInfoSvcSubAddrDlg	SubAddrDlg;
    SubAddrDlg.DoModal(&dwSubAddr4);
}


void CAddressInfo::OnSubaddrBtn5()
{
	CInfoSvcSubAddrDlg	SubAddrDlg;
    SubAddrDlg.DoModal(&dwSubAddr5);
}



int CAddressInfo::OnPrintHelper(CStringArray &rStringArray, UINT nPageNumber, int nAvailableLines)
{
   char szTemp[100];
   CString szLocal;
    
   // if not enough lines available, wait for next page...
   const int nNumLinesRequired = 10;    // count the number of 'rStringArray.Add' calls
   if (nAvailableLines < nNumLinesRequired)
     return(-1);    
   int nNumLinesBegin = rStringArray.GetSize();
     
   rStringArray.Add("ADDRESSING");
   
   rStringArray.Add("   Information-Service Addresses and Enabled Sub-Addresses");
     ConstructInfoSvcPrintString(szLocal, m_bInfoSvc0Disable, m_dwInfoSvcAddr0, dwSubAddr0);
     rStringArray.Add(szLocal); 
     ConstructInfoSvcPrintString(szLocal, m_bInfoSvc1Disable, m_dwInfoSvcAddr1, dwSubAddr1);
     rStringArray.Add(szLocal); 
     ConstructInfoSvcPrintString(szLocal, m_bInfoSvc2Disable, m_dwInfoSvcAddr2, dwSubAddr2);
     rStringArray.Add(szLocal); 
     ConstructInfoSvcPrintString(szLocal, m_bInfoSvc3Disable, m_dwInfoSvcAddr3, dwSubAddr3);
     rStringArray.Add(szLocal); 
     ConstructInfoSvcPrintString(szLocal, m_bInfoSvc4Disable, m_dwInfoSvcAddr4, dwSubAddr4);
     rStringArray.Add(szLocal); 
     ConstructInfoSvcPrintString(szLocal, m_bInfoSvc5Disable, m_dwInfoSvcAddr5, dwSubAddr5);
     rStringArray.Add(szLocal); 
   
   szLocal = "   Personal Addresses:  ";
   sprintf(szTemp, "%10lu  %10lu", m_dwPersonalAddress1, m_dwPersonalAddress2); 
   szLocal += szTemp;
   rStringArray.Add(szLocal);
   
   // check for Reflex50 enabled
   CP_reflex50_enable.bSetFromDoc(&bR50Enable);
   if (bR50Enable)
   {                                                  
     szLocal = "   Electronic Serial Number:  ";
     sprintf(szTemp, "%10lu", m_dwElectronicSerialNum);
     szLocal += szTemp;
     rStringArray.Add(szLocal);
   }
    
   rStringArray.Add("");
   return(rStringArray.GetSize() - nNumLinesBegin);
}


void CAddressInfo::ConstructInfoSvcPrintString(CString &rStr, BOOL bInfoSvcDisable,
                                               DWORD dwInfoSvcAddr, DWORD dwSubAddr)
{                              
  char 	  szBuff[100];
  int 	  lcv;
  DWORD   dwMask = 1;
  
  if (bInfoSvcDisable) 
  {
    rStr = "\t\tDisabled ";
    return;
  }
  else
    rStr = "\t\t"; 
  
  sprintf(szBuff, "%10lu\t", dwInfoSvcAddr);
  rStr += szBuff;
   
  for (lcv = 0; lcv < 32; ++lcv)
  {
    if (dwSubAddr & (dwMask << lcv))
    {
      sprintf(szBuff, "%02d ", lcv);
      rStr += szBuff;
    }
  }
  return;
}



void CAddressInfo::OnInfoSvcDisableChk()
{
  if (!UpdateData(TRUE)) 
    return;
  UpdateData(FALSE);
}

