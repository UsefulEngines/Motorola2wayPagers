//*************************************************************
//  History:    Date       Author               Comment
//              1996       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
// canmsgs.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCannedMessages form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
 
 
#ifndef _CANNEDMSGS_ 
#define _CANNEDMSGS_
 

                                   
class CCannedMessages : public CPpsFormView
{
  DECLARE_DYNCREATE(CCannedMessages) 
  
  int		m_nMaxNumMsgs;
    
  // Form Data
  public:
	//{{AFX_DATA(CCannedMessages)
	enum { IDD = IDD_CANNEDMSGS_FORM };
	CString	m_szCanMsgZero;
	CString	m_szCanMsgOne;
	CString	m_szCanMsgTwo;
	CString	m_szCanMsgThree;
	CString	m_szCanMsgFour;
	CString	m_szCanMsgFive;
	CString	m_szCanMsgSix;
	CString	m_szCanMsgSeven;
	CString	m_szCanMsgEight;
	CString	m_szCanMsgNine;
	CString	m_szCanMsgTen;
	CString	m_szCanMsgEleven;
	CString	m_szCanMsgTwelve;
	CString	m_szCanMsgThirteen;
	CString	m_szCanMsgFourteen;
	CString	m_szCanMsgFifteen; 
	BYTE	m_byDivFwdRev;
	BYTE	m_byDivIod;
	BOOL	m_bSendMsgEnable;
	BOOL	m_bEnableMultipleChoiceResponses;
	BYTE	m_byReplyAttemptLimit;
	//}}AFX_DATA

  private:
    void    DoWarnUserOfInvalidEntry(void);
    void	DoCannedMessageModify(int nResourceID, CString& rMsg, int nIndex);
    
  // Operations
  public:
	CCannedMessages();			
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

  #ifdef MTEL_AFFILIATE_PPS  
  private:
    void EditFieldEnable(UINT nResourceId, BOOL bEnable=FALSE); 
  #endif // MTEL_AFFILIATE_PPS
  
  // Implementation
  protected:
	virtual ~CCannedMessages();
    virtual void OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint);    
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	// Generated message map functions
	//{{AFX_MSG(CCannedMessages)
	afx_msg void OnAcceptDefaultsBtn();  
	afx_msg void OnUndoBtn();             
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif   // _CANNEDMSGS_
 
/////////////////////////////////////////////////////////////////////////////
