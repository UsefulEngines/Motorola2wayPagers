//*************************************************************
//  History:    Date       Author               Comment
//              1996       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
// proto50.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\pps.h"
#include "..\..\ppsdoc.h"
#include "..\..\ppsview.h" 
#include "..\..\cpager\codeplug.h" 
#include "proto50.h"
#include "math.h"		// for the 'ceil' function

extern CPpsApp    		theApp;

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif





/////////////////////////////////////////////////////////////////////////////
// CReflex50Protocol

IMPLEMENT_DYNCREATE(CReflex50Protocol, CPpsFormView)

CReflex50Protocol::CReflex50Protocol()
	: CPpsFormView(CReflex50Protocol::IDD)
{
	//{{AFX_DATA_INIT(CReflex50Protocol)
	m_dFwdChanFreq0 = 929.00625;
	m_byHomeLogicalNum = 1;
	m_byInfoFrame = 0;
	m_byInfoSvcCollapse = 0;
	m_byPersonalCollapse = 0;
	m_byPersonalAddressFrame = 0;
	m_dRevChanFreq0 = 901.00625;
	m_dRevChanFreq1 = 901.00625;
	m_dRevChanFreq2 = 901.00625;
	m_dRevChanFreq3 = 901.00625;
	m_dRevChanFreq4 = 901.00625;
	m_dRevChanFreq5 = 901.00625;
	m_dRevChanFreq6 = 901.00625;
	m_byAsyncSleepFrames = 0;
	m_bDisableRevCh0 = FALSE;
	m_bDisableRevCh1 = FALSE;
	m_bDisableRevCh2 = FALSE;
	m_bDisableRevCh3 = FALSE;
	m_bDisableRevCh4 = FALSE;
	m_bDisableRevCh5 = FALSE;
	m_bDisableRevCh6 = FALSE;
	m_bChannel_Id = FALSE;
	//}}AFX_DATA_INIT
}

CReflex50Protocol::~CReflex50Protocol()
{
}


static const char szEmptyString[] = "";
//
void CReflex50Protocol::DoRevChanDDX(CDataExchange *pDX, WORD wResId, 
									 double &rdFreq, BOOL bDisable, CEdit &rEdit)
{
   if (!bDisable)
   {
	 DDX_Text(pDX, wResId, rdFreq);
     DDV_MinMaxDouble(pDX, rdFreq, 901.00625, 901.99375);
   }
   else 
     rEdit.SetWindowText(szEmptyString);    // if disabled, show empty box
   return;
}


void CReflex50Protocol::DoDataExchange(CDataExchange* pDX)
{
	CPpsFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReflex50Protocol)
	DDX_Text(pDX, IDC_HOMECTRLCHAN_EDIT, m_byHomeLogicalNum);
	DDV_MinMaxByte(pDX, m_byHomeLogicalNum, 1, 12);
	DDX_Text(pDX, IDC_INFOFRAME_EDIT, m_byInfoFrame);
	DDV_MinMaxByte(pDX, m_byInfoFrame, 0, 127);
	DDX_Text(pDX, IDC_INFOCOLLAPSE_EDIT, m_byInfoSvcCollapse);
	DDV_MinMaxByte(pDX, m_byInfoSvcCollapse, 0, 7);
	DDX_Text(pDX, IDC_PERSFRAME_EDIT, m_byPersonalAddressFrame);
	DDV_MinMaxByte(pDX, m_byPersonalAddressFrame, 0, 127);
	DDX_Text(pDX, IDC_ASYNCSLEEP_EDIT, m_byAsyncSleepFrames);
	DDV_MinMaxByte(pDX, m_byAsyncSleepFrames, 0, 255);
	DDX_Check(pDX, IDC_DISABLE_REVCH0, m_bDisableRevCh0);
	DDX_Check(pDX, IDC_DISABLE_REVCH1, m_bDisableRevCh1);
	DDX_Check(pDX, IDC_DISABLE_REVCH2, m_bDisableRevCh2);
	DDX_Check(pDX, IDC_DISABLE_REVCH3, m_bDisableRevCh3);
	DDX_Check(pDX, IDC_DISABLE_REVCH4, m_bDisableRevCh4);
	DDX_Check(pDX, IDC_DISABLE_REVCH5, m_bDisableRevCh5);
	DDX_Check(pDX, IDC_DISABLE_REVCH6, m_bDisableRevCh6);
	DDX_Check(pDX, IDC_CHANNEL_ID_CHK, m_bChannel_Id);
	//}}AFX_DATA_MAP
   
   DoRevChanDDX(pDX, IDC_REVCHAN0_EDIT, m_dRevChanFreq0, m_bDisableRevCh0, RevChanFreq0Edit);
   DoRevChanDDX(pDX, IDC_REVCHAN1_EDIT, m_dRevChanFreq1, m_bDisableRevCh1, RevChanFreq1Edit);
   DoRevChanDDX(pDX, IDC_REVCHAN2_EDIT, m_dRevChanFreq2, m_bDisableRevCh2, RevChanFreq2Edit);
   DoRevChanDDX(pDX, IDC_REVCHAN3_EDIT, m_dRevChanFreq3, m_bDisableRevCh3, RevChanFreq3Edit);
   DoRevChanDDX(pDX, IDC_REVCHAN4_EDIT, m_dRevChanFreq4, m_bDisableRevCh4, RevChanFreq4Edit);
   DoRevChanDDX(pDX, IDC_REVCHAN5_EDIT, m_dRevChanFreq5, m_bDisableRevCh5, RevChanFreq5Edit);
   DoRevChanDDX(pDX, IDC_REVCHAN6_EDIT, m_dRevChanFreq6, m_bDisableRevCh6, RevChanFreq6Edit);

   BYTE  byDeviceId, byCPVersion;
   CP_Version.bySetFromDoc(&byCPVersion);
   CP_OTA_Protected_H.bySetFromDoc(&byDeviceId);

   DDX_Text(pDX, IDC_PERSCOLLAPSE_EDIT, m_byPersonalCollapse);
   // Note:  A DSP V1.01 bug with the Pagefinder, codeplug version 8, 
   // necessitates the restriction of the personal collapse to
   // range 0-6.  Feb 11, 1997. P. Pennington
   if ((byDeviceId == PAGEFINDER) && (byCPVersion == 0x08))
	  DDV_MinMaxByte(pDX, m_byPersonalCollapse, 0, 6);
   else 
	  DDV_MinMaxByte(pDX, m_byPersonalCollapse, 0, 7);

   DDX_Text(pDX, IDC_FWDCHAN_EDIT, m_dFwdChanFreq0);
   // Different calculations have been implemented over time.  
   // This if-else sequence assumes future device id's will 
   // increase from the PAGERFINDER device id.
   if ((byDeviceId < PAGEFINDER) && (byCPVersion <= 0x05))
   	 DDV_MinMaxDouble(pDX, m_dFwdChanFreq0, 929.00625, 941.8);
   else if ((byDeviceId <= PAGEFINDER) && (byCPVersion < 0x07))
   	 DDV_MinMaxDouble(pDX, m_dFwdChanFreq0, 929.00625, 949.485);
   else
   	 DDV_MinMaxDouble(pDX, m_dFwdChanFreq0, 929.00625, 969.965);
  
  return;
}



BEGIN_MESSAGE_MAP(CReflex50Protocol, CPpsFormView)
	//{{AFX_MSG_MAP(CReflex50Protocol)
	ON_BN_CLICKED(IDC_PROTO50_DEFAULTS_BTN, OnAcceptDefaultsBtn)
	ON_BN_CLICKED(IDC_PROTO50_UNDO_BTN, OnUndoBtn)
	ON_BN_CLICKED(IDC_DISABLE_REVCH0, OnDisableRevch0)
	ON_BN_CLICKED(IDC_DISABLE_REVCH1, OnDisableRevch1)
	ON_BN_CLICKED(IDC_DISABLE_REVCH2, OnDisableRevch2)
	ON_BN_CLICKED(IDC_DISABLE_REVCH3, OnDisableRevch3)
	ON_BN_CLICKED(IDC_DISABLE_REVCH4, OnDisableRevch4)
	ON_BN_CLICKED(IDC_DISABLE_REVCH5, OnDisableRevch5)
	ON_BN_CLICKED(IDC_DISABLE_REVCH6, OnDisableRevch6)
	//}}AFX_MSG_MAP 
	ON_MESSAGE(WM_SPIN_BUTTON, OnWmSpinButton)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CReflex50Protocol diagnostics

#ifdef _DEBUG
void CReflex50Protocol::AssertValid() const
{
	CPpsFormView::AssertValid();
}

void CReflex50Protocol::Dump(CDumpContext& dc) const
{
	CPpsFormView::Dump(dc);
}
#endif //_DEBUG




BOOL CReflex50Protocol::IsPresentable(void)
{ 
  BOOL		bR50Enable;
  CP_reflex50_enable.bSetFromDoc(&bR50Enable); 
  return(bR50Enable);
}  	




////////////////////////////////////////////////////////////////////////////
//	OnInitialUpdate()
//  
//  Update DDX variables from document and resize frame around view.
//
void CReflex50Protocol::OnInitialUpdate(void)
{
  TRACE("CReflex50Protocol::OnInitialUpdate : CReflex50Protocol::IDD=%X\n", CReflex50Protocol::IDD);

  // init the owner draw buttons and subclass the CEdits              

  VERIFY(InfoSvcFrameUpBtn.AutoLoad(IDC_INFOSVCFRAME_UP, this));
  VERIFY(InfoSvcFrameDnBtn.AutoLoad(IDC_INFOSVCFRAME_DN, this));
  InfoSvcFrameEdit.SubclassDlgItem(IDC_INFOFRAME_EDIT, this);
  InfoSvcFrameEdit.LimitText(3);
  
  VERIFY(InfoSvcCollapseUpBtn.AutoLoad(IDC_INFOSVCCOLLAPSE_UP, this));
  VERIFY(InfoSvcCollapseDnBtn.AutoLoad(IDC_INFOSVCCOLLAPSE_DN, this)); 
  InfoSvcCollapseEdit.SubclassDlgItem(IDC_INFOCOLLAPSE_EDIT, this);
  InfoSvcCollapseEdit.LimitText(1);
  
  VERIFY(PersonalFrameUpBtn.AutoLoad(IDC_PERSFRAME_UP, this));
  VERIFY(PersonalFrameDnBtn.AutoLoad(IDC_PERSFRAME_DN, this)); 
  PersonalFrameEdit.SubclassDlgItem(IDC_PERSFRAME_EDIT, this);
  PersonalFrameEdit.LimitText(3);
  
  VERIFY(PersonalCollapseUpBtn.AutoLoad(IDC_PERSCOLLAPSE_UP, this));
  VERIFY(PersonalCollapseDnBtn.AutoLoad(IDC_PERSCOLLAPSE_DN, this));
  PersonalCollapseEdit.SubclassDlgItem(IDC_PERSCOLLAPSE_EDIT, this);
  PersonalCollapseEdit.LimitText(1);
  
  VERIFY(HomeLogicalCtrlUpBtn.AutoLoad(IDC_HOMECTRL_UP, this));
  VERIFY(HomeLogicalCtrlDnBtn.AutoLoad(IDC_HOMECTRL_DN, this));
  HomeLogicalCtrlEdit.SubclassDlgItem(IDC_HOMECTRLCHAN_EDIT, this);
  HomeLogicalCtrlEdit.LimitText(2);

  VERIFY(FwdChanFreq0UpBtn.AutoLoad(IDC_FWDCH_UP, this));
  VERIFY(FwdChanFreq0DnBtn.AutoLoad(IDC_FWDCH_DN, this));
  FwdChanFreq0Edit.SubclassDlgItem(IDC_FWDCHAN_EDIT, this);
  FwdChanFreq0Edit.LimitText(10);

  VERIFY(AsyncSleepUpBtn.AutoLoad(IDC_ASYNCSLEEP_UP, this));
  VERIFY(AsyncSleepDnBtn.AutoLoad(IDC_ASYNCSLEEP_DN, this));
  AsyncSleepEdit.SubclassDlgItem(IDC_ASYNCSLEEP_EDIT, this);
  AsyncSleepEdit.LimitText(3);
  
  VERIFY(RevChanFreq0UpBtn.AutoLoad(IDC_REVCH0_UP, this));
  VERIFY(RevChanFreq0DnBtn.AutoLoad(IDC_REVCH0_DN, this));
  RevChanFreq0Edit.SubclassDlgItem(IDC_REVCHAN0_EDIT, this);
  RevChanFreq0Edit.LimitText(10);
    
  VERIFY(RevChanFreq1UpBtn.AutoLoad(IDC_REVCH1_UP, this));
  VERIFY(RevChanFreq1DnBtn.AutoLoad(IDC_REVCH1_DN, this));
  RevChanFreq1Edit.SubclassDlgItem(IDC_REVCHAN1_EDIT, this);
  RevChanFreq1Edit.LimitText(10);
    
  VERIFY(RevChanFreq2UpBtn.AutoLoad(IDC_REVCH2_UP, this));
  VERIFY(RevChanFreq2DnBtn.AutoLoad(IDC_REVCH2_DN, this));
  RevChanFreq2Edit.SubclassDlgItem(IDC_REVCHAN2_EDIT, this);
  RevChanFreq2Edit.LimitText(10);
    
  VERIFY(RevChanFreq3UpBtn.AutoLoad(IDC_REVCH3_UP, this));
  VERIFY(RevChanFreq3DnBtn.AutoLoad(IDC_REVCH3_DN, this));
  RevChanFreq3Edit.SubclassDlgItem(IDC_REVCHAN3_EDIT, this);
  RevChanFreq3Edit.LimitText(10);
    
  VERIFY(RevChanFreq4UpBtn.AutoLoad(IDC_REVCH4_UP, this));
  VERIFY(RevChanFreq4DnBtn.AutoLoad(IDC_REVCH4_DN, this));
  RevChanFreq4Edit.SubclassDlgItem(IDC_REVCHAN4_EDIT, this);
  RevChanFreq4Edit.LimitText(10);
    
  VERIFY(RevChanFreq5UpBtn.AutoLoad(IDC_REVCH5_UP, this));
  VERIFY(RevChanFreq5DnBtn.AutoLoad(IDC_REVCH5_DN, this));
  RevChanFreq5Edit.SubclassDlgItem(IDC_REVCHAN5_EDIT, this);
  RevChanFreq5Edit.LimitText(10);
    
  VERIFY(RevChanFreq6UpBtn.AutoLoad(IDC_REVCH6_UP, this));
  VERIFY(RevChanFreq6DnBtn.AutoLoad(IDC_REVCH6_DN, this));
  RevChanFreq6Edit.SubclassDlgItem(IDC_REVCHAN6_EDIT, this);
  RevChanFreq6Edit.LimitText(10);
   
  // disable select visual controls for Mtel's Affiliates PPS
  #ifdef MTEL_AFFILIATE_PPS 
     // Reverse channel disables
     CButton   *pButton = (CButton*)GetDlgItem(IDC_DISABLE_REVCH0); ASSERT(pButton != NULL);
     pButton->EnableWindow(FALSE); 
     CSpinBtn::EnableSpinGroup(this, 0, IDC_REVCHAN0_EDIT, RevChanFreq0UpBtn, 
                               RevChanFreq0DnBtn, FALSE);
     //
     pButton = (CButton*)GetDlgItem(IDC_DISABLE_REVCH1); ASSERT(pButton != NULL);
     pButton->EnableWindow(FALSE);
     CSpinBtn::EnableSpinGroup(this, 0, IDC_REVCHAN1_EDIT, RevChanFreq1UpBtn, 
                               RevChanFreq1DnBtn, FALSE);
                               // 
     pButton = (CButton*)GetDlgItem(IDC_DISABLE_REVCH2); ASSERT(pButton != NULL);
     pButton->EnableWindow(FALSE);
     CSpinBtn::EnableSpinGroup(this, 0, IDC_REVCHAN2_EDIT, RevChanFreq2UpBtn, 
                               RevChanFreq2DnBtn, FALSE);
                               //  
     pButton = (CButton*)GetDlgItem(IDC_DISABLE_REVCH3); ASSERT(pButton != NULL);
     pButton->EnableWindow(FALSE);
     CSpinBtn::EnableSpinGroup(this, 0, IDC_REVCHAN3_EDIT, RevChanFreq3UpBtn, 
                               RevChanFreq3DnBtn, FALSE);
                               //  
     pButton = (CButton*)GetDlgItem(IDC_DISABLE_REVCH4); ASSERT(pButton != NULL);
     pButton->EnableWindow(FALSE);
     CSpinBtn::EnableSpinGroup(this, 0, IDC_REVCHAN4_EDIT, RevChanFreq4UpBtn, 
                               RevChanFreq4DnBtn, FALSE);
                               //  
     pButton = (CButton*)GetDlgItem(IDC_DISABLE_REVCH5); ASSERT(pButton != NULL);
     pButton->EnableWindow(FALSE);
     CSpinBtn::EnableSpinGroup(this, 0, IDC_REVCHAN5_EDIT, RevChanFreq5UpBtn, 
                               RevChanFreq5DnBtn, FALSE);
                               //  
     pButton = (CButton*)GetDlgItem(IDC_DISABLE_REVCH6); ASSERT(pButton != NULL);
     pButton->EnableWindow(FALSE);
     CSpinBtn::EnableSpinGroup(this, 0, IDC_REVCHAN6_EDIT, RevChanFreq6UpBtn, 
                               RevChanFreq6DnBtn, FALSE);
                               //
     // Base Frame Management disables       
     CSpinBtn::EnableSpinGroup(this, 0, IDC_INFOFRAME_EDIT, InfoSvcFrameUpBtn, 
                               InfoSvcFrameDnBtn, FALSE);
                               //
     // Forward Channel disables
     pButton = (CButton*)GetDlgItem(IDC_CHANNEL_ID_CHK);
     ASSERT(pButton != NULL);
     pButton->EnableWindow(FALSE);
     // 
     CSpinBtn::EnableSpinGroup(this, IDC_HOMECTRL_STATIC, IDC_HOMECTRLCHAN_EDIT, HomeLogicalCtrlUpBtn, 
                               HomeLogicalCtrlDnBtn, FALSE);
                               //
     CSpinBtn::EnableSpinGroup(this, IDC_FWDCHAN_STATIC, IDC_FWDCHAN_EDIT, FwdChanFreq0UpBtn, 
                               FwdChanFreq0DnBtn, FALSE);     
  #endif

  CPpsFormView::OnInitialUpdate();
  return;
}





/////////////////////////////////////////////////////////////////////////////
//	OnUpdate()
//
void CReflex50Protocol::OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint)
{
  TRACE("\nCReflex50Protocol::OnUpdate : IDD=%X  LHintHI=%X  lHintLO=%X\n", 
  		CReflex50Protocol::IDD,  HIWORD(lHint), LOWORD(lHint));

  if (HIWORD(lHint))        		// user switched views, now active
    ResizeParentToFit(FALSE);
  
  // check for Reflex50 enabled.  The user may have performed a read of
  // a non-Reflex50 pager while this view was visible.
  BOOL bR50Enable;
  CP_reflex50_enable.bSetFromDoc(&bR50Enable);
  if (bR50Enable == FALSE) 
  {
    theApp.SwitchView(1);    // switch to the logo view
    return;
  }
  else
    SetFromDoc(); 
  
  // Different calculations have been implemented over time.  
  // This if-else sequence assumes future device id's will 
  // increase from the PAGERFINDER device id.
  BYTE byDeviceId;
  BYTE byCPVersion;
  CP_Version.bySetFromDoc(&byCPVersion);
  CP_OTA_Protected_H.bySetFromDoc(&byDeviceId);
  //
  if ((byDeviceId <= PAGEFINDER) && (byCPVersion < 0x07))
    m_nMinFwdChanNum = 0;
  else  
    m_nMinFwdChanNum = 1;
    
  CPpsFormView::OnUpdate(pSender, lHint, pHint);
}	

/////////////////////////////////////////////////////////////////////////////
//  Reflex50 specific channel assignments


////////////////////////////////////////////////////////////////////////////
//  SetFromDoc()
//
void CReflex50Protocol::SetFromDoc(void)
{
  TRACE("\nCReflex50Protocol::SetFromDoc\n");
  CPpsDoc	*pDoc = GetDocument(); 
   
  // CP_InfoFrame
  CP_InfoFrame.bySetFromDoc(&m_byInfoFrame);
  
  // CP_PagerFrame
  CP_PagerFrame.bySetFromDoc(&m_byPersonalAddressFrame);
  
  // CP_HomeLogicalNum
  CP_HomeLogicalNum.bySetFromDoc(&m_byHomeLogicalNum); 
   
  // CP_PagerCollapse
  CP_PersonalCollapse.bySetFromDoc(&m_byPersonalCollapse);
  CP_InfoServiceCollapse.bySetFromDoc(&m_byInfoSvcCollapse);
  
  // CP_RevChannelMap 
  // NOTE: Consider the channel disabled if the CP_RevChannelNum is equal to 0x7F.
  // 
  CP_RevChannelNum.bySetFromDoc(&m_byRevChan0, 0);
  m_dRevChanFreq0 = dCalcReverseChanFreq(m_byRevChan0); 
  m_bDisableRevCh0 = (m_byRevChan0 == 0x7F);
  //
  CP_RevChannelNum.bySetFromDoc(&m_byRevChan1, 1);
  m_dRevChanFreq1 = dCalcReverseChanFreq(m_byRevChan1);
  m_bDisableRevCh1 = (m_byRevChan1 == 0x7F);
  //
  CP_RevChannelNum.bySetFromDoc(&m_byRevChan2, 2);
  m_dRevChanFreq2 = dCalcReverseChanFreq(m_byRevChan2);
  m_bDisableRevCh2 = (m_byRevChan2 == 0x7F);
  //
  CP_RevChannelNum.bySetFromDoc(&m_byRevChan3, 3);
  m_dRevChanFreq3 = dCalcReverseChanFreq(m_byRevChan3);
  m_bDisableRevCh3 = (m_byRevChan3 == 0x7F);
  //
  CP_RevChannelNum.bySetFromDoc(&m_byRevChan4, 4);
  m_dRevChanFreq4 = dCalcReverseChanFreq(m_byRevChan4);
  m_bDisableRevCh4 = (m_byRevChan4 == 0x7F);
  //
  CP_RevChannelNum.bySetFromDoc(&m_byRevChan5, 5);
  m_dRevChanFreq5 = dCalcReverseChanFreq(m_byRevChan5);
  m_bDisableRevCh5 = (m_byRevChan5 == 0x7F);
  //
  CP_RevChannelNum.bySetFromDoc(&m_byRevChan6, 6);
  m_dRevChanFreq6 = dCalcReverseChanFreq(m_byRevChan6);
  m_bDisableRevCh6 = (m_byRevChan6 == 0x7F);

  #ifndef MTEL_AFFILIATE_PPS 
     // Reverse channel disables
     CSpinBtn::EnableSpinGroup(this, 0, IDC_REVCHAN0_EDIT, RevChanFreq0UpBtn, 
                               RevChanFreq0DnBtn, !m_bDisableRevCh0);
     //
     CSpinBtn::EnableSpinGroup(this, 0, IDC_REVCHAN1_EDIT, RevChanFreq1UpBtn, 
                               RevChanFreq1DnBtn, !m_bDisableRevCh1);
                               // 
     CSpinBtn::EnableSpinGroup(this, 0, IDC_REVCHAN2_EDIT, RevChanFreq2UpBtn, 
                               RevChanFreq2DnBtn, !m_bDisableRevCh2);
                               //  
     CSpinBtn::EnableSpinGroup(this, 0, IDC_REVCHAN3_EDIT, RevChanFreq3UpBtn, 
                               RevChanFreq3DnBtn, !m_bDisableRevCh3);
                               //  
     CSpinBtn::EnableSpinGroup(this, 0, IDC_REVCHAN4_EDIT, RevChanFreq4UpBtn, 
                               RevChanFreq4DnBtn, !m_bDisableRevCh4);
                               //  
     CSpinBtn::EnableSpinGroup(this, 0, IDC_REVCHAN5_EDIT, RevChanFreq5UpBtn, 
                               RevChanFreq5DnBtn, !m_bDisableRevCh5);
                               //  
     CSpinBtn::EnableSpinGroup(this, 0, IDC_REVCHAN6_EDIT, RevChanFreq6UpBtn, 
                               RevChanFreq6DnBtn, !m_bDisableRevCh6);
  #endif

  // CP_Control_Channel - first entry in codeplug field only
  CP_CtrlChanNumber.nSetFromDoc((UINT *)&m_nFwdChan0); 
  m_dFwdChanFreq0 = dCalcForwardChanFreq(m_nFwdChan0);
  
  // CP_AsyncSleepTime
  CP_AsyncSleepTime.bySetFromDoc(&m_byAsyncSleepFrames);
  
  // CP_R50_Misc
  CP_channel_id.bSetFromDoc(&m_bChannel_Id);
  
  UpdateData(FALSE);	// set controls data
}




////////////////////////////////////////////////////////////////////////////
//	SetToDoc()
//
BOOL CReflex50Protocol::SetToDoc(void)
{
  TRACE("\nCReflex50Protocol::SetToDoc\n");
  
  if (!(UpdateData(TRUE)))  // get controls data
    return FALSE;
   
  // CP_InfoFrame
  CP_InfoFrame.bySetToDoc(m_byInfoFrame);
  
  // CP_PagerFrame
  CP_PagerFrame.bySetToDoc(m_byPersonalAddressFrame);
  
  // CP_HomeLogicalNum
  CP_HomeLogicalNum.bySetToDoc(m_byHomeLogicalNum); 
   
  // CP_PagerCollapse
  CP_PersonalCollapse.bySetToDoc(m_byPersonalCollapse);
  CP_InfoServiceCollapse.bySetToDoc(m_byInfoSvcCollapse);
  
  // CP_RevChannelMap
  m_byRevChan0 = byCalcReverseChanNum(m_dRevChanFreq0, NO_ROUNDING);
  m_byRevChan1 = byCalcReverseChanNum(m_dRevChanFreq1, NO_ROUNDING);
  m_byRevChan2 = byCalcReverseChanNum(m_dRevChanFreq2, NO_ROUNDING);
  m_byRevChan3 = byCalcReverseChanNum(m_dRevChanFreq3, NO_ROUNDING);
  m_byRevChan4 = byCalcReverseChanNum(m_dRevChanFreq4, NO_ROUNDING);
  m_byRevChan5 = byCalcReverseChanNum(m_dRevChanFreq5, NO_ROUNDING);
  m_byRevChan6 = byCalcReverseChanNum(m_dRevChanFreq6, NO_ROUNDING);
  // 
  CP_RevChannelNum.bySetToDoc(m_byRevChan0, 0);
  CP_RevChannelNum.bySetToDoc(m_byRevChan1, 1);
  CP_RevChannelNum.bySetToDoc(m_byRevChan2, 2);
  CP_RevChannelNum.bySetToDoc(m_byRevChan3, 3);
  CP_RevChannelNum.bySetToDoc(m_byRevChan4, 4);
  CP_RevChannelNum.bySetToDoc(m_byRevChan5, 5);
  CP_RevChannelNum.bySetToDoc(m_byRevChan6, 6);
                                            
  // CP_Control_Channel - first entry in codeplug field only
  m_nFwdChan0 = nCalcForwardChanNum(m_dFwdChanFreq0, NO_ROUNDING);
  CP_CtrlChanNumber.nSetToDoc((UINT)m_nFwdChan0);
  
  // CP_AsyncSleepTime
  CP_AsyncSleepTime.bySetToDoc(m_byAsyncSleepFrames); 
  
  // CP_R50_Misc
  CP_channel_id.bSetToDoc(m_bChannel_Id);

  return(TRUE);
}



double CReflex50Protocol::dCalcForwardChanFreq(int nChanNum)
{  
  BYTE  byDeviceId, byCPVersion;
  
  CP_Version.bySetFromDoc(&byCPVersion);
  CP_OTA_Protected_H.bySetFromDoc(&byDeviceId);
  
  // Different calculations have been implemented over time.  
  // This if-else sequence assumes future device id's will 
  // increase from the PAGERFINDER device id.
  
  if ((byDeviceId < PAGEFINDER) && (byCPVersion <= 0x05))
    return(929.00625 + (0.00625 * nChanNum));
  else if ((byDeviceId <= PAGEFINDER) && (byCPVersion < 0x07))
    return(929.010 + (0.005 * nChanNum));
  else
    return(929.015 + (0.010 * nChanNum));
}



int CReflex50Protocol::nCalcForwardChanNum(double dFwdFreq, BYTE byRounding)
{
  int 	nCeil;
  int	nFloor; 
  BYTE  byCPVersion, byDeviceId;
  
  CP_Version.bySetFromDoc(&byCPVersion);
  CP_OTA_Protected_H.bySetFromDoc(&byDeviceId);
   
  // Different calculations have been implemented over time.
  // This if-else sequence assumes future device id's will 
  // increase from the PAGERFINDER device id.
  if ((byDeviceId < PAGEFINDER) && (byCPVersion <= 0x05))
  {
    nCeil = (int)(ceil((dFwdFreq - 929.00625) / 0.00625));
    nFloor = (int)(floor((dFwdFreq - 929.00625) / 0.00625));
  }
  else if ((byDeviceId <= PAGEFINDER) && (byCPVersion < 0x07))
  {
    nCeil = (int)(ceil((dFwdFreq - 929.010) / 0.005));
    nFloor = (int)(floor((dFwdFreq - 929.010) / 0.005));
  }
  else
  {
    nCeil = (int)(ceil((dFwdFreq - 929.015) / 0.010));
    nFloor = (int)(floor((dFwdFreq - 929.015) / 0.010));
  }
  
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



double CReflex50Protocol::dCalcReverseChanFreq(BYTE byChanNum)
{  
  return(901.00625 + (0.0125 * byChanNum));
} 


BYTE CReflex50Protocol::byCalcReverseChanNum(double dRevFreq, BYTE byRounding)
{ 
  BYTE byCeil = (BYTE)(ceil((dRevFreq - 901.00625) / 0.0125));
  BYTE byFloor = (BYTE)(floor((dRevFreq - 901.00625) / 0.0125));
                                              
  if (byRounding == ROUND_UP)
    return(byCeil);
  else if (byRounding == ROUND_DN)
    return(byFloor);
  else if (byCeil != byFloor)  // reverse the calculation and compare to given dRevFreq
  { 
    double dCeil = dCalcReverseChanFreq(byCeil);
    double dFloor = dCalcReverseChanFreq(byFloor);
    double dDistanceHi = pow((dCeil - dRevFreq), 2);
    double dDistanceLo = pow((dRevFreq - dFloor), 2); 
    if (dDistanceHi < dDistanceLo)
      return(byCeil);
  }
  return(byFloor);
}




////////////////////////////////////////////////////////////////////////
//  OnAcceptDefaultsBtn()
//
//  This function initiates a move of data from select fields of the
//  CPpsDoc.m_byDefaultCodePlugArray to the CPpsDoc.m_byCodePlugArray.
//  The m_byDefaultCodePlugArray is loaded from file 'default.s19' upon
//  application execution.  If the file has not been found, then no 
//  default values are available.  
//
void CReflex50Protocol::OnAcceptDefaultsBtn()
{
  TRACE("\nCReflex50Protocol::OnAcceptDefaultsBtn\n"); 
  CPpsDoc	*pDoc = GetDocument(); 
   
  #ifdef MTEL_AFFILIATE_PPS
    if (pDoc->SetFieldFromDefault(CP_AsyncSleepTime.Offset(), CP_AsyncSleepTime.Length()))
      return;
    if (pDoc->SetFieldFromDefault(CP_PagerCollapse.Offset(), CP_PagerCollapse.Length()))
      return;
  #else
    if (pDoc->SetFieldFromDefault(CP_InfoFrame.Offset(), CP_InfoFrame.Length()))
      return;
    if (pDoc->SetFieldFromDefault(CP_PagerCollapse.Offset(), CP_PagerCollapse.Length()))
      return;
    if (pDoc->SetFieldFromDefault(CP_RevChannelMap.Offset(), CP_RevChannelMap.Length()))
      return;
    if (pDoc->SetFieldFromDefault(CP_Control_Channel.Offset(), CP_Control_Channel.Length()))
      return; 
    if (pDoc->SetFieldFromDefault(CP_AsyncSleepTime.Offset(), CP_AsyncSleepTime.Length()))
      return;
  #endif
  
  SetFromDoc();   // update this view
}


////////////////////////////////////////////////////////////////////////
//  OnArchived()
//
//  This function initiates a move of data from select fields of the
//  CPpsDoc.m_byArchivedCodePlugArray to the CPpsDoc.m_byCodePlugArray.
//
void CReflex50Protocol::OnArchived(void)
{
  CPpsDoc	*pDoc = GetDocument(); 
   
  #ifdef MTEL_AFFILIATE_PPS
    if (pDoc->SetFieldFromArchived(CP_AsyncSleepTime.Offset(), CP_AsyncSleepTime.Length()))
      return;
    if (pDoc->SetFieldFromArchived(CP_PagerCollapse.Offset(), CP_PagerCollapse.Length()))
      return;
  #else
    if (pDoc->SetFieldFromArchived(CP_InfoFrame.Offset(), CP_InfoFrame.Length()))
      return;
    if (pDoc->SetFieldFromArchived(CP_PagerCollapse.Offset(), CP_PagerCollapse.Length()))
      return;
    if (pDoc->SetFieldFromArchived(CP_RevChannelMap.Offset(), CP_RevChannelMap.Length()))
      return;
    if (pDoc->SetFieldFromArchived(CP_Control_Channel.Offset(), CP_Control_Channel.Length()))
      return; 
    if (pDoc->SetFieldFromArchived(CP_AsyncSleepTime.Offset(), CP_AsyncSleepTime.Length()))
      return;
  #endif  
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
void CReflex50Protocol::OnUndoBtn()
{
  TRACE("\nCReflex50Protocol::OnUndoBtn\n"); 
  CPpsDoc	*pDoc = GetDocument(); 
  
  if (pDoc->SetFieldFromOriginal(CP_InfoFrame.Offset(), CP_InfoFrame.Length()))
    return;
  if (pDoc->SetFieldFromOriginal(CP_PagerFrame.Offset(), CP_PagerFrame.Length()))
    return;
  if (pDoc->SetFieldFromOriginal(CP_HomeLogicalNum.Offset(), CP_HomeLogicalNum.Length()))
    return;
  if (pDoc->SetFieldFromOriginal(CP_PagerCollapse.Offset(), CP_PagerCollapse.Length()))
    return;
  if (pDoc->SetFieldFromOriginal(CP_RevChannelMap.Offset(), CP_RevChannelMap.Length()))
    return;
  if (pDoc->SetFieldFromOriginal(CP_Control_Channel.Offset(), CP_Control_Channel.Length()))
    return;
  if (pDoc->SetFieldFromOriginal(CP_AsyncSleepTime.Offset(), CP_AsyncSleepTime.Length()))
    return;
      
  SetFromDoc();   // update this view
}



void CReflex50Protocol::OnAsyncsleepUp()
{
  if ((m_byAsyncSleepFrames + 1) > 255)
    m_byAsyncSleepFrames = 255; 
  else
    m_byAsyncSleepFrames += 1;
  return;  
}

void CReflex50Protocol::OnAsyncsleepDn()
{
  if ((int)(m_byAsyncSleepFrames - 1) < 0)
    m_byAsyncSleepFrames = 0;
  else
    m_byAsyncSleepFrames -= 1;
  return;  
}


void CReflex50Protocol::OnFwdChanNumIncrement(int nOffset)
{
  m_nFwdChan0 = nCalcForwardChanNum(m_dFwdChanFreq0 /*, ROUND_UP*/);
  if ((int)(m_nFwdChan0 + nOffset) > MAXFWDCHAN)
    m_nFwdChan0 = MAXFWDCHAN;
  else 
    m_nFwdChan0 += nOffset;
  
  m_dFwdChanFreq0 = dCalcForwardChanFreq(m_nFwdChan0);
  return;
}

void CReflex50Protocol::OnFwdChanNumDecrement(int nOffset)
{
  m_nFwdChan0 = nCalcForwardChanNum(m_dFwdChanFreq0 /*, ROUND_DN*/);
  if ((int)(m_nFwdChan0 - nOffset) < m_nMinFwdChanNum)
    m_nFwdChan0 = m_nMinFwdChanNum;
  else
    m_nFwdChan0 -= nOffset;

  m_dFwdChanFreq0 = dCalcForwardChanFreq(m_nFwdChan0);
  return;
}


/////////////////////
void CReflex50Protocol::OnRevChanNumIncrement(BYTE *pbyRevChanNum, 
											  double *pdFrequency,
											  int nOffsetBy)
{
  *pbyRevChanNum = byCalcReverseChanNum(*pdFrequency /*, ROUND_UP*/);
  if (*pbyRevChanNum > MAXREVCHAN)
    *pbyRevChanNum = MINREVCHAN;
  else if ((int)(*pbyRevChanNum + nOffsetBy) > MAXREVCHAN) 
    *pbyRevChanNum = MAXREVCHAN;
  else
    *pbyRevChanNum = (BYTE)(*pbyRevChanNum + nOffsetBy);
    
  *pdFrequency = dCalcReverseChanFreq(*pbyRevChanNum);    
  return;
}


//////////////////////
void CReflex50Protocol::OnRevChanNumDecrement(BYTE *pbyRevChanNum, 
											  double *pdFrequency,
											  int nOffsetBy)
{
  *pbyRevChanNum = byCalcReverseChanNum(*pdFrequency /*, ROUND_DN*/);
  if (*pbyRevChanNum > MAXREVCHAN)
    *pbyRevChanNum = MAXREVCHAN;
  else if ((int)(*pbyRevChanNum - nOffsetBy) < MINREVCHAN)
    *pbyRevChanNum = MINREVCHAN;
  else  
    *pbyRevChanNum = (BYTE)(*pbyRevChanNum - nOffsetBy);
    
  *pdFrequency = dCalcReverseChanFreq(*pbyRevChanNum);    
  return; 	
}

 
 
/////////////////////////////////////////////////////////////////////////
//  OnWmSpinButton()
//
//  Calls to this function are generated from the CSpinButton::OnTimer()
//  function.  These calls result from the user holding down the left
//  mouse button.  The effect is to provide a miniature scroll button.
//
afx_msg LONG CReflex50Protocol::OnWmSpinButton(UINT wParam, LONG lParam)
{
  TRACE("\nCReflex50Protocol::OnWmSpinButton : nBtnID=%04X : nAcceleration=%d\n", wParam, (LOWORD(lParam)));
  
  int nAcceleration = (LOWORD(lParam));
  
  if (!UpdateData(TRUE))  // get controls data 
    return(0L); 

  switch(wParam)
  {
	case IDC_FWDCH_DN: 
	  OnFwdChanNumDecrement(nAcceleration);
	  break;
	case IDC_FWDCH_UP: 
	  OnFwdChanNumIncrement(nAcceleration); 
	  break;
	case IDC_REVCH0_UP:
	  m_bDisableRevCh0 = FALSE; 
	  OnRevChanNumIncrement(&m_byRevChan0, &m_dRevChanFreq0, nAcceleration); 
	  break;
	case IDC_REVCH0_DN: 
	  m_bDisableRevCh0 = FALSE; 
	  OnRevChanNumDecrement(&m_byRevChan0, &m_dRevChanFreq0, nAcceleration); 
	  break;
	case IDC_REVCH1_UP: 
	  m_bDisableRevCh1 = FALSE; 
	  OnRevChanNumIncrement(&m_byRevChan1, &m_dRevChanFreq1, nAcceleration); 
	  break;
	case IDC_REVCH1_DN: 
	  m_bDisableRevCh1 = FALSE; 
	  OnRevChanNumDecrement(&m_byRevChan1, &m_dRevChanFreq1, nAcceleration); 
	  break;
	case IDC_REVCH2_UP: 
	  m_bDisableRevCh2 = FALSE; 
	  OnRevChanNumIncrement(&m_byRevChan2, &m_dRevChanFreq2, nAcceleration); 
	  break;
	case IDC_REVCH2_DN: 
	  m_bDisableRevCh2 = FALSE; 
	  OnRevChanNumDecrement(&m_byRevChan2, &m_dRevChanFreq2, nAcceleration); 
	  break;
	case IDC_REVCH3_UP: 
	  m_bDisableRevCh3 = FALSE; 
	  OnRevChanNumIncrement(&m_byRevChan3, &m_dRevChanFreq3, nAcceleration); 
	  break;
	case IDC_REVCH3_DN: 
	  m_bDisableRevCh3 = FALSE; 
	  OnRevChanNumDecrement(&m_byRevChan3, &m_dRevChanFreq3, nAcceleration); 
	  break;
	case IDC_REVCH4_UP: 
	  m_bDisableRevCh4 = FALSE; 
	  OnRevChanNumIncrement(&m_byRevChan4, &m_dRevChanFreq4, nAcceleration); 
	  break;
	case IDC_REVCH4_DN: 
	  m_bDisableRevCh4 = FALSE; 
	  OnRevChanNumDecrement(&m_byRevChan4, &m_dRevChanFreq4, nAcceleration); 
	  break;
	case IDC_REVCH5_UP: 
	  m_bDisableRevCh5 = FALSE; 
	  OnRevChanNumIncrement(&m_byRevChan5, &m_dRevChanFreq5, nAcceleration); 
	  break;
	case IDC_REVCH5_DN: 
	  m_bDisableRevCh5 = FALSE; 
	  OnRevChanNumDecrement(&m_byRevChan5, &m_dRevChanFreq5, nAcceleration); 
	  break;
	case IDC_REVCH6_UP: 
	  m_bDisableRevCh6 = FALSE; 
	  OnRevChanNumIncrement(&m_byRevChan6, &m_dRevChanFreq6, nAcceleration); 
	  break;
	case IDC_REVCH6_DN: 
	  m_bDisableRevCh6 = FALSE; 
	  OnRevChanNumDecrement(&m_byRevChan6, &m_dRevChanFreq6, nAcceleration); 
	  break;
	case IDC_ASYNCSLEEP_UP: OnAsyncsleepUp(); break;
	case IDC_ASYNCSLEEP_DN: OnAsyncsleepDn(); break;
	case IDC_HOMECTRL_UP: OnHomectrlUp(); break;
	case IDC_HOMECTRL_DN: OnHomectrlDn(); break;
	case IDC_INFOSVCFRAME_UP: OnInfosvcframeUp(); break;
	case IDC_INFOSVCFRAME_DN: OnInfosvcframeDn(); break;
	case IDC_INFOSVCCOLLAPSE_UP: OnInfosvccollapseUp(); break;
	case IDC_INFOSVCCOLLAPSE_DN: OnInfosvccollapseDn(); break;
	case IDC_PERSFRAME_UP: OnPersframeUp(); break;
	case IDC_PERSFRAME_DN: OnPersframeDn(); break;
	case IDC_PERSCOLLAPSE_UP: OnPerscollapseUp(); break;
	case IDC_PERSCOLLAPSE_DN: OnPerscollapseDn(); break;
    default: break;
  }  

  UpdateData(FALSE);

  return(0L);
}


void CReflex50Protocol::OnHomectrlUp()
{ 
  if ((m_byHomeLogicalNum + 1) > 12)
    m_byHomeLogicalNum = 12;
  else
    m_byHomeLogicalNum += 1;
  return;  
}

void CReflex50Protocol::OnHomectrlDn()
{
  if ((int)(m_byHomeLogicalNum - 1) < 1)
    m_byHomeLogicalNum = 1; 
  else
    m_byHomeLogicalNum -= 1;
  return;
}



void CReflex50Protocol::OnInfosvcframeUp()
{
  if ((m_byInfoFrame + 1) > 127)
    m_byInfoFrame = 127;
  else
    m_byInfoFrame += 1;
  return;  
}

void CReflex50Protocol::OnInfosvcframeDn()
{
  if ((int)(m_byInfoFrame - 1) < 0)
    m_byInfoFrame = 0;
  else
    m_byInfoFrame -= 1;
  return;	
}



void CReflex50Protocol::OnInfosvccollapseUp()
{
  if ((m_byInfoSvcCollapse + 1) > 7)
    m_byInfoSvcCollapse = 7;
  else
  m_byInfoSvcCollapse += 1;
  return;  
}

void CReflex50Protocol::OnInfosvccollapseDn()
{
  if ((int)(m_byInfoSvcCollapse - 1) < 0)
    m_byInfoSvcCollapse = 0;
  else
    m_byInfoSvcCollapse -= 1;
  return;	
}



void CReflex50Protocol::OnPersframeUp()
{
  if ((m_byPersonalAddressFrame + 1) > 127)
    m_byPersonalAddressFrame = 127;
  else
    m_byPersonalAddressFrame += 1;
  return;  
}

void CReflex50Protocol::OnPersframeDn()
{
  if ((int)(m_byPersonalAddressFrame - 1) < 0)
    m_byPersonalAddressFrame = 0;
  else
    m_byPersonalAddressFrame -= 1;
  return;	
}


void CReflex50Protocol::OnPerscollapseUp()
{
  if ((m_byPersonalCollapse + 1) > 7)
    m_byPersonalCollapse = 7;
  else
    m_byPersonalCollapse += 1;
  return;  
}

void CReflex50Protocol::OnPerscollapseDn()
{
  if ((int)(m_byPersonalCollapse - 1) < 0)
    m_byPersonalCollapse = 0;
  else
    m_byPersonalCollapse -= 1;
  return;	
}


int CReflex50Protocol::OnPrintHelper(CStringArray &rStringArray, UINT nPageNumber, int nAvailableLines)
{ 
   // if not enough lines available, wait for next page...
   const int nNumLinesRequired = 12;    // count the number of 'rStringArray.Add' calls
   if (nAvailableLines < nNumLinesRequired)
     return(-1);    
   int nNumLinesBegin = rStringArray.GetSize();

   BOOL bR50Enable = FALSE;
  
   // check for Reflex50 enabled
   CP_reflex50_enable.bSetFromDoc(&bR50Enable);
   if (bR50Enable == FALSE)
     return(0);

   char szTemp[200];
   
   rStringArray.Add("PROTOCOL");
   
   rStringArray.Add("    Reverse Channels:");
     sprintf(szTemp, "        (0) %10lf  (1) %10lf  (2) %10lf  (3) %10lf", 
             m_dRevChanFreq0, m_dRevChanFreq1, m_dRevChanFreq2, m_dRevChanFreq3);
     rStringArray.Add(szTemp);
     sprintf(szTemp, "        (4) %10lf  (5) %10lf  (6) %10lf", 
             m_dRevChanFreq4, m_dRevChanFreq5, m_dRevChanFreq6);
     rStringArray.Add(szTemp);
     
   rStringArray.Add("    Base-Frame Management:");
     sprintf(szTemp, "        Info-Service:  Base-Frame %d    Collapse %d", m_byInfoFrame, m_byInfoSvcCollapse);
     rStringArray.Add(szTemp);
     sprintf(szTemp, "        Personal Addresses:  Base-Frame %d    Collapse %d", m_byPersonalAddressFrame, m_byPersonalCollapse);
     rStringArray.Add(szTemp);
     
   rStringArray.Add("    Forward Channel:");
     sprintf(szTemp, "        Logical Control Channel:  %d", m_byHomeLogicalNum);
     rStringArray.Add(szTemp);
     sprintf(szTemp, "        Frequency:  %10lf", m_dFwdChanFreq0);
     rStringArray.Add(szTemp);
     sprintf(szTemp, "        Sleep Duty Cycle:  %d", m_byAsyncSleepFrames); 
     rStringArray.Add(szTemp);
   
   rStringArray.Add("");
   return(rStringArray.GetSize() - nNumLinesBegin);
}



 
void CReflex50Protocol::OnDisableRevch0()
{ 
  OnDisableReverseChan(RevChanFreq0Edit, RevChanFreq0UpBtn, RevChanFreq0DnBtn, 
                       &m_byRevChan0, &m_dRevChanFreq0, &m_bDisableRevCh0);    
  return;
}

void CReflex50Protocol::OnDisableRevch1()
{
  OnDisableReverseChan(RevChanFreq1Edit, RevChanFreq1UpBtn, RevChanFreq1DnBtn, 
                       &m_byRevChan1, &m_dRevChanFreq1, &m_bDisableRevCh1);    
  return;	
}

void CReflex50Protocol::OnDisableRevch2()
{
  OnDisableReverseChan(RevChanFreq2Edit, RevChanFreq2UpBtn, RevChanFreq2DnBtn, 
                       &m_byRevChan2, &m_dRevChanFreq2, &m_bDisableRevCh2);    
  return;	
}

void CReflex50Protocol::OnDisableRevch3()
{
  OnDisableReverseChan(RevChanFreq3Edit, RevChanFreq3UpBtn, RevChanFreq3DnBtn, 
                       &m_byRevChan3, &m_dRevChanFreq3, &m_bDisableRevCh3);    
  return;	
}

void CReflex50Protocol::OnDisableRevch4()
{
  OnDisableReverseChan(RevChanFreq4Edit, RevChanFreq4UpBtn, RevChanFreq4DnBtn, 
                       &m_byRevChan4, &m_dRevChanFreq4, &m_bDisableRevCh4);    
  return;	
}

void CReflex50Protocol::OnDisableRevch5()
{
  OnDisableReverseChan(RevChanFreq5Edit, RevChanFreq5UpBtn, RevChanFreq5DnBtn, 
                       &m_byRevChan5, &m_dRevChanFreq5, &m_bDisableRevCh5);    
  return;	
}

void CReflex50Protocol::OnDisableRevch6()
{
  OnDisableReverseChan(RevChanFreq6Edit, RevChanFreq6UpBtn, RevChanFreq6DnBtn, 
                       &m_byRevChan6, &m_dRevChanFreq6, &m_bDisableRevCh6);    
  return;	
}


void CReflex50Protocol::OnDisableReverseChan(CDecimalEdit &rEdit, 
                                             CSpinBtn &rUpBtn, 
                                             CSpinBtn &rDnBtn,
                                             BYTE *pbyRevChanX,
                                             double *pdRevChanXFreq, 
                                             BOOL *pbDisable)
{
  UpdateData(TRUE);
  
  if (*pbDisable)
    *pbyRevChanX = 0x7F;
  else
    *pbyRevChanX = 0x00;

  *pdRevChanXFreq = dCalcReverseChanFreq(*pbyRevChanX); 
  UpdateData(FALSE);

  CSpinBtn::EnableSpinGroup(this, 0, rEdit, rUpBtn, rDnBtn, !(*pbDisable));
  return;
}


