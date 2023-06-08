//*************************************************************
//  History:    Date       Author               Comment
//              1996       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
// alerts.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\pps.h"
#include "..\..\ppsdoc.h"
#include "..\..\ppsview.h" 
#include "..\..\cpager\codeplug.h" 
#include "..\..\misc\spinbtn\spinbtn.h"
#include "alerts.h"
#include "progalrt.h"

extern CPpsApp    		theApp;

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAlerts

IMPLEMENT_DYNCREATE(CAlerts, CPpsFormView)

CAlerts::CAlerts()
	: CPpsFormView(CAlerts::IDD)
{	   
	//{{AFX_DATA_INIT(CAlerts)
	m_szAlertDuration = "";
	m_bEnableAllAlerts = FALSE;
	m_bEnableAudibleAlerts = FALSE;
	m_bEnableLowCellAlert = FALSE;
	m_bEnableMemoryFullAlert = FALSE;
	m_bEnableOutOfRangeIndicator = FALSE;
	m_bEnableVibrator = FALSE;
	m_nReminderAlertInterval = 0;
	m_nDisplayReadTimeout = 0;
	m_nReminderTimeoutPeriod = 0;
	m_fScrollRate = 0;
	m_nStandardAlertFrequency = 0;
	m_bSilentUpdate = FALSE;
	m_bOneWayPrompt = FALSE;
	m_bTwoWayPrompt = FALSE;
	m_bOutOfRangePrompt = FALSE;
	m_szOneWayPrompt = "";
	m_szTwoWayPrompt = "";
	m_szOutOfRangePrompt = "";
	//}}AFX_DATA_INIT
}

CAlerts::~CAlerts()
{
}

void CAlerts::DoDataExchange(CDataExchange* pDX)
{
	CPpsFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlerts)
	DDX_Text(pDX, IDC_REPEAT_EDIT, m_szAlertDuration);
	DDV_MaxChars(pDX, m_szAlertDuration, 7);
	DDX_Check(pDX, IDC_EN_ALL_CHECK, m_bEnableAllAlerts);
	DDX_Check(pDX, IDC_EN_AUDIBLE_CHECK, m_bEnableAudibleAlerts);
	DDX_Check(pDX, IDC_LOW_CELL_CHK, m_bEnableLowCellAlert);
	DDX_Check(pDX, IDC_OOR_CHECK, m_bEnableOutOfRangeIndicator);
	DDX_Check(pDX, IDC_VIBRATOR_CHK, m_bEnableVibrator);
	DDX_Text(pDX, IDC_UNREADMSG_INT_EDIT, m_nReminderAlertInterval);
	DDV_MinMaxInt(pDX, m_nReminderAlertInterval, 0, 1800);
	DDX_Text(pDX, IDC_INACTIVITY_EDIT, m_nDisplayReadTimeout);
	DDV_MinMaxInt(pDX, m_nDisplayReadTimeout, 0, 304);
	DDX_Text(pDX, IDC_ALERT_TIMEOUT_EDIT, m_nReminderTimeoutPeriod);
	DDV_MinMaxInt(pDX, m_nReminderTimeoutPeriod, 0, 1800);
	DDX_Text(pDX, IDC_SCROLLRATE_EDIT, m_fScrollRate);
	DDV_MinMaxFloat(pDX, m_fScrollRate, 0., 30.);
	DDX_Text(pDX, IDC_STD_FREQ_EDIT, m_nStandardAlertFrequency);
	DDX_Check(pDX, IDC_SILENTUPDATE_CHK, m_bSilentUpdate);
	DDX_Check(pDX, IDC_1WAYPROMPT_CHECK, m_bOneWayPrompt);
	DDX_Check(pDX, IDC_2WAYPROMPT_CHECK, m_bTwoWayPrompt);
	DDX_Check(pDX, IDC_OORPROMPT_CHECK, m_bOutOfRangePrompt);
	DDX_Text(pDX, IDC_1WAYPROMPT_EDIT, m_szOneWayPrompt);
	DDV_MaxChars(pDX, m_szOneWayPrompt, 20);
	DDX_Text(pDX, IDC_2WAYPROMPT_EDIT, m_szTwoWayPrompt);
	DDV_MaxChars(pDX, m_szTwoWayPrompt, 20);
	DDX_Text(pDX, IDC_OORPROMPT_EDIT, m_szOutOfRangePrompt);
	DDV_MaxChars(pDX, m_szOutOfRangePrompt, 20);
	//}}AFX_DATA_MAP
    
    if ((GetDocument()->GetDeviceId()) < EXPANDAUR)
    {
 	  DDX_Check(pDX, IDC_MSG_MEM_FULL_CHK, m_bEnableMemoryFullAlert);
    }
}



BEGIN_MESSAGE_MAP(CAlerts, CPpsFormView)
	//{{AFX_MSG_MAP(CAlerts)
	ON_BN_CLICKED(IDC_ALERTS_DEFAULTS_BTN, OnAcceptDefaultsBtn)
	ON_BN_CLICKED(IDC_ALERTS_UNDO_BTN, OnUndoBtn)
	ON_BN_CLICKED(IDC_PROG_ALERT_BTN, OnProgAlertBtn)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SPIN_BUTTON, OnWmSpinButton)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAlerts diagnostics

#ifdef _DEBUG
void CAlerts::AssertValid() const
{
	CPpsFormView::AssertValid();
}



void CAlerts::Dump(CDumpContext& dc) const
{
	CPpsFormView::Dump(dc);
}

#endif //_DEBUG



////////////////////////////////////////////////////////////////////////////
//	OnInitialUpdate()
//  
//  Update DDX variables from document and resize frame around view.
//
void CAlerts::OnInitialUpdate(void)
{
  TRACE("CAlerts::OnInitialUpdate : CAlerts::IDD=%X\n", CAlerts::IDD);

  // Load the Bitmap Button images
  VERIFY(m_AlertDurationUpBtn.AutoLoad(IDC_REPEAT_UP_BTN, this));
  VERIFY(m_AlertDurationDnBtn.AutoLoad(IDC_REPEAT_DWN_BTN, this));
  VERIFY(m_InactivityUpBtn.AutoLoad(IDC_INACTIVITY_UP_BTN, this));
  VERIFY(m_InactivityDnBtn.AutoLoad(IDC_INACTIVITY_DWN_BTN, this)); 
  VERIFY(m_ScrollRateUpBtn.AutoLoad(IDC_SCROLLRATE_UP_BTN, this));
  VERIFY(m_ScrollRateDnBtn.AutoLoad(IDC_SCROLLRATE_DWN_BTN, this));
  VERIFY(m_UnReadMsgRemindUpBtn.AutoLoad(IDC_UNREAD_UP_BTN, this));
  VERIFY(m_UnReadMsgRemindDnBtn.AutoLoad(IDC_UNREAD_DWN_BTN, this));
  VERIFY(m_AlertTimeOutUpBtn.AutoLoad(IDC_TIMEOUT_UP_BTN, this));
  VERIFY(m_AlertTimeOutDnBtn.AutoLoad(IDC_TIMEOUT_DWN_BTN, this));  
  VERIFY(m_StandardAlertFreqUpBtn.AutoLoad(IDC_STDFREQ_UP_BTN, this));  
  VERIFY(m_StandardAlertFreqDnBtn.AutoLoad(IDC_STDFREQ_DWN_BTN, this));  
  
  CPpsFormView::OnInitialUpdate();
  return;
}




/////////////////////////////////////////////////////////////////////////////
//	OnUpdate()
//
void CAlerts::OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint)
{
  TRACE("\nCAlerts::OnUpdate : IDD=%X  LHintHI=%X  lHintLO=%X\n", 
  		CAlerts::IDD,  HIWORD(lHint), LOWORD(lHint));

  if (HIWORD(lHint))        		// user switched views, now active
    ResizeParentToFit(FALSE);

  CPpsDoc	*pDoc = GetDocument(); 
  BYTE		byThisCPVersion = pDoc->GetCpVersion();
  BYTE 		byThisDeviceId = pDoc->GetDeviceId();
  // 
  m_bOneWayPrompt = FALSE;
  m_bTwoWayPrompt = FALSE;
  m_bOutOfRangePrompt = FALSE;
  m_szOneWayPrompt = "";
  m_szTwoWayPrompt = "";
  m_szOutOfRangePrompt = "";
  //
  // if the codeplug version is < 0x04, disable select display options  
  BOOL bEnable = ((byThisDeviceId > PAGEFINDER) ||
                  ((byThisDeviceId == PAGEFINDER) && (byThisCPVersion >= 0x04))); 
  CButton	*pButton = (CButton*)GetDlgItem(IDC_OORPROMPT_CHECK);
  ASSERT(pButton != NULL);
  pButton->EnableWindow(bEnable);
  CEdit *pEdit = (CEdit*)GetDlgItem(IDC_OORPROMPT_EDIT);
  ASSERT(pEdit != NULL);
  pEdit->EnableWindow(bEnable);
  //
  pButton = (CButton*)GetDlgItem(IDC_1WAYPROMPT_CHECK);
  ASSERT(pButton != NULL);
  pButton->EnableWindow(bEnable);
  pEdit = (CEdit*)GetDlgItem(IDC_1WAYPROMPT_EDIT);
  ASSERT(pEdit != NULL);
  pEdit->EnableWindow(bEnable);
  //
  pButton = (CButton*)GetDlgItem(IDC_2WAYPROMPT_CHECK);
  ASSERT(pButton != NULL);
  pButton->EnableWindow(bEnable);
  pEdit = (CEdit*)GetDlgItem(IDC_2WAYPROMPT_EDIT);
  ASSERT(pEdit != NULL);
  pEdit->EnableWindow(bEnable);
  
  // if the device is the Expandaur, disable the 'memory full' checkbox
  pButton = (CButton*)GetDlgItem(IDC_MSG_MEM_FULL_CHK);
  ASSERT(pButton != NULL);
  pButton->EnableWindow(byThisDeviceId < EXPANDAUR);
                                                  
  SetFromDoc();
  CPpsFormView::OnUpdate(pSender, lHint, pHint);
}	



///// **** Static Data from the codeplug description document

// ***								  
static char *szAlertDurationStrings[] = { "2", "12" };
static const nNumAlertDurationStrings = 2;
static BYTE byAlertDuration = 0x00;


// ***
static BYTE byReminderAlertIntervals[] = { 0x00, 0x0F, 0x17, 0x1E, 0x26, 0x4B, 0x71, 0xE1 };
static const int nNumReminderAlertIntervals = sizeof(byReminderAlertIntervals);
static int nIndexReminderAlertIntervals = 0;
static BOOL bModifiedReminderAlertIntervals = FALSE;


// *** 
static BYTE byReminderTimeoutPeriods[] = { 0x00, 0x0F, 0x17, 0x1E, 0x26, 0x4B, 0x71, 0xE1 };
static const int nNumReminderTimeoutPeriods = sizeof(byReminderTimeoutPeriods);
static int nIndexReminderTimeoutPeriods = 0;
static BOOL bModifiedReminderTimeoutPeriods = FALSE;


// *** 
static BYTE byReadTimeouts[] = { 0x02, 0x04, 0x08, 0x26 };
static const int nNumReadTimeouts = sizeof(byReadTimeouts);
static int nIndexReadTimeouts = 0;
static BOOL bModifiedReadTimeouts = FALSE;

// *** 
static BYTE byReadScrollTimes[] = { 0x04, 0x08, 0x10, 0x18, 0x20, 0x30, 0x40, 0x50, 0xF0 };
static const int nNumReadScrollTimes = sizeof(byReadScrollTimes);
static int nIndexReadScrollTimes = 0;
static BOOL bModifiedReadScrollTimes = FALSE;

// ***
static int nStandardAlertFreqs[] = { 3840, 3200, 2500, 1371, 1240 };
static const int nNumStandardAlertFreqs = 5; 
static int nIndexStandardAlertFreqs = 0;
static BOOL bModifiedStandardAlertFreqs = FALSE;


////////////////////////////////////////////////////////////////////////////
//  SetFromDoc()
//
void CAlerts::SetFromDoc(void)
{
  TRACE("\nCAlerts::SetFromDoc\n");
  CPpsDoc	*pDoc = GetDocument();
  int 	nTemp;
  BYTE 	byTemp;
  
  // programmable pleasing alert 
  for (int lcv = 0; lcv < sizeof(byProgAlert); ++lcv) 
    CP_FrequencyIndex.bySetFromDoc(&byProgAlert[lcv], lcv);

  // alert repeat duration 
  CP_alert_duration.bySetFromDoc(&byAlertDuration);
  ASSERT(byAlertDuration < nNumAlertDurationStrings);
  m_szAlertDuration = szAlertDurationStrings[(int)byAlertDuration];
    
  // alert checkbox enable options 
  CP_non_deaf_pager.bSetFromDoc(&m_bEnableAudibleAlerts);
  CP_enable_all_alerts.bSetFromDoc(&m_bEnableAllAlerts);
  if ((pDoc->GetDeviceId()) < EXPANDAUR)
    CP_memory_full.bSetFromDoc(&m_bEnableMemoryFullAlert);
  CP_vibrator_enabled.bSetFromDoc(&m_bEnableVibrator);
  CP_low_cell_alert.bSetFromDoc(&m_bEnableLowCellAlert);
  CP_silent_update.bSetFromDoc(&m_bSilentUpdate);
  m_bSilentUpdate = (!m_bSilentUpdate);  // logic is reversed (0 == enable)
  
  // display checkbox enable options
  if ((pDoc->GetDeviceId() <= PAGEFINDER) && (pDoc->GetCpVersion() < 0x04))
    CP_oor_indicator.bSetFromDoc(&m_bEnableOutOfRangeIndicator);
  else
  { 
    // get the checkbox data
    CP_oor_icon.bSetFromDoc(&m_bEnableOutOfRangeIndicator);
    CP_oor_prompt.bSetFromDoc(&m_bOutOfRangePrompt);
    CP_one_way_prompt.bSetFromDoc(&m_bOneWayPrompt);
    CP_two_way_prompt.bSetFromDoc(&m_bTwoWayPrompt);
    // get the edit box data
    CP_OutOfRange_Prompt.szSetFromDoc(m_szOutOfRangePrompt.GetBuffer(20), 20);
    m_szOutOfRangePrompt.ReleaseBuffer(20);
    CP_OneWay_Prompt.szSetFromDoc(m_szOneWayPrompt.GetBuffer(20), 20);
    m_szOneWayPrompt.ReleaseBuffer(20);
    CP_TwoWay_Prompt.szSetFromDoc(m_szTwoWayPrompt.GetBuffer(20), 20);
    m_szTwoWayPrompt.ReleaseBuffer(20);
  }
  
  // periodic information
  pDoc->Field(CP_Reminder_Alert_Interval.Offset(), 1, &byTemp);
  for (nTemp = 0; nTemp < nNumReminderAlertIntervals; ++nTemp)
  {
    if (byReminderAlertIntervals[nTemp] == byTemp)
      nIndexReminderAlertIntervals = nTemp;
  }
  bModifiedReminderAlertIntervals = FALSE;
  m_nReminderAlertInterval = (int)byTemp * 8;

  pDoc->Field(CP_Reminder_Timeout_Period.Offset(), 1, &byTemp);
  for (nTemp = 0; nTemp < nNumReminderTimeoutPeriods; ++nTemp)
  {
    if (byReminderTimeoutPeriods[nTemp] == byTemp)
      nIndexReminderTimeoutPeriods = nTemp;
  }
  bModifiedReminderTimeoutPeriods = FALSE;
  m_nReminderTimeoutPeriod = (int)byTemp * 8;

  pDoc->Field(CP_Read_Timeout.Offset(), 1, &byTemp);
  for (nTemp = 0; nTemp < nNumReadTimeouts; ++nTemp)
  {
    if (byReadTimeouts[nTemp] == byTemp)
      nIndexReadTimeouts = nTemp;
  }
  bModifiedReadTimeouts = FALSE;
  m_nDisplayReadTimeout = (int)byTemp * 8;

  pDoc->Field(CP_Read_Scroll_Time.Offset(), 1, &byTemp);
  for (nTemp = 0; nTemp < nNumReadScrollTimes; ++nTemp)
  {
    if (byReadScrollTimes[nTemp] == byTemp)
      nIndexReadScrollTimes = nTemp;
  }
  bModifiedReadScrollTimes = FALSE;
  m_fScrollRate = ((float)byTemp / 8);
  
  // standard alert freq
  pDoc->Field(CP_Alert_Frequency.Offset(), 1, &byTemp);
  if (((int)byTemp) < nNumStandardAlertFreqs)
    nIndexStandardAlertFreqs = (int)byTemp;
  bModifiedStandardAlertFreqs = FALSE;
  m_nStandardAlertFrequency = nStandardAlertFreqs[nIndexStandardAlertFreqs];

  UpdateData(FALSE);	// set controls data
}



////////////////////////////////////////////////////////////////////////////
//	SetToDoc()
//
BOOL CAlerts::SetToDoc(void)
{
  TRACE("\nCAlerts::SetToDoc\n");
  CPpsDoc		*pDoc = GetDocument();
  BYTE			byTemp;
  BOOL			bTemp;
  
  if (!(UpdateData(TRUE)))  // get controls data
    return FALSE;
  
  // the programmable pleasing alert is only saved upon
  // the user pressing the 'Save' button from the progammable alert dialog.
  
  // alert repeat duration
  CP_alert_duration.bySetToDoc(byAlertDuration);
    
  // checkbox enable options
  CP_non_deaf_pager.bSetToDoc(m_bEnableAudibleAlerts);
  CP_enable_all_alerts.bSetToDoc(m_bEnableAllAlerts);
  if ((pDoc->GetDeviceId()) < EXPANDAUR)
    CP_memory_full.bSetToDoc(m_bEnableMemoryFullAlert);
  CP_vibrator_enabled.bSetToDoc(m_bEnableVibrator);
  CP_low_cell_alert.bSetToDoc(m_bEnableLowCellAlert);  
  bTemp = (!m_bSilentUpdate);
  CP_silent_update.bSetToDoc(bTemp);  // logic is reversed between codeplug and screen
  
  // display checkbox enable options
  if ((pDoc->GetDeviceId() <= PAGEFINDER) && (pDoc->GetCpVersion() < 0x04))
    CP_oor_indicator.bSetToDoc(m_bEnableOutOfRangeIndicator);
  else
  { 
    // set the checkbox data
    CP_oor_icon.bSetToDoc(m_bEnableOutOfRangeIndicator);
    CP_oor_prompt.bSetToDoc(m_bOutOfRangePrompt);
    CP_one_way_prompt.bSetToDoc(m_bOneWayPrompt);
    CP_two_way_prompt.bSetToDoc(m_bTwoWayPrompt); 
    // set the edit box data
    CP_OutOfRange_Prompt.szSetToDoc((LPCTSTR)m_szOutOfRangePrompt, CP_OutOfRange_Prompt.Length());
    CP_OneWay_Prompt.szSetToDoc((LPCTSTR)m_szOneWayPrompt, CP_OneWay_Prompt.Length());
    CP_TwoWay_Prompt.szSetToDoc((LPCTSTR)m_szTwoWayPrompt, CP_TwoWay_Prompt.Length());
  }

  // periodic information 
  if (bModifiedReminderAlertIntervals)
    pDoc->SetField(CP_Reminder_Alert_Interval.Offset(), 1, 
                   &byReminderAlertIntervals[nIndexReminderAlertIntervals]);
  
  if (bModifiedReminderTimeoutPeriods)
    pDoc->SetField(CP_Reminder_Timeout_Period.Offset(), 1, 
                   &byReminderTimeoutPeriods[nIndexReminderTimeoutPeriods]);
  
  if (bModifiedReadTimeouts)
    pDoc->SetField(CP_Read_Timeout.Offset(), 1,
                   &byReadTimeouts[nIndexReadTimeouts]);
   
  if (bModifiedReadScrollTimes)
    pDoc->SetField(CP_Read_Scroll_Time.Offset(), 1, 
                   &byReadScrollTimes[nIndexReadScrollTimes]);

  // standard alert freq 
  if (bModifiedStandardAlertFreqs) 
  {  
    byTemp = (BYTE)nIndexStandardAlertFreqs;
    pDoc->SetField(CP_Alert_Frequency.Offset(), 1, &byTemp);
  }

  return(TRUE);
}




/////////////////////////////////////////////////////////////////////////////
// CAlerts message handlers




////////////////////////////////////////////////////////////////////////
//  OnAcceptDefaultsBtn()
//
//  This function initiates a move of data from select fields of the
//  CPpsDoc.m_byDefaultCodePlugArray to the CPpsDoc.m_byCodePlugArray.
//  The m_byDefaultCodePlugArray is loaded from file 'default.s19' upon
//  application execution.  If the file has not been found, then no 
//  default values are available.  
//
void CAlerts::OnAcceptDefaultsBtn()
{
  TRACE("\nCAlerts::OnAcceptDefaultsBtn\n"); 
  CPpsDoc	*pDoc = GetDocument(); 
  
  if (pDoc->SetFieldFromDefault(CP_Programmable_Pleasing_Alert.Offset(), CP_Programmable_Pleasing_Alert.Length()))
    return;  
  if (pDoc->SetFieldFromDefault(CP_Alert_Opts_1.Offset(), CP_Alert_Opts_1.Length()))
    return;  
  if (pDoc->SetFieldFromDefault(CP_Function1_L.Offset(), CP_Function1_L.Length()))
    return;  
  if (pDoc->SetFieldFromDefault(CP_Reminder_Alert_Interval.Offset(), CP_Reminder_Alert_Interval.Length()))
    return;  
  if (pDoc->SetFieldFromDefault(CP_Reminder_Timeout_Period.Offset(), CP_Reminder_Timeout_Period.Length()))
    return;  
  if (pDoc->SetFieldFromDefault(CP_Read_Timeout.Offset(), CP_Read_Timeout.Length()))
    return;  
  if (pDoc->SetFieldFromDefault(CP_Read_Scroll_Time.Offset(), CP_Read_Scroll_Time.Length()))
    return;    
  if (pDoc->SetFieldFromDefault(CP_Alert_Frequency.Offset(), CP_Alert_Frequency.Length())) 
    return;
  if (pDoc->SetFieldFromDefault(CP_Display_L.Offset(), CP_Display_L.Length())) 
    return;
  if ((pDoc->GetDeviceId() > PAGEFINDER) ||
      ((pDoc->GetDeviceId() == PAGEFINDER) && (pDoc->GetCpVersion() >= 0x04)))
  {
    if (pDoc->SetFieldFromDefault(CP_OutOfRange_Prompt.Offset(), CP_OutOfRange_Prompt.Length()))
      return;
    if (pDoc->SetFieldFromDefault(CP_OneWay_Prompt.Offset(), CP_OneWay_Prompt.Length()))
      return;
    if (pDoc->SetFieldFromDefault(CP_TwoWay_Prompt.Offset(), CP_TwoWay_Prompt.Length()))
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
void CAlerts::OnArchived(void)
{
  CPpsDoc	*pDoc = GetDocument(); 
   
  if (pDoc->SetFieldFromArchived(CP_Programmable_Pleasing_Alert.Offset(), CP_Programmable_Pleasing_Alert.Length()))
    return;  
  if (pDoc->SetFieldFromArchived(CP_Alert_Opts_1.Offset(), CP_Alert_Opts_1.Length()))
    return;  
  if (pDoc->SetFieldFromArchived(CP_Function1_L.Offset(), CP_Function1_L.Length()))
    return;  
  if (pDoc->SetFieldFromArchived(CP_Reminder_Alert_Interval.Offset(), CP_Reminder_Alert_Interval.Length()))
    return;  
  if (pDoc->SetFieldFromArchived(CP_Reminder_Timeout_Period.Offset(), CP_Reminder_Timeout_Period.Length()))
    return;  
  if (pDoc->SetFieldFromArchived(CP_Read_Timeout.Offset(), CP_Read_Timeout.Length()))
    return;  
  if (pDoc->SetFieldFromArchived(CP_Read_Scroll_Time.Offset(), CP_Read_Scroll_Time.Length()))
    return;    
  if (pDoc->SetFieldFromArchived(CP_Alert_Frequency.Offset(), CP_Alert_Frequency.Length())) 
    return;
  if (pDoc->SetFieldFromArchived(CP_Display_L.Offset(), CP_Display_L.Length())) 
    return;
  if ((pDoc->GetDeviceId() > PAGEFINDER) ||
      ((pDoc->GetDeviceId() == PAGEFINDER) && (pDoc->GetCpVersion() >= 0x04)))
  {
    if (pDoc->SetFieldFromArchived(CP_OutOfRange_Prompt.Offset(), CP_OutOfRange_Prompt.Length()))
      return;
    if (pDoc->SetFieldFromArchived(CP_OneWay_Prompt.Offset(), CP_OneWay_Prompt.Length()))
      return;
    if (pDoc->SetFieldFromArchived(CP_TwoWay_Prompt.Offset(), CP_TwoWay_Prompt.Length()))
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
void CAlerts::OnUndoBtn()
{
  TRACE("\nCAlerts::OnUndoBtn\n"); 
  CPpsDoc	*pDoc = GetDocument(); 
  
  if (pDoc->SetFieldFromOriginal(CP_Programmable_Pleasing_Alert.Offset(), CP_Programmable_Pleasing_Alert.Length()))
    return;  
  if (pDoc->SetFieldFromOriginal(CP_Alert_Opts_1.Offset(), CP_Alert_Opts_1.Length()))
    return;  
  if (pDoc->SetFieldFromOriginal(CP_Function1_L.Offset(), CP_Function1_L.Length()))
    return;  
  if (pDoc->SetFieldFromOriginal(CP_Reminder_Alert_Interval.Offset(), CP_Reminder_Alert_Interval.Length()))
    return;  
  if (pDoc->SetFieldFromOriginal(CP_Reminder_Timeout_Period.Offset(), CP_Reminder_Timeout_Period.Length()))
    return;  
  if (pDoc->SetFieldFromOriginal(CP_Read_Timeout.Offset(), CP_Read_Timeout.Length()))
    return;  
  if (pDoc->SetFieldFromOriginal(CP_Read_Scroll_Time.Offset(), CP_Read_Scroll_Time.Length()))
    return;  
  if (pDoc->SetFieldFromOriginal(CP_Alert_Frequency.Offset(), CP_Alert_Frequency.Length())) 
    return;
  if (pDoc->SetFieldFromOriginal(CP_Display_L.Offset(), CP_Display_L.Length())) 
    return;
  if ((pDoc->GetDeviceId() > PAGEFINDER) ||
      ((pDoc->GetDeviceId() == PAGEFINDER) && (pDoc->GetCpVersion() >= 0x04)))
  {
    if (pDoc->SetFieldFromOriginal(CP_OutOfRange_Prompt.Offset(), CP_OutOfRange_Prompt.Length()))
      return;
    if (pDoc->SetFieldFromOriginal(CP_OneWay_Prompt.Offset(), CP_OneWay_Prompt.Length()))
      return;
    if (pDoc->SetFieldFromOriginal(CP_TwoWay_Prompt.Offset(), CP_TwoWay_Prompt.Length()))
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
afx_msg LONG CAlerts::OnWmSpinButton(UINT wParam, LONG lParam)
{
  TRACE("\nCAlerts::OnWmSpinButton : nBtnID=%04X\n", wParam);
  
  int nAcceleration = LOWORD(lParam);
  
  if (!UpdateData(TRUE))
    return(0L);
  
  switch(wParam)
  {
    case IDC_REPEAT_UP_BTN :
        if (byAlertDuration < (nNumAlertDurationStrings -1))
        {
          byAlertDuration++;
          m_szAlertDuration = szAlertDurationStrings[byAlertDuration];
        }
        break; 

    case IDC_REPEAT_DWN_BTN :
        if (byAlertDuration)
        {
          byAlertDuration--;
          m_szAlertDuration = szAlertDurationStrings[byAlertDuration];
        }
        break; 

    case IDC_INACTIVITY_UP_BTN :
        if (nIndexReadTimeouts < (nNumReadTimeouts - 1))
        {
          nIndexReadTimeouts++;
          m_nDisplayReadTimeout = (int)(byReadTimeouts[nIndexReadTimeouts] * 8);
         bModifiedReadTimeouts = TRUE;
        }
        break; 

    case IDC_INACTIVITY_DWN_BTN :
        if (nIndexReadTimeouts)
        {
          nIndexReadTimeouts--;
          m_nDisplayReadTimeout = (int)(byReadTimeouts[nIndexReadTimeouts] * 8);
          bModifiedReadTimeouts = TRUE;
        }
        break; 

    case IDC_SCROLLRATE_UP_BTN :
        if (nIndexReadScrollTimes < (nNumReadScrollTimes - 1))
        {
          nIndexReadScrollTimes++;
          m_fScrollRate = ((float)byReadScrollTimes[nIndexReadScrollTimes] / 8);
          bModifiedReadScrollTimes = TRUE;
        }
        break; 

    case IDC_SCROLLRATE_DWN_BTN :
        if (nIndexReadScrollTimes)
        {
          nIndexReadScrollTimes--;
          m_fScrollRate = ((float)byReadScrollTimes[nIndexReadScrollTimes] / 8);
          bModifiedReadScrollTimes = TRUE;
        }
        break; 

    case IDC_UNREAD_UP_BTN :
        if (nIndexReminderAlertIntervals < (nNumReminderAlertIntervals - 1))
        {
          nIndexReminderAlertIntervals++;
          m_nReminderAlertInterval = (int)(byReminderAlertIntervals[nIndexReminderAlertIntervals] * 8);
          bModifiedReminderAlertIntervals = TRUE;
        }
        break; 

    case IDC_UNREAD_DWN_BTN :
        if (nIndexReminderAlertIntervals)
        {
          nIndexReminderAlertIntervals--;
          m_nReminderAlertInterval = (int)(byReminderAlertIntervals[nIndexReminderAlertIntervals] * 8);
          bModifiedReminderAlertIntervals = TRUE;
        }
        break; 

    case IDC_TIMEOUT_UP_BTN :
        if (nIndexReminderTimeoutPeriods < (nNumReminderTimeoutPeriods - 1))
        {
          nIndexReminderTimeoutPeriods++;
          m_nReminderTimeoutPeriod = (int)(byReminderTimeoutPeriods[nIndexReminderTimeoutPeriods] * 8);
          bModifiedReminderTimeoutPeriods = TRUE;
        }
        break; 

    case IDC_TIMEOUT_DWN_BTN :
        if (nIndexReminderTimeoutPeriods)
        {
          nIndexReminderTimeoutPeriods--;
          m_nReminderTimeoutPeriod = (int)(byReminderTimeoutPeriods[nIndexReminderTimeoutPeriods] * 8);
          bModifiedReminderTimeoutPeriods = TRUE;
        }
        break; 

    case IDC_STDFREQ_UP_BTN :
        // Note:  the order of the valid frequencies is from High to Low in the
        // Codeplug Description document.  This is opposite the order of the 
        // frequency list index (which is Low to High).  Consequently, treat the
        // up button as a decrement and the down button as an increment.
        if (nIndexStandardAlertFreqs)
        {
          nIndexStandardAlertFreqs--;
          m_nStandardAlertFrequency = nStandardAlertFreqs[nIndexStandardAlertFreqs];
          bModifiedStandardAlertFreqs = TRUE;
        }
        break; 

    case IDC_STDFREQ_DWN_BTN :
        if (nIndexStandardAlertFreqs < (nNumStandardAlertFreqs - 1))
        {
          nIndexStandardAlertFreqs++;
          m_nStandardAlertFrequency = nStandardAlertFreqs[nIndexStandardAlertFreqs];
          bModifiedStandardAlertFreqs = TRUE;
        }
        break; 

    default:
        break;
  }                 
  
  UpdateData(FALSE);
  return(0L);
}



int CAlerts::OnPrintHelper(CStringArray &rStringArray, UINT nPageNumber, int nAvailableLines)
{ 
   // wait for the next page if required...
   const int nNumLinesRequired = 22;  
   if (nAvailableLines < nNumLinesRequired)
     return(-1);    
   int nNumLinesBegin = rStringArray.GetSize();
     
   char    szTemp[200];
   CString szLocal;
   
   rStringArray.Add("ALERTS"); 
     rStringArray.Add("    Enable Alerts:");
       //
       sprintf(szTemp, "        Audible:  %d", m_bEnableAudibleAlerts); 
       rStringArray.Add(szTemp);
       //
       sprintf(szTemp, "        All Alerts:  %d", m_bEnableAllAlerts); 
       rStringArray.Add(szTemp);
       //
       if ((GetDocument()->GetDeviceId()) < EXPANDAUR)
       {
         sprintf(szTemp, "        Memory Full:  %d", m_bEnableMemoryFullAlert); 
         rStringArray.Add(szTemp);
       }
       //
       sprintf(szTemp, "        Vibrator:  %d", m_bEnableVibrator); 
       rStringArray.Add(szTemp);
       //
       sprintf(szTemp, "        Low Cell:  %d", m_bEnableLowCellAlert); 
       rStringArray.Add(szTemp);
       //
       sprintf(szTemp, "        Message Update:  %d", m_bSilentUpdate); 
       rStringArray.Add(szTemp);

     rStringArray.Add("    Standard Alerts:");
       //
       sprintf(szTemp, "        Alert Frequency:  %d", m_nStandardAlertFrequency);
       rStringArray.Add(szTemp);
       //
       sprintf(szTemp, "        Alert Duration:  %s", m_szAlertDuration);
       rStringArray.Add(szTemp);
       //
       sprintf(szTemp, "        Reminder Alert Interval:  %d", m_nReminderAlertInterval);
       rStringArray.Add(szTemp);
       //
       sprintf(szTemp, "        Reminder Alert Timeout:  %d", m_nReminderTimeoutPeriod);
       rStringArray.Add(szTemp); 
   
     rStringArray.Add("    Enable Display:"); 
       //
       sprintf(szTemp, "        Out-of-Range Indicator:  %d", m_bEnableOutOfRangeIndicator); 
       rStringArray.Add(szTemp);
       //
       sprintf(szTemp, "        Out-of-Range Prompt:  %d  %s", m_bOutOfRangePrompt, (LPCTSTR)m_szOutOfRangePrompt); 
       rStringArray.Add(szTemp);
       //
       sprintf(szTemp, "        One-Way Prompt:  %d  %s", m_bOneWayPrompt, (LPCTSTR)m_szOneWayPrompt); 
       rStringArray.Add(szTemp);
       //
       sprintf(szTemp, "        Two-Way Prompt:  %d  %s", m_bTwoWayPrompt, (LPCTSTR)m_szTwoWayPrompt); 
       rStringArray.Add(szTemp);
       
     rStringArray.Add("    Display Behaviour:");
       //
       sprintf(szTemp, "        Inactivity Time-Out:  %d", m_nDisplayReadTimeout);
       rStringArray.Add(szTemp);
       //
       sprintf(szTemp, "        Scroll Rate:  %f", m_fScrollRate);
       rStringArray.Add(szTemp);
     
   rStringArray.Add("");
   return(rStringArray.GetSize() - nNumLinesBegin);
}



void CAlerts::OnProgAlertBtn()
{
  CProgAlertDlg  dlg;
  
  if (theApp.GetThePager()->IsOperationInProgress())
    return;
  
  // if the user presses 'Save' then update the document
  if (dlg.DoModal(byProgAlert) != IDCANCEL) 
  {
    for (int lcv = 0; lcv < sizeof(byProgAlert); ++lcv) 
      CP_FrequencyIndex.bySetToDoc(byProgAlert[lcv], lcv);
  }
  
  return;
}
