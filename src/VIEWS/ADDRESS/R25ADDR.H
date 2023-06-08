//*************************************************************
//  History:    Date       Author               Comment
//              1997       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
// r25addr.h : header file
//
// This class declaration is specific to the Reflex25 Centuar
// Family of pagers.  Specifically, units with PagerId=0x85 or
// greater.
//

/////////////////////////////////////////////////////////////////////////////
// CR25Address form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
 
#ifndef _R25ADDRESS_
#define _R25ADDRESS_ 
 
#include "..\..\misc\numedit.h"
                                   
                                   
class CR25Address : public CPpsFormView
{
  DECLARE_DYNCREATE(CR25Address) 
   
  // the following CEdit controls accept only numeric input 
  CDecimalEdit 		PersAddrEdit1, HomeIndexEdit;
  CDecimalEdit		InfoSvcEdit0, InfoSvcEdit1, InfoSvcEdit2, 
  					InfoSvcEdit3, InfoSvcEdit4, InfoSvcEdit5; 
  					
  CDecimalEdit		SpidEdit0, SpidEdit1, SpidEdit2, SpidEdit3,
   	                SpidEdit4, SpidEdit5, SpidEdit6, SpidEdit7;
   	                
  CDecimalEdit		SpidZnEdit0, SpidZnEdit1, SpidZnEdit2, SpidZnEdit3,
                    SpidZnEdit4, SpidZnEdit5, SpidZnEdit6, SpidZnEdit7;
  
  int				m_nOrigNumSpids, m_nOrigNumScids, m_nSavedNumSpids, m_nSavedNumScids;
  
  // Form Data
  public:
	//{{AFX_DATA(CR25Address)
	enum { IDD = IDD_R25ADDRESS_FORM };
	DWORD	m_dwPersonalAddress1;
	DWORD	m_dwInfoSvcAddr0;
	DWORD	m_dwInfoSvcAddr1;
	DWORD	m_dwInfoSvcAddr2;
	DWORD	m_dwInfoSvcAddr3;
	DWORD	m_dwInfoSvcAddr4;
	DWORD	m_dwInfoSvcAddr5;
	BOOL	m_bInfoSvc0Disable;
	BOOL	m_bInfoSvc1Disable;
	BOOL	m_bInfoSvc2Disable;
	BOOL	m_bInfoSvc3Disable;
	BOOL	m_bInfoSvc4Disable;
	BOOL	m_bInfoSvc5Disable;
	BOOL	m_bSpidPrimChk0;
	BOOL	m_bSpidPrimChk1;
	BOOL	m_bSpidPrimChk2;
	BOOL	m_bSpidPrimChk3;
	BOOL	m_bSpidPrimChk4;
	BOOL	m_bSpidPrimChk5;
	BOOL	m_bSpidPrimChk6;
	BOOL	m_bSpidPrimChk7;
	UINT	m_nSpid0;
	UINT	m_nSpid1;
	UINT	m_nSpid2;
	UINT	m_nSpid3;
	UINT	m_nSpid4;
	UINT	m_nSpid5;
	UINT	m_nSpid6;
	UINT	m_nSpid7;
	BOOL	m_bSpidZoneChk0;
	BOOL	m_bSpidZoneChk1;
	BOOL	m_bSpidZoneChk2;
	BOOL	m_bSpidZoneChk3;
	BOOL	m_bSpidZoneChk4;
	BOOL	m_bSpidZoneChk5;
	BOOL	m_bSpidZoneChk6;
	BOOL	m_bSpidZoneChk7;
	UINT	m_nSpidZone0;
	UINT	m_nSpidZone1;
	UINT	m_nSpidZone2;
	UINT	m_nSpidZone3;
	UINT	m_nSpidZone4;
	UINT	m_nSpidZone5;
	UINT	m_nSpidZone6;
	UINT	m_nSpidZone7;
	BOOL	m_bSpidDisableChk0;
	BOOL	m_bSpidDisableChk1;
	BOOL	m_bSpidDisableChk2;
	BOOL	m_bSpidDisableChk3;
	BOOL	m_bSpidDisableChk4;
	BOOL	m_bSpidDisableChk5;
	BOOL	m_bSpidDisableChk6;
	BOOL	m_bSpidDisableChk7;
	DWORD	m_dwHomeIndex;
	//}}AFX_DATA

  // Operations
  public:
	CR25Address();			
    virtual void OnInitialUpdate();
  	virtual void SetFromDoc();
  	virtual BOOL SetToDoc();	
	virtual void OnDefaults(void) { OnAcceptDefaultsBtn(); }
	virtual void OnUndo(void) { OnUndoBtn(); }
	virtual void OnArchived(void);
  	virtual int OnPrintHelper(CStringArray &rStringArray, UINT nPageNumber, int nAvailableLines);
    #ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
    #endif
    
  // Implementation 
  private:
    void UnPackSpid(BOOL *pbZone, BOOL *pbPrim, BOOL *pbDisable, 
                    UINT *pnSpid, UINT *pnZone, int *pnIndex);
    void DoSpidSpecialDDX(BOOL bDisable, BOOL bZoned, UINT nSpidEditIDC, 
	                      UINT nDisableCheckIDC, UINT nPrimCheckIDC,
	                      UINT nZoneCheckIDC, UINT nZoneEditIDC);
    void DoInfoSvcSpecialDDX(CDataExchange* pDX, BOOL bDisable, 
                             UINT nInfoSvcEditIDC, DWORD &rdwInfoSvcAddr,
                             CDecimalEdit &rInfoSvcEdit);
    void ConstructInfoSvcPrintString(CString &rStr, BOOL bInfoSvcDisable, 
    								 DWORD dwInfoSvcAddr);
    void PackSpid(BOOL bZone, BOOL bPrim, UINT nSpid, UINT nZone, int *pnIndex);
    void OnSpidDisable(BOOL &rbDisable, BOOL &rbZoned, BOOL &rbPrim,
                       UINT &rnSpid, UINT &rnZone);
    void ConstructSpidPrintString(CString &rStr, BOOL bDisable, UINT nSpid,
                                  BOOL bPrim, BOOL bZoned, UINT nZone);
    void PackSpidOverFlowMsg(void);
    
  protected:
	virtual ~CR25Address();
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    virtual void OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint);
	// Generated message map functions
	//{{AFX_MSG(CR25Address)
	afx_msg void OnAcceptDefaultsBtn();
	afx_msg void OnUndoBtn();
	afx_msg void OnSpidDisableChk0();
	afx_msg void OnSpidDisableChk1();
	afx_msg void OnSpidDisableChk2();
	afx_msg void OnSpidDisableChk3();
	afx_msg void OnSpidDisableChk4();
	afx_msg void OnSpidDisableChk5();
	afx_msg void OnSpidDisableChk6();
	afx_msg void OnSpidDisableChk7();
	//}}AFX_MSG
	afx_msg void OnInfoSvcDisableChk();
	afx_msg void OnSpidZoneChk();
	DECLARE_MESSAGE_MAP()
};


#endif   // _R25ADDRESS_

/////////////////////////////////////////////////////////////////////////////
