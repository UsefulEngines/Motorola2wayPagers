//*************************************************************
//  History:    Date       Author               Comment
//
//  Legacy code for reference only.
//*************************************************************
//
// boiler.h : header file
//

////////////////////////////////////////////////////////////////
// CBoilerPlate form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
 
 
#ifndef _BOILER_ 
#define _BOILER_
 
///**** THIS FILE IS NOT PART OF THE PROJECT
//
// Note:  This file is a boiler plate for all of the
// views in the pps project.  Do not include this file
// in the compilation process.  
//
// The purpose of this file is to make it easy to 
// add new views to the project.  Since all of the
// views must have the same subset of member functions
// as this boiler plate, it helps maintain continuity.
//
// Note also, the corresponding modifications to files
// 'pps.h', 'pps.cpp', and 'mainfrm.cpp' which are required to link a
// new view into the application.
//                                    


// ***** HOW TO ADD A NEW VIEW TO THE PROJECT
//
// 0.  Create a new directory for the view under the
//     'c:\reflxpps\pps16\views' directory.
// 1.  Using file manager, copy files 'boiler.h' and
// 	   'boiler.cpp' to the new directory.
// 3.  Rename the 'boiler.*' files to 'NewView.*'.
// 4.  Open the 'NewView.h' file and globally replace the string
//     'CBoilerPlate' with 'CNewView'.  Also replace the _BOILER_
//     conditional #define with _NEWVIEW_.
// 5.  Replace the string 'IDD_BOILERPLATE_FORM' with
//     'IDD_NEWVIEW_FORM'. 
// 6.  Open the 'NewView.cpp' file and globally replace the string
//     'CBoilerPlate' with 'CNewView'.
// 7.  Replace the string 'IDC_BOILER_DEFAULTS_BTN' with
//     'IDC_NEWVIEW_DEFAULTS_BTN'.  Also replace the string
//     'IDC_BOILER_UNDO_BTN' with 'IDC_NEWVIEW_UNDO_BTN'.
// 8.  Open the 'resource.h' file, search for the next sequential 
//     _APS_NEXT_COMMAND_VALUE.  Manually add a new command ID define 
//     for the new view (e.g. ID_VIEW_NEWVIEW).  Be sure to increment the
//     _APS_NEXT_COMMAND_VALUE at the bottom of the file.
// 9.  From AppStudio, copy the IDD_BOILERPLATE_FORM dialog resource
//     and issue a paste operation.  A new dialog resource will be 
//     automatically added to the dialog resource list.  Change the
//     new resource ID via the 'Properties' button to IDD_NEWVIEW_FORM.
// 10.  Invoke the Dialog Editor and change the resource ID's of the 
//     'Accept Defaults' button and the 'Undo' button to IDC_NEWVIEW_DEFAULTS_BTN
//     and IDC_NEWVIEW_UNDO_BTN respectively.
// 11. Invoke the String Editor and add a status bar prompt for the new view
//     command ID.  Ensure that string resource ID is the same as the 
//     command ID which was manually added in step 8 above (e.g. ID_VIEW_NEWVIEW).
//     Also, add a '\nNewView' tool-tip string to the end of the status bar
//     prompt string.  This tool-tip string will also serve as the View sub-menu
//     option (the view sub-menu is created dynamically, see file mainfrm.cpp).  
// 12. Invoke the Bitmap Editor and add a new bitmap to the right end of the
//     application tool bar bitmap.
// 13. Edit files 'mainfrm.h' and 'mainfrm.cpp' adding OnCommand and OnCommandUI 
//     member functions to the CMainFrame class for the new menu item ID_VIEW_NEWVIEW.
// 14. Edit file 'pps.cpp' and add the view class information to the ViewInfo
//     structure array.  See instructions in that file.  
// 15. Save all files and add the 'NewView.cpp' file to the project via the
//     'Project-Edit' menu selection.
// 16. Build the project.  The new view should have a bitmap on the toolbar
//     with tool-tip support.  It should also have an Defaults and Undo button.
// 17. Customize the view by adding resources and support code.  Note the commented
//     code sections in the member functions which should provide a quide for
//     accessing the CDocument object.  Analyze the other application views for
//     additional help.


//  This class is inherited from the CPpsFormView class as declared in
//  file 'ppsview.h' and defined in file 'ppsview.cpp'.  
// 
//  *** NOTE:  Be sure to review the base class virtual functions declared
//  in class CPpsFormView (file ppsview.h).  Some of those functions may
//  need to be overridden in this class definition (e.g. printing support...).
//                                  
class CBoilerPlate : public CPpsFormView
{
  DECLARE_DYNCREATE(CBoilerPlate) 
    
  public:
	//{{AFX_DATA(CBoilerPlate)
	enum { IDD = IDD_BOILERPLATE_FORM };
	//}}AFX_DATA

  public:
	CBoilerPlate();			         
	
	// See class CPpsFormView in file ppsview.h for details
	// about the following virtual functions and others.
    virtual void OnInitialUpdate();   
  	virtual void SetFromDoc();                  
  	virtual BOOL SetToDoc();	                
	virtual void OnDefaults(void) { OnAcceptDefaultsBtn(); }
	virtual void OnUndo(void) { OnUndoBtn(); }
	virtual void OnArchived(void);
	
	// diagnostics
    #ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
    #endif
  
  protected:
	virtual ~CBoilerPlate();
    virtual void OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint);    
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	// Generated message map functions
	//{{AFX_MSG(CBoilerPlate)
	afx_msg void OnAcceptDefaultsBtn();  
	afx_msg void OnUndoBtn();             
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

 
 
#endif   // _BOILER_
 
/////////////////////////////////////////////////////////////////////////////
