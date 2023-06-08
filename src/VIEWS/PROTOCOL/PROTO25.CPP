//*************************************************************
//  History:    Date       Author               Comment
//              1997       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
// proto25.cpp : Reflex25 channel configuration implementation file.
//               This view designed upon change from 8 to 16 control
//               channels.
//

#include "stdafx.h"
#include "..\..\pps.h"
#include "..\..\ppsdoc.h"
#include "..\..\ppsview.h" 
#include "..\..\cpager\codeplug.h"
#include "protocol.h" 
#include "proto25.h"
#include "math.h"		// for the 'ceil' function

extern CPpsApp    theApp;

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProto25View

IMPLEMENT_DYNCREATE(CProto25View, CPpsFormView)

CProto25View::CProto25View()
	: CPpsFormView(CProto25View::IDD)
{
	m_nF0_ChanNum = 0;
	m_nF1_ChanNum = 0;
	m_nF2_ChanNum = 0;
	m_nF3_ChanNum = 0;
	m_nF4_ChanNum = 0;
	m_nF5_ChanNum = 0;
	m_nF6_ChanNum = 0;
	m_nF7_ChanNum = 0;
	m_nF8_ChanNum = 0;
	m_nF9_ChanNum = 0;
	m_nF10_ChanNum = 0;
	m_nF11_ChanNum = 0;
	m_nF12_ChanNum = 0;
	m_nF13_ChanNum = 0;
	m_nF14_ChanNum = 0;
	m_nF15_ChanNum = 0;

	//{{AFX_DATA_INIT(CProto25View)
	m_bF0_Disable = FALSE;
	m_bF1_Disable = FALSE;
	m_bF2_Disable = FALSE;
	m_bF3_Disable = FALSE;
	m_bF4_Disable = FALSE;
	m_bF5_Disable = FALSE;
	m_bF6_Disable = FALSE;
	m_bF7_Disable = FALSE;
	m_bF8_Disable = FALSE;
	m_bF9_Disable = FALSE;
	m_bF10_Disable = FALSE;
	m_bF11_Disable = FALSE;
	m_bF12_Disable = FALSE;
	m_bF13_Disable = FALSE;
	m_bF14_Disable = FALSE;
	m_bF15_Disable = FALSE;
	m_dF0_ScanFreq = 929.00625;
	m_dF1_ScanFreq = 929.00625;
	m_dF2_ScanFreq = 929.00625;
	m_dF3_ScanFreq = 929.00625;
	m_dF4_ScanFreq = 929.00625;
	m_dF5_ScanFreq = 929.00625;
	m_dF6_ScanFreq = 929.00625;
	m_dF7_ScanFreq = 929.00625;
	m_dF8_ScanFreq = 929.00625;
	m_dF9_ScanFreq = 929.00625;
	m_dF10_ScanFreq = 929.00625;
	m_dF11_ScanFreq = 929.00625;
	m_dF12_ScanFreq = 929.00625;
	m_dF13_ScanFreq = 929.00625;
	m_dF14_ScanFreq = 929.00625;
	m_dF15_ScanFreq = 929.00625;
	m_nF0_ScanDelay = 0;
	m_nF1_ScanDelay = 0;
	m_nF2_ScanDelay = 0;
	m_nF3_ScanDelay = 0;
	m_nF4_ScanDelay = 0;
	m_nF5_ScanDelay = 0;
	m_nF6_ScanDelay = 0;
	m_nF7_ScanDelay = 0;
	m_nF8_ScanDelay = 0;
	m_nF9_ScanDelay = 0;
	m_nF10_ScanDelay = 0;
	m_nF11_ScanDelay = 0;
	m_nF12_ScanDelay = 0;
	m_nF13_ScanDelay = 0;
	m_nF14_ScanDelay = 0;
	m_nF15_ScanDelay = 0;
	//}}AFX_DATA_INIT
}

CProto25View::~CProto25View()
{
}


void CProto25View::DoDataExchange(CDataExchange* pDX)
{
	CPpsFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProto25View)
	DDX_Check(pDX, IDC_DISABLE_F0, m_bF0_Disable);
	DDX_Check(pDX, IDC_DISABLE_F1, m_bF1_Disable);
	DDX_Check(pDX, IDC_DISABLE_F2, m_bF2_Disable);
	DDX_Check(pDX, IDC_DISABLE_F3, m_bF3_Disable);
	DDX_Check(pDX, IDC_DISABLE_F4, m_bF4_Disable);
	DDX_Check(pDX, IDC_DISABLE_F5, m_bF5_Disable);
	DDX_Check(pDX, IDC_DISABLE_F6, m_bF6_Disable);
	DDX_Check(pDX, IDC_DISABLE_F7, m_bF7_Disable);
	DDX_Check(pDX, IDC_DISABLE_F8, m_bF8_Disable);
	DDX_Check(pDX, IDC_DISABLE_F9, m_bF9_Disable);
	DDX_Check(pDX, IDC_DISABLE_F10, m_bF10_Disable);
	DDX_Check(pDX, IDC_DISABLE_F11, m_bF11_Disable);
	DDX_Check(pDX, IDC_DISABLE_F12, m_bF12_Disable);
	DDX_Check(pDX, IDC_DISABLE_F13, m_bF13_Disable);
	DDX_Check(pDX, IDC_DISABLE_F14, m_bF14_Disable);
	DDX_Check(pDX, IDC_DISABLE_F15, m_bF15_Disable);
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
	DDX_Text(pDX, IDC_SCAN_F8, m_dF8_ScanFreq);
	DDV_MinMaxDouble(pDX, m_dF8_ScanFreq, 929.00625, 941.80000);
	DDX_Text(pDX, IDC_SCAN_F9, m_dF9_ScanFreq);
	DDV_MinMaxDouble(pDX, m_dF9_ScanFreq, 929.00625, 941.80000);
	DDX_Text(pDX, IDC_SCAN_F10, m_dF10_ScanFreq);
	DDV_MinMaxDouble(pDX, m_dF10_ScanFreq, 929.00625, 941.80000);
	DDX_Text(pDX, IDC_SCAN_F11, m_dF11_ScanFreq);
	DDV_MinMaxDouble(pDX, m_dF11_ScanFreq, 929.00625, 941.80000);
	DDX_Text(pDX, IDC_SCAN_F12, m_dF12_ScanFreq);
	DDV_MinMaxDouble(pDX, m_dF12_ScanFreq, 929.00625, 941.80000);
	DDX_Text(pDX, IDC_SCAN_F13, m_dF13_ScanFreq);
	DDV_MinMaxDouble(pDX, m_dF13_ScanFreq, 929.00625, 941.80000);
	DDX_Text(pDX, IDC_SCAN_F14, m_dF14_ScanFreq);
	DDV_MinMaxDouble(pDX, m_dF14_ScanFreq, 929.00625, 941.80000);
	DDX_Text(pDX, IDC_SCAN_F15, m_dF15_ScanFreq);
	DDV_MinMaxDouble(pDX, m_dF15_ScanFreq, 929.00625, 941.80000);
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
	DDX_Text(pDX, IDC_DELAY_F8, m_nF8_ScanDelay);
	DDV_MinMaxInt(pDX, m_nF8_ScanDelay, 0, 7);
	DDX_Text(pDX, IDC_DELAY_F9, m_nF9_ScanDelay);
	DDV_MinMaxInt(pDX, m_nF9_ScanDelay, 0, 7);
	DDX_Text(pDX, IDC_DELAY_F10, m_nF10_ScanDelay);
	DDV_MinMaxInt(pDX, m_nF10_ScanDelay, 0, 7);
	DDX_Text(pDX, IDC_DELAY_F11, m_nF11_ScanDelay);
	DDV_MinMaxInt(pDX, m_nF11_ScanDelay, 0, 7);
	DDX_Text(pDX, IDC_DELAY_F12, m_nF12_ScanDelay);
	DDV_MinMaxInt(pDX, m_nF12_ScanDelay, 0, 7);
	DDX_Text(pDX, IDC_DELAY_F13, m_nF13_ScanDelay);
	DDV_MinMaxInt(pDX, m_nF13_ScanDelay, 0, 7);
	DDX_Text(pDX, IDC_DELAY_F14, m_nF14_ScanDelay);
	DDV_MinMaxInt(pDX, m_nF14_ScanDelay, 0, 7);
	DDX_Text(pDX, IDC_DELAY_F15, m_nF15_ScanDelay);
	DDV_MinMaxInt(pDX, m_nF15_ScanDelay, 0, 7);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProto25View, CPpsFormView)
	//{{AFX_MSG_MAP(CProto25View)
	ON_BN_CLICKED(IDC_PRO_DEFAULTS_BTN, OnAcceptDefaultsBtn)
	ON_BN_CLICKED(IDC_PRO_UNDO_BTN, OnUndoBtn)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SPIN_BUTTON, OnWmSpinButton)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CProto25View diagnostics

#ifdef _DEBUG
void CProto25View::AssertValid() const
{
	CPpsFormView::AssertValid();
}

void CProto25View::Dump(CDumpContext& dc) const
{
	CPpsFormView::Dump(dc);
}
#endif //_DEBUG





////////////////////////////////////////////////////////////////////////////
//	OnInitialUpdate()
//  
//  Update DDX variables from document and resize frame around view.
//
void CProto25View::OnInitialUpdate(void)
{
  TRACE("CProto25View::OnInitialUpdate : CProto25View::IDD=%X\n", CProto25View::IDD);

  // subclass the edit controls in order to force numeric only input.
  m_F0_FreqEdit.SubclassDlgItem(IDC_SCAN_F0, this);
  m_F1_FreqEdit.SubclassDlgItem(IDC_SCAN_F1, this);
  m_F2_FreqEdit.SubclassDlgItem(IDC_SCAN_F2, this);
  m_F3_FreqEdit.SubclassDlgItem(IDC_SCAN_F3, this);
  m_F4_FreqEdit.SubclassDlgItem(IDC_SCAN_F4, this);
  m_F5_FreqEdit.SubclassDlgItem(IDC_SCAN_F5, this);
  m_F6_FreqEdit.SubclassDlgItem(IDC_SCAN_F6, this);
  m_F7_FreqEdit.SubclassDlgItem(IDC_SCAN_F7, this);
  m_F8_FreqEdit.SubclassDlgItem(IDC_SCAN_F8, this);
  m_F9_FreqEdit.SubclassDlgItem(IDC_SCAN_F9, this);
  m_F10_FreqEdit.SubclassDlgItem(IDC_SCAN_F10, this);
  m_F11_FreqEdit.SubclassDlgItem(IDC_SCAN_F11, this);
  m_F12_FreqEdit.SubclassDlgItem(IDC_SCAN_F12, this);
  m_F13_FreqEdit.SubclassDlgItem(IDC_SCAN_F13, this);
  m_F14_FreqEdit.SubclassDlgItem(IDC_SCAN_F14, this);
  m_F15_FreqEdit.SubclassDlgItem(IDC_SCAN_F15, this);
  
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
  //
  m_F8_ScanDelayEdit.SubclassDlgItem(IDC_DELAY_F8, this);
  m_F8_ScanDelayEdit.LimitText(1);
  //
  m_F9_ScanDelayEdit.SubclassDlgItem(IDC_DELAY_F9, this);
  m_F9_ScanDelayEdit.LimitText(1);
  //
  m_F10_ScanDelayEdit.SubclassDlgItem(IDC_DELAY_F10, this);
  m_F10_ScanDelayEdit.LimitText(1);
  //
  m_F11_ScanDelayEdit.SubclassDlgItem(IDC_DELAY_F11, this);
  m_F11_ScanDelayEdit.LimitText(1);
  //
  m_F12_ScanDelayEdit.SubclassDlgItem(IDC_DELAY_F12, this);
  m_F12_ScanDelayEdit.LimitText(1);
  //
  m_F13_ScanDelayEdit.SubclassDlgItem(IDC_DELAY_F13, this);
  m_F13_ScanDelayEdit.LimitText(1);
  //
  m_F14_ScanDelayEdit.SubclassDlgItem(IDC_DELAY_F14, this);
  m_F14_ScanDelayEdit.LimitText(1);
  //
  m_F15_ScanDelayEdit.SubclassDlgItem(IDC_DELAY_F15, this);
  m_F15_ScanDelayEdit.LimitText(1);

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

  // ** Row #8
  VERIFY(m_F8_ChNumUpBtn.AutoLoad(IDC_CHNUM_UP_F8, this));
  VERIFY(m_F8_ChNumDnBtn.AutoLoad(IDC_CHNUM_DN_F8, this));
  VERIFY(m_F8_DelayUpBtn.AutoLoad(IDC_DELAY_UP_F8, this));
  VERIFY(m_F8_DelayDnBtn.AutoLoad(IDC_DELAY_DN_F8, this));

  // ** Row #9
  VERIFY(m_F9_ChNumUpBtn.AutoLoad(IDC_CHNUM_UP_F9, this));
  VERIFY(m_F9_ChNumDnBtn.AutoLoad(IDC_CHNUM_DN_F9, this));
  VERIFY(m_F9_DelayUpBtn.AutoLoad(IDC_DELAY_UP_F9, this));
  VERIFY(m_F9_DelayDnBtn.AutoLoad(IDC_DELAY_DN_F9, this));

  // ** Row #10
  VERIFY(m_F10_ChNumUpBtn.AutoLoad(IDC_CHNUM_UP_F10, this));
  VERIFY(m_F10_ChNumDnBtn.AutoLoad(IDC_CHNUM_DN_F10, this));
  VERIFY(m_F10_DelayUpBtn.AutoLoad(IDC_DELAY_UP_F10, this));
  VERIFY(m_F10_DelayDnBtn.AutoLoad(IDC_DELAY_DN_F10, this));

  // ** Row #11
  VERIFY(m_F11_ChNumUpBtn.AutoLoad(IDC_CHNUM_UP_F11, this));
  VERIFY(m_F11_ChNumDnBtn.AutoLoad(IDC_CHNUM_DN_F11, this));
  VERIFY(m_F11_DelayUpBtn.AutoLoad(IDC_DELAY_UP_F11, this));
  VERIFY(m_F11_DelayDnBtn.AutoLoad(IDC_DELAY_DN_F11, this));

  // ** Row #12
  VERIFY(m_F12_ChNumUpBtn.AutoLoad(IDC_CHNUM_UP_F12, this));
  VERIFY(m_F12_ChNumDnBtn.AutoLoad(IDC_CHNUM_DN_F12, this));
  VERIFY(m_F12_DelayUpBtn.AutoLoad(IDC_DELAY_UP_F12, this));
  VERIFY(m_F12_DelayDnBtn.AutoLoad(IDC_DELAY_DN_F12, this));

  // ** Row #13
  VERIFY(m_F13_ChNumUpBtn.AutoLoad(IDC_CHNUM_UP_F13, this));
  VERIFY(m_F13_ChNumDnBtn.AutoLoad(IDC_CHNUM_DN_F13, this));
  VERIFY(m_F13_DelayUpBtn.AutoLoad(IDC_DELAY_UP_F13, this));
  VERIFY(m_F13_DelayDnBtn.AutoLoad(IDC_DELAY_DN_F13, this));

  // ** Row #14
  VERIFY(m_F14_ChNumUpBtn.AutoLoad(IDC_CHNUM_UP_F14, this));
  VERIFY(m_F14_ChNumDnBtn.AutoLoad(IDC_CHNUM_DN_F14, this));
  VERIFY(m_F14_DelayUpBtn.AutoLoad(IDC_DELAY_UP_F14, this));
  VERIFY(m_F14_DelayDnBtn.AutoLoad(IDC_DELAY_DN_F14, this));

  // ** Row #15
  VERIFY(m_F15_ChNumUpBtn.AutoLoad(IDC_CHNUM_UP_F15, this));
  VERIFY(m_F15_ChNumDnBtn.AutoLoad(IDC_CHNUM_DN_F15, this));
  VERIFY(m_F15_DelayUpBtn.AutoLoad(IDC_DELAY_UP_F15, this));
  VERIFY(m_F15_DelayDnBtn.AutoLoad(IDC_DELAY_DN_F15, this));

  CPpsFormView::OnInitialUpdate();
  return;
}




/////////////////////////////////////////////////////////////////////////////
//	OnUpdate()
//
void CProto25View::OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint)
{
  TRACE("\nCProto25View::OnUpdate : nViewID=%X  LHintHI=%X  lHintLO=%X\n", 
  		CProto25View::IDD,  HIWORD(lHint), LOWORD(lHint));

  if (HIWORD(lHint))        		// user switched views, now active
    ResizeParentToFit(FALSE); 
    
  SetFromDoc();    
  CPpsFormView::OnUpdate(pSender, lHint, pHint);
}	


////////////////////////////////////////////////////////////////////////////
//  SetFromDoc()
//
void CProto25View::SetFromDoc(void)
{
  TRACE("\nCProto25View::SetFromDoc\n");
  
  // ** Row0  
  CP_CtrlChanNumber.nSetFromDoc((UINT *)&m_nF0_ChanNum);
  CP_CtrlChanScanDelay.nSetFromDoc((UINT *)&m_nF0_ScanDelay);
  CP_CtrlChanDisable.bSetFromDoc(&m_bF0_Disable);
  m_dF0_ScanFreq = CReflex25Protocol::dCalcForwardChanFreq(m_nF0_ChanNum);
  
  // ** Row1 
  CP_CtrlChanNumber.nSetFromDoc((UINT *)&m_nF1_ChanNum, 2);
  CP_CtrlChanScanDelay.nSetFromDoc((UINT *)&m_nF1_ScanDelay, 2);
  CP_CtrlChanDisable.bSetFromDoc(&m_bF1_Disable, 2);
  m_dF1_ScanFreq = CReflex25Protocol::dCalcForwardChanFreq(m_nF1_ChanNum);
  
  // ** Row2
  CP_CtrlChanNumber.nSetFromDoc((UINT *)&m_nF2_ChanNum, 4);
  CP_CtrlChanScanDelay.nSetFromDoc((UINT *)&m_nF2_ScanDelay, 4);
  CP_CtrlChanDisable.bSetFromDoc(&m_bF2_Disable, 4);
  m_dF2_ScanFreq = CReflex25Protocol::dCalcForwardChanFreq(m_nF2_ChanNum);
  
  // ** Row3
  CP_CtrlChanNumber.nSetFromDoc((UINT *)&m_nF3_ChanNum, 6);
  CP_CtrlChanScanDelay.nSetFromDoc((UINT *)&m_nF3_ScanDelay, 6);
  CP_CtrlChanDisable.bSetFromDoc(&m_bF3_Disable, 6);
  m_dF3_ScanFreq = CReflex25Protocol::dCalcForwardChanFreq(m_nF3_ChanNum);
  
  // ** Row4
  CP_CtrlChanNumber.nSetFromDoc((UINT *)&m_nF4_ChanNum, 8);
  CP_CtrlChanScanDelay.nSetFromDoc((UINT *)&m_nF4_ScanDelay, 8);
  CP_CtrlChanDisable.bSetFromDoc(&m_bF4_Disable, 8);
  m_dF4_ScanFreq = CReflex25Protocol::dCalcForwardChanFreq(m_nF4_ChanNum);
  
  // ** Row5
  CP_CtrlChanNumber.nSetFromDoc((UINT *)&m_nF5_ChanNum, 10);
  CP_CtrlChanScanDelay.nSetFromDoc((UINT *)&m_nF5_ScanDelay, 10);
  CP_CtrlChanDisable.bSetFromDoc(&m_bF5_Disable, 10);
  m_dF5_ScanFreq = CReflex25Protocol::dCalcForwardChanFreq(m_nF5_ChanNum);
  
  // ** Row6
  CP_CtrlChanNumber.nSetFromDoc((UINT *)&m_nF6_ChanNum, 12);
  CP_CtrlChanScanDelay.nSetFromDoc((UINT *)&m_nF6_ScanDelay, 12);
  CP_CtrlChanDisable.bSetFromDoc(&m_bF6_Disable, 12);
  m_dF6_ScanFreq = CReflex25Protocol::dCalcForwardChanFreq(m_nF6_ChanNum);
  
  // ** Row7
  CP_CtrlChanNumber.nSetFromDoc((UINT *)&m_nF7_ChanNum, 14);
  CP_CtrlChanScanDelay.nSetFromDoc((UINT *)&m_nF7_ScanDelay, 14);
  CP_CtrlChanDisable.bSetFromDoc(&m_bF7_Disable, 14);
  m_dF7_ScanFreq = CReflex25Protocol::dCalcForwardChanFreq(m_nF7_ChanNum);

  // ** Row8
  CP_CtrlChanNumber.nSetFromDoc((UINT *)&m_nF8_ChanNum, 16);
  CP_CtrlChanScanDelay.nSetFromDoc((UINT *)&m_nF8_ScanDelay, 16);
  CP_CtrlChanDisable.bSetFromDoc(&m_bF8_Disable, 16);
  m_dF8_ScanFreq = CReflex25Protocol::dCalcForwardChanFreq(m_nF8_ChanNum);

  // ** Row9
  CP_CtrlChanNumber.nSetFromDoc((UINT *)&m_nF9_ChanNum, 18);
  CP_CtrlChanScanDelay.nSetFromDoc((UINT *)&m_nF9_ScanDelay, 18);
  CP_CtrlChanDisable.bSetFromDoc(&m_bF9_Disable, 18);
  m_dF9_ScanFreq = CReflex25Protocol::dCalcForwardChanFreq(m_nF9_ChanNum);

  // ** Row10
  CP_CtrlChanNumber.nSetFromDoc((UINT *)&m_nF10_ChanNum, 20);
  CP_CtrlChanScanDelay.nSetFromDoc((UINT *)&m_nF10_ScanDelay, 20);
  CP_CtrlChanDisable.bSetFromDoc(&m_bF10_Disable, 20);
  m_dF10_ScanFreq = CReflex25Protocol::dCalcForwardChanFreq(m_nF10_ChanNum);

  // ** Row11
  CP_CtrlChanNumber.nSetFromDoc((UINT *)&m_nF11_ChanNum, 22);
  CP_CtrlChanScanDelay.nSetFromDoc((UINT *)&m_nF11_ScanDelay, 22);
  CP_CtrlChanDisable.bSetFromDoc(&m_bF11_Disable, 22);
  m_dF11_ScanFreq = CReflex25Protocol::dCalcForwardChanFreq(m_nF11_ChanNum);

  // ** Row12
  CP_CtrlChanNumber.nSetFromDoc((UINT *)&m_nF12_ChanNum, 24);
  CP_CtrlChanScanDelay.nSetFromDoc((UINT *)&m_nF12_ScanDelay, 24);
  CP_CtrlChanDisable.bSetFromDoc(&m_bF12_Disable, 24);
  m_dF12_ScanFreq = CReflex25Protocol::dCalcForwardChanFreq(m_nF12_ChanNum);

  // ** Row13
  CP_CtrlChanNumber.nSetFromDoc((UINT *)&m_nF13_ChanNum, 26);
  CP_CtrlChanScanDelay.nSetFromDoc((UINT *)&m_nF13_ScanDelay, 26);
  CP_CtrlChanDisable.bSetFromDoc(&m_bF13_Disable, 26);
  m_dF13_ScanFreq = CReflex25Protocol::dCalcForwardChanFreq(m_nF13_ChanNum);

  // ** Row14
  CP_CtrlChanNumber.nSetFromDoc((UINT *)&m_nF14_ChanNum, 28);
  CP_CtrlChanScanDelay.nSetFromDoc((UINT *)&m_nF14_ScanDelay, 28);
  CP_CtrlChanDisable.bSetFromDoc(&m_bF14_Disable, 28);
  m_dF14_ScanFreq = CReflex25Protocol::dCalcForwardChanFreq(m_nF14_ChanNum);

  // ** Row15
  CP_CtrlChanNumber.nSetFromDoc((UINT *)&m_nF15_ChanNum, 30);
  CP_CtrlChanScanDelay.nSetFromDoc((UINT *)&m_nF15_ScanDelay, 30);
  CP_CtrlChanDisable.bSetFromDoc(&m_bF15_Disable, 30);
  m_dF15_ScanFreq = CReflex25Protocol::dCalcForwardChanFreq(m_nF15_ChanNum);

  UpdateData(FALSE);	// set controls data
}



////////////////////////////////////////////////////////////////////////////
//	SetToDoc()
// 
//
BOOL CProto25View::SetToDoc(void)
{
  TRACE("\nCProto25View::SetToDoc\n");
  CPpsDoc		*pDoc = GetDocument(); 
  
  if (!(UpdateData(TRUE)))  // get controls data
    return FALSE;

  m_nF0_ChanNum = CReflex25Protocol::nCalcForwardChanNum(m_dF0_ScanFreq, NO_ROUNDING);
  m_nF1_ChanNum = CReflex25Protocol::nCalcForwardChanNum(m_dF1_ScanFreq, NO_ROUNDING);
  m_nF2_ChanNum = CReflex25Protocol::nCalcForwardChanNum(m_dF2_ScanFreq, NO_ROUNDING);
  m_nF3_ChanNum = CReflex25Protocol::nCalcForwardChanNum(m_dF3_ScanFreq, NO_ROUNDING);
  m_nF4_ChanNum = CReflex25Protocol::nCalcForwardChanNum(m_dF4_ScanFreq, NO_ROUNDING);
  m_nF5_ChanNum = CReflex25Protocol::nCalcForwardChanNum(m_dF5_ScanFreq, NO_ROUNDING);
  m_nF6_ChanNum = CReflex25Protocol::nCalcForwardChanNum(m_dF6_ScanFreq, NO_ROUNDING);
  m_nF7_ChanNum = CReflex25Protocol::nCalcForwardChanNum(m_dF7_ScanFreq, NO_ROUNDING);
  m_nF8_ChanNum = CReflex25Protocol::nCalcForwardChanNum(m_dF8_ScanFreq, NO_ROUNDING);
  m_nF9_ChanNum = CReflex25Protocol::nCalcForwardChanNum(m_dF9_ScanFreq, NO_ROUNDING);
  m_nF10_ChanNum = CReflex25Protocol::nCalcForwardChanNum(m_dF10_ScanFreq, NO_ROUNDING);
  m_nF11_ChanNum = CReflex25Protocol::nCalcForwardChanNum(m_dF11_ScanFreq, NO_ROUNDING);
  m_nF12_ChanNum = CReflex25Protocol::nCalcForwardChanNum(m_dF12_ScanFreq, NO_ROUNDING);
  m_nF13_ChanNum = CReflex25Protocol::nCalcForwardChanNum(m_dF13_ScanFreq, NO_ROUNDING);
  m_nF14_ChanNum = CReflex25Protocol::nCalcForwardChanNum(m_dF14_ScanFreq, NO_ROUNDING);
  m_nF15_ChanNum = CReflex25Protocol::nCalcForwardChanNum(m_dF15_ScanFreq, NO_ROUNDING);
   
  
  // *** The device requires that enabled entries
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
  if (!m_bF8_Disable) // ** Row8
    SaveControlChannel(m_nF8_ChanNum, m_bF8_Disable, m_nF8_ScanDelay, &index);
  if (!m_bF9_Disable) // ** Row9
    SaveControlChannel(m_nF9_ChanNum, m_bF9_Disable, m_nF9_ScanDelay, &index);
  if (!m_bF10_Disable) // ** Row10 
    SaveControlChannel(m_nF10_ChanNum, m_bF10_Disable, m_nF10_ScanDelay, &index);
  if (!m_bF11_Disable) // ** Row11
    SaveControlChannel(m_nF11_ChanNum, m_bF11_Disable, m_nF11_ScanDelay, &index);
  if (!m_bF12_Disable) // ** Row12 
    SaveControlChannel(m_nF12_ChanNum, m_bF12_Disable, m_nF12_ScanDelay, &index);
  if (!m_bF13_Disable) // ** Row13 
    SaveControlChannel(m_nF13_ChanNum, m_bF13_Disable, m_nF13_ScanDelay, &index);
  if (!m_bF14_Disable) // ** Row14 
    SaveControlChannel(m_nF14_ChanNum, m_bF14_Disable, m_nF14_ScanDelay, &index);
  if (!m_bF15_Disable) // ** Row15 
    SaveControlChannel(m_nF15_ChanNum, m_bF15_Disable, m_nF15_ScanDelay, &index);  

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
  if (m_bF8_Disable) // ** Row8
    SaveControlChannel(m_nF8_ChanNum, m_bF8_Disable, m_nF8_ScanDelay, &index);
  if (m_bF9_Disable) // ** Row9
    SaveControlChannel(m_nF9_ChanNum, m_bF9_Disable, m_nF9_ScanDelay, &index);
  if (m_bF10_Disable) // ** Row10 
    SaveControlChannel(m_nF10_ChanNum, m_bF10_Disable, m_nF10_ScanDelay, &index);
  if (m_bF11_Disable) // ** Row11
    SaveControlChannel(m_nF11_ChanNum, m_bF11_Disable, m_nF11_ScanDelay, &index);
  if (m_bF12_Disable) // ** Row12 
    SaveControlChannel(m_nF12_ChanNum, m_bF12_Disable, m_nF12_ScanDelay, &index);
  if (m_bF13_Disable) // ** Row13 
    SaveControlChannel(m_nF13_ChanNum, m_bF13_Disable, m_nF13_ScanDelay, &index);
  if (m_bF14_Disable) // ** Row14 
    SaveControlChannel(m_nF14_ChanNum, m_bF14_Disable, m_nF14_ScanDelay, &index);
  if (m_bF15_Disable) // ** Row15 
    SaveControlChannel(m_nF15_ChanNum, m_bF15_Disable, m_nF15_ScanDelay, &index);  

  return(TRUE);
}
                                                               

void CProto25View::SaveControlChannel(int nChannel, BOOL bDisabled, int nScanTime, int *nIndex)
{
  CP_CtrlChanNumber.nSetToDoc((UINT)nChannel, *nIndex);
  CP_CtrlChanScanDelay.nSetToDoc((UINT)nScanTime, *nIndex);
  CP_CtrlChanDisable.bSetToDoc(bDisabled, *nIndex);
  *nIndex += 2;  
  return;
}

 

/////////////////////////////////////////////////////////////////////////////
// CProto25View message handlers



////////////////////////////////////////////////////////////////////////
//  OnAcceptDefaultsBtn()
//
//  This function initiates a move of data from select fields of the
//  CPpsDoc.m_byDefaultCodePlugArray to the CPpsDoc.m_byCodePlugArray.
//  The m_byDefaultCodePlugArray is loaded from file 'default.s19' upon
//  application execution.  If the file has not been found, then no 
//  default values are available.  
//
void CProto25View::OnAcceptDefaultsBtn()
{
  TRACE("\nCProto25View::OnAcceptDefaultsBtn\n"); 
  CPpsDoc	*pDoc = GetDocument(); 
  
  if (pDoc->SetFieldFromDefault(CP_Control_Channel.Offset(), CP_Control_Channel.Length()))
    return;

  SetFromDoc();   // update this view
}


////////////////////////////////////////////////////////////////////////
//  OnArchived()
//
//  This function initiates a move of data from select fields of the
//  CPpsDoc.m_byArchivedCodePlugArray to the CPpsDoc.m_byCodePlugArray.
//
void CProto25View::OnArchived(void)
{
  CPpsDoc	*pDoc = GetDocument(); 
   
  if (pDoc->SetFieldFromArchived(CP_Control_Channel.Offset(), CP_Control_Channel.Length()))
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
void CProto25View::OnUndoBtn()
{
  TRACE("\nCProto25View::OnUndoBtn\n"); 
  CPpsDoc	*pDoc = GetDocument(); 

  if (pDoc->SetFieldFromOriginal(CP_Control_Channel.Offset(), CP_Control_Channel.Length()))
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
afx_msg LONG CProto25View::OnWmSpinButton(UINT wParam, LONG lParam)
{
  TRACE("\nCProto25View::OnWmSpinButton : nBtnID=%04X\n", wParam);
  
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
    case IDC_CHNUM_UP_F8 : 
        OnChannelNumberIncrement(&m_nF8_ChanNum, &m_dF8_ScanFreq, nAcceleration);
        break; 
    case IDC_CHNUM_DN_F8 : 
        OnChannelNumberDecrement(&m_nF8_ChanNum, &m_dF8_ScanFreq, nAcceleration); 
        break;
    case IDC_CHNUM_UP_F9 :
        OnChannelNumberIncrement(&m_nF9_ChanNum, &m_dF9_ScanFreq, nAcceleration);
        break; 
    case IDC_CHNUM_DN_F9 : 
        OnChannelNumberDecrement(&m_nF9_ChanNum, &m_dF9_ScanFreq, nAcceleration); 
        break;
    case IDC_CHNUM_UP_F10 :
        OnChannelNumberIncrement(&m_nF10_ChanNum, &m_dF10_ScanFreq, nAcceleration);
        break; 
    case IDC_CHNUM_DN_F10 : 
        OnChannelNumberDecrement(&m_nF10_ChanNum, &m_dF10_ScanFreq, nAcceleration); 
        break;
    case IDC_CHNUM_UP_F11 :
        OnChannelNumberIncrement(&m_nF11_ChanNum, &m_dF11_ScanFreq, nAcceleration);
        break; 
    case IDC_CHNUM_DN_F11 : 
        OnChannelNumberDecrement(&m_nF11_ChanNum, &m_dF11_ScanFreq, nAcceleration); 
        break;
    case IDC_CHNUM_UP_F12 :
        OnChannelNumberIncrement(&m_nF12_ChanNum, &m_dF12_ScanFreq, nAcceleration);
        break; 
    case IDC_CHNUM_DN_F12 : 
        OnChannelNumberDecrement(&m_nF12_ChanNum, &m_dF12_ScanFreq, nAcceleration); 
        break;
    case IDC_CHNUM_UP_F13 :
        OnChannelNumberIncrement(&m_nF13_ChanNum, &m_dF13_ScanFreq, nAcceleration);
        break; 
    case IDC_CHNUM_DN_F13 : 
        OnChannelNumberDecrement(&m_nF13_ChanNum, &m_dF13_ScanFreq, nAcceleration); 
        break;
    case IDC_CHNUM_UP_F14 :
        OnChannelNumberIncrement(&m_nF14_ChanNum, &m_dF14_ScanFreq, nAcceleration);
        break; 
    case IDC_CHNUM_DN_F14 : 
        OnChannelNumberDecrement(&m_nF14_ChanNum, &m_dF14_ScanFreq, nAcceleration); 
        break;
    case IDC_CHNUM_UP_F15 :
        OnChannelNumberIncrement(&m_nF15_ChanNum, &m_dF15_ScanFreq, nAcceleration);
        break; 
    case IDC_CHNUM_DN_F15 : 
        OnChannelNumberDecrement(&m_nF15_ChanNum, &m_dF15_ScanFreq, nAcceleration); 
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
    case IDC_DELAY_UP_F8 :
        OnScanDelayIncrement(&m_nF8_ScanDelay, 1);
        break;
    case IDC_DELAY_DN_F8 :
        OnScanDelayDecrement(&m_nF8_ScanDelay, 1);
        break;
    case IDC_DELAY_UP_F9 :
        OnScanDelayIncrement(&m_nF9_ScanDelay, 1);
        break;
    case IDC_DELAY_DN_F9 :
        OnScanDelayDecrement(&m_nF9_ScanDelay, 1);
        break;
    case IDC_DELAY_UP_F10 :
        OnScanDelayIncrement(&m_nF10_ScanDelay, 1);
        break;
    case IDC_DELAY_DN_F10 :
        OnScanDelayDecrement(&m_nF10_ScanDelay, 1);
        break;
    case IDC_DELAY_UP_F11 :
        OnScanDelayIncrement(&m_nF11_ScanDelay, 1);
        break;
    case IDC_DELAY_DN_F11 :
        OnScanDelayDecrement(&m_nF11_ScanDelay, 1);
        break;
    case IDC_DELAY_UP_F12 :
        OnScanDelayIncrement(&m_nF12_ScanDelay, 1);
        break;
    case IDC_DELAY_DN_F12 :
        OnScanDelayDecrement(&m_nF12_ScanDelay, 1);
        break;
    case IDC_DELAY_UP_F13 :
        OnScanDelayIncrement(&m_nF13_ScanDelay, 1);
        break;
    case IDC_DELAY_DN_F13 :
        OnScanDelayDecrement(&m_nF13_ScanDelay, 1);
        break;
    case IDC_DELAY_UP_F14 :
        OnScanDelayIncrement(&m_nF14_ScanDelay, 1);
        break;
    case IDC_DELAY_DN_F14 :
        OnScanDelayDecrement(&m_nF14_ScanDelay, 1);
        break;
    case IDC_DELAY_UP_F15 :
        OnScanDelayIncrement(&m_nF15_ScanDelay, 1);
        break;
    case IDC_DELAY_DN_F15 :
        OnScanDelayDecrement(&m_nF15_ScanDelay, 1);
        break; 
    default:
      break;
  }
  
  UpdateData(FALSE);	// set controls data 
  return(0L);
}


/////////////////////
void CProto25View::OnChannelNumberIncrement(int *pnChanNum, 
											double *pdFrequency,
											int nOffsetBy)
{ 
  *pnChanNum = CReflex25Protocol::nCalcForwardChanNum(*pdFrequency, ROUND_UP);   
  
  *pnChanNum += nOffsetBy;  
  if (((*pnChanNum) % 2) == 0)    // allow only odd channel numbers
    *pnChanNum += 1;
  
  if ((*pnChanNum) > MAX_FWD_CHAN)
    *pnChanNum = MAX_FWD_CHAN;

  *pdFrequency = CReflex25Protocol::dCalcForwardChanFreq(*pnChanNum);  
  return; 	
}


//////////////////////
void CProto25View::OnChannelNumberDecrement(int *pnChanNum, 
										    double *pdFrequency,
											int nOffsetBy)
{
  *pnChanNum = CReflex25Protocol::nCalcForwardChanNum(*pdFrequency, ROUND_DN);
  
  *pnChanNum -= nOffsetBy;  
  if (((*pnChanNum) % 2) == 0)    // allow only odd channel numbers
    *pnChanNum -= 1;

  if ((*pnChanNum) < MIN_FWD_CHAN)
    *pnChanNum = MIN_FWD_CHAN;

  *pdFrequency = CReflex25Protocol::dCalcForwardChanFreq(*pnChanNum);
  return; 	
}


 
////////////////////
void CProto25View::OnScanDelayIncrement(int *pnScanDelay, 
											int nOffsetBy)
{
  if ((*pnScanDelay + nOffsetBy) > 7)
    *pnScanDelay = 7;
  else 
    *pnScanDelay += nOffsetBy;
  return;
}


void CProto25View::OnScanDelayDecrement(int *pnScanDelay,
											int nOffsetBy)
{
  if ((*pnScanDelay - nOffsetBy) < 0)
    *pnScanDelay = 0;
  else
    *pnScanDelay -= nOffsetBy;
  return;
}



int CProto25View::OnPrintHelper(CStringArray &rStringArray, UINT nPageNumber, int nAvailableLines)
{ 
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
     sprintf(szTemp, "        (8)\t %10lf, scan-delay=%d, disable=%d", m_dF8_ScanFreq, m_nF8_ScanDelay, m_bF8_Disable);
     rStringArray.Add(szTemp);
     sprintf(szTemp, "        (9)\t %10lf, scan-delay=%d, disable=%d", m_dF9_ScanFreq, m_nF9_ScanDelay, m_bF9_Disable);
     rStringArray.Add(szTemp);
     sprintf(szTemp, "        (10)\t %10lf, scan-delay=%d, disable=%d", m_dF10_ScanFreq, m_nF10_ScanDelay, m_bF10_Disable);
     rStringArray.Add(szTemp);
     sprintf(szTemp, "        (11)\t %10lf, scan-delay=%d, disable=%d", m_dF11_ScanFreq, m_nF11_ScanDelay, m_bF11_Disable);
     rStringArray.Add(szTemp);
     sprintf(szTemp, "        (12)\t %10lf, scan-delay=%d, disable=%d", m_dF12_ScanFreq, m_nF12_ScanDelay, m_bF12_Disable);
     rStringArray.Add(szTemp);
     sprintf(szTemp, "        (13)\t %10lf, scan-delay=%d, disable=%d", m_dF13_ScanFreq, m_nF13_ScanDelay, m_bF13_Disable);
     rStringArray.Add(szTemp);
     sprintf(szTemp, "        (14)\t %10lf, scan-delay=%d, disable=%d", m_dF14_ScanFreq, m_nF14_ScanDelay, m_bF14_Disable);
     rStringArray.Add(szTemp);
     sprintf(szTemp, "        (15)\t %10lf, scan-delay=%d, disable=%d", m_dF15_ScanFreq, m_nF15_ScanDelay, m_bF15_Disable);
     rStringArray.Add(szTemp);
   
   rStringArray.Add("");
   return(rStringArray.GetSize() - nNumLinesBegin);
}


