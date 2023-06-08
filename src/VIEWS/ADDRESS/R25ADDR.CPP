//*************************************************************
//  History:    Date       Author               Comment
//              1997       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
// r25addr.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\pps.h"
#include "..\..\ppsdoc.h"
#include "..\..\ppsview.h" 
#include "..\..\mainfrm.h"
#include "..\..\cpager\codeplug.h" 
#include "r25addr.h"

extern CPpsApp    theApp;

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CR25Address

IMPLEMENT_DYNCREATE(CR25Address, CPpsFormView)

CR25Address::CR25Address()
	: CPpsFormView(CR25Address::IDD)
{
	//{{AFX_DATA_INIT(CR25Address)
	m_dwPersonalAddress1 = 0;
	m_dwInfoSvcAddr0 = 0;
	m_dwInfoSvcAddr1 = 0;
	m_dwInfoSvcAddr2 = 0;
	m_dwInfoSvcAddr3 = 0;
	m_dwInfoSvcAddr4 = 0;
	m_dwInfoSvcAddr5 = 0;
	m_bInfoSvc0Disable = TRUE;
	m_bInfoSvc1Disable = TRUE;
	m_bInfoSvc2Disable = TRUE;
	m_bInfoSvc3Disable = TRUE;
	m_bInfoSvc4Disable = TRUE;
	m_bInfoSvc5Disable = TRUE;
	m_bSpidPrimChk0 = FALSE;
	m_bSpidPrimChk1 = FALSE;
	m_bSpidPrimChk2 = FALSE;
	m_bSpidPrimChk3 = FALSE;
	m_bSpidPrimChk4 = FALSE;
	m_bSpidPrimChk5 = FALSE;
	m_bSpidPrimChk6 = FALSE;
	m_bSpidPrimChk7 = FALSE;
	m_nSpid0 = 0;
	m_nSpid1 = 0;
	m_nSpid2 = 0;
	m_nSpid3 = 0;
	m_nSpid4 = 0;
	m_nSpid5 = 0;
	m_nSpid6 = 0;
	m_nSpid7 = 0;
	m_bSpidZoneChk0 = FALSE;
	m_bSpidZoneChk1 = FALSE;
	m_bSpidZoneChk2 = FALSE;
	m_bSpidZoneChk3 = FALSE;
	m_bSpidZoneChk4 = FALSE;
	m_bSpidZoneChk5 = FALSE;
	m_bSpidZoneChk6 = FALSE;
	m_bSpidZoneChk7 = FALSE;
	m_nSpidZone0 = 0;
	m_nSpidZone1 = 0;
	m_nSpidZone2 = 0;
	m_nSpidZone3 = 0;
	m_nSpidZone4 = 0;
	m_nSpidZone5 = 0;
	m_nSpidZone6 = 0;
	m_nSpidZone7 = 0;
	m_bSpidDisableChk0 = FALSE;
	m_bSpidDisableChk1 = FALSE;
	m_bSpidDisableChk2 = FALSE;
	m_bSpidDisableChk3 = FALSE;
	m_bSpidDisableChk4 = FALSE;
	m_bSpidDisableChk5 = FALSE;
	m_bSpidDisableChk6 = FALSE;
	m_bSpidDisableChk7 = FALSE;
	m_dwHomeIndex = 0;
	//}}AFX_DATA_INIT 
	
	m_nOrigNumSpids = m_nOrigNumScids = m_nSavedNumSpids = m_nSavedNumScids = 0;
}

CR25Address::~CR25Address()
{
}


static const DWORD dwUnusedPersonalAddr = 0xFFFFFFFF;
static const char szEmptyString[] = "";

void CR25Address::DoDataExchange(CDataExchange* pDX)
{ 
	CPpsFormView::DoDataExchange(pDX);
	
	//{{AFX_DATA_MAP(CR25Address)
	DDX_Check(pDX, IDC_ISVC_DISABLE_CHK0, m_bInfoSvc0Disable);
	DDX_Check(pDX, IDC_ISVC_DISABLE_CHK1, m_bInfoSvc1Disable);
	DDX_Check(pDX, IDC_ISVC_DISABLE_CHK2, m_bInfoSvc2Disable);
	DDX_Check(pDX, IDC_ISVC_DISABLE_CHK3, m_bInfoSvc3Disable);
	DDX_Check(pDX, IDC_ISVC_DISABLE_CHK4, m_bInfoSvc4Disable);
	DDX_Check(pDX, IDC_ISVC_DISABLE_CHK5, m_bInfoSvc5Disable);
	DDX_Check(pDX, IDC_PRIMSPID_CHECK0, m_bSpidPrimChk0);
	DDX_Check(pDX, IDC_PRIMSPID_CHECK1, m_bSpidPrimChk1);
	DDX_Check(pDX, IDC_PRIMSPID_CHECK2, m_bSpidPrimChk2);
	DDX_Check(pDX, IDC_PRIMSPID_CHECK3, m_bSpidPrimChk3);
	DDX_Check(pDX, IDC_PRIMSPID_CHECK4, m_bSpidPrimChk4);
	DDX_Check(pDX, IDC_PRIMSPID_CHECK5, m_bSpidPrimChk5);
	DDX_Check(pDX, IDC_PRIMSPID_CHECK6, m_bSpidPrimChk6);
	DDX_Check(pDX, IDC_PRIMSPID_CHECK7, m_bSpidPrimChk7);
	DDX_Text(pDX, IDC_SPID_EDIT0, m_nSpid0);
	DDV_MinMaxUInt(pDX, m_nSpid0, 0, 16383);
	DDX_Text(pDX, IDC_SPID_EDIT1, m_nSpid1);
	DDV_MinMaxUInt(pDX, m_nSpid1, 0, 16383);
	DDX_Text(pDX, IDC_SPID_EDIT2, m_nSpid2);
	DDV_MinMaxUInt(pDX, m_nSpid2, 0, 16383);
	DDX_Text(pDX, IDC_SPID_EDIT3, m_nSpid3);
	DDV_MinMaxUInt(pDX, m_nSpid3, 0, 16383);
	DDX_Text(pDX, IDC_SPID_EDIT4, m_nSpid4);
	DDV_MinMaxUInt(pDX, m_nSpid4, 0, 16383);
	DDX_Text(pDX, IDC_SPID_EDIT5, m_nSpid5);
	DDV_MinMaxUInt(pDX, m_nSpid5, 0, 16383);
	DDX_Text(pDX, IDC_SPID_EDIT6, m_nSpid6);
	DDV_MinMaxUInt(pDX, m_nSpid6, 0, 16383);
	DDX_Text(pDX, IDC_SPID_EDIT7, m_nSpid7);
	DDV_MinMaxUInt(pDX, m_nSpid7, 0, 16383);
	DDX_Check(pDX, IDC_SPIDZN_CHECK0, m_bSpidZoneChk0);
	DDX_Check(pDX, IDC_SPIDZN_CHECK1, m_bSpidZoneChk1);
	DDX_Check(pDX, IDC_SPIDZN_CHECK2, m_bSpidZoneChk2);
	DDX_Check(pDX, IDC_SPIDZN_CHECK3, m_bSpidZoneChk3);
	DDX_Check(pDX, IDC_SPIDZN_CHECK4, m_bSpidZoneChk4);
	DDX_Check(pDX, IDC_SPIDZN_CHECK5, m_bSpidZoneChk5);
	DDX_Check(pDX, IDC_SPIDZN_CHECK6, m_bSpidZoneChk6);
	DDX_Check(pDX, IDC_SPIDZN_CHECK7, m_bSpidZoneChk7);
	DDX_Text(pDX, IDC_SPIDZN_EDIT0, m_nSpidZone0);
	DDV_MinMaxUInt(pDX, m_nSpidZone0, 0, 16383);
	DDX_Text(pDX, IDC_SPIDZN_EDIT1, m_nSpidZone1);
	DDV_MinMaxUInt(pDX, m_nSpidZone1, 0, 16383);
	DDX_Text(pDX, IDC_SPIDZN_EDIT2, m_nSpidZone2);
	DDV_MinMaxUInt(pDX, m_nSpidZone2, 0, 16383);
	DDX_Text(pDX, IDC_SPIDZN_EDIT3, m_nSpidZone3);
	DDV_MinMaxUInt(pDX, m_nSpidZone3, 0, 16383);
	DDX_Text(pDX, IDC_SPIDZN_EDIT4, m_nSpidZone4);
	DDV_MinMaxUInt(pDX, m_nSpidZone4, 0, 16383);
	DDX_Text(pDX, IDC_SPIDZN_EDIT5, m_nSpidZone5);
	DDV_MinMaxUInt(pDX, m_nSpidZone5, 0, 16383);
	DDX_Text(pDX, IDC_SPIDZN_EDIT6, m_nSpidZone6);
	DDV_MinMaxUInt(pDX, m_nSpidZone6, 0, 16383);
	DDX_Text(pDX, IDC_SPIDZN_EDIT7, m_nSpidZone7);
	DDV_MinMaxUInt(pDX, m_nSpidZone7, 0, 16383);
	DDX_Check(pDX, IDC_SPIDDISABLE_CHK0, m_bSpidDisableChk0);
	DDX_Check(pDX, IDC_SPIDDISABLE_CHK1, m_bSpidDisableChk1);
	DDX_Check(pDX, IDC_SPIDDISABLE_CHK2, m_bSpidDisableChk2);
	DDX_Check(pDX, IDC_SPIDDISABLE_CHK3, m_bSpidDisableChk3);
	DDX_Check(pDX, IDC_SPIDDISABLE_CHK4, m_bSpidDisableChk4);
	DDX_Check(pDX, IDC_SPIDDISABLE_CHK5, m_bSpidDisableChk5);
	DDX_Check(pDX, IDC_SPIDDISABLE_CHK6, m_bSpidDisableChk6);
	DDX_Check(pDX, IDC_SPIDDISABLE_CHK7, m_bSpidDisableChk7);
	DDX_Text(pDX, IDC_HOME_INDEX, m_dwHomeIndex);
	DDV_MinMaxDWord(pDX, m_dwHomeIndex, 0, 4294967295);
	//}}AFX_DATA_MAP
	
	//  SPID Info DDX/DDV Customization
	DoSpidSpecialDDX(m_bSpidDisableChk0, m_bSpidZoneChk0, IDC_SPID_EDIT0, 
	                 IDC_SPIDDISABLE_CHK0, IDC_PRIMSPID_CHECK0,
	                 IDC_SPIDZN_CHECK0, IDC_SPIDZN_EDIT0);
	//
	DoSpidSpecialDDX(m_bSpidDisableChk1, m_bSpidZoneChk1, IDC_SPID_EDIT1, 
	                 IDC_SPIDDISABLE_CHK1, IDC_PRIMSPID_CHECK1,
	                 IDC_SPIDZN_CHECK1, IDC_SPIDZN_EDIT1);
	//
	DoSpidSpecialDDX(m_bSpidDisableChk2, m_bSpidZoneChk2, IDC_SPID_EDIT2, 
	                 IDC_SPIDDISABLE_CHK2, IDC_PRIMSPID_CHECK2,
	                 IDC_SPIDZN_CHECK2, IDC_SPIDZN_EDIT2);
	//
	DoSpidSpecialDDX(m_bSpidDisableChk3, m_bSpidZoneChk3, IDC_SPID_EDIT3, 
	                 IDC_SPIDDISABLE_CHK3, IDC_PRIMSPID_CHECK3,
	                 IDC_SPIDZN_CHECK3, IDC_SPIDZN_EDIT3);
	//
	DoSpidSpecialDDX(m_bSpidDisableChk4, m_bSpidZoneChk4, IDC_SPID_EDIT4, 
	                 IDC_SPIDDISABLE_CHK4, IDC_PRIMSPID_CHECK4,
	                 IDC_SPIDZN_CHECK4, IDC_SPIDZN_EDIT4);
	//
	DoSpidSpecialDDX(m_bSpidDisableChk5, m_bSpidZoneChk5, IDC_SPID_EDIT5, 
	                 IDC_SPIDDISABLE_CHK5, IDC_PRIMSPID_CHECK5,
	                 IDC_SPIDZN_CHECK5, IDC_SPIDZN_EDIT5);
	//
	DoSpidSpecialDDX(m_bSpidDisableChk6, m_bSpidZoneChk6, IDC_SPID_EDIT6, 
	                 IDC_SPIDDISABLE_CHK6, IDC_PRIMSPID_CHECK6,
	                 IDC_SPIDZN_CHECK6, IDC_SPIDZN_EDIT6);
	//
	DoSpidSpecialDDX(m_bSpidDisableChk7, m_bSpidZoneChk7, IDC_SPID_EDIT7, 
	                 IDC_SPIDDISABLE_CHK7, IDC_PRIMSPID_CHECK7,
	                 IDC_SPIDZN_CHECK7, IDC_SPIDZN_EDIT7);
	//
	
	//  *** Personal Address DDX/DDV 
	if (pDX->m_bSaveAndValidate == FALSE) // to the screen
	{
	  if (m_dwPersonalAddress1 == dwUnusedPersonalAddr)	 
	    PersAddrEdit1.SetWindowText(szEmptyString);
	  else
	  {
	    DDX_Text(pDX, IDC_PER_ADDR1_EDIT, m_dwPersonalAddress1);
	    DDV_MinMaxDWord(pDX, m_dwPersonalAddress1, 0, 0x3FFFFFFF); 
	  }
	}
	else if (pDX->m_bSaveAndValidate == TRUE)  // from the screen
	{
	   if (PersAddrEdit1.GetWindowTextLength() > 0)
	   {
	     DDX_Text(pDX, IDC_PER_ADDR1_EDIT, m_dwPersonalAddress1);
	     DDV_MinMaxDWord(pDX, m_dwPersonalAddress1, 0, 0x3FFFFFFF); 
	   }
	   else if (PersAddrEdit1.GetModify() == TRUE)
	     m_dwPersonalAddress1 = dwUnusedPersonalAddr;
    }
	 
    // *** Information Services DDX/DDV 
    DoInfoSvcSpecialDDX(pDX, m_bInfoSvc0Disable, IDC_SVC_ADDR_EDIT0, 
                        m_dwInfoSvcAddr0, InfoSvcEdit0);  
    DoInfoSvcSpecialDDX(pDX, m_bInfoSvc1Disable, IDC_SVC_ADDR_EDIT1, 
                        m_dwInfoSvcAddr1, InfoSvcEdit1);  
    DoInfoSvcSpecialDDX(pDX, m_bInfoSvc2Disable, IDC_SVC_ADDR_EDIT2, 
                        m_dwInfoSvcAddr2, InfoSvcEdit2);  
    DoInfoSvcSpecialDDX(pDX, m_bInfoSvc3Disable, IDC_SVC_ADDR_EDIT3, 
                        m_dwInfoSvcAddr3, InfoSvcEdit3);  
    DoInfoSvcSpecialDDX(pDX, m_bInfoSvc4Disable, IDC_SVC_ADDR_EDIT4, 
                        m_dwInfoSvcAddr4, InfoSvcEdit4);  
    DoInfoSvcSpecialDDX(pDX, m_bInfoSvc5Disable, IDC_SVC_ADDR_EDIT5, 
                         m_dwInfoSvcAddr5, InfoSvcEdit5);  

  return;	
}



////////////////////////////////////////////////////////////////////////////////
//  DoSpidSpecialDDX
//
void CR25Address::DoSpidSpecialDDX(BOOL bDisable, BOOL bZoned, UINT nSpidEditIDC, 
	                               UINT nDisableCheckIDC, UINT nPrimCheckIDC,
	                               UINT nZoneCheckIDC, UINT nZoneEditIDC)
{ 
  CEdit *pEdit = NULL;
  CButton *pButton = NULL;
  
  // if SPID entry is disabled, gray all resources except disable check button
  pEdit = (CEdit *)GetDlgItem(nSpidEditIDC);  ASSERT(pEdit != NULL);
  pEdit->EnableWindow(!bDisable);
  pButton = (CButton *)GetDlgItem(nPrimCheckIDC); ASSERT(pButton != NULL);
  pButton->EnableWindow(!bDisable);
  pButton = (CButton *)GetDlgItem(nZoneCheckIDC); ASSERT(pButton != NULL);
  pButton->EnableWindow(!bDisable);
  // also, if Zone check is disabled, gray the zone edit field
  pEdit = (CEdit *)GetDlgItem(nZoneEditIDC);  ASSERT(pEdit != NULL);
  pEdit->EnableWindow(bZoned && !bDisable);  
  return;
}



////////////////////////////////////////////////////////////////////////////////
//  DoInfoSvcSpecialDDX
//
void CR25Address::DoInfoSvcSpecialDDX(CDataExchange* pDX, BOOL bDisable, 
                                      UINT nInfoSvcEditIDC, DWORD &rdwInfoSvcAddr,
                                      CDecimalEdit &rInfoSvcEdit)
{ 
  // If the InfoService field is disabled, don't do xfer or
  // validation. 
  if (bDisable == FALSE)
  {
    DDX_Text(pDX, nInfoSvcEditIDC, rdwInfoSvcAddr);
    DDV_MinMaxDWord(pDX, rdwInfoSvcAddr, 0x4000, 0xFFFFFF);  // Reflex25	   
  } 
  else  // entry disabled
    rInfoSvcEdit.SetWindowText(szEmptyString);
  return;
}




BEGIN_MESSAGE_MAP(CR25Address, CPpsFormView)
	//{{AFX_MSG_MAP(CR25Address)
	ON_BN_CLICKED(IDC_ADDR_DEFAULTS_BTN, OnAcceptDefaultsBtn)
	ON_BN_CLICKED(IDC_ADDR_UNDO_BTN, OnUndoBtn)
	ON_BN_CLICKED(IDC_SPIDDISABLE_CHK0, OnSpidDisableChk0)
	ON_BN_CLICKED(IDC_SPIDDISABLE_CHK1, OnSpidDisableChk1)
	ON_BN_CLICKED(IDC_SPIDDISABLE_CHK2, OnSpidDisableChk2)
	ON_BN_CLICKED(IDC_SPIDDISABLE_CHK3, OnSpidDisableChk3)
	ON_BN_CLICKED(IDC_SPIDDISABLE_CHK4, OnSpidDisableChk4)
	ON_BN_CLICKED(IDC_SPIDDISABLE_CHK5, OnSpidDisableChk5)
	ON_BN_CLICKED(IDC_SPIDDISABLE_CHK6, OnSpidDisableChk6)
	ON_BN_CLICKED(IDC_SPIDDISABLE_CHK7, OnSpidDisableChk7)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_ISVC_DISABLE_CHK0, OnInfoSvcDisableChk)
	ON_BN_CLICKED(IDC_ISVC_DISABLE_CHK1, OnInfoSvcDisableChk)
	ON_BN_CLICKED(IDC_ISVC_DISABLE_CHK2, OnInfoSvcDisableChk)
	ON_BN_CLICKED(IDC_ISVC_DISABLE_CHK3, OnInfoSvcDisableChk)
	ON_BN_CLICKED(IDC_ISVC_DISABLE_CHK4, OnInfoSvcDisableChk)
	ON_BN_CLICKED(IDC_ISVC_DISABLE_CHK5, OnInfoSvcDisableChk)
	ON_BN_CLICKED(IDC_SPIDZN_CHECK0, OnSpidZoneChk)
	ON_BN_CLICKED(IDC_SPIDZN_CHECK1, OnSpidZoneChk)
	ON_BN_CLICKED(IDC_SPIDZN_CHECK2, OnSpidZoneChk)
	ON_BN_CLICKED(IDC_SPIDZN_CHECK3, OnSpidZoneChk)
	ON_BN_CLICKED(IDC_SPIDZN_CHECK4, OnSpidZoneChk)
	ON_BN_CLICKED(IDC_SPIDZN_CHECK5, OnSpidZoneChk)
	ON_BN_CLICKED(IDC_SPIDZN_CHECK6, OnSpidZoneChk)
	ON_BN_CLICKED(IDC_SPIDZN_CHECK7, OnSpidZoneChk)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CR25Address diagnostics

#ifdef _DEBUG
void CR25Address::AssertValid() const
{
	CPpsFormView::AssertValid();
}



void CR25Address::Dump(CDumpContext& dc) const
{
	CPpsFormView::Dump(dc);
}

#endif //_DEBUG





////////////////////////////////////////////////////////////////////////////
//	OnInitialUpdate()
//  
//  Custom resource initialization...
//
void CR25Address::OnInitialUpdate(void)
{
  TRACE("CR25Address::OnInitialUpdate : thisViewID=%X\n", CR25Address::IDD);

  // subclass the edit controls in order to force numeric only input.
  PersAddrEdit1.SubclassDlgItem(IDC_PER_ADDR1_EDIT, this);
  PersAddrEdit1.LimitText(10);
    
  HomeIndexEdit.SubclassDlgItem(IDC_HOME_INDEX, this);
  HomeIndexEdit.LimitText(10);
  
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
  
  SpidEdit0.SubclassDlgItem(IDC_SPID_EDIT0, this);
  SpidEdit0.LimitText(5); 

  SpidEdit1.SubclassDlgItem(IDC_SPID_EDIT1, this);
  SpidEdit1.LimitText(5); 

  SpidEdit2.SubclassDlgItem(IDC_SPID_EDIT2, this);
  SpidEdit2.LimitText(5); 

  SpidEdit3.SubclassDlgItem(IDC_SPID_EDIT3, this);
  SpidEdit3.LimitText(5); 

  SpidEdit4.SubclassDlgItem(IDC_SPID_EDIT4, this);
  SpidEdit4.LimitText(5); 

  SpidEdit5.SubclassDlgItem(IDC_SPID_EDIT5, this);
  SpidEdit5.LimitText(5); 

  SpidEdit6.SubclassDlgItem(IDC_SPID_EDIT6, this);
  SpidEdit6.LimitText(5); 

  SpidEdit7.SubclassDlgItem(IDC_SPID_EDIT7, this);
  SpidEdit7.LimitText(5); 

  SpidZnEdit0.SubclassDlgItem(IDC_SPIDZN_EDIT0, this);
  SpidZnEdit0.LimitText(5); 

  SpidZnEdit1.SubclassDlgItem(IDC_SPIDZN_EDIT1, this);
  SpidZnEdit1.LimitText(5); 

  SpidZnEdit2.SubclassDlgItem(IDC_SPIDZN_EDIT2, this);
  SpidZnEdit2.LimitText(5); 

  SpidZnEdit3.SubclassDlgItem(IDC_SPIDZN_EDIT3, this);
  SpidZnEdit3.LimitText(5); 

  SpidZnEdit4.SubclassDlgItem(IDC_SPIDZN_EDIT4, this);
  SpidZnEdit4.LimitText(5); 

  SpidZnEdit5.SubclassDlgItem(IDC_SPIDZN_EDIT5, this);
  SpidZnEdit5.LimitText(5); 

  SpidZnEdit6.SubclassDlgItem(IDC_SPIDZN_EDIT6, this);
  SpidZnEdit6.LimitText(5); 

  SpidZnEdit7.SubclassDlgItem(IDC_SPIDZN_EDIT7, this);
  SpidZnEdit7.LimitText(5); 

  CPpsFormView::OnInitialUpdate();  
  return;
}




/////////////////////////////////////////////////////////////////////////////
//	OnUpdate()
//
void CR25Address::OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint)
{
  TRACE("\nCR25Address::OnUpdate : nCR25Address::IDD=%X  LHintHI=%X  lHintLO=%X\n", 
  		CR25Address::IDD,  HIWORD(lHint), LOWORD(lHint));

  if (HIWORD(lHint))        		// user switched views, now active
    ResizeParentToFit(FALSE);
  
  SetFromDoc();
  CPpsFormView::OnUpdate(pSender, lHint, pHint);
}	



////////////////////////////////////////////////////////////////////////////
//  SetFromDoc()
//
void CR25Address::SetFromDoc(void)
{
  TRACE("\nCR25Address::SetFromDoc\n");
  int nSpidPackIndex = 0; 
  m_nOrigNumSpids = m_nOrigNumScids = 0;
  
  // *** Up to 8 Service Provider ID's 
  UnPackSpid(&m_bSpidZoneChk0, &m_bSpidPrimChk0, &m_bSpidDisableChk0, 
             &m_nSpid0, &m_nSpidZone0, &nSpidPackIndex); 
  //                       
  UnPackSpid(&m_bSpidZoneChk1, &m_bSpidPrimChk1, &m_bSpidDisableChk1, 
             &m_nSpid1, &m_nSpidZone1, &nSpidPackIndex);
  //           
  UnPackSpid(&m_bSpidZoneChk2, &m_bSpidPrimChk2, &m_bSpidDisableChk2, 
             &m_nSpid2, &m_nSpidZone2, &nSpidPackIndex);
  //           
  UnPackSpid(&m_bSpidZoneChk3, &m_bSpidPrimChk3, &m_bSpidDisableChk3, 
             &m_nSpid3, &m_nSpidZone3, &nSpidPackIndex); 
  //           
  if (nSpidPackIndex < (CP_Service_Provider_Ids.Length()))           
    UnPackSpid(&m_bSpidZoneChk4, &m_bSpidPrimChk4, &m_bSpidDisableChk4, 
               &m_nSpid4, &m_nSpidZone4, &nSpidPackIndex);
  else
    m_bSpidDisableChk4 = TRUE; 
  //  
  if (nSpidPackIndex < (CP_Service_Provider_Ids.Length()))           
    UnPackSpid(&m_bSpidZoneChk5, &m_bSpidPrimChk5, &m_bSpidDisableChk5, 
               &m_nSpid5, &m_nSpidZone5, &nSpidPackIndex);
  else
    m_bSpidDisableChk5 = TRUE;
  //  
  if (nSpidPackIndex < (CP_Service_Provider_Ids.Length()))           
    UnPackSpid(&m_bSpidZoneChk6, &m_bSpidPrimChk6, &m_bSpidDisableChk6, 
               &m_nSpid6, &m_nSpidZone6, &nSpidPackIndex);
  else
    m_bSpidDisableChk6 = TRUE;
  //  
  if (nSpidPackIndex < (CP_Service_Provider_Ids.Length()))           
    UnPackSpid(&m_bSpidZoneChk7, &m_bSpidPrimChk7, &m_bSpidDisableChk7, 
               &m_nSpid7, &m_nSpidZone7, &nSpidPackIndex);    
  else
    m_bSpidDisableChk7 = TRUE;
  //
  
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
  
  // *** Personal Addresses 
  CP_Personal_addrs.dwSetFromDoc(&m_dwPersonalAddress1);
  
  // *** Home Index
  CP_Home_Index.dwSetFromDoc(&m_dwHomeIndex);
   
  UpdateData(FALSE);	// set controls data
}



/////////////////////////////////////////////////////////////////////////////
//  UnPackSpid
//
//  Local helper function for SetFromDoc().
//
void CR25Address::UnPackSpid(BOOL *pbZone, BOOL *pbPrim, BOOL *pbDisable, 
                             UINT *pnSpid, UINT *pnZone, int *pnIndex)
{
  CP_SpidZoneIndicator.bSetFromDoc(pbZone, (*pnIndex));
  CP_SpidAncPrimIndicator.bSetFromDoc(pbPrim, (*pnIndex));
  CP_SpidNumber.nSetFromDoc(pnSpid, (*pnIndex));
  if (*pbZone)  // Is this SPID also a SCID?  
  { 
     *pnIndex = (*pnIndex) + 2; 
     if ((*pnIndex) < (CP_Service_Provider_Ids.Length())) 
     {
       CP_SpidZoneNumber.nSetFromDoc(pnZone, (*pnIndex));
       m_nOrigNumScids += 1;
     } 
  }
  else
    m_nOrigNumSpids += 1;
  *pbDisable = (((*pbZone) == FALSE) && ((*pbPrim) == FALSE) && ((*pnSpid) == 0)); 
  *pnIndex = (*pnIndex) + 2;
  return;
}
  

  

////////////////////////////////////////////////////////////////////////////
//	SetToDoc()
//
BOOL CR25Address::SetToDoc(void)
{
  TRACE("\nCR25Address::SetToDoc\n");
  CPpsDoc		*pDoc = GetDocument();
  
  if (!(UpdateData(TRUE)))  // get controls data
    return FALSE;

  int nSpidPackIndex = 0;  
  int nMaxSpidIndex = CP_Service_Provider_Ids.Length();
  m_nSavedNumSpids = m_nSavedNumScids = 0;
  
  // *** Service Provider ID's 
  // *** Ordered by primary indicator, enabled ancillary, then disabled 
  // *** Allow no more than nMaxSpidIndex to be written
  if ((m_bSpidPrimChk0) && (!m_bSpidDisableChk0) && (nSpidPackIndex < nMaxSpidIndex))
    PackSpid(m_bSpidZoneChk0, m_bSpidPrimChk0, m_nSpid0, m_nSpidZone0, &nSpidPackIndex);
  if ((m_bSpidPrimChk1) && (!m_bSpidDisableChk1) && (nSpidPackIndex < nMaxSpidIndex))
    PackSpid(m_bSpidZoneChk1, m_bSpidPrimChk1, m_nSpid1, m_nSpidZone1, &nSpidPackIndex);
  if ((m_bSpidPrimChk2) && (!m_bSpidDisableChk2) && (nSpidPackIndex < nMaxSpidIndex))
    PackSpid(m_bSpidZoneChk2, m_bSpidPrimChk2, m_nSpid2, m_nSpidZone2, &nSpidPackIndex);
  if ((m_bSpidPrimChk3) && (!m_bSpidDisableChk3) && (nSpidPackIndex < nMaxSpidIndex))
    PackSpid(m_bSpidZoneChk3, m_bSpidPrimChk3, m_nSpid3, m_nSpidZone3, &nSpidPackIndex);
  if ((m_bSpidPrimChk4) && (!m_bSpidDisableChk4) && (nSpidPackIndex < nMaxSpidIndex))
    PackSpid(m_bSpidZoneChk4, m_bSpidPrimChk4, m_nSpid4, m_nSpidZone4, &nSpidPackIndex);
  if ((m_bSpidPrimChk5) && (!m_bSpidDisableChk5) && (nSpidPackIndex < nMaxSpidIndex))
    PackSpid(m_bSpidZoneChk5, m_bSpidPrimChk5, m_nSpid5, m_nSpidZone5, &nSpidPackIndex);
  if ((m_bSpidPrimChk6) && (!m_bSpidDisableChk6) && (nSpidPackIndex < nMaxSpidIndex))
    PackSpid(m_bSpidZoneChk6, m_bSpidPrimChk6, m_nSpid6, m_nSpidZone6, &nSpidPackIndex);
  if ((m_bSpidPrimChk7) && (!m_bSpidDisableChk7) && (nSpidPackIndex < nMaxSpidIndex))
    PackSpid(m_bSpidZoneChk7, m_bSpidPrimChk7, m_nSpid7, m_nSpidZone7, &nSpidPackIndex);
  //
  if ((!m_bSpidPrimChk0) && (!m_bSpidDisableChk0) && (nSpidPackIndex < nMaxSpidIndex))
    PackSpid(m_bSpidZoneChk0, m_bSpidPrimChk0, m_nSpid0, m_nSpidZone0, &nSpidPackIndex);
  if ((!m_bSpidPrimChk1) && (!m_bSpidDisableChk1) && (nSpidPackIndex < nMaxSpidIndex))
    PackSpid(m_bSpidZoneChk1, m_bSpidPrimChk1, m_nSpid1, m_nSpidZone1, &nSpidPackIndex);
  if ((!m_bSpidPrimChk2) && (!m_bSpidDisableChk2) && (nSpidPackIndex < nMaxSpidIndex))
    PackSpid(m_bSpidZoneChk2, m_bSpidPrimChk2, m_nSpid2, m_nSpidZone2, &nSpidPackIndex);
  if ((!m_bSpidPrimChk3) && (!m_bSpidDisableChk3) && (nSpidPackIndex < nMaxSpidIndex))
    PackSpid(m_bSpidZoneChk3, m_bSpidPrimChk3, m_nSpid3, m_nSpidZone3, &nSpidPackIndex);
  if ((!m_bSpidPrimChk4) && (!m_bSpidDisableChk4) && (nSpidPackIndex < nMaxSpidIndex))
    PackSpid(m_bSpidZoneChk4, m_bSpidPrimChk4, m_nSpid4, m_nSpidZone4, &nSpidPackIndex);
  if ((!m_bSpidPrimChk5) && (!m_bSpidDisableChk5) && (nSpidPackIndex < nMaxSpidIndex))
    PackSpid(m_bSpidZoneChk5, m_bSpidPrimChk5, m_nSpid5, m_nSpidZone5, &nSpidPackIndex);
  if ((!m_bSpidPrimChk6) && (!m_bSpidDisableChk6) && (nSpidPackIndex < nMaxSpidIndex))
    PackSpid(m_bSpidZoneChk6, m_bSpidPrimChk6, m_nSpid6, m_nSpidZone6, &nSpidPackIndex);
  if ((!m_bSpidPrimChk7) && (!m_bSpidDisableChk7) && (nSpidPackIndex < nMaxSpidIndex))
    PackSpid(m_bSpidZoneChk7, m_bSpidPrimChk7, m_nSpid7, m_nSpidZone7, &nSpidPackIndex);
  //
  if ((m_bSpidDisableChk0) && (nSpidPackIndex < nMaxSpidIndex))
    PackSpid(m_bSpidZoneChk0, m_bSpidPrimChk0, m_nSpid0, m_nSpidZone0, &nSpidPackIndex);
  if ((m_bSpidDisableChk1) && (nSpidPackIndex < nMaxSpidIndex))
    PackSpid(m_bSpidZoneChk1, m_bSpidPrimChk1, m_nSpid1, m_nSpidZone1, &nSpidPackIndex);
  if ((m_bSpidDisableChk2) && (nSpidPackIndex < nMaxSpidIndex))
    PackSpid(m_bSpidZoneChk2, m_bSpidPrimChk2, m_nSpid2, m_nSpidZone2, &nSpidPackIndex);
  if ((m_bSpidDisableChk3) && (nSpidPackIndex < nMaxSpidIndex))
    PackSpid(m_bSpidZoneChk3, m_bSpidPrimChk3, m_nSpid3, m_nSpidZone3, &nSpidPackIndex);
  if ((m_bSpidDisableChk4) && (nSpidPackIndex < nMaxSpidIndex))
    PackSpid(m_bSpidZoneChk4, m_bSpidPrimChk4, m_nSpid4, m_nSpidZone4, &nSpidPackIndex);
  if ((m_bSpidDisableChk5) && (nSpidPackIndex < nMaxSpidIndex))
    PackSpid(m_bSpidZoneChk5, m_bSpidPrimChk5, m_nSpid5, m_nSpidZone5, &nSpidPackIndex);
  if ((m_bSpidDisableChk6) && (nSpidPackIndex < nMaxSpidIndex))
    PackSpid(m_bSpidZoneChk6, m_bSpidPrimChk6, m_nSpid6, m_nSpidZone6, &nSpidPackIndex);
  if ((m_bSpidDisableChk7) && (nSpidPackIndex < nMaxSpidIndex))
    PackSpid(m_bSpidZoneChk7, m_bSpidPrimChk7, m_nSpid7, m_nSpidZone7, &nSpidPackIndex);
  
  // *** Let user know of possible truncation of SPID configuration data
  if ((nSpidPackIndex > nMaxSpidIndex) && 
      ((m_nSavedNumSpids != m_nOrigNumSpids) || (m_nSavedNumScids != m_nOrigNumScids)))
    PackSpidOverFlowMsg();
  
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

  // *** Personal Addresses 
  CP_Personal_addrs.dwSetToDoc(m_dwPersonalAddress1);

  // *** Home Index
  CP_Home_Index.dwSetToDoc(m_dwHomeIndex);

  return(TRUE);
}


/////////////////////////////////////////////////////////////////////////////
//  PackSpid
//
//  Local helper function for SetToDoc().
// 
void CR25Address::PackSpid(BOOL bZone, BOOL bPrim, UINT nSpid, UINT nZone, int *pnIndex)
{
  CP_SpidZoneIndicator.bSetToDoc(bZone, (*pnIndex));
  CP_SpidAncPrimIndicator.bSetToDoc(bPrim, (*pnIndex));
  CP_SpidNumber.nSetToDoc(nSpid, (*pnIndex));
  if (bZone)  // Is this SPID also a SCID?  
  { 
     *pnIndex = (*pnIndex) + 2;  
     if ((*pnIndex) < (CP_Service_Provider_Ids.Length())) 
     {
       CP_SpidZoneNumber.nSetToDoc(nZone, (*pnIndex));
       m_nSavedNumScids += 1;
     }                                                         
     else  // unable to save entry as SCID, make it a SPID  
       CP_SpidZoneIndicator.bSetToDoc(FALSE, ((*pnIndex) - 2));  
  }  
  else
    m_nSavedNumSpids += 1;
  *pnIndex = (*pnIndex) + 2;
  return;
}
  


static char *szPackWarning = "The list of enabled Service Providers has\n"
                             "been truncated to fit the codeplug field \n"
                             "length of %d bytes.\n";
//
void CR25Address::PackSpidOverFlowMsg(void)
{
  char szMsg[300];
  sprintf(szMsg, szPackWarning, (CP_Service_Provider_Ids.Length()));
  AfxMessageBox((LPCSTR)szMsg);
  return;
}



/////////////////////////////////////////////////////////////////////////////
// CR25Address message handlers

////////////////////////////////////////////////////////////////////////
//  OnAcceptDefaultsBtn()
//
//  This function initiates a move of data from select fields of the
//  CPpsDoc.m_byDefaultCodePlugArray to the CPpsDoc.m_byCodePlugArray.
//  The m_byDefaultCodePlugArray is loaded from file 'default.s19' upon
//  application execution.  If the file has not been found, then no 
//  default values are available.  
//
void CR25Address::OnAcceptDefaultsBtn()
{
  TRACE("\nCR25Address::OnAcceptDefaultsBtn\n");
  CPpsDoc	*pDoc = GetDocument(); 
   
  if (pDoc->SetFieldFromDefault(CP_InfoSvc_Addrs.Offset(), CP_InfoSvc_Addrs.Length()))
    return;        
  if (pDoc->SetFieldFromDefault(CP_Home_Index.Offset(), CP_Home_Index.Length()))
    return;
  if (pDoc->SetFieldFromDefault(CP_Service_Provider_Ids.Offset(), CP_Service_Provider_Ids.Length()))
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
void CR25Address::OnUndoBtn(void)
{
  TRACE("\nCR25Address::OnUndoBtn\n");
  CPpsDoc	*pDoc = GetDocument(); 
  
  if (pDoc->SetFieldFromOriginal(CP_InfoSvc_Addrs.Offset(), CP_InfoSvc_Addrs.Length()))
    return;  
  if (pDoc->SetFieldFromOriginal(CP_Personal_Addrs.Offset(), CP_Personal_Addrs.Length()))
    return;
  if (pDoc->SetFieldFromOriginal(CP_Home_Index.Offset(), CP_Home_Index.Length()))
    return;
  if (pDoc->SetFieldFromOriginal(CP_Service_Provider_Ids.Offset(), CP_Service_Provider_Ids.Length()))
    return;

  SetFromDoc();   // update this view
  return;
}


////////////////////////////////////////////////////////////////////////
//  OnArchived()
//
//  This function initiates a move of data from select fields of the
//  CPpsDoc.m_byArchivedCodePlugArray to the CPpsDoc.m_byCodePlugArray.
//
void CR25Address::OnArchived(void)
{
  CPpsDoc	*pDoc = GetDocument(); 
   
  if (pDoc->SetFieldFromArchived(CP_InfoSvc_Addrs.Offset(), CP_InfoSvc_Addrs.Length()))
    return;  
  if (pDoc->SetFieldFromArchived(CP_Home_Index.Offset(), CP_Home_Index.Length()))
    return;
  if (pDoc->SetFieldFromArchived(CP_Service_Provider_Ids.Offset(), CP_Service_Provider_Ids.Length()))
    return;
    
  SetFromDoc();   // update this view
}



///////////////////////////////////////////////////////////////////////////////
//  OnPrintHelper()
//
int CR25Address::OnPrintHelper(CStringArray &rStringArray, UINT nPageNumber, int nAvailableLines)
{
   char szTemp[100];
   CString szLocal;
    
   // if not enough lines available, wait for next page...
   const int nNumLinesRequired = 20;    // count the number of 'rStringArray.Add' calls
   if (nAvailableLines < nNumLinesRequired)
     return(-1);    
   int nNumLinesBegin = rStringArray.GetSize();
     
   rStringArray.Add("ADDRESSING");
   
   rStringArray.Add("   Service Provider Identifiers");
     ConstructSpidPrintString(szLocal, m_bSpidDisableChk0, m_nSpid0, m_bSpidPrimChk0, 
                                       m_bSpidZoneChk0, m_nSpidZone0);
     rStringArray.Add(szLocal); 
     ConstructSpidPrintString(szLocal, m_bSpidDisableChk1, m_nSpid1, m_bSpidPrimChk1, 
                                       m_bSpidZoneChk1, m_nSpidZone1);
     rStringArray.Add(szLocal); 
     ConstructSpidPrintString(szLocal, m_bSpidDisableChk2, m_nSpid2, m_bSpidPrimChk2, 
                                       m_bSpidZoneChk2, m_nSpidZone2);
     rStringArray.Add(szLocal); 
     ConstructSpidPrintString(szLocal, m_bSpidDisableChk3, m_nSpid3, m_bSpidPrimChk3, 
                                       m_bSpidZoneChk3, m_nSpidZone3);
     rStringArray.Add(szLocal); 
     ConstructSpidPrintString(szLocal, m_bSpidDisableChk4, m_nSpid4, m_bSpidPrimChk4, 
                                       m_bSpidZoneChk4, m_nSpidZone4);
     rStringArray.Add(szLocal); 
     ConstructSpidPrintString(szLocal, m_bSpidDisableChk5, m_nSpid5, m_bSpidPrimChk5, 
                                       m_bSpidZoneChk5, m_nSpidZone5);
     rStringArray.Add(szLocal); 
     ConstructSpidPrintString(szLocal, m_bSpidDisableChk6, m_nSpid6, m_bSpidPrimChk6, 
                                       m_bSpidZoneChk6, m_nSpidZone6);
     rStringArray.Add(szLocal); 
     ConstructSpidPrintString(szLocal, m_bSpidDisableChk7, m_nSpid7, m_bSpidPrimChk7, 
                                       m_bSpidZoneChk7, m_nSpidZone7);
     rStringArray.Add(szLocal); 
   
   rStringArray.Add("   Information-Service Addresses");
     ConstructInfoSvcPrintString(szLocal, m_bInfoSvc0Disable, m_dwInfoSvcAddr0);
     rStringArray.Add(szLocal); 
     ConstructInfoSvcPrintString(szLocal, m_bInfoSvc1Disable, m_dwInfoSvcAddr1);
     rStringArray.Add(szLocal); 
     ConstructInfoSvcPrintString(szLocal, m_bInfoSvc2Disable, m_dwInfoSvcAddr2);
     rStringArray.Add(szLocal); 
     ConstructInfoSvcPrintString(szLocal, m_bInfoSvc3Disable, m_dwInfoSvcAddr3);
     rStringArray.Add(szLocal); 
     ConstructInfoSvcPrintString(szLocal, m_bInfoSvc4Disable, m_dwInfoSvcAddr4);
     rStringArray.Add(szLocal); 
     ConstructInfoSvcPrintString(szLocal, m_bInfoSvc5Disable, m_dwInfoSvcAddr5);
     rStringArray.Add(szLocal); 
   
   sprintf(szTemp, "   Personal Address:  %10lu", m_dwPersonalAddress1); 
   szLocal += szTemp;
   rStringArray.Add(szLocal);
   
   sprintf(szTemp, "   Home Index:  %10lu", m_dwHomeIndex);
   rStringArray.Add(szTemp);

   rStringArray.Add("");
   return(rStringArray.GetSize() - nNumLinesBegin);
}


void CR25Address::ConstructSpidPrintString(CString &rStr, BOOL bDisable, UINT nSpid,
                                           BOOL bPrim, BOOL bZoned, UINT nZone)
{
  char szBuff[100];
  
  if (bDisable)
  {
    rStr = "\t\tDisabled ";
    return;
  }
  else
    rStr = "\t\t";
  
  sprintf(szBuff, "SPID=%d\t", nSpid);
  rStr += szBuff;
  if (bPrim)
    rStr += "primary\t";
  if (bZoned)
  {
    sprintf(szBuff, "zoned\tZoneID=%d", nZone);
    rStr += szBuff;
  }
  return;
}


void CR25Address::ConstructInfoSvcPrintString(CString &rStr, BOOL bInfoSvcDisable,
                                               DWORD dwInfoSvcAddr)
{                              
  char 	  szBuff[100];
  
  if (bInfoSvcDisable) 
  {
    rStr = "\t\tDisabled ";
    return;
  }
  else
    rStr = "\t\t"; 
  
  sprintf(szBuff, "%10lu\t", dwInfoSvcAddr);
  rStr += szBuff;   
  return;
}



/////////////////////////////////////////////////////////////////////////
//  OnInfoSvcDisableChk()
//
void CR25Address::OnInfoSvcDisableChk()
{
  if (!UpdateData(TRUE)) 
    return;
  UpdateData(FALSE);
}



/////////////////////////////////////////////////////////////////////////
//  OnSpidDisable()
// 
//  Local helper function for the OnSpidDisableChkX event handlers...
//
void CR25Address::OnSpidDisable(BOOL &rbDisable, BOOL &rbZoned, BOOL &rbPrim,
                                UINT &rnSpid, UINT &rnZone)
{
  if (!UpdateData(TRUE))
    return;
  if (rbDisable == TRUE)
  {
    rbZoned = FALSE;
    rbPrim = FALSE;
    rnSpid = 0;
    rnZone = 0;
  }
  UpdateData(FALSE);
  return;
}

void CR25Address::OnSpidDisableChk0()
{ 
  OnSpidDisable(m_bSpidDisableChk0, m_bSpidZoneChk0, m_bSpidPrimChk0, m_nSpid0, m_nSpidZone0);    
  return;
}

void CR25Address::OnSpidDisableChk1()
{
  OnSpidDisable(m_bSpidDisableChk1, m_bSpidZoneChk1, m_bSpidPrimChk1, m_nSpid1, m_nSpidZone1);    
  return;
}

void CR25Address::OnSpidDisableChk2()
{
  OnSpidDisable(m_bSpidDisableChk2, m_bSpidZoneChk2, m_bSpidPrimChk2, m_nSpid2, m_nSpidZone2);    
  return;
}

void CR25Address::OnSpidDisableChk3()
{
  OnSpidDisable(m_bSpidDisableChk3, m_bSpidZoneChk3, m_bSpidPrimChk3, m_nSpid3, m_nSpidZone3);    
  return;
}

void CR25Address::OnSpidDisableChk4()
{
  OnSpidDisable(m_bSpidDisableChk4, m_bSpidZoneChk4, m_bSpidPrimChk4, m_nSpid4, m_nSpidZone4);    
  return;
}

void CR25Address::OnSpidDisableChk5()
{
  OnSpidDisable(m_bSpidDisableChk5, m_bSpidZoneChk5, m_bSpidPrimChk5, m_nSpid5, m_nSpidZone5);    
  return;
}

void CR25Address::OnSpidDisableChk6()
{
  OnSpidDisable(m_bSpidDisableChk6, m_bSpidZoneChk6, m_bSpidPrimChk6, m_nSpid6, m_nSpidZone6);    
  return;
}

void CR25Address::OnSpidDisableChk7()
{
  OnSpidDisable(m_bSpidDisableChk7, m_bSpidZoneChk7, m_bSpidPrimChk7, m_nSpid7, m_nSpidZone7);    
  return;
}


////////////////////////////////////////////////////////////////////////////
//  OnSpidZoneChk()
//
void CR25Address::OnSpidZoneChk()
{
  if (!UpdateData(TRUE)) 
    return;
  UpdateData(FALSE);
}

