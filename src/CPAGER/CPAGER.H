//*************************************************************
//  File name: CPAGER.H
//
//  Description:  
//      Declaration file for the CPager class.
//
//
//  Comments:
//      The CPager class implements an abstract pager object
//		intended for use as an encapsulation of the
//      pager/application interface protocol.
//
//      
//
//  History:    Date       Author               Comment
//              3/13/96    P. Pennington        Created
//
// Written by Motorola AMSD, PMU Engineering.
// Legacy code for reference only.
//*************************************************************
 
 
#ifndef _CPAGER_
#define _CPAGER_

#include "ciooper.h"



// PAGER STATES
#define NO_STATE					0x0000
#define READ_REQUESTED				0x0001
#define WRITE_REQUESTED				0x0002 
#define TEST_MODE					0x0004
#define TEST_MODE_REQUESTED			0x0008




/////////////////////////////////////////////////////////////////////
//  CPager
// 
//  This class is defined as an abstract base class from which 
//  pager model-specific derived classes implement the protocol
//  interface.  Note that the pure virtual functions herein must
//  be redefined in the child class.
//
class CPager : public CObject
{   
  DECLARE_DYNAMIC(CPager)

  protected:  // pager data elements 
    BYTE			m_byPagerId;
    UINT			m_nCurrentState;        // see flags above
    BYTE			*m_pbyPagerCodePlug;
	  
  protected:  // IoOperation progress indicator elements 
    float 			m_fTotalIoOpTime;  // in seconds
    float			m_fCompletedIoOpTime; 
    CString			m_szCurrentOpString;
  
  protected:  // error message elements
    CString			m_szDisplayOnFailure;
    
  public:   
    CPager()
    { 
      m_nCurrentState = NO_STATE;
      m_pbyPagerCodePlug = NULL;
    } 
    // copy constructor
    CPager(CPager &rCPager)
    {
      m_byPagerId = rCPager.m_byPagerId;
      m_nCurrentState = rCPager.m_nCurrentState;
      m_pbyPagerCodePlug = rCPager.m_pbyPagerCodePlug;
      m_fTotalIoOpTime = rCPager.m_fTotalIoOpTime;
      m_fCompletedIoOpTime = rCPager.m_fCompletedIoOpTime;
      m_szCurrentOpString = rCPager.m_szCurrentOpString;
      m_szDisplayOnFailure = rCPager.m_szDisplayOnFailure; 
    }
    // destructor
    ~CPager() { DisConnect(); }
                       
    // member functions
    int		InitiateRead(void);
    BOOL 	IsOperationInProgress(void); 
    BOOL	IsReadRequested()
    		{ return((m_nCurrentState & READ_REQUESTED) > 0); }
    BOOL	IsWriteRequested()
    		{ return((m_nCurrentState & WRITE_REQUESTED) > 0); }
    BOOL	IsTestModeRequested()
    		{ return((m_nCurrentState & TEST_MODE_REQUESTED) > 0); }
    BOOL	IsPagerInTestMode()
    		{ return((m_nCurrentState & TEST_MODE) > 0); }
    int 	ConfigurePort();
    int 	DisConnect(void); 
    const char  *GetCurrentOpString(void)
    	    { return((LPCTSTR)m_szCurrentOpString); }
    int 	GetPercentComplete(void);
    
    // overridables and pure functions.
    virtual int 	OnOpCompletion(UINT nStatus, UINT nIoOpId); 
    virtual int 	ReadCodePlug(void) = 0;
    virtual int 	WriteCodePlug(const CByteArray& rbyBuffer) = 0;
    virtual int		WriteCodePlug(int nIndex, int nNumBytes) = 0;
	virtual void 	EncodePassword(const char pwd[], char epwd[]) = 0;
	virtual void 	DecodePassword(const char epwd[], char pwd[]) = 0;
	virtual UINT	GetCodePlugBaseAddress(void) = 0; 
	virtual int		GetCodePlugSize(void) = 0;
    virtual int		GetPagerMajorReleaseNumber(BYTE *pDest) = 0;
    virtual int		GetPagerMinorReleaseNumber(BYTE *pDest) = 0; 
    virtual int		DoSoftwareReset(BYTE byWhen = 1) = 0;
    virtual int		DoHardwareReset(BYTE byWhen = 1) = 0;    
    virtual int		SetPagerPassword(const char *pSource) = 0;
    virtual int		SetPagerCodePlug(const CByteArray& rSource) = 0;        		  
    virtual int		GetPagerCodePlug(CByteArray& rDest) = 0;
    virtual void	GetPagerIdBlockInfo(CString& rString) = 0;
    virtual void	GetProductName(CString& rString) = 0;
    virtual BOOL	IsCodePlugUnprogrammed(void) = 0;
    
  protected:
    virtual int		ExecuteIoOpList(int nTimeInSecs, const char *pszIoOp);
    virtual void 	OnOpFailure(const char *pszFailure);
  
  private:
	int     HandleIsHandshakeAcked(void);
    
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
};

  

  
#endif  // _CPAGER_

