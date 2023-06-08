//*************************************************************
//  History:    Date       Author               Comment
//              1997       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
// opts25.h : Reflex25 misc/options header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionsR25 form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
 
 
#ifndef _OPTIONS25_ 
#define _OPTIONS25_ 
                                   
class COptionsR25 : public CPpsFormView
{
  DECLARE_DYNCREATE(COptionsR25) 
    
  CSpinBtn			m_MsgReadUpBtn;
  CSpinBtn			m_MsgReadDnBtn;

  CSpinBtn			m_AckMsgPctgUpBtn;
  CSpinBtn			m_AckMsgPctgDnBtn;
  
  private:
    enum { MIN_ACKMSGPCTG=0, MAX_ACKMSGPCTG=20 };
  	BYTE			m_byAckMsgPctg;
  	
  // Form Data
  public:
	//{{AFX_DATA(COptionsR25)
	enum { IDD = IDD_OPTIONS_R25_FORM };
	BOOL	m_bInternationalDate;
	CString	m_szMessageReadResponseMode;
	float	m_fAckMsgPctg;
	BOOL	m_bDisplayTimestamp;
	BOOL	m_bPowerOnRamCheck;
	BYTE	m_byRegFailRestTime;
	BYTE	m_byAsyncSleepFrames;
	//}}AFX_DATA

  // Operations
  public:
	COptionsR25();			
    virtual void OnInitialUpdate();   
  	virtual void SetFromDoc();                  
  	virtual BOOL SetToDoc();	                
	virtual void OnDefaults(void) { OnAcceptDefaultsBtn(); }
	virtual void OnUndo(void) { OnUndoBtn(); }
	virtual void OnArchived(void);
  	virtual int OnPrintHelper(CStringArray &rStringArray,  UINT nPageNumber, int nAvailableLines);
    #ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
    #endif
  
  // Implementation
  protected:
	virtual ~COptionsR25();
    virtual void OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint);    
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	// Generated message map functions
	//{{AFX_MSG(COptionsR25)
	afx_msg void OnAcceptDefaultsBtn();  
	afx_msg void OnUndoBtn();             
	//}}AFX_MSG
	afx_msg LONG OnWmSpinButton(UINT, LONG);
	DECLARE_MESSAGE_MAP()
};


#endif   // _OPTIONSR25_
 
/////////////////////////////////////////////////////////////////////////////
