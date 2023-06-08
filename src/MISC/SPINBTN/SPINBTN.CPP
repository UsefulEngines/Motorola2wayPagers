//*************************************************************
//  History:    Date       Author               Comment
//              1996       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
// spinbtn.cpp : implementation file
//

#include "stdafx.h"
#include "spinbtn.h"
#include "..\..\pps.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CSpinBtn

BEGIN_MESSAGE_MAP (CSpinBtn, CBitmapButton)
  //{{AFX_MSG_MAP(CSpinBtn)
  //}}AFX_MSG_MAP
  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONUP()
  ON_WM_TIMER()
  ON_WM_KEYDOWN()
  ON_WM_MOUSEMOVE()
END_MESSAGE_MAP ()




IMPLEMENT_DYNAMIC(CSpinBtn, CBitmapButton);


BOOL CSpinBtn::AutoLoad(UINT nID, CWnd *pParent)
{ 
  m_nID = nID; 
  m_nAcceleration = 0;                
  
  return(CBitmapButton::AutoLoad(nID, pParent));
}



afx_msg void CSpinBtn::OnLButtonDown(UINT nFlags, CPoint point)
{
  TRACE("\nCSpinBtn::OnLButtonDown\n");
  
  m_bBtnStillDown = TRUE;
  m_nAcceleration = 1;
  if (!SetTimer(SPIN_TIMER_ID, SPIN_TIME_150, NULL))
    m_bBtnStillDown = FALSE;  // failed to acquire timer
  
  // send the start spin message
  GetOwner()->PostMessage(WM_SPIN_START, (WPARAM)m_nID, 0L);
   
  // send the first message
  GetOwner()->PostMessage(WM_SPIN_BUTTON, (WPARAM)m_nID, (LPARAM)m_nAcceleration);
  
  CBitmapButton::OnLButtonDown(nFlags, point);
}


afx_msg void CSpinBtn::OnLButtonUp(UINT nFlags, CPoint point)
{
  TRACE("\nCSpinBtn::OnLButtonUp\n");
  
  // stop scrolling upon mouse button up.
  if (m_bBtnStillDown == TRUE)
    KillTimer(SPIN_TIMER_ID);  
  m_bBtnStillDown == FALSE; 

  // send the stop spin message
  GetOwner()->PostMessage(WM_SPIN_STOP, (WPARAM)m_nID, 0L);

  CBitmapButton::OnLButtonUp(nFlags, point);
}



void CSpinBtn::OnTimer(UINT nIDEvent)
{   
  //TRACE("\nCSpinBtn::OnTimer\n");
   
  // if the button has lost user focus for any reason, stop scrolling 
  if (GetFocus() != this)
  {
    KillTimer(SPIN_TIMER_ID);
    m_bBtnStillDown = FALSE;
    // send the stop spin message
    GetOwner()->PostMessage(WM_SPIN_STOP, (WPARAM)m_nID, 0L);
  }
  
  // Use the m_nAcceleration variable to accelerate scrolling over longer ranges.

  if (m_bBtnStillDown == TRUE) 
  {
    if ((m_nAcceleration - 10) > 0)   // user holding button down longer than 20 scrolls
      GetOwner()->PostMessage(WM_SPIN_BUTTON, (WPARAM)m_nID, (LPARAM)m_nAcceleration); 
    else
      GetOwner()->PostMessage(WM_SPIN_BUTTON, (WPARAM)m_nID, 1L); 
    m_nAcceleration++;
  }
        
  CWnd::OnTimer(nIDEvent);
}

 
afx_msg void CSpinBtn::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  TRACE("\nCSpinBtn::OnKeyDown\n");
  
  // the space-bar must act as a mouse button press
  if (nChar == VK_SPACE) 
  {
     // send the start spin message
     GetOwner()->PostMessage(WM_SPIN_START, (WPARAM)m_nID, 0L);
     // send the spin message
     GetOwner()->PostMessage(WM_SPIN_BUTTON, (WPARAM)m_nID, 1L); 
     // send the stop spin message
     GetOwner()->PostMessage(WM_SPIN_STOP, (WPARAM)m_nID, 0L);
  }
  
  CBitmapButton::OnKeyDown(nChar, nRepCnt, nFlags);
  return;
}


afx_msg void CSpinBtn::OnMouseMove(UINT nFlags, CPoint point)
{
  CRect	client;
  
  GetClientRect((LPRECT)client);
  if (client.PtInRect(point) == FALSE)
  {
    KillTimer(SPIN_TIMER_ID);
    m_bBtnStillDown = FALSE;
    // send the stop spin message
    GetOwner()->PostMessage(WM_SPIN_STOP, (WPARAM)m_nID, 0L);
  } 
  
  CBitmapButton::OnMouseMove(nFlags, point);
  return;
}
 


void CSpinBtn::EnableSpinGroup(CWnd *pParent, UINT nStaticId, UINT nEditId, 
							   CSpinBtn &rSpinUp, CSpinBtn &rSpinDn, BOOL bEnable)
{
  CEdit			*pEdit = NULL;
  CStatic		*pStatic = NULL;
  
  if (nStaticId)  // pass a zero if no static is to be disabled
  {
    pStatic = (CStatic *)pParent->GetDlgItem(nStaticId);
    ASSERT(pStatic != NULL);
    pStatic->EnableWindow(bEnable);
  }
  pEdit = (CEdit *)pParent->GetDlgItem(nEditId);
  ASSERT(pEdit != NULL);
  pEdit->EnableWindow(bEnable);
  rSpinUp.EnableWindow(bEnable);
  rSpinDn.EnableWindow(bEnable); 
  return;
}



void CSpinBtn::EnableSpinGroup(CWnd *pParent, UINT nStaticId, CEdit &rEdit, 
							   CSpinBtn &rSpinUp, CSpinBtn &rSpinDn, BOOL bEnable)
{
  CEdit			*pEdit = NULL;
  CStatic		*pStatic = NULL;
  
  if (nStaticId)  // pass a zero if no static is to be disabled
  {
    pStatic = (CStatic *)pParent->GetDlgItem(nStaticId);
    ASSERT(pStatic != NULL);
    pStatic->EnableWindow(bEnable);
  }
  rEdit.EnableWindow(bEnable);
  rSpinUp.EnableWindow(bEnable);
  rSpinDn.EnableWindow(bEnable); 
  return;
}


