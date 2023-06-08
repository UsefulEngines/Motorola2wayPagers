//*************************************************************
//  History:    Date       Author               Comment
//              1996       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
// alerts.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAlerts form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
 
 
#ifndef _ALERTS_ 
#define _ALERTS_ 

                                   
class CAlerts : public CPpsFormView
{
  DECLARE_DYNCREATE(CAlerts) 
    
  CSpinBtn			m_AlertDurationUpBtn;
  CSpinBtn			m_AlertDurationDnBtn; 
  CSpinBtn			m_InactivityUpBtn;
  CSpinBtn			m_InactivityDnBtn;
  CSpinBtn			m_ScrollRateUpBtn;
  CSpinBtn			m_ScrollRateDnBtn;
  CSpinBtn			m_UnReadMsgRemindUpBtn;
  CSpinBtn			m_UnReadMsgRemindDnBtn;
  CSpinBtn			m_AlertTimeOutUpBtn;
  CSpinBtn			m_AlertTimeOutDnBtn;
  CSpinBtn			m_StandardAlertFreqUpBtn;
  CSpinBtn			m_StandardAlertFreqDnBtn;
  
  BYTE				byProgAlert[16];   // pleasing alert #7
  
  // Form Data
  public:
	//{{AFX_DATA(CAlerts)
	enum { IDD = IDD_ALERTS_FORM };
	CString	m_szAlertDuration;
	BOOL	m_bEnableAllAlerts;
	BOOL	m_bEnableAudibleAlerts;
	BOOL	m_bEnableLowCellAlert;
	BOOL	m_bEnableMemoryFullAlert;
	BOOL	m_bEnableOutOfRangeIndicator;
	BOOL	m_bEnableVibrator;
	int		m_nReminderAlertInterval;
	int		m_nDisplayReadTimeout;
	int		m_nReminderTimeoutPeriod;
	float	m_fScrollRate;
	int		m_nStandardAlertFrequency;
	BOOL	m_bSilentUpdate;
	BOOL	m_bOneWayPrompt;
	BOOL	m_bTwoWayPrompt;
	BOOL	m_bOutOfRangePrompt;
	CString	m_szOneWayPrompt;
	CString	m_szTwoWayPrompt;
	CString	m_szOutOfRangePrompt;
	//}}AFX_DATA

  // Operations
  public:
	CAlerts();			
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
    
  // Implementation
  protected:
	virtual ~CAlerts();
    virtual void OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint);    
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	// Generated message map functions
	//{{AFX_MSG(CAlerts)
	afx_msg void OnAcceptDefaultsBtn();  
	afx_msg void OnUndoBtn();
	afx_msg void OnProgAlertBtn();
	//}}AFX_MSG
	afx_msg LONG OnWmSpinButton(UINT, LONG);
	DECLARE_MESSAGE_MAP()
};


#endif   // _ALERTS_
 
/////////////////////////////////////////////////////////////////////////////
