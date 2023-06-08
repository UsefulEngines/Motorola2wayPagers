//*************************************************************
//  History:    Date       Author               Comment
//              1996       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
// tune.h : header file
//
// NOTE:  With codeplug version 0x05 of the Pegasus/Centaur
// product family, the quick-tune functionality was removed.
// The corresponding host release is CENTAUR_FHa01.12.  
// Consequently, this file and the corresponding .cpp file
// implement a tuning procedure to replace the quick-tune 
// method.


#ifndef _TUNINGDLG_
#define _TUNINGDLG_      

// Note:  The following test identifiers may not actually equal
// the test number as indicated in the Factory Test/Download 
// Document.  The test identifiers in that document have changed
// with time.  The actual test number is selected from a database
// in the 'tune.cpp' file.
//
typedef enum { REF_OSC=0x05, 
               SILENT_CARRIER=0x07,
               MODBALLOW=0x08, 
               MODBALHIGH=0x09,
               TX_ON=0x02, 
               RCVR_ON=0x00,
               TX_SPLT=0x11 } TUNING_TEST;


#ifndef DISABLE_TUNING    // conditional compilation of this file


#include "..\..\misc\spinbtn\spinbtn.h"
#include "..\..\misc\numedit.h"
#include "..\..\cpager\pegasus.h"  


/////////////////////////////////////////////////////////////////////////////
// CTheTuningDlg dialog  


class CTheTuningDlg : public CDialog
{  
  CSpinBtn			m_CtrFreqUP;
  CSpinBtn			m_CtrFreqDN;
  
  CSpinBtn			m_ModRateUP;
  CSpinBtn			m_ModRateDN;
  
  CSpinBtn			m_RefOscUP;
  CSpinBtn			m_RefOscDN; 

  CSpinBtn			m_ModBalUP;
  CSpinBtn			m_ModBalDN; 

  CSpinBtn			m_Pos2400UP;
  CSpinBtn			m_Pos2400DN; 

  CSpinBtn			m_Pos800UP;
  CSpinBtn			m_Pos800DN; 

  CSpinBtn			m_Neg800UP;
  CSpinBtn			m_Neg800DN; 

  CSpinBtn			m_Neg2400UP;
  CSpinBtn			m_Neg2400DN; 
  
  CNumericEdit		m_RefOscEdit;
  CNumericEdit		m_ModBalEdit;
  CNumericEdit		m_Pos2400Edit;
  CNumericEdit		m_Pos800Edit;
  CNumericEdit		m_Neg800Edit;
  CNumericEdit		m_Neg2400Edit;
  
  private:
    enum { MIN_CNTR_FREQ=0, MAX_CNTR_FREQ=4096 };
    enum { HZ_MIN_MOD_RATE=100, HZ_MAX_MOD_RATE=4800, BPS_MIN_MOD_RATE = 0, BPS_MAX_MOD_RATE = 3 };   
    
    // test configuration and control parameters
	BOOL	m_bReflex50Enabled;	 			// reflex50 enabled?
    BYTE	m_byTestId;          			// selected test 
    int		m_nCenterFreqAdjust; 			// tf field of TestModeControl Register
    int		m_nRxCenterFrqSave;				// tf field for RxTests save for switching
    int		m_nTxCenterFrqSave;				// tf field for TxTests save for switching
    int		m_nModulationRate;   			// td field of TestModeControl Register
    int		m_nMinModRate, m_nMaxModRate;   // modulation rate range
    BOOL	m_bTestInProgress;				// indication of test running 
    BOOL	m_bTestTriggered;				// indication of test trigger requested 
	BYTE	m_byRefOsc;                     
	BYTE	m_byModBalance;
	BYTE	m_byNeg2400deviation;
	BYTE	m_byNeg800deviation;
	BYTE	m_byPos2400deviation;
	BYTE	m_byPos800deviation;  
	
	// auto-tune control parameters  
	enum { NONE, 
	       MODBALLOW_CONFIG, MODBALLOW_ANALYSIS, 
	       MODBALHIGH_CONFIG, MODBALHIGH_ANALYSIS,
	       SYMBOLDEV_CONFIG, SYMBOLDEV_ANALYSIS };
    BOOL	m_bAutoTuneInProgress;			// indication of auto-tune running
    BYTE	m_byNextAutoTuneOperation;		// indication of next auto-tune operation 
    double	m_dModBalLowMean;				// ModBalLow analysis result
    double  m_dModBalHighMean;				// ModBalHigh analysis result
    BOOL	m_bAutoTuneModifications;		// indication of autotune config changes
	
    // DSP/ASIC register data - see 'pegasus.h'
    BYTE	byTestNum;							   // Tuning Test Number
    BYTE    byDspAsicDownloadMethod;               // 0=NoRegs, 1=One Shot, 2=Update On-the-Fly
    CTestModeControlRegister	TestModeControlRegister;
    CTransmitRegister			TransmitRegister;
    CReceiveRegister			ReceiveRegister;
    CTransmitWarmupRegister		TransmitWarmupRegister;	
    
    // dialog controls enable/disable flags
    DWORD	m_dwPermDisabledControls;    // disabled for life of dialog
    DWORD	m_dwTempDisabledControls;    // disabled only during test or temporarily

  // Construction
  public:
	CTheTuningDlg(CWnd* pParent = NULL); // standard constructor
    virtual int DoModal(BYTE byTestId);

  // Dialog Data
	//{{AFX_DATA(CTheTuningDlg)
	enum { IDD = IDD_THETUNING_DLG };
	double	m_dCenterFreq;
	CString	m_szModulationRate;
	//}}AFX_DATA

  private: 
    void	StartTest(BYTE byTestId, BOOL bTrigger = FALSE, int nTriggerDelay=5);
    int 	DoPreTuneAnalysis(BYTE byTestId);
    void 	EnableDisableControls(void);
  	double 	dCalcForwardChanFreq(UINT nChanNum);
    int    	nCalcForwardChanNum(double dFwdFreq, BYTE byRounding = 0);
  	double 	dCalcReverseChanFreq(UINT nChanNum);
    int    	nCalcReverseChanNum(double dRevFreq, BYTE byRounding = 0);
    void   	HandleCenterFreqUp(int nAcceleration);
    void   	HandleCenterFreqDn(int nAcceleration);
    void   	OnSwitchTest(BYTE byNewTestId); 
    void	HandleNextAutoTuneOperation(void);
	int		DoSymbolDevAdjust(BYTE *pbySymDevDacSetting, double dSymFreqDelta);
	
  // Implementation
  protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    
	// Generated message map functions
	//{{AFX_MSG(CTheTuningDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnAccept();
	afx_msg void OnStopTest();
	virtual void OnCancel();
	afx_msg void OnUndoBtn();
	afx_msg void OnUseDefaultBtn();
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnTuningHelp();
	afx_msg void OnStartTest();
	afx_msg void OnNextTest();
	afx_msg void OnPrevTest();
	afx_msg void OnKillfocusModbalEdit();
	afx_msg void OnKillfocusNeg2400Edit();
	afx_msg void OnKillfocusNeg800Edit();
	afx_msg void OnKillfocusPos2400Edit();
	afx_msg void OnKillfocusPos800Edit();
	afx_msg void OnKillfocusRefoscEdit();
	afx_msg void OnAutotuneBtn();
	afx_msg void OnStartStopBurst();
	//}}AFX_MSG
	afx_msg LONG OnWmSpinStart(UINT, LONG);
	afx_msg LONG OnWmSpinButton(UINT, LONG);
	afx_msg LONG OnWmSpinStop(UINT, LONG);
	afx_msg LONG OnWmIoOpListDone(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()          
	
  friend class CHpMda;	// allow the MDA full access to this class
};


#endif  // DISABLE_TUNING - conditional compilation of this file


#endif  // _TUNINGDLG_

