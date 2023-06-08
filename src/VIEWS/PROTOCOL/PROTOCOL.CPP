//*************************************************************
//  History:    Date       Author               Comment
//              1996       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
// protocol.cpp : Reflex25 channel configuration implementation file
//

#include "stdafx.h"
#include "..\..\pps.h"
#include "..\..\ppsdoc.h"
#include "..\..\ppsview.h" 
#include "..\..\cpager\codeplug.h" 
#include "protocol.h"
#include "math.h"		// for the 'ceil' function

extern CPpsApp    theApp;

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReflex25Protocol

IMPLEMENT_DYNCREATE(CReflex25Protocol, CPpsFormView)

CReflex25Protocol::CReflex25Protocol()
	: CPpsFormView(CReflex25Protocol::IDD)
{
	m_nF0_ChanNum = 0;
	m_nF1_ChanNum = 0;
	m_nF2_ChanNum = 0;
	m_nF3_ChanNum = 0;
	m_nF4_ChanNum = 0;
	m_nF5_ChanNum = 0;
	m_nF6_ChanNum = 0;
	m_nF7_ChanNum = 0;

	// *** these fields are not longer modified from this view 10/25/96
	//m_bSvcProv0Disable = FALSE;
	//m_bSvcProv0Ancillary = FALSE;
	//m_bSvcProv1Ancillary = FALSE;
	//m_bSvcProv2Ancillary = FALSE;
	//m_bSvcProv3Ancillary = FALSE;

	//{{AFX_DATA_INIT(CReflex25Protocol)
	m_bF0_Disable = FALSE;
	m_bF1_Disable = FALSE;
	m_bF2_Disable = FALSE;
	m_bF3_Disable = FALSE;
	m_bF4_Disable = FALSE;
	m_bF5_Disable = FALSE;
	m_bF6_Disable = FALSE;
	m_bF7_Disable = FALSE;
	m_dF0_ScanFreq = 0;
	m_dF1_ScanFreq = 0;
	m_dF2_ScanFreq = 0;
	m_dF3_ScanFreq = 0;
	m_dF4_ScanFreq = 0;
	m_dF5_ScanFreq = 0;
	m_dF6_ScanFreq = 0;
	m_dF7_ScanFreq = 0;
	m_nF0_ScanDelay = 0;
	m_nF1_ScanDelay = 0;
	m_nF2_ScanDelay = 0;
	m_nF3_ScanDelay = 0;
	m_nF4_ScanDelay = 0;
	m_nF5_ScanDelay = 0;
	m_nF6_ScanDelay = 0;
	m_nF7_ScanDelay = 0;
	m_dwHomeIndex = 0;
	m_nSvcProvId0 = 1;
	m_nSvcProvId1 = 1;
	m_nSvcProvId2 = 1;
	m_nSvcProvId3 = 1;
	m_bSvcProv1Disable = FALSE;
	m_bSvcProv2Disable = FALSE;
	m_bSvcProv3Disable = FALSE;
	m_byRegFailRestTime = 0;
	m_byAsyncSleepFrames = 0;
	//}}AFX_DATA_INIT
}

CReflex25Protocol::~CReflex25Protocol()
{
}


void CReflex25Protocol::DoDataExchange(CDataExchange* pDX)
{
	CPpsFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReflex25Protocol)
	DDX_Check(pDX, IDC_DISABLE_F0, m_bF0_Disable);
	DDX_Check(pDX, IDC_DISABLE_F1, m_bF1_Disable);
	DDX_Check(pDX, IDC_DISABLE_F2, m_bF2_Disable);
	DDX_Check(pDX, IDC_DISABLE_F3, m_bF3_Disable);
	DDX_Check(pDX, IDC_DISABLE_F4, m_bF4_Disable);
	DDX_Check(pDX, IDC_DISABLE_F5, m_bF5_Disable);
	DDX_Check(pDX, IDC_DISABLE_F6, m_bF6_Disable);
	DDX_Check(pDX, IDC_DISABLE_F7, m_bF7_Disable);
	DDX_Text(pDX, IDC_SCAN_F0, m_dF0_ScanFreq);
	DDV_MinMaxDouble(pDX, m_dF0_ScanFreq, 929.00625, 941.80000);
	DDX_Text(pDX, IDC_SCAN_F1, m_dF1_ScanFreq);
	DDV_MinMaxDouble(pDX, m_dF1_ScanFreq, 929.00625, 941.80000);
	DDX_Text(pDX, IDC_SCAN_F2, m_dF2_ScanFreq);
	DDV_MinMaxDouble(pDX, m_dF2_ScanFreq, 929.00625, 941.80000);
	DDX_Text(pDX, IDC_SCAN_F3, m_dF3_ScanFreq);
	DDV_MinMaxDouble(pDX, m_dF3_ScanFreq, 929.00625, 941.80000);
	DDX_Text(pDX, IDC_SCAN_F4, m_dF4_ScanFreq);
	DDV_MinMaxDouble(pDX, m_dF4_ScanFreq, 929.00625, 941.80000);
	DDX_Text(pDX, IDC_SCAN_F5, m_dF5_ScanFreq);
	DDV_MinMaxDouble(pDX, m_dF5_ScanFreq, 929.00625, 941.80000);
	DDX_Text(pDX, IDC_SCAN_F6, m_dF6_ScanFreq);
	DDV_MinMaxDouble(pDX, m_dF6_ScanFreq, 929.00625, 941.80000);
	DDX_Text(pDX, IDC_SCAN_F7, m_dF7_ScanFreq);
	DDV_MinMaxDouble(pDX, m_dF7_ScanFreq, 929.00625, 941.80000);
	DDX_Text(pDX, IDC_DELAY_F0, m_nF0_ScanDelay);
	DDV_MinMaxInt(pDX, m_nF0_ScanDelay, 0, 7);
	DDX_Text(pDX, IDC_DELAY_F1, m_nF1_ScanDelay);
	DDV_MinMaxInt(pDX, m_nF1_ScanDelay, 0, 7);
	DDX_Text(pDX, IDC_DELAY_F2, m_nF2_ScanDelay);
	DDV_MinMaxInt(pDX, m_nF2_ScanDelay, 0, 7);
	DDX_Text(pDX, IDC_DELAY_F3, m_nF3_ScanDelay);
	DDV_MinMaxInt(pDX, m_nF3_ScanDelay, 0, 7);
	DDX_Text(pDX, IDC_DELAY_F4, m_nF4_ScanDelay);
	DDV_MinMaxInt(pDX, m_nF4_ScanDelay, 0, 7);
	DDX_Text(pDX, IDC_DELAY_F5, m_nF5_ScanDelay);
	DDV_MinMaxInt(pDX, m_nF5_ScanDelay, 0, 7);
	DDX_Text(pDX, IDC_DELAY_F6, m_nF6_ScanDelay);
	DDV_MinMaxInt(pDX, m_nF6_ScanDelay, 0, 7);
	DDX_Text(pDX, IDC_DELAY_F7, m_nF7_ScanDelay);
	DDV_MinMaxInt(pDX, m_nF7_ScanDelay, 0, 7);
	DDX_Text(pDX, IDC_HOME_INDEX, m_dwHomeIndex);
	DDV_MinMaxDWord(pDX, m_dwHomeIndex, 0, 4294967294);
	DDX_Text(pDX, IDC_SVC_PROV_ID0, m_nSvcProvId0);
	DDV_MinMaxUInt(pDX, m_nSvcProvId0, 0, 16384);
	DDX_Text(pDX, IDC_SVC_PROV_ID1, m_nSvcProvId1);
	DDV_MinMaxUInt(pDX, m_nSvcProvId1, 0, 16384);
	DDX_Text(pDX, IDC_SVC_PROV_ID2, m_nSvcProvId2);
	DDV_MinMaxUInt(pDX, m_nSvcProvId2, 0, 16384);
	DDX_Text(pDX, IDC_SVC_PROV_ID3, m_nSvcProvId3);
	DDV_MinMaxUInt(pDX, m_nSvcProvId3, 0, 16384);
	DDX_Check(pDX, IDC_SVC_PROV_DISABLE1, m_bSvcProv1Disable);
	DDX_Check(pDX, IDC_SVC_PROV_DISABLE2, m_bSvcProv2Disable);
	DDX_Check(pDX, IDC_SVC_PROV_DISABLE3, m_bSvcProv3Disable);
	DDX_Text(pDX, IDC_REGFAILRESTTIME_EDIT, m_byRegFailRestTime);
	DDV_MinMaxByte(pDX, m_byRegFailRestTime, 0, 255);
	DDX_Text(pDX, IDC_ASYNCSLEEPTIME_EDIT, m_byAsyncSleepFrames);
	DDV_MinMaxByte(pDX, m_byAsyncSleepFrames, 0, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReflex25Protocol, CPpsFormView)
	//{{AFX_MSG_MAP(CReflex25Protocol)
	ON_BN_CLICKED(IDC_PRO_DEFAULTS_BTN, OnAcceptDefaultsBtn)
	ON_BN_CLICKED(IDC_PRO_UNDO_BTN, OnUndoBtn)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SPIN_BUTTON, OnWmSpinButton)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CReflex25Protocol diagnostics

#ifdef _DEBUG
void CReflex25Protocol::AssertValid() const
{
	CPpsFormView::AssertValid();
}

void CReflex25Protocol::Dump(CDumpContext& dc) const
{
	CPpsFormView::Dump(dc);
}
#endif //_DEBUG




BOOL CReflex25Protocol::IsPresentable(void)
{ 
  BOOL		bR50Enable;
  CP_reflex50_enable.bSetFromDoc(&bR50Enable); 
  return(!bR50Enable);
}  	




////////////////////////////////////////////////////////////////////////////
//	OnInitialUpdate()
//  
//  Update DDX variables from document and resize frame around view.
//
void CReflex25Protocol::OnInitialUpdate(void)
{
  TRACE("CReflex25Protocol::OnInitialUpdate : CReflex25Protocol::IDD=%X\n", CReflex25Protocol::IDD);

  // subclass the edit controls in order to force numeric only input.
  m_F0_FreqEdit.SubclassDlgItem(IDC_SCAN_F0, this);
  m_F1_FreqEdit.SubclassDlgItem(IDC_SCAN_F1, this);
  m_F2_FreqEdit.SubclassDlgItem(IDC_SCAN_F2, this);
  m_F3_FreqEdit.SubclassDlgItem(IDC_SCAN_F3, this);
  m_F4_FreqEdit.SubclassDlgItem(IDC_SCAN_F4, this);
  m_F5_FreqEdit.SubclassDlgItem(IDC_SCAN_F5, this);
  m_F6_FreqEdit.SubclassDlgItem(IDC_SCAN_F6, this);
  m_F7_FreqEdit.SubclassDlgItem(IDC_SCAN_F7, this);
  
  m_F0_ScanDelayEdit.SubclassDlgItem(IDC_DELAY_F0, this);
  m_F0_ScanDelayEdit.LimitText(1);
  //
  m_F1_ScanDelayEdit.SubclassDlgItem(IDC_DELAY_F1, this);
  m_F1_ScanDelayEdit.LimitText(1); 
  //
  m_F2_ScanDelayEdit.SubclassDlgItem(IDC_DELAY_F2, this);
  m_F2_ScanDelayEdit.LimitText(1);
  //
  m_F3_ScanDelayEdit.SubclassDlgItem(IDC_DELAY_F3, this);
  m_F3_ScanDelayEdit.LimitText(1);
  //
  m_F4_ScanDelayEdit.SubclassDlgItem(IDC_DELAY_F4, this);
  m_F4_ScanDelayEdit.LimitText(1);
  //
  m_F5_ScanDelayEdit.SubclassDlgItem(IDC_DELAY_F5, this);
  m_F5_ScanDelayEdit.LimitText(1);
  //
  m_F6_ScanDelayEdit.SubclassDlgItem(IDC_DELAY_F6, this);
  m_F6_ScanDelayEdit.LimitText(1);
  //
  m_F7_ScanDelayEdit.SubclassDlgItem(IDC_DELAY_F7, this);
  m_F7_ScanDelayEdit.LimitText(1);

  // init the owner draw buttons              
  // ** Row #0
  VERIFY(m_F0_ChNumUpBtn.AutoLoad(IDC_CHNUM_UP_F0, this));
  VERIFY(m_F0_ChNumDnBtn.AutoLoad(IDC_CHNUM_DN_F0, this));
  VERIFY(m_F0_DelayUpBtn.AutoLoad(IDC_DELAY_UP_F0, this));
  VERIFY(m_F0_DelayDnBtn.AutoLoad(IDC_DELAY_DN_F0, this));

  // ** Row #1
  VERIFY(m_F1_ChNumUpBtn.AutoLoad(IDC_CHNUM_UP_F1, this));
  VERIFY(m_F1_ChNumDnBtn.AutoLoad(IDC_CHNUM_DN_F1, this));
  VERIFY(m_F1_DelayUpBtn.AutoLoad(IDC_DELAY_UP_F1, this));
  VERIFY(m_F1_DelayDnBtn.AutoLoad(IDC_DELAY_DN_F1, this));

  // ** Row #2
  VERIFY(m_F2_ChNumUpBtn.AutoLoad(IDC_CHNUM_UP_F2, this));
  VERIFY(m_F2_ChNumDnBtn.AutoLoad(IDC_CHNUM_DN_F2, this));
  VERIFY(m_F2_DelayUpBtn.AutoLoad(IDC_DELAY_UP_F2, this));
  VERIFY(m_F2_DelayDnBtn.AutoLoad(IDC_DELAY_DN_F2, this));

  // ** Row #3
  VERIFY(m_F3_ChNumUpBtn.AutoLoad(IDC_CHNUM_UP_F3, this));
  VERIFY(m_F3_ChNumDnBtn.AutoLoad(IDC_CHNUM_DN_F3, this));
  VERIFY(m_F3_DelayUpBtn.AutoLoad(IDC_DELAY_UP_F3, this));
  VERIFY(m_F3_DelayDnBtn.AutoLoad(IDC_DELAY_DN_F3, this));

  // ** Row #4
  VERIFY(m_F4_ChNumUpBtn.AutoLoad(IDC_CHNUM_UP_F4, this));
  VERIFY(m_F4_ChNumDnBtn.AutoLoad(IDC_CHNUM_DN_F4, this));
  VERIFY(m_F4_DelayUpBtn.AutoLoad(IDC_DELAY_UP_F4, this));
  VERIFY(m_F4_DelayDnBtn.AutoLoad(IDC_DELAY_DN_F4, this));

  // ** Row #5
  VERIFY(m_F5_ChNumUpBtn.AutoLoad(IDC_CHNUM_UP_F5, this));
  VERIFY(m_F5_ChNumDnBtn.AutoLoad(IDC_CHNUM_DN_F5, this));
  VERIFY(m_F5_DelayUpBtn.AutoLoad(IDC_DELAY_UP_F5, this));
  VERIFY(m_F5_DelayDnBtn.AutoLoad(IDC_DELAY_DN_F5, this));

  // ** Row #6
  VERIFY(m_F6_ChNumUpBtn.AutoLoad(IDC_CHNUM_UP_F6, this));
  VERIFY(m_F6_ChNumDnBtn.AutoLoad(IDC_CHNUM_DN_F6, this));
  VERIFY(m_F6_DelayUpBtn.AutoLoad(IDC_DELAY_UP_F6, this));
  VERIFY(m_F6_DelayDnBtn.AutoLoad(IDC_DELAY_DN_F6, this));

  // ** Row #7
  VERIFY(m_F7_ChNumUpBtn.AutoLoad(IDC_CHNUM_UP_F7, this));
  VERIFY(m_F7_ChNumDnBtn.AutoLoad(IDC_CHNUM_DN_F7, this));
  VERIFY(m_F7_DelayUpBtn.AutoLoad(IDC_DELAY_UP_F7, this));
  VERIFY(m_F7_DelayDnBtn.AutoLoad(IDC_DELAY_DN_F7, this));

  HomeIndexEdit.SubclassDlgItem(IDC_HOME_INDEX, this);
  HomeIndexEdit.LimitText(10);
  
  SvcProvEdit0.SubclassDlgItem(IDC_SVC_PROV_ID0, this);
  SvcProvEdit0.LimitText(5);
  
  SvcProvEdit1.SubclassDlgItem(IDC_SVC_PROV_ID1, this);
  SvcProvEdit1.LimitText(5);
  
  SvcProvEdit2.SubclassDlgItem(IDC_SVC_PROV_ID2, this);
  SvcProvEdit2.LimitText(5);
  
  SvcProvEdit3.SubclassDlgItem(IDC_SVC_PROV_ID3, this);
  SvcProvEdit3.LimitText(5);
  
  CPpsFormView::OnInitialUpdate();
  return;
}




/////////////////////////////////////////////////////////////////////////////
//	OnUpdate()
//
void CReflex25Protocol::OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint)
{
  TRACE("\nCReflex25Protocol::OnUpdate : nViewID=%X  LHintHI=%X  lHintLO=%X\n", 
  		CReflex25Protocol::IDD,  HIWORD(lHint), LOWORD(lHint));

  if (HIWORD(lHint))        		// user switched views, now active
    ResizeParentToFit(FALSE);
  
  // check for Reflex50 enabled.  The user may have performed a read of
  // a Reflex50 pager while this view was visible.
  BOOL bR50Enable;
  CP_reflex50_enable.bSetFromDoc(&bR50Enable);
  if (bR50Enable == TRUE) 
  {
    theApp.SwitchView(1);    // switch to the logo view
    return;
  }
  else
    SetFromDoc(); 
  
  CEdit *pEdit = NULL;
  CStatic *pStatic = NULL;      
  BYTE	byDeviceId = GetDocument()->GetDeviceId();
  BYTE	byCpVersion = GetDocument()->GetCpVersion();
  BOOL  bEnable = ((byDeviceId >= PAGEFINDER) ||
                   ((byDeviceId <= PAGEFINDER) && (byCpVersion >= 0x06)));
  pEdit = (CEdit *)GetDlgItem(IDC_REGFAILRESTTIME_EDIT);
  ASSERT(pEdit != NULL);
  pEdit->EnableWindow(bEnable);
  pStatic = (CStatic *)GetDlgItem(IDC_PCRT1_STATIC);
  ASSERT(pStatic != NULL);
  pStatic->EnableWindow(bEnable);
  pStatic = (CStatic *)GetDlgItem(IDC_PCRT2_STATIC);
  ASSERT(pStatic != NULL);
  pStatic->EnableWindow(bEnable);
    
  CPpsFormView::OnUpdate(pSender, lHint, pHint);
}	


////////////////////////////////////////////////////////////////////////////
//  SetFromDoc()
//
void CReflex25Protocol::SetFromDoc(void)
{
  TRACE("\nCReflex25Protocol::SetFromDoc\n");
  
  // ** Row0  
  CP_CtrlChanNumber.nSetFromDoc((UINT *)&m_nF0_ChanNum);
  CP_CtrlChanScanDelay.nSetFromDoc((UINT *)&m_nF0_ScanDelay);
  CP_CtrlChanDisable.bSetFromDoc(&m_bF0_Disable);
  m_dF0_ScanFreq = dCalcForwardChanFreq(m_nF0_ChanNum);
  
  // ** Row1 
  CP_CtrlChanNumber.nSetFromDoc((UINT *)&m_nF1_ChanNum, 2);
  CP_CtrlChanScanDelay.nSetFromDoc((UINT *)&m_nF1_ScanDelay, 2);
  CP_CtrlChanDisable.bSetFromDoc(&m_bF1_Disable, 2);
  m_dF1_ScanFreq = dCalcForwardChanFreq(m_nF1_ChanNum);
  
  // ** Row2
  CP_CtrlChanNumber.nSetFromDoc((UINT *)&m_nF2_ChanNum, 4);
  CP_CtrlChanScanDelay.nSetFromDoc((UINT *)&m_nF2_ScanDelay, 4);
  CP_CtrlChanDisable.bSetFromDoc(&m_bF2_Disable, 4);
  m_dF2_ScanFreq = dCalcForwardChanFreq(m_nF2_ChanNum);
  
  // ** Row3
  CP_CtrlChanNumber.nSetFromDoc((UINT *)&m_nF3_ChanNum, 6);
  CP_CtrlChanScanDelay.nSetFromDoc((UINT *)&m_nF3_ScanDelay, 6);
  CP_CtrlChanDisable.bSetFromDoc(&m_bF3_Disable, 6);
  m_dF3_ScanFreq = dCalcForwardChanFreq(m_nF3_ChanNum);
  
  // ** Row4
  CP_CtrlChanNumber.nSetFromDoc((UINT *)&m_nF4_ChanNum, 8);
  CP_CtrlChanScanDelay.nSetFromDoc((UINT *)&m_nF4_ScanDelay, 8);
  CP_CtrlChanDisable.bSetFromDoc(&m_bF4_Disable, 8);
  m_dF4_ScanFreq = dCalcForwardChanFreq(m_nF4_ChanNum);
  
  // ** Row5
  CP_CtrlChanNumber.nSetFromDoc((UINT *)&m_nF5_ChanNum, 10);
  CP_CtrlChanScanDelay.nSetFromDoc((UINT *)&m_nF5_ScanDelay, 10);
  CP_CtrlChanDisable.bSetFromDoc(&m_bF5_Disable, 10);
  m_dF5_ScanFreq = dCalcForwardChanFreq(m_nF5_ChanNum);
  
  // ** Row6
  CP_CtrlChanNumber.nSetFromDoc((UINT *)&m_nF6_ChanNum, 12);
  CP_CtrlChanScanDelay.nSetFromDoc((UINT *)&m_nF6_ScanDelay, 12);
  CP_CtrlChanDisable.bSetFromDoc(&m_bF6_Disable, 12);
  m_dF6_ScanFreq = dCalcForwardChanFreq(m_nF6_ChanNum);
  
  // ** Row7
  CP_CtrlChanNumber.nSetFromDoc((UINT *)&m_nF7_ChanNum, 14);
  CP_CtrlChanScanDelay.nSetFromDoc((UINT *)&m_nF7_ScanDelay, 14);
  CP_CtrlChanDisable.bSetFromDoc(&m_bF7_Disable, 14);
  m_dF7_ScanFreq = dCalcForwardChanFreq(m_nF7_ChanNum);

  // *** Service Provider Ids  
  //CP_ServiceProviderDisable.bSetFromDoc(&m_bSvcProv0Disable, 0);
  //CP_ServiceProviderAncillary.bSetFromDoc(&m_bSvcProv0Ancillary, 0);
  CP_ServiceProviderId.nSetFromDoc((UINT *)&m_nSvcProvId0, 0);

  CP_ServiceProviderDisable.bSetFromDoc(&m_bSvcProv1Disable, 2);
  //CP_ServiceProviderAncillary.bSetFromDoc(&m_bSvcProv1Ancillary, 2);
  CP_ServiceProviderId.nSetFromDoc((UINT *)&m_nSvcProvId1, 2);

  CP_ServiceProviderDisable.bSetFromDoc(&m_bSvcProv2Disable, 4);
  //CP_ServiceProviderAncillary.bSetFromDoc(&m_bSvcProv2Ancillary, 4);
  CP_ServiceProviderId.nSetFromDoc((UINT *)&m_nSvcProvId2, 4);

  CP_ServiceProviderDisable.bSetFromDoc(&m_bSvcProv3Disable, 6);
  //CP_ServiceProviderAncillary.bSetFromDoc(&m_bSvcProv3Ancillary, 6);
  CP_ServiceProviderId.nSetFromDoc((UINT *)&m_nSvcProvId3, 6);
  
  // *** Home Index
  CP_Home_Index.dwSetFromDoc(&m_dwHomeIndex);
   
  // *** CP_RegFailRestTime  
  BYTE	byDeviceId = GetDocument()->GetDeviceId();
  BYTE	byCpVersion = GetDocument()->GetCpVersion();
  if ((byDeviceId >= PAGEFINDER) ||
      ((byDeviceId <= PAGEFINDER) && (byCpVersion >= 0x06)))
    CP_RegFailRestTime.bySetFromDoc(&m_byRegFailRestTime);
  
  // CP_AsyncSleepTime
  CP_AsyncSleepTime.bySetFromDoc(&m_byAsyncSleepFrames);

  UpdateData(FALSE);	// set controls data
}



////////////////////////////////////////////////////////////////////////////
//	SetToDoc()
// 
static char *szMsgInvalidSPID = "A Service Provider ID equal to zero is allowed\n"
                                "only if the Service Provider entry is also disabled.\n"
                                "Either disable the entry or enter a non-zero ID.";
//
BOOL CReflex25Protocol::SetToDoc(void)
{
  TRACE("\nCReflex25Protocol::SetToDoc\n");
  CPpsDoc		*pDoc = GetDocument(); 
  
  if (!(UpdateData(TRUE)))  // get controls data
    return FALSE;

  // allow a SPID==0 only if the SPID is also disabled
  if ((m_nSvcProvId0 == 0))  // primary SPID entry cannot be 0 and cannot be disabled
  { 
    AfxMessageBox(szMsgInvalidSPID);
    ((CDialog *)this)->GotoDlgCtrl(&SvcProvEdit0);
    return(FALSE);
  }
  if ((m_nSvcProvId1 == 0) && (m_bSvcProv1Disable == FALSE))
  { 
    AfxMessageBox(szMsgInvalidSPID);
    ((CDialog *)this)->GotoDlgCtrl(&SvcProvEdit1);
    return(FALSE);
  }
  if ((m_nSvcProvId2 == 0) && (m_bSvcProv2Disable == FALSE))
  { 
    AfxMessageBox(szMsgInvalidSPID);
    ((CDialog *)this)->GotoDlgCtrl(&SvcProvEdit2);
    return(FALSE);
  }
  if ((m_nSvcProvId3 == 0) && (m_bSvcProv3Disable == FALSE))
  { 
    AfxMessageBox(szMsgInvalidSPID);
    ((CDialog *)this)->GotoDlgCtrl(&SvcProvEdit3);
    return(FALSE);
  }

  m_nF0_ChanNum = nCalcForwardChanNum(m_dF0_ScanFreq, NO_ROUNDING);
  m_nF1_ChanNum = nCalcForwardChanNum(m_dF1_ScanFreq, NO_ROUNDING);
  m_nF2_ChanNum = nCalcForwardChanNum(m_dF2_ScanFreq, NO_ROUNDING);
  m_nF3_ChanNum = nCalcForwardChanNum(m_dF3_ScanFreq, NO_ROUNDING);
  m_nF4_ChanNum = nCalcForwardChanNum(m_dF4_ScanFreq, NO_ROUNDING);
  m_nF5_ChanNum = nCalcForwardChanNum(m_dF5_ScanFreq, NO_ROUNDING);
  m_nF6_ChanNum = nCalcForwardChanNum(m_dF6_ScanFreq, NO_ROUNDING);
  m_nF7_ChanNum = nCalcForwardChanNum(m_dF7_ScanFreq, NO_ROUNDING);
   
  
  // *** CHANGE 10/25/96.  The device requires that enabled entries
  // *** in the forward channel scan list be saved contiguously from
  // *** the top of the list (top = index[0]).  Consequently, first
  // *** save all enabled channels, then save all disabled channel.  
  int index = 0;
  
  // **** SAVE ENABLED CHANNELS CONTIGOUSLY FIRST   
  if (!m_bF0_Disable) // ** Row0
    SaveControlChannel(m_nF0_ChanNum, m_bF0_Disable, m_nF0_ScanDelay, &index);
  if (!m_bF1_Disable) // ** Row1
    SaveControlChannel(m_nF1_ChanNum, m_bF1_Disable, m_nF1_ScanDelay, &index);
  if (!m_bF2_Disable) // ** Row2 
    SaveControlChannel(m_nF2_ChanNum, m_bF2_Disable, m_nF2_ScanDelay, &index);
  if (!m_bF3_Disable) // ** Row3
    SaveControlChannel(m_nF3_ChanNum, m_bF3_Disable, m_nF3_ScanDelay, &index);
  if (!m_bF4_Disable) // ** Row4 
    SaveControlChannel(m_nF4_ChanNum, m_bF4_Disable, m_nF4_ScanDelay, &index);
  if (!m_bF5_Disable) // ** Row5 
    SaveControlChannel(m_nF5_ChanNum, m_bF5_Disable, m_nF5_ScanDelay, &index);
  if (!m_bF6_Disable) // ** Row6 
    SaveControlChannel(m_nF6_ChanNum, m_bF6_Disable, m_nF6_ScanDelay, &index);
  if (!m_bF7_Disable) // ** Row7 
    SaveControlChannel(m_nF7_ChanNum, m_bF7_Disable, m_nF7_ScanDelay, &index);  

  // **** SAVE DISABLED CHANNELS CONTIGUOUSLY NEXT   
  if (m_bF0_Disable) // ** Row0
    SaveControlChannel(m_nF0_ChanNum, m_bF0_Disable, m_nF0_ScanDelay, &index);
  if (m_bF1_Disable) // ** Row1
    SaveControlChannel(m_nF1_ChanNum, m_bF1_Disable, m_nF1_ScanDelay, &index);
  if (m_bF2_Disable) // ** Row2 
    SaveControlChannel(m_nF2_ChanNum, m_bF2_Disable, m_nF2_ScanDelay, &index);
  if (m_bF3_Disable) // ** Row3
    SaveControlChannel(m_nF3_ChanNum, m_bF3_Disable, m_nF3_ScanDelay, &index);
  if (m_bF4_Disable) // ** Row4 
    SaveControlChannel(m_nF4_ChanNum, m_bF4_Disable, m_nF4_ScanDelay, &index);
  if (m_bF5_Disable) // ** Row5 
    SaveControlChannel(m_nF5_ChanNum, m_bF5_Disable, m_nF5_ScanDelay, &index);
  if (m_bF6_Disable) // ** Row6 
    SaveControlChannel(m_nF6_ChanNum, m_bF6_Disable, m_nF6_ScanDelay, &index);
  if (m_bF7_Disable) // ** Row7 
    SaveControlChannel(m_nF7_ChanNum, m_bF7_Disable, m_nF7_ScanDelay, &index);  

   
  // *** Service Provider Ids 
  CP_ServiceProviderDisable.bSetToDoc(FALSE, 0);               // primary SPID - force to FALSE
  CP_ServiceProviderAncillary.bSetToDoc(FALSE, 0);             // primary SPID - force to FALSE
  CP_ServiceProviderId.nSetToDoc((UINT)m_nSvcProvId0, 0);
  //
  CP_ServiceProviderDisable.bSetToDoc(m_bSvcProv1Disable, 2);
  //CP_ServiceProviderAncillary.bSetToDoc(m_bSvcProv1Ancillary, 2);
  CP_ServiceProviderId.nSetToDoc((UINT)m_nSvcProvId1, 2); 
  //
  CP_ServiceProviderDisable.bSetToDoc(m_bSvcProv2Disable, 4);
  //CP_ServiceProviderAncillary.bSetToDoc(m_bSvcProv2Ancillary, 4);
  CP_ServiceProviderId.nSetToDoc((UINT)m_nSvcProvId2, 4);
  //
  CP_ServiceProviderDisable.bSetToDoc(m_bSvcProv3Disable, 6);
  //CP_ServiceProviderAncillary.bSetToDoc(m_bSvcProv3Ancillary, 6);
  CP_ServiceProviderId.nSetToDoc((UINT)m_nSvcProvId3, 6);
  
  // *** Home Index
  CP_Home_Index.dwSetToDoc(m_dwHomeIndex);   

  // *** CP_RegFailRestTime
  BYTE	byDeviceId = GetDocument()->GetDeviceId();
  BYTE	byCpVersion = GetDocument()->GetCpVersion();
  if ((byDeviceId >= PAGEFINDER) ||
      ((byDeviceId <= PAGEFINDER) && (byCpVersion >= 0x06)))
    CP_RegFailRestTime.bySetToDoc(m_byRegFailRestTime);
  
  // CP_AsyncSleepTime
  CP_AsyncSleepTime.bySetToDoc(m_byAsyncSleepFrames); 

  return(TRUE);
}
                                                               

void CReflex25Protocol::SaveControlChannel(int nChannel, BOOL bDisabled, int nScanTime, int *nIndex)
{
  CP_CtrlChanNumber.nSetToDoc((UINT)nChannel, *nIndex);
  CP_CtrlChanScanDelay.nSetToDoc((UINT)nScanTime, *nIndex);
  CP_CtrlChanDisable.bSetToDoc(bDisabled, *nIndex);
  *nIndex += 2;  
  return;
}



int CReflex25Protocol::nCalcForwardChanNum(double dFwdFreq, BYTE byRounding)
{ 
  int	nCeil = (int)(ceil((dFwdFreq - 929.00625F) / 0.00625F));
  int   nFloor = (int)(floor((dFwdFreq - 929.00625F) / 0.00625F));

  if (byRounding == ROUND_UP)
  {
    return(nCeil);
  }
  else if (byRounding == ROUND_DN)
  {
    return(nFloor);
  }
  else if (nCeil != nFloor)  // reverse calculation and compare to given dFwdFreq
  {
    double dCeil = dCalcForwardChanFreq(nCeil);
    double dFloor = dCalcForwardChanFreq(nFloor); 
    double dDistanceHi = pow((dCeil - dFwdFreq), 2);
    double dDistanceLo = pow((dFwdFreq - dFloor), 2); 
    if (dDistanceHi < dDistanceLo)
      return(nCeil);
  }
  return(nFloor);   
}


int CReflex25Protocol::nCalcReverseChanNum(double dRevFreq, BYTE byRounding)
{ 
  int	nCeil = (int)(ceil((dRevFreq - 896.0125F) / 0.00625F));
  int   nFloor = (int)(floor((dRevFreq - 896.0125F) / 0.00625F));

  if (byRounding == ROUND_UP)
  {
    return(nCeil);
  }
  else if (byRounding == ROUND_DN)
  {
    return(nFloor);
  }
  else if (nCeil != nFloor)  // reverse calculation and compare to given dRevFreq
  {
    double dCeil = dCalcReverseChanFreq(nCeil);
    double dFloor = dCalcReverseChanFreq(nFloor); 
    double dDistanceHi = pow((dCeil - dRevFreq), 2);
    double dDistanceLo = pow((dRevFreq - dFloor), 2); 
    if (dDistanceHi < dDistanceLo)
      return(nCeil);
  }
  return(nFloor);   
}



/////////////////////////////////////////////////////////////////////////////
// CReflex25Protocol message handlers



////////////////////////////////////////////////////////////////////////
//  OnAcceptDefaultsBtn()
//
//  This function initiates a move of data from select fields of the
//  CPpsDoc.m_byDefaultCodePlugArray to the CPpsDoc.m_byCodePlugArray.
//  The m_byDefaultCodePlugArray is loaded from file 'default.s19' upon
//  application execution.  If the file has not been found, then no 
//  default values are available.  
//
void CReflex25Protocol::OnAcceptDefaultsBtn()
{
  TRACE("\nCReflex25Protocol::OnAcceptDefaultsBtn\n"); 
  CPpsDoc	*pDoc = GetDocument(); 
  
  if (pDoc->SetFieldFromDefault(CP_Control_Channel.Offset(), CP_Control_Channel.Length()))
    return;
  if (pDoc->SetFieldFromDefault(CP_Service_Provider_Ids.Offset(), CP_Service_Provider_Ids.Length()))
    return;    
  if (pDoc->SetFieldFromDefault(CP_Home_Index.Offset(), CP_Home_Index.Length()))
    return;
  BYTE	byDeviceId = pDoc->GetDeviceId();
  BYTE	byCpVersion = pDoc->GetCpVersion();
  if ((byDeviceId >= PAGEFINDER) ||
      ((byDeviceId <= PAGEFINDER) && (byCpVersion >= 0x06)))
  {
    if (pDoc->SetFieldFromDefault(CP_RegFailRestTime.Offset(), CP_RegFailRestTime.Length()))
      return;
  }  
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
void CReflex25Protocol::OnArchived(void)
{
  CPpsDoc	*pDoc = GetDocument(); 
   
  if (pDoc->SetFieldFromArchived(CP_Control_Channel.Offset(), CP_Control_Channel.Length()))
    return;
  if (pDoc->SetFieldFromArchived(CP_Service_Provider_Ids.Offset(), CP_Service_Provider_Ids.Length()))
    return;    
  if (pDoc->SetFieldFromArchived(CP_Home_Index.Offset(), CP_Home_Index.Length()))
    return;
  BYTE	byDeviceId = pDoc->GetDeviceId();
  BYTE	byCpVersion = pDoc->GetCpVersion();
  if ((byDeviceId >= PAGEFINDER) ||
      ((byDeviceId <= PAGEFINDER) && (byCpVersion >= 0x06)))
  {
    if (pDoc->SetFieldFromArchived(CP_RegFailRestTime.Offset(), CP_RegFailRestTime.Length()))
      return;
  }  
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
void CReflex25Protocol::OnUndoBtn()
{
  TRACE("\nCReflex25Protocol::OnUndoBtn\n"); 
  CPpsDoc	*pDoc = GetDocument(); 

  if (pDoc->SetFieldFromOriginal(CP_Control_Channel.Offset(), CP_Control_Channel.Length()))
    return;
  if (pDoc->SetFieldFromOriginal(CP_Service_Provider_Ids.Offset(), CP_Service_Provider_Ids.Length()))
    return;    
  if (pDoc->SetFieldFromOriginal(CP_Home_Index.Offset(), CP_Home_Index.Length()))
    return;    
  BYTE	byDeviceId = pDoc->GetDeviceId();
  BYTE	byCpVersion = pDoc->GetCpVersion();
  if ((byDeviceId >= PAGEFINDER) ||
      ((byDeviceId <= PAGEFINDER) && (byCpVersion >= 0x06)))
  {
    if (pDoc->SetFieldFromOriginal(CP_RegFailRestTime.Offset(), CP_RegFailRestTime.Length()))
      return;
  }  
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
afx_msg LONG CReflex25Protocol::OnWmSpinButton(UINT wParam, LONG lParam)
{
  TRACE("\nCReflex25Protocol::OnWmSpinButton : nBtnID=%04X\n", wParam);
  
  int nAcceleration = LOWORD(lParam);
  
  if (!UpdateData(TRUE))  // get controls data, return on error
    return(0L);

  switch(wParam)
  {
    case IDC_CHNUM_UP_F0 : 
        OnChannelNumberIncrement(&m_nF0_ChanNum, &m_dF0_ScanFreq, nAcceleration);
        break; 
    case IDC_CHNUM_DN_F0 : 
        OnChannelNumberDecrement(&m_nF0_ChanNum, &m_dF0_ScanFreq, nAcceleration); 
        break;
    case IDC_CHNUM_UP_F1 :
        OnChannelNumberIncrement(&m_nF1_ChanNum, &m_dF1_ScanFreq, nAcceleration);
        break; 
    case IDC_CHNUM_DN_F1 : 
        OnChannelNumberDecrement(&m_nF1_ChanNum, &m_dF1_ScanFreq, nAcceleration); 
        break;
    case IDC_CHNUM_UP_F2 :
        OnChannelNumberIncrement(&m_nF2_ChanNum, &m_dF2_ScanFreq, nAcceleration);
        break; 
    case IDC_CHNUM_DN_F2 : 
        OnChannelNumberDecrement(&m_nF2_ChanNum, &m_dF2_ScanFreq, nAcceleration); 
        break;
    case IDC_CHNUM_UP_F3 :
        OnChannelNumberIncrement(&m_nF3_ChanNum, &m_dF3_ScanFreq, nAcceleration);
        break; 
    case IDC_CHNUM_DN_F3 : 
        OnChannelNumberDecrement(&m_nF3_ChanNum, &m_dF3_ScanFreq, nAcceleration); 
        break;
    case IDC_CHNUM_UP_F4 :
        OnChannelNumberIncrement(&m_nF4_ChanNum, &m_dF4_ScanFreq, nAcceleration);
        break; 
    case IDC_CHNUM_DN_F4 : 
        OnChannelNumberDecrement(&m_nF4_ChanNum, &m_dF4_ScanFreq, nAcceleration); 
        break;
    case IDC_CHNUM_UP_F5 :
        OnChannelNumberIncrement(&m_nF5_ChanNum, &m_dF5_ScanFreq, nAcceleration);
        break; 
    case IDC_CHNUM_DN_F5 : 
        OnChannelNumberDecrement(&m_nF5_ChanNum, &m_dF5_ScanFreq, nAcceleration); 
        break;
    case IDC_CHNUM_UP_F6 :
        OnChannelNumberIncrement(&m_nF6_ChanNum, &m_dF6_ScanFreq, nAcceleration);
        break; 
    case IDC_CHNUM_DN_F6 : 
        OnChannelNumberDecrement(&m_nF6_ChanNum, &m_dF6_ScanFreq, nAcceleration); 
        break;
    case IDC_CHNUM_UP_F7 :
        OnChannelNumberIncrement(&m_nF7_ChanNum, &m_dF7_ScanFreq, nAcceleration);
        break; 
    case IDC_CHNUM_DN_F7 : 
        OnChannelNumberDecrement(&m_nF7_ChanNum, &m_dF7_ScanFreq, nAcceleration); 
        break;
    //
    case IDC_DELAY_UP_F0 :
        OnScanDelayIncrement(&m_nF0_ScanDelay, 1);
        break;
    case IDC_DELAY_DN_F0 :
        OnScanDelayDecrement(&m_nF0_ScanDelay, 1);
        break;
    case IDC_DELAY_UP_F1 :
        OnScanDelayIncrement(&m_nF1_ScanDelay, 1);
        break;
    case IDC_DELAY_DN_F1 :
        OnScanDelayDecrement(&m_nF1_ScanDelay, 1);
        break;
    case IDC_DELAY_UP_F2 :
        OnScanDelayIncrement(&m_nF2_ScanDelay, 1);
        break;
    case IDC_DELAY_DN_F2 :
        OnScanDelayDecrement(&m_nF2_ScanDelay, 1);
        break;
    case IDC_DELAY_UP_F3 :
        OnScanDelayIncrement(&m_nF3_ScanDelay, 1);
        break;
    case IDC_DELAY_DN_F3 :
        OnScanDelayDecrement(&m_nF3_ScanDelay, 1);
        break;
    case IDC_DELAY_UP_F4 :
        OnScanDelayIncrement(&m_nF4_ScanDelay, 1);
        break;
    case IDC_DELAY_DN_F4 :
        OnScanDelayDecrement(&m_nF4_ScanDelay, 1);
        break;
    case IDC_DELAY_UP_F5 :
        OnScanDelayIncrement(&m_nF5_ScanDelay, 1);
        break;
    case IDC_DELAY_DN_F5 :
        OnScanDelayDecrement(&m_nF5_ScanDelay, 1);
        break;
    case IDC_DELAY_UP_F6 :
        OnScanDelayIncrement(&m_nF6_ScanDelay, 1);
        break;
    case IDC_DELAY_DN_F6 :
        OnScanDelayDecrement(&m_nF6_ScanDelay, 1);
        break;
    case IDC_DELAY_UP_F7 :
        OnScanDelayIncrement(&m_nF7_ScanDelay, 1);
        break;
    case IDC_DELAY_DN_F7 :
        OnScanDelayDecrement(&m_nF7_ScanDelay, 1);
        break; 
    default:
      break;
  }
  
  UpdateData(FALSE);	// set controls data 
  return(0L);
}


/////////////////////
void CReflex25Protocol::OnChannelNumberIncrement(int *pnChanNum, 
											     double *pdFrequency,
											     int nOffsetBy)
{ 
  *pnChanNum = nCalcForwardChanNum(*pdFrequency, ROUND_UP);
  
  *pnChanNum += nOffsetBy;  
  if (((*pnChanNum) % 2) == 0)    // allow only odd channel numbers
    *pnChanNum += 1;
  
  if ((*pnChanNum) > MAX_FWD_CHAN)
    *pnChanNum = MAX_FWD_CHAN;

  *pdFrequency = dCalcForwardChanFreq(*pnChanNum);  
  return; 	
}


//////////////////////
void CReflex25Protocol::OnChannelNumberDecrement(int *pnChanNum, 
												 double *pdFrequency,
												 int nOffsetBy)
{
  *pnChanNum = nCalcForwardChanNum(*pdFrequency, ROUND_DN);
  
  *pnChanNum -= nOffsetBy;  
  if (((*pnChanNum) % 2) == 0)    // allow only odd channel numbers
    *pnChanNum -= 1;

  if ((*pnChanNum) < MIN_FWD_CHAN)
    *pnChanNum = MIN_FWD_CHAN;

  *pdFrequency = dCalcForwardChanFreq(*pnChanNum);
  return; 	
}


 
////////////////////
void CReflex25Protocol::OnScanDelayIncrement(int *pnScanDelay, 
											int nOffsetBy)
{
  if ((*pnScanDelay + nOffsetBy) > 7)
    *pnScanDelay = 7;
  else 
    *pnScanDelay += nOffsetBy;
  return;
}


void CReflex25Protocol::OnScanDelayDecrement(int *pnScanDelay,
											int nOffsetBy)
{
  if ((*pnScanDelay - nOffsetBy) < 0)
    *pnScanDelay = 0;
  else
    *pnScanDelay -= nOffsetBy;
  return;
}



int CReflex25Protocol::OnPrintHelper(CStringArray &rStringArray, UINT nPageNumber, int nAvailableLines)
{ 
   BOOL bR50Enable = FALSE;
  
   // check for Reflex50 enabled
   CP_reflex50_enable.bSetFromDoc(&bR50Enable);
   if (bR50Enable == TRUE)
     return(0);

   // if not enough lines available, wait for next page...
   const int nNumLinesRequired = 19;    // count the number of 'rStringArray.Add' calls
   if (nAvailableLines < nNumLinesRequired)
     return(-1);    
   int nNumLinesBegin = rStringArray.GetSize();

   char    szTemp[200];
   
   rStringArray.Add("PROTOCOL");
   
   rStringArray.Add("    Control Channels:");
     sprintf(szTemp, "        (0)\t %10lf, scan-delay=%d, disable=%d", m_dF0_ScanFreq, m_nF0_ScanDelay, m_bF0_Disable);
     rStringArray.Add(szTemp);
     sprintf(szTemp, "        (1)\t %10lf, scan-delay=%d, disable=%d", m_dF1_ScanFreq, m_nF1_ScanDelay, m_bF1_Disable);
     rStringArray.Add(szTemp);
     sprintf(szTemp, "        (2)\t %10lf, scan-delay=%d, disable=%d", m_dF2_ScanFreq, m_nF2_ScanDelay, m_bF2_Disable);
     rStringArray.Add(szTemp);
     sprintf(szTemp, "        (3)\t %10lf, scan-delay=%d, disable=%d", m_dF3_ScanFreq, m_nF3_ScanDelay, m_bF3_Disable);
     rStringArray.Add(szTemp);
     sprintf(szTemp, "        (4)\t %10lf, scan-delay=%d, disable=%d", m_dF4_ScanFreq, m_nF4_ScanDelay, m_bF4_Disable);
     rStringArray.Add(szTemp);
     sprintf(szTemp, "        (5)\t %10lf, scan-delay=%d, disable=%d", m_dF5_ScanFreq, m_nF5_ScanDelay, m_bF5_Disable);
     rStringArray.Add(szTemp);
     sprintf(szTemp, "        (6)\t %10lf, scan-delay=%d, disable=%d", m_dF6_ScanFreq, m_nF6_ScanDelay, m_bF6_Disable);
     rStringArray.Add(szTemp);
     sprintf(szTemp, "        (7)\t %10lf, scan-delay=%d, disable=%d", m_dF7_ScanFreq, m_nF7_ScanDelay, m_bF7_Disable);
     rStringArray.Add(szTemp);
   
   rStringArray.Add("    Service Providers:");
     sprintf(szTemp, "        (0)\t %d, primary", m_nSvcProvId0);
     rStringArray.Add(szTemp);
     sprintf(szTemp, "        (1)\t %d, disable=%d", m_nSvcProvId1, m_bSvcProv1Disable);
     rStringArray.Add(szTemp);
     sprintf(szTemp, "        (2)\t %d, disable=%d", m_nSvcProvId2, m_bSvcProv2Disable);
     rStringArray.Add(szTemp);
     sprintf(szTemp, "        (3)\t %d, disable=%d", m_nSvcProvId3, m_bSvcProv3Disable);
     rStringArray.Add(szTemp);
     
   sprintf(szTemp, "    Home Index:  %10lu", m_dwHomeIndex);
   rStringArray.Add(szTemp);
   
   BYTE	byDeviceId = GetDocument()->GetDeviceId();
   BYTE	byCpVersion = GetDocument()->GetCpVersion();
   if ((byDeviceId >= PAGEFINDER) ||
       ((byDeviceId <= PAGEFINDER) && (byCpVersion >= 0x06)))
   {
     sprintf(szTemp, "    Partial Coverage Registration Rest Timer:  %d", m_byRegFailRestTime);
     rStringArray.Add(szTemp);
   } 
   
   sprintf(szTemp, "      Asynchronous Sleep Cycle:  %d", m_byAsyncSleepFrames); 
   rStringArray.Add(szTemp);
   
   rStringArray.Add("");
   return(rStringArray.GetSize() - nNumLinesBegin);
}


