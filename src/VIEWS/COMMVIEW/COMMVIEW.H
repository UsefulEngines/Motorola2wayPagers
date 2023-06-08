//*************************************************************
//  History:    Date       Author               Comment
//              1996       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
// commview.h : header file
//
///////////////////////////////////////////////////////////////

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
 
 
#ifndef _COMMVIEW_ 
#define _COMMVIEW_


#ifndef DISABLE_HEXEDIT    // conditional compilation of this file



                                   
class CCommViewer : public CPpsFormView
{
  DECLARE_DYNCREATE(CCommViewer) 
     
    // screen elements 
    CFont 	m_screenFont;
    int		m_nPageY;
    int		m_nLineY;
    
  public:
	//{{AFX_DATA(CCommViewer)
	enum { IDD = IDD_COMMVIEW_FORM };
	//}}AFX_DATA

  public:
	CCommViewer();			
    virtual void OnInitialUpdate();   
  	virtual void SetFromDoc();                  
  	virtual BOOL SetToDoc();	                
    static BOOL IsPresentable(void);  	
    #ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
    #endif                                                             
    // Override to ignore calls to this member function.  An unresolved
    // bug exists with this view class in which the screen is repainted
    // incorrectly upon a call to this function.
    virtual void ResizeParentToFit(BOOL bShrinkOnly = TRUE) { return; }
  
  protected:
	virtual ~CCommViewer();
    virtual void OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint);    
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    virtual void OnDraw(CDC* pCDC);
	virtual BOOL OnPreparePrinting(CPrintInfo *pInfo);
	virtual void OnPrint(CDC *pDC, CPrintInfo *pInfo);

	// Generated message map functions
	//{{AFX_MSG(CCommViewer)
	//}}AFX_MSG
	afx_msg LONG OnWmIoOpListDone(WPARAM, LPARAM); 
	DECLARE_MESSAGE_MAP()
   
  private:   
    // both "ViewPlugs" share these member functions
    void OnNote(const char *pszPort, const char *pszOp, BYTE *pData, int nNumBytes);
    void OnNote(const char *pszPort, const char *pszOp, const char *pszString);
    void OnError(const char *pszPort, const char *pszOp, DWORD nStatusCode); 
   
   // CWin16Comm callback interface - see file 'w16comm.h'.
   class ICommViewPlug : public ICommViewSocket
   {
     virtual void	CommNote(const char *pszPort, const char *pszOp, BYTE *pData, int nNumBytes);
     virtual void   CommNote(const char *pszPort, const char *pszOp, const char *pszString); 
     virtual void   CommError(const char *pszPort, const char *pszOp, DWORD nStatusCode);     
   } m_ICommViewPlug;
   friend class ICommViewPlug;  

   // CWin16Gpib callback interface - see file 'w16gpib.h'.
   class IGpibViewPlug : public IGpibViewSocket
   {
     virtual void	GpibNote(const char *pszPort, const char *pszOp, BYTE *pData, int nNumBytes);
     virtual void   GpibNote(const char *pszPort, const char *pszOp, const char *pszString); 
     virtual void   GpibError(const char *pszPort, const char *pszOp, DWORD nStatusCode);     
   } m_IGpibViewPlug;
   friend class IGpibViewPlug;  
};


#endif  // DISABLE_HEXEDIT - conditional compilation of this file
 
 
#endif   // _COMMVIEW_
 
/////////////////////////////////////////////////////////////////////////////
