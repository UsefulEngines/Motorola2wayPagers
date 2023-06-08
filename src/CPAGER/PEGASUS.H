//*************************************************************
//  File name: PEGASUS.H
//
//  Description:  
//		Declaration file for the CPegasus class.
//
//
//  Comments:
//      The CPegasus class inherits the base CPager
//		class functionality an provides specific
//		support for the Pegasus pager (aka Tango Pro).
//		
//      
//      
//
//  History:    Date       Author               Comment
//              3/13/96    P. Pennington        Created
//
// Written by Motorola AMSD, PMU Engineering.
// Legacy code for reference only.
//*************************************************************
 
 
#ifndef _CPEGASUS_
#define _CPEGASUS_ 

#include "cpager.h"

 
/////////////////////////////////////////////////////////////
//  DSP/ASIC Registers
//
//  The following structs detail the layout of the Dsp/Asic
//  Registers which are used to tune the device.
// 
typedef struct 
{
  UINT	nTestMode;
  UINT 	nTestFreq;
  UINT 	nTestData;
} CTestModeControlRegister;

typedef struct
{               
  BYTE	byL1;	// -2400 symbol  CP_SymbolTxLevel[0]
  BYTE	byL2;	// -800 symbol   CP_SymbolTxLevel[1]
  BYTE	byL3;	// +2400 symbol  CP_SymbolTxLevel[2]
  BYTE	byL4;	// +800 symbol   CP_SymbolTxLevel[3]
  BYTE	byBat;	// CP_Balance_Tune
  BYTE	byL0;	// CP_Tx_Idle_Symbol
} CTransmitRegister;

typedef struct
{
  BYTE	byAt1;	// was used for flap antenna tuning
  BYTE	byAt2;	// ""
  BYTE	byAt3;	// ""
  BYTE	byRosc;	// CP_AFC_Make
} CReceiveRegister;

typedef struct
{
  BYTE	byTu1;  // CP_TxWarmupTimes
  BYTE	byTu2;
  BYTE	byTu3;
  BYTE	byTu4;
  BYTE	byTu5;
  BYTE	byTu6;
  BYTE	byTu7;
  BYTE	byTu8;
} CTransmitWarmupRegister;
 



// NOTE:  12/02/96 - P. Pennington
// This class is the base class for the Pegasus family of
// pagers (including the Pegasus-Alpha, Centaur, and Aspen
// units).  Since there is virtually no difference between
// these devices to date (except for device_id), it seems
// appropriate not to bother with inheriting off of this
// class in order to implement the Centaur and Aspen.
// This condition may change at any time in the future.
// If so, the CPager derived object instantiated in file
// 'pps.cpp' may need to be dynamically instantiated 
// based upon the device id.   


class CPegasus : public CPager
{
  DECLARE_DYNAMIC(CPegasus) 

  protected:  // pager data elements
    enum { NOW=0, LATER=1 };   
    enum { BASE_ADDRESS=0x0D00, CP_SIZE=768 };
    enum { PEGASUS_ALPHA=0x50, CENTAUR=0x80, PEGASUS_ACK=0x81, 
           PAGEFINDER=0x82, ASPEN=0x83, EXPANDAUR=0x85,
           LYNX=0x86 };
    BYTE 			*m_pbyPagerIdBlock;
    BYTE			*m_pbyPagerSerialNum;
    BYTE			*m_pbyPagerICN;
    BYTE 			*m_pbyPagerStatus;
	BYTE			*m_pbyPagerPassword;
	BYTE			*m_pbyPagerDownloadControl; 
  
  public:   
    // constructor
    CPegasus() : CPager()
    {
      m_byPagerId = PEGASUS_ALPHA; 
      m_pbyPagerIdBlock = NULL;
      m_pbyPagerSerialNum = NULL;
      m_pbyPagerICN = NULL;
      m_pbyPagerStatus = NULL;
      m_pbyPagerPassword = NULL;
      m_pbyPagerDownloadControl = NULL;
    }
    // copy constructor
    CPegasus(CPegasus &rCPegasus) : CPager((CPager &)rCPegasus)
    {
      m_pbyPagerIdBlock = rCPegasus.m_pbyPagerIdBlock;
      m_pbyPagerSerialNum = rCPegasus.m_pbyPagerSerialNum;
      m_pbyPagerICN = rCPegasus.m_pbyPagerICN;
      m_pbyPagerStatus = rCPegasus.m_pbyPagerStatus;
      m_pbyPagerPassword = rCPegasus.m_pbyPagerPassword;
      m_pbyPagerDownloadControl = rCPegasus.m_pbyPagerDownloadControl;
    }
    // destructor
    ~CPegasus() { DisConnect(); }
    
    // base class overrides 
    virtual int 	ReadCodePlug(void);
    virtual int 	WriteCodePlug(const CByteArray& rbyBuffer);
    virtual int		WriteCodePlug(int nIndex, int nNumBytes);
	virtual UINT	GetCodePlugBaseAddress(void) { return(BASE_ADDRESS); } 
	virtual int		GetCodePlugSize(void)  { return(CP_SIZE); }
	virtual void 	EncodePassword(const char pwd[], char epwd[]);
	virtual void 	DecodePassword(const char epwd[], char pwd[]);
    virtual int		DoSoftwareReset(BYTE byWhen = LATER);
    virtual int		DoHardwareReset(BYTE byWhen = LATER);
    virtual void	GetPagerIdBlockInfo(CString& rString);
    virtual void	GetProductName(CString& rString);
    virtual int		GetPagerMajorReleaseNumber(BYTE *pDest)
    				{ return(RetrieveFromPagerIdBlock(pDest, 11, 2)); }
    virtual int		GetPagerMinorReleaseNumber(BYTE *pDest)
    				{ return(RetrieveFromPagerIdBlock(pDest, 13, 2)); }
    virtual BOOL	IsCodePlugUnprogrammed();
    virtual int		GetPagerCodePlug(CByteArray& rDest);
    virtual int		SetPagerCodePlug(const CByteArray& rSource);       		  
    virtual int		SetPagerPassword(const char *pSource);
	
	// 	CPegasus specific test mode member functions.    
	int		EnterTestMode(void);
	int		ExitTestMode(BYTE byExitTestCmd);
	int		CancelPreviousTest(BYTE byCancelPrevTestCmd); 
	int		DoTuningTest(BYTE byTestId, BYTE byDspAsicDownloadMethod,
	                     CTestModeControlRegister &rTestModeControlRegister,
	                     CTransmitRegister &rTransmitRegister,
	                     CReceiveRegister &rReceiveRegister,
	                     CTransmitWarmupRegister &rTransmitWarmupRegister,
	                     BYTE byInsertCancelTest=0, int nDelayTicks=5, 
	                     BOOL bIgnoreTxWarmupRegister = TRUE);
	int     DoTuningUpdate(CTransmitRegister &rTransmitRegister,
	                       CReceiveRegister &rReceiveRegister);                     
    int		DoTestUseDefaultData(BYTE byTestId);
    //   CPegasus data access functions:
    // 	 Returns 1 on error or 0 on success.  Data is
    //	 copied to pDest without appending '\0'.  if
    //	 data has not been uploaded from pager nothing is
    //   copied to pDest and 1 or FALSE (if return type BOOL) is returned.  
    int		GetPagerID(BYTE *pDest)
    		{ return(RetrieveFromPagerIdBlock(pDest, 0, 1)); }
    int		GetPagerType(BYTE *pDest)
    		{ return(RetrieveFromPagerIdBlock(pDest, 1, 1)); }
    int		GetPagerAPC(BYTE *pDest)
    		{ return(RetrieveFromPagerIdBlock(pDest, 2, 3)); }
    int		GetPagerDerivative(BYTE *pDest)
    		{ return(RetrieveFromPagerIdBlock(pDest, 5, 3)); }
    int		GetPagerReleaseType(BYTE *pDest)
    		{ return(RetrieveFromPagerIdBlock(pDest, 9, 2)); }
    int		GetPagerSerialNumber(BYTE *pDest);
    int		GetPagerICN(BYTE *pDest);
    int		GetPagerStatus(BYTE *pDest);
    BOOL	IsPagerDownloadDisabled();
    BOOL	IsPagerPasswordRequired();
    BOOL	IsPagerOtaDisabled();
    int		GetPagerPassword(char *pDest);
    int		GetPagerDownloadControlByte(BYTE *pDest);

  private:           
    int		RetrieveFromPagerIdBlock(BYTE *pDest, int nOffset, int nSize);

    // OnOpCallback handlers
	virtual int  HandleIsHandshakeAcked(void);
    int		HandleIsPasswordRequired(void);
    int		HandleIsPasswordAccepted(void);
    int		HandleIsCheckSumAcked(void);
    int		HandleIsCodePlugWriteAcked(void); 
    int		HandleIsTestModeAcked(void);
    int		HandleIsTestRegisterDataAcked(void);		
	int     HandleIsCodePlugUnprogrammed(void);
	int		HandleIsCodePlugVersionValid(void);

    // the following adopted from Tango 
	void 	padRSpace(char s[], int length);
	int 	strloc(char s[], char c);

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
};   // CPegasus





#endif // _CPEGASUS_
