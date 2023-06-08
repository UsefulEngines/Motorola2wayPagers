//*************************************************************
//  File name: comports.cpp
//
//  Description:  
//      Emplementation file for the CComPortSettings class.
//      
//      
//
//  History:    Date       Author               Comment
//              7/29/97    P. Pennington        Created
//
// Written by Motorola AMSD, PMU Engineering.
// Legacy code for reference only.
//*************************************************************


#include "stdafx.h"
#include "..\pps.h"
#include "w16comm.h"
#include "comports.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CComPortSettings dialog


CComPortSettings::CComPortSettings(CWnd* pParent /*=NULL*/)
	: CDialog(CComPortSettings::IDD, pParent)
{
	//{{AFX_DATA_INIT(CComPortSettings)
	m_szPort = "";
	m_szBaud = "";
	//}}AFX_DATA_INIT 
	
	m_pCWin16Comm = NULL;		// initialize pointer
}

void CComPortSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CComPortSettings)
	DDX_CBString(pDX, IDC_PORTCB, m_szPort);
	DDV_MaxChars(pDX, m_szPort, 4);
	DDX_CBString(pDX, IDC_BAUDCB, m_szBaud);
	DDV_MaxChars(pDX, m_szBaud, 4);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CComPortSettings, CDialog)
	//{{AFX_MSG_MAP(CComPortSettings)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CComPortSettings message handlers


BOOL CComPortSettings::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// initialize the member strings...
	m_szPort = m_pCWin16Comm->m_pszCOMn;
	m_szBaud = m_pCWin16Comm->m_pszBuildString;

    // extract the middle baud rate value.  Assumes only baud rates of 
    // 4 characters are in use (i.e. 9600, 4800, 2400, 1200). 
    m_szBaud = m_szBaud.Mid(5, 4);
    TRACE("\nCComPortSettings::OnInitDialog : m_szPort=%s : m_szBaud=%s\n", (LPCTSTR)m_szPort, (LPCTSTR)m_szBaud);
	
	// display the current port selection
	CListBox *pLB = (CListBox *)GetDlgItem(IDC_PORTCB);
	ASSERT(pLB);    
    pLB->SelectString(0, (LPCTSTR)m_szPort); 
    
    // display the current baud rate selection
	pLB = (CListBox *)GetDlgItem(IDC_BAUDCB);
	ASSERT(pLB);
    pLB->SelectString(0, (LPCTSTR)m_szBaud); 

	return TRUE;  // return TRUE  unless you set the focus to a control
}




void CComPortSettings::OnOK()
{
    char szBuffer[25];

    if (!(UpdateData(TRUE)))  // get controls data
      return;   
      
    // set port strings   
    strncpy(m_pCWin16Comm->m_pszCOMn, (LPCTSTR)m_szPort, 4);    
    sprintf(szBuffer, "%s:%s,n,8,1", (LPCTSTR)m_szPort, (LPCTSTR)m_szBaud);    
    strncpy(m_pCWin16Comm->m_pszBuildString, szBuffer, 15);

    TRACE("\nCComPortSettings::OnOK : m_szPort=%s : m_szBaud=%s\n", (LPCTSTR)m_szPort, (LPCTSTR)m_szBaud);    
	CDialog::OnOK();
	return;
}
