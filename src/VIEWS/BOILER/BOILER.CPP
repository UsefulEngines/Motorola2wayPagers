//*************************************************************
//  History:    Date       Author               Comment
//
//  Legacy code for reference only.
//*************************************************************
//
// boiler.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\pps.h"
#include "..\..\ppsdoc.h"
#include "..\..\ppsview.h" 
#include "..\..\cpager\codeplug.h" 
#include "boiler.h"

extern CPpsApp    		theApp;

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBoilerPlate

IMPLEMENT_DYNCREATE(CBoilerPlate, CPpsFormView)

CBoilerPlate::CBoilerPlate()
	: CPpsFormView(CBoilerPlate::IDD)
{
	//{{AFX_DATA_INIT(CBoilerPlate)
	//}}AFX_DATA_INIT
}

CBoilerPlate::~CBoilerPlate()
{
}

void CBoilerPlate::DoDataExchange(CDataExchange* pDX)
{
	CPpsFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBoilerPlate)
	//}}AFX_DATA_MAP
}


// **** NOTE: be sure to uniquely name the Defaults and
// Undo button resource IDs for this view...

BEGIN_MESSAGE_MAP(CBoilerPlate, CPpsFormView)
	//{{AFX_MSG_MAP(CBoilerPlate)
	ON_BN_CLICKED(IDC_BOILER_DEFAULTS_BTN, OnAcceptDefaultsBtn)
	ON_BN_CLICKED(IDC_BOILER_UNDO_BTN, OnUndoBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBoilerPlate diagnostics

#ifdef _DEBUG
void CBoilerPlate::AssertValid() const
{
	CPpsFormView::AssertValid();
}

void CBoilerPlate::Dump(CDumpContext& dc) const
{
	CPpsFormView::Dump(dc);
}
#endif //_DEBUG



////////////////////////////////////////////////////////////////////////////
//	OnInitialUpdate()
//  
//
void CBoilerPlate::OnInitialUpdate(void)
{
  TRACE("CBoilerPlate::OnInitialUpdate : CBoilerPlate::IDD=%X\n", CBoilerPlate::IDD);
  
  // NOTE: any one time initialization follows:
  
  CPpsFormView::OnInitialUpdate();
  return;
}




/////////////////////////////////////////////////////////////////////////////
//	OnUpdate()
//
//  This function is required in order for the view to fit into the
//  'view-switching' mechanizm controlled by the CPpsApp class.
//
void CBoilerPlate::OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint)
{
  TRACE("\nCBoilerPlate::OnUpdate : IDD=%X  LHintHI=%X  lHintLO=%X\n", 
  		CBoilerPlate::IDD,  HIWORD(lHint), LOWORD(lHint));

  if (HIWORD(lHint))        		// user switched views, now active
    ResizeParentToFit(FALSE);

  SetFromDoc();
  CPpsFormView::OnUpdate(pSender, lHint, pHint);
}	


////////////////////////////////////////////////////////////////////////////
//  SetFromDoc()
//
//  This function is required in order for the view to fit into the
//  'view-switching' mechanizm controlled by the CPpsApp class.
//
void CBoilerPlate::SetFromDoc(void)
{
  TRACE("\nCBoilerPlate::SetFromDoc\n");
  CPpsDoc	*pDoc = GetDocument();
  
  // **** Move data from the document to the individual view elements...
  // **** See the other 'view'.cpp files for examples...
      
  UpdateData(FALSE);	// set controls data
}



////////////////////////////////////////////////////////////////////////////
//	SetToDoc()
//
//  This function is required in order for the view to fit into the
//  'view-switching' mechanizm controlled by the CPpsApp class.
//
BOOL CBoilerPlate::SetToDoc(void)
{
  TRACE("\nCBoilerPlate::SetToDoc\n");
  CPpsDoc		*pDoc = GetDocument();
  
  if (!(UpdateData(TRUE)))  // get controls data
    return FALSE;
  
  // *** If the user has modified any view elements, move the data to the document...
  // *** See the other 'view'.cpp files for examples...
    
  return(TRUE);
}




/////////////////////////////////////////////////////////////////////////////
// CBoilerPlate message handlers



////////////////////////////////////////////////////////////////////////
//  OnAcceptDefaultsBtn()
//
//  This function initiates a move of data from select fields of the
//  CPpsDoc.m_byDefaultCodePlugArray to the CPpsDoc.m_byCodePlugArray.
//  The m_byDefaultCodePlugArray is loaded from file 'default.s19' upon
//  application execution.  If the file has not been found, then no 
//  default values are available.  
//
void CBoilerPlate::OnAcceptDefaultsBtn()
{
  TRACE("\nCBoilerPlate::OnAcceptDefaultsBtn\n"); 
  CPpsDoc	*pDoc = GetDocument(); 
   
  // see other view files for examples...
  
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
void CBoilerPlate::OnUndoBtn()
{
  TRACE("\nCBoilerPlate::OnUndoBtn\n"); 
  CPpsDoc	*pDoc = GetDocument(); 
  
  // see other view files for examples...
  
  SetFromDoc();   // update this view
}



////////////////////////////////////////////////////////////////////////
//  OnArchived()
//
//  This function initiates a move of data from select fields of the
//  CPpsDoc.m_byArchivedCodePlugArray to the CPpsDoc.m_byCodePlugArray.
//  The m_byArchivedCodePlugArray is loaded upon user-selection of the
//  Utility-Import_Archived menu option.
//
void CBoiler::OnArchived(void)
{
  CPpsDoc	*pDoc = GetDocument(); 
   
  // see other view files for examples...
    
  SetFromDoc();   // update this view
}

