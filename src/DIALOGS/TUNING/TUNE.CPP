//*************************************************************
//  History:    Date       Author               Comment
//              1996       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
// tune.cpp : implementation file
//

#include "stdafx.h"
#include <math.h>

#include "..\..\pps.h"
#include "..\..\ppsdoc.h"
#include "..\..\ppsview.h"
#include "..\..\cpager\ciooper.h" 
#include "..\..\cpager\pegasus.h"
#include "..\..\cpager\codeplug.h"
#include "..\..\mainfrm.h"
#include "tune.h" 
#include "hpmda.h"
#include "..\..\views\protocol\protocol.h"
#include "..\..\views\proto50\proto50.h"
#include "..\..\w16gpib\w16gpib.h"


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


#ifndef DISABLE_TUNING    // conditional compilation of this file


extern CPpsApp    		theApp;
extern CWin16Gpib		theGpibPort;
extern CHpMda			theHpMda;		

 
////////////////////////////////////////////////////////////////
//  Test Type Database
//
//  The test mode protocol is dependent upon the Pegasus
//  Factory Download/Test Document (AMSD-TX72_SRE_95K0021).
//  Occassionaly, this document changes.  This small data-base
//  is designed to account for such changes.  Specifically, the
//  test_numbers's have changed between builds of the Pegasus family
//  of pagers.  Based upon the Pager_ID and the Codeplug_Version,
//  this database returns the correct TestId number.  The database
//  might be extended to store more information as needed. 
// 
typedef struct 
{ 
  BYTE		byPagerId;
  BYTE 		byCodeplugVersion;
  BYTE		byTestNumber;
} TestRecord;


class TestType
{
  private:
    int			nNumRecords;
    TestRecord	*pRecords;
  public:
    TestType(TestRecord *pRecs, int nNumRecs);
    BYTE	TestNumber(void);  
  private:
    int  	Search(void);
    CPpsDoc *GetTheDocument(void);
};  // TestType


TestType::TestType(TestRecord *pRecs, int nNumRecs) 
{ 
  pRecords = pRecs; 
  nNumRecords = nNumRecs; 
}

BYTE TestType::TestNumber(void)    
{ 
  return(pRecords[(Search())].byTestNumber); 
}

int TestType::Search(void)           
{                        		    
  for (int lcv = (nNumRecords - 1); lcv >= 0; lcv--)
  { 
    ASSERT(pRecords != NULL);
    if (pRecords[lcv].byPagerId < (GetTheDocument()->GetDeviceId()))
      return(lcv);
    if ((pRecords[lcv].byPagerId <= (GetTheDocument()->GetDeviceId())) &&
        (pRecords[lcv].byCodeplugVersion <= (GetTheDocument()->GetCpVersion())))
       return(lcv);
  }
  return(nNumRecords-1);   // default to the latest entry if not found
} 


CPpsDoc *TestType::GetTheDocument(void)
{
  #ifdef _DEBUG
    CPpsDoc *pDoc = (CPpsDoc*)((CFrameWnd *)theApp.m_pMainWnd)->GetActiveDocument();
    ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(CPpsDoc)));
    return(pDoc);
  #else
    return((CPpsDoc*)((CFrameWnd *)theApp.m_pMainWnd)->GetActiveDocument());
  #endif
}


 
#define TESTTYPE(name) TestType name##(##name##_REC, (sizeof(##name##_REC)/sizeof(TestRecord)));

// *** PAGER *** CP_VER *** TEST_ID ***

static TestRecord ID_RefOscTest_REC[] =
{ { PEGASUS,    0x01, 0x08 },
  { PAGEFINDER, 0x04, 0x05 },
  { PAGEFINDER, 0x05, 0x05 } };
TESTTYPE( ID_RefOscTest )

static TestRecord ID_SilentCarrierTest_REC[] =
{ { PEGASUS,    0x01, 0x0F },
  { PAGEFINDER, 0x04, 0x07 },
  { PAGEFINDER, 0x05, 0x07 } };
TESTTYPE( ID_SilentCarrierTest )

static TestRecord ID_BalanceLowTest_REC[] =
{ { PEGASUS,    0x01, 0x10 },
  { PAGEFINDER, 0x04, 0x08 },
  { PAGEFINDER, 0x05, 0x08 } };
TESTTYPE( ID_BalanceLowTest )

static TestRecord ID_BalanceHighTest_REC[] =
{ { PEGASUS,    0x01, 0x11 },
  { PAGEFINDER, 0x04, 0x09 },
  { PAGEFINDER, 0x05, 0x09 } };
TESTTYPE( ID_BalanceHighTest )

static TestRecord ID_SplatterFilterTest_REC[] =
{ { PEGASUS,    0x01, 0x1E },
  { PAGEFINDER, 0x04, 0x11 },
  { PAGEFINDER, 0x05, 0x11 } };
TESTTYPE( ID_SplatterFilterTest )

static TestRecord ID_ReceiverOnTest_REC[] =
{ { PEGASUS,    0x01, 0x00 },
  { PAGEFINDER, 0x04, 0x00 },
  { PAGEFINDER, 0x05, 0x00 } };
TESTTYPE( ID_ReceiverOnTest )

static TestRecord ID_TransmitterOnTest_REC[] =
{ { PEGASUS,    0x01, 0x04 },
  { PAGEFINDER, 0x04, 0x02 },
  { PAGEFINDER, 0x05, 0x02 } };
TESTTYPE( ID_TransmitterOnTest )

static TestRecord ID_CancelPreviousTest_REC[] =
{ { PEGASUS,    0x01, 0xFF },
  { PAGEFINDER, 0x04, 0xFF },
  { PAGEFINDER, 0x05, 0xFF } };
TESTTYPE( ID_CancelPreviousTest )

static TestRecord ID_ExitTestMode_REC[] =
{ { PEGASUS,    0x01, 0x1F },
  { PAGEFINDER, 0x04, 0x12 },
  { PAGEFINDER, 0x05, 0x12 } };
TESTTYPE( ID_ExitTestMode )

// Note:  This field does not contain test record information
// but rather an indication of whether the current pager may
// be dynamically tuned or not.  If not, the PT_GRP visual
// elements are disabled when the user presses Start Test.
// The user must then press Stop Test in order to make any
// tuning parameter modifications.
static TestRecord IS_DynamicTuningAllowed_REC[] =
{ { PEGASUS,    0x01, 0x00 },
  { PAGEFINDER, 0x04, 0x01 },
  { PAGEFINDER, 0x05, 0x01 } };
TESTTYPE( IS_DynamicTuningAllowed )






/////////////////////////////////////////////////////////////////////////////
// CTheTuningDlg dialog

int CTheTuningDlg::DoModal(BYTE byTuningTest)
{ 
  m_byTestId = byTuningTest;

  return(CDialog::DoModal());
}



CTheTuningDlg::CTheTuningDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTheTuningDlg::IDD, pParent)
{
   m_nCenterFreqAdjust = 0;
   m_byModBalance = 0;
   m_byNeg2400deviation = 0;
   m_byNeg800deviation = 0;
   m_byPos2400deviation = 0;
   m_byPos800deviation = 0;
   m_byRefOsc = 0;
   m_byNextAutoTuneOperation = NONE;
   m_bAutoTuneInProgress = FALSE;
   //{{AFX_DATA_INIT(CTheTuningDlg)
    m_dCenterFreq = 0;
    m_szModulationRate = "";
	//}}AFX_DATA_INIT
}


////////////////////////////////////////////////////////////////////////////
//  DoDataExchange()
//
//  Portions of this routine deviate from normal MFC DDX/DDV functionality.
//  The CNumericEdit::DDX_HexText routine is used to translate user input
//  as hex input.
//
void CTheTuningDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   
   //{{AFX_DATA_MAP(CTheTuningDlg)
   DDX_Text(pDX, IDC_CENTERFRQ_EDIT, m_dCenterFreq);
   DDV_MinMaxDouble(pDX, m_dCenterFreq, 896.0125, 941.80625);
   DDX_Text(pDX, IDC_MODRATE_EDIT, m_szModulationRate);
   DDV_MaxChars(pDX, m_szModulationRate, 20);
   //}}AFX_DATA_MAP 

   // Use the custom DDX routine for these variables.
   m_RefOscEdit.DDX_HexText(pDX, &m_byRefOsc); 
   m_ModBalEdit.DDX_HexText(pDX, &m_byModBalance); 
   m_Pos2400Edit.DDX_HexText(pDX, &m_byPos2400deviation); 
   m_Pos800Edit.DDX_HexText(pDX, &m_byPos800deviation); 
   m_Neg800Edit.DDX_HexText(pDX, &m_byNeg800deviation); 
   m_Neg2400Edit.DDX_HexText(pDX, &m_byNeg2400deviation); 
    
   return;
}


BEGIN_MESSAGE_MAP(CTheTuningDlg, CDialog)
	//{{AFX_MSG_MAP(CTheTuningDlg)
	ON_BN_CLICKED(IDC_TUNING_ACCEPT, OnAccept)
	ON_BN_CLICKED(IDC_STOP_TEST, OnStopTest)
	ON_BN_CLICKED(IDC_TUNING_UNDO, OnUndoBtn)
	ON_BN_CLICKED(IDC_TUNING_DEFAULT, OnUseDefaultBtn)
	ON_WM_RBUTTONDBLCLK()
	ON_BN_CLICKED(ID_TUNINGDLG_HELP, OnTuningHelp)
	ON_BN_CLICKED(IDC_START_TEST, OnStartTest)
	ON_BN_CLICKED(IDC_NEXT_TEST, OnNextTest)
	ON_BN_CLICKED(IDC_PREV_TEST, OnPrevTest)
	ON_EN_KILLFOCUS(IDC_MODBAL_EDIT, OnKillfocusModbalEdit)
	ON_EN_KILLFOCUS(IDC_NEG2400_EDIT, OnKillfocusNeg2400Edit)
	ON_EN_KILLFOCUS(IDC_NEG800_EDIT, OnKillfocusNeg800Edit)
	ON_EN_KILLFOCUS(IDC_POS2400_EDIT, OnKillfocusPos2400Edit)
	ON_EN_KILLFOCUS(IDC_POS800_EDIT, OnKillfocusPos800Edit)
	ON_EN_KILLFOCUS(IDC_REFOSC_EDIT, OnKillfocusRefoscEdit)
	ON_BN_CLICKED(IDC_AUTOTUNE_BTN, OnAutotuneBtn)
	ON_WM_CREATE()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_ONOFF_BURST, OnStartStopBurst)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SPIN_START, OnWmSpinStart)
	ON_MESSAGE(WM_SPIN_BUTTON, OnWmSpinButton)
	ON_MESSAGE(WM_SPIN_STOP, OnWmSpinStop)
	ON_MESSAGE(WM_IO_OPLIST_DONE, OnWmIoOpListDone)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTheTuningDlg message handlers


// Local Data 
static char pszString[25];
static char *psz9600bps = "9600 bps";
static char *psz6400bps = "6400 bps";
static char *psz1600bps = "1600 bps";
static char *psz800bps = "800 bps";

// use these defines to selectively disable a control
// for the duration of the dialog or for the duration of
// test activation.
//
#define CF_GRP  0x00000001     // Center Freq controls
#define MR_GRP  0x00000002	   // Modulation Rate Controls
#define RO_GRP  0x00000004     // RefOsc controls
#define MB_GRP  0x00000008	   // ModBal controls 
#define P24_GRP 0x00000010	   // +2400 Dev controls
#define P8_GRP  0x00000020	   // +800 Dev controls
#define N8_GRP  0x00000040	   // -800 Dev controls
#define N24_GRP 0x00000080	   // -2400 Dev controls
#define PT_GRP  0x00000100 	   // All GRP's except CF_GRP & MR_GRP
#define AC_BTN  0x00000200     // Accept button
#define DF_BTN  0x00000400     // Defaults button
#define UN_BTN  0x00000800     // Undo button
#define SR_BTN  0x00001000	   // Start Test button
#define SP_BTN  0x00002000     // Stop Test button
#define PV_BTN  0x00004000     // Previous Test button
#define NT_BTN  0x00008000     // Next Test button
#define AT_BTN  0x00010000	   // Auto-Tune button
#define TR_BTN	0x00020000	   // Start/Stop for Trigger button


////////////////////////////////////////////////////////////////////////////
//  OnInitDialog()
//
//  Initialize the dialog elements.
//
BOOL CTheTuningDlg::OnInitDialog()
{   
    TRACE("\nCTheTuningDlg::OnInitDialog\n");
    char	pszString[20];
    
    m_dwPermDisabledControls = MR_GRP;  // allow modulation rate change only via backdoor (DBLCLK_RBUTTON)
    m_dwTempDisabledControls = 0x0000;
    
    CP_reflex50_enable.bSetFromDoc(&m_bReflex50Enabled);    
    
    // the base carrier frequency value is somewhat arbitrary and there is no
    // corresponding codeplug field.  According to the document "DSP/ASIC
    // Register Map, rev 3.8, dtd 5/01/96, paragraph 4.9 (Test Mode Control
    // Register), the tx frequency may be equal to (896.0125 + tf * 0.00625) kHz.
    // Additionally, the rx frequency may be equal to (929.00625 + tf * 0.00625) kHz.
    // Accordingly, we will init the center frequency edit control to the 
    // minimum and adjust 'tf' with each button click.  Note Reflex50 uses the
    // equation (901.00625 + tf * 0.0125) for the reverse channel.
    
    CP_Tx1_Test_Freq.nSetFromDoc((UINT*)&m_nCenterFreqAdjust); 
    m_nTxCenterFrqSave = m_nCenterFreqAdjust;
    CP_Rx1_Test_Freq.nSetFromDoc((UINT*)&m_nRxCenterFrqSave); 
    //
    m_dCenterFreq = dCalcReverseChanFreq(m_nCenterFreqAdjust);       
    CP_BalanceTune.bySetFromDoc(&m_byModBalance);    
    CP_Neg2400SymbolTxLevel.bySetFromDoc(&m_byNeg2400deviation);
	CP_Neg800SymbolTxLevel.bySetFromDoc(&m_byNeg800deviation);
	CP_Pos2400SymbolTxLevel.bySetFromDoc(&m_byPos2400deviation);
	CP_Pos800SymbolTxLevel.bySetFromDoc(&m_byPos800deviation);
    CP_AfcMake.bySetFromDoc(&m_byRefOsc); 
    
    // since these values as not adjusted, load them here once 
    // instead of the in the 'DoPreTuneAnalysis' function below.
    CP_TxIdleSymbol.bySetFromDoc(&TransmitRegister.byL0);    
    ReceiveRegister.byAt1 = 0x00;
    ReceiveRegister.byAt2 = 0x00;
    ReceiveRegister.byAt3 = 0x00;
    CP_TxWarmupTime.bySetFromDoc(&TransmitWarmupRegister.byTu1, 0);
    CP_TxWarmupTime.bySetFromDoc(&TransmitWarmupRegister.byTu2, 1);
    CP_TxWarmupTime.bySetFromDoc(&TransmitWarmupRegister.byTu3, 2);
    CP_TxWarmupTime.bySetFromDoc(&TransmitWarmupRegister.byTu4, 3);
    CP_TxWarmupTime.bySetFromDoc(&TransmitWarmupRegister.byTu5, 4);
    CP_TxWarmupTime.bySetFromDoc(&TransmitWarmupRegister.byTu6, 5);
    CP_TxWarmupTime.bySetFromDoc(&TransmitWarmupRegister.byTu7, 6);
    CP_TxWarmupTime.bySetFromDoc(&TransmitWarmupRegister.byTu8, 7);

    if (IS_DynamicTuningAllowed.TestNumber()) 
      byDspAsicDownloadMethod = 0x02;     // update DSP/ASIC's on the fly 
    else 
    {
      byDspAsicDownloadMethod = 0x01;     // update DSP/ASIC's only on start test
      m_dwPermDisabledControls |= TR_BTN;
    } 
      
    
    // the test configuration based upon the test id   
    m_nMinModRate = HZ_MIN_MOD_RATE;
    m_nMaxModRate = HZ_MAX_MOD_RATE;
    
    switch(m_byTestId)
    {
      case SILENT_CARRIER:  
        m_nModulationRate = 0;  // 0 Hz
        m_dwPermDisabledControls = ( MR_GRP | PT_GRP | UN_BTN | DF_BTN | AT_BTN );
        SetWindowText("Tx Tuning : Silent Carrier Frequency Test");
        break;
      case MODBALLOW:
        m_dwPermDisabledControls = ( MR_GRP | PT_GRP | UN_BTN | DF_BTN | AT_BTN );
        m_nModulationRate = 100;  // 100 Hz
        SetWindowText("Tx Tuning : Modulation Balance at Low Frequency"); 
        break;
      case MODBALHIGH:
        m_dwPermDisabledControls = ( MR_GRP | RO_GRP | P24_GRP | P8_GRP | N8_GRP | N24_GRP );
        m_nModulationRate = 1600;  // 1600 Hz
        SetWindowText("Tx Tuning : Modulation Balance at High Frequency"); 
        break;
     case TX_SPLT :
        m_dwPermDisabledControls = ( RO_GRP | MB_GRP );
        m_nModulationRate = 0x03;  // 9600 bits per sec
        m_nMinModRate = BPS_MIN_MOD_RATE;
        m_nMaxModRate = BPS_MAX_MOD_RATE; 
        SetWindowText("Tx Tuning : Symbol Deviation");
        break;
      case TX_ON:
        //byDspAsicDownloadMethod = 0x01;     // update DSP/ASIC's only on start test
        m_dwPermDisabledControls = ( PT_GRP | DF_BTN | UN_BTN | NT_BTN | AT_BTN );
        m_nModulationRate = 0x03;  // 9600 bits per sec 
        m_nMinModRate = BPS_MIN_MOD_RATE;
        m_nMaxModRate = BPS_MAX_MOD_RATE; 
        SetWindowText("Tx Tuning : Transmitter-On Test");
        break;         
      case REF_OSC:       
        m_dwPermDisabledControls = ( MR_GRP | MB_GRP | P24_GRP | P8_GRP | N8_GRP | N24_GRP | PV_BTN | AT_BTN );  
        m_nModulationRate = 0;  // 0 Hz
        CP_Rx1_Test_Freq.nSetFromDoc((UINT*)&m_nCenterFreqAdjust); 
        m_dCenterFreq = dCalcForwardChanFreq(m_nCenterFreqAdjust);
        SetWindowText("Rx Tuning : Reference Oscillator");
        break; 
      case RCVR_ON:       
        byDspAsicDownloadMethod = 0x01;     // update DSP/ASIC's only on start test
        m_dwPermDisabledControls = ( MR_GRP | PT_GRP | DF_BTN | UN_BTN | AT_BTN );
        m_nModulationRate = 0;  // 0 Hz
        CP_Rx1_Test_Freq.nSetFromDoc((UINT*)&m_nCenterFreqAdjust); 
        m_dCenterFreq = dCalcForwardChanFreq(m_nCenterFreqAdjust);
        SetWindowText("Rx Tuning : Receiver-On Test");
        break; 
      default:
        ASSERT(0);
        break;
    }
    
    // with the TX_ON and TX_SPLT tests, the modulation rate is specified
    // in bps (bits-per-sec).  Otherwise, the the modulation rate is Hz.
    if (m_nMinModRate == BPS_MIN_MOD_RATE)
    {
      m_szModulationRate = psz9600bps;   // 00b=800 : 01b=1600 : 10b=6400 : 11b=9600
    }
    else
    {
      sprintf(pszString, "%d Hz", m_nModulationRate);
      m_szModulationRate = (const char *)pszString;
    }    
   
   // indicate no test started initially.
   m_bTestInProgress = FALSE;

   // Load the Bitmap Button images
   VERIFY(m_CtrFreqUP.AutoLoad(IDC_CENTERFRQ_UP, this));
   VERIFY(m_CtrFreqDN.AutoLoad(IDC_CENTERFRQ_DN, this));

   VERIFY(m_ModRateUP.AutoLoad(IDC_MODRATE_UP, this));
   VERIFY(m_ModRateDN.AutoLoad(IDC_MODRATE_DN, this));

   VERIFY(m_RefOscUP.AutoLoad(IDC_REFOSC_UP, this));
   VERIFY(m_RefOscDN.AutoLoad(IDC_REFOSC_DN, this));

   VERIFY(m_ModBalUP.AutoLoad(IDC_MODBAL_UP, this));
   VERIFY(m_ModBalDN.AutoLoad(IDC_MODBAL_DN, this));

   VERIFY(m_Pos2400UP.AutoLoad(IDC_POS2400_UP, this));
   VERIFY(m_Pos2400DN.AutoLoad(IDC_POS2400_DN, this));

   VERIFY(m_Pos800UP.AutoLoad(IDC_POS800_UP, this));
   VERIFY(m_Pos800DN.AutoLoad(IDC_POS800_DN, this));

   VERIFY(m_Neg800UP.AutoLoad(IDC_NEG800_UP, this));
   VERIFY(m_Neg800DN.AutoLoad(IDC_NEG800_DN, this));

   VERIFY(m_Neg2400UP.AutoLoad(IDC_NEG2400_UP, this));
   VERIFY(m_Neg2400DN.AutoLoad(IDC_NEG2400_DN, this));
   
   // sub-class these specific CEdit's 
   m_RefOscEdit.SubclassDlgItem(IDC_REFOSC_EDIT, this);
   m_ModBalEdit.SubclassDlgItem(IDC_MODBAL_EDIT, this);
   m_Pos2400Edit.SubclassDlgItem(IDC_POS2400_EDIT, this);
   m_Pos800Edit.SubclassDlgItem(IDC_POS800_EDIT, this);
   m_Neg800Edit.SubclassDlgItem(IDC_NEG800_EDIT, this);
   m_Neg2400Edit.SubclassDlgItem(IDC_NEG2400_EDIT, this);
   
   // limit the input allowed on these subclassed CEdit's since
   // input is regarded as hex bytes.
   m_RefOscEdit.LimitText(2);
   m_ModBalEdit.LimitText(2);
   m_Pos2400Edit.LimitText(2);
   m_Pos800Edit.LimitText(2);
   m_Neg800Edit.LimitText(2);
   m_Neg2400Edit.LimitText(2);

   CDialog::OnInitDialog();

   EnableDisableControls();
   return TRUE;  // return TRUE  unless you set the focus to a control
}



void CTheTuningDlg::OnOK()
{                              
  // dummy OnOK function -- do not call CDialog::OnOK()
  // overridden in order to prevent accidental exits from dialog
  // when user presses ENTER.  Action of OK button redirected to
  // OnAccept() below.
  TRACE("\nCTheTuningDlg::OnOK\n");  
  
  // Action of 'ENTER' key redirected to OnWmSpinStop() defined
  // herein.  If the user edit's a CEdit during test mode enabled,
  // then consider the entry of ENTER to be a non-SpinButton update
  // of a CEdit.
  if (m_bTestInProgress) 
    OnWmSpinStop(0, 0L);
  return;
}



void CTheTuningDlg::OnAccept()
{ 
  TRACE("\nCTheTuningDlg::OnAccept\n");

  if (!UpdateData(TRUE))    // don't allow exit on invalid data
    return;

  // store the adjusted parameter(s) 
  CP_BalanceTune.bySetToDoc(m_byModBalance);    
  CP_Neg2400SymbolTxLevel.bySetToDoc(m_byNeg2400deviation);
  CP_Neg800SymbolTxLevel.bySetToDoc(m_byNeg800deviation);
  CP_Pos2400SymbolTxLevel.bySetToDoc(m_byPos2400deviation);
  CP_Pos800SymbolTxLevel.bySetToDoc(m_byPos800deviation);
  CP_AfcMake.bySetToDoc(m_byRefOsc);
   
  if (theApp.GetThePager()->IsPagerInTestMode() == TRUE)
  {
    if (byDspAsicDownloadMethod == 0x02) 
      ((CPegasus*)(theApp.GetThePager()))->CancelPreviousTest(ID_CancelPreviousTest.TestNumber());
    ((CPegasus*)(theApp.GetThePager()))->ExitTestMode(ID_ExitTestMode.TestNumber());
  }
  
  CDialog::OnOK();	
  return;	
}



//////////////////////////////////////////////////////////////////////////////
//  StartTest()
//
//  If bTrigger, the test will be started and stopped after 
//  nTriggerDelay*100 ms.
//
void CTheTuningDlg::StartTest(BYTE byTestId, BOOL bTrigger, int nTriggerDelay)
{
  TRACE("\nCTheTuningDlg::OnStartTest\n");

  CPegasus *pThePager = (CPegasus*)theApp.GetThePager();
  ASSERT(pThePager);
  
  if (DoPreTuneAnalysis(byTestId))
    return;

  // make it obvious to user which window controls may not be
  // used during test activation. 
  m_dwTempDisabledControls |= (CF_GRP | DF_BTN | UN_BTN | MR_GRP | SR_BTN | AT_BTN | TR_BTN);
  if ((byDspAsicDownloadMethod != 0x02) || (bTrigger == TRUE)) 
    m_dwTempDisabledControls |= PT_GRP;
  EnableDisableControls();   
   
  
  // tuning mode test initiation 
  if ((pThePager->IsPagerInTestMode() == FALSE) &&
      (pThePager->IsTestModeRequested() == FALSE))
  {
    if (pThePager->EnterTestMode())
    {
      OnStopTest();  // abort on error
      return;
    }
  }
  
  ASSERT(m_bTestInProgress == FALSE);
  
  // the test may be started and shortly thereafter cancelled in order
  // to effect an MDA trigger.  The CancelPreviousTest test mode command
  // value is used to indicate the choice.
  BYTE	byEffectTrigger = 0;
  if ((byDspAsicDownloadMethod == 0x02) && (bTrigger == TRUE)) 
  {
    byEffectTrigger = ID_CancelPreviousTest.TestNumber(); 
    m_bTestTriggered = TRUE;
  }
                           
  // set state of test (reset upon IoOp complete)                         
  m_bTestInProgress = TRUE;   
                     
  // inform CPegasus derived class to send or not to send the 
  // TxWarmup DSP ASIC Register content.  Should only be sent
  // if doing auto-tune of symbol deviations (TX_SPLT).
  BOOL bIgnoreTxWarmupRegister = ((m_bAutoTuneInProgress == FALSE) &&
                                  (byTestId != TX_SPLT)); 
  
  // start the test                                
  if (pThePager->DoTuningTest(byTestNum, byDspAsicDownloadMethod,
    						  TestModeControlRegister,
    						  TransmitRegister,
    						  ReceiveRegister,
    						  TransmitWarmupRegister, 
    						  byEffectTrigger, nTriggerDelay, 
    						  bIgnoreTxWarmupRegister))
      OnStopTest();  // abort on error
  return;
}



void CTheTuningDlg::OnStartTest()
{
  TRACE("\nCTheTuningDlg::OnStartTest\n");
  StartTest(m_byTestId, FALSE);
  return;
}



void CTheTuningDlg::OnStartStopBurst()
{ 
  TRACE("\nCTheTuningDlg::OnStartStopBurst\n");
  StartTest(m_byTestId, TRUE);
  return;
}



int CTheTuningDlg::DoPreTuneAnalysis(BYTE byTestId)
{
  //  NOTE:  the following parameters are set upon initiation
  //  of a test mode operation.
  
  //  TEST MODE REGISTER INFORMATION
  //  TestModeControlRegister.nTestMode = 'test dependent'
  //  TestModeControlRegister.nTestFreq = m_nCenterFreqAdjust
  //  TestModeControlRegister.nTestData = 'test dependent' m_nModulationRate value
    
  //  TRANSMITTER TUNING REGISTER INFORMATION 
  //  Note:  byL0 is initialized one in InitDialog()
  //  TransmitRegister.byL0 = Idle symbol
  //  TransmitRegister.byL1 = symbol offset at -2400 Hz
  //  TransmitRegister.byL2 = symbol offset at -800 Hz
  //  TransmitRegister.byL3 = symbol offset at +2400 Hz
  //  TransmitRegister.byL4 = symbol offset at +800 Hz
  //  TransmitRegister.byBat = balance tune
  
  //  RECEIVER TUNING REGISTER INFORMATION
  //  Note:  byAtx values are initialized once in InitDialog()
  //  ReceiveRegister.byAt1 = tuned antenna D/A setting channel 0
  //  ReceiveRegister.byAt2 = tuned antenna D/A setting channel 1
  //  ReceiveRegister.byAt3 = tuned antenna D/A setting channel 2
  //  ReceiveRegister.byRosc = intial AFC warp value
  
  //  TRANSMITTER WARMUP REGISTER INFORMATION 
  //  Note:  Values are initialized once in InitDialog()
  
  if (!UpdateData(TRUE))   // return on invalid data
    return(1);

  if ((byTestId == REF_OSC) || (byTestId == RCVR_ON))
    m_nCenterFreqAdjust = nCalcForwardChanNum(m_dCenterFreq); 
  else
    m_nCenterFreqAdjust = nCalcReverseChanNum(m_dCenterFreq); 
  
  // unless otherwise annotated with an *
  TestModeControlRegister.nTestFreq = m_nCenterFreqAdjust;  // tf = tf
  TransmitRegister.byL1 = m_byNeg2400deviation;             // L1 = L1
  TransmitRegister.byL2 = m_byNeg800deviation;    			// L2 = L2
  TransmitRegister.byL3 = m_byPos2400deviation;    			// L3 = L3
  TransmitRegister.byL4 = m_byPos800deviation;    			// L4 = L4
  TransmitRegister.byBat = m_byModBalance;                  // bat = bat 
  //CP_TxIdleSymbol.bySetFromDoc(&TransmitRegister.byL0);   // L0 = L0  // done above once
  ReceiveRegister.byRosc = m_byRefOsc;			            // rosc = rosc
  
  switch(byTestId)
  {                                                  
    case SILENT_CARRIER:   
      byTestNum = ID_SilentCarrierTest.TestNumber();
      TestModeControlRegister.nTestMode = 0x11;				 // bal modulation = 10001 binary
      TestModeControlRegister.nTestData = 0;   				 // td = don't care
      CP_TxIdleSymbol.bySetFromDoc(&TransmitRegister.byL0);  // L0 = L0 *
      TransmitRegister.byL1 = TransmitRegister.byL0;         // L1 = L0 *
      break;
    case MODBALLOW: 
      byTestNum = ID_BalanceLowTest.TestNumber();
      TestModeControlRegister.nTestMode = 0x11;	// bal modulation = 10001 binary  
      // may be auto-tune invoked... ensure test data at mod low default rate
      if (byTestId != m_byTestId)
        TestModeControlRegister.nTestData = 48;
      else if (m_nModulationRate > 0)
        TestModeControlRegister.nTestData = 4800/m_nModulationRate;   // td = Hz
      break;
    case MODBALHIGH: 
      byTestNum = ID_BalanceHighTest.TestNumber();
      TestModeControlRegister.nTestMode = 0x11;	// bal modulation = 10001 binary
      if (m_nModulationRate > 0)
        TestModeControlRegister.nTestData = 4800/m_nModulationRate;   // td = Hz 
      break;
    case TX_SPLT:
      byTestNum = ID_SplatterFilterTest.TestNumber();
      TestModeControlRegister.nTestMode = 0x12; // splatter filter = 10010 binary
      TestModeControlRegister.nTestData = m_nModulationRate;          // td = bps 
      break;
    case TX_ON:
      byTestNum = ID_TransmitterOnTest.TestNumber();
      TestModeControlRegister.nTestMode = 0x10; // async tx pulse = 10000 binary
      TestModeControlRegister.nTestData = m_nModulationRate;          // td = bps 
      break;
    case REF_OSC:    	
      byTestNum = ID_RefOscTest.TestNumber();
      TestModeControlRegister.nTestMode = 0x04; // reference oscillator = 00100 binary
      TestModeControlRegister.nTestData = 0;                          // td = td
      break; 
    case RCVR_ON:         
      byTestNum = ID_ReceiverOnTest.TestNumber();
      TestModeControlRegister.nTestMode = 0x00; // rx on (fast page)                
      TestModeControlRegister.nTestData = 0;                          // td = td
      break; 
    default:
      ASSERT(0);
      break;
  }
 return(0);
}



void CTheTuningDlg::OnStopTest()
{  
  TRACE("\nCTheTuningDlg::OnStopTest\n"); 

  m_dwTempDisabledControls &= (~(CF_GRP | DF_BTN | UN_BTN | MR_GRP | SR_BTN | AT_BTN | TR_BTN)); 
  if ((byDspAsicDownloadMethod != 0x02) || (m_bTestTriggered)) 
    m_dwTempDisabledControls &= (~PT_GRP);
  EnableDisableControls();  // reenable visual resources
  
  CPegasus *pThePager = (CPegasus*)theApp.GetThePager();
  ASSERT(pThePager);

  if (pThePager->IsPagerInTestMode() == TRUE) 
  { 
    if (byDspAsicDownloadMethod == 0x02) 
      pThePager->CancelPreviousTest(ID_CancelPreviousTest.TestNumber());
    pThePager->ExitTestMode(ID_ExitTestMode.TestNumber());            
  } 
  m_bTestInProgress = FALSE;
  m_bTestTriggered = FALSE; 
  m_bAutoTuneInProgress = FALSE;
  m_byNextAutoTuneOperation = NONE;
  // reset the warmup value after auto-tune just in case...
  CP_TxWarmupTime.bySetFromDoc(&TransmitWarmupRegister.byTu4, 3);
  return;
}



void CTheTuningDlg::OnCancel()
{
  TRACE("\nCTheTuningDlg::OnCancel\n");

  if (theApp.GetThePager()->IsPagerInTestMode() == TRUE)
  {
    if (byDspAsicDownloadMethod == 0x02)  
      ((CPegasus*)(theApp.GetThePager()))->CancelPreviousTest(ID_CancelPreviousTest.TestNumber());
    ((CPegasus*)(theApp.GetThePager()))->ExitTestMode(ID_ExitTestMode.TestNumber()); 
  }                                                                    
  m_bTestInProgress = FALSE;
  
  CDialog::OnCancel();
}



 
/////////////////////////////////////////////////////////////////////////
//  OnWmIoOpListDone()
//
//  CPager::OnOpCompletion initiated event handler.  Receives WM_IO_OPLIST_DONE
//  messages when the IoOperation list completes.  
//    wParam = nStatus (0=COMPLETED, 1=FAILED)
//    LOWORD(lParam) = Last CIoOperation ID
afx_msg LONG CTheTuningDlg::OnWmIoOpListDone(UINT wParam, LONG lParam)
{
  TRACE("\nCTheTuningDlg::OnWmIoOpListDone\n");
  
  // on error, abort operations                          
  if (wParam) 
  { 
    OnStopTest(); // failure
    return(0L);
  }

  // test was triggered, set condition 
  if (m_bTestTriggered == TRUE)
  {
    m_bTestTriggered = FALSE;
    m_bTestInProgress = FALSE;
    if (m_bAutoTuneInProgress == FALSE)
      m_dwTempDisabledControls &= (~(CF_GRP | DF_BTN | UN_BTN | MR_GRP | SR_BTN | AT_BTN | TR_BTN | PT_GRP));
  }  

  // auto-tune in progress, pager triggered, handle next auto-tune operation.                          
  if (m_bAutoTuneInProgress) 
  {
    HandleNextAutoTuneOperation();
    if (m_bAutoTuneInProgress == FALSE)
      m_dwTempDisabledControls &= (~(CF_GRP | DF_BTN | UN_BTN | MR_GRP | SR_BTN | AT_BTN | TR_BTN | PT_GRP));    
  }
   
  // trigger and auto-tune finished, reenable the tuning buttons
  if ((m_bAutoTuneInProgress == FALSE) && (m_bTestTriggered == FALSE))
  {
    EnableDisableControls();
  } 
  return(0L);
}
  




//////////////////////////////////////////////////////////////////
//  EnableDisableControls()
//
//  This function disable/enables those visual controls which
//  should not be activated while the pager is in test mode or
//  permanently for the duration of the dialog depending upon
//  the selected test.
//
void CTheTuningDlg::EnableDisableControls(void)
{ 
  TRACE("\nCTheTuningDlg::EnableDisableControls : Perm=%08X : Test=%08X\n",
        m_dwPermDisabledControls, m_dwTempDisabledControls);

  CButton		*pBtn = NULL;
  BOOL			bEnable;
  DWORD			dwDisabledControls;
  
  dwDisabledControls = (m_dwPermDisabledControls | m_dwTempDisabledControls);
   					     
  // enable/disable the center frequency grp 
  bEnable = ((dwDisabledControls & CF_GRP) == 0); 
  CSpinBtn::EnableSpinGroup(this, 0, IDC_CENTERFRQ_EDIT, m_CtrFreqUP, m_CtrFreqDN, bEnable);
  
  // enable/disable the modulation rate grp
  bEnable = ((dwDisabledControls & MR_GRP) == 0); 
  CSpinBtn::EnableSpinGroup(this, 0, IDC_MODRATE_EDIT, m_ModRateUP, m_ModRateDN, bEnable);

  // enable/disable the RO_GRP
  bEnable = ((dwDisabledControls & (RO_GRP | PT_GRP)) == 0);
  CSpinBtn::EnableSpinGroup(this, 0, IDC_REFOSC_EDIT, m_RefOscUP, m_RefOscDN, bEnable);

  // enable/disable the MB_GRP
  bEnable = ((dwDisabledControls & (MB_GRP | PT_GRP)) == 0);
  CSpinBtn::EnableSpinGroup(this, 0, IDC_MODBAL_EDIT, m_ModBalUP, m_ModBalDN, bEnable);

  // enable/disable the P24_GRP
  bEnable = ((dwDisabledControls & (P24_GRP | PT_GRP)) == 0);
  CSpinBtn::EnableSpinGroup(this, 0, IDC_POS2400_EDIT, m_Pos2400UP, m_Pos2400DN, bEnable);

  // enable/disable the P8_GRP
  bEnable = ((dwDisabledControls & (P8_GRP | PT_GRP)) == 0);
  CSpinBtn::EnableSpinGroup(this, 0, IDC_POS800_EDIT, m_Pos800UP, m_Pos800DN, bEnable);

  // enable/disable the N8_GRP
  bEnable = ((dwDisabledControls & (N8_GRP | PT_GRP)) == 0);
  CSpinBtn::EnableSpinGroup(this, 0, IDC_NEG800_EDIT, m_Neg800UP, m_Neg800DN, bEnable);

  // enable/disable the N24_GRP
  bEnable = ((dwDisabledControls & (N24_GRP | PT_GRP)) == 0);
  CSpinBtn::EnableSpinGroup(this, 0, IDC_NEG2400_EDIT, m_Neg2400UP, m_Neg2400DN, bEnable);

  // enable/disable the accept button 
  bEnable = ((dwDisabledControls & AC_BTN) == 0);
  pBtn = (CButton *)GetDlgItem(IDC_TUNING_ACCEPT);
  ASSERT(pBtn != NULL);
  pBtn->EnableWindow(bEnable); 
    
  // enable/disable the use defaults button
  bEnable = ((dwDisabledControls & DF_BTN) == 0);
  pBtn = (CButton *)GetDlgItem(IDC_TUNING_DEFAULT);
  ASSERT(pBtn != NULL);
  pBtn->EnableWindow(bEnable);
                              
  // enable/disable the undo button
  bEnable = ((dwDisabledControls & UN_BTN) == 0);
  pBtn = (CButton *)GetDlgItem(IDC_TUNING_UNDO);
  ASSERT(pBtn != NULL);
  pBtn->EnableWindow(bEnable);

  // enable/disable the start test button
  bEnable = ((dwDisabledControls & SR_BTN) == 0);
  pBtn = (CButton *)GetDlgItem(IDC_START_TEST);
  ASSERT(pBtn != NULL);
  pBtn->EnableWindow(bEnable); 

  // enable/disable the stop test button
  bEnable = ((dwDisabledControls & SP_BTN) == 0);
  pBtn = (CButton *)GetDlgItem(IDC_STOP_TEST);
  ASSERT(pBtn != NULL);
  pBtn->EnableWindow(bEnable);

  // enable/disable the previous test button
  bEnable = ((dwDisabledControls & PV_BTN) == 0);
  pBtn = (CButton *)GetDlgItem(IDC_PREV_TEST);
  ASSERT(pBtn != NULL);
  pBtn->EnableWindow(bEnable);

  // enable/disable the next test button
  bEnable = ((dwDisabledControls & NT_BTN) == 0);
  pBtn = (CButton *)GetDlgItem(IDC_NEXT_TEST);
  ASSERT(pBtn != NULL);
  pBtn->EnableWindow(bEnable);

  // enable/disable the auto-tune button
  bEnable = ((dwDisabledControls & AT_BTN) == 0);
  pBtn = (CButton *)GetDlgItem(IDC_AUTOTUNE_BTN);
  ASSERT(pBtn != NULL);
  pBtn->EnableWindow(bEnable); 
  
  // enable/disable the start/stop for trigger button
  bEnable = ((dwDisabledControls & TR_BTN) == 0);
  pBtn = (CButton *)GetDlgItem(IDC_ONOFF_BURST);
  ASSERT(pBtn != NULL);
  pBtn->EnableWindow(bEnable); 
  
  return;
}




 
double CTheTuningDlg::dCalcForwardChanFreq(UINT nChanNum)
{  
  if (m_bReflex50Enabled)
    return(CReflex50Protocol::dCalcForwardChanFreq(nChanNum));
  else
    return(CReflex25Protocol::dCalcForwardChanFreq(nChanNum));
}

int CTheTuningDlg::nCalcForwardChanNum(double dFwdFreq, BYTE byRounding)
{
  if (m_bReflex50Enabled)
    return(CReflex50Protocol::nCalcForwardChanNum(dFwdFreq, byRounding));
  else
    return(CReflex25Protocol::nCalcForwardChanNum(dFwdFreq, byRounding));
}

   
 
double CTheTuningDlg::dCalcReverseChanFreq(UINT nChanNum)
{ 
  if (m_bReflex50Enabled) 
    return(CReflex50Protocol::dCalcReverseChanFreq((BYTE)nChanNum));
  else 
    return(CReflex25Protocol::dCalcReverseChanFreq(nChanNum)); 
}

int CTheTuningDlg::nCalcReverseChanNum(double dRevFreq, BYTE byRounding)
{
  if (m_bReflex50Enabled)
    return((int)CReflex50Protocol::byCalcReverseChanNum(dRevFreq, byRounding));
  else
    return(CReflex25Protocol::nCalcReverseChanNum(dRevFreq, byRounding));
}


/////////////////////////////////////////////////////////////////////////
//  OnWmSpinStart()
//
//  Calls to this function are generated from the CSpinButton member
//  functions.  This message gives notice that the user is beginning
//  to scroll.
//
//  wParam = Resource ID of the CSpinBtn
//
afx_msg LONG CTheTuningDlg::OnWmSpinStart(UINT wParam, LONG lParam)
{ 
  TRACE("\nCTheTuningDlg::OnWmSpinStart\n");
  
  //  ** Add any processing as required **
  
  return(0L);
}


/////////////////////////////////////////////////////////////////////////
//  OnWmSpinStop()
//
//  Calls to this function are generated from the CSpinButton member
//  functions.  This message gives notice that the user has stopped
//  scrolling.
//  
//  wParam = Resource ID of the CSpinBtn
//  
//  NOTE:  This function is also called from the 'OnOk()' function
//  above and from the 'OnKillFocus...()' functions below to deal
//  with manual edits of the CEdit's during the test.
//
afx_msg LONG CTheTuningDlg::OnWmSpinStop(UINT wParam, LONG lParam)
{
  TRACE("\nCTheTuningDlg::OnWmSpinStop\n");
 
  CPegasus *pThePager = (CPegasus*)theApp.GetThePager();
  ASSERT(pThePager);

  // if not in test mode, ignore
  if (pThePager->IsPagerInTestMode() == FALSE)
    return(0L);
  
  // Note:  If this device does not support tuning updates
  // on the fly, then the spin button should be disabled
  // during test mode.
  if (byDspAsicDownloadMethod != 0x02) 
    return(0L);  
  
  // The device may be in test mode, but no test started...
  if (m_bTestInProgress == FALSE)
    return(0L); 

  // If in test mode update the current test with new data.
  // Note, all of the test data has not changed, but just to be 
  // save, redo the analysis.
  if (DoPreTuneAnalysis(m_byTestId))
    return(0L);
  
  if (pThePager->DoTuningUpdate(TransmitRegister, ReceiveRegister))
      OnStopTest();  // abort on error
  
  return(0L);
}



/////////////////////////////////////////////////////////////////////////
//  OnWmSpinButton()
//
//  Calls to this function are generated from the CSpinButton::OnTimer()
//  function.  These calls result from the user holding down the left
//  mouse button.  The effect is to provide a miniature scroll button.
//
afx_msg LONG CTheTuningDlg::OnWmSpinButton(UINT wParam, LONG lParam)
{
  TRACE("\nCTheTuningDlg::OnWmSpinButton : nBtnID=%04X\n", wParam);  
  int nAcceleration = LOWORD(lParam);

  if (!UpdateData(TRUE))
    return(0L);
  
  switch(wParam)
  {
    case IDC_CENTERFRQ_UP :
      HandleCenterFreqUp(nAcceleration);
      break;
      
    case IDC_CENTERFRQ_DN :
      HandleCenterFreqDn(nAcceleration);
      break;
      
    case IDC_MODRATE_UP :
      if (m_nMaxModRate == HZ_MAX_MOD_RATE)
      { 
        do  // mod rate must be evenly divisible into 4800
        { 
          if ((m_nModulationRate + 100) >= m_nMaxModRate) 
            m_nModulationRate = m_nMaxModRate;
          else 
            m_nModulationRate += 100;
        } while((4800 % m_nModulationRate) > 0);
        sprintf(pszString, "%d Hz", m_nModulationRate);
        m_szModulationRate = (const char *)pszString;
      }
      else
      {
        if ((m_nModulationRate + 1) <= m_nMaxModRate)
          m_nModulationRate += 1;
        else
          m_nModulationRate = m_nMaxModRate;  
        if (m_nModulationRate == 3) m_szModulationRate = psz9600bps;
        else if (m_nModulationRate == 2) m_szModulationRate = psz6400bps;
        else if (m_nModulationRate == 1) m_szModulationRate = psz1600bps;
        else m_szModulationRate = psz800bps;
      }
      break;
      
    case IDC_MODRATE_DN :
      if (m_nMinModRate == HZ_MIN_MOD_RATE)   // modulation rate is in Hz
      { 
        do  // mod rate must be evenly divisible into 4800
        { 
          if ((m_nModulationRate - 100) <= m_nMinModRate) 
            m_nModulationRate = m_nMinModRate;
          else 
            m_nModulationRate -= 100;
          if (m_nModulationRate == 0) break;
        } while((4800 % m_nModulationRate) > 0);
        sprintf(pszString, "%d Hz", m_nModulationRate);
        m_szModulationRate = (const char *)pszString;
      }
      else  // modulation rate is in bps
      {
        if ((m_nModulationRate - 1) >= m_nMinModRate)
          m_nModulationRate -= 1;
        else
          m_nModulationRate = m_nMinModRate;  
        if (m_nModulationRate == 3) m_szModulationRate = psz9600bps;
        else if (m_nModulationRate == 2) m_szModulationRate = psz6400bps;
        else if (m_nModulationRate == 1) m_szModulationRate = psz1600bps;
        else m_szModulationRate = psz800bps;
      }
      break;
      
    case IDC_REFOSC_UP : 
      if ((int)(m_byRefOsc + nAcceleration) > 0xFF)
        m_byRefOsc = 0xFF;
      else
        m_byRefOsc = (BYTE)(m_byRefOsc + nAcceleration);
      break;
      
    case IDC_REFOSC_DN :
      if ((int)(m_byRefOsc - nAcceleration) < 0)
        m_byRefOsc = 0;
      else
        m_byRefOsc = (BYTE)(m_byRefOsc - nAcceleration);
      break;
      
    case IDC_MODBAL_UP :
      if ((int)(m_byModBalance + nAcceleration) > 0x3F)
        m_byModBalance = 0x3F;
      else
        m_byModBalance = (BYTE)(m_byModBalance + nAcceleration);
      break;
      
    case IDC_MODBAL_DN :
      if ((int)(m_byModBalance - nAcceleration) < 0)
        m_byModBalance = 0;
      else
        m_byModBalance = (BYTE)(m_byModBalance - nAcceleration);
      break;
      
    case IDC_POS2400_UP :
      if ((int)(m_byPos2400deviation + nAcceleration) > 0xFF)
        m_byPos2400deviation = 0xFF;
      else
        m_byPos2400deviation = (BYTE)(m_byPos2400deviation + nAcceleration);
      break;
      
    case IDC_POS2400_DN :
      if ((int)(m_byPos2400deviation - nAcceleration) < 0)
        m_byPos2400deviation = 0;
      else
        m_byPos2400deviation = (BYTE)(m_byPos2400deviation - nAcceleration);
      break;
      
    case IDC_POS800_UP :
      if ((int)(m_byPos800deviation + nAcceleration) > 0xFF)
        m_byPos800deviation = 0xFF;
      else
        m_byPos800deviation = (BYTE)(m_byPos800deviation + nAcceleration);
      break;
      
    case IDC_POS800_DN :
      if ((int)(m_byPos800deviation - nAcceleration) < 0)
        m_byPos800deviation = 0;
      else
        m_byPos800deviation = (BYTE)(m_byPos800deviation - nAcceleration);
      break;
      
    case IDC_NEG800_UP :
      if ((int)(m_byNeg800deviation + nAcceleration) > 0xFF)
        m_byNeg800deviation = 0xFF;
      else
        m_byNeg800deviation = (BYTE)(m_byNeg800deviation + nAcceleration);
      break;
      
    case IDC_NEG800_DN :
      if ((int)(m_byNeg800deviation - nAcceleration) < 0)
        m_byNeg800deviation = 0;
      else
        m_byNeg800deviation = (BYTE)(m_byNeg800deviation - nAcceleration);
      break;
      
    case IDC_NEG2400_UP :
      if ((int)(m_byNeg2400deviation + nAcceleration) > 0xFF)
        m_byNeg2400deviation = 0xFF;
      else
        m_byNeg2400deviation = (BYTE)(m_byNeg2400deviation + nAcceleration);
      break;
      
    case IDC_NEG2400_DN :
      if ((int)(m_byNeg2400deviation - nAcceleration) < 0)
        m_byNeg2400deviation = 0;
      else
        m_byNeg2400deviation = (BYTE)(m_byNeg2400deviation - nAcceleration);
      break;
      
    default :
      break;
  }
  
  UpdateData(FALSE);
  return(0L);
}


 
void CTheTuningDlg::HandleCenterFreqUp(int nAcceleration)
{  
  if ((m_byTestId == REF_OSC) || (m_byTestId == RCVR_ON))
    m_nCenterFreqAdjust = nCalcForwardChanNum(m_dCenterFreq, 1);  // round_up
  else
    m_nCenterFreqAdjust = nCalcReverseChanNum(m_dCenterFreq, 1);  // round_up

  if ((m_nCenterFreqAdjust + nAcceleration) <= MAX_CNTR_FREQ)
    m_nCenterFreqAdjust += nAcceleration;
  else
    m_nCenterFreqAdjust = MAX_CNTR_FREQ;

  if ((m_byTestId == REF_OSC) || (m_byTestId == RCVR_ON))
    m_dCenterFreq = dCalcForwardChanFreq(m_nCenterFreqAdjust);
  else	
    m_dCenterFreq = dCalcReverseChanFreq(m_nCenterFreqAdjust);
  
  return;
}


void CTheTuningDlg::HandleCenterFreqDn(int nAcceleration)
{  
  if ((m_byTestId == REF_OSC) || (m_byTestId == RCVR_ON))
    m_nCenterFreqAdjust = nCalcForwardChanNum(m_dCenterFreq, 2);  // round_down
  else
    m_nCenterFreqAdjust = nCalcReverseChanNum(m_dCenterFreq, 2);  // round_down

  if ((m_nCenterFreqAdjust - nAcceleration) >= MIN_CNTR_FREQ)
    m_nCenterFreqAdjust -= nAcceleration;
  else
    m_nCenterFreqAdjust = MIN_CNTR_FREQ;

  if ((m_byTestId == REF_OSC) || (m_byTestId == RCVR_ON))
    m_dCenterFreq = dCalcForwardChanFreq(m_nCenterFreqAdjust);
  else	
    m_dCenterFreq = dCalcReverseChanFreq(m_nCenterFreqAdjust);

  return;
}


void CTheTuningDlg::OnUndoBtn()
{ 
  TRACE("\nCTheTuningDlg::OnUndoBtn\n"); 
  CPpsDoc *pDoc = (CPpsDoc*)((CFrameWnd *)theApp.m_pMainWnd)->GetActiveDocument();
  ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(CPpsDoc)));
  
  if (m_byTestId == MODBALHIGH)
    pDoc->SetFieldFromOriginal(CP_BalanceTune.Offset(), CP_BalanceTune.Length()); 
  else if (m_byTestId == TX_SPLT)
    pDoc->SetFieldFromOriginal(CP_SymbolTxLevel.Offset(), CP_SymbolTxLevel.Length());
  else if (m_byTestId == REF_OSC)
    pDoc->SetFieldFromOriginal(CP_AfcMake.Offset(), CP_AfcMake.Length());

  CP_BalanceTune.bySetFromDoc(&m_byModBalance);    
  CP_Neg2400SymbolTxLevel.bySetFromDoc(&m_byNeg2400deviation);
  CP_Neg800SymbolTxLevel.bySetFromDoc(&m_byNeg800deviation);
  CP_Pos2400SymbolTxLevel.bySetFromDoc(&m_byPos2400deviation);
  CP_Pos800SymbolTxLevel.bySetFromDoc(&m_byPos800deviation);
  CP_AfcMake.bySetFromDoc(&m_byRefOsc); 

  UpdateData(FALSE);
  return;	
}


void CTheTuningDlg::OnUseDefaultBtn()
{ 
  TRACE("\nCTheTuningDlg::OnUseDefaultBtn\n"); 
  CPpsDoc *pDoc = (CPpsDoc*)((CFrameWnd *)theApp.m_pMainWnd)->GetActiveDocument();
  ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(CPpsDoc)));

  if (m_byTestId == MODBALHIGH)
  {
    if (pDoc->SetFieldFromDefault(CP_BalanceTune.Offset(), CP_BalanceTune.Length()))
      return;
  }
  else if (m_byTestId == TX_SPLT)
  {
    if (pDoc->SetFieldFromDefault(CP_SymbolTxLevel.Offset(), CP_SymbolTxLevel.Length()))
      return;
  }
  else if (m_byTestId == REF_OSC)
  {
    if (pDoc->SetFieldFromDefault(CP_AfcMake.Offset(), CP_AfcMake.Length()))
      return;
  }

  CP_BalanceTune.bySetFromDoc(&m_byModBalance);    
  CP_Neg2400SymbolTxLevel.bySetFromDoc(&m_byNeg2400deviation);
  CP_Neg800SymbolTxLevel.bySetFromDoc(&m_byNeg800deviation);
  CP_Pos2400SymbolTxLevel.bySetFromDoc(&m_byPos2400deviation);
  CP_Pos800SymbolTxLevel.bySetFromDoc(&m_byPos800deviation);
  CP_AfcMake.bySetFromDoc(&m_byRefOsc); 

  UpdateData(FALSE);
  return;	
}




void CTheTuningDlg::OnRButtonDblClk(UINT nFlags, CPoint point)
{
   // Back-door to enable all the parameter groups.  Normally,
   // the user would use only the enabled groups specific to
   // each test.
    
   // Note:  If this device does not support tuning updates
   // on the fly, then the spin button should be disabled
   // during test mode.
   if (byDspAsicDownloadMethod == 0x02) 
   {
      m_dwPermDisabledControls &= (~(DF_BTN | UN_BTN | MB_GRP | MR_GRP | RO_GRP | P24_GRP | P8_GRP | N8_GRP | N24_GRP | PT_GRP));
      EnableDisableControls();
   }
   CDialog::OnRButtonDblClk(nFlags, point);
}


void CTheTuningDlg::OnTuningHelp()
{ 
  LPARAM  	ulTopic = HID_BASE_COMMAND;
  
  switch (m_byTestId)
  { 
    case SILENT_CARRIER:
      ulTopic += ID_TXTUNE_SCF; 
      break;
    case MODBALLOW:
      ulTopic += ID_TXTUNE_MODBAL_LOW; 
      break;
    case MODBALHIGH:
      ulTopic += ID_TXTUNE_MODBAL_HIGH; 
      break;
    case TX_SPLT: 
      ulTopic += ID_TXTUNE_SYMBOL_DEV; 
      break;
    case TX_ON:
      ulTopic += ID_TXTUNE_TXON_TEST; 
      break;         
    case REF_OSC:    
      ulTopic += ID_RXTUNE_REFOSC; 
      break;
    case RCVR_ON:
      ulTopic += ID_RXTUNE_RCVRON_TEST;
      break;
    default:
      ASSERT(0);
      break;
  }
  AfxGetApp()->WinHelp(ulTopic);
  return;
}
 


void CTheTuningDlg::OnSwitchTest(BYTE byNewTestId)
{    
  // the test configuration based upon the test id   
  m_nMinModRate = HZ_MIN_MOD_RATE;
  m_nMaxModRate = HZ_MAX_MOD_RATE;
  m_dCenterFreq = dCalcReverseChanFreq(m_nCenterFreqAdjust);
    
  if (IS_DynamicTuningAllowed.TestNumber()) 
    byDspAsicDownloadMethod = 0x02;     // update DSP/ASIC's on the fly 
  else
    byDspAsicDownloadMethod = 0x01;     // update DSP/ASIC's only on start test

  switch(byNewTestId)
  {
    case SILENT_CARRIER:  
      m_dwPermDisabledControls = ( MR_GRP | PT_GRP | UN_BTN | DF_BTN | AT_BTN );
      m_nModulationRate = 0;  // 0 Hz
      SetWindowText("Tx Tuning : Silent Carrier Frequency Test");
      break;
    case MODBALLOW:
      m_dwPermDisabledControls = ( MR_GRP | PT_GRP | UN_BTN | DF_BTN | AT_BTN );
      m_nModulationRate = 100;  // 100 Hz
      SetWindowText("Tx Tuning : Modulation Balance at Low Frequency"); 
      break;
    case MODBALHIGH:
      m_dwPermDisabledControls = ( MR_GRP | RO_GRP | P24_GRP | P8_GRP | N8_GRP | N24_GRP );
      m_nModulationRate = 1600;  // 1600 Hz
      SetWindowText("Tx Tuning : Modulation Balance at High Frequency"); 
      break;
   case TX_SPLT :
      m_dwPermDisabledControls = ( RO_GRP | MB_GRP );
      m_nModulationRate = 0x03;  // 9600 bits per sec
      m_nMinModRate = BPS_MIN_MOD_RATE;
      m_nMaxModRate = BPS_MAX_MOD_RATE; 
      SetWindowText("Tx Tuning : Symbol Deviation");
      break;
    case TX_ON:
      //byDspAsicDownloadMethod = 0x01;     // update DSP/ASIC's only on start test
      m_dwPermDisabledControls = ( PT_GRP | DF_BTN | UN_BTN | NT_BTN | AT_BTN );
      m_nModulationRate = 0x03;  // 9600 bits per sec 
      m_nMinModRate = BPS_MIN_MOD_RATE;
      m_nMaxModRate = BPS_MAX_MOD_RATE; 
      SetWindowText("Tx Tuning : Transmitter-On Test");
      break;         
    case REF_OSC:       
      m_dwPermDisabledControls = ( MR_GRP | MB_GRP | P24_GRP | P8_GRP | N8_GRP | N24_GRP | PV_BTN | AT_BTN );  
      m_nModulationRate = 0;  // 0 Hz
      m_dCenterFreq = dCalcForwardChanFreq(m_nCenterFreqAdjust);
      SetWindowText("Rx Tuning : Reference Oscillator");
      break; 
    case RCVR_ON:       
      byDspAsicDownloadMethod = 0x01;     // update DSP/ASIC's only on start test
      m_dwPermDisabledControls = ( MR_GRP | PT_GRP | DF_BTN | UN_BTN | AT_BTN );
      m_nModulationRate = 0;  // 0 Hz
      m_dCenterFreq = dCalcForwardChanFreq(m_nCenterFreqAdjust);
      SetWindowText("Rx Tuning : Receiver-On Test");
      break; 
    default:
      ASSERT(0);
      break;
  }
    
  m_byTestId = byNewTestId;

  // with the TX_ON and TX_SPLT tests, the modulation rate is specified
  // in bps (bits-per-sec).  Otherwise, the the modulation rate is Hz.
  if (m_nMinModRate == BPS_MIN_MOD_RATE)
  {
    m_szModulationRate = psz9600bps;   // 00b=800 : 01b=1600 : 10b=6400 : 11b=9600
  }
  else
  {
    sprintf(pszString, "%d Hz", m_nModulationRate);
    m_szModulationRate = (const char *)pszString;
  }    
  
  UpdateData(FALSE);  
  EnableDisableControls();
  return;
}


void CTheTuningDlg::OnNextTest()
{ 
  if (!UpdateData(TRUE))
    return;

  if (m_bTestInProgress) 
  {
    if (byDspAsicDownloadMethod == 0x02) 
    {
      ((CPegasus*)(theApp.GetThePager()))->CancelPreviousTest(ID_CancelPreviousTest.TestNumber());
      m_bTestInProgress = FALSE;
      m_dwTempDisabledControls &= (~(CF_GRP | DF_BTN | UN_BTN | MR_GRP | SR_BTN | AT_BTN | TR_BTN)); 
    }
    else
      OnStopTest();
  }

  if ((m_byTestId == REF_OSC) || (m_byTestId == RCVR_ON))
    m_nCenterFreqAdjust = nCalcForwardChanNum(m_dCenterFreq, 0);  
  else
    m_nCenterFreqAdjust = nCalcReverseChanNum(m_dCenterFreq, 0);  

  switch (m_byTestId)
  { 
    case SILENT_CARRIER:
      OnSwitchTest(MODBALLOW);
      break;
    case MODBALLOW:
      OnSwitchTest(MODBALHIGH);
      break;
    case MODBALHIGH:
      OnSwitchTest(TX_SPLT);
      break;
    case TX_SPLT:
      m_nTxCenterFrqSave = m_nCenterFreqAdjust;
      m_nCenterFreqAdjust = m_nRxCenterFrqSave;
      OnSwitchTest(RCVR_ON); 
      break;
    case TX_ON:
      break;         
    case REF_OSC:    
      m_nRxCenterFrqSave = m_nCenterFreqAdjust;
      m_nCenterFreqAdjust = m_nTxCenterFrqSave;
      OnSwitchTest(SILENT_CARRIER);
      break;
    case RCVR_ON:
      m_nRxCenterFrqSave = m_nCenterFreqAdjust;
      m_nCenterFreqAdjust = m_nTxCenterFrqSave;
      OnSwitchTest(TX_ON); 
      break;
    default:
      ASSERT(0);
      break;
  } 
  return;	
}



void CTheTuningDlg::OnPrevTest()
{
  if (!UpdateData(TRUE))
    return;

  if (m_bTestInProgress) 
  {
    if (byDspAsicDownloadMethod == 0x02) 
    {
      ((CPegasus*)(theApp.GetThePager()))->CancelPreviousTest(ID_CancelPreviousTest.TestNumber());
      m_bTestInProgress = FALSE;
      m_dwTempDisabledControls &= (~(CF_GRP | DF_BTN | UN_BTN | MR_GRP | SR_BTN | AT_BTN | TR_BTN)); 
    }
    else
      OnStopTest();
  }

  if ((m_byTestId == REF_OSC) || (m_byTestId == RCVR_ON))
    m_nCenterFreqAdjust = nCalcForwardChanNum(m_dCenterFreq, 0);  
  else
    m_nCenterFreqAdjust = nCalcReverseChanNum(m_dCenterFreq, 0);  

  switch (m_byTestId)
  { 
    case SILENT_CARRIER:
      m_nTxCenterFrqSave = m_nCenterFreqAdjust;
      m_nCenterFreqAdjust = m_nRxCenterFrqSave;
      OnSwitchTest(REF_OSC); 
      break;
    case MODBALLOW:
      OnSwitchTest(SILENT_CARRIER); 
      break;
    case MODBALHIGH:
      OnSwitchTest(MODBALLOW); 
      break;
    case TX_SPLT: 
      OnSwitchTest(MODBALHIGH); 
      break;
    case TX_ON:
      m_nTxCenterFrqSave = m_nCenterFreqAdjust;
      m_nCenterFreqAdjust = m_nRxCenterFrqSave;
      OnSwitchTest(RCVR_ON); 
      break;         
    case REF_OSC:    
      break;
    case RCVR_ON: 
      m_nRxCenterFrqSave = m_nCenterFreqAdjust;
      m_nCenterFreqAdjust = m_nTxCenterFrqSave;
      OnSwitchTest(TX_SPLT); 
      break;
    default:
      ASSERT(0);
      break;
  }
  return;	
}


////////////////////////////////////////////////////////////
//  OnKillFocus...
//  Treat focus loss as a tab event and update the test
//  if necessary.
//
void CTheTuningDlg::OnKillfocusModbalEdit()
{ 
  if ((m_ModBalEdit.GetModify()) && (m_bTestInProgress))
  {
    OnWmSpinStop(0, 0L); 
    m_ModBalEdit.SetModify(FALSE);
  }
  return;	
}

void CTheTuningDlg::OnKillfocusNeg2400Edit()
{
  if ((m_Neg2400Edit.GetModify()) && (m_bTestInProgress))
  {
    OnWmSpinStop(0, 0L);
    m_Neg2400Edit.SetModify(FALSE);
  }
  return;	
	
}

void CTheTuningDlg::OnKillfocusNeg800Edit()
{
  if ((m_Neg800Edit.GetModify()) && (m_bTestInProgress))
  {
    OnWmSpinStop(0, 0L);
    m_Neg800Edit.SetModify(FALSE);
  }
  return;	
	
}

void CTheTuningDlg::OnKillfocusPos2400Edit()
{
  if ((m_Pos2400Edit.GetModify()) && (m_bTestInProgress))
  {
    OnWmSpinStop(0, 0L);
    m_Pos2400Edit.SetModify(FALSE);
  }
  return;	
	
}

void CTheTuningDlg::OnKillfocusPos800Edit()
{
  if ((m_Pos800Edit.GetModify()) && (m_bTestInProgress))
  {
    OnWmSpinStop(0, 0L);
    m_Pos800Edit.SetModify(FALSE);
  }
  return;	
	
}

void CTheTuningDlg::OnKillfocusRefoscEdit()
{
  if ((m_RefOscEdit.GetModify()) && (m_bTestInProgress))
  {
    OnWmSpinStop(0, 0L);
    m_RefOscEdit.SetModify(FALSE);
  }
  return;	
}



//////////////////////////////////////////////////////////////////////////
//  OnAutotuneBtn()
//
void CTheTuningDlg::OnAutotuneBtn()
{ 
  TRACE("\nCTheTuningDlg::OnAutotuneBtn\n");
  
  if (m_bAutoTuneInProgress == TRUE)
    return;
  
  if (!UpdateData(TRUE))
    return;

  // configure the MDA
  if (theHpMda.IsDeviceConfigured() == FALSE)
  {
	if (theHpMda.ConfigureDevice())
	{ 
	  theHpMda.OnOpFailure();
	  return;              
	}
  } 
  
  // reset the MDA
  if (theHpMda.Reset())
  { 
    theHpMda.OnOpFailure();
    return;              
  }

  m_bAutoTuneInProgress = TRUE;  
  m_bAutoTuneModifications = FALSE;

  // configure the MDA for the currently selected auto-tune operation.  
  if (m_byTestId == MODBALHIGH) 
    m_byNextAutoTuneOperation = MODBALLOW_CONFIG;
  else if (m_byTestId == TX_SPLT)  
  {
    m_byNextAutoTuneOperation = SYMBOLDEV_CONFIG;
    // adjust the transmitter warmup time so an initial silent carrier
    // signal can be sampled...
    TransmitWarmupRegister.byTu4 = 0x19;
  }  
  else 
  {
    ASSERT(0);  
    return;   // this condition should not occur 
  }
  
  // initiate the tuning... 
  HandleNextAutoTuneOperation();
  return;
}



//////////////////////////////////////////////////////////////////////
//  HandleNextAutoTuneOperation()
//
//  This function is called from OnAutotuneBtn() or OnWmIoOpListDone().
//  It may also be called recursively.
//
void CTheTuningDlg::HandleNextAutoTuneOperation(void)
{ 
  TRACE("\nCTheTuningDlg::HandleNextAutoTuneOperation : AT=%d : OP=%d\n",
         m_bAutoTuneInProgress, m_byNextAutoTuneOperation);
  double dDelta;
  int	 nStep, nDiff;
  
  if ((m_bAutoTuneInProgress == FALSE) || (m_byNextAutoTuneOperation == NONE))
    return;
  
  ///////////
  if (m_byNextAutoTuneOperation == MODBALLOW_CONFIG)
  {
    // configure the MDA for the initial ModBalLow sample.
    if (theHpMda.ConfigureModBalLow(m_dCenterFreq))
    { 
      theHpMda.OnOpFailure();
      return;              
    } 
    // start/stop the test to trigger the MDA
    StartTest(MODBALLOW, TRUE);
    m_byNextAutoTuneOperation = MODBALLOW_ANALYSIS;
    return;  // come back here upon WM_IO_OPLIST_DONE
  }
  
  ////////////  
  if (m_byNextAutoTuneOperation == MODBALLOW_ANALYSIS)
  { 
    // analyze the ModBalLow MDA capture...
    if (theHpMda.AnalyzeModBalLow(&m_dModBalLowMean))
    { 
      m_bAutoTuneInProgress = FALSE;  // quit auto-tune on error
      theHpMda.OnOpFailure();
      return;              
    } 
    TRACE("\nCTheTuningDlg::HandleNextAutoTuneOperation : P-P=%f\n", m_dModBalLowMean);
    m_byNextAutoTuneOperation = MODBALHIGH_CONFIG; 
  } 
  
  ////////////
  if (m_byNextAutoTuneOperation == MODBALHIGH_CONFIG)
  {
    // initiate the ModBalHigh operation
    if (theHpMda.ConfigureModBalHigh())
    { 
      m_bAutoTuneInProgress = FALSE;  // quit auto-tune on error
      theHpMda.OnOpFailure();
      return;              
    } 
    // start/stop the test to trigger the MDA
    StartTest(MODBALHIGH, TRUE);
    m_byNextAutoTuneOperation = MODBALHIGH_ANALYSIS;
    return;  // come back here upon WM_IO_OPLIST_DONE
  }
    
  ////////////  
  if (m_byNextAutoTuneOperation == MODBALHIGH_ANALYSIS)
  {
    // analyze the ModBalLow MDA capture...
    if (theHpMda.AnalyzeModBalHigh(&m_dModBalHighMean))
    { 
      m_bAutoTuneInProgress = FALSE;  // quit auto-tune on error
      OnStopTest();
      UpdateData(FALSE);      // display results to point of failure
      if (m_bAutoTuneModifications) 
        theHpMda.OnOpFailure("\nAutoTune : Tuning parameter(s) modified.\n");
      else
        theHpMda.OnOpFailure();
      return;              
    }
    dDelta = CHpMda::DoubleGetDelta(m_dModBalHighMean, m_dModBalLowMean);
    nStep = (int)(dDelta / 120.0);  // 120-150 Hz per step 
    nDiff = CHpMda::IntGetDelta((int)m_byModBalance, (int)(m_byModBalance+nStep)); 
    TRACE("CTheTuningDlg::HandleNextAutoTuneOperation : DELTA=%f : STEP=%d : DIFF=%d\n",
           dDelta, nStep, nDiff); 
    // adjust and re-measure if required...
    if ((nStep) && (nDiff > 1))  
    {
      if (m_dModBalHighMean > m_dModBalLowMean)
        m_byModBalance = (BYTE)(m_byModBalance - nStep);
      else
        m_byModBalance = (BYTE)(m_byModBalance + nStep); 
      m_bAutoTuneModifications = TRUE;
      m_byNextAutoTuneOperation = MODBALHIGH_CONFIG;  
      UpdateData(FALSE); 
      HandleNextAutoTuneOperation();  // adjust and retry
      return;
    }  
    // otherwise, consider mod balance tuned
    OnStopTest();
    UpdateData(FALSE);
    return;
  } 
  
  ////////////  
  if (m_byNextAutoTuneOperation == SYMBOLDEV_CONFIG) 
  {
    // configure the MDA for the initial ModBalLow sample.
    if (theHpMda.ConfigureSymbolDev(m_dCenterFreq))
    { 
      theHpMda.OnOpFailure();
      return;              
    } 
    // start/stop the test to trigger the MDA
    StartTest(TX_SPLT, TRUE, 20);
    m_byNextAutoTuneOperation = SYMBOLDEV_ANALYSIS;
    return;  // come back here upon WM_IO_OPLIST_DONE
  }
  
  ////////////
  if (m_byNextAutoTuneOperation == SYMBOLDEV_ANALYSIS) 
  { 
    double dNeg24Delta, dPos24Delta, dNeg8Delta, dPos8Delta;
    // analyze the SymbolDeviation MDA capture...  
    if (theHpMda.AnalyzeSymbolDev(&dNeg24Delta, &dPos24Delta, &dNeg8Delta, &dPos8Delta))
    { 
      m_bAutoTuneInProgress = FALSE;  // quit auto-tune on error
      OnStopTest();  
      if (m_bAutoTuneModifications)
        theHpMda.OnOpFailure("\nAutoTune : Tuning parameter(s) modified.\n");
      else
        theHpMda.OnOpFailure();
      UpdateData(FALSE);      // display results to point of failure 
      return;              
    } 
    // adjust symbol deviations and remeasure if required
    if ((DoSymbolDevAdjust(&m_byNeg2400deviation, dNeg24Delta)) ||
        (DoSymbolDevAdjust(&m_byPos2400deviation, dPos24Delta)) ||
        (DoSymbolDevAdjust(&m_byNeg800deviation, dNeg8Delta)) || 
        (DoSymbolDevAdjust(&m_byPos800deviation, dPos8Delta)))
    { 
      m_bAutoTuneModifications = TRUE;
      m_byNextAutoTuneOperation = SYMBOLDEV_CONFIG;  
      UpdateData(FALSE); 
      HandleNextAutoTuneOperation();  // retry measurement
      return;
    }    
    // otherwise, consider symbols tuned...    
    OnStopTest();
    UpdateData(FALSE); 
    theHpMda.DisplayHistogram(FALSE);    // revert to time display
    return;
  }
      
  return;
}



/////////////////////////////////////////////////////////////////////////////////////
//  DoSymbolDevAdjust()
//
//  Helper function for the SYMBOLDEV_ANALYSIS section of HandleNextAutoTuneOperation().
//  Returns 0 if no adjustment made, otherwise returns 1. dSymFreqDelta may be negative.
//
int CTheTuningDlg::DoSymbolDevAdjust(BYTE *pbySymDevDacSetting, double dSymFreqDelta)
{
    int 	nStep = (int)(dSymFreqDelta / 45.0);  // approx. 50 Hz per step 
    
    TRACE("\nCTheTuningDlg::DoSymbolDevAdjust : PARAM=%02X : DELTA=%fHz : STEP=%d\n",
           *pbySymDevDacSetting, dSymFreqDelta, nStep); 
    
    // adjust and re-measure if required...
    if (nStep)   
    {
      *pbySymDevDacSetting = (BYTE)(*pbySymDevDacSetting + nStep);  
      return(1);    // adjustment made
    }  
    return(0);   // no adjustment needed
}


#endif   // DISABLE_TUNING - conditional compilation of this file

