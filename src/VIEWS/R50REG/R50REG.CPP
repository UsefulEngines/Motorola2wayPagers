//*************************************************************
//  History:    Date       Author               Comment
//              1996       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
// r50reg.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\pps.h"
#include "..\..\ppsdoc.h"
#include "..\..\ppsview.h" 
#include "..\..\cpager\codeplug.h" 
#include "r50reg.h"

extern CPpsApp    theApp;

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReflex50Registration

IMPLEMENT_DYNCREATE(CReflex50Registration, CPpsFormView)

CReflex50Registration::CReflex50Registration()
	: CPpsFormView(CReflex50Registration::IDD)
{
	//{{AFX_DATA_INIT(CReflex50Registration)
	m_nAggRegRetryCnt = 0;
	m_nAggRegRetryTmr = 0;
	m_nAggRegCeiling = 0;
	m_byAggRegAttack = 0;
	m_nAlohaRetryTimer = 0;
	m_nAlohaRetryCount = 0;
	m_bAllowAutoRegOnZoneChg = FALSE;
	m_nCommunicadoTmr = 0;
	m_nInCommunicadoTmr = 0;
	m_byInRangeThreshold = 0;
	m_byOutOfRangeThreshold = 0;
	m_bNotifyTxChange = FALSE;
	m_byAlohaRndInterval = 0;
	//}}AFX_DATA_INIT
}

CReflex50Registration::~CReflex50Registration()
{
}

void CReflex50Registration::DoDataExchange(CDataExchange* pDX)
{
	CPpsFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReflex50Registration)
	DDX_Text(pDX, IDC_AGGRETRYCOUNT_EDIT, m_nAggRegRetryCnt);
	DDV_MinMaxUInt(pDX, m_nAggRegRetryCnt, 0, 15);
	DDX_Text(pDX, IDC_AGGRETRYTIMER_EDIT, m_nAggRegRetryTmr);
	DDV_MinMaxUInt(pDX, m_nAggRegRetryTmr, 0, 65535);
	DDX_Text(pDX, IDC_AGGREGCEILING_EDIT, m_nAggRegCeiling);
	DDV_MinMaxUInt(pDX, m_nAggRegCeiling, 0, 65535);
	DDX_Text(pDX, IDC_AGGREG_ATTACK_EDIT, m_byAggRegAttack);
	DDV_MinMaxByte(pDX, m_byAggRegAttack, 0, 255);
	DDX_Text(pDX, IDC_ALOHARETRYTIMER_EDIT, m_nAlohaRetryTimer);
	DDV_MinMaxUInt(pDX, m_nAlohaRetryTimer, 0, 65535);
	DDX_Text(pDX, IDC_ALOHARETRYCOUNT_EDIT, m_nAlohaRetryCount);
	DDV_MinMaxUInt(pDX, m_nAlohaRetryCount, 0, 15);
	DDX_Check(pDX, IDC_AUTOREG_CHECK, m_bAllowAutoRegOnZoneChg);
	DDX_Text(pDX, IDC_COMMUNICADO_EDIT, m_nCommunicadoTmr);
	DDV_MinMaxUInt(pDX, m_nCommunicadoTmr, 0, 65535);
	DDX_Text(pDX, IDC_INCOMMUNICADO_EDIT, m_nInCommunicadoTmr);
	DDV_MinMaxUInt(pDX, m_nInCommunicadoTmr, 0, 65535);
	DDX_Text(pDX, IDC_INRANGE_EDIT, m_byInRangeThreshold);
	DDV_MinMaxByte(pDX, m_byInRangeThreshold, 0, 15);
	DDX_Text(pDX, IDC_OUTOFRANGE_EDIT, m_byOutOfRangeThreshold);
	DDV_MinMaxByte(pDX, m_byOutOfRangeThreshold, 0, 15);
	DDX_Check(pDX, IDC_TXCHGNOTIFY_CHECK, m_bNotifyTxChange);
	DDX_Text(pDX, IDC_ALOHA_RANDOM_INTERVAL_EDIT, m_byAlohaRndInterval);
	DDV_MinMaxByte(pDX, m_byAlohaRndInterval, 0, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReflex50Registration, CPpsFormView)
	//{{AFX_MSG_MAP(CReflex50Registration)
	ON_BN_CLICKED(IDC_R50REG_DEFAULTS_BTN, OnAcceptDefaultsBtn)
	ON_BN_CLICKED(IDC_R50REG_UNDO_BTN, OnUndoBtn)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SPIN_BUTTON, OnWmSpinButton)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CReflex50Registration diagnostics

#ifdef _DEBUG
void CReflex50Registration::AssertValid() const
{
	CPpsFormView::AssertValid();
}

void CReflex50Registration::Dump(CDumpContext& dc) const
{
	CPpsFormView::Dump(dc);
}
#endif //_DEBUG



BOOL CReflex50Registration::IsPresentable(void)
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
void CReflex50Registration::OnInitialUpdate(void)
{
  TRACE("CReflex50Registration::OnInitialUpdate : CReflex50Registration::IDD=%X\n", CReflex50Registration::IDD);

  VERIFY(AggrAttackUp.AutoLoad(IDC_AGGRATTACK_UP, this));
  VERIFY(AggrAttackDn.AutoLoad(IDC_AGGRATTACK_DN, this));
  
  VERIFY(AggrCeilingUp.AutoLoad(IDC_AGGRCEILING_UP, this));
  VERIFY(AggrCeilingDn.AutoLoad(IDC_AGGRCEILING_DN, this));
  
  VERIFY(AggrRetryCntUp.AutoLoad(IDC_AGGRRETRYCNT_UP, this));
  VERIFY(AggrRetryCntDn.AutoLoad(IDC_AGGRRETRYCNT_DN, this));
  
  VERIFY(AggrRetryTimerUp.AutoLoad(IDC_AGGRRETRYTIMER_UP, this));
  VERIFY(AggrRetryTimerDn.AutoLoad(IDC_AGGRRETRYTIMER_DN, this));
  
  VERIFY(CommunicadoUp.AutoLoad(IDC_COMMUNICADO_UP, this));
  VERIFY(CommunicadoDn.AutoLoad(IDC_COMMUNICADO_DN, this));
  
  VERIFY(InCommunicadoUp.AutoLoad(IDC_INCOMMUNICADO_UP, this));
  VERIFY(InCommunicadoDn.AutoLoad(IDC_INCOMMUNICADO_DN, this));
  
  VERIFY(InRangeTimerUp.AutoLoad(IDC_INRANGETIMER_UP, this));
  VERIFY(InRangeTimerDn.AutoLoad(IDC_INRANGETIMER_DN, this));
  
  VERIFY(OutOfRangeTimerUp.AutoLoad(IDC_OORTIMER_UP, this));
  VERIFY(OutOfRangeTimerDn.AutoLoad(IDC_OORTIMER_DN, this));
  
  VERIFY(AlohaRetryCntUp.AutoLoad(IDC_RETRYCNT_UP, this));
  VERIFY(AlohaRetryCntDn.AutoLoad(IDC_RETRYCNT_DN, this));
  
  VERIFY(AlohaRetryTimerUp.AutoLoad(IDC_RETRYTIMER_UP, this));
  VERIFY(AlohaRetryTimerDn.AutoLoad(IDC_RETRYTIMER_DN, this));

  VERIFY(AlohaRandomIntervalUp.AutoLoad(IDC_RANDOM_INT_UP, this));
  VERIFY(AlohaRandomIntervalDn.AutoLoad(IDC_RANDOM_INT_DN, this));
  

  // disable select visual controls for Mtel's Affiliates PPS
  #ifdef MTEL_AFFILIATE_PPS 
     CButton   *pButton = (CButton*)GetDlgItem(IDC_AUTOREG_CHECK); ASSERT(pButton != NULL);
     pButton->EnableWindow(FALSE);
     //
     CSpinBtn::EnableSpinGroup(this, IDC_INCOMMTIMER_STATIC, IDC_INCOMMUNICADO_EDIT, 
     						   InCommunicadoUp, InCommunicadoDn, FALSE);
     // 
     CSpinBtn::EnableSpinGroup(this, IDC_COMMTIMER_STATIC, IDC_COMMUNICADO_EDIT, 
     						   CommunicadoUp, CommunicadoDn, FALSE);
     // 
     CSpinBtn::EnableSpinGroup(this, IDC_AGGRATTACK_STATIC, IDC_AGGREG_ATTACK_EDIT, 
     						   AggrAttackUp, AggrAttackDn, FALSE);
     // 
     CSpinBtn::EnableSpinGroup(this, IDC_AGGRCEILING_STATIC, IDC_AGGREGCEILING_EDIT, 
     						   AggrCeilingUp, AggrCeilingDn, FALSE);
     // 
     CSpinBtn::EnableSpinGroup(this, IDC_AGGRETRYTIMER_STATIC, IDC_AGGRETRYTIMER_EDIT, 
     						   AggrRetryTimerUp, AggrRetryTimerDn, FALSE);
     // 
     CSpinBtn::EnableSpinGroup(this, IDC_AGGRRETRY_STATIC, IDC_AGGRETRYCOUNT_EDIT, 
     						   AggrRetryCntUp, AggrRetryCntDn, FALSE);
     // 
     CSpinBtn::EnableSpinGroup(this, IDC_INRANGETIMER_STATIC, IDC_INRANGE_EDIT, 
     						   InRangeTimerUp, InRangeTimerDn, FALSE);
     // 
     CSpinBtn::EnableSpinGroup(this, IDC_OUTOFRANGE_STATIC, IDC_OUTOFRANGE_EDIT, 
     						   OutOfRangeTimerUp, OutOfRangeTimerDn, FALSE);
     // 
     pButton = (CButton*)GetDlgItem(IDC_TXCHGNOTIFY_CHECK); ASSERT(pButton != NULL);
     pButton->EnableWindow(FALSE);
     //
     CSpinBtn::EnableSpinGroup(this, IDC_ALOHARETRYTIMER_STATIC, IDC_ALOHARETRYTIMER_EDIT, 
                               AlohaRetryTimerUp, AlohaRetryTimerDn, FALSE);
     // 
     CSpinBtn::EnableSpinGroup(this, IDC_ALOHARETRYCOUNT_STATIC, IDC_ALOHARETRYCOUNT_EDIT, 
     						   AlohaRetryCntUp, AlohaRetryCntDn, FALSE);
     // 
     CSpinBtn::EnableSpinGroup(this, IDC_RANDOMIZEINT_STATIC, IDC_ALOHA_RANDOM_INTERVAL_EDIT, 
     						   AlohaRandomIntervalUp, AlohaRandomIntervalDn, FALSE);
     //
     pButton = (CButton*)GetDlgItem(IDC_R50REG_DEFAULTS_BTN); ASSERT(pButton != NULL);
     pButton->EnableWindow(FALSE);
     //
     pButton = (CButton*)GetDlgItem(IDC_R50REG_UNDO_BTN); ASSERT(pButton != NULL);
     pButton->EnableWindow(FALSE);
     //
      
  #endif


  CPpsFormView::OnInitialUpdate();
  return;
}




/////////////////////////////////////////////////////////////////////////////
//	OnUpdate()
//
void CReflex50Registration::OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint)
{
  TRACE("\nCReflex50Registration::OnUpdate : IDD=%X  LHintHI=%X  lHintLO=%X\n", 
  		CReflex50Registration::IDD,  HIWORD(lHint), LOWORD(lHint));

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

  CPpsFormView::OnUpdate(pSender, lHint, pHint);
}	


////////////////////////////////////////////////////////////////////////////
//  SetFromDoc()
//
void CReflex50Registration::SetFromDoc(void)
{
  TRACE("\nCReflex50Registration::SetFromDoc\n");
  CPpsDoc	*pDoc = GetDocument();
  
  CP_allow_auto_reg.bSetFromDoc(&m_bAllowAutoRegOnZoneChg);
  m_bAllowAutoRegOnZoneChg = !(m_bAllowAutoRegOnZoneChg);  // LOGIC REVERSED -> 0 == TRUE
  CP_notify_tx_change.bSetFromDoc(&m_bNotifyTxChange);
  
  CP_IncommunicadoTmr.nSetFromDoc((UINT*)&m_nInCommunicadoTmr);
  
  CP_CommunicadoTmr.nSetFromDoc((UINT*)&m_nCommunicadoTmr);
  
  CP_AlohaTimeout.nSetFromDoc((UINT*)&m_nAlohaRetryTimer);
  
  CP_Retry_Cnt.nSetFromDoc((UINT*)&m_nAlohaRetryCount);
   
  CP_AggReg_Attack.bySetFromDoc(&m_byAggRegAttack);
  
  CP_AggReg_Ceiling.nSetFromDoc((UINT*)&m_nAggRegCeiling);

  CP_AggReg_RetryTmr.nSetFromDoc((UINT*)&m_nAggRegRetryTmr);

  CP_AggReg_RetryCnt.nSetFromDoc((UINT*)&m_nAggRegRetryCnt);

  CP_OutOfRangeSyncs.bySetFromDoc(&m_byOutOfRangeThreshold); 
  CP_InRangeSyncs.bySetFromDoc(&m_byInRangeThreshold);
  
  CP_AlohaRndInterval.bySetFromDoc(&m_byAlohaRndInterval);  

  UpdateData(FALSE);	// set controls data
}



////////////////////////////////////////////////////////////////////////////
//	SetToDoc()
//
BOOL CReflex50Registration::SetToDoc(void)
{
  TRACE("\nCReflex50Registration::SetToDoc\n");
  BOOL			bTemp; 
  
  if (!(UpdateData(TRUE)))  // get controls data
    return FALSE;
  
  bTemp = !(m_bAllowAutoRegOnZoneChg);  // LOGIC REVERSED -> 0 == TRUE
  CP_allow_auto_reg.bSetToDoc(bTemp);
  CP_notify_tx_change.bSetToDoc(m_bNotifyTxChange);
   
  CP_IncommunicadoTmr.nSetToDoc((UINT)m_nInCommunicadoTmr);
  
  CP_CommunicadoTmr.nSetToDoc((UINT)m_nCommunicadoTmr);
  
  CP_AlohaTimeout.nSetToDoc((UINT)m_nAlohaRetryTimer);
  
  CP_Retry_Cnt.nSetToDoc((UINT)m_nAlohaRetryCount);
   
  CP_AggReg_Attack.bySetToDoc(m_byAggRegAttack);
  
  CP_AggReg_Ceiling.nSetToDoc((UINT)m_nAggRegCeiling);

  CP_AggReg_RetryTmr.nSetToDoc((UINT)m_nAggRegRetryTmr);

  CP_AggReg_RetryCnt.nSetToDoc((UINT)m_nAggRegRetryCnt);

  CP_OutOfRangeSyncs.bySetToDoc(m_byOutOfRangeThreshold); 
  CP_InRangeSyncs.bySetToDoc(m_byInRangeThreshold);
  
  CP_AlohaRndInterval.bySetToDoc(m_byAlohaRndInterval);  

  return(TRUE);
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
void CReflex50Registration::OnAcceptDefaultsBtn()
{
  TRACE("\nCReflex50Registration::OnAcceptDefaultsBtn\n"); 
  CPpsDoc	*pDoc = GetDocument(); 
 
  #ifndef MTEL_AFFILIATE_PPS  
  if (pDoc->SetFieldFromDefault(CP_R50_Misc.Offset(), CP_R50_Misc.Length()))
    return;
  if (pDoc->SetFieldFromDefault(CP_AggReg_RetryTmr.Offset(), CP_AggReg_RetryTmr.Length()))
    return;  
  if (pDoc->SetFieldFromDefault(CP_AlohaTimeout.Offset(), CP_AlohaTimeout.Length()))
    return;  
  if (pDoc->SetFieldFromDefault(CP_Retry_Cnt.Offset(), CP_Retry_Cnt.Length()))
    return;  
  if (pDoc->SetFieldFromDefault(CP_AlohaRndInterval.Offset(), CP_AlohaRndInterval.Length()))
    return;  
  if (pDoc->SetFieldFromDefault(CP_IncommunicadoTmr.Offset(), CP_IncommunicadoTmr.Length()))
    return;  
  if (pDoc->SetFieldFromDefault(CP_CommunicadoTmr.Offset(), CP_CommunicadoTmr.Length()))
    return;  
  if (pDoc->SetFieldFromDefault(CP_AggReg_Attack.Offset(), CP_AggReg_Attack.Length()))
    return;  
  if (pDoc->SetFieldFromDefault(CP_AggReg_Ceiling.Offset(), CP_AggReg_Ceiling.Length()))
    return;  
  if (pDoc->SetFieldFromDefault(CP_AggReg_RetryCnt.Offset(), CP_AggReg_RetryCnt.Length()))
    return; 
  if (pDoc->SetFieldFromDefault(CP_InOutOfRangeThreshold.Offset(), CP_InOutOfRangeThreshold.Length()))
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
void CReflex50Registration::OnArchived(void)
{
  CPpsDoc	*pDoc = GetDocument(); 
   
  #ifndef MTEL_AFFILIATE_PPS  
  if (pDoc->SetFieldFromArchived(CP_R50_Misc.Offset(), CP_R50_Misc.Length()))
    return;
  if (pDoc->SetFieldFromArchived(CP_AggReg_RetryTmr.Offset(), CP_AggReg_RetryTmr.Length()))
    return;  
  if (pDoc->SetFieldFromArchived(CP_AlohaTimeout.Offset(), CP_AlohaTimeout.Length()))
    return;             
  if (pDoc->SetFieldFromArchived(CP_Retry_Cnt.Offset(), CP_Retry_Cnt.Length()))
    return;  
  if (pDoc->SetFieldFromArchived(CP_AlohaRndInterval.Offset(), CP_AlohaRndInterval.Length()))
    return;
  if (pDoc->SetFieldFromArchived(CP_IncommunicadoTmr.Offset(), CP_IncommunicadoTmr.Length()))
    return;  
  if (pDoc->SetFieldFromArchived(CP_CommunicadoTmr.Offset(), CP_CommunicadoTmr.Length()))
    return;  
  if (pDoc->SetFieldFromArchived(CP_AggReg_Attack.Offset(), CP_AggReg_Attack.Length()))
    return;  
  if (pDoc->SetFieldFromArchived(CP_AggReg_Ceiling.Offset(), CP_AggReg_Ceiling.Length()))
    return;  
  if (pDoc->SetFieldFromArchived(CP_AggReg_RetryCnt.Offset(), CP_AggReg_RetryCnt.Length()))
    return; 
  if (pDoc->SetFieldFromArchived(CP_InOutOfRangeThreshold.Offset(), CP_InOutOfRangeThreshold.Length()))
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
void CReflex50Registration::OnUndoBtn()
{
  TRACE("\nCReflex50Registration::OnUndoBtn\n"); 
  CPpsDoc	*pDoc = GetDocument(); 
  
  if (pDoc->SetFieldFromOriginal(CP_R50_Misc.Offset(), CP_R50_Misc.Length()))
    return;
  if (pDoc->SetFieldFromOriginal(CP_IncommunicadoTmr.Offset(), CP_IncommunicadoTmr.Length()))
    return;  
  if (pDoc->SetFieldFromOriginal(CP_CommunicadoTmr.Offset(), CP_CommunicadoTmr.Length()))
    return;  
  if (pDoc->SetFieldFromOriginal(CP_AlohaTimeout.Offset(), CP_AlohaTimeout.Length()))
    return;  
  if (pDoc->SetFieldFromOriginal(CP_Retry_Cnt.Offset(), CP_Retry_Cnt.Length()))
    return;  
  if (pDoc->SetFieldFromOriginal(CP_AggReg_Attack.Offset(), CP_AggReg_Attack.Length()))
    return;  
  if (pDoc->SetFieldFromOriginal(CP_AggReg_Ceiling.Offset(), CP_AggReg_Ceiling.Length()))
    return;  
  if (pDoc->SetFieldFromOriginal(CP_AggReg_RetryTmr.Offset(), CP_AggReg_RetryTmr.Length()))
    return;  
  if (pDoc->SetFieldFromOriginal(CP_AggReg_RetryCnt.Offset(), CP_AggReg_RetryCnt.Length()))
    return; 
  if (pDoc->SetFieldFromOriginal(CP_InOutOfRangeThreshold.Offset(), CP_InOutOfRangeThreshold.Length()))
    return;
  if (pDoc->SetFieldFromOriginal(CP_AlohaRndInterval.Offset(), CP_AlohaRndInterval.Length()))
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
afx_msg LONG CReflex50Registration::OnWmSpinButton(UINT wParam, LONG lParam)
{
  TRACE("\nCReflex50Registration::OnWmSpinButton : nBtnID=%04X\n", wParam);
  
  UINT nAcceleration = LOWORD(lParam);
  if (!UpdateData(TRUE))  // get controls data
    return(0L);
    
  switch(wParam)
  {
	case IDC_AGGRATTACK_UP: 
	  if ((m_byAggRegAttack + nAcceleration) <= 0xFF) 
	    m_byAggRegAttack = (BYTE)(m_byAggRegAttack + nAcceleration);
	  else
	    m_byAggRegAttack = 0xFF;  
	  break;
	case IDC_AGGRATTACK_DN:
	  if (nAcceleration < m_byAggRegAttack)
	    m_byAggRegAttack = (BYTE)(m_byAggRegAttack - nAcceleration);
	  else
	    m_byAggRegAttack = 0;  
	  break;
	  //
	case IDC_AGGRCEILING_UP:
	  if ((0xFFFF - nAcceleration) > m_nAggRegCeiling)  
	    m_nAggRegCeiling += nAcceleration;
	  else
	    m_nAggRegCeiling = 0xFFFF; 
	  break;
	case IDC_AGGRCEILING_DN:
	  if (nAcceleration < m_nAggRegCeiling)
	    m_nAggRegCeiling -= nAcceleration;
	  else
	    m_nAggRegCeiling = 0;  
	  break;
	  //
	case IDC_AGGRRETRYCNT_UP:  
	  if ((m_nAggRegRetryCnt + nAcceleration) <= 15) 
	    m_nAggRegRetryCnt += nAcceleration;
	  else
	    m_nAggRegRetryCnt = 15;  
	  break;
	case IDC_AGGRRETRYCNT_DN:
	  if (nAcceleration < m_nAggRegRetryCnt)
	    m_nAggRegRetryCnt -= nAcceleration;
	  else
	    m_nAggRegRetryCnt = 0;  
	  break;
	  //
	case IDC_AGGRRETRYTIMER_UP:
	  if ((0xFFFF - nAcceleration) > m_nAggRegRetryTmr)  
	    m_nAggRegRetryTmr += nAcceleration;
	  else
	    m_nAggRegRetryTmr = 0xFFFF;  
	  break;
	case IDC_AGGRRETRYTIMER_DN:
	  if (nAcceleration < m_nAggRegRetryTmr)
	    m_nAggRegRetryTmr -= nAcceleration;
	  else
	    m_nAggRegRetryTmr = 0;  
	  break;
	  //
	case IDC_COMMUNICADO_UP: 
	  if ((0xFFFF - nAcceleration) > m_nCommunicadoTmr) 
	    m_nCommunicadoTmr += nAcceleration; 
	  else
	    m_nCommunicadoTmr = 0xFFFF; 
      break;
	case IDC_COMMUNICADO_DN:
	  if (nAcceleration < m_nCommunicadoTmr)
	    m_nCommunicadoTmr -= nAcceleration;
	  else
	    m_nCommunicadoTmr = 0;  
	  break;
      //
	case IDC_INCOMMUNICADO_UP:
	  if ((0xFFFF - nAcceleration) > m_nInCommunicadoTmr)  
	    m_nInCommunicadoTmr += nAcceleration;
	  else
	    m_nInCommunicadoTmr = 0xFFFF;  
	  break; 
	case IDC_INCOMMUNICADO_DN: 
	  if (nAcceleration < m_nInCommunicadoTmr)
	    m_nInCommunicadoTmr -= nAcceleration;
	  else
	    m_nInCommunicadoTmr = 0;  
	  break;
	  //
	case IDC_INRANGETIMER_UP:
	  if ((m_byInRangeThreshold + 1) <= 0x0F)  
	    m_byInRangeThreshold += 1;  
	  break;
	case IDC_INRANGETIMER_DN:
	  if (m_byInRangeThreshold > 0)
	    m_byInRangeThreshold -= 1;  
	  break;
	  //
	case IDC_OORTIMER_UP: 
	  if ((m_byOutOfRangeThreshold + 1) <= 0x0F)  
	    m_byOutOfRangeThreshold += 1;
	  break;
	case IDC_OORTIMER_DN:
	  if (m_byOutOfRangeThreshold > 0) 
	    m_byOutOfRangeThreshold -= 1;
	  break;
	  //
	case IDC_RETRYCNT_UP:
	  if ((m_nAlohaRetryCount + nAcceleration) <= 15) 
	    m_nAlohaRetryCount += nAcceleration; 
	  else
	    m_nAlohaRetryCount = 15;
	  break;
	case IDC_RETRYCNT_DN:
	  if (m_nAlohaRetryCount > 0) 
	    m_nAlohaRetryCount -= 1;
	  break;
	  //
	case IDC_RETRYTIMER_UP:
	  if ((0xFFFF - nAcceleration) > m_nAlohaRetryTimer)  
	    m_nAlohaRetryTimer += nAcceleration;
	  else
	    m_nAlohaRetryTimer = 0xFFFF;  
	  break;
	case IDC_RETRYTIMER_DN:
	  if (nAcceleration < m_nAlohaRetryTimer)
	    m_nAlohaRetryTimer -= nAcceleration;
	  else
	    m_nAlohaRetryTimer = 0;  
	  break;
	  //
	case IDC_RANDOM_INT_UP:
	  if ((m_byAlohaRndInterval + nAcceleration) < 0xFF) 
	    m_byAlohaRndInterval = (BYTE)(m_byAlohaRndInterval + nAcceleration);
	  else
	    m_byAlohaRndInterval = 0xFF;
	  break;
	case IDC_RANDOM_INT_DN:  
	  if (nAcceleration < m_byAlohaRndInterval)
	    m_byAlohaRndInterval = (BYTE)(m_byAlohaRndInterval - nAcceleration);
	  else
	    m_byAlohaRndInterval = 0;
	  break;	
	  //  
    default: 
      break;
  }  

  UpdateData(FALSE);
  return(0L);
}


int CReflex50Registration::OnPrintHelper(CStringArray &rStringArray, UINT nPageNumber, int nAvailableLines)
{ 
   BOOL bR50Enable = FALSE;
  
   // check for Reflex50 enabled
   CP_reflex50_enable.bSetFromDoc(&bR50Enable);
   if (bR50Enable == FALSE)
     return(0);

   // if not enough lines available, wait for next page...
   const int nNumLinesRequired = 16;    // count the number of 'rStringArray.Add' calls
   if (nAvailableLines < nNumLinesRequired)
     return(-1);    
   int nNumLinesBegin = rStringArray.GetSize();
     
   char szTemp[200];
   CString szLocal;
   
   rStringArray.Add("REGISTRATION");                
     szLocal = "    Auto-Registration Mode:  ";
     if (m_bAllowAutoRegOnZoneChg)
       szLocal += "Enabled";
     else
       szLocal += "Disabled";
     rStringArray.Add(szLocal);     
     sprintf(szTemp, "    InCommunicado Timer:  %d", m_nInCommunicadoTmr);
     rStringArray.Add(szTemp);     
     sprintf(szTemp, "    Communicado Timer:  %d", m_nCommunicadoTmr);
     rStringArray.Add(szTemp);    
     sprintf(szTemp, "    Aggressive Attack:  %d", m_byAggRegAttack);
     rStringArray.Add(szTemp);     
     sprintf(szTemp, "    Aggressive Ceiling:  %d", m_nAggRegCeiling);
     rStringArray.Add(szTemp);     
     sprintf(szTemp, "    Aggressive Retry Timer:  %d", m_nAggRegRetryTmr);
     rStringArray.Add(szTemp);     
     sprintf(szTemp, "    Aggressive Retry Count:  %d", m_nAggRegRetryCnt);
     rStringArray.Add(szTemp);     
     sprintf(szTemp, "    In-Range Timer:  %d",  m_byInRangeThreshold);
     rStringArray.Add(szTemp);
     sprintf(szTemp, "    Out-of-Range Timer:  %d", m_byOutOfRangeThreshold);
     rStringArray.Add(szTemp);     
     rStringArray.Add("    Aloha Packet Transmission:");
       szLocal = "        Transmitter State Change Notify:  ";
       if (m_bNotifyTxChange)
         szLocal += "Enabled";
       else
         szLocal += "Disabled";
       rStringArray.Add(szLocal);         
       sprintf(szTemp, "        Retry Timer:  %d", m_nAlohaRetryTimer);
       rStringArray.Add(szTemp);       
       sprintf(szTemp, "        Retry Count:  %d", m_nAlohaRetryCount);
       rStringArray.Add(szTemp);      
       sprintf(szTemp, "    Randomize Interval:  %d", m_byAlohaRndInterval);
       rStringArray.Add(szTemp);       
   
   rStringArray.Add("");
   return(rStringArray.GetSize() - nNumLinesBegin);
}



