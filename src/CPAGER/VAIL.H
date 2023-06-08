//*************************************************************
//  File name: vail.h
//
//  Description:  
//		Declaration file for the CVail class.
//
//
//  Comments:
//      The CVail class inherits the CPegasus
//		class functionality an provides specific
//		support for the Vail pager.
//		
//      
//      
//
//  History:    Date       Author               Comment
//
// Written by Motorola AMSD, PMU Engineering.
// Legacy code for reference only.
//*************************************************************
 
 
#ifndef _CVAIL_
#define _CVAIL_ 

#include "pegasus.h"

 


class CVail : public CPegasus
{
  DECLARE_DYNAMIC(CVail) 

  protected:  // pager data elements
    enum { VAIL=0x84 };

  public:   
    // constructor
    CVail() : CPegasus()
    {
      m_byPagerId = VAIL; 
    }
    // copy constructor
    CVail(CVail &rCVail) : CPegasus((CPegasus &)rCVail)
    {
      m_byPagerId = rCVail.m_byPagerId; 
    }
    // destructor
    ~CVail() { DisConnect(); }
    
    // base class overrides 
  public:
    virtual void	GetPagerIdBlockInfo(CString& rString);
    virtual void	GetProductName(CString& rString);

  private:           
	virtual int     HandleIsHandshakeAcked(void); // override of CPegasus function

  public:  // MFC debug routines
    #ifdef _DEBUG
    void 	AssertValid() const;
    void 	Dump(CDumpContext& dc) const;
    #endif

  // IoOperation callback interface - see file 'ciooper.h'
  class IoCallBackInterface : public ICallBack
  {
    virtual int	OnIoOperationCallBack(int nCallBackId); 
    virtual int	OnIoOperationComplete(UINT nStatus, UINT nIoOpId); 
  } m_IoCallBackInterface;

  friend class IoCallBackInterface;  
};   // CVail





#endif // _CVAIL_
