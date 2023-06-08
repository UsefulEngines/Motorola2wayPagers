//*************************************************************
//  History:    Date       Author               Comment
//              1996       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
// security.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSecurity form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

                                   
#ifndef _SECURITY_
#define _SECURITY_
                                  
                                   
class CSecurity : public CPpsFormView
{
  DECLARE_DYNCREATE(CSecurity) 
    BYTE		byCP_OTA_Protected_L;	// bit 7 is the rf_programming_disabled bit
    BYTE		byCP_System_Type_1_L;   // bits 3,2 are the dl_security bits (00=no pwd required)
	CString		m_szOtaPassword;
	CString		m_szReleasePassword;
	CString		m_szSerialAccessPassword;
    
  // Form Data
  public:
	//{{AFX_DATA(CSecurity)
	enum { IDD = IDD_SECURITY_FORM };
	BOOL	m_bOtaDisable;
	BOOL	m_bSerialAccessPwdRequired;
	//}}AFX_DATA

  // Operations
  public:
	CSecurity();			
    virtual void OnInitialUpdate();
  	virtual void SetFromDoc();
  	virtual BOOL SetToDoc();	
	//virtual void OnDefaults(void) { OnAcceptDefaultsBtn(); }
	virtual void OnUndo(void) { OnUndoBtn(); }
	//virtual void OnArchived(void);
  	virtual int OnPrintHelper(CStringArray &rStringArray, UINT nPageNumber, int nAvailableLines);
    #ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
    #endif
  
  private:
    void ChangePassword(int nField, int nFieldLen, CString& rCurrentPwd);
    int OnChangePassword(CString &rCurrentPwd);

  // Implementation
  protected:
	virtual ~CSecurity();
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    virtual void OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint);
	// Generated message map functions
	//{{AFX_MSG(CSecurity)
	afx_msg void OnUndoBtn();
	afx_msg void OnChgOtaPwdBtn();
	afx_msg void OnChgOtaReleasePwdBtn();
	afx_msg void OnChgSerialPwdBtn();
	afx_msg void OnSerialCheck();
	//}}AFX_MSG
	//afx_msg void OnAcceptDefaultsBtn();
	DECLARE_MESSAGE_MAP()
};


#endif   // _SECURITY_

/////////////////////////////////////////////////////////////////////////////
