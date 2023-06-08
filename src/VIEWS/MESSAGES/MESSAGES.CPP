//*************************************************************
//  History:    Date       Author               Comment
//              1996       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
// messages.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\pps.h"
#include "..\..\ppsdoc.h" 
#include "..\..\ppsview.h"
#include "..\..\cpager\codeplug.h" 
#include "messages.h"

extern CPpsApp    theApp;

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMessages

IMPLEMENT_DYNCREATE(CMessages, CPpsFormView)

CMessages::CMessages()
	: CPpsFormView(CMessages::IDD)
{ 
	//{{AFX_DATA_INIT(CMessages)
	m_bDuplicateMsgCheck = FALSE;
	m_szFormatDelimChar = "";
	m_byDelimitPosFour = 0;
	m_byDelimitPosOne = 0;
	m_byDelimitPosThree = 0;
	m_byDelimitPosTwo = 0;
	m_bRetainErrOneWayAlphaNumeric = FALSE;
	m_bRetainErrOneWayNumeric = FALSE;
	m_bRetainErrOneWayXParent = FALSE;
	m_bRetainErrTwoWayMsgs = FALSE;
	m_byMsgUpdateTimeout = 0;
	//}}AFX_DATA_INIT
}

CMessages::~CMessages()
{
}

void CMessages::DoDataExchange(CDataExchange* pDX)
{
	CPpsFormView::DoDataExchange(pDX);  
	//{{AFX_DATA_MAP(CMessages)
	DDX_Check(pDX, IDC_DUP_MSG_CHECK, m_bDuplicateMsgCheck);
	DDX_Text(pDX, IDC_FMT_DELIM_CHAR, m_szFormatDelimChar);
	DDV_MaxChars(pDX, m_szFormatDelimChar, 1);
	DDX_Text(pDX, IDC_DELIMIT_POS_FOUR, m_byDelimitPosFour);
	DDV_MinMaxByte(pDX, m_byDelimitPosFour, 0, 255);
	DDX_Text(pDX, IDC_DELIMIT_POS_ONE, m_byDelimitPosOne);
	DDV_MinMaxByte(pDX, m_byDelimitPosOne, 0, 255);
	DDX_Text(pDX, IDC_DELIMIT_POS_THREE, m_byDelimitPosThree);
	DDV_MinMaxByte(pDX, m_byDelimitPosThree, 0, 255);
	DDX_Text(pDX, IDC_DELIMIT_POS_TWO, m_byDelimitPosTwo);
	DDV_MinMaxByte(pDX, m_byDelimitPosTwo, 0, 255);
	DDX_Check(pDX, IDC_RET_ERR_1_ALPHNUM, m_bRetainErrOneWayAlphaNumeric);
	DDX_Check(pDX, IDC_RET_ERR_1_NUM, m_bRetainErrOneWayNumeric);
	DDX_Check(pDX, IDC_RET_ERR_1_XPARENT, m_bRetainErrOneWayXParent);
	DDX_Check(pDX, IDC_RET_ERR_2_MSGS, m_bRetainErrTwoWayMsgs);
	DDX_Text(pDX, IDC_MSGUPDATETIMEOUT_EDIT, m_byMsgUpdateTimeout);
	DDV_MinMaxByte(pDX, m_byMsgUpdateTimeout, 0, 255);
	//}}AFX_DATA_MAP
}



BEGIN_MESSAGE_MAP(CMessages, CPpsFormView)
	//{{AFX_MSG_MAP(CMessages)
	ON_BN_CLICKED(IDC_MSGS_DEFAULTS_BTN, OnAcceptDefaultsBtn)
	ON_BN_CLICKED(IDC_MSGS_UNDO_BTN, OnUndoBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMessages diagnostics

#ifdef _DEBUG
void CMessages::AssertValid() const
{
	CPpsFormView::AssertValid();
}

void CMessages::Dump(CDumpContext& dc) const
{
	CPpsFormView::Dump(dc);
}
#endif //_DEBUG



////////////////////////////////////////////////////////////////////////////
//	OnInitialUpdate()
//  
//  Update DDX variables from document and resize frame around view.
//
void CMessages::OnInitialUpdate(void)
{
  TRACE("CMessages::OnInitialUpdate : CMessages::IDD=%X\n", CMessages::IDD);                                

  CEdit *pEdit = (CEdit *)GetDlgItem(IDC_FMT_DELIM_CHAR);
  ASSERT(pEdit != NULL);
  pEdit->LimitText(1);
  
  CPpsFormView::OnInitialUpdate();
  return;
}




/////////////////////////////////////////////////////////////////////////////
//	OnUpdate()
//
void CMessages::OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint)
{
  TRACE("\nCMessages::OnUpdate : IDD=%X  LHintHI=%X  lHintLO=%X\n", 
  		CMessages::IDD,  HIWORD(lHint), LOWORD(lHint));

  if (HIWORD(lHint))        		// user switched views, now active
    ResizeParentToFit(FALSE);

  SetFromDoc();
  CPpsFormView::OnUpdate(pSender, lHint, pHint);
}	


////////////////////////////////////////////////////////////////////////////
//  SetFromDoc()
//
void CMessages::SetFromDoc(void)
{
  TRACE("\nCMessages::SetFromDoc\n");
  CPpsDoc	*pDoc = GetDocument();
  BYTE		byTemp[4];

  // format delimiter info
  pDoc->Field(CP_Format_Delimiter.Offset(), 1, byTemp);
  byTemp[1] = '\0';
  m_szFormatDelimChar = (char *)byTemp;
  
  CP_delimiter_location.bySetFromDoc(&m_byDelimitPosFour, 0);
  CP_delimiter_location.bySetFromDoc(&m_byDelimitPosThree, 1);
  CP_delimiter_location.bySetFromDoc(&m_byDelimitPosTwo, 2);
  CP_delimiter_location.bySetFromDoc(&m_byDelimitPosOne, 3);
  
  // CP_Memory_Opts_L info
  CP_retain_errored_alpha.bSetFromDoc(&m_bRetainErrOneWayAlphaNumeric);
  CP_retain_errored_numeric.bSetFromDoc(&m_bRetainErrOneWayNumeric);
  CP_retain_errored_transparent.bSetFromDoc(&m_bRetainErrOneWayXParent);
  CP_retain_errored_2way.bSetFromDoc(&m_bRetainErrTwoWayMsgs);
  
  // CP_Memory_Opts_H info
  CP_duplication.bSetFromDoc(&m_bDuplicateMsgCheck);
  
  // CP_MsgUpdateTimeout
  CP_MsgUpdateTimeout.bySetFromDoc(&m_byMsgUpdateTimeout); 
   
  UpdateData(FALSE);	// set controls data
}



////////////////////////////////////////////////////////////////////////////
//	SetToDoc()
//
BOOL CMessages::SetToDoc(void)
{
  TRACE("\nCMessages::SetToDoc\n");
  CEdit 		*pEdit = NULL;
  CPpsDoc		*pDoc = GetDocument();
  BYTE			byTemp[4];
  
  if (!(UpdateData(TRUE)))  // get controls data
    return FALSE;
  
  // format delimiter info 
  pEdit = (CEdit*)GetDlgItem(IDC_FMT_DELIM_CHAR);
  ASSERT(pEdit != NULL);
  if (pEdit->GetModify())
  {
    byTemp[0] = m_szFormatDelimChar[0];
    pDoc->SetField(CP_Format_Delimiter.Offset(), 1, byTemp);
  }
  
  CP_delimiter_location.bySetToDoc(m_byDelimitPosFour, 0);
  CP_delimiter_location.bySetToDoc(m_byDelimitPosThree, 1);
  CP_delimiter_location.bySetToDoc(m_byDelimitPosTwo, 2);
  CP_delimiter_location.bySetToDoc(m_byDelimitPosOne, 3);
  
  // CP_Memory_Opts_L info
  CP_retain_errored_alpha.bSetToDoc(m_bRetainErrOneWayAlphaNumeric);
  CP_retain_errored_numeric.bSetToDoc(m_bRetainErrOneWayNumeric);
  CP_retain_errored_transparent.bSetToDoc(m_bRetainErrOneWayXParent);
  CP_retain_errored_2way.bSetToDoc(m_bRetainErrTwoWayMsgs);
  
  // CP_Memory_Opts_H info
  CP_duplication.bSetToDoc(m_bDuplicateMsgCheck);
  
  // CP_MsgUpdateTimeout
  CP_MsgUpdateTimeout.bySetToDoc(m_byMsgUpdateTimeout); 
  
  return(TRUE);
}



/////////////////////////////////////////////////////////////////////////////
// CMessages message handlers




////////////////////////////////////////////////////////////////////////
//  OnAcceptDefaultsBtn()
//
//  This function initiates a move of data from select fields of the
//  CPpsDoc.m_byDefaultCodePlugArray to the CPpsDoc.m_byCodePlugArray.
//  The m_byDefaultCodePlugArray is loaded from file 'default.s19' upon
//  application execution.  If the file has not been found, then no 
//  default values are available.  
//
void CMessages::OnAcceptDefaultsBtn()
{
  TRACE("\nCMessages::OnAcceptDefaultsBtn\n"); 
  CPpsDoc	*pDoc = GetDocument(); 
  
  if (pDoc->SetFieldFromDefault(CP_Format_Delimiter.Offset(), CP_Format_Delimiter.Length()))
    return;
  if (pDoc->SetFieldFromDefault(CP_Delimiter_Location.Offset(), CP_Delimiter_Location.Length()))
    return;
  if (pDoc->SetFieldFromDefault(CP_Memory_Opts_L.Offset(), CP_Memory_Opts_L.Length()))
    return;
  if (pDoc->SetFieldFromDefault(CP_Memory_Opts_H.Offset(), CP_Memory_Opts_H.Length()))
    return; 
  if (pDoc->SetFieldFromDefault(CP_MsgUpdateTimeout.Offset(), CP_MsgUpdateTimeout.Length()))
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
void CMessages::OnUndoBtn()
{
  TRACE("\nCMessages::OnUndoBtn\n"); 
  CPpsDoc	*pDoc = GetDocument(); 
  
  if (pDoc->SetFieldFromOriginal(CP_Format_Delimiter.Offset(), CP_Format_Delimiter.Length()))
    return;
  if (pDoc->SetFieldFromOriginal(CP_Delimiter_Location.Offset(), CP_Delimiter_Location.Length()))
    return;
  if (pDoc->SetFieldFromOriginal(CP_Memory_Opts_L.Offset(), CP_Memory_Opts_L.Length()))
    return;
  if (pDoc->SetFieldFromOriginal(CP_Memory_Opts_H.Offset(), CP_Memory_Opts_H.Length()))
    return;
  if (pDoc->SetFieldFromOriginal(CP_MsgUpdateTimeout.Offset(), CP_MsgUpdateTimeout.Length()))
    return; 

  SetFromDoc();   // update this view
}


////////////////////////////////////////////////////////////////////////
//  OnArchived()
//
//  This function initiates a move of data from select fields of the
//  CPpsDoc.m_byArchivedCodePlugArray to the CPpsDoc.m_byCodePlugArray.
//
void CMessages::OnArchived(void)
{
  CPpsDoc	*pDoc = GetDocument(); 
   
  if (pDoc->SetFieldFromArchived(CP_Format_Delimiter.Offset(), CP_Format_Delimiter.Length()))
    return;
  if (pDoc->SetFieldFromArchived(CP_Delimiter_Location.Offset(), CP_Delimiter_Location.Length()))
    return;
  if (pDoc->SetFieldFromArchived(CP_Memory_Opts_L.Offset(), CP_Memory_Opts_L.Length()))
    return;
  if (pDoc->SetFieldFromArchived(CP_Memory_Opts_H.Offset(), CP_Memory_Opts_H.Length()))
    return;
  if (pDoc->SetFieldFromArchived(CP_MsgUpdateTimeout.Offset(), CP_MsgUpdateTimeout.Length()))
    return; 
    
  SetFromDoc();   // update this view
}



int CMessages::OnPrintHelper(CStringArray &rStringArray, UINT nPageNumber, int nAvailableLines)
{ 
   // if not enough lines available, wait for next page...
   const int nNumLinesRequired = 10;
   if (nAvailableLines < nNumLinesRequired)
     return(-1);    
   int nNumLinesBegin = rStringArray.GetSize();
   
   char    szTemp[200];
   
   rStringArray.Add("MESSAGE PROCESSING");
     sprintf(szTemp, "    Perform duplication check on personal messages:  %d", m_bDuplicateMsgCheck); 
     rStringArray.Add(szTemp); 
     //
     sprintf(szTemp, "        Regard duplicate messages as new messages after %d hours.", m_byMsgUpdateTimeout);
     rStringArray.Add(szTemp);   
     //
     sprintf(szTemp, "    Retain errored one-way alpha-numeric messages:  %d", m_bRetainErrOneWayAlphaNumeric);
     rStringArray.Add(szTemp);
     //
     sprintf(szTemp, "    Retain errored one-way numeric-only messages:  %d", m_bRetainErrOneWayNumeric); 
     rStringArray.Add(szTemp);
     // 
     sprintf(szTemp, "    Retain errored one-way transparent messages:  %d", m_bRetainErrOneWayXParent);
     rStringArray.Add(szTemp);
     //
     sprintf(szTemp, "    Retain errored two-way messages:  %d", m_bRetainErrTwoWayMsgs);
     rStringArray.Add(szTemp); 
     //
     sprintf(szTemp, "    Numeric message format delimiting character:  %s", m_szFormatDelimChar);     
     rStringArray.Add(szTemp);
     //
     sprintf(szTemp, "    Delimiting character positions:  %d  %d  %d  %d", 
             m_byDelimitPosFour, m_byDelimitPosThree, m_byDelimitPosTwo, m_byDelimitPosOne);
     rStringArray.Add(szTemp);
     	     
   rStringArray.Add("");
   return(rStringArray.GetSize() - nNumLinesBegin);
}


 

