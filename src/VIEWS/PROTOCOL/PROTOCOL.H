//*************************************************************
//  History:    Date       Author               Comment
//              1996       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
// protocol.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CReflex25Protocol form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif


#ifndef _PROTOCOL_
#define _PROTOCOL_


#include "..\..\misc\spinbtn\spinbtn.h"                                  
#include "..\..\misc\numedit.h"
                                   
                                   
class CReflex25Protocol : public CPpsFormView
{
  DECLARE_DYNCREATE(CReflex25Protocol) 
  
  //// **** NUMERIC EDIT CONTROLS
  // the following CEdit controls accept only numeric input 
  CNumericEdit		m_F0_FreqEdit;
  CNumericEdit		m_F1_FreqEdit;
  CNumericEdit		m_F2_FreqEdit;
  CNumericEdit		m_F3_FreqEdit;
  CNumericEdit		m_F4_FreqEdit;
  CNumericEdit		m_F5_FreqEdit;
  CNumericEdit		m_F6_FreqEdit;
  CNumericEdit		m_F7_FreqEdit;
  CNumericEdit 		m_F0_ScanDelayEdit;
  CNumericEdit 		m_F1_ScanDelayEdit;
  CNumericEdit 		m_F2_ScanDelayEdit;
  CNumericEdit 		m_F3_ScanDelayEdit;
  CNumericEdit 		m_F4_ScanDelayEdit;
  CNumericEdit 		m_F5_ScanDelayEdit;
  CNumericEdit 		m_F6_ScanDelayEdit;
  CNumericEdit 		m_F7_ScanDelayEdit;

  CNumericEdit		HomeIndexEdit;
  CNumericEdit		SvcProvEdit0, SvcProvEdit1, SvcProvEdit2, SvcProvEdit3;
  
  ////
  //// **** SPINBTN CONTROLS
  // *** Row #0  
  CSpinBtn			m_F0_ChNumUpBtn;
  CSpinBtn			m_F0_ChNumDnBtn;
  CSpinBtn			m_F0_DelayUpBtn;
  CSpinBtn			m_F0_DelayDnBtn;

  // *** Row #1  
  CSpinBtn			m_F1_ChNumUpBtn;
  CSpinBtn			m_F1_ChNumDnBtn;
  CSpinBtn			m_F1_DelayUpBtn;
  CSpinBtn			m_F1_DelayDnBtn;

  // *** Row #2  
  CSpinBtn			m_F2_ChNumUpBtn;
  CSpinBtn			m_F2_ChNumDnBtn;
  CSpinBtn			m_F2_DelayUpBtn;
  CSpinBtn			m_F2_DelayDnBtn;

  // *** Row #3  
  CSpinBtn			m_F3_ChNumUpBtn;
  CSpinBtn			m_F3_ChNumDnBtn;
  CSpinBtn			m_F3_DelayUpBtn;
  CSpinBtn			m_F3_DelayDnBtn;

  // *** Row #4  
  CSpinBtn			m_F4_ChNumUpBtn;
  CSpinBtn			m_F4_ChNumDnBtn;
  CSpinBtn			m_F4_DelayUpBtn;
  CSpinBtn			m_F4_DelayDnBtn;

  // *** Row #5  
  CSpinBtn			m_F5_ChNumUpBtn;
  CSpinBtn			m_F5_ChNumDnBtn;
  CSpinBtn			m_F5_DelayUpBtn;
  CSpinBtn			m_F5_DelayDnBtn;

  // *** Row #6  
  CSpinBtn			m_F6_ChNumUpBtn;
  CSpinBtn			m_F6_ChNumDnBtn;
  CSpinBtn			m_F6_DelayUpBtn;
  CSpinBtn			m_F6_DelayDnBtn;

  // *** Row #7  
  CSpinBtn			m_F7_ChNumUpBtn;
  CSpinBtn			m_F7_ChNumDnBtn;
  CSpinBtn			m_F7_DelayUpBtn;
  CSpinBtn			m_F7_DelayDnBtn;

  int		m_nF0_ChanNum;  
  int		m_nF1_ChanNum;
  int		m_nF2_ChanNum;
  int		m_nF3_ChanNum;
  int		m_nF4_ChanNum;
  int		m_nF5_ChanNum;
  int		m_nF6_ChanNum;
  int		m_nF7_ChanNum;
  
  // *** these fields are no longer modified from this view - 10/25/96
  //BOOL	    m_bSvcProv0Disable;
  //BOOL		m_bSvcProv0Ancillary;
  //BOOL		m_bSvcProv1Ancillary;
  //BOOL		m_bSvcProv2Ancillary;
  //BOOL		m_bSvcProv3Ancillary;
  
  // Form Data
  public:
	//{{AFX_DATA(CReflex25Protocol)
	enum { IDD = IDD_PROTOCOL_FORM };
	BOOL	m_bF0_Disable;
	BOOL	m_bF1_Disable;
	BOOL	m_bF2_Disable;
	BOOL	m_bF3_Disable;
	BOOL	m_bF4_Disable;
	BOOL	m_bF5_Disable;
	BOOL	m_bF6_Disable;
	BOOL	m_bF7_Disable;
	double	m_dF0_ScanFreq;
	double	m_dF1_ScanFreq;
	double	m_dF2_ScanFreq;
	double	m_dF3_ScanFreq;
	double	m_dF4_ScanFreq;
	double	m_dF5_ScanFreq;
	double	m_dF6_ScanFreq;
	double	m_dF7_ScanFreq;
	int		m_nF0_ScanDelay;
	int		m_nF1_ScanDelay;
	int		m_nF2_ScanDelay;
	int		m_nF3_ScanDelay;
	int		m_nF4_ScanDelay;
	int		m_nF5_ScanDelay;
	int		m_nF6_ScanDelay;
	int		m_nF7_ScanDelay;
	DWORD	m_dwHomeIndex;
	UINT	m_nSvcProvId0;
	UINT	m_nSvcProvId1;
	UINT	m_nSvcProvId2;
	UINT	m_nSvcProvId3;
	BOOL	m_bSvcProv1Disable;
	BOOL	m_bSvcProv2Disable;
	BOOL	m_bSvcProv3Disable;
	BYTE	m_byRegFailRestTime;
	BYTE	m_byAsyncSleepFrames;
	//}}AFX_DATA

  // Operations
  public:
	CReflex25Protocol();			
    virtual void OnInitialUpdate();
  	virtual void SetFromDoc();
  	virtual BOOL SetToDoc();	
	virtual void OnDefaults(void) { OnAcceptDefaultsBtn(); }
	virtual void OnUndo(void) { OnUndoBtn(); }
	virtual void OnArchived(void);
  	virtual int OnPrintHelper(CStringArray &rStringArray, UINT nPageNumber, int nAvailableLines);
    static BOOL IsPresentable(void); 	
    #ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
    #endif
  
  private:
    enum { MIN_FWD_CHAN=0x00, MAX_FWD_CHAN=0x7FF };
  	enum { NO_ROUNDING=0, ROUND_UP=1, ROUND_DN=2  };
 
  public:   // make these accessible to all classes in the application
  	static double dCalcForwardChanFreq(int nChanNum)
  	  {  return(929.00625 + (0.00625 * nChanNum)); } 
  	static int nCalcForwardChanNum(double dFwdFreq, BYTE byRounding = NO_ROUNDING);
    static double dCalcReverseChanFreq(UINT nChanNum)
      {  return(896.0125 + (0.00625 * nChanNum)); }
    static int nCalcReverseChanNum(double dRevFreq, BYTE byRounding = NO_ROUNDING);
    
  private: 
    void OnChannelNumberIncrement(int *pnChanNum, 
    							  double *pdFrequency,
    							  int nOffsetBy = 1);
    void OnChannelNumberDecrement(int *pnChanNum, 
    							  double *pdFrequency,
    							  int nOffsetBy = 1);
    void OnScanDelayIncrement(int *pnScanDelay, 
    						  int nOffsetBy = 1);
    void OnScanDelayDecrement(int *pnScanDelay, 
    						  int nOffsetBy = 1);
    void SaveControlChannel(int nChannel, BOOL bDisabled, int nScanTime, int *nIndex);
  
  // Implementation
  protected:
	virtual ~CReflex25Protocol();
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    virtual void OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint);
	// Generated message map functions
	//{{AFX_MSG(CReflex25Protocol)
	afx_msg void OnAcceptDefaultsBtn();
	afx_msg void OnUndoBtn();
	//}}AFX_MSG
	afx_msg LONG OnWmSpinButton(UINT, LONG);
	DECLARE_MESSAGE_MAP()
};


#endif   // _PROTOCOL_

/////////////////////////////////////////////////////////////////////////////
