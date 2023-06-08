//*************************************************************
//  History:    Date       Author               Comment
//              1997       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
// proto25.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProto25View form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif


#ifndef _PROTO25VIEW_
#define _PROTO25VIEW_


#include "..\..\misc\spinbtn\spinbtn.h"                                  
#include "..\..\misc\numedit.h"
                                   
                                   
class CProto25View : public CPpsFormView
{
  DECLARE_DYNCREATE(CProto25View) 
  
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
  CNumericEdit		m_F8_FreqEdit;
  CNumericEdit		m_F9_FreqEdit;
  CNumericEdit		m_F10_FreqEdit;
  CNumericEdit		m_F11_FreqEdit;
  CNumericEdit		m_F12_FreqEdit;
  CNumericEdit		m_F13_FreqEdit;
  CNumericEdit		m_F14_FreqEdit;
  CNumericEdit		m_F15_FreqEdit;
  CNumericEdit 		m_F0_ScanDelayEdit;
  CNumericEdit 		m_F1_ScanDelayEdit;
  CNumericEdit 		m_F2_ScanDelayEdit;
  CNumericEdit 		m_F3_ScanDelayEdit;
  CNumericEdit 		m_F4_ScanDelayEdit;
  CNumericEdit 		m_F5_ScanDelayEdit;
  CNumericEdit 		m_F6_ScanDelayEdit;
  CNumericEdit 		m_F7_ScanDelayEdit;
  CNumericEdit 		m_F8_ScanDelayEdit;
  CNumericEdit 		m_F9_ScanDelayEdit;
  CNumericEdit 		m_F10_ScanDelayEdit;
  CNumericEdit 		m_F11_ScanDelayEdit;
  CNumericEdit 		m_F12_ScanDelayEdit;
  CNumericEdit 		m_F13_ScanDelayEdit;
  CNumericEdit 		m_F14_ScanDelayEdit;
  CNumericEdit 		m_F15_ScanDelayEdit;

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

  // *** Row #8  
  CSpinBtn			m_F8_ChNumUpBtn;
  CSpinBtn			m_F8_ChNumDnBtn;
  CSpinBtn			m_F8_DelayUpBtn;
  CSpinBtn			m_F8_DelayDnBtn;

  // *** Row #9  
  CSpinBtn			m_F9_ChNumUpBtn;
  CSpinBtn			m_F9_ChNumDnBtn;
  CSpinBtn			m_F9_DelayUpBtn;
  CSpinBtn			m_F9_DelayDnBtn;

  // *** Row #10  
  CSpinBtn			m_F10_ChNumUpBtn;
  CSpinBtn			m_F10_ChNumDnBtn;
  CSpinBtn			m_F10_DelayUpBtn;
  CSpinBtn			m_F10_DelayDnBtn;

  // *** Row #11  
  CSpinBtn			m_F11_ChNumUpBtn;
  CSpinBtn			m_F11_ChNumDnBtn;
  CSpinBtn			m_F11_DelayUpBtn;
  CSpinBtn			m_F11_DelayDnBtn;

  // *** Row #12  
  CSpinBtn			m_F12_ChNumUpBtn;
  CSpinBtn			m_F12_ChNumDnBtn;
  CSpinBtn			m_F12_DelayUpBtn;
  CSpinBtn			m_F12_DelayDnBtn;

  // *** Row #13  
  CSpinBtn			m_F13_ChNumUpBtn;
  CSpinBtn			m_F13_ChNumDnBtn;
  CSpinBtn			m_F13_DelayUpBtn;
  CSpinBtn			m_F13_DelayDnBtn;

  // *** Row #14  
  CSpinBtn			m_F14_ChNumUpBtn;
  CSpinBtn			m_F14_ChNumDnBtn;
  CSpinBtn			m_F14_DelayUpBtn;
  CSpinBtn			m_F14_DelayDnBtn;

  // *** Row #15  
  CSpinBtn			m_F15_ChNumUpBtn;
  CSpinBtn			m_F15_ChNumDnBtn;
  CSpinBtn			m_F15_DelayUpBtn;
  CSpinBtn			m_F15_DelayDnBtn;


  int		m_nF0_ChanNum;  
  int		m_nF1_ChanNum;
  int		m_nF2_ChanNum;
  int		m_nF3_ChanNum;
  int		m_nF4_ChanNum;
  int		m_nF5_ChanNum;
  int		m_nF6_ChanNum;
  int		m_nF7_ChanNum;
  int		m_nF8_ChanNum;  
  int		m_nF9_ChanNum;
  int		m_nF10_ChanNum;
  int		m_nF11_ChanNum;
  int		m_nF12_ChanNum;
  int		m_nF13_ChanNum;
  int		m_nF14_ChanNum;
  int		m_nF15_ChanNum;
  
  // Form Data
  public:
	//{{AFX_DATA(CProto25View)
	enum { IDD = IDD_PROTO25_FORM };
	BOOL	m_bF0_Disable;
	BOOL	m_bF1_Disable;
	BOOL	m_bF2_Disable;
	BOOL	m_bF3_Disable;
	BOOL	m_bF4_Disable;
	BOOL	m_bF5_Disable;
	BOOL	m_bF6_Disable;
	BOOL	m_bF7_Disable;
	BOOL	m_bF8_Disable;
	BOOL	m_bF9_Disable;
	BOOL	m_bF10_Disable;
	BOOL	m_bF11_Disable;
	BOOL	m_bF12_Disable;
	BOOL	m_bF13_Disable;
	BOOL	m_bF14_Disable;
	BOOL	m_bF15_Disable;
	double	m_dF0_ScanFreq;
	double	m_dF1_ScanFreq;
	double	m_dF2_ScanFreq;
	double	m_dF3_ScanFreq;
	double	m_dF4_ScanFreq;
	double	m_dF5_ScanFreq;
	double	m_dF6_ScanFreq;
	double	m_dF7_ScanFreq;
	double	m_dF8_ScanFreq;
	double	m_dF9_ScanFreq;
	double	m_dF10_ScanFreq;
	double	m_dF11_ScanFreq;
	double	m_dF12_ScanFreq;
	double	m_dF13_ScanFreq;
	double	m_dF14_ScanFreq;
	double	m_dF15_ScanFreq;
	int		m_nF0_ScanDelay;
	int		m_nF1_ScanDelay;
	int		m_nF2_ScanDelay;
	int		m_nF3_ScanDelay;
	int		m_nF4_ScanDelay;
	int		m_nF5_ScanDelay;
	int		m_nF6_ScanDelay;
	int		m_nF7_ScanDelay;
	int		m_nF8_ScanDelay;
	int		m_nF9_ScanDelay;
	int		m_nF10_ScanDelay;
	int		m_nF11_ScanDelay;
	int		m_nF12_ScanDelay;
	int		m_nF13_ScanDelay;
	int		m_nF14_ScanDelay;
	int		m_nF15_ScanDelay;
	//}}AFX_DATA

  // Operations
  public:
	CProto25View();			
    virtual void OnInitialUpdate();
  	virtual void SetFromDoc();
  	virtual BOOL SetToDoc();	
	virtual void OnDefaults(void) { OnAcceptDefaultsBtn(); }
	virtual void OnUndo(void) { OnUndoBtn(); }
	virtual void OnArchived(void);
  	virtual int OnPrintHelper(CStringArray &rStringArray, UINT nPageNumber, int nAvailableLines);
    #ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
    #endif
  
  private:
    enum { MIN_FWD_CHAN=0x00, MAX_FWD_CHAN=0x7FF };
  	enum { NO_ROUNDING=0, ROUND_UP=1, ROUND_DN=2  };
 
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
	virtual ~CProto25View();
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    virtual void OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint);
	// Generated message map functions
	//{{AFX_MSG(CProto25View)
	afx_msg void OnAcceptDefaultsBtn();
	afx_msg void OnUndoBtn();
	//}}AFX_MSG
	afx_msg LONG OnWmSpinButton(UINT, LONG);
	DECLARE_MESSAGE_MAP()
};


#endif   // _PROTO25VIEW_

/////////////////////////////////////////////////////////////////////////////
