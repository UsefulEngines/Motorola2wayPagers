//*************************************************************
//  History:    Date       Author               Comment
//              1996       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
// address.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAddressInfo form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
 
#ifndef _ADDRESS_
#define _ADDRESS_ 
 
#include "..\..\misc\numedit.h"
                                   
                                   
class CAddressInfo : public CPpsFormView
{
  DECLARE_DYNCREATE(CAddressInfo) 
   
  // the following CEdit controls accept only hex numeric input 
  CDecimalEdit 		PersAddrEdit1, PersAddrEdit2;
  CDecimalEdit		InfoSvcEdit0, InfoSvcEdit1, InfoSvcEdit2, 
  					InfoSvcEdit3, InfoSvcEdit4, InfoSvcEdit5;
  
  // Active sub-address bit fields
  DWORD				dwSubAddr0, dwSubAddr1, dwSubAddr2, 
  					dwSubAddr3, dwSubAddr4, dwSubAddr5;
  
  // reflex 50 support check	
  BOOL				bR50Enable;
  DWORD				dwOriginalPersAddr1;
  DWORD				dwOriginalEsn;
  
  // Form Data
  public:
	//{{AFX_DATA(CAddressInfo)
	enum { IDD = IDD_ADDRESSINFO_FORM };
	DWORD	m_dwElectronicSerialNum;
	DWORD	m_dwPersonalAddress2;
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
	//}}AFX_DATA

  // Operations
  public:
	CAddressInfo();			
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
    void ConstructInfoSvcPrintString(CString &rStr, BOOL bInfoSvcDisable, 
    								 DWORD dwInfoSvcAddr, DWORD dwSubAddr);
    BOOL  IsMtelEngUseOnly(DWORD dwPersAddr);
    
  protected:
	virtual ~CAddressInfo();
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    virtual void OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint);
	// Generated message map functions
	//{{AFX_MSG(CAddressInfo)
	afx_msg void OnAcceptDefaultsBtn();
	afx_msg void OnUndoBtn();
	afx_msg void OnSubaddrBtn0();
	afx_msg void OnSubaddrBtn1();
	afx_msg void OnSubaddrBtn2();
	afx_msg void OnSubaddrBtn3();
	afx_msg void OnSubaddrBtn4();
	afx_msg void OnSubaddrBtn5();
	afx_msg void OnKillFocusPerAddr1Edit();
	//}}AFX_MSG
	afx_msg void OnInfoSvcDisableChk();
	DECLARE_MESSAGE_MAP()
};


#endif   // _ADDRESS_

/////////////////////////////////////////////////////////////////////////////
