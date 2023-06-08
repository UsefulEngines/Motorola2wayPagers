
//*************************************************************
//  File name: CIOOPER.H
//
//  Description:  
//      Declaration file for the CIoOperation class.
//
//
//  Comments: 
//		A CIoOperation object encompasses a sequence of
//		basic IoCommands.  A CIoOperation object is
// 		typically constructed as an element of a CIoOperation
//  	list.  Thus a compound protocol transaction can
//		be generically defined as a sequence of simple
//		CIoOperations.  See the 'cpager.h' and 'pegasus.cpp'
//		files for a examples.
//
//
//  History:    Date       Author               Comment
//              3/96       Phillip Pennington   Created
//
// Legacy code for reference only.
//*************************************************************
 
 
#ifndef _CIOOPER_
#define _CIOOPER_

#include <stddef.h>		// for the offsetof MACRO

// any application which utilizes the CIoOperation class must
// implement support for the following user-defined message.
// See 'ciooper.cpp' for lParam, wParam definitions.
#define WM_IO_OPERATION			(WM_USER+10) 
#define WM_IO_OPLIST_DONE	    (WM_USER+11)

// IO COMMANDS - corresponds to the 'nCommand' field below
#define IO_READ					0x0001
#define IO_WRITE				0x0002
#define IO_REPEATED_WRITE		0x0004  // writes on each timer event until comm event or timeout
#define IO_CALLBACK				0x0008  // intermediate callback via the ICallBack interface
#define IO_NOOP					0x0010  // for embedded delays


// IO COMMAND STRUCTURE  
// *** Usage:  See file 'pegasus.cpp' for example IoCommand arrays.  
// General usage rules:
//   (1) The nCommand field may be one of the above IO COMMANDS.
//   (2) For IO_CALLBACK commands, the pData field is NULL and the nBytes
//       field is a user-defined callback identifier.
//   (3) For IO_NOOP commands, the pData field is NULL and the nBytes field
//       indicates the number of timer ticks to delay (0 indicates delay until
//       current CIoOperation times-out).  If a comm event occurs during a 
//       IO_NOOP command, then the command times-out immediately.
//
typedef struct IoCommand 
{
    UINT			nCommand;  // IO COMMANDS
    BYTE			*pData;    // in/out depends upon command
    int 			nBytes;    // number of data bytes 
} IoCommand;
      

// IO CALLBACK INTERFACE - any class which utilizes the CIoOperation class
// must also implement this interface as a nested class. - see 'pegasus.h'
//
struct ICallBack
{
  virtual int	OnIoOperationCallBack(int nCallBackId) = 0;           
  virtual int	OnIoOperationComplete(UINT nStatus, UINT nIoOpId) = 0; 
};



// Use this macro to construct a pointer to the class which nests an
// ICallBack derived class.  This macro is for 16-bit Windows only.
//
#define METHOD_PROLOGUE(theClass, localClass) \
  theClass* pThis = ((theClass*)((char*)(this) - offsetof(theClass, localClass)));



#define IO_COMPLETED	0
#define IO_FAILED		1 
#define IO_NOOPTIMEOUT  2     
 	


class CIoOperation : public CObject
{
  DECLARE_DYNAMIC(CIoOperation) 
  
  enum { TIMETICK=100 };	// 100 milli-seconds between timer ticks
  protected: 
    IoCommand		*pCmds;
    IoCommand		*pCurCmd;
    int				nNumCmds; 
    UINT			nIOOPid;
    int				nCountDown;
    int 			nAccumulatedRead;
    BOOL			bThisIoOpCompleted;
    int				nNumTicks;
    int				nNoOpTickCounter;
    ICallBack		*pICallBack;
    
  public: 
    // constructors
    CIoOperation(IoCommand *pCommands, int nNum, UINT IOOPid, 
                 int nTicks, ICallBack *pCallBackInterface)
    {                                 
      ASSERT(pCommands);
      ASSERT(pCallBackInterface);
      pCmds = pCommands; 
      pCurCmd = pCmds;
      nNumCmds = nNum;
      nIOOPid = IOOPid;
      nNumTicks = nTicks;
      nCountDown = 0;
      nAccumulatedRead = 0;  
      bThisIoOpCompleted = FALSE;
      pICallBack = pCallBackInterface;
    }
    CIoOperation(CIoOperation &rCIoOperation)   // copy constructor
    {
      pCmds = rCIoOperation.pCmds; 
      pCurCmd = rCIoOperation.pCurCmd;
      nNumCmds = rCIoOperation.nNumCmds;
      nIOOPid = rCIoOperation.nIOOPid;
      nNumTicks = rCIoOperation.nNumTicks;
      nCountDown = rCIoOperation.nCountDown;
      nAccumulatedRead = rCIoOperation.nAccumulatedRead;  
      bThisIoOpCompleted = rCIoOperation.bThisIoOpCompleted;
      pICallBack = rCIoOperation.pICallBack;
    }
    // destructor 
    ~CIoOperation() { if (nCountDown) EndOperation(IO_FAILED); }

    // member functions 
    int BeginOperation(void);
    int EndOperation(UINT nStatus);
    //int OnCommEvent(void); 
    UINT GetIoOpId(void)
    { 
      return(nIOOPid); 
    }
    void SkipIoCommand(int nSkip = 1)
    { 
      if ((&pCurCmd[nSkip]) < (&pCmds[nNumCmds]))   
        pCurCmd += nSkip;
    } 
    float GetTotalTimeInSeconds(void)
    { 
      return((float)(TIMETICK * nNumTicks / 1000.0F));
    }
    float GetElapsedTimeInSeconds(void)
    { 
      return((float)((TIMETICK * (nNumTicks - nCountDown)) / 1000.0F));
    } 
    const char *GetCurrentCmdName(void);
    int GetNumTicksRemaining(void)
    { 
      return(nCountDown); 
    }
    int GetCurrentCmdNumBytes(void)
    { 
      if (pCurCmd < (&pCmds[nNumCmds]))
        return(pCurCmd->nBytes);
      else
        return(0);
    }
    virtual LONG OnTimerEvent(UINT nIdEvent, BOOL bOverrideRepeat=FALSE);
    virtual LONG OnCommNotify(WPARAM wParam, LPARAM lParam) = 0;
    
  protected:
    virtual LONG OnCommEvent(void);             
    virtual UINT GetChannelId(void) = 0;                               
    virtual int DoIoWrite(void) = 0;
    virtual int DoIoRead(int *nActualRead, int nAccumulatorOffset) = 0;
    int StartTimer(void);
    int DoIoCommandLoop(void); 
 
  public:  // MFC debug routines
    #ifdef _DEBUG
    void AssertValid() const;
    void Dump(CDumpContext& dc) const;
    #endif                         
    
  friend class CIoController;
};
   


#include "..\w16comm\w16comm.h"


class CSerialIoOperation : public CIoOperation
{   
  DECLARE_DYNAMIC(CSerialIoOperation) 
  
  private:                  
    CWin16Comm	*m_pCWin16Comm;		// serial channel 
    
  public: 
    // constructors
    CSerialIoOperation(CWin16Comm *pCWin16Comm, IoCommand *pCommands, 
                       int nNumCommands, UINT IOOPid, 
                       int nTicks, ICallBack *pCallBackInterface) :
                       CIoOperation(pCommands, nNumCommands, IOOPid, 
                       nTicks, pCallBackInterface)
    {
      ASSERT(pCWin16Comm);                                
      m_pCWin16Comm = pCWin16Comm; 
    } 
    CSerialIoOperation(CSerialIoOperation &rCSerialIoOperation)
     : CIoOperation((CIoOperation &)rCSerialIoOperation)
    {
      m_pCWin16Comm = rCSerialIoOperation.m_pCWin16Comm;
    } 

    // destructor 
    ~CSerialIoOperation() { CIoOperation::~CIoOperation(); }  
    
    // member functions
  public:
    virtual LONG OnTimerEvent(UINT nIdEvent, BOOL bOverrideRepeat=FALSE);
    virtual LONG OnCommNotify(WPARAM wParam, LPARAM lParam);   

  protected:
    virtual UINT GetChannelId(void);
    virtual int DoIoWrite(void);
    virtual int DoIoRead(int *nActualRead, int nAccumulatorOffset);
 
  public:  // MFC debug routines
    #ifdef _DEBUG
    void AssertValid() const;
    void Dump(CDumpContext& dc) const;
    #endif
};




#ifndef DISABLE_TUNING
#include "..\w16gpib\w16gpib.h"

class CGpibIoOperation : public CIoOperation
{   
  DECLARE_DYNAMIC(CGpibIoOperation)
   
  private:                  
    CWin16Gpib	*m_pCWin16Gpib;		// gpib channel 
    int			m_nDvm;				// gpib device descriptor
    
  public: 
    // constructors
    CGpibIoOperation(CWin16Gpib *pCWin16Gpib, int nDvm,
                     IoCommand *pCommands, 
                     int nNumCommands, UINT IOOPid, 
                     int nTicks, ICallBack *pCallBackInterface) :
                     CIoOperation(pCommands, nNumCommands, IOOPid, 
                     nTicks, pCallBackInterface)
    {
      ASSERT(pCWin16Gpib);                                
      m_pCWin16Gpib = pCWin16Gpib;            // gpib card
      m_nDvm = nDvm;                          // Gpib "device"
    } 
    // copy constructor
    CGpibIoOperation(CGpibIoOperation &rCGpibIoOperation)
     : CIoOperation((CIoOperation &)rCGpibIoOperation)
    {
      m_pCWin16Gpib = rCGpibIoOperation.m_pCWin16Gpib; 
      m_nDvm = rCGpibIoOperation.m_nDvm;
    } 
    // destructor 
    ~CGpibIoOperation() { CIoOperation::~CIoOperation(); }

    // member functions 
  public:
    virtual LONG OnCommNotify(WPARAM wParam, LPARAM lParam);  
  
  protected:
    virtual UINT GetChannelId(void); 
    virtual int DoIoWrite(void);
    virtual int DoIoRead(int *nActualRead, int nAccumulatorOffset);
 
  public:  // MFC debug routines
    #ifdef _DEBUG
    void AssertValid() const;
    void Dump(CDumpContext& dc) const;
    #endif
};

#endif // DISABLE_TUNING

 

/////////////////////////////////////////////////////////////////////////
//  CIoController
//
//  This class manages all CIoOperations.
//
class CIoController : public CObject
{
  DECLARE_DYNAMIC(CIoController)

    // protocol IO control elements     
    CIoOperation		*m_pIoOp;
    CObList				m_IoOpList;
           
  public:
    // constructor 
    CIoController()
    {
      m_pIoOp = NULL;
    }
    // copy constructor
    CIoController(CIoController &rCIoController)
    {
      m_pIoOp = NULL;
      ASSERT(1);     // force no use of copy constructor with this class
    }
    // destructor
    ~CIoController() 
    { 
      if (!m_IoOpList.IsEmpty()) 
       EmptyIoOperationList(); 
    }  
    
    // IO control functions
  public:
    BOOL 	IsOperationInProgress() 
    		{ return(m_pIoOp != NULL); }
    BOOL	IsOpListEmpty()
    		{ return(m_IoOpList.IsEmpty()); } 
    const CIoOperation *GetCurrentIoOperation(void)
            { return(m_pIoOp); }
    void	AddTail(CIoOperation *pCIoOperation);
    void	AddHead(CIoOperation *pCIoOperation);   
    LONG	OnTimer(UINT nIDEvent); 
	LONG 	OnCommNotify(WPARAM wParam, LPARAM lParam);
    LONG	OnIoOpCompletion(WPARAM wParam, LPARAM lParam); 
    int		ExecuteIoOpList(void);
    void	EmptyIoOperationList(int nStatus=0);

  public:  // MFC debug routines
    #ifdef _DEBUG
    void AssertValid() const;
    void Dump(CDumpContext& dc) const;
    #endif
};
 
 
 
#endif  // _CIOOPER_
  