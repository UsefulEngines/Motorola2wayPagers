//*************************************************************
//  History:    Date       Author               Comment
//              1996       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
// options.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptions form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
 
 
#ifndef _OPTIONS_ 
#define _OPTIONS_ 
                                   
class COptions : public CPpsFormView
{
  DECLARE_DYNCREATE(COptions) 
    
  CSpinBtn			m_MsgReadUpBtn;
  CSpinBtn			m_MsgReadDnBtn;

  CSpinBtn			m_AckMsgPctgUpBtn;
  CSpinBtn			m_AckMsgPctgDnBtn;
  
  BOOL				m_bR50Enabled;
  
  private:
    enum { MIN_ACKMSGPCTG=0, MAX_ACKMSGPCTG=20 };
  	BYTE			m_byAckMsgPctg;
  	
  // Form Data
  public:
	//{{AFX_DATA(COptions)
	enum { IDD = IDD_OPTIONS_FORM };
	BOOL	m_bInternationalDate;
	CString	m_szMessageReadResponseMode;
	float	m_fAckMsgPctg;
	BOOL	m_bDisplayTimestamp;
	BOOL	m_bPowerOnRamCheck;
	//}}AFX_DATA

  // Operations
  public:
	COptions();			
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
	virtual ~COptions();
    virtual void OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint);    
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	// Generated message map functions
	//{{AFX_MSG(COptions)
	afx_msg void OnAcceptDefaultsBtn();  
	afx_msg void OnUndoBtn();             
	//}}AFX_MSG
	afx_msg LONG OnWmSpinButton(UINT, LONG);
	DECLARE_MESSAGE_MAP()
};


#endif   // _OPTIONS_
 
/////////////////////////////////////////////////////////////////////////////
