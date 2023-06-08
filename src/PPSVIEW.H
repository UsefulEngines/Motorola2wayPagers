//*************************************************************
//  History:    Date       Author               Comment
//              1996       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
/////////////////////////////////////////////////////////////////////////////
// ppsview.h : interface of the CPpsView class
//
/////////////////////////////////////////////////////////////////////////////


#ifndef _PPSVIEW_
#define _PPSVIEW_ 


/////////////////////////////////////////////////////////////////////////
// **** NOTE
// The subdirectory 'views/boiler' contains files 'boiler.h' and 'boiler.cpp'
// which contain an example implementation of a new view for the application.
// When additional views are needed, simply copy these files to a new 
// 'views' subdirectory and follow the instructions in the 'boiler.h' file.
//////////////////////////////////////////////////////////////////////////

 

//////////////////////////////////////////////////////////////////////////
// CPpsFormView : base class for all views in the PPS application
//
// All PPS application view classes should be derived from this
// class.  The virtual functions declared herein should be 
// redefined in the inherited class.  
//
class CPpsFormView : public CFormView
{
  DECLARE_DYNAMIC(CPpsFormView)
  
  UINT			nHelpContextHook;  // Use the derived class Resource ID to
                                   // link into context-sensitive help.
  protected:      
	CPpsFormView(UINT nIDTemplate);

  public:
  #ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
  #endif
  
  // The following two functions need to be overridden in inherited classes
  // and must be made public in order for the CPpsApp class
  // to access them.  See the implementation of these two functions in the
  // various view classes used in this application for examples.
  public: 
	virtual void OnInitialUpdate(void);
    virtual void OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint);
	// override to fix bug in MFC and modify default behavior
    virtual void ResizeParentToFit(BOOL bShrinkOnly = TRUE);  

  public:
	// The following functions must be defined in each class
	// derived from this class. 
  	virtual void SetFromDoc() = 0;
  	virtual BOOL SetToDoc() = 0;   
  	// The following function may be re-defined in any derived class
  	// if that class has printing information to contribute.  Returns the
  	// number of lines contributed.  If not enough available lines, return (-1);	
  	virtual int OnPrintHelper(CStringArray &rStringArray, UINT nPageNumber, int nAvailableLines) { return(0); }
  	// The following functions may be re-defined in any derived class
  	// if that class implements default/undo/archive capability.
	virtual void OnDefaults(void) { return; }
	virtual void OnUndo(void) { return; } 
	virtual void OnArchived(void) { return; }
  	 
    // The following function may need to be implemented in a derived class
    // if there are access restrictions to the view.  Ensure that it is implemented
    // statically such that the class need not be instantiated in order to 
    // determine accessability.  See pps.cpp and the views which implement it (e.g. CCannedMsgs).
    static BOOL IsPresentable(void) { return(TRUE); }
    
  protected:
	CPpsDoc* GetDocument();
	// Printing support
	virtual BOOL OnPreparePrinting(CPrintInfo *pInfo);
	virtual void OnBeginPrinting(CDC *pDC, CPrintInfo *pInfo);
	virtual void OnEndPrinting(CDC *pDC, CPrintInfo *pInfo);
	virtual void OnPrint(CDC *pDC, CPrintInfo *pInfo);

	//{{AFX_MSG(CPpsFormView)
	//}}AFX_MSG 
	afx_msg LRESULT OnHelpHitTest(WPARAM wParam, LPARAM lParam); 
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ppsview.cpp
inline CPpsDoc* CPpsFormView::GetDocument()
   { return (CPpsDoc*)m_pDocument; }
#endif



   






/////////////////////////////////////////////////////////////////////
// CPpsView : The initial application view (with the Logo bitmap)
// 
class CPpsView : public CPpsFormView
{
	DECLARE_DYNCREATE(CPpsView) 
	BOOL 	bBitmapLoaded;
	
  public:
  	// need these public for view switching and initialization 
	CPpsView();
    virtual void OnInitialUpdate();
       
  public:
	//{{AFX_DATA(CPpsView)
	enum{ IDD = IDD_PPS_FORM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

  // Attributes
  public:
    // for field updating and saving
  	virtual void SetFromDoc();
  	virtual BOOL SetToDoc();	// called from SwitchView() 
  	virtual int OnPrintHelper(CStringArray &rStringArray,  UINT nPageNumber, int nAvailableLines);

  protected:
    CDC m_dcMem;            // Compatible Memory DC for form view
    CBitmap m_bmpLogo;    	// Bitmap to display
    HBITMAP m_hBmpOld;    	// Handle of old bitmap to save
    BITMAP m_bmInfo;        // Bitmap Information structure
    CSize m_size;        	// Size (width and height) of bitmap

  // Implementation
  public:
	virtual ~CPpsView();
  protected:
	virtual void OnPrepareDC(CDC *pDC, CPrintInfo* pInfo);
	virtual void OnDraw(CDC *pDC);
	
  #ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
  #endif

  protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual void OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint);
    
  // Generated message map functions
  protected:
	//{{AFX_MSG(CPpsView)
	afx_msg BOOL OnEraseBkgnd(CDC *pDC);
	//}}AFX_MSG
	afx_msg LONG OnWmIoOpListDone(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP() 
};


#endif   // _PPSVIEW_

/////////////////////////////////////////////////////////////////////////////
