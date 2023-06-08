//*************************************************************
//  History:    Date       Author               Comment
//              1996       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
// dwnldpwd.cpp : implementation file
//

#include "stdafx.h" 
#include "..\..\pps.h"
#include "dwnldpwd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDwnLdPwd dialog


CDwnLdPwd::CDwnLdPwd(CWnd* pParent /*=NULL*/)
	: CDialog(CDwnLdPwd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDwnLdPwd)
	m_Edit1 = "";
	m_Edit2 = "";
	m_szCurrentPwd = "";
	//}}AFX_DATA_INIT
}

void CDwnLdPwd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDwnLdPwd)
	DDX_Text(pDX, IDC_EDIT1, m_Edit1);
	DDV_MaxChars(pDX, m_Edit1, 10);
	DDX_Text(pDX, IDC_EDIT2, m_Edit2);
	DDV_MaxChars(pDX, m_Edit2, 10);
	DDX_Text(pDX, IDC_CURPWD_EDIT, m_szCurrentPwd);
	DDV_MaxChars(pDX, m_szCurrentPwd, 50);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDwnLdPwd, CDialog)
	//{{AFX_MSG_MAP(CDwnLdPwd)
	ON_BN_CLICKED(IDC_OK, OnClickedOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDwnLdPwd message handlers

void CDwnLdPwd::OnOK()
{                              
  // dummy OnOK function -- do not call CDialog::OnOK()
  // overridden in order to prevent accidental exits from dialog
  // when user presses ENTER.  Action of OK button redirected to
  // OnClickedOk() below.
  TRACE("\nCDwnLdPwd::OnOK\n");
}

BOOL CDwnLdPwd::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDwnLdPwd::OnClickedOk()
{
  TRACE("\nCDwnLdPwd::OnClickedOk\n");
  CDialog::OnOK();
}
