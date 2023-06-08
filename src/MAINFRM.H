//*************************************************************
//  History:    Date       Author               Comment
//              1996       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
/////////////////////////////////////////////////////////////////////////////
// mainfrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

class CMainFrame : public CFrameWnd
{ 
  protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

  // Implementation
  public:
	virtual ~CMainFrame();
  #ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
  #endif

  protected:
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CToolBar    m_wndTuningToolBar; 
	enum { TBSHORT=0x01, TBLONG=0x02, TBTUNING=0x04, TBLABONLY=0x08 };  // toolbars visible
	BYTE		m_byVisibleToolBars;
	BOOL		m_bEnableTuning;
	BOOL		m_bEnableHexEdit;
    int			nReadBmpIndex, nWriteBmpIndex;  // dynamic for effect
    
  public:             
	void StatusBarMsg(const char *pszMsg)
	  { m_wndStatusBar.SetPaneText(0, pszMsg, TRUE); }
	void ViewToolBarShort(void);
	void ViewToolBarLong(void);
    void ViewToolBarLabUseOnly(void); 
    void RefreshToolBar(void);
    void ToggleReadWriteBmps(BOOL bNormal = TRUE);
	BOOL IsAccessRestricted(void) { return(m_bEnableHexEdit == FALSE); }	
    virtual BOOL PreCreateWindow(CREATESTRUCT &cs);
	void RefreshViewMenu(void);
	void ToggleStatusIndicators(void);
    
  private:
    void OnPagerTuning(BYTE byTestId);
    void OnUpdatePagerTuning(CCmdUI* pCmdUI);
    void GetMenuStringFromStringTable(UINT nCommandId, CString &rStr);
    CMenu *ClearTheViewMenu(void);

  // Generated message map functions
  protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPagerPasswd();
	afx_msg void OnUpdatePagerPasswd(CCmdUI* pCmdUI);
	afx_msg void OnTxtuneModbalHigh();
	afx_msg void OnUpdateTxtuneModbalHigh(CCmdUI* pCmdUI);
	afx_msg void OnTxtuneModbalLow();
	afx_msg void OnUpdateTxtuneModbalLow(CCmdUI* pCmdUI);
	afx_msg void OnTxtuneScf();
	afx_msg void OnUpdateTxtuneScf(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	afx_msg void OnRxtuneRefosc();
	afx_msg void OnUpdateRxtuneRefosc(CCmdUI* pCmdUI);
	afx_msg void OnPagerSwReset();
	afx_msg void OnUpdatePagerSwReset(CCmdUI* pCmdUI);
	afx_msg void OnPagerHwReset();
	afx_msg void OnUpdatePagerHwReset(CCmdUI* pCmdUI);
	afx_msg void OnTxtuneTxonTest();
	afx_msg void OnUpdateTxtuneTxonTest(CCmdUI* pCmdUI);
	afx_msg void OnViewBanner();
	afx_msg void OnUpdateViewBanner(CCmdUI* pCmdUI);
	afx_msg void OnRxtuneRcvronTest();
	afx_msg void OnUpdateRxtuneRcvronTest(CCmdUI* pCmdUI);
	afx_msg void OnComdrvCnreceive();
	afx_msg void OnTxTuneSymbolDev();
	afx_msg void OnUpdateTxTuneSymbolDev(CCmdUI* pCmdUI);
	afx_msg void OnEnterIdle(UINT nWhy, CWnd* pWho);
	afx_msg void OnDeviceTuning();
	afx_msg void OnUpdateDeviceTuning(CCmdUI* pCmdUI);
	afx_msg void OnViewNext();
	afx_msg void OnViewPrevious();
	afx_msg void OnDeviceComsettings();
	afx_msg void OnUpdateDeviceComsettings(CCmdUI* pCmdUI);
	//}}AFX_MSG
	// the following command msg handlers inserted by hand.
	afx_msg LONG OnCommNotify(WPARAM, LPARAM);
	afx_msg LONG OnWmIoOperation(WPARAM, LPARAM);
	afx_msg LONG OnWmReadCodeplugDone(WPARAM, LPARAM);
	afx_msg LONG OnWmWriteCodeplugDone(WPARAM, LPARAM);
	afx_msg LONG OnWmIoOpListDone(WPARAM, LPARAM);
	afx_msg void OnUpdateIndicatorPort(CCmdUI *pCmdUI);
	afx_msg void OnUpdateIndicatorIoOp(CCmdUI *pCmdUI);
	afx_msg void OnUpdateIndicatorProgress(CCmdUI *pCmdUI);
	// dynamic view sub-menu command msg handlers.  see note in .cpp file.
	afx_msg void OnViewPagerid();
	afx_msg void OnUpdateViewPagerid(CCmdUI* pCmdUI);
	afx_msg void OnViewSecurity();
	afx_msg void OnUpdateViewSecurity(CCmdUI* pCmdUI);
	afx_msg void OnViewAddresses();
	afx_msg void OnUpdateViewAddresses(CCmdUI* pCmdUI);
	afx_msg void OnViewProtocol();
	afx_msg void OnUpdateViewProtocol(CCmdUI* pCmdUI);
	afx_msg void OnViewMessages();
	afx_msg void OnUpdateViewMessages(CCmdUI* pCmdUI);
	afx_msg void OnViewAlerts();
	afx_msg void OnUpdateViewAlerts(CCmdUI* pCmdUI);
	afx_msg void OnViewOptions();
	afx_msg void OnUpdateViewOptions(CCmdUI* pCmdUI);
	afx_msg void OnViewHexedit();
	afx_msg void OnUpdateViewHexedit(CCmdUI* pCmdUI);
	afx_msg void OnViewPclink();
	afx_msg void OnUpdateViewPclink(CCmdUI* pCmdUI);
	afx_msg void OnViewRegistration();
	afx_msg void OnUpdateViewRegistration(CCmdUI* pCmdUI);
	afx_msg void OnViewCannedmsgs();
	afx_msg void OnUpdateViewCannedmsgs(CCmdUI* pCmdUI);
	afx_msg void OnViewCommview();
	afx_msg void OnUpdateViewCommview(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
};



/////////////////////////////////////////////////////////////////////////////
