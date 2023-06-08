//*************************************************************
//  History:    Date       Author               Comment
//              1996       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
// pagerid.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPagerId form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#ifndef _PAGERID_
#define _PAGERID_

class CPagerId : public CPpsFormView
{
  DECLARE_DYNCREATE(CPagerId) 

  BOOL		bReflex50Enabled;	
	
  // Form Data
  public:
	//{{AFX_DATA(CPagerId)
	enum { IDD = IDD_PAGERID_FORM };
	BYTE	m_byCodeplugVersion;
	BYTE	m_byOtaRevision;
	CString	m_szPowerUpPrompt;
	CString	m_szSerialNum;
	CString	m_szIdBlockInfo;
	CString	m_szInventoryControlNum;
	BYTE	m_byDspPatchLevel;
	BYTE	m_byHostPatchLevel;
	CString	m_szCapCode;
	//}}AFX_DATA

  // Operations
  public:
	CPagerId();			
    virtual void OnInitialUpdate();
  	virtual void SetFromDoc();
  	virtual BOOL SetToDoc();	
	virtual void OnDefaults(void) { OnAcceptDefaultsBtn(); }
	virtual void OnUndo(void) { OnUndoBtn(); }
	virtual void OnArchived(void);
  	virtual int OnPrintHelper(CStringArray &rStringArray,  UINT nPageNumber, int nAvailableLines);
    #ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
    #endif

  // Implementation
  protected:
	virtual ~CPagerId();
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    virtual void OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint);
	// Generated message map functions
	//{{AFX_MSG(CPagerId)
	afx_msg void OnAcceptDefaultsBtn();
	afx_msg void OnUndoBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif   // _PAGERID_

/////////////////////////////////////////////////////////////////////////////
