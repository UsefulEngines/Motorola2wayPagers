//*************************************************************
//  History:    Date       Author               Comment
//              1996       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
// dwnldpwd.h : header file
//

 
#ifndef _DWNLDPWD_
#define _DWNLDPWD_ 
 
 
/////////////////////////////////////////////////////////////////////////////
// CDwnLdPwd dialog

class CDwnLdPwd : public CDialog
{
  // Construction
  public:
	CDwnLdPwd(CWnd* pParent = NULL);	// standard constructor

  // Dialog Data
	//{{AFX_DATA(CDwnLdPwd)
	enum { IDD = IDD_PASSWORD_DLG };
	CString	m_Edit1;
	CString	m_Edit2;
	CString	m_szCurrentPwd;
	//}}AFX_DATA

    virtual int DoModal(const char *pCurrentPwd)
    {
      m_szCurrentPwd = pCurrentPwd; 
      return(CDialog::DoModal());
    }
    virtual int DoModal(void)
    {
      return(CDialog::DoModal());
    }
    
  // Implementation
  protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CDwnLdPwd)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnClickedOk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


#endif   // _DWNLDPWD_