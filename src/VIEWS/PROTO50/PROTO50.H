//*************************************************************
//  History:    Date       Author               Comment
//              1996       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
// proto50.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CReflex50Protocol form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
 
 
#ifndef _PROTO50_ 
#define _PROTO50_
 
#include "..\..\misc\spinbtn\spinbtn.h"                                  
#include "..\..\misc\numedit.h"


                                   
class CReflex50Protocol : public CPpsFormView
{
  DECLARE_DYNCREATE(CReflex50Protocol) 
  
  CSpinBtn		InfoSvcFrameUpBtn;
  CSpinBtn		InfoSvcFrameDnBtn;
  CDecimalEdit	InfoSvcFrameEdit;
  
  CSpinBtn		InfoSvcCollapseUpBtn;
  CSpinBtn		InfoSvcCollapseDnBtn;
  CDecimalEdit	InfoSvcCollapseEdit;
  
  CSpinBtn		PersonalFrameUpBtn;
  CSpinBtn		PersonalFrameDnBtn; 
  CDecimalEdit	PersonalFrameEdit;
  
  CSpinBtn		PersonalCollapseUpBtn;
  CSpinBtn		PersonalCollapseDnBtn;
  CDecimalEdit	PersonalCollapseEdit;
  
  CSpinBtn		HomeLogicalCtrlUpBtn;
  CSpinBtn		HomeLogicalCtrlDnBtn;
  CDecimalEdit	HomeLogicalCtrlEdit;
  
  CSpinBtn		FwdChanFreq0UpBtn;
  CSpinBtn		FwdChanFreq0DnBtn;
  CDecimalEdit	FwdChanFreq0Edit;
  
  CSpinBtn		AsyncSleepUpBtn;
  CSpinBtn		AsyncSleepDnBtn;
  CDecimalEdit	AsyncSleepEdit;
  
  CSpinBtn		RevChanFreq0UpBtn;
  CSpinBtn		RevChanFreq0DnBtn;
  CDecimalEdit	RevChanFreq0Edit;
    
  CSpinBtn		RevChanFreq1UpBtn;
  CSpinBtn		RevChanFreq1DnBtn;
  CDecimalEdit	RevChanFreq1Edit;
    
  CSpinBtn		RevChanFreq2UpBtn;
  CSpinBtn		RevChanFreq2DnBtn;
  CDecimalEdit	RevChanFreq2Edit;
    
  CSpinBtn		RevChanFreq3UpBtn;
  CSpinBtn		RevChanFreq3DnBtn;
  CDecimalEdit	RevChanFreq3Edit;
    
  CSpinBtn		RevChanFreq4UpBtn;
  CSpinBtn		RevChanFreq4DnBtn;
  CDecimalEdit	RevChanFreq4Edit;
    
  CSpinBtn		RevChanFreq5UpBtn;
  CSpinBtn		RevChanFreq5DnBtn;
  CDecimalEdit	RevChanFreq5Edit;
    
  CSpinBtn		RevChanFreq6UpBtn;
  CSpinBtn		RevChanFreq6DnBtn;
  CDecimalEdit	RevChanFreq6Edit;
   
  // local data
  UINT			m_nFwdChan0;
  BYTE			m_byRevChan0;
  BYTE			m_byRevChan1;
  BYTE			m_byRevChan2;
  BYTE			m_byRevChan3;
  BYTE			m_byRevChan4;
  BYTE			m_byRevChan5;
  BYTE			m_byRevChan6;
  enum			{ MINFWDCHAN=0, MAXFWDCHAN=0xFFF };    // 4096-1
  enum			{ MINREVCHAN=0, MAXREVCHAN=0x4F };     // 79
  int			m_nMinFwdChanNum;                      // variable w/CP_Version >= 0x07

  // Form Data
  public:
	//{{AFX_DATA(CReflex50Protocol)
	enum { IDD = IDD_PROTOCOL50_FORM };
	double	m_dFwdChanFreq0;
	BYTE	m_byHomeLogicalNum;
	BYTE	m_byInfoFrame;
	BYTE	m_byInfoSvcCollapse;
	BYTE	m_byPersonalCollapse;
	BYTE	m_byPersonalAddressFrame;
	double	m_dRevChanFreq0;
	double	m_dRevChanFreq1;
	double	m_dRevChanFreq2;
	double	m_dRevChanFreq3;
	double	m_dRevChanFreq4;
	double	m_dRevChanFreq5;
	double	m_dRevChanFreq6;
	BYTE	m_byAsyncSleepFrames;
	BOOL	m_bDisableRevCh0;
	BOOL	m_bDisableRevCh1;
	BOOL	m_bDisableRevCh2;
	BOOL	m_bDisableRevCh3;
	BOOL	m_bDisableRevCh4;
	BOOL	m_bDisableRevCh5;
	BOOL	m_bDisableRevCh6;
	BOOL	m_bChannel_Id;
	//}}AFX_DATA

  // Operations
  public:
	CReflex50Protocol();			
    virtual void OnInitialUpdate();   
  	virtual void SetFromDoc();                  
  	virtual BOOL SetToDoc();	                
	virtual void OnDefaults(void) { OnAcceptDefaultsBtn(); }
	virtual void OnUndo(void) { OnUndoBtn(); }
	virtual void OnArchived(void);
  	virtual int OnPrintHelper(CStringArray &rStringArray, UINT nPageNumber, int nAvailableLines);
    static BOOL IsPresentable(void);  	
    #ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
    #endif
  
  private:
  	enum {  NO_ROUNDING=0, ROUND_UP=1, ROUND_DN=2 };  
  
  // Implementation
  public:  // make these functions accessible to other classes 
  	static double dCalcReverseChanFreq(BYTE byChanNum);
  	static double dCalcForwardChanFreq(int nChanNum);
  	static int nCalcForwardChanNum(double dFwdFreq, BYTE byRounding = NO_ROUNDING); 
  	static BYTE byCalcReverseChanNum(double dRevFreq, BYTE byRounding = NO_ROUNDING);

  private:
    void DoRevChanDDX(CDataExchange *pDX, WORD wResId, 
					  double &rdFreq, BOOL bDisable, CEdit &rEdit);
    void OnRevChanNumIncrement(BYTE *pbyRevChanNum, double *pdFrequency, int nOffsetBy=1);
    void OnRevChanNumDecrement(BYTE *pbyRevChanNum, double *pdFrequency, int nOffsetBy=1);
    void OnFwdChanNumIncrement(int nOffset);
    void OnFwdChanNumDecrement(int nOffset);
	void OnHomectrlUp(void);
	void OnHomectrlDn(void);
	void OnInfosvcframeUp(void);
	void OnInfosvcframeDn(void);
	void OnInfosvccollapseUp(void);
	void OnInfosvccollapseDn(void);
	void OnPersframeUp(void);
	void OnPersframeDn(void);
	void OnPerscollapseUp(void);
	void OnPerscollapseDn(void);
	void OnAsyncsleepUp(void);
	void OnAsyncsleepDn(void);
    void OnDisableReverseChan(CDecimalEdit &rEdit, 
                              CSpinBtn &rUpBtn, 
                              CSpinBtn &rDnBtn,
                              BYTE *pbyRevChanX,
                              double *pdRevChanXFreq, 
                              BOOL *pbEnable);
  	
  protected:
	virtual ~CReflex50Protocol();
    virtual void OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint);    
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	// Generated message map functions
	//{{AFX_MSG(CReflex50Protocol)
	afx_msg void OnAcceptDefaultsBtn();  
	afx_msg void OnUndoBtn();             
	afx_msg void OnDisableRevch0();
	afx_msg void OnDisableRevch1();
	afx_msg void OnDisableRevch2();
	afx_msg void OnDisableRevch3();
	afx_msg void OnDisableRevch4();
	afx_msg void OnDisableRevch5();
	afx_msg void OnDisableRevch6();
	//}}AFX_MSG
	afx_msg LONG OnWmSpinButton(UINT, LONG);
	DECLARE_MESSAGE_MAP()
};

#endif   // _PROTO50_
 
/////////////////////////////////////////////////////////////////////////////
