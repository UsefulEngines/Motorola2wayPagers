//*************************************************************
//  History:    Date       Author               Comment
//              1996       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
// messages.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMessages form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif


#ifndef _MESSAGES_
#define _MESSAGES_
                                   
class CMessages : public CPpsFormView
{
  DECLARE_DYNCREATE(CMessages) 
    
  // Form Data
  public:
	//{{AFX_DATA(CMessages)
	enum { IDD = IDD_MESSAGES_FORM };
	BOOL	m_bDuplicateMsgCheck;
	CString	m_szFormatDelimChar;
	BYTE	m_byDelimitPosFour;
	BYTE	m_byDelimitPosOne;
	BYTE	m_byDelimitPosThree;
	BYTE	m_byDelimitPosTwo;
	BOOL	m_bRetainErrOneWayAlphaNumeric;
	BOOL	m_bRetainErrOneWayNumeric;
	BOOL	m_bRetainErrOneWayXParent;
	BOOL	m_bRetainErrTwoWayMsgs;
	BYTE	m_byMsgUpdateTimeout;
	//}}AFX_DATA

  // Operations
  public:
	CMessages();			
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
	virtual ~CMessages();
    virtual void OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint);    
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	// Generated message map functions
	//{{AFX_MSG(CMessages)
	afx_msg void OnAcceptDefaultsBtn();  
	afx_msg void OnUndoBtn();             
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

 
 
#endif  // _MESSAGES_
 
/////////////////////////////////////////////////////////////////////////////
