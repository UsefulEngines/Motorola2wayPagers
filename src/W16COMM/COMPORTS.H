//*************************************************************
//  File name: COMPORTS.H
//
//  Description:  
//      Declaration file for the CComPortSettings class.
//
//
//  Comments:
//      
//
//  History:    Date       Author               Comment
//
// Written by Motorola AMSD, PMU Engineering.
// Legacy code for reference only.
//*************************************************************
 
#ifndef _COMPORTSETTINGS_
#define _COMPORTSETTINGS_ 
 

/////////////////////////////////////////////////////////////////////////////
// CComPortSettings dialog
 
class CWin16Comm;	// forward reference

 
class CComPortSettings : public CDialog
{ 
  private:
    CWin16Comm		*m_pCWin16Comm;   // access to port of interest

  // Construction
  public:
	CComPortSettings(CWnd* pParent = NULL);	// standard constructor

  // Dialog Data
	//{{AFX_DATA(CComPortSettings)
	enum { IDD = IDD_COMSETTINGS };
	CString	m_szPort;
	CString	m_szBaud;
	//}}AFX_DATA

    virtual int DoModal(CWin16Comm *pThePort)
    { 
      ASSERT(pThePort != NULL);
      m_pCWin16Comm = pThePort;
      return(CDialog::DoModal());
    }


  // Implementation
  protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CComPortSettings)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


#endif
