//*************************************************************
//  History:    Date       Author               Comment
//              1996       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//
// numedit.cpp : implementation file
//

#include "stdafx.h"
#include "numedit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CNumericEdit
BEGIN_MESSAGE_MAP (CNumericEdit, CEdit)
    ON_WM_CHAR ()
END_MESSAGE_MAP ()

void CNumericEdit::OnChar (UINT nChar, UINT nRepCnt, UINT nFlags)
{ 
    TRACE("\nCNumericEdit::OnChar\n");
    
    if (((nChar >= '0') && (nChar <= '9')) || 
        ((nChar >= 'a') && (nChar <= 'f')) ||
        ((nChar >= 'A') && (nChar <= 'F')) ||
         (nChar == '.') ||
         (nChar == VK_BACK))
    { 
      CEdit::OnChar (nChar, nRepCnt, nFlags);
    }
}

 
BYTE CNumericEdit::byHexAlpha2Binary(const char ch)
{
   if ((ch >= '0') && (ch <= '9'))   
     return((BYTE)(ch - 0x30));
   else if ((ch >= 'A') && (ch <= 'F'))
     return((BYTE)(ch - 0x37));
   else if ((ch >= 'a') && (ch <= 'f'))
     return((BYTE)(ch - 0x57)); 
   return (0x00);
} 


static char szBuff[4];
//
void CNumericEdit::DDX_HexText(CDataExchange *pDX, BYTE *pbyVariable)
{
  // if FALSE move data to window elements from member variable
  if (pDX->m_bSaveAndValidate == FALSE)
  {
    sprintf(szBuff, "%02X", *pbyVariable);
    SetWindowText(szBuff); 
  }
  else  // TRUE, move data from window element to member variable
  {  
    if (GetModify())
    {
      GetWindowText(szBuff, 3); 
  	  if (szBuff[0] != '\0')
        *pbyVariable = byHexAlpha2Binary(szBuff[0]);
  	  if (szBuff[1] != '\0')
        *pbyVariable = (BYTE)((*pbyVariable << 4) | (byHexAlpha2Binary(szBuff[1])));
    }
  }
  return;
}
  




/////////////////////////////////////////////////////////////////////////////
//  CDecimalEdit - member functions 

BEGIN_MESSAGE_MAP(CDecimalEdit, CEdit)
    ON_WM_CHAR()
END_MESSAGE_MAP()


// This class may be used to subclass a CEdit to work with
// only positive decimal integer or float digits.
void CDecimalEdit::OnChar (UINT nChar, UINT nRepCnt, UINT nFlags)
{ 
    TRACE("\nCDecimalEdit::OnChar\n");
    
    if (((nChar >= '0') && (nChar <= '9')) || 
         (nChar == '.') ||
         (nChar == VK_BACK))
    { 
      CEdit::OnChar(nChar, nRepCnt, nFlags);
    }
    return;
}


