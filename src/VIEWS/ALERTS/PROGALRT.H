//*************************************************************
//  History:    Date       Author               Comment
//              1996       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
// progalrt.h : header file
//

 
#ifndef _PROGALRT_
#define _PROGALRT_ 
 
#include "..\..\misc\bsound.h"                                  


#define ALERT_TIMER_ID 		  2012
#define ALERT_TIMER_TIME       100



 
/////////////////////////////////////////////////////////////////////////////
// CProgAlertDlg dialog

class CProgAlertDlg : public CDialog
{
  private: 
    // NOTE:  MAX_SCROLL is set to the maximum index specified in the
    // table of valid frequencies listed in the Pegasus Codeplug
    // Description Document (field CP_Programmable_Pleasing_Alert).
    // Currently, it appears that only indices 0x00-0x0C work on the
    // Centaur unit.  
    enum { MIN_SCROLL=0, MAX_SCROLL=0x0C, NUM_BEATS=16 }; 
    
    CBasicSound         sndTest[NUM_BEATS];    	// array of CBasicSound objects 
    CBasicSound			sndScroll;				// CBasicSound object, play during scroll
    BYTE				*pbyAlert;              // pointer to 16 bytes of freq indices
    BOOL		        m_bAlertTimerActive;
  
  // Construction
  public:
	CProgAlertDlg(CWnd* pParent = NULL);	// standard constructor

  // Dialog Data
	//{{AFX_DATA(CProgAlertDlg)
	enum { IDD = IDD_PROG_ALERT_DLG };
	//}}AFX_DATA

    virtual int DoModal(void)
    { 
      ASSERT(0);    // don't use this override, use the following one.
      return(CDialog::DoModal());
    }
    
    virtual int DoModal(BYTE *pbyProgAlert)
    {
      ASSERT(pbyProgAlert != NULL);
      pbyAlert = pbyProgAlert;   
      return(CDialog::DoModal());
    }
  
  private:
	void StopAlertTest();
  
  // Implementation
  protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CProgAlertDlg)
	virtual void OnOK(); 
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSaveBtn();
	afx_msg void OnTestBtn();
	afx_msg void OnProgalertHelpBtn();
	//}}AFX_MSG
    afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()
};


#endif   // _PROGALRT_