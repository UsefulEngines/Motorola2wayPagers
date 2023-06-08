//*************************************************************
//  History:    Date       Author               Comment
//              1996       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
// commview.cpp : implementation file
//


#include "stdafx.h"
#include "..\..\pps.h"
#include "..\..\ppsdoc.h"
#include "..\..\ppsview.h" 
#include "..\..\mainfrm.h"
#include "..\..\cpager\codeplug.h"
#include "..\..\w16comm\w16comm.h"
#include "..\..\w16gpib\w16gpib.h" 
#include "commview.h"


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#ifndef DISABLE_HEXEDIT    // conditional compilation of this file


extern CPpsApp    		theApp;
extern CWin16Comm 		theSerialPort;
extern CWin16Gpib		theGpibPort;


// maximum number of lines available to view, excess is deleted from front.
static const int nMaxNumStrings = 500;

// data buffer
static CStringArray	strStrings;



/////////////////////////////////////////////////////////////////////////////
// CCommViewer

IMPLEMENT_DYNCREATE(CCommViewer, CPpsFormView)

CCommViewer::CCommViewer()
	: CPpsFormView(CCommViewer::IDD)
{
	//{{AFX_DATA_INIT(CCommViewer)
	//}}AFX_DATA_INIT
}

CCommViewer::~CCommViewer()
{ 
  strStrings.RemoveAll();             // remove all strings
  theSerialPort.DetachCommViewPlug();  // detach from the serial port
  theGpibPort.DetachGpibViewPlug();    // detach from the gpib port
}

void CCommViewer::DoDataExchange(CDataExchange* pDX)
{
	CPpsFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCommViewer)
	//}}AFX_DATA_MAP
}



BEGIN_MESSAGE_MAP(CCommViewer, CPpsFormView)
	//{{AFX_MSG_MAP(CCommViewer)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_IO_OPLIST_DONE, OnWmIoOpListDone)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCommViewer diagnostics

#ifdef _DEBUG
void CCommViewer::AssertValid() const
{
	CPpsFormView::AssertValid();
}

void CCommViewer::Dump(CDumpContext& dc) const
{
	CPpsFormView::Dump(dc);
}
#endif //_DEBUG




BOOL CCommViewer::IsPresentable(void)
{
  return(!(((CMainFrame*)theApp.m_pMainWnd)->IsAccessRestricted()));
}


////////////////////////////////////////////////////////////////////////////
//	OnInitialUpdate()
//  
//  Update DDX variables from document and resize frame around view.
//
void CCommViewer::OnInitialUpdate(void)
{
  TRACE("CCommViewer::OnInitialUpdate : CCommViewer::IDD=%X\n", CCommViewer::IDD);
  
  // attach to the serial port
  theSerialPort.AttachCommViewPlug(&m_ICommViewPlug);

  // attach to the gpib port
  theGpibPort.AttachGpibViewPlug(&m_IGpibViewPlug);

  CClientDC dc (this);
  int nHeight = -((dc.GetDeviceCaps(LOGPIXELSY) * 10) / 72);

  m_screenFont.CreateFont(nHeight, 0, 0, 0, FW_NORMAL, 0, 0, 0,
       DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
       DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Courier New");
  // calculate screen element sizes...
  TEXTMETRIC tm;
  CFont* pOldFont = dc.SelectObject(&m_screenFont);
  dc.GetTextMetrics(&tm);
  m_nLineY = tm.tmHeight + tm.tmExternalLeading + 1;   // screen line height
  m_nPageY = (GetTotalSize().cy) / m_nLineY;
  ASSERT((m_nPageY >= 0) && (m_nLineY >= 0)); 
  TRACE("\nCCommViewer::OnUpdate : m_nLineY=%d : m_nPageY=%d\n", m_nLineY, m_nPageY);  
  // 
  dc.SelectObject(pOldFont);
  SetScrollSizes(MM_TEXT, CSize(0, strStrings.GetSize() * m_nLineY),
                 CSize(0, m_nPageY), CSize(0, m_nLineY));

  CPpsFormView::OnInitialUpdate();
  return;
}




/////////////////////////////////////////////////////////////////////////////
//	OnUpdate()
//
void CCommViewer::OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint)
{
  TRACE("\nCCommViewer::OnUpdate : IDD=%X  LHintHI=%X  lHintLO=%X\n", 
  		CCommViewer::IDD,  HIWORD(lHint), LOWORD(lHint));

  if (HIWORD(lHint))        		// user switched views, now active
  {    
    // ** Note:  a bug exists herein in which this particular view does
    // not resize properly after the initial switch to this view.  The
    // problem centers about the call to 'SetScrollSizes' below.  To date,
    // it is not debugged (11/26/96).  The CWnd::ResizeParentToFit() 
    // function has been overridden with the 'CPpsFormView' class in order
    // to implement a MFC bug-fix specific to 'CScrollView' derived 
    // classes, but the effect is not noticable.  In order for the view
    // to display properly, the following call to ResizeParentToFit is
    // being commented-out for this view.
    //
    //ResizeParentToFit(FALSE);
  } 

  int nScrollTo = strStrings.GetUpperBound() * m_nLineY;
  if (nScrollTo < 0)
    nScrollTo = 0;
  ScrollToPosition(CPoint(0, nScrollTo)); 

  CPpsFormView::OnUpdate(pSender, lHint, pHint); 
}	



////////////////////////////////////////////////////////////////////////////
//  SetFromDoc()
//
void CCommViewer::SetFromDoc(void)
{
  TRACE("\nCCommViewer::SetFromDoc\n");
  
  // This function serves no purpose in this class since this view
  // does not display the CPpsDoc contents, but rather the serial
  // and gpib port data.
      
  // UpdateData(FALSE);	// set controls data
  return;
}



////////////////////////////////////////////////////////////////////////////
//	SetToDoc()
//
BOOL CCommViewer::SetToDoc(void)
{
  TRACE("\nCCommViewer::SetToDoc\n");
  
  // This function serves only as a placeholder function since this view
  // is derived form CPpsFormView.  This view does not modify the codeplug
  // document
  
  //if (!(UpdateData(TRUE)))  // get controls data
  //  return FALSE;
    
  return(TRUE);
}



/////////////////////////////////////////////////////////////////////////////
// OnDraw
//
void CCommViewer::OnDraw(CDC* pDC)
{ 
  // redraw text strings... 
  if (strStrings.GetSize() > 0)
  {  
     CRect rect;
     pDC->GetClipBox(&rect);
     TRACE("\nCCommViewer::OnDraw : T=%d : B=%d : ScrollPos=%d\n", rect.top, rect.bottom,
           (GetScrollPosition().y)); 
     
     int nStart = rect.top / m_nLineY;
     int nEnd = rect.bottom / m_nLineY + 1;
     
     if (nStart > strStrings.GetUpperBound())
       return;
       
     if (nEnd > strStrings.GetSize())
       nEnd = strStrings.GetSize(); 
     
     CFont* pOldFont = pDC->SelectObject(&m_screenFont);

     ASSERT((nEnd >= 0) && (nStart >= 0));
     TRACE("\nCCommViewer::OnDraw : nStart=%d : nEnd=%d\n", nStart, nEnd);

     for (int i=nStart; i < nEnd; i++) 
     {
        pDC->TextOut(2, (i * m_nLineY), strStrings[i]);
     }
     pDC->SelectObject(pOldFont);
  }
  return;
}



/////////////////////////////////////////////////////////////////////////
//  OnWmIoOpListDone()
//
//  CPager::OnOpCompletion initiated event handler.  Receives WM_IO_OPLIST_DONE
//  messages.  Use this notification to trigger an update of the view.  This
//  is intended to minimize interference with the operation of serial
//  interface processing when this view is active.  Note:  this function is 
//  called from the "OnNote, OnError" functions below to cause incremental
//  screen updates (lParam=0L).  
//    wParam = nStatus (0=COMPLETED, 1=FAILED)
//    LOWORD(lParam) = Last CIoOperation ID
afx_msg LONG CCommViewer::OnWmIoOpListDone(UINT wParam, LONG lParam)
{
  TRACE("\nCCommViewer::OnWmIoOpListDone : wParam=%04X : lParam=%08lX\n", wParam, lParam);
  
  int nScrollTo = strStrings.GetUpperBound() * m_nLineY;  
    
  SetScrollSizes(MM_TEXT, CSize(0, strStrings.GetSize() * m_nLineY),
                 CSize(0, m_nPageY), CSize(0, m_nLineY));
  ScrollToPosition(CPoint(0, nScrollTo));  
  TRACE("\nCCommViewer::OnWmIoOpListDone : nNumStrings=%d : nScrollTo=%d\n", 
         strStrings.GetSize(), nScrollTo);
   
  // The following statement is a quick-fix to a bug.  The client area clip-
  // region is not being refreshed properly during a scroll of the first few
  // display lines.  Once the string list grows past the first screen, then
  // the client area is drawn properly. (:-<)  No time to analyze cause... 
  if (strStrings.GetUpperBound() < 20)  
    InvalidateRect(NULL);
  return(0L);
}
  


static char szTemp[200];
static const char *szFmt1 = "%s:%s: %0.2X %0.2X %0.2X %0.2X %0.2X %0.2X %0.2X %0.2X " \
                                   "%0.2X %0.2X %0.2X %0.2X %0.2X %0.2X %0.2X %0.2X";
static const char *szFmt2 = "%0.2X ";


void CCommViewer::OnNote(const char *pszPort, const char *pszOp, BYTE *pB, int nNumBytes)
{
  TRACE("\nCCommViewer::OnNote : %s : %s : nNumBytes=%d\n", pszPort, pszOp, nNumBytes);
  
  // add new data to the buffer
  ASSERT(pB != NULL);
  ASSERT(nNumBytes > 0);
  
  CString szLocal;
  
  int nNumLines = nNumBytes / 16;
  int nPartialLine = nNumBytes % 16;
  
  // save all full lines of data 
  int i = 0;
  for ( ; i < nNumLines; ++i)
  { 
    sprintf(szTemp, szFmt1, pszPort, pszOp, pB[(i*16)+0],  pB[(i*16)+1],  pB[(i*16)+2],  pB[(i*16)+3], 
                                            pB[(i*16)+4],  pB[(i*16)+5],  pB[(i*16)+6],  pB[(i*16)+7], 
                                            pB[(i*16)+8],  pB[(i*16)+9],  pB[(i*16)+10], pB[(i*16)+11], 
                                            pB[(i*16)+12], pB[(i*16)+13], pB[(i*16)+14], pB[(i*16)+15]); 
    strStrings.Add(szTemp);
  }
  
  // save any partial line of data
  if (nPartialLine)
  { 
    sprintf(szTemp, "%s:%s: ", pszPort, pszOp);
    szLocal = szTemp;    
    for (int j = 0; j < nPartialLine; ++j)
    {   
      sprintf(szTemp, szFmt2, pB[(nNumLines*16)+j]); 
      szLocal += szTemp;
    }
    strStrings.Add(szLocal);
  }
  
  // remove from beginning of string array if max encountered
  if (strStrings.GetSize() >= nMaxNumStrings) 
  {
    strStrings.RemoveAt(0, (nMaxNumStrings / 4));   // remove first quarter
    strStrings.FreeExtra();                         // garbage collect 
  }
  
  // update the display 
  OnWmIoOpListDone(0, 0L);
  return;
}
 



void CCommViewer::OnNote(const char *pszPort, const char *pszOp, const char *pszString)
{
  TRACE("\nCCommViewer::OnNote : %s\n", pszString);
  
  CString strLocal = pszPort; 
  strLocal += ":";
  strLocal += pszOp;
  strLocal += ": ";
  strLocal += pszString;                                                  

  // break long strings into multiple small strings 
  int nStrLen = strLocal.GetLength();
  if (nStrLen > 60)
  { 
    for (int nTemp = 0; nTemp < (nStrLen / 60 + 1); ++nTemp)
    {
      strStrings.Add(strLocal.Mid((nTemp * 60), 60)); 
    }
  }   
  else // add new data to the buffer  
    strStrings.Add(strLocal);
   
  // remove from beginning of string array if max encountered
  if (strStrings.GetSize() >= nMaxNumStrings) 
  {
    strStrings.RemoveAt(0, (nMaxNumStrings / 4));   // remove first quarter
    strStrings.FreeExtra();                         // garbage collect 
  }

  // update the display 
  OnWmIoOpListDone(0, 0L);
  return;
}



void CCommViewer::OnError(const char *pszPort, const char *pszOp, DWORD nStatusCode)
{
  TRACE("\nCCommViewer::OnError\n");
  
  CString  strLocal = pszPort;
  strLocal += ":";
  strLocal += pszOp;
  sprintf(szTemp, ":StatusCode=%08X", nStatusCode);
  strLocal += szTemp; 
  strStrings.Add(strLocal);
  
  // remove from beginning of string array if max encountered
  if (strStrings.GetSize() >= nMaxNumStrings) 
  {
    strStrings.RemoveAt(0, (nMaxNumStrings / 4));   // remove first quarter
    strStrings.FreeExtra();                         // garbage collect 
  }

  // update the display 
  OnWmIoOpListDone(0, 0L);  
  return;
}
   



// *** COMM VIEW

void CCommViewer::ICommViewPlug::CommNote(const char *pszPort, const char *pszOp, BYTE *pB, int nNumBytes)
{
  TRACE("\nICommViewPlug::CommNote : %s : %s : nNumBytes=%d\n", pszPort, pszOp, nNumBytes);
  METHOD_PROLOGUE(CCommViewer, m_ICommViewPlug)  // makes pThis (see 'w16Comm.h')
  
  // re-use the view member functions
  pThis->OnNote(pszPort, pszOp, pB, nNumBytes);
  return;
}


void CCommViewer::ICommViewPlug::CommNote(const char *pszPort, const char *pszOp, const char *pszString)
{
  TRACE("\nICommViewPlug::CommNote : %s\n", pszString);
  METHOD_PROLOGUE(CCommViewer, m_ICommViewPlug)  // makes pThis (see 'w16Comm.h')
  
  // re-use the view member functions
  pThis->OnNote(pszPort, pszOp, pszString);
  return;
}


void CCommViewer::ICommViewPlug::CommError(const char *pszPort, const char *pszOp, DWORD nStatusCode)
{
  TRACE("\nICommViewPlug::CommError\n");
  METHOD_PROLOGUE(CCommViewer, m_ICommViewPlug)  // makes pThis (see 'w16Comm.h')
  
  // re-use the view member functions
  pThis->OnError(pszPort, pszOp, nStatusCode);
  return;
}
   




// *** GPIB VIEW
//
void CCommViewer::IGpibViewPlug::GpibNote(const char *pszPort, const char *pszOp, BYTE *pB, int nNumBytes)
{
  TRACE("\nIGpibViewPlug::GpibNote : %s : %s : nNumBytes=%d\n", pszPort, pszOp, nNumBytes);
  METHOD_PROLOGUE(CCommViewer, m_IGpibViewPlug)  // makes pThis (see 'w16gpib.h')
  
  // re-use the view member functions
  pThis->OnNote(pszPort, pszOp, pB, nNumBytes);
  return;
}
  

void CCommViewer::IGpibViewPlug::GpibNote(const char *pszPort, const char *pszOp, const char *pszString)
{
  TRACE("\nIGpibViewPlug::GpibNote : %s\n", pszString);
  METHOD_PROLOGUE(CCommViewer, m_IGpibViewPlug)  // makes pThis (see 'w16gpib.h')
  
  // re-use the view member functions
  pThis->OnNote(pszPort, pszOp, pszString);
  return;
}


void CCommViewer::IGpibViewPlug::GpibError(const char *pszPort, const char *pszOp, DWORD nStatusCode)
{
  TRACE("\nIGpibViewPlug::GpibError\n");
  METHOD_PROLOGUE(CCommViewer, m_IGpibViewPlug)  // makes pThis (see 'w16gpib.h')
  
  // re-use the view member functions
  pThis->OnError(pszPort, pszOp, nStatusCode);
  return;
}
   



////////////////////////////////////////////////////////////////////////
// OnPreparePrinting
//
BOOL CCommViewer::OnPreparePrinting(CPrintInfo* pInfo)
{
  TRACE("CCommViewer::OnPreparePrinting\n");

  int nNumPages = (strStrings.GetSize() / 55);
  if (strStrings.GetSize() % 55)
    nNumPages += 1;
  
  pInfo->SetMaxPage(nNumPages);                                               
  
  return DoPreparePrinting(pInfo);
}


//////////////////////////////////////////////////////////////////////
//  OnPrint
//
static char *szPageNum = "Page %d of %d";
static char szPageNumBuff[25];
//
void CCommViewer::OnPrint(CDC *pDC, CPrintInfo *pInfo)
{ 
  TRACE("CCommViewer::OnPrint\n");
  
  // set the print font 
  int 			i, j, k, nHeight;
  CFont			font;
  TEXTMETRIC	tm;
  
  pDC->SetMapMode(MM_TWIPS);
  // use 10 point Roman font
  font.CreateFont(-200, 0, 0, 0, 400, FALSE, FALSE, 0, ANSI_CHARSET,
  				  OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
  				  DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,
  				  "Times New Roman");
  CFont *pOldFont = (CFont *) pDC->SelectObject(&font);
  pDC->GetTextMetrics(&tm);
  nHeight = tm.tmHeight + tm.tmExternalLeading;
  
  // print the strings for the current page
  j = strStrings.GetSize();
  k = (pInfo->m_nCurPage - 1) * 55; 
  
  for (i = 0; i < j; i++)
  { 
    if (i == 55)
      break;
    pDC->TextOut(720, -i * nHeight - 720, strStrings[k+i]);
  }
  sprintf(szPageNumBuff, szPageNum, pInfo->m_nCurPage, pInfo->GetMaxPage());                                          
  pDC->TextOut(720, (-(i+1) * nHeight - 720), szPageNumBuff);
  
  pDC->SelectObject(pOldFont);
  return;
}


#endif  // DISABLE_HEXEDIT - conditional compilation of this file

