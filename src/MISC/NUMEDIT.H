//*************************************************************
//  History:    Date       Author               Comment
//              1996       Phillip Pennington   Created
//
//  Legacy code for reference only.
//*************************************************************
//

////////////////////////////////////////////////////////////////////////
//  NUMEDIT.H
//
//  The CNumericEdit class may be used to enhance an edit control to
//  accept only numeric characters (0-9, A-F).  Example usage is
//  in file '..\views\address\address.h'.  Ensure the following
//  line appears in the CDialog overridden 'OnInitDialog()' function:
//  	m_NumericEdit.SubclassDlgItem(IDC_EDIT###, this);
// 


////////////////////////////////////////////////////////////////////////
//

#ifndef _NUMERIC_EDIT_
#define _NUMERIC_EDIT_

// This class may be used to subclass a CEdit to work with hex or
// decimal digits.
class CNumericEdit : public CEdit
{
  public:
    static BYTE byHexAlpha2Binary(const char ch);
    void DDX_HexText(CDataExchange *pDX, BYTE *pbyVariable);
    
  protected:
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    DECLARE_MESSAGE_MAP()
};


// This class may be used to subclass a CEdit to work with
// only positive decimal integer or float digits.
class CDecimalEdit : public CEdit
{
  protected:
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    DECLARE_MESSAGE_MAP()
};


    


#endif  // _NUMERIC_EDIT_