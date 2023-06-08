//*************************************************************
//  History:    Date       Author               Comment
//              1996       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//

////////////////////////////////////////////////////////////////////////
//  SPINBTN.H
//
//  This class implements a 'SpinButton' type of custom control.  The
//  intent is to provide a miniature scroll bar mechanism for rapid 
//  updates of corresponding edit boxes.  The class is derived from
//  CBitmapButton in order for the control to be drawn via the owner-draw
//  mechanism.  Essentially, this class intercepts the OnLButtonDown
//  WM message and starts a timer.  On each timer tick, a WM_SPIN_BUTTON
//  message is sent to the parent window in order for the window to
//  update the corresponding edit box contents.  When the user releases
//  the left mouse button, the OnLButtonUp WM message is intercepted.
//  The timer is then killed and the WM_SPIN_BUTTON messages cease.
//
//  In order for the CSpinBtn class to be used, the window must be
//  coded to handle WM_SPIN_BUTTON messages.  The wParam paramater 
//  identifies the specific CSpinBtn by resource ID.  Note the 
//  override of CBitmapButton::AutoLoad().
//
//  See files 'protocol.h' and 'protocol.cpp' for an example implementation.
//


#ifndef _SPINBTN_
#define _SPINBTN_

#define SPIN_TIMER_ID 		  2011   
#define SPIN_TIME_150          150  // milliseconds

#define WM_SPIN_BUTTON			(WM_USER+9)
#define WM_SPIN_START			(WM_USER+12) 
#define WM_SPIN_STOP			(WM_USER+13) 


class CSpinBtn : public CBitmapButton
{
  DECLARE_DYNAMIC(CSpinBtn);
  
    BOOL	m_bBtnStillDown;
    int		m_nID;
    int		m_nAcceleration;
    
  public:
    CSpinBtn() : CBitmapButton() 
    { 
      m_bBtnStillDown = FALSE; 
    }
    virtual BOOL AutoLoad(UINT nID, CWnd *pParent);
    static void EnableSpinGroup(CWnd *pParent, UINT nStaticId, UINT nEditId, 
                                CSpinBtn &rSpinUp, CSpinBtn &rSpinDn, 
                                BOOL bEnable);
    static void EnableSpinGroup(CWnd *pParent, UINT nStaticId, CEdit &rEdit, 
                                CSpinBtn &rSpinUp, CSpinBtn &rSpinDn, 
                                BOOL bEnable);
    
  protected: 
    //{{AFX_MSG(CSpinBtn)
    //}}AFX_MSG
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point); 
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  DECLARE_MESSAGE_MAP()
};

 
 
#endif   // _SPINBTN_