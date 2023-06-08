//*************************************************************
//  History:    Date       Author               Comment
//              1996       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
// pclink.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\pps.h"
#include "..\..\ppsdoc.h"
#include "..\..\ppsview.h" 
#include "..\..\mainfrm.h"
#include "..\..\cpager\codeplug.h" 
#include "pclink.h"

extern CPpsApp    theApp;

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPCLink

IMPLEMENT_DYNCREATE(CPCLink, CPpsFormView)

CPCLink::CPCLink()
	: CPpsFormView(CPCLink::IDD)
{
	//{{AFX_DATA_INIT(CPCLink)
	m_byDataFlagResignal = 0;
	m_byDataFlagTimer = 0;
	m_bSendErrMsgCnt = FALSE;
	m_bSendInventoryControlNum = FALSE;
	m_bNonProtocolMode = FALSE;
	m_bEnableDataFlags = FALSE;
	m_bSendSource = FALSE;
	m_bSendTimeDate = FALSE;
	m_byDataFlagCharacter = 0;
	//}}AFX_DATA_INIT
}

CPCLink::~CPCLink()
{
}



BOOL CPCLink::IsPresentable(void)
{ 
  // *** Note:  This view is visible only with the LabUse version
  // of the PPS.  This is due to the fact that PC support has not
  // yet (12/30/96) been implemented with this pager.  The only
  // real support of the 'Y-command Protocol' is intended for
  // use with the 'PegMon' application.
  return(!(((CMainFrame*)theApp.m_pMainWnd)->IsAccessRestricted()));
}



void CPCLink::DoDataExchange(CDataExchange* pDX)
{
	CPpsFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPCLink)
	DDX_Text(pDX, IDC_DATAFLAG_RESIG_EDIT, m_byDataFlagResignal);
	DDV_MinMaxByte(pDX, m_byDataFlagResignal, 0, 7);
	DDX_Text(pDX, IDC_DATAFLAG_TIMER_EDIT, m_byDataFlagTimer);
	DDV_MinMaxByte(pDX, m_byDataFlagTimer, 0, 7);
	DDX_Check(pDX, IDC_ERRMSGCNT_CHK, m_bSendErrMsgCnt);
	DDX_Check(pDX, IDC_ICN_CHK, m_bSendInventoryControlNum);
	DDX_Check(pDX, IDC_NONPROTOCOL_CHK, m_bNonProtocolMode);
	DDX_Check(pDX, IDC_SENDDATAFLAG_CHK, m_bEnableDataFlags);
	DDX_Check(pDX, IDC_SENDSOURCE_CHK, m_bSendSource);
	DDX_Check(pDX, IDC_SENDTIMEDATE_CHK, m_bSendTimeDate);
	DDX_Text(pDX, IDC_DATAFLAG_EDIT, m_byDataFlagCharacter);
	DDV_MinMaxByte(pDX, m_byDataFlagCharacter, 0, 255);
	//}}AFX_DATA_MAP
}



BEGIN_MESSAGE_MAP(CPCLink, CPpsFormView)
	//{{AFX_MSG_MAP(CPCLink)
	ON_BN_CLICKED(IDC_PCLINK_DEFAULTS_BTN, OnAcceptDefaultsBtn)
	ON_BN_CLICKED(IDC_PCLINK_UNDO_BTN, OnUndoBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPCLink diagnostics

#ifdef _DEBUG
void CPCLink::AssertValid() const
{
	CPpsFormView::AssertValid();
}

void CPCLink::Dump(CDumpContext& dc) const
{
	CPpsFormView::Dump(dc);
}
#endif //_DEBUG




////////////////////////////////////////////////////////////////////////////
//	OnInitialUpdate()
//  
//  Update DDX variables from document and resize frame around view.
//
void CPCLink::OnInitialUpdate(void)
{
  TRACE("CPCLink::OnInitialUpdate : CPCLink::IDD=%X\n", CPCLink::IDD);
  
  CPpsFormView::OnInitialUpdate();
  return;
}




/////////////////////////////////////////////////////////////////////////////
//	OnUpdate()
//
void CPCLink::OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint)
{
  TRACE("\nCPCLink::OnUpdate : IDD=%X  LHintHI=%X  lHintLO=%X\n", 
  		CPCLink::IDD,  HIWORD(lHint), LOWORD(lHint));

  if (HIWORD(lHint))        		// user switched views, now active
    ResizeParentToFit(FALSE);

  SetFromDoc();
  CPpsFormView::OnUpdate(pSender, lHint, pHint);
}	


////////////////////////////////////////////////////////////////////////////
//  SetFromDoc()
//
void CPCLink::SetFromDoc(void)
{
  TRACE("\nCPCLink::SetFromDoc\n");
  CPpsDoc	*pDoc = GetDocument();
  BYTE		byTemp;
  
  // ***** CLEAN THIS UP SOMEDAY WITH THE CpField MECHANISM..... 

  pDoc->Field(CP_Function1_3.Offset(), 1, &byTemp);
  m_bSendErrMsgCnt = ((byTemp & 0x20) > 0);
  m_bSendTimeDate = ((byTemp & 0x10) > 0);
  m_bSendInventoryControlNum = ((byTemp & 0x08) > 0);
  m_bSendSource = ((byTemp & 0x04) > 0);
  m_bEnableDataFlags = ((byTemp & 0x02) > 0);
  m_bNonProtocolMode = ((byTemp & 0x01) > 0);
  
  pDoc->Field(CP_DataFlag.Offset(), 1, &m_byDataFlagCharacter);
  
  pDoc->Field(CP_DataFlagInfo.Offset(), 1, &byTemp);
  m_byDataFlagResignal = (BYTE)((byTemp & 0x38) >> 3);
  m_byDataFlagTimer = (BYTE)(byTemp & 0x07);

  UpdateData(FALSE);	// set controls data
}



////////////////////////////////////////////////////////////////////////////
//	SetToDoc()
//
BOOL CPCLink::SetToDoc(void)
{
  TRACE("\nCPCLink::SetToDoc\n");
  CEdit 		*pEdit = NULL;
  CPpsDoc		*pDoc = GetDocument();
  BYTE			byTemp;
  BYTE			byTempCopy;
  CEdit			*pEdit1 = NULL;
  
  if (!(UpdateData(TRUE)))  // get controls data
    return FALSE;
   
  // ***** CLEAN THIS UP SOMEDAY WITH THE CpField MECHANISM..... 
   
  // CP_Function1_3
  pDoc->Field(CP_Function1_3.Offset(), 1, &byTemp); 
  byTempCopy = byTemp;
  if (m_bSendErrMsgCnt != ((byTemp & 0x20) > 0))
  {
    if (m_bSendErrMsgCnt) byTemp |= 0x20;
    else byTemp &= 0xDF;
  }
  if (m_bSendTimeDate != ((byTemp & 0x10) > 0))
  {
    if (m_bSendTimeDate) byTemp |= 0x10;
    else byTemp &= 0xEF;
  }
  if (m_bSendInventoryControlNum != ((byTemp & 0x08) > 0))
  {
    if (m_bSendInventoryControlNum) byTemp |= 0x08;
    else byTemp &= 0xF7;
  }
  if (m_bSendSource != ((byTemp & 0x04) > 0))
  {
    if (m_bSendSource) byTemp |= 0x04;
    else byTemp &= 0xFB;
  }
  if (m_bEnableDataFlags != ((byTemp & 0x02) > 0))
  {
    if (m_bEnableDataFlags) byTemp |= 0x02;
    else byTemp &= 0xFD;
  }
  if (m_bNonProtocolMode != ((byTemp & 0x01) > 0))
  {
    if (m_bNonProtocolMode) byTemp |= 0x01;
    else byTemp &= 0xFE;
  }
  if (byTemp != byTempCopy)
    pDoc->SetField(CP_Function1_3.Offset(), 1, &byTemp); 
  
  //  CP_DataFlag
  pEdit1 = (CEdit *)GetDlgItem(IDC_DATAFLAG_EDIT);
  ASSERT(pEdit1 != NULL);
  if (pEdit1->GetModify())
    pDoc->SetField(CP_DataFlag.Offset(), 1, &m_byDataFlagCharacter);
  
  // CP_DataFlagInfo
  pDoc->Field(CP_DataFlagInfo.Offset(), 1, &byTemp);
  byTempCopy = byTemp;
  if (m_byDataFlagResignal != ((byTemp & 0x38) >> 3))
  {
    byTemp &= 0xC7;
    byTemp |= (m_byDataFlagResignal << 3);
  }
  if (m_byDataFlagTimer != (byTemp & 0x07))
  {
    byTemp &= 0xF8;
    byTemp |= m_byDataFlagTimer;
  }
  if (byTemp != byTempCopy)
    pDoc->SetField(CP_DataFlagInfo.Offset(), 1, &byTemp);

  
  return(TRUE);
}




/////////////////////////////////////////////////////////////////////////////
// CPCLink message handlers




////////////////////////////////////////////////////////////////////////
//  OnAcceptDefaultsBtn()
//
//  This function initiates a move of data from select fields of the
//  CPpsDoc.m_byDefaultCodePlugArray to the CPpsDoc.m_byCodePlugArray.
//  The m_byDefaultCodePlugArray is loaded from file 'default.s19' upon
//  application execution.  If the file has not been found, then no 
//  default values are available.  
//
void CPCLink::OnAcceptDefaultsBtn()
{
  TRACE("\nCPCLink::OnAcceptDefaultsBtn\n"); 
  CPpsDoc	*pDoc = GetDocument(); 
   
  if (pDoc->SetFieldFromDefault(CP_Function1_3.Offset(), CP_Function1_3.Length()))
    return; 
  if (pDoc->SetFieldFromDefault(CP_DataFlag.Offset(), CP_DataFlag.Length()))
    return; 
  if (pDoc->SetFieldFromDefault(CP_DataFlagInfo.Offset(), CP_DataFlagInfo.Length()))
    return;  
  
  SetFromDoc();   // update this view
}



////////////////////////////////////////////////////////////////////////
//  OnArchived()
//
//  This function initiates a move of data from select fields of the
//  CPpsDoc.m_byArchivedCodePlugArray to the CPpsDoc.m_byCodePlugArray.
//
void CPCLink::OnArchived(void)
{
  CPpsDoc	*pDoc = GetDocument(); 
   
  if (pDoc->SetFieldFromArchived(CP_Function1_3.Offset(), CP_Function1_3.Length()))
    return;  
  if (pDoc->SetFieldFromArchived(CP_DataFlag.Offset(), CP_DataFlag.Length()))
    return;  
  if (pDoc->SetFieldFromArchived(CP_DataFlagInfo.Offset(), CP_DataFlagInfo.Length()))
    return;  
    
  SetFromDoc();   // update this view
}


////////////////////////////////////////////////////////////////////////
//  OnUndoBtn()
//
//  This function initiates a move of data from select fields of the
//  CPpsDoc.m_byOriginalCodePlugArray to the CPpsDoc.m_byCodePlugArray.
//  The m_byOriginalCodePlugArray is loaded upon 'pager-read' or 
//  'file-open' menu or toolbar actions.
//
void CPCLink::OnUndoBtn()
{
  TRACE("\nCPCLink::OnUndoBtn\n"); 
  CPpsDoc	*pDoc = GetDocument(); 
  
  if (pDoc->SetFieldFromOriginal(CP_Function1_3.Offset(), CP_Function1_3.Length()))
    return;  
  
  if (pDoc->SetFieldFromOriginal(CP_DataFlag.Offset(), CP_DataFlag.Length()))
    return;  
  
  if (pDoc->SetFieldFromOriginal(CP_DataFlagInfo.Offset(), CP_DataFlagInfo.Length()))
    return;  
  
  SetFromDoc();   // update this view
}




