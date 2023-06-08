//*************************************************************
//  History:    Date       Author               Comment
//              1996       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
//////////////////////////////////////////////////////////////////////////
// hexedit.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CHexEdit form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#ifndef _HEXEDIT_
#define _HEXEDIT_


#ifndef DISABLE_HEXEDIT    // conditional compilation of this file


#define NUM_ROWS	10
#define NUM_COLS	16
  
typedef struct
{ 
  BOOL  bModified;
  UINT	nAddress;
  BYTE	byRow[NUM_COLS];
  char	szText[NUM_COLS+1];
} TRow;


class CNumEdit : public CEdit
{
  public:
    int		nCtrlID;
    BOOL	IsFirstCtrl(void) 
    		{ return(nCtrlID == IDC_ROW0COL0); }
    BOOL	IsLastCtrl(void) 
    		{ return(nCtrlID == IDC_ROW9COLF); } 
    BOOL 	IsInFirstRow(void) 
    		{ return((nCtrlID >= IDC_ROW0COL0) && (nCtrlID <= IDC_ROW0COLF)); }
    BOOL 	IsInLastRow(void) 
    		{ return((nCtrlID >= IDC_ROW9COL0) && (nCtrlID <= IDC_ROW9COLF)); }
  protected:
    afx_msg void OnChar (UINT nChar, UINT nRepCnt, UINT nFlags);
    DECLARE_MESSAGE_MAP ()
};


  
class CHexEdit : public CPpsFormView
{
	DECLARE_DYNCREATE(CHexEdit)
	enum 		{ MIN_SCROLL = 0 };
	int			nMaxScroll;		// max # rows
	int			nPartialRow;
	TRow		Rows[NUM_ROWS];
	CNumEdit    EditCells[NUM_ROWS * NUM_COLS];
    BOOL		bModifiedFlag;
    
    private:
	   BYTE byHexAlpha2Binary(const char ch);
	   
	public:             
		// need these public for view switching and initialization
		CHexEdit();
		virtual void OnInitialUpdate();
	 	BOOL ProcessMessageFilter(int code, LPMSG lpMsg);    
        void UpdateRowText(const CNumEdit *pEdit);

	// Form Data
	public:
	//{{AFX_DATA(CHexEdit)
	enum { IDD = IDD_HEXEDITDLG };
	//}}AFX_DATA

	// Attributes
	public:
		#ifdef _DEBUG
		virtual void AssertValid() const;
		virtual void Dump(CDumpContext& dc) const;
		#endif

	// Operations
	public:
		virtual void SetFromDoc();
		virtual BOOL SetToDoc();
        static  BOOL IsPresentable(void);  	

	// Implementation
	protected:
		virtual ~CHexEdit();
		virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
		virtual void OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint);	
		// Generated message map functions
		//{{AFX_MSG(CHexEdit)
		afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	    afx_msg void OnDownloadNowBtn();
	    //}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
 

#endif  // DISABLE_HEXEDIT - conditional compilation of this file

 
#endif   // _HEXEDIT_ 
 
/////////////////////////////////////////////////////////////////////////////
