//*************************************************************
//  History:    Date       Author               Comment
//              1996       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
// pclink.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPCLink form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
 
 
#ifndef _PCLINK_ 
#define _PCLINK_
 

                                   
class CPCLink : public CPpsFormView
{
  DECLARE_DYNCREATE(CPCLink) 
    
  // Form Data
  public:
	//{{AFX_DATA(CPCLink)
	enum { IDD = IDD_PCLINK_FORM };
	BYTE	m_byDataFlagResignal;
	BYTE	m_byDataFlagTimer;
	BOOL	m_bSendErrMsgCnt;
	BOOL	m_bSendInventoryControlNum;
	BOOL	m_bNonProtocolMode;
	BOOL	m_bEnableDataFlags;
	BOOL	m_bSendSource;
	BOOL	m_bSendTimeDate;
	BYTE	m_byDataFlagCharacter;
	//}}AFX_DATA

  // Operations
  public:
	CPCLink();			
    virtual void OnInitialUpdate();   
  	virtual void SetFromDoc();                  
  	virtual BOOL SetToDoc();	                
	virtual void OnDefaults(void) { OnAcceptDefaultsBtn(); }
	virtual void OnUndo(void) { OnUndoBtn(); }
	virtual void OnArchived(void);
    static BOOL IsPresentable(void);  	
    #ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
    #endif
  
  // Implementation
  protected:
	virtual ~CPCLink();
    virtual void OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint);    
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	// Generated message map functions
	//{{AFX_MSG(CPCLink)
	afx_msg void OnAcceptDefaultsBtn();  
	afx_msg void OnUndoBtn();             
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif   // _PCLINK_
 
/////////////////////////////////////////////////////////////////////////////
