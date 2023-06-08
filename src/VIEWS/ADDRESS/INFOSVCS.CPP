//*************************************************************
//  History:    Date       Author               Comment
//              1996       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
// infosvcs.cpp : implementation file
//

#include "stdafx.h" 
#include "..\..\pps.h"
#include "infosvcs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInfoSvcSubAddrDlg dialog


CInfoSvcSubAddrDlg::CInfoSvcSubAddrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInfoSvcSubAddrDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInfoSvcSubAddrDlg)
	m_bSub0 = FALSE;
	m_bSub1 = FALSE;
	m_bSub10 = FALSE;
	m_bSub11 = FALSE;
	m_bSub12 = FALSE;
	m_bSub13 = FALSE;
	m_bSub14 = FALSE;
	m_bSub15 = FALSE;
	m_bSub16 = FALSE;
	m_bSub17 = FALSE;
	m_bSub18 = FALSE;
	m_bSub19 = FALSE;
	m_bSub2 = FALSE;
	m_bSub20 = FALSE;
	m_bSub21 = FALSE;
	m_bSub22 = FALSE;
	m_bSub23 = FALSE;
	m_bSub24 = FALSE;
	m_bSub25 = FALSE;
	m_bSub26 = FALSE;
	m_bSub27 = FALSE;
	m_bSub28 = FALSE;
	m_bSub29 = FALSE;
	m_bSub3 = FALSE;
	m_bSub30 = FALSE;
	m_bSub31 = FALSE;
	m_bSub4 = FALSE;
	m_bSub5 = FALSE;
	m_bSub6 = FALSE;
	m_bSub7 = FALSE;
	m_bSub8 = FALSE;
	m_bSub9 = FALSE;
	//}}AFX_DATA_INIT
}

void CInfoSvcSubAddrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInfoSvcSubAddrDlg)
	DDX_Check(pDX, IDC_SUB0, m_bSub0);
	DDX_Check(pDX, IDC_SUB1, m_bSub1);
	DDX_Check(pDX, IDC_SUB10, m_bSub10);
	DDX_Check(pDX, IDC_SUB11, m_bSub11);
	DDX_Check(pDX, IDC_SUB12, m_bSub12);
	DDX_Check(pDX, IDC_SUB13, m_bSub13);
	DDX_Check(pDX, IDC_SUB14, m_bSub14);
	DDX_Check(pDX, IDC_SUB15, m_bSub15);
	DDX_Check(pDX, IDC_SUB16, m_bSub16);
	DDX_Check(pDX, IDC_SUB17, m_bSub17);
	DDX_Check(pDX, IDC_SUB18, m_bSub18);
	DDX_Check(pDX, IDC_SUB19, m_bSub19);
	DDX_Check(pDX, IDC_SUB2, m_bSub2);
	DDX_Check(pDX, IDC_SUB20, m_bSub20);
	DDX_Check(pDX, IDC_SUB21, m_bSub21);
	DDX_Check(pDX, IDC_SUB22, m_bSub22);
	DDX_Check(pDX, IDC_SUB23, m_bSub23);
	DDX_Check(pDX, IDC_SUB24, m_bSub24);
	DDX_Check(pDX, IDC_SUB25, m_bSub25);
	DDX_Check(pDX, IDC_SUB26, m_bSub26);
	DDX_Check(pDX, IDC_SUB27, m_bSub27);
	DDX_Check(pDX, IDC_SUB28, m_bSub28);
	DDX_Check(pDX, IDC_SUB29, m_bSub29);
	DDX_Check(pDX, IDC_SUB3, m_bSub3);
	DDX_Check(pDX, IDC_SUB30, m_bSub30);
	DDX_Check(pDX, IDC_SUB31, m_bSub31);
	DDX_Check(pDX, IDC_SUB4, m_bSub4);
	DDX_Check(pDX, IDC_SUB5, m_bSub5);
	DDX_Check(pDX, IDC_SUB6, m_bSub6);
	DDX_Check(pDX, IDC_SUB7, m_bSub7);
	DDX_Check(pDX, IDC_SUB8, m_bSub8);
	DDX_Check(pDX, IDC_SUB9, m_bSub9);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CInfoSvcSubAddrDlg, CDialog)
	//{{AFX_MSG_MAP(CInfoSvcSubAddrDlg)
	ON_BN_CLICKED(IDC_DISABLE_BTN, OnDisableBtn)
	ON_BN_CLICKED(IDC_ENABLE_BTN, OnEnableBtn)
	ON_BN_CLICKED(IDC_OK_SUBADDR_DLG, OnAcceptChanges)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_HELPHITTEST, OnHelpHitTest)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CInfoSvcSubAddrDlg message handlers


BOOL CInfoSvcSubAddrDlg::OnInitDialog()
{
    TRACE("\nCInfoSvcSubAddrDlg::OnInitDialog : pdwSubAddrs=%08lX\n",
          *pdwSubAddrs);
     
	m_bSub0 =  ((*pdwSubAddrs & 0x00000001) > 0);
	m_bSub1 =  ((*pdwSubAddrs & 0x00000002) > 0);
	m_bSub2 =  ((*pdwSubAddrs & 0x00000004) > 0);
	m_bSub3 =  ((*pdwSubAddrs & 0x00000008) > 0);
	
	m_bSub4 =  ((*pdwSubAddrs & 0x00000010) > 0);
	m_bSub5 =  ((*pdwSubAddrs & 0x00000020) > 0);
	m_bSub6 =  ((*pdwSubAddrs & 0x00000040) > 0);
	m_bSub7 =  ((*pdwSubAddrs & 0x00000080) > 0);
	
	m_bSub8 =  ((*pdwSubAddrs & 0x00000100) > 0);
	m_bSub9 =  ((*pdwSubAddrs & 0x00000200) > 0);
	m_bSub10 = ((*pdwSubAddrs & 0x00000400) > 0);
	m_bSub11 = ((*pdwSubAddrs & 0x00000800) > 0);
	
	m_bSub12 = ((*pdwSubAddrs & 0x00001000) > 0);
	m_bSub13 = ((*pdwSubAddrs & 0x00002000) > 0);
	m_bSub14 = ((*pdwSubAddrs & 0x00004000) > 0);
	m_bSub15 = ((*pdwSubAddrs & 0x00008000) > 0);
	
	m_bSub16 = ((*pdwSubAddrs & 0x00010000) > 0);
	m_bSub17 = ((*pdwSubAddrs & 0x00020000) > 0);
	m_bSub18 = ((*pdwSubAddrs & 0x00040000) > 0);
	m_bSub19 = ((*pdwSubAddrs & 0x00080000) > 0);
	
	m_bSub20 = ((*pdwSubAddrs & 0x00100000) > 0);
	m_bSub21 = ((*pdwSubAddrs & 0x00200000) > 0);
	m_bSub22 = ((*pdwSubAddrs & 0x00400000) > 0);
	m_bSub23 = ((*pdwSubAddrs & 0x00800000) > 0);      
	
	m_bSub24 = ((*pdwSubAddrs & 0x01000000) > 0);
	m_bSub25 = ((*pdwSubAddrs & 0x02000000) > 0);
	m_bSub26 = ((*pdwSubAddrs & 0x04000000) > 0);
	m_bSub27 = ((*pdwSubAddrs & 0x08000000) > 0);
	
	m_bSub28 = ((*pdwSubAddrs & 0x10000000) > 0);
	m_bSub29 = ((*pdwSubAddrs & 0x20000000) > 0);
	m_bSub30 = ((*pdwSubAddrs & 0x40000000) > 0);
	m_bSub31 = ((*pdwSubAddrs & 0x80000000) > 0);
               
	CDialog::OnInitDialog();
	return TRUE;  // return TRUE  unless you set the focus to a control
}



void CInfoSvcSubAddrDlg::OnOK()
{
  // dummy OnOK function -- do not call CDialog::OnOK()
  // overridden in order to prevent accidental exits from dialog
  // when user presses ENTER.  Action of OK button redirected to
  // OnAccept() below.
  TRACE("\nCInfoSvcSubAddrDlg::OnOK\n");
}


void CInfoSvcSubAddrDlg::OnDisableBtn()
{
	m_bSub0 =  0;
	m_bSub1 =  0;
	m_bSub2 =  0;
	m_bSub3 =  0;
	m_bSub4 =  0;
	m_bSub5 =  0;
	m_bSub6 =  0;
	m_bSub7 =  0;
	m_bSub8 =  0;
	m_bSub9 =  0;
	m_bSub10 = 0;
	m_bSub11 = 0;
	m_bSub12 = 0;
	m_bSub13 = 0;
	m_bSub14 = 0;
	m_bSub15 = 0;
	m_bSub16 = 0;
	m_bSub17 = 0;
	m_bSub18 = 0;
	m_bSub19 = 0;
	m_bSub20 = 0;
	m_bSub21 = 0;
	m_bSub22 = 0;
	m_bSub23 = 0;
	m_bSub24 = 0;
	m_bSub25 = 0;
	m_bSub26 = 0;
	m_bSub27 = 0;
	m_bSub28 = 0;
	m_bSub29 = 0;
	m_bSub30 = 0;
	m_bSub31 = 0;
	
    UpdateData(FALSE);
    return;	
}



void CInfoSvcSubAddrDlg::OnEnableBtn()
{
	m_bSub0 =  1;
	m_bSub1 =  1;
	m_bSub2 =  1;
	m_bSub3 =  1;
	m_bSub4 =  1;
	m_bSub5 =  1;
	m_bSub6 =  1;
	m_bSub7 =  1;
	m_bSub8 =  1;
	m_bSub9 =  1;
	m_bSub10 = 1;
	m_bSub11 = 1;
	m_bSub12 = 1;
	m_bSub13 = 1;
	m_bSub14 = 1;
	m_bSub15 = 1;
	m_bSub16 = 1;
	m_bSub17 = 1;
	m_bSub18 = 1;
	m_bSub19 = 1;
	m_bSub20 = 1;
	m_bSub21 = 1;
	m_bSub22 = 1;
	m_bSub23 = 1;
	m_bSub24 = 1;
	m_bSub25 = 1;
	m_bSub26 = 1;
	m_bSub27 = 1;
	m_bSub28 = 1;
	m_bSub29 = 1;
	m_bSub30 = 1;
	m_bSub31 = 1;
	
    UpdateData(FALSE);
    return;	
}



void CInfoSvcSubAddrDlg::OnAcceptChanges()
{
    UpdateData(TRUE);  // get controls data

    DWORD	ulTemp = 0L;
    
    if (m_bSub0)
      ulTemp |= 0x00000001;
    if (m_bSub1)
      ulTemp |= 0x00000002;
    if (m_bSub2)
      ulTemp |= 0x00000004;
    if (m_bSub3)
      ulTemp |= 0x00000008;
      
    if (m_bSub4)
      ulTemp |= 0x00000010;
    if (m_bSub5)
      ulTemp |= 0x00000020;
    if (m_bSub6)
      ulTemp |= 0x00000040;
    if (m_bSub7)
      ulTemp |= 0x00000080;
      
    if (m_bSub8)
      ulTemp |= 0x00000100;
    if (m_bSub9)
      ulTemp |= 0x00000200;
    if (m_bSub10)
      ulTemp |= 0x00000400;
    if (m_bSub11)
      ulTemp |= 0x00000800;
      
    if (m_bSub12)
      ulTemp |= 0x00001000;
    if (m_bSub13)
      ulTemp |= 0x00002000;
    if (m_bSub14)
      ulTemp |= 0x00004000;
    if (m_bSub15)
      ulTemp |= 0x00008000;
      
    if (m_bSub16)
      ulTemp |= 0x00010000;
    if (m_bSub17)
      ulTemp |= 0x00020000;
    if (m_bSub18)
      ulTemp |= 0x00040000;
    if (m_bSub19)
      ulTemp |= 0x00080000;
      
    if (m_bSub20)
      ulTemp |= 0x00100000;
    if (m_bSub21)
      ulTemp |= 0x00200000;
    if (m_bSub22)
      ulTemp |= 0x00400000;
    if (m_bSub23)
      ulTemp |= 0x00800000;
      
    if (m_bSub24)
      ulTemp |= 0x01000000;
    if (m_bSub25)
      ulTemp |= 0x02000000;
    if (m_bSub26)
      ulTemp |= 0x04000000;
    if (m_bSub27)
      ulTemp |= 0x08000000;
      
    if (m_bSub28)
      ulTemp |= 0x10000000;
    if (m_bSub29)
      ulTemp |= 0x20000000;
    if (m_bSub30)
      ulTemp |= 0x40000000;
    if (m_bSub31)
      ulTemp |= 0x80000000;
    
    *pdwSubAddrs = ulTemp;
 
    TRACE("\nCInfoSvcSubAddrDlg::OnAcceptChanges : ulTemp=%08lX : dwSubAddrs=%08lX\n",
          ulTemp, *pdwSubAddrs);
          
	CDialog::OnOK();	
}

 

LRESULT CInfoSvcSubAddrDlg::OnHelpHitTest(WPARAM wParam, LPARAM lParam)
{                                            
  TRACE("CInfoSvcSubAddrDlg::OnHelpHitTest\n");
  return(HID_BASE_RESOURCE + IDD_SUBADDR_DLG); 
}
 
