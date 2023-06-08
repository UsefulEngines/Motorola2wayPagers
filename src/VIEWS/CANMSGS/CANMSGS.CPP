//*************************************************************
//  History:    Date       Author               Comment
//              1996       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
// canmsgs.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\pps.h"
#include "..\..\ppsdoc.h"
#include "..\..\ppsview.h" 
#include "..\..\cpager\codeplug.h" 
#include "..\..\mainfrm.h"
#include "canmsgs.h"

extern CPpsApp    theApp;

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////
// CCannedMessages

IMPLEMENT_DYNCREATE(CCannedMessages, CPpsFormView)

CCannedMessages::CCannedMessages()
	: CPpsFormView(CCannedMessages::IDD)
{
	//{{AFX_DATA_INIT(CCannedMessages)
	m_szCanMsgZero = "";
	m_szCanMsgOne = "";
	m_szCanMsgTwo = "";
	m_szCanMsgThree = "";
	m_szCanMsgFour = "";
	m_szCanMsgFive = "";
	m_szCanMsgSix = "";
	m_szCanMsgSeven = "";
	m_szCanMsgEight = "";
	m_szCanMsgNine = "";
	m_szCanMsgTen = "";
	m_szCanMsgEleven = "";
	m_szCanMsgTwelve = "";
	m_szCanMsgThirteen = "";
	m_szCanMsgFourteen = "";
	m_szCanMsgFifteen = "";
	m_byDivFwdRev = 0;
	m_byDivIod = 0;
	m_bSendMsgEnable = FALSE;
	m_bEnableMultipleChoiceResponses = FALSE;
	m_byReplyAttemptLimit = 0;
	//}}AFX_DATA_INIT  
	m_nMaxNumMsgs = 12;
}

CCannedMessages::~CCannedMessages()
{
}

void CCannedMessages::DoDataExchange(CDataExchange* pDX)
{
	CPpsFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCannedMessages)
	DDX_Text(pDX, IDC_CAN_MSG0, m_szCanMsgZero);
	DDV_MaxChars(pDX, m_szCanMsgZero, 15);
	DDX_Text(pDX, IDC_CAN_MSG1, m_szCanMsgOne);
	DDV_MaxChars(pDX, m_szCanMsgOne, 15);
	DDX_Text(pDX, IDC_CAN_MSG2, m_szCanMsgTwo);
	DDV_MaxChars(pDX, m_szCanMsgTwo, 15);
	DDX_Text(pDX, IDC_CAN_MSG3, m_szCanMsgThree);
	DDV_MaxChars(pDX, m_szCanMsgThree, 15);
	DDX_Text(pDX, IDC_CAN_MSG4, m_szCanMsgFour);
	DDV_MaxChars(pDX, m_szCanMsgFour, 15);
	DDX_Text(pDX, IDC_CAN_MSG5, m_szCanMsgFive);
	DDV_MaxChars(pDX, m_szCanMsgFive, 15);
	DDX_Text(pDX, IDC_CAN_MSG6, m_szCanMsgSix);
	DDV_MaxChars(pDX, m_szCanMsgSix, 15);
	DDX_Text(pDX, IDC_CAN_MSG7, m_szCanMsgSeven);
	DDV_MaxChars(pDX, m_szCanMsgSeven, 15);
	DDX_Text(pDX, IDC_CAN_MSG8, m_szCanMsgEight);
	DDV_MaxChars(pDX, m_szCanMsgEight, 15);
	DDX_Text(pDX, IDC_CAN_MSG9, m_szCanMsgNine);
	DDV_MaxChars(pDX, m_szCanMsgNine, 15);
	DDX_Text(pDX, IDC_CAN_MSG10, m_szCanMsgTen);
	DDV_MaxChars(pDX, m_szCanMsgTen, 15);
	DDX_Text(pDX, IDC_CAN_MSG11, m_szCanMsgEleven);
	DDV_MaxChars(pDX, m_szCanMsgEleven, 15);
	DDX_Text(pDX, IDC_CAN_MSG12, m_szCanMsgTwelve);
	DDV_MaxChars(pDX, m_szCanMsgTwelve, 15);
	DDX_Text(pDX, IDC_CAN_MSG13, m_szCanMsgThirteen);
	DDV_MaxChars(pDX, m_szCanMsgThirteen, 15);
	DDX_Text(pDX, IDC_CAN_MSG14, m_szCanMsgFourteen);
	DDV_MaxChars(pDX, m_szCanMsgFourteen, 15);
	DDX_Text(pDX, IDC_CAN_MSG15, m_szCanMsgFifteen);
	DDV_MaxChars(pDX, m_szCanMsgFifteen, 15);
	DDX_Check(pDX, IDC_SENDMSGENABLE_CHECK, m_bSendMsgEnable);
	DDX_Check(pDX, IDC_MULTICHOICE_RESPONSE, m_bEnableMultipleChoiceResponses);
	DDX_Text(pDX, IDC_MCR_CNT_EDIT, m_byReplyAttemptLimit);
	DDV_MinMaxByte(pDX, m_byReplyAttemptLimit, 0, 255);
	//}}AFX_DATA_MAP
	
    // max div index dependent upon max number of allowed msgs
    DDX_Text(pDX, IDC_DIV_FWD_REV_EDIT, m_byDivFwdRev);
    DDV_MinMaxByte(pDX, m_byDivFwdRev, 0, (BYTE)m_nMaxNumMsgs);   
    DDX_Text(pDX, IDC_DIV_IOD_EDIT, m_byDivIod);
    DDV_MinMaxByte(pDX, m_byDivIod, 0, (BYTE)m_nMaxNumMsgs);	
	return;
}



BEGIN_MESSAGE_MAP(CCannedMessages, CPpsFormView)
	//{{AFX_MSG_MAP(CCannedMessages)
	ON_BN_CLICKED(IDC_CANNEDMSGS_DEFAULTS_BTN, OnAcceptDefaultsBtn)
	ON_BN_CLICKED(IDC_CANNEDMSGS_UNDO_BTN, OnUndoBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCannedMessages diagnostics

#ifdef _DEBUG
void CCannedMessages::AssertValid() const
{
	CPpsFormView::AssertValid();
}

void CCannedMessages::Dump(CDumpContext& dc) const
{
	CPpsFormView::Dump(dc);
}
#endif //_DEBUG


// locally scoped data
static  BOOL  bMultiChoiceResponseCapable = TRUE;
static  BOOL  bInfoOnDemandCapable = TRUE;


BOOL CCannedMessages::IsPresentable(void)
{  
  CPpsDoc   *pDoc = (CPpsDoc*)((CFrameWnd *)theApp.m_pMainWnd)->GetActiveDocument();

  CP_multi_response_capable.bSetFromDoc(&bMultiChoiceResponseCapable);
  CP_IOD_msg_capable.bSetFromDoc(&bInfoOnDemandCapable);

  // make this view always visible for the lab.
  if (theApp.IsLabUseOnly() == TRUE)
    return(TRUE); 
 
  // This view should not be visible with the PageFinder device
  // unless it is a multiple choice response model or an 
  // info on demand model. 
  if ((bMultiChoiceResponseCapable == FALSE) && (bInfoOnDemandCapable == FALSE))  
  {
    // else, if either option is currently set regardless of capable bit, let 
    // the user access it...
    //CP_IOD_msg_enable.bSetFromDoc(&m_bSendMsgEnable);
    //CP_mult_choice_enable.bSetFromDoc(&m_bEnableMultipleChoiceResponses);  
    //if ((m_bSendMsgEnable == TRUE) || (m_bEnableMultipleChoiceResponses == TRUE)) 
    //  return(TRUE);
    return(FALSE);  // not capable and not enabled...
  }
  return(TRUE);
}  	



////////////////////////////////////////////////////////////////////////////
//	OnInitialUpdate()
//  
//  Update DDX variables from document and resize frame around view.
//
void CCannedMessages::OnInitialUpdate(void)
{
  TRACE("CCannedMessages::OnInitialUpdate : CCannedMessages::IDD=%X\n", CCannedMessages::IDD);

  // Disable select fields if Mtel Affiliate PPS 
  #ifdef MTEL_AFFILIATE_PPS 
    // disable all canned message entries 
    EditFieldEnable(IDC_CAN_MSG0);
    EditFieldEnable(IDC_CAN_MSG1);
    EditFieldEnable(IDC_CAN_MSG2);
    EditFieldEnable(IDC_CAN_MSG3);
    EditFieldEnable(IDC_CAN_MSG4);
    EditFieldEnable(IDC_CAN_MSG5);
    EditFieldEnable(IDC_CAN_MSG6);
    EditFieldEnable(IDC_CAN_MSG7);
    EditFieldEnable(IDC_CAN_MSG8);
    EditFieldEnable(IDC_CAN_MSG9);
    EditFieldEnable(IDC_CAN_MSG10);
    EditFieldEnable(IDC_CAN_MSG11);
    EditFieldEnable(IDC_CAN_MSG12);
    EditFieldEnable(IDC_CAN_MSG13);
    EditFieldEnable(IDC_CAN_MSG14);
    EditFieldEnable(IDC_CAN_MSG15);
    // disable all other fields in the canned message group
    CStatic *pStatic = (CStatic *)GetDlgItem(IDC_DIVFWDREV_STATIC); ASSERT(pStatic != NULL);
    pStatic->EnableWindow(FALSE);
    pStatic = (CStatic *)GetDlgItem(IDC_DIVIOD_STATIC); ASSERT(pStatic != NULL);
    pStatic->EnableWindow(FALSE); 
    pStatic = (CStatic *)GetDlgItem(IDC_DIVIOD_2_STATIC); ASSERT(pStatic != NULL);
    pStatic->EnableWindow(FALSE);
    EditFieldEnable(IDC_DIV_FWD_REV_EDIT);
    EditFieldEnable(IDC_DIV_IOD_EDIT); 
    // disable the response count field
    pStatic = (CStatic *)GetDlgItem(IDC_NREPEAT_STATIC); ASSERT(pStatic != NULL);
    pStatic->EnableWindow(FALSE);
    EditFieldEnable(IDC_MCR_CNT_EDIT);
  #endif  // MTEL_AFFILIATE_PPS

  CPpsFormView::OnInitialUpdate();
  return;
}


#ifdef MTEL_AFFILIATE_PPS 
void CCannedMessages::EditFieldEnable(UINT nResourceId, BOOL bEnable)
{
  CEdit *pEdit = (CEdit *)GetDlgItem(nResourceId);
  ASSERT(pEdit != NULL);
  pEdit->EnableWindow(bEnable);
  return;
}
#endif


/////////////////////////////////////////////////////////////////////////////
//	OnUpdate()
//
void CCannedMessages::OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint)
{
  TRACE("\nCCannedMessages::OnUpdate : IDD=%X  LHintHI=%X  lHintLO=%X\n", 
  		CCannedMessages::IDD,  HIWORD(lHint), LOWORD(lHint));

  if (HIWORD(lHint))        		// user switched views, now active
    ResizeParentToFit(FALSE);
  
  m_nMaxNumMsgs = ((CP_Canned_Msgs.Length()) / (CP_canned_msg.Length()));

  if ((m_nMaxNumMsgs < 0) || (m_nMaxNumMsgs > 16))
    m_nMaxNumMsgs = 12;
  
  m_szCanMsgTwelve = "";
  m_szCanMsgThirteen = "";
  m_szCanMsgFourteen = "";
  m_szCanMsgFifteen = "";   

  CEdit *pEdit = NULL;
  CStatic *pStatic = NULL;
  CButton *pButton = NULL;
  
  #ifndef MTEL_AFFILIATE_PPS  
  // enable/disable canned msg field 12, 13, 14, and 15 based upon max 
  // number of canned msgs allowed...
  pEdit = (CEdit *)GetDlgItem(IDC_CAN_MSG12);
  ASSERT(pEdit != NULL);
  pEdit->EnableWindow(m_nMaxNumMsgs > 12);
  pStatic = (CStatic *)GetDlgItem(IDC_STATIC_12);
  ASSERT(pStatic != NULL);
  pStatic->EnableWindow(m_nMaxNumMsgs > 12);
  //
  pEdit = (CEdit *)GetDlgItem(IDC_CAN_MSG13);
  ASSERT(pEdit != NULL);
  pEdit->EnableWindow(m_nMaxNumMsgs > 13);
  pStatic = (CStatic *)GetDlgItem(IDC_STATIC_13);
  ASSERT(pStatic != NULL);
  pStatic->EnableWindow(m_nMaxNumMsgs > 13);
  //
  pEdit = (CEdit *)GetDlgItem(IDC_CAN_MSG14);
  ASSERT(pEdit != NULL);
  pEdit->EnableWindow(m_nMaxNumMsgs > 14); 
  pStatic = (CStatic *)GetDlgItem(IDC_STATIC_14);
  ASSERT(pStatic != NULL);
  pStatic->EnableWindow(m_nMaxNumMsgs > 14);
  // 
  pEdit = (CEdit *)GetDlgItem(IDC_CAN_MSG15);
  ASSERT(pEdit != NULL);
  pEdit->EnableWindow(m_nMaxNumMsgs > 15);
  pStatic = (CStatic *)GetDlgItem(IDC_STATIC_15);
  ASSERT(pStatic != NULL);
  pStatic->EnableWindow(m_nMaxNumMsgs > 15);
  #endif  // MTEL_AFFILIATE_PPS
  
  // enable/disable/hide fields based upon PMU model opts... 
  CPpsDoc *pDoc = GetDocument();
  BYTE byDeviceId = pDoc->GetDeviceId();
  BYTE byCpVersion = pDoc->GetCpVersion();
  CP_multi_response_capable.bSetFromDoc(&bMultiChoiceResponseCapable);
  CP_IOD_msg_capable.bSetFromDoc(&bInfoOnDemandCapable);
  //
  // *** Multiple Choice Response count field...
  BOOL bEnable = (((byDeviceId == PAGEFINDER) && (byCpVersion >= 0x0A)) ||
                  (byDeviceId > PAGEFINDER));
  pStatic = (CStatic *)GetDlgItem(IDC_NREPEAT_STATIC); ASSERT(pStatic != NULL); 
  if (bEnable)
    pStatic->ShowWindow(SW_SHOW);
  else
    pStatic->ShowWindow(SW_HIDE);
  //
  pEdit = (CEdit *)GetDlgItem(IDC_MCR_CNT_EDIT); ASSERT(pEdit != NULL);
  if (bEnable)
    pEdit->ShowWindow(SW_SHOW);
  else
    pEdit->ShowWindow(SW_HIDE); 
  
  // *** Info-On-Demand Fields...
  if ((byDeviceId >= PAGEFINDER) && (bInfoOnDemandCapable == FALSE))                                                 
  {
    pStatic = (CStatic *)GetDlgItem(IDC_DIVIOD_STATIC); ASSERT(pStatic != NULL);
    pStatic->ShowWindow(SW_HIDE); 
    pButton = (CButton *)GetDlgItem(IDC_SENDMSGENABLE_CHECK); ASSERT(pButton != NULL);
    pButton->ShowWindow(SW_HIDE);
  }
  else   // IOD capable...
  {
    pStatic = (CStatic *)GetDlgItem(IDC_DIVIOD_2_STATIC); ASSERT(pStatic != NULL);
    pStatic->ShowWindow(SW_HIDE);
  }

  SetFromDoc();
  CPpsFormView::OnUpdate(pSender, lHint, pHint);
}	


////////////////////////////////////////////////////////////////////////////
//  SetFromDoc()
//
void CCannedMessages::SetFromDoc(void)
{
  TRACE("\nCCannedMessages::SetFromDoc\n");
  CPpsDoc	*pDoc = GetDocument();
  BYTE		byTemp[16];
  int		nEos;
  
  // canned messages
  pDoc->Field(CP_Canned_Msgs.Offset(), 16, byTemp);
  nEos = (int)byTemp[15];
  if (nEos < 16) byTemp[nEos] = '\0'; else byTemp[0] = '\0';
  m_szCanMsgZero = (char *)byTemp;
  
  pDoc->Field(CP_Canned_Msgs.Offset()+16, 16, byTemp);
  nEos = (int)byTemp[15];
  if (nEos < 16) byTemp[nEos] = '\0'; else byTemp[0] = '\0';
  m_szCanMsgOne = (char *)byTemp;
  
  pDoc->Field(CP_Canned_Msgs.Offset()+32, 16, byTemp);
  nEos = (int)byTemp[15];
  if (nEos < 16) byTemp[nEos] = '\0'; else byTemp[0] = '\0';
  m_szCanMsgTwo = (char *)byTemp;
  
  pDoc->Field(CP_Canned_Msgs.Offset()+48, 16, byTemp);
  nEos = (int)byTemp[15];
  if (nEos < 16) byTemp[nEos] = '\0'; else byTemp[0] = '\0';
  m_szCanMsgThree = (char *)byTemp;
  
  pDoc->Field(CP_Canned_Msgs.Offset()+64, 16, byTemp);
  nEos = (int)byTemp[15];
  if (nEos < 16) byTemp[nEos] = '\0'; else byTemp[0] = '\0';
  m_szCanMsgFour = (char *)byTemp;
  
  pDoc->Field(CP_Canned_Msgs.Offset()+80, 16, byTemp);
  nEos = (int)byTemp[15];
  if (nEos < 16) byTemp[nEos] = '\0'; else byTemp[0] = '\0';
  m_szCanMsgFive = (char *)byTemp;
  
  pDoc->Field(CP_Canned_Msgs.Offset()+96, 16, byTemp);
  nEos = (int)byTemp[15];
  if (nEos < 16) byTemp[nEos] = '\0'; else byTemp[0] = '\0';
  m_szCanMsgSix = (char *)byTemp;
  
  pDoc->Field(CP_Canned_Msgs.Offset()+112, 16, byTemp);
  nEos = (int)byTemp[15];
  if (nEos < 16) byTemp[nEos] = '\0'; else byTemp[0] = '\0';
  m_szCanMsgSeven = (char *)byTemp;
  
  pDoc->Field(CP_Canned_Msgs.Offset()+128, 16, byTemp);
  nEos = (int)byTemp[15];
  if (nEos < 16) byTemp[nEos] = '\0'; else byTemp[0] = '\0';
  m_szCanMsgEight = (char *)byTemp;
  
  pDoc->Field(CP_Canned_Msgs.Offset()+144, 16, byTemp);
  nEos = (int)byTemp[15];
  if (nEos < 16) byTemp[nEos] = '\0'; else byTemp[0] = '\0';
  m_szCanMsgNine = (char *)byTemp;
  
  pDoc->Field(CP_Canned_Msgs.Offset()+160, 16, byTemp);
  nEos = (int)byTemp[15];
  if (nEos < 16) byTemp[nEos] = '\0'; else byTemp[0] = '\0';
  m_szCanMsgTen = (char *)byTemp;
  
  pDoc->Field(CP_Canned_Msgs.Offset()+176, 16, byTemp);
  nEos = (int)byTemp[15];
  if (nEos < 16) byTemp[nEos] = '\0'; else byTemp[0] = '\0';
  m_szCanMsgEleven = (char *)byTemp;
 
  if (m_nMaxNumMsgs > 12)
  {
    pDoc->Field(CP_Canned_Msgs.Offset()+192, 16, byTemp);
    nEos = (int)byTemp[15];
    if (nEos < 16) byTemp[nEos] = '\0'; else byTemp[0] = '\0';
    m_szCanMsgTwelve = (char *)byTemp;
  }
  
  if (m_nMaxNumMsgs > 13)
  {
    pDoc->Field(CP_Canned_Msgs.Offset()+208, 16, byTemp);
    nEos = (int)byTemp[15];
    if (nEos < 16) byTemp[nEos] = '\0'; else byTemp[0] = '\0';
    m_szCanMsgThirteen = (char *)byTemp;
  }
  
  if (m_nMaxNumMsgs > 14)
  {
    pDoc->Field(CP_Canned_Msgs.Offset()+224, 16, byTemp);
    nEos = (int)byTemp[15];
    if (nEos < 16) byTemp[nEos] = '\0'; else byTemp[0] = '\0';
    m_szCanMsgFourteen = (char *)byTemp;
  }
  
  if (m_nMaxNumMsgs > 15)
  {
    pDoc->Field(CP_Canned_Msgs.Offset()+240, 16, byTemp);
    nEos = (int)byTemp[15];
    if (nEos < 16) byTemp[nEos] = '\0'; else byTemp[0] = '\0';
    m_szCanMsgFifteen = (char *)byTemp; 
  }
  
  // CP_Div_Fwd_Rev
  CP_Div_Fwd_Rev.bySetFromDoc(&m_byDivFwdRev);
  
  // CP_Div_Iod
  CP_Div_Iod.bySetFromDoc(&m_byDivIod);
  
  // CP_IOD_msg_enable
  CP_IOD_msg_enable.bSetFromDoc(&m_bSendMsgEnable);
  
  // CP_Function1_L 
  CP_mult_choice_enable.bSetFromDoc(&m_bEnableMultipleChoiceResponses);  
  
  // CP_Reply_Attempt_Limit
  if (((pDoc->GetDeviceId() == PAGEFINDER) && (pDoc->GetCpVersion() >= 0x0A)) ||
      (pDoc->GetDeviceId() >= EXPANDAUR))
    CP_Reply_Attempt_Limit.bySetFromDoc(&m_byReplyAttemptLimit);

  UpdateData(FALSE);	// set controls data
}



////////////////////////////////////////////////////////////////////////////
//	SetToDoc()
//  
BOOL CCannedMessages::SetToDoc(void)
{
  TRACE("\nCCannedMessages::SetToDoc\n");
  CEdit 		*pEditDivFwdRev = NULL;
  CEdit 		*pEditDivIod = NULL;
  CPpsDoc		*pDoc = GetDocument();
  
  if (!(UpdateData(TRUE)))  // get controls data
    return FALSE;
  
  // canned message info 
  DoCannedMessageModify(IDC_CAN_MSG0, m_szCanMsgZero, 0); 
  DoCannedMessageModify(IDC_CAN_MSG1, m_szCanMsgOne, 16); 
  DoCannedMessageModify(IDC_CAN_MSG2, m_szCanMsgTwo, 32); 
  DoCannedMessageModify(IDC_CAN_MSG3, m_szCanMsgThree, 48); 
  DoCannedMessageModify(IDC_CAN_MSG4, m_szCanMsgFour, 64); 
  DoCannedMessageModify(IDC_CAN_MSG5, m_szCanMsgFive, 80); 
  DoCannedMessageModify(IDC_CAN_MSG6, m_szCanMsgSix, 96); 
  DoCannedMessageModify(IDC_CAN_MSG7, m_szCanMsgSeven, 112); 
  DoCannedMessageModify(IDC_CAN_MSG8, m_szCanMsgEight, 128); 
  DoCannedMessageModify(IDC_CAN_MSG9, m_szCanMsgNine, 144); 
  DoCannedMessageModify(IDC_CAN_MSG10, m_szCanMsgTen, 160); 
  DoCannedMessageModify(IDC_CAN_MSG11, m_szCanMsgEleven, 176); 
  
  if (m_nMaxNumMsgs > 12)
    DoCannedMessageModify(IDC_CAN_MSG12, m_szCanMsgTwelve, 192);
  if (m_nMaxNumMsgs > 13)
    DoCannedMessageModify(IDC_CAN_MSG13, m_szCanMsgThirteen, 208); 
  if (m_nMaxNumMsgs > 14)
    DoCannedMessageModify(IDC_CAN_MSG14, m_szCanMsgFourteen, 224); 
  if (m_nMaxNumMsgs > 15)
    DoCannedMessageModify(IDC_CAN_MSG15, m_szCanMsgFifteen, 240); 
  
  pEditDivFwdRev = (CEdit*)GetDlgItem(IDC_DIV_FWD_REV_EDIT);
  ASSERT(pEditDivFwdRev != NULL);
  pEditDivIod = (CEdit*)GetDlgItem(IDC_DIV_IOD_EDIT);
  ASSERT(pEditDivIod != NULL);

  // CP_Div_Fwd_Rev 
  if (pEditDivFwdRev->GetModify())
  { 
    if ((m_byDivFwdRev >= m_byDivIod) && 
        (m_byDivFwdRev != m_nMaxNumMsgs) &&
        (pEditDivIod->IsWindowEnabled() == TRUE))
    { 
      DoWarnUserOfInvalidEntry();
      ((CDialog *)this)->GotoDlgCtrl(pEditDivFwdRev);        
      return(FALSE);
    }                      
    pDoc->SetField(CP_Div_Fwd_Rev.Offset(), 1, &m_byDivFwdRev);
  }
  
  // CP_Div_Iod
  if (pEditDivIod->GetModify())
  {
    if ((m_byDivFwdRev >= m_byDivIod) && 
        (m_byDivFwdRev != m_nMaxNumMsgs) &&
        (pEditDivIod->IsWindowEnabled() == TRUE))
    {
      DoWarnUserOfInvalidEntry();
      ((CDialog *)this)->GotoDlgCtrl(pEditDivIod);        
      return(FALSE);
    }                      
    pDoc->SetField(CP_Div_Iod.Offset(), 1, &m_byDivIod);  
  }
  
  // CP_IOD_msg_enable
  CP_IOD_msg_enable.bSetToDoc(m_bSendMsgEnable);

  // CP_Function1_L
  CP_mult_choice_enable.bSetToDoc(m_bEnableMultipleChoiceResponses);  

  // CP_Reply_Attempt_Limit
  if (((pDoc->GetDeviceId() == PAGEFINDER) && (pDoc->GetCpVersion() >= 0x0A)) ||
      (pDoc->GetDeviceId() >= EXPANDAUR))
    CP_Reply_Attempt_Limit.bySetToDoc(m_byReplyAttemptLimit);

  return(TRUE);
}


static char *pErrorString = "The Fwd/Rev CMC index entry must be less than\n"
                            "the %s entry.\n\n"
                            "Exception: Set the Fwd/Rev CMC index entry to %d\n"
                            "which indicates that all CMC entries are Fwd/Rev CMC's.";
static char *pSubStr1 = "Fwd-Only/Info-on-Demand CMC index";
static char *pSubStr2 = "maximum number of canned messages";
//
void CCannedMessages::DoWarnUserOfInvalidEntry(void)
{
  char szTemp[300];
  BYTE byDeviceId = GetDocument()->GetDeviceId();
  BOOL bR50Enabled = TRUE;
  CP_reflex50_enable.bSetFromDoc(&bR50Enabled);      

  if ((byDeviceId >= PAGEFINDER) && (bInfoOnDemandCapable == FALSE))                                                 
    sprintf(szTemp, pErrorString, pSubStr2, m_nMaxNumMsgs);
  else
    sprintf(szTemp, pErrorString, pSubStr1, m_nMaxNumMsgs);
    
  AfxMessageBox (szTemp, MB_OK | MB_ICONHAND);
  return; 
}




///////////////////////////////////////////////////////////////////////////
// DoCannedMessageModify()
//
// Helper function for the SetToDoc function above.
//
void CCannedMessages::DoCannedMessageModify(int nResourceID, CString& rMsg, int nIndex)
{
  CPpsDoc		*pDoc = GetDocument();
  CEdit *pEdit = (CEdit *)GetDlgItem(nResourceID); 
  ASSERT(pEdit != NULL);
  BYTE	byTemp[16];
  
  // if modified, save the msg as max 15 characters followed by
  // the number of characters in the 16th byte.
  if (pEdit->GetModify())
  { 
    strncpy((char *)byTemp, (const char *)rMsg, 15);
    byTemp[15] = (BYTE)rMsg.GetLength(); 
    pDoc->SetField(CP_Canned_Msgs.Offset()+nIndex, 16, byTemp);
    pEdit->SetModify(FALSE);
  }
  return;
}

  



/////////////////////////////////////////////////////////////////////////////
// CCannedMessages message handlers




////////////////////////////////////////////////////////////////////////
//  OnAcceptDefaultsBtn()
//
//  This function initiates a move of data from select fields of the
//  CPpsDoc.m_byDefaultCodePlugArray to the CPpsDoc.m_byCodePlugArray.
//  The m_byDefaultCodePlugArray is loaded from file 'default.s19' upon
//  application execution.  If the file has not been found, then no 
//  default values are available.  
//
void CCannedMessages::OnAcceptDefaultsBtn()
{
  TRACE("\nCCannedMessages::OnAcceptDefaultsBtn\n"); 
  CPpsDoc	*pDoc = GetDocument(); 
   
  if (pDoc->SetFieldFromDefault(CP_Canned_Msgs.Offset(), CP_Canned_Msgs.Length()))
    return;
  if (pDoc->SetFieldFromDefault(CP_Div_Fwd_Rev.Offset(), CP_Div_Fwd_Rev.Length()))
    return;
  if (pDoc->SetFieldFromDefault(CP_Div_Iod.Offset(), CP_Div_Iod.Length()))
    return;
  if (pDoc->SetFieldFromDefault(CP_Function1_L.Offset(), CP_Function1_L.Length()))
    return;
  if (((pDoc->GetDeviceId() == PAGEFINDER) && (pDoc->GetCpVersion() >= 0x0A)) ||
      (pDoc->GetDeviceId() >= EXPANDAUR))
  {
    if (pDoc->SetFieldFromDefault(CP_Reply_Attempt_Limit.Offset(), CP_Reply_Attempt_Limit.Length()))
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
void CCannedMessages::OnArchived(void)
{
  CPpsDoc	*pDoc = GetDocument(); 
   
  if (pDoc->SetFieldFromArchived(CP_Canned_Msgs.Offset(), CP_Canned_Msgs.Length()))
    return;
  if (pDoc->SetFieldFromArchived(CP_Div_Fwd_Rev.Offset(), CP_Div_Fwd_Rev.Length()))
    return;
  if (pDoc->SetFieldFromArchived(CP_Div_Iod.Offset(), CP_Div_Iod.Length()))
    return;
  if (pDoc->SetFieldFromArchived(CP_Function1_L.Offset(), CP_Function1_L.Length()))
    return;
  if (((pDoc->GetDeviceId() == PAGEFINDER) && (pDoc->GetCpVersion() >= 0x0A)) ||
      (pDoc->GetDeviceId() >= EXPANDAUR))
  {
    if (pDoc->SetFieldFromArchived(CP_Reply_Attempt_Limit.Offset(), CP_Reply_Attempt_Limit.Length()))
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
void CCannedMessages::OnUndoBtn()
{
  TRACE("\nCCannedMessages::OnUndoBtn\n"); 
  CPpsDoc	*pDoc = GetDocument(); 

  if (pDoc->SetFieldFromOriginal(CP_Canned_Msgs.Offset(), CP_Canned_Msgs.Length()))
    return;
  if (pDoc->SetFieldFromOriginal(CP_Div_Fwd_Rev.Offset(), CP_Div_Fwd_Rev.Length()))
    return;
  if (pDoc->SetFieldFromOriginal(CP_Div_Iod.Offset(), CP_Div_Iod.Length()))
    return;
  if (pDoc->SetFieldFromOriginal(CP_Function1_L.Offset(), CP_Function1_L.Length()))
    return;
  if (((pDoc->GetDeviceId() == PAGEFINDER) && (pDoc->GetCpVersion() >= 0x0A)) ||
      (pDoc->GetDeviceId() >= EXPANDAUR))
  {
    if (pDoc->SetFieldFromOriginal(CP_Reply_Attempt_Limit.Offset(), CP_Reply_Attempt_Limit.Length()))
      return;
  }
  
  SetFromDoc();   // update this view
}


int CCannedMessages::OnPrintHelper(CStringArray &rStringArray, UINT nPageNumber, int nAvailableLines)
{    
   // if not enough lines available, wait for next page...
   const int nNumLinesRequired = 23;
   if (nAvailableLines < nNumLinesRequired)
     return(-1);    
   int nNumLinesBegin = rStringArray.GetSize();

   char    szTemp[200];
   
   rStringArray.Add("CANNED MESSAGES");
     sprintf(szTemp, "    (0) %s", (LPCTSTR)m_szCanMsgZero);
     rStringArray.Add(szTemp);
     sprintf(szTemp, "    (1) %s", (LPCTSTR)m_szCanMsgOne);
     rStringArray.Add(szTemp);
     sprintf(szTemp, "    (2) %s", (LPCTSTR)m_szCanMsgTwo);
     rStringArray.Add(szTemp);
     sprintf(szTemp, "    (3) %s", (LPCTSTR)m_szCanMsgThree);
     rStringArray.Add(szTemp);
     sprintf(szTemp, "    (4) %s", (LPCTSTR)m_szCanMsgFour);
     rStringArray.Add(szTemp);
     sprintf(szTemp, "    (5) %s", (LPCTSTR)m_szCanMsgFive);
     rStringArray.Add(szTemp);
     sprintf(szTemp, "    (6) %s", (LPCTSTR)m_szCanMsgSix);
     rStringArray.Add(szTemp);
     sprintf(szTemp, "    (7) %s", (LPCTSTR)m_szCanMsgSeven);
     rStringArray.Add(szTemp);
     sprintf(szTemp, "    (8) %s", (LPCTSTR)m_szCanMsgEight);
     rStringArray.Add(szTemp);
     sprintf(szTemp, "    (9) %s", (LPCTSTR)m_szCanMsgNine);
     rStringArray.Add(szTemp);
     sprintf(szTemp, "    (10) %s", (LPCTSTR)m_szCanMsgTen);
     rStringArray.Add(szTemp);
     sprintf(szTemp, "    (11) %s", (LPCTSTR)m_szCanMsgEleven);
     rStringArray.Add(szTemp); 
     
     if (m_nMaxNumMsgs > 12)
     {
       sprintf(szTemp, "    (12) %s", (LPCTSTR)m_szCanMsgTwelve);
       rStringArray.Add(szTemp);
     }
     if (m_nMaxNumMsgs > 13)
     {
       sprintf(szTemp, "    (13) %s", (LPCTSTR)m_szCanMsgThirteen);
       rStringArray.Add(szTemp);
     }
     if (m_nMaxNumMsgs > 14)
     {
       sprintf(szTemp, "    (14) %s", (LPCTSTR)m_szCanMsgFourteen);
       rStringArray.Add(szTemp);
     }
     if (m_nMaxNumMsgs > 15)
     {
       sprintf(szTemp, "    (15) %s", (LPCTSTR)m_szCanMsgFifteen);
       rStringArray.Add(szTemp);
     }
     
     sprintf(szTemp, "    Dividing Index Between Fwd/Rev CMC and Fwd-Only CMC:  %d", m_byDivFwdRev);
     rStringArray.Add(szTemp);
     sprintf(szTemp, "    Dividing Index Between Fwd-Only CMC and Info-On-Demand:  %d", m_byDivIod);
     rStringArray.Add(szTemp); 
     sprintf(szTemp, "    Enable Info-On-Demand:  %d", m_bSendMsgEnable); 
     rStringArray.Add(szTemp);
     sprintf(szTemp, "    Enable Multiple-Choice Responses:  %d", m_bEnableMultipleChoiceResponses);
     rStringArray.Add(szTemp);   
     
     CPpsDoc *pDoc = (CPpsDoc *)GetDocument();  ASSERT(pDoc);
     if (((pDoc->GetDeviceId() == PAGEFINDER) && (pDoc->GetCpVersion() >= 0x0A)) ||
         (pDoc->GetDeviceId() >= EXPANDAUR))
     {
       sprintf(szTemp, "    Number of Responses Allowed:  %d", m_byReplyAttemptLimit);
       rStringArray.Add(szTemp);
     } 
   
   rStringArray.Add("");
   return(rStringArray.GetSize() - nNumLinesBegin);
}


