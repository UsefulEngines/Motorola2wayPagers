//*************************************************************
//  History:    Date       Author               Comment
//              1996       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
// hexedit.cpp : implementation file
//


#include "stdafx.h"
#include "..\..\pps.h"
#include "..\..\ppsdoc.h"
#include "..\..\ppsview.h" 
#include "..\..\mainfrm.h"
#include "hexedit.h"


extern CPpsApp    	theApp;


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


#ifndef DISABLE_HEXEDIT    // conditional compilation of this file

/////////////////////////////////////////////////////////////////////////////
// CNumEdit
BEGIN_MESSAGE_MAP (CNumEdit, CEdit)
    ON_WM_CHAR ()
END_MESSAGE_MAP ()

void CNumEdit::OnChar (UINT nChar, UINT nRepCnt, UINT nFlags)
{ 
    TRACE("\nCNumEdit::OnChar\n");
    if (((nChar >= '0') && (nChar <= '9')) || 
        ((nChar >= 'a') && (nChar <= 'f')) ||
        ((nChar >= 'A') && (nChar <= 'F')) ||
        (nChar == VK_BACK))
    {
      CEdit::OnChar (nChar, nRepCnt, nFlags);
      ((CHexEdit *)GetParent())->UpdateRowText(this);
    }
}




//////////////////////////////////////////////////////////////////////////////





/////////////////////////////////////////////////////////////////////////////
// CHexEdit

IMPLEMENT_DYNCREATE(CHexEdit, CPpsFormView)


/////////////////////////////////////////////////////////////////////////////
// CHexEdit diagnostics

#ifdef _DEBUG
void CHexEdit::AssertValid() const
{
	CPpsFormView::AssertValid();
}

void CHexEdit::Dump(CDumpContext& dc) const
{
 	register int row, col;

	CPpsFormView::Dump(dc);
	for (row = 0; row < NUM_ROWS; ++row)
	{
	  dc << "\n\t" << Rows[row].nAddress << " ";
	  for (col = 0; col < NUM_COLS; ++col)
	  {
	    TRACE(" %02X", Rows[row].byRow[col]);
	  }
	  dc << "  " << Rows[row].szText;
 	}
}
#endif //_DEBUG


///////////////////////////////////////////////////////////////////////////
//


BOOL CHexEdit::IsPresentable(void)
{
  return(!(((CMainFrame*)theApp.m_pMainWnd)->IsAccessRestricted()));
}



CHexEdit::CHexEdit()
	: CPpsFormView(CHexEdit::IDD)
{
	nPartialRow = 0;
	nMaxScroll = 0;
	bModifiedFlag = FALSE;
	//{{AFX_DATA_INIT(CHexEdit)
	//}}AFX_DATA_INIT
}

CHexEdit::~CHexEdit()
{
  // add any destructor specific coding here
}                         

void CHexEdit::DoDataExchange(CDataExchange* pDX)
{
	TRACE("\nCHexEdit::DoDataExchange : m_bSaveAndValidate=%d\n",
		  pDX->m_bSaveAndValidate);
		  
  	register int row, col;
    char	szBuff[5];
    CEdit *pEdit = NULL; 
  	
	CPpsFormView::DoDataExchange(pDX);
	
	// if FALSE move data to window elements from Rows[]
	if (pDX->m_bSaveAndValidate == FALSE)
	{
	  // fill the hex address column edit controls
  	  for (row = 0; row < NUM_ROWS; ++row)
      {
        pEdit = (CEdit *)GetDlgItem(IDC_HEXADDR0 + row); 
        ASSERT(pEdit != NULL);
        sprintf(szBuff, "%04X", Rows[row].nAddress);
        pEdit->SetWindowText(szBuff);
      }
   
      // fill the hex data edit control cells
      for (row = 0; row < NUM_ROWS; ++row)
      {
        for (col = 0; col < NUM_COLS; ++col)
        {
          pEdit = (CEdit *)GetDlgItem(row*NUM_COLS+col+IDC_ROW0COL0);
          ASSERT(pEdit != NULL);
          sprintf(szBuff, "%02X", Rows[row].byRow[col]);
          pEdit->SetWindowText(szBuff);
        }
      }
   
      // fill the row text edit controls   
      for (row = 0; row < NUM_ROWS; ++row)
      {
        pEdit = (CEdit *)GetDlgItem(IDC_ROW0TEXT + row);
        ASSERT(pEdit != NULL);
        pEdit->SetWindowText(Rows[row].szText);
      }
	}
	else	// m_bSaveAndValidate == TRUE
	{		// move data from modified window elements to Rows[] 
	  for (row = 0; row < NUM_ROWS; ++row)
	  {
	    for (col = 0; col < NUM_COLS; ++col)
	    {
	      pEdit = (CEdit *)GetDlgItem(row*NUM_COLS+col+IDC_ROW0COL0);
	      ASSERT(pEdit != NULL);
	      if (!(pEdit->GetModify()))
	        continue;
	      bModifiedFlag = TRUE;             // this page modified
	      Rows[row].bModified = TRUE;      // this row modified
  		  pEdit->GetWindowText(szBuff, 3); 
  		  if (szBuff[0] != '\0')
    	    Rows[row].byRow[col] = byHexAlpha2Binary(szBuff[0]);
  		  if (szBuff[1] != '\0')
    	    Rows[row].byRow[col] = (BYTE)((Rows[row].byRow[col] << 4) | 
    	                                  (byHexAlpha2Binary(szBuff[1])));
	    }
	  }	
	}
	
	//{{AFX_DATA_MAP(CHexEdit)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHexEdit, CPpsFormView)
	//{{AFX_MSG_MAP(CHexEdit)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_DOWNLOAD_NOW_BTN, OnDownloadNowBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////
//	OnInitialUpdate()
//
//	Override of CView::OnInitialUpdate in order to provide a public
//	wrapper as well as to perform locally specific inits.  This 
//	function is initially called from the CPpsApp::InitInstance 
//	function after the view is created. 
//
void CHexEdit::OnInitialUpdate(void)
{ 
  TRACE("\nCHexEdit::OnInitialUpdate : nCHexEdit::IDD=%X\n", IDD_HEXEDITDLG);
        
  CScrollBar *pSB = NULL;
  register int lcv;
    
  // init the edit controls 
  //	NOTE:  the edit control resource id's must
  //	be sequentially numbered.  if any resource
  //	editing causes a change, the resource id's
  //	must be verified to be sequential.  
  for (lcv = 0; lcv < (NUM_ROWS * NUM_COLS); ++lcv)
  { 
    EditCells[lcv].SubclassDlgItem(IDC_ROW0COL0 + lcv, this);
    EditCells[lcv].LimitText(2);
    EditCells[lcv].nCtrlID = IDC_ROW0COL0 + lcv; 
  }
  
  // since window is not visible, just init with min values
  pSB = (CScrollBar *)GetDlgItem(IDC_VSCROLL);
  pSB->SetScrollRange(MIN_SCROLL, nMaxScroll);
  pSB->SetScrollPos(0); 
  //EditCells[0].SetFocus();
  CPpsFormView::OnInitialUpdate();
  return;
}


////////////////////////////////////////////////////////////////////////
//	OnUpdate()
//
//	Override of CView::OnUpdate in order to take advantage of the
//	lHint information which is passed upon user view switching. 
//	The OnUpdate() call may not only be due to view switching but
//	also upon a document change initiated external to this view.
//	Consequently, recalculate scroll ranges and update the screen.
//	If the view is not active, ignore updates until user has
//	switched to this view. 
//
void CHexEdit::OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint)
{
  TRACE("\nCHexEdit::OnUpdate : CHexEdit::IDD=%X  lHintLO=%X  lHintHI=%X\n", 
  		IDD_HEXEDITDLG, LOWORD(lHint), HIWORD(lHint));

  CPpsDoc *pDoc = NULL;
  CScrollBar *pSB = (CScrollBar *)GetDlgItem(IDC_VSCROLL);
  
  if (HIWORD(lHint))  					  // user just switched views 
  {
    ResizeParentToFit(FALSE);
    pSB->SetScrollPos(MIN_SCROLL);
    EditCells[0].SetFocus();
  }

  // init the scroll bar 
  pDoc = (CPpsDoc *)GetDocument();
  nMaxScroll = ((pDoc->GetDocSize() / NUM_COLS) - NUM_ROWS);
  nPartialRow = (pDoc->GetDocSize() % NUM_COLS);
  pSB->SetScrollRange(MIN_SCROLL, nMaxScroll);
  if ((pSB->GetScrollPos()) > nMaxScroll)
    pSB->SetScrollPos(nMaxScroll);
    
  SetFromDoc(); 
  CPpsFormView::OnUpdate(pSender, lHint, pHint);
}	




////////////////////////////////////////////////////////////////////////
//	SetFromDoc()
//
//	This function accomplishes the movement of data from the CPpsDoc
//  member m_byCodePlugArray to the CHexEdit member Rows struct array
//  and subsequently to the CEdit controls on the window.  The window
//	controls are arranged in a spreadsheet fashion with NUM_ROWS rows
// 	and NUM_COLS columns of hex data preceeded by a single column of
//  address data and followed by a single column of text data.  Each 
//  address and text data column entry is read-only.  
//
//  The scroll position indicates which data row (MIN_SCROLL - nMaxScroll)
//	occupies the top row on the screen.  When nScrollPos=0 then the top row
//	on the screen is the first data row.  When nScrollPos=nMaxScroll, then
//  the top row on the screen is (NUM_ROWS - 1) from the last data row.  
//	That is, the nScrollPos indicates which 16 byte segment of data occupies
// 	the top visual row.
void CHexEdit::SetFromDoc(void)
{ 
  TRACE("\nCHexEdit::SetFromDoc : \n");

  register int row, col;
  CPpsDoc *pDoc = NULL;
  CScrollBar *pSB = NULL;
  int nScrollPos;
  UINT nBaseAddress;                       
  
  pDoc = (CPpsDoc *)GetDocument();
  if (pDoc->GetDocSize() < (NUM_ROWS * NUM_COLS))  // no document loaded 
    return;

  pSB = (CScrollBar *)GetDlgItem(IDC_VSCROLL);
  nScrollPos = pSB->GetScrollPos();
  ASSERT((nScrollPos >= MIN_SCROLL) && (nScrollPos <= nMaxScroll));
  
  // read new screen data from PpsDoc 
  if ((nScrollPos <= nMaxScroll) && (!nPartialRow))
  {
    for (row = 0; row < NUM_ROWS; ++row)
    {
      pDoc->Field((NUM_COLS * (nScrollPos + row)), 
      			  NUM_COLS, &Rows[row].byRow[0]);
    }
  }
  else  // last page and only partial last row
  {
    for (row = 0; row < (NUM_ROWS-1); ++row)
    {
      pDoc->Field((NUM_COLS * (nScrollPos + row)), 
      			  NUM_COLS, &Rows[row].byRow[0]);
    } 
    pDoc->Field((NUM_COLS * (nScrollPos + row)),
    			 nPartialRow, &Rows[row].byRow[0]);
  }
  
  // set the row address data 
  nBaseAddress = pDoc->GetDocBaseAddress();
  for (row = 0; row < NUM_ROWS; ++row)
  {
    Rows[row].nAddress = nBaseAddress + 
                         (NUM_COLS * nScrollPos) +
                         (NUM_COLS * row);
  }
  
  // set the row text data - turn non-printable values to '.'
  for (row = 0; row < NUM_ROWS; ++row)
  {
    for (col = 0; col < NUM_COLS; ++col)
    {
      if ((Rows[row].byRow[col] >= '!') &&
          (Rows[row].byRow[col] <= '~'))
        Rows[row].szText[col] = Rows[row].byRow[col];
      else
        Rows[row].szText[col] = '.';
    }
    Rows[row].szText[col] = '\0';
  }
  
  #ifdef _DEBUG 
    TRACE("\tnScrollPos = %d\n", nScrollPos);
	for (row = 0; row < NUM_ROWS; ++row)
	{ 
	  TRACE("\n\t%04X ", Rows[row].nAddress);
	  for (col = 0; col < NUM_COLS; ++col)
	  {
	    TRACE(" %02X", Rows[row].byRow[col]);
	  } 
	  TRACE("  %s", Rows[row].szText);
 	}
  #endif
  
  // reset row modification indicators
  for (row = 0; row < NUM_ROWS; ++row)
    Rows[row].bModified = FALSE;
  bModifiedFlag = FALSE;   // reset page modification indicator 

  // translate the Rows struct data to the screen controls 
  UpdateData(FALSE);
}





////////////////////////////////////////////////////////////////////////
//	SetToDoc()
//
//	This function accomplishes the movement of data from the HexEdit
//  view to the CPpsDoc document.  This function is called upon user
//  scrolling as well as upon view switching.  
BOOL CHexEdit::SetToDoc(void)
{
  CPpsDoc *pDoc = NULL;
  register int row, col;
  CScrollBar *pSB = NULL;
  int nScrollPos;
  CEdit	*pEdit = NULL;
  
  if (!(UpdateData(TRUE)))
    return FALSE;
  
  if (!(bModifiedFlag))  // no need for xfer if page not modified
    return TRUE;
      
  // update document from member data
  pDoc = (CPpsDoc *)GetDocument();
  pSB = (CScrollBar *)GetDlgItem(IDC_VSCROLL);
  nScrollPos = pSB->GetScrollPos();
  ASSERT((nScrollPos >= MIN_SCROLL) && (nScrollPos <= nMaxScroll));
  
  if ((nScrollPos <= nMaxScroll) && (!nPartialRow))  // full page
  {
    for (row = 0; row < NUM_ROWS; ++row)
    { 
      // ignore any unmodified rows
      if (Rows[row].bModified)
      {
        for (col = 0; col < NUM_COLS; ++col)
        {
          // look for modifications by column
          pEdit = (CEdit*)GetDlgItem(row*NUM_COLS+col+IDC_ROW0COL0);
          ASSERT(pEdit);
          if (!(pEdit->GetModify())) continue;  // ignore unmodified CEdit
          pDoc->SetField((NUM_COLS * (nScrollPos + row) + col), 1, &Rows[row].byRow[col]);
          pEdit->SetModify(FALSE);
        }
      }
    }
  }
  else  // last page and only partial last row
  {
    for (row = 0; row < (NUM_ROWS-1); ++row)
    {
      if (Rows[row].bModified)
      {
        for (col = 0; col < NUM_COLS; ++col)
        {
          // look for modifications by column
          pEdit = (CEdit*)GetDlgItem(row*NUM_COLS+col+IDC_ROW0COL0);
          ASSERT(pEdit);
          if (!(pEdit->GetModify())) continue;  // ignore unmodified CEdit
          pDoc->SetField((NUM_COLS * (nScrollPos + row) + col), 1, &Rows[row].byRow[col]);
          pEdit->SetModify(FALSE);
        }
      }
    }
    if (Rows[row].bModified)  // partial last row
    {
      for (col = 0; col < nPartialRow; ++col)
      {
        // look for modifications by column
        pEdit = (CEdit*)GetDlgItem(row*NUM_COLS+col+IDC_ROW0COL0);
        ASSERT(pEdit);
        if (!(pEdit->GetModify())) continue;  // ignore unmodified CEdit
        pDoc->SetField((NUM_COLS * (nScrollPos + row) + col), 1, &Rows[row].byRow[col]);
        pEdit->SetModify(FALSE);
      }
    }
  }
    
  return TRUE;
}





/////////////////////////////////////////////////////////////////////////////
//  OnVScroll()
//
//  Handle the user scroll commands.
//
void CHexEdit::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int	nScrollPos;
	register int col;
    CPpsDoc *pDoc = (CPpsDoc *)GetDocument();
	
    // NOTE:  a gotcha may occur herein due to the fact that any and 
    // all attached vertical scroll bars generate the same WM_VSCROLL 
    // message.  Although the view may have only 1 child scroll bar, if
    // the user reduces the view size, additional scroll bars will appear.
    // Consequently, we must match the scroll bar ID with the ID of
    // our own child in order to determine which scroll bar has triggered.

	if ((pScrollBar != NULL) &&  
	    (pScrollBar->GetDlgCtrlID() == IDC_VSCROLL))
	{
	  TRACE("\nCHexEdit::OnVScroll : IDC_VSCROLL : nSBCode=%X  nPos=%X\n",
	        nSBCode, nPos);
	  
	  // Note:  the nPos parameter has value only if the nSBCode is 
	  // SB_THUMBPOSITION or SB_THUMBTRACK.  For all other nSBCodes
	  // get the scroll position implicitly.
      nScrollPos = pScrollBar->GetScrollPos();
	  
	  // save any modifications to the CPpsDoc document
	  SetToDoc();
	  
	  switch (nSBCode)
	  {
	    case SB_LINEUP:
	      TRACE("\tSB_LINEUP\n");
	      if (nScrollPos == MIN_SCROLL)
	        return;
	      pScrollBar->SetScrollPos(nScrollPos-1);
	      // move the current Row data down one row 
	      memmove(&Rows[1], &Rows[0], ((NUM_ROWS - 1) * sizeof(TRow)));
	      // read one row of data from the CPpsDoc
	      pDoc->Field((NUM_COLS * (nScrollPos-1)), NUM_COLS,
	      			  &Rows[0].byRow[0]);                
	      // set the address and text data field in the new row
	      Rows[0].nAddress = Rows[1].nAddress - NUM_COLS;
	      for (col = 0; col < NUM_COLS; ++col)
    	  {
            if ((Rows[0].byRow[col] >= '!') &&
               (Rows[0].byRow[col] <= '~'))
              Rows[0].szText[col] = Rows[0].byRow[col];
            else
              Rows[0].szText[col] = '.';
          }
          Rows[0].szText[col] = '\0';
          Rows[0].bModified = FALSE;
          // update the screen
	      UpdateData(FALSE);
	      break;
	      
	    case SB_LINEDOWN:
	      TRACE("\tSB_LINEDOWN\n");
	      if (nScrollPos == nMaxScroll)
	        return;
	      pScrollBar->SetScrollPos(nScrollPos+1);
	      // move the current Row data up one row 
	      memmove(&Rows[0], &Rows[1], ((NUM_ROWS - 1) * sizeof(TRow)));
	      // read one row of data from the CPpsDoc
	      pDoc->Field((NUM_COLS * (nScrollPos+NUM_ROWS)), NUM_COLS,
	      			  &Rows[NUM_ROWS-1].byRow[0]); 
	      // set the address and text data field in the new row
	      Rows[NUM_ROWS-1].nAddress = Rows[NUM_ROWS-2].nAddress + NUM_COLS;
	      for (col = 0; col < NUM_COLS; ++col)
    	  {
            if ((Rows[NUM_ROWS-1].byRow[col] >= '!') &&
               (Rows[NUM_ROWS-1].byRow[col] <= '~'))
              Rows[NUM_ROWS-1].szText[col] = Rows[NUM_ROWS-1].byRow[col];
            else
              Rows[NUM_ROWS-1].szText[col] = '.';
          }
          Rows[NUM_ROWS-1].szText[col] = '\0'; 
          Rows[NUM_ROWS-1].bModified = FALSE;
          // update the screen
	      UpdateData(FALSE);
	      break;
	      
	    case SB_BOTTOM:
	      TRACE("\tSB_BOTTOM\n");
	      pScrollBar->SetScrollPos(nMaxScroll);
	      SetFromDoc();
	      break;
	      
	    case SB_TOP:
	      TRACE("\tSB_TOP\n");
	      pScrollBar->SetScrollPos(MIN_SCROLL);
	      SetFromDoc();
	      break;
	      
	    case SB_PAGEUP:                             
	      TRACE("\tSB_PAGEUP\n");
	      if (nScrollPos == MIN_SCROLL)
	        return;
	      if ((nScrollPos - NUM_ROWS) < MIN_SCROLL)
	        pScrollBar->SetScrollPos(MIN_SCROLL);
	      else
	        pScrollBar->SetScrollPos(nScrollPos - NUM_ROWS);
	      SetFromDoc();
	      break;
	      
	    case SB_PAGEDOWN:
	      TRACE("\tSB_PAGEDOWN\n");
	      if (nScrollPos == nMaxScroll)
	        return;
	      if ((nScrollPos + NUM_ROWS) > nMaxScroll)
	        pScrollBar->SetScrollPos(nMaxScroll);
	      else
	        pScrollBar->SetScrollPos(nScrollPos + NUM_ROWS);
	      SetFromDoc();
	      break;
	      
	    case SB_THUMBPOSITION:
	    case SB_THUMBTRACK:
	      TRACE("\tSB_THUMBposORtrack\n");
	      if (((int)nPos >= MIN_SCROLL) && ((int)nPos <= nMaxScroll)) 
	      {
	        pScrollBar->SetScrollPos(nPos);
	        SetFromDoc(); 
	      }
	    break;
	      
	    default:
	      TRACE("\tSB_DEFAULT\n");
	      break;
	  }
	  return;
	}
	CPpsFormView::OnVScroll(nSBCode, nPos, pScrollBar);
}


//////////////////////////////////////////////////////////////////////////
//	ProcessMessageFilter()
//
//  This function, called from CPpsApp::ProcessMessageFilter(), serves to
//  filter intercepted windows WM_KEYDOWN messages in order to override default 
//  handling of select VK_XXX keyboard input messages.  The filtering
//  is only activated if the CHexEdit view is visible.  There is a 
//  slight performance hit in utilizing this method, however, alternatives
//  are limited as the MFC architecture documentation indicates.
//  The function returns 1 if the message was processed herein or
//  0 otherwise.
BOOL CHexEdit::ProcessMessageFilter(int code, LPMSG lpMsg)    
{
  TRACE("\nCHexEdit::ProcessMessageFilter : nChar=%X\n", lpMsg->wParam);
  
  CScrollBar *pSB = (CScrollBar *)GetDlgItem(IDC_VSCROLL); 
  CNumEdit *pEdit = (CNumEdit *)GetFocus();    
  int nScrollPos = pSB->GetScrollPos(); 
    
  switch(lpMsg->wParam)
  {   
    case VK_UP:		// up arrow
      if (pEdit->IsInFirstRow())  
      { 
        if (nScrollPos == MIN_SCROLL)
          return TRUE;
        OnVScroll(SB_LINEUP, 0, pSB);
        pEdit->SetSel(0, -1, TRUE);  // re-select all in current control    
	  }        
      else
      { 
        pEdit->SetSel(-1, -1, TRUE); // de-select all in current control  
        ((CDialog *)this)->GotoDlgCtrl(GetDlgItem(pEdit->nCtrlID - NUM_COLS));
      }
      return TRUE;

    case VK_DOWN:   // dn arrow
      if (pEdit->IsInLastRow()) 
      { 
        if (nScrollPos == nMaxScroll)
          return TRUE;
        OnVScroll(SB_LINEDOWN, 0, pSB);
        pEdit->SetSel(0, -1, TRUE);  // re-select all in current control   
	  }        
      else
      { 
        pEdit->SetSel(-1, -1, TRUE);    
        ((CDialog *)this)->GotoDlgCtrl(GetDlgItem(pEdit->nCtrlID + NUM_COLS));        
      }
      return TRUE;
      
    case VK_LEFT:	// left arrow 
      if (pEdit->IsFirstCtrl())
      {
        if (nScrollPos == MIN_SCROLL)
          return TRUE;
        OnVScroll(SB_LINEUP, 0, pSB);
        pEdit->SetSel(-1, -1, TRUE);    
        ((CDialog *)this)->GotoDlgCtrl(GetDlgItem(IDC_ROW0COLF));
      }
      else
      {
        pEdit->SetSel(-1, -1, TRUE);    
        ((CDialog *)this)->PrevDlgCtrl(); 
      }        
      return TRUE;
     
    case VK_PRIOR:	// page up
      OnVScroll(SB_PAGEUP, 0, pSB);
      return TRUE;
      
    case VK_NEXT:	// page dn  
      OnVScroll(SB_PAGEDOWN, 0, pSB);
      return TRUE;
      
    case VK_RIGHT:	// right arrow 
    case VK_RETURN:	// enter key
    case VK_TAB:	// tab key
      if (pEdit->IsLastCtrl()) 
      {
        if (nScrollPos == nMaxScroll)
          return TRUE;
        OnVScroll(SB_LINEDOWN, 0, pSB);
        pEdit->SetSel(-1, -1, TRUE);    
        ((CDialog *)this)->GotoDlgCtrl(GetDlgItem(IDC_ROW9COL0));
      }
      else
      {
        pEdit->SetSel(-1, -1, TRUE);    
        ((CDialog *)this)->NextDlgCtrl();
      }
	  return TRUE;
	    
    default:
      break;
  }	
  return FALSE;    // did not process this message    
}



void CHexEdit::UpdateRowText(const CNumEdit *pEdit)
{
  int row = (pEdit->nCtrlID - IDC_ROW0COL0) / NUM_COLS;
  int col = (pEdit->nCtrlID - IDC_ROW0COL0) % NUM_COLS;
  char pszBuff[3];
  CEdit *pTextEdit = NULL; 
   
  TRACE("\nCHexEdit::UpdateRowText : row=%d  col=%d\n", row, col);
    
  // also update the Row[].byRow buffer 
  pEdit->GetWindowText(pszBuff, 3); 
  if (pszBuff[0] != '\0')
    Rows[row].byRow[col] = byHexAlpha2Binary(pszBuff[0]);
  if (pszBuff[1] != '\0')
    Rows[row].byRow[col] = (BYTE)((Rows[row].byRow[col] << 4) | 
                                  (byHexAlpha2Binary(pszBuff[1])));
  
  //TRACE("\t\tpszBuff=%s\tRows[row].byRow[col]=%02X\n", 
  //      pszBuff, Rows[row].byRow[col]);
   
  // translate the updated buffer byte to the text edit control
  if ((Rows[row].byRow[col] >= '!') &&
      (Rows[row].byRow[col] <= '~'))
    Rows[row].szText[col] = Rows[row].byRow[col];
  else
    Rows[row].szText[col] = '.';
  pTextEdit = (CEdit *)GetDlgItem(IDC_ROW0TEXT + row);
  ASSERT(pTextEdit != NULL);
  pTextEdit->SetWindowText(Rows[row].szText);
  return;
}



BYTE CHexEdit::byHexAlpha2Binary(const char ch)
{
   if ((ch >= '0') && (ch <= '9'))   
     return((BYTE)(ch - 0x30));
   else if ((ch >= 'A') && (ch <= 'F'))
     return((BYTE)(ch - 0x37));
   else if ((ch >= 'a') && (ch <= 'f'))
     return((BYTE)(ch - 0x57)); 
   return (0x00);
}  
    

    
void CHexEdit::OnDownloadNowBtn()
{ 
   CPpsDoc *pDoc = (CPpsDoc *)GetDocument();
   pDoc->WriteEntireCodeplug();
   return;
}


#endif  // DISABLE_HEXEDIT - conditional compilation of this file
