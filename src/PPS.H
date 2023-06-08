//*************************************************************
//  File name: PPS.H
//
//  Description:
//		Main header file for the pps.exe application.  This
//		application is a MFC Single Document Interface (SDI)
//		with multiple views.    
//
//
//  Comments:
//		
//      
//      
//
//  History:    Date       Author               Comment
//              3/96       P. Pennington        Created
//
// Written by Motorola AMG, PMU Engineering.
// Legacy code for reference only.
//*************************************************************
#ifndef _PPS_
#define _PPS_

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       				// main symbols
#include "cpager\cpager.h"

// WM_USER messages defined for this application
#define WM_READ_CP_DONE  			(WM_USER+5)
#define WM_WRITE_CP_DONE			(WM_USER+6) 
#define WM_UNUSED		    		(WM_USER+7)   // use this entry next
// #define WM_SPIN_BUTTON			(WM_USER+9)   // see 'spinbtn.h' 
// #define WM_IO_OPERATION          (WM_USER+10)  // see 'ciooper.h' 
// #define WM_IO_OPLIST_DONE	    (WM_USER+11)  // see 'ciooper.h'
// #define WM_SPIN_START			(WM_USER+12)  // see 'spinbtn.h'
// #define WM_SPIN_STOP				(WM_USER+13)  // see 'spinbtn.h' 


/////////////////////////////////////////////////////////////////////////////
//  CPpsApp:
//  
//  The CPpsApp class serves to instantiate all the document/view classes
//  in the application.  This MFC base application is to be considered a 
//  Single Document Interface (SDI) with multiple views.  Only 1 view is
//  selected at any time.  This view is the active view.  The CPpsApp class
//  manages the switching of views.  The user may initiate the switching via
//  menu or toolbar selections, or, the application may switch itself based
//  upon run-time events (e.g. Device-Read, File-Open, etc.).
//
//  Only 1 view class is instantiated at any time unless the user initiates
//  an action which requires the application to iterate through all of the
//  views in the set.  Then, each inactive view is instantiated (but not
//  displayed) and the required member function called.  The inactive view
//  is thereafter destroyed.
//
//  See pps.cpp for the implementation of this class
//


typedef struct tagViewInfo
{
  CRuntimeClass		*pViewClass;     			// runtime info struct
  UINT				nCommandId;		 			// corresponding menu command id
  int				nToolBarBmp;	 			// corresponding toolbar bitmap index
  BOOL				(*pfnIsPresentable)(void);  // check with the view class				
} ViewInfo;
 



typedef struct tagPagerInfo
{
  CRuntimeClass		*pPagerClass;			    // runtime info struct
  BYTE				byPagerId;					// unique device identifier
  BYTE				byCodeplugVersion;			// supported codeplug version
  BOOL				bReflex50;					// TRUE if R50 enabled, FALSE otherwise
  char				*pszDefaultS19File;			// default codeplug file name
} PagerInfo;



class CPpsApp : public CWinApp
{
  protected: 
    CPager		*m_pThePager;			// current CPager derived class pointer
    int			m_nCurView;             // current view array index 
    int			m_nNumViews;			// number of structs in ViewInfo list   
    ViewInfo	*m_pViewInfo; 			// ptr to current ViewInfo list
    CString		m_strAppPathName;		// initial working directory
    
  public:
	CPpsApp();    
    
    // Operations 
    BOOL		IsPagerSupported(BYTE byPagerId);
    BOOL		IsPagerSupported(BYTE byPagerId, BYTE byCodeplugVersion, BOOL bR50Enabled);
    const char	*GetDefaultS19FileName(BYTE byPagerId, BYTE byCodeplugVersion, BOOL bR50Enabled);
    void		SelectThePager(BYTE byPagerId);
	void		SelectViewSet(BYTE byDeviceId, BYTE byCodePlugVersion, BOOL bR50Enabled);
    CView		*SwitchView(int nIndex, BOOL bValidateAndSave = TRUE);	
	CView 		*SwitchView(CRuntimeClass *pViewClass, BOOL bValidateAndSave = TRUE); 
	CView		*SwitchViewOnCommand(UINT nCommandID, BOOL bValidateAndSave = TRUE);
    int			GetNumViews(void) { return(m_nNumViews); }
	int			GetCurViewIndex(void) { return(m_nCurView); }
	const CRuntimeClass *GetCurViewClass(void) 
	            { return(m_pViewInfo[m_nCurView].pViewClass); } 
    int 		GetViewIndex(CRuntimeClass *pViewClass);
	UINT		GetCurViewCommandId(void)
	            {
	              return(m_pViewInfo[m_nCurView].nCommandId);
	            }
	UINT		GetViewCommandId(int nIndex)  
	            { if (ValidIndex(nIndex)) 
	                return(m_pViewInfo[nIndex].nCommandId);
	              else
	                return(m_pViewInfo[0].nCommandId);
	            }
	int			GetViewToolbarBmp(int nIndex)
	            { if (ValidIndex(nIndex)) 
	                return(m_pViewInfo[nIndex].nToolBarBmp);
	              else
	                return(m_pViewInfo[0].nToolBarBmp);
	            }
	BOOL		IsViewPresentable(CRuntimeClass *pViewClass);
	BOOL		IsViewPresentable(int nIndex);
    BOOL		SaveActiveViewsData();		
    CPager      *GetThePager(void);
    const CString &GetAppPathName(void) { return(m_strAppPathName); }
	void 		OnPrintHelper(CStringArray &rStringArray, UINT nPageNumber); 
	void		StatusMsg(const CString &rstrMsg);
	void		StatusMsg(UINT nStringResourceId);
	void		StatusMsgClear(void);
	BOOL		IsLabUseOnly(void);
	
	  
    // Overrides
	virtual 	BOOL InitInstance();
	virtual 	BOOL ProcessMessageFilter(int code, LPMSG lpMsg);    

  private:
	CView 		*CreateNewView(CRuntimeClass *pViewClass); 
	void 		DestroyOldView(CView *pView);
	BOOL		ValidIndex(int Index)  { return((Index >= 0) && (Index < m_nNumViews)); }
	
  public:
	//{{AFX_MSG(CPpsApp)
	afx_msg void OnAppAbout();
	afx_msg void OnUtilitySetAllDefault();
	afx_msg void OnUpdateUtilitySetAllDefault(CCmdUI* pCmdUI);
	afx_msg void OnSetAllDefault();
	afx_msg void OnUtilitySetAllArchive();
	afx_msg void OnUpdateUtilitySetAllArchive(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
 
 
#endif  // _PPS_



 