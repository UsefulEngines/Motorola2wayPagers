//*************************************************************
//  History:    Date       Author               Comment
//              1996       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
// progalrt.cpp : implementation file
//

#include "stdafx.h" 
#include <mmsystem.h>    // for the SND_LOOP constant
#include "..\..\pps.h"
#include "progalrt.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgAlertDlg dialog


// ***  available sounds are stored as resources
static UINT Notes[] =  
  {                    // see file 'pps.rc2'
	IDSOUND_OFF,       // 0 
	IDSOUND_345HZ,     // 1
	IDSOUND_480HZ,     // 2
	IDSOUND_686HZ,     // 3
	IDSOUND_800HZ,     // 4
    IDSOUND_900HZ,     // 5
    IDSOUND_960HZ,     // 6
    IDSOUND_1125HZ,    // 7
    IDSOUND_1200HZ,    // 8
    IDSOUND_1300HZ,    // 9
    IDSOUND_1371HZ,    // 10
    IDSOUND_1551HZ,    // 11
    IDSOUND_1600HZ,    // 12
    /*
    IDSOUND_1920HZ,    // 13    The unit doesn't appear
    IDSOUND_2400HZ,    // 14    to be able to generate
    IDSOUND_2473HZ,    // 15    1920Hz thru 6400 Hz. If
    IDSOUND_2500HZ,    // 16    future releases enable 
    IDSOUND_2800HZ,    // 17    this capability, uncomment 
    IDSOUND_2900HZ,    // 18    here and also in file 
    IDSOUND_2997HZ,    // 19    pps.rc2.  
    IDSOUND_3095HZ,    // 20
    IDSOUND_3200HZ,    // 21
    IDSOUND_3840HZ,    // 22
    IDSOUND_4800HZ,    // 23
    IDSOUND_6400HZ,    // 24
    */
  }; 
//
static const int nNumNotes = (sizeof(Notes) / sizeof(UINT));



CProgAlertDlg::CProgAlertDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProgAlertDlg::IDD, pParent)
{
    pbyAlert = NULL;
    m_bAlertTimerActive = FALSE;
	//{{AFX_DATA_INIT(CProgAlertDlg)
	//}}AFX_DATA_INIT
}


void CProgAlertDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProgAlertDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProgAlertDlg, CDialog)
	//{{AFX_MSG_MAP(CProgAlertDlg)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_SAVE_BTN, OnSaveBtn)
	ON_BN_CLICKED(IDC_TEST_BTN, OnTestBtn)
	ON_BN_CLICKED(IDC_PROGALERT_HELP_BTN, OnProgalertHelpBtn)
	//}}AFX_MSG_MAP
    ON_WM_TIMER()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CProgAlertDlg message handlers


BOOL CProgAlertDlg::OnInitDialog()
{
	CScrollBar *pSB = NULL;

	CDialog::OnInitDialog();
	
    ASSERT(pbyAlert != NULL);  // used the wrong 'DoModal' call

	// init the scroll bars	
	pSB = (CScrollBar*)GetDlgItem(IDC_SCROLL_BEAT0);
	ASSERT(pSB != NULL);
	pSB->SetScrollRange(MIN_SCROLL, MAX_SCROLL);
	pSB->SetScrollPos(MAX_SCROLL - pbyAlert[0]);
	//
	pSB = (CScrollBar*)GetDlgItem(IDC_SCROLL_BEAT1);
	ASSERT(pSB != NULL);
	pSB->SetScrollRange(MIN_SCROLL, MAX_SCROLL);
	pSB->SetScrollPos(MAX_SCROLL - pbyAlert[1]);
	//
	pSB = (CScrollBar*)GetDlgItem(IDC_SCROLL_BEAT2);
	ASSERT(pSB != NULL);
	pSB->SetScrollRange(MIN_SCROLL, MAX_SCROLL);
	pSB->SetScrollPos(MAX_SCROLL - pbyAlert[2]);
	//
	pSB = (CScrollBar*)GetDlgItem(IDC_SCROLL_BEAT3);
	ASSERT(pSB != NULL);
	pSB->SetScrollRange(MIN_SCROLL, MAX_SCROLL);
	pSB->SetScrollPos(MAX_SCROLL - pbyAlert[3]);
	//
	pSB = (CScrollBar*)GetDlgItem(IDC_SCROLL_BEAT4);
	ASSERT(pSB != NULL);
	pSB->SetScrollRange(MIN_SCROLL, MAX_SCROLL);
	pSB->SetScrollPos(MAX_SCROLL - pbyAlert[4]);
	//
	pSB = (CScrollBar*)GetDlgItem(IDC_SCROLL_BEAT5);
	ASSERT(pSB != NULL);
	pSB->SetScrollRange(MIN_SCROLL, MAX_SCROLL);
	pSB->SetScrollPos(MAX_SCROLL - pbyAlert[5]);
	//
	pSB = (CScrollBar*)GetDlgItem(IDC_SCROLL_BEAT6);
	ASSERT(pSB != NULL);
	pSB->SetScrollRange(MIN_SCROLL, MAX_SCROLL);
	pSB->SetScrollPos(MAX_SCROLL - pbyAlert[6]);
	//
	pSB = (CScrollBar*)GetDlgItem(IDC_SCROLL_BEAT7);
	ASSERT(pSB != NULL);
	pSB->SetScrollRange(MIN_SCROLL, MAX_SCROLL);
	pSB->SetScrollPos(MAX_SCROLL - pbyAlert[7]);
	//
	pSB = (CScrollBar*)GetDlgItem(IDC_SCROLL_BEAT8);
	ASSERT(pSB != NULL);
	pSB->SetScrollRange(MIN_SCROLL, MAX_SCROLL);
	pSB->SetScrollPos(MAX_SCROLL - pbyAlert[8]);
	//
	pSB = (CScrollBar*)GetDlgItem(IDC_SCROLL_BEAT9);
	ASSERT(pSB != NULL);
	pSB->SetScrollRange(MIN_SCROLL, MAX_SCROLL);
	pSB->SetScrollPos(MAX_SCROLL - pbyAlert[9]);
	//
	pSB = (CScrollBar*)GetDlgItem(IDC_SCROLL_BEAT10);
	ASSERT(pSB != NULL);
	pSB->SetScrollRange(MIN_SCROLL, MAX_SCROLL);
	pSB->SetScrollPos(MAX_SCROLL - pbyAlert[10]);
	//
	pSB = (CScrollBar*)GetDlgItem(IDC_SCROLL_BEAT11);
	ASSERT(pSB != NULL);
	pSB->SetScrollRange(MIN_SCROLL, MAX_SCROLL);
	pSB->SetScrollPos(MAX_SCROLL - pbyAlert[11]);
	//
	pSB = (CScrollBar*)GetDlgItem(IDC_SCROLL_BEAT12);
	ASSERT(pSB != NULL);
	pSB->SetScrollRange(MIN_SCROLL, MAX_SCROLL);
	pSB->SetScrollPos(MAX_SCROLL - pbyAlert[12]);
	//
	pSB = (CScrollBar*)GetDlgItem(IDC_SCROLL_BEAT13);
	ASSERT(pSB != NULL);
	pSB->SetScrollRange(MIN_SCROLL, MAX_SCROLL);
	pSB->SetScrollPos(MAX_SCROLL - pbyAlert[13]);
	//
	pSB = (CScrollBar*)GetDlgItem(IDC_SCROLL_BEAT14);
	ASSERT(pSB != NULL);
	pSB->SetScrollRange(MIN_SCROLL, MAX_SCROLL);
	pSB->SetScrollPos(MAX_SCROLL - pbyAlert[14]);
	//
	pSB = (CScrollBar*)GetDlgItem(IDC_SCROLL_BEAT15);
	ASSERT(pSB != NULL);
	pSB->SetScrollRange(MIN_SCROLL, MAX_SCROLL);
	pSB->SetScrollPos(MAX_SCROLL - pbyAlert[15]);
	//
		
	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CProgAlertDlg::OnOK()
{                              
  // dummy OnOK function -- do not call CDialog::OnOK()
  // overridden in order to prevent accidental exits from dialog
  // when user presses ENTER.  
  TRACE("\nCProgAlertDlg::OnOK\n");
}


void CProgAlertDlg::OnCancel()
{
  StopAlertTest();
  CDialog::OnCancel();
}



void CProgAlertDlg::OnSaveBtn()
{
  StopAlertTest();
  CDialog::OnOK();
  return;	
}


void CProgAlertDlg::OnProgalertHelpBtn()
{
  AfxGetApp()->WinHelp(HID_BASE_RESOURCE + IDD);
}



void CProgAlertDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
  if ((pScrollBar == NULL) || (nSBCode == SB_ENDSCROLL))  
  {
    CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
    return;
  }
  
  int nCurPos = pScrollBar->GetScrollPos();
  int nCtrlId = pScrollBar->GetDlgCtrlID();

  ASSERT((nCtrlId >= IDC_SCROLL_BEAT0) && (nCtrlId <= IDC_SCROLL_BEAT15));
  ASSERT((nCtrlId - IDC_SCROLL_BEAT0 >= 0) && (nCtrlId - IDC_SCROLL_BEAT0 <= NUM_BEATS));

  // use the scroll bar resource ID as a pbyAlert array index
  nCtrlId = nCtrlId - IDC_SCROLL_BEAT0;
     
  switch(nSBCode)
  { 
    // NOTE:  scroll bar perspective is opposite high/low note perspective.
    case SB_BOTTOM:
      pScrollBar->SetScrollPos(MAX_SCROLL);
      pbyAlert[nCtrlId] = (BYTE)MIN_SCROLL; 
      TRACE("\nCProgAlertDlg::OnVScroll : SB_BOTTOM : %d\n", pbyAlert[nCtrlId]);
      break;
  
    case SB_TOP:
      pScrollBar->SetScrollPos(MIN_SCROLL); 
      pbyAlert[nCtrlId] = (BYTE)MAX_SCROLL;
      TRACE("\nCProgAlertDlg::OnVScroll : SB_TOP : %d\n", pbyAlert[nCtrlId]);
      break;
  
    case SB_LINEDOWN:
      if (nCurPos >= MAX_SCROLL)
        break; 
      nCurPos++;
      pScrollBar->SetScrollPos(nCurPos);
      pbyAlert[nCtrlId] = (BYTE)(MAX_SCROLL - nCurPos);
      TRACE("\nCProgAlertDlg::OnVScroll : SB_LINEDOWN : %d\n", pbyAlert[nCtrlId]);
      break;
      
    case SB_LINEUP:
      if (nCurPos <= MIN_SCROLL)
        break;                        
      nCurPos--;
      pScrollBar->SetScrollPos(nCurPos);
      pbyAlert[nCtrlId] = (BYTE)(MAX_SCROLL - nCurPos);
      TRACE("\nCProgAlertDlg::OnVScroll : SB_LINEUP : %d\n", pbyAlert[nCtrlId]);
      break;
      
    case SB_PAGEDOWN:
      if ((nCurPos + 4) > MAX_SCROLL)
        break; 
      nCurPos += 4;
      pScrollBar->SetScrollPos(nCurPos);
      pbyAlert[nCtrlId] = (BYTE)(MAX_SCROLL - nCurPos);
      TRACE("\nCProgAlertDlg::OnVScroll : SB_PAGEDOWN : %d\n", pbyAlert[nCtrlId]);
      break;
      
    case SB_PAGEUP:
      if ((nCurPos - 4) < MIN_SCROLL)
        break;
      nCurPos -= 4;
      pScrollBar->SetScrollPos(nCurPos);
      pbyAlert[nCtrlId] = (BYTE)(MAX_SCROLL - nCurPos);
      TRACE("\nCProgAlertDlg::OnVScroll : SB_PAGEUP : %d\n", pbyAlert[nCtrlId]);
      break;
      
    case SB_THUMBPOSITION:
    case SB_THUMBTRACK:
      if ((nPos >= MIN_SCROLL) && (nPos <= MAX_SCROLL)) 
      {
        pScrollBar->SetScrollPos(nPos);
        pbyAlert[nCtrlId] = (BYTE)(MAX_SCROLL - nPos);
        TRACE("\nCProgAlertDlg::OnVScroll : SB_THUMBxxx : %d\n", pbyAlert[nCtrlId]);
      }
      break;
    
    default:
      break;
  }
  
  // play the note as the user scrolls
  if ((!m_bAlertTimerActive) && (CBasicSound::IsSoundDriverEnabled()))
  {
    TRACE("\nCProgAlertDlg::OnVScroll : Playing Notes[%d]\n", pbyAlert[nCtrlId]); 
    ASSERT(pbyAlert[nCtrlId] < nNumNotes);
    if (sndScroll.Load(Notes[(pbyAlert[nCtrlId])]))
      sndScroll.Play();
  }
      
  return;  
}



void CProgAlertDlg::OnTestBtn()
{ 
  int lcv;
  
  if (m_bAlertTimerActive)    // ignore quick button presses
    return;
   
  if (CBasicSound::IsSoundDriverEnabled() == FALSE)
  {
	AfxMessageBox("First enable a sound driver via the Control Panel.");
	return;
  }
      
  // load the notes  
  for (lcv = 0; lcv < NUM_BEATS; ++lcv)
  { 
    TRACE("\nCProgAlertDlg::OnTestBtn : index=%d : FreqIndex=%d\n", lcv, pbyAlert[lcv]);
    if (pbyAlert[lcv] < nNumNotes)
    {
      if ((sndTest[lcv].Load(Notes[(pbyAlert[lcv])])) == FALSE)
      {
        StopAlertTest();
	    AfxMessageBox("Unable to load sound resources.");
	    return;
	  } 
	}
  } 
  
  // initiate first sound, remainder will play upon receipt of timer msgs
  if (sndTest[0].Play(SND_LOOP) == FALSE) 
  { 
    StopAlertTest();
    return;
  }

  if (!SetTimer(ALERT_TIMER_ID, ALERT_TIMER_TIME, NULL))
  {
    StopAlertTest();
    return;  // failed to acquire timer
  }
  
  m_bAlertTimerActive = TRUE;
      
  return;
}




void CProgAlertDlg::OnTimer(UINT nIDEvent)
{   
  TRACE("\nAlerts::OnTimer\n");  
  ASSERT(nIDEvent == ALERT_TIMER_ID);
  static int nSndIndex = 1;    
  
  CWnd::OnTimer(nIDEvent);

  if (!m_bAlertTimerActive)    // ignore any late messages
    return;
  
  // if end of sndTest array, stop test
  if (nSndIndex >= NUM_BEATS)
  {
    StopAlertTest();
    nSndIndex = 1;
    return; 
  }  

  // play the next sound if different from currently playing sound 
  if (sndTest[nSndIndex].GetResourceId() != sndTest[nSndIndex-1].GetResourceId()) 
  { 
    if (sndTest[nSndIndex].Play(SND_LOOP) == FALSE)
    {
      StopAlertTest();
      nSndIndex = 1;
      return;
    } 
  }  
  nSndIndex++;        
  return;
}

 


void CProgAlertDlg::StopAlertTest(void)
{                         
  CBasicSound::Stop();
  if (m_bAlertTimerActive)
    KillTimer(ALERT_TIMER_ID);
  for (int lcv = 0; lcv < NUM_BEATS; ++lcv)
    sndTest[lcv].ReleaseSound(); 
  sndScroll.ReleaseSound();
  m_bAlertTimerActive = FALSE;
  return;
}


