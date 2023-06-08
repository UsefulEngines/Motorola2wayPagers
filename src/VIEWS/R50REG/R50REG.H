//*************************************************************
//  History:    Date       Author               Comment
//              1996       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
// r50reg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CReflex50Registration form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
 
 
#ifndef _R50REG_ 
#define _R50REG_
 
#include "..\..\misc\spinbtn\spinbtn.h"                                  

                                   
class CReflex50Registration : public CPpsFormView
{
  DECLARE_DYNCREATE(CReflex50Registration) 
  
  CSpinBtn		AggrAttackUp;
  CSpinBtn		AggrAttackDn;
  
  CSpinBtn		AggrCeilingUp;
  CSpinBtn		AggrCeilingDn;
  
  CSpinBtn		AggrRetryCntUp;
  CSpinBtn		AggrRetryCntDn;
  
  CSpinBtn		AggrRetryTimerUp;
  CSpinBtn		AggrRetryTimerDn;
  
  CSpinBtn		CommunicadoUp;
  CSpinBtn		CommunicadoDn;
  
  CSpinBtn		InCommunicadoUp;
  CSpinBtn		InCommunicadoDn;
  
  CSpinBtn		InRangeTimerUp;
  CSpinBtn		InRangeTimerDn;
  
  CSpinBtn		OutOfRangeTimerUp;
  CSpinBtn		OutOfRangeTimerDn;
  
  CSpinBtn		AlohaRetryCntUp;
  CSpinBtn		AlohaRetryCntDn;
  
  CSpinBtn		AlohaRetryTimerUp;
  CSpinBtn		AlohaRetryTimerDn;			  

  CSpinBtn		AlohaRandomIntervalUp;
  CSpinBtn		AlohaRandomIntervalDn;			  

  // Form Data
  public:
	//{{AFX_DATA(CReflex50Registration)
	enum { IDD = IDD_R50REG_FORM };
	UINT	m_nAggRegRetryCnt;
	UINT	m_nAggRegRetryTmr;
	UINT	m_nAggRegCeiling;
	BYTE	m_byAggRegAttack;
	UINT	m_nAlohaRetryTimer;
	UINT	m_nAlohaRetryCount;
	BOOL	m_bAllowAutoRegOnZoneChg;
	UINT	m_nCommunicadoTmr;
	UINT	m_nInCommunicadoTmr;
	BYTE	m_byInRangeThreshold;
	BYTE	m_byOutOfRangeThreshold;
	BOOL	m_bNotifyTxChange;
	BYTE	m_byAlohaRndInterval;
	//}}AFX_DATA

  // Operations
  public:
	CReflex50Registration();			
    virtual void OnInitialUpdate();   
  	virtual void SetFromDoc();                  
  	virtual BOOL SetToDoc();	                
	virtual void OnDefaults(void) { OnAcceptDefaultsBtn(); }
	virtual void OnUndo(void) { OnUndoBtn(); }
	virtual void OnArchived(void);
  	virtual int OnPrintHelper(CStringArray &rStringArray,  UINT nPageNumber, int nAvailableLines);
    static BOOL IsPresentable(void); 	
    #ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
    #endif
  
  // Implementation
  protected:
	virtual ~CReflex50Registration();
    virtual void OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint);    
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	// Generated message map functions
	//{{AFX_MSG(CReflex50Registration)
	afx_msg void OnAcceptDefaultsBtn();  
	afx_msg void OnUndoBtn();             
	//}}AFX_MSG
	afx_msg LONG OnWmSpinButton(UINT, LONG);
	DECLARE_MESSAGE_MAP()
};

 
#endif   // _R50REG_
 
/////////////////////////////////////////////////////////////////////////////
