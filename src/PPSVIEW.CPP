//*************************************************************
//  History:    Date       Author               Comment
//              1996       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
////////////////////////////////////////////////////////////////////////////
// ppsview.cpp : implementation of the CPpsFormView and CPpsView classes
//

#include "stdafx.h"
#include "pps.h"

#include "ppsdoc.h"
#include "ppsview.h"
#include "cpager\cpager.h"

extern CPpsApp    theApp;

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPpsFormView

IMPLEMENT_DYNAMIC(CPpsFormView, CFormView)

BEGIN_MESSAGE_MAP(CPpsFormView, CFormView)
	//{{AFX_MSG_MAP(CPpsFormView)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_HELPHITTEST, OnHelpHitTest)
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
END_MESSAGE_MAP()

// constructor
CPpsFormView::CPpsFormView(UINT nIDTemplate)
				: CFormView(nIDTemplate)
{ 
  nHelpContextHook = nIDTemplate;
}



//////////////////////////////////////////////////////////////////////////
//  OnInitialUpdate()
//
//  Each derived class should return via this member function if the
//  derived class overrides OnInitialUpdate().
//
void CPpsFormView::OnInitialUpdate()
{ 
  CFormView::OnInitialUpdate();
  return; 
}


//////////////////////////////////////////////////////////////////////////
//  OnUpdate()
//
//  Each derived class should return via this member function if the
//  derived class overrides OnUpdate().
//
void CPpsFormView::OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint)
{ 
  CFormView::OnUpdate(pSender, lHint, pHint);
  return; 
}



/////////////////////////////////////////////////////////////////////////////
// CPpsFormView context sensitive help support
LRESULT CPpsFormView::OnHelpHitTest(WPARAM wParam, LPARAM lParam)
{                                            
  TRACE("CPpsFormView::OnHelpHitTest\n");
  return(HID_BASE_RESOURCE + nHelpContextHook); 
}

/////////////////////////////////////////////////////////////////////////////
// CPpsFormView printing
BOOL CPpsFormView::OnPreparePrinting(CPrintInfo* pInfo)
{
  TRACE("CPpsFormView::OnPreparePrinting\n");
   
  // default preparation
  pInfo->SetMaxPage(2);
  return DoPreparePrinting(pInfo);
}

void CPpsFormView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{ 
  TRACE("CPpsFormView::OnBeginPrinting\n");
  // TODO: add extra initialization before printing
}


void CPpsFormView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
  TRACE("CPpsFormView::OnEndPrinting\n");
  // TODO: add cleanup after printing
}


void CPpsFormView::OnPrint(CDC *pDC, CPrintInfo *pInfo)
{ 
  TRACE("CPpsFormView::OnPrint\n");
  CStringArray szStringArray;
  
  // NOTE:  if a derived class overrides 'OnPrepareDC', then it should
  // also ignore the call to OnPrepareDC when the device context is
  // to be used for printing.  The 'IsPrinting' CDC member function
  // is useful (see the override of OnPrepareDC in the CPpsView class
  // definition below).
  
  // allow theApp to iterate through each view to obtain print strings
  theApp.OnPrintHelper(szStringArray, pInfo->m_nCurPage);
  
  // print the strings
  int 			i, j, nHeight;
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
  j = szStringArray.GetSize();
  for (i = 0; i < j; i++)
  {
    pDC->TextOut(720, -i * nHeight - 720, szStringArray[i]);
  }
  pDC->SelectObject(pOldFont);
  szStringArray.RemoveAll();  
  return;
}


//////////////////////////////////////////////////////////////////////
//  ResizeParentToFit()
// 
//  Override of CScrollView::ResizeParentToFit() in order to make 
//  a bugfix as published via the MSDN knowledge base.  MFC classes
//  include fix after V2.2. 
//
void CPpsFormView::ResizeParentToFit(BOOL bShrinkOnly)
{
	// adjust parent rect so client rect is appropriate size
	ASSERT(m_nMapMode != 0);  // mapping mode must be known // change to msvc\mfc\src\viewscrl.cpp

	// determine current size of the client area as if no scrollbars present
	CRect rectClient;
	GetWindowRect(rectClient);
	CRect rect = rectClient;
	CalcWindowRect(rect);
	rectClient.left += rectClient.left - rect.left;
	rectClient.top += rectClient.top - rect.top;
	rectClient.right -= rect.right - rectClient.right;
	rectClient.bottom -= rect.bottom - rectClient.bottom;
	rectClient.OffsetRect(-rectClient.left, -rectClient.top);
	ASSERT(rectClient.left == 0 && rectClient.top == 0);

	// determine desired size of the view
	CRect rectView(0, 0, m_totalDev.cx, m_totalDev.cy);
	if (bShrinkOnly)
	{
		if (rectClient.right <= m_totalDev.cx)
			rectView.right = rectClient.right;
		if (rectClient.bottom <= m_totalDev.cy)
			rectView.bottom = rectClient.bottom;
	}
	CalcWindowRect(rectView, CWnd::adjustOutside);
    rectView.OffsetRect(-rectView.left, -rectView.top);   // change to msvc\mfc\src\viewscrl.cpp
    ASSERT(rectView.left == 0 && rectView.top == 0);      // change to msvc\mfc\src\viewscrl.cpp
	if (bShrinkOnly)
	{
		if (rectClient.right <= m_totalDev.cx)
			rectView.right = rectClient.right;
		if (rectClient.bottom <= m_totalDev.cy)
			rectView.bottom = rectClient.bottom;
	}

	// determine and set size of frame based on desired size of view
	CRect rectFrame;
	CFrameWnd* pFrame = (CFrameWnd*)GetParentFrame();
	ASSERT_VALID(pFrame);
    // if maximized, ignore resizing.  ** Modification to default behaviour.
    if (pFrame->IsZoomed())
      return;
	pFrame->GetWindowRect(rectFrame);
	CSize size = rectFrame.Size(); 
	// ** Modification to default behaviour.  If the frame
	// ** is currently the same size as previous, don't bother
	// ** adjusting.
	CSize orig = size;
	size.cx += rectView.right - rectClient.right;
	size.cy += rectView.bottom - rectClient.bottom;
	if ((size.cx == orig.cx) && (size.cy == orig.cy))
	  return;
	pFrame->SetWindowPos(NULL, 0, 0, size.cx, size.cy,
	                     SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
    return;
}



/////////////////////////////////////////////////////////////////////////////
// CPpsFormView diagnostics
#ifdef _DEBUG
void CPpsFormView::AssertValid() const
{
	CFormView::AssertValid();
}

void CPpsFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CPpsDoc* CPpsFormView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPpsDoc)));
	return (CPpsDoc*)m_pDocument;
}
#endif //_DEBUG






/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CPpsView

IMPLEMENT_DYNCREATE(CPpsView, CPpsFormView)

BEGIN_MESSAGE_MAP(CPpsView, CPpsFormView)
	//{{AFX_MSG_MAP(CPpsView)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_IO_OPLIST_DONE, OnWmIoOpListDone)
END_MESSAGE_MAP()
                                  


/////////////////////////////////////////////////////////////////////////////
// CPpsView construction/destruction

CPpsView::CPpsView()
	: CPpsFormView(CPpsView::IDD)
{
    bBitmapLoaded = FALSE;
	//{{AFX_DATA_INIT(CPpsView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// TODO: add construction code here
}


CPpsView::~CPpsView()
{
    // Select old bitmap into memory dc (selecting out logo bitmap)
    // Need to create a temporary pointer to pass to do this
    ASSERT(m_hBmpOld);
    VERIFY(m_dcMem.SelectObject(CBitmap::FromHandle(m_hBmpOld)));
    
    // Need to DeleteObject() the bitmap that was loaded
    m_bmpLogo.DeleteObject();
    bBitmapLoaded = FALSE;
    // m_dcMem destructor will handle rest of cleanup    
}


void CPpsView::DoDataExchange(CDataExchange* pDX)
{
	CPpsFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPpsView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


/////////////////////////////////////////////////////////////////////////////
// CPpsView diagnostics

#ifdef _DEBUG
void CPpsView::AssertValid() const
{
	CPpsFormView::AssertValid();
}

void CPpsView::Dump(CDumpContext& dc) const
{
	CPpsFormView::Dump(dc);
}

#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
//	CPpsView member functions

////////////////////////////////////////////////////////////////////////////
//	OnInitialUpdate()
//  
//  Update DDX variables from document and resize frame around view.
//
void CPpsView::OnInitialUpdate(void)
{ 
  TRACE("\nCPpsView::OnInitialUpdate : CPpsView::IDD=%X\n", IDD_PPS_FORM);
        
  CFrameWnd	*pFrame;
  CRect		rectFrame, rectView;
  static BOOL bFirstTimeFlag = TRUE;
                                
  // ensure original window display resize occurs only once
  if (bFirstTimeFlag == TRUE)  
  {
    // resize parent to fit dialog template exactly 
    // while not resizing beyond size of screen
    VERIFY(pFrame = (CFrameWnd *)GetParentFrame());
    pFrame->GetClientRect(rectFrame);
    GetClientRect(rectView);
    if ((rectFrame.Width() < rectView.Width()) ||
        (rectFrame.Height() < rectView.Height()))
    {
      // resize so can be refit to any template that fits the screen
      pFrame->MoveWindow(0, 0, GetSystemMetrics(SM_CXSCREEN), 
    				     GetSystemMetrics(SM_CYSCREEN), FALSE);
    }
    pFrame->RecalcLayout();
    ResizeParentToFit(TRUE);	// shrink to fit template 
    bFirstTimeFlag = FALSE; 
  }

  if (bBitmapLoaded == FALSE)   // only once per instantiation
  {
    // display the logo bitmap
    CBitmap * pBmpOld;
        
    // Load bitmap resource - remember to call DeleteObject when done.
    VERIFY(m_bmpLogo.LoadBitmap(IDB_AMPDLOGO_BMP));

    // Get bitmap information    
    m_bmpLogo.GetObject(sizeof(BITMAP), &m_bmInfo);
    m_size.cx = m_bmInfo.bmWidth;
    m_size.cy = m_bmInfo.bmHeight;

    // Get temporary DC for view - Will be released in dc destructor
    CClientDC dc(this);
    
    // Create compatible memory DC using the dialogs DC
    VERIFY(m_dcMem.CreateCompatibleDC(&dc));
    
    // Select circle bitmap into DC.  
    // Get pointer to original bitmap
    // NOTE! This is temporary - save the handle instead
    VERIFY(pBmpOld = m_dcMem.SelectObject(&m_bmpLogo));
    VERIFY(m_hBmpOld = (HBITMAP)pBmpOld->GetSafeHandle());
    // local CCLientDC destructor for 'dc' will release device context
    bBitmapLoaded = TRUE;
  }
  
  CPpsFormView::OnInitialUpdate();  
  return;
}


/////////////////////////////////////////////////////////////////////////////
//	OnUpdate()
//
void CPpsView::OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint)
{
  TRACE("\nCPpsView::OnUpdate : nViewId=%X  LHintHI=%X  lHintLO=%X\n", 
  		IDD_PPS_FORM, HIWORD(lHint), LOWORD(lHint));

  CPpsFormView::OnUpdate(pSender, lHint, pHint);

  if (HIWORD(lHint))        			// user switched views
    ResizeParentToFit(FALSE);

  SetFromDoc();
}	


////////////////////////////////////////////////////////////////////////////
//  SetFromDoc()
//
void CPpsView::SetFromDoc(void)
{
  CPpsDoc	*pDoc = (CPpsDoc *) GetDocument();
  
  // set this view member data

  UpdateData(FALSE);	// set controls data
}


////////////////////////////////////////////////////////////////////////////
//	SetToDoc()
//
BOOL CPpsView::SetToDoc(void)
{
  CPpsDoc	*pDoc = (CPpsDoc *) GetDocument();
  
  if (!(UpdateData(TRUE)))  // get controls data
    return FALSE;
    
  // set doc data from this view
  
  return(TRUE);
}


afx_msg BOOL CPpsView::OnEraseBkgnd(CDC* pDC)
{
  // Set brush to desired background color
  CBrush backBrush(RGB(192, 192, 192));
 
  // Save old brush
  CBrush* pOldBrush = pDC->SelectObject(&backBrush);
 
  CRect rect;
  pDC->GetClipBox(&rect);     // Erase the area needed
 
  pDC->PatBlt(rect.left, rect.top, rect.Width(),
  rect.Height(), PATCOPY);
  pDC->SelectObject(pOldBrush);
  return TRUE;
}


void CPpsView::OnPrepareDC(CDC *pDC, CPrintInfo* pInfo)
{ 
  TRACE("\nCPpsView::OnPrepareDC\n");
  CRect  clientRect;
  
  if (pDC->IsPrinting())   // if printDC use default mapping
    return;
    
  GetClientRect(clientRect);
  pDC->SetMapMode(MM_ANISOTROPIC); // +y = down, banner changes with window sizing 
  // make the logical and device coordinates the same...
  pDC->SetWindowExt(clientRect.right, clientRect.bottom);
  pDC->SetViewportExt(clientRect.right, clientRect.bottom);
  pDC->SetViewportOrg(0, 0);
  return;
}



void CPpsView::OnDraw(CDC *pDC)
{
  TRACE("\nCPpsView::OnDraw\n");
  CFont		BannerFont, *pOldFont;
  CRect  	clientRect;
  int 		nHeight, nLen, nPreviousMode;
  CString	szBanner;
  CPpsDoc	*pDoc = GetDocument();

  ASSERT(pDoc);
  
  GetClientRect(clientRect);
  
  // if the pager APC code is available display the product name
  theApp.GetThePager()->GetProductName(szBanner);
  if ((!(szBanner.IsEmpty())) && (pDoc->GetDocSize()))
  {
    pDC->SetTextColor(RGB(128, 128, 128));    // light gray
    
    // associate the font height with the banner string length
    // and the window width  
    nLen = szBanner.GetLength();
    nHeight = (clientRect.right / nLen);
    BannerFont.CreateFont(-nHeight, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0,
  						  ANSI_CHARSET, OUT_TT_PRECIS,
  						  CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
  						  DEFAULT_PITCH | FF_SWISS, "Arial");

    // center the banner 
    pOldFont = pDC->SelectObject(&BannerFont);
    CSize TextSize = pDC->GetTextExtent((LPCTSTR)szBanner, nLen);  
    nPreviousMode = pDC->SetBkMode(TRANSPARENT);
    pDC->TextOut(((clientRect.right / 2) - (TextSize.cx / 2)), 
                 ((clientRect.bottom / 2) - (TextSize.cy / 2)), 
                 (LPCTSTR)szBanner);
  
    // output trademark 
    pDC->SelectObject(pOldFont);        
    pDC->TextOut(((clientRect.right / 2) + (TextSize.cx / 2) + 3), 
                 ((clientRect.bottom / 2) - (TextSize.cy / 2) + 7), 
                 "TM");

    pDC->SetBkMode(nPreviousMode);
  }
  else  // display the AMPD logo
  {
    // BitBlt Logo bitmap onto view 
    pDC->BitBlt((clientRect.right / 2) - (m_size.cx / 2), 
  			    (clientRect.bottom / 2) - (m_size.cy / 2), 
  			    m_size.cx, m_size.cy, &m_dcMem, 0, 0, SRCCOPY );
    /*
     * First two parameters are upper left position to place bitmap.
     * Third and fourth parameters are width and height to copy 
     * (could be less than actual size of bitmap)
     * Sixth and seventh are position in memory dc to start from
     * SRCCOPY specifies copy.
     * See BitBlt documentation for more details.
     */
  }
  return;
} 



int CPpsView::OnPrintHelper(CStringArray &rStringArray, UINT nPageNumber, int nAvailableLines)
{
   CString  szLocal;
   CTime	time = CTime::GetCurrentTime();
                             
   szLocal = "DEVICE CONFIGURATION RECORD - ";                          
   szLocal += time.Format("%A, %B %d, %Y (%H:%M:%S)"); 
   rStringArray.Add(szLocal);
   
   rStringArray.Add("");   // newline
   return(2);
}


 
/////////////////////////////////////////////////////////////////////////
//  OnWmIoOpListDone()
//
//  CPager::OnOpCompletion initiated event handler.  Receives WM_IO_OPLIST_DONE
//  messages.  
//    wParam = nStatus (0=COMPLETED, 1=FAILED)
//    LOWORD(lParam) = Last CIoOperation ID
afx_msg LONG CPpsView::OnWmIoOpListDone(UINT wParam, LONG lParam)
{
  TRACE("\nCPpsView::OnWmIoOpListDone\n");
  CPpsDoc	*pDoc = GetDocument();
  
  if (pDoc->GetDocSize() > 0)
    theApp.SwitchView(1);	

  return(0L);
}

