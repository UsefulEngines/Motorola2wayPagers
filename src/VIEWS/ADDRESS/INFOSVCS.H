//*************************************************************
//  History:    Date       Author               Comment
//              1996       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
// infosvcs.h : header file
//
 
#ifndef _INFOSVCS_
#define _INFOSVCS_ 
 
 
/////////////////////////////////////////////////////////////////////////////
// CInfoSvcSubAddrDlg dialog

class CInfoSvcSubAddrDlg : public CDialog
{
	DWORD	*pdwSubAddrs;
	
  // Construction
  public:
	CInfoSvcSubAddrDlg(CWnd* pParent = NULL);	// standard constructor
    virtual int DoModal(DWORD *pdwSubs)
    {
      pdwSubAddrs = pdwSubs; 
      return(CDialog::DoModal());
    }

  // Dialog Data
	//{{AFX_DATA(CInfoSvcSubAddrDlg)
	enum { IDD = IDD_SUBADDR_DLG };
	BOOL	m_bSub0;
	BOOL	m_bSub1;
	BOOL	m_bSub10;
	BOOL	m_bSub11;
	BOOL	m_bSub12;
	BOOL	m_bSub13;
	BOOL	m_bSub14;
	BOOL	m_bSub15;
	BOOL	m_bSub16;
	BOOL	m_bSub17;
	BOOL	m_bSub18;
	BOOL	m_bSub19;
	BOOL	m_bSub2;
	BOOL	m_bSub20;
	BOOL	m_bSub21;
	BOOL	m_bSub22;
	BOOL	m_bSub23;
	BOOL	m_bSub24;
	BOOL	m_bSub25;
	BOOL	m_bSub26;
	BOOL	m_bSub27;
	BOOL	m_bSub28;
	BOOL	m_bSub29;
	BOOL	m_bSub3;
	BOOL	m_bSub30;
	BOOL	m_bSub31;
	BOOL	m_bSub4;
	BOOL	m_bSub5;
	BOOL	m_bSub6;
	BOOL	m_bSub7;
	BOOL	m_bSub8;
	BOOL	m_bSub9;
	//}}AFX_DATA

  // Implementation
  protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CInfoSvcSubAddrDlg)
	virtual void OnOK();
	afx_msg void OnDisableBtn();
	afx_msg void OnEnableBtn();
	afx_msg void OnAcceptChanges();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg LRESULT OnHelpHitTest(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};


#endif   // _INFOSVCS_ 