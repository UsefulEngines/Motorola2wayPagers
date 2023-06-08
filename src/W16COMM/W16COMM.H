//*************************************************************
//  File name: W16COMM.H
//
//  Description:  
//      Declaration file for the CWin16Comm class.
//
//
//  Comments:
//      This class implements a serial communications
//      mechanizm for Windows 3.1 based applications.
//      It is not portable to the WIN32 API and programs
//      compiled for WIN32 should incorporate an
//      implementation based on the improved WIN32 API.
//      The extent of modifications required has not been
//      determined as of 3/13/96.
//      
//      
//
//  History:    Date       Author               Comment
//              3/12/96    P. Pennington        Created
//
// Written by Motorola AMSD, PMU Engineering.
// Legacy code for reference only.
//*************************************************************
 
 
#ifndef _WIN16COMM_
#define _WIN16COMM_
 
#include "comports.h"

// CALLBACK INTERFACE - Another class may use this callback
// interface to 'plug-into' the CWin16Comm class and receive
// notifications of serial port transactions.  See the files
// '..\views\commview\commview.*'. 
struct ICommViewSocket
{
  virtual void	CommNote(const char *pszPort, const char *pszOp, BYTE *pData, int nNumBytes) = 0;
  virtual void  CommNote(const char *pszPort, const char *pszOp, const char *pszString) = 0;
  virtual void  CommError(const char *pszPort, const char *pszOp, DWORD dwStatusCode) = 0; 
};


// Use this macro to construct a pointer to the class which nests an
// ICommViewSocket derived class.  This macro is for 16-bit Windows only.
#define METHOD_PROLOGUE(theClass, localClass) \
  theClass* pThis = ((theClass*)((char*)(this) - offsetof(theClass, localClass)));



// Ensure CN_POLL is not equal to CN_RECEIVE, CN_EVENT or CN_TRANSMIT.
// You may have to check the 'windows.h' file. 
#define CN_POLL	 0x0008 
 
typedef enum { NOCOMM, COM1, COM2, COM3, COM4 } CONNECTOR;      
typedef enum { QUERYPORT, OPENPORT, 
			   BUILDDCB, SETSTATE, 
			   ENABLEPORT, TRANSMIT, 
			   RECEIVE, CLOSEPORT, 
			   PAUSE, RESUME, RESET,
			   FLUSH_RCV_QUE } OPERATION;
typedef enum { NOTAVAILABLE, AVAILABLE } AVAILABILITY;
typedef enum { NOFLUSH, FLUSH } FLUSHQ;                                   
 
typedef struct
{
  char	*pszPortName;
  char	*pszBuildString;
} PortDefaults;
 



/////////////////////////////////////////////////////////////////////
//  CWin16Comm
//
//  Encapsulation of the WIN16 serial communications API.  Note
//  that the API changes with WIN32.  Porting the application to
//  WIN32 will require modifications to this class.
// 
class CWin16Comm : public CObject
{
  DECLARE_DYNAMIC(CWin16Comm)
    enum { INQUESIZE = 2048, OUTQUESIZE = 1024 } COMM_BUF_SIZE;
    char			m_pszCOMn[5];			// e.g. "COM1"
    char 			m_pszBuildString[25];	// e.g. "COM1:9600:n,8,1"
    DCB			    m_DCB;					// windows device control block
    OPERATION		m_nOperation;			// last operation for user msg
    LONG			m_nStatusCode;			// returned COMM API error
    COMSTAT		    m_COMSTAT;              // windows port status struct
    BOOL			m_bConfiguredFlag;		// indicator of configured or not 
    UINT			m_nNotifyMethod;		// seen 'enablecomms'
    ICommViewSocket   *m_pICommViewPlug;    // comm viewer callback mechanizm
                                                                                    
  public: 
    // constructors
    CWin16Comm();
    CWin16Comm(const char *szCOMn, const char *szBuildString);
    CWin16Comm(CWin16Comm &rCWin16Comm);
    
    // destructor
    ~CWin16Comm() 
    {
      if (IsPortConfigured())
        CloseCommPort(FLUSH);
    }       
    
    // member functions
    UINT 	GetBaudRate(void)
            {  return(m_DCB.BaudRate); }
    // note:  return of 1 indicates error; 0 indicates success 
    int 	FindFirstAvailPort();
    BOOL 	IsPortAvailable(char *pszCOMn);
    BOOL 	IsCnReceiveInUse(void) 
    		{  return(m_nNotifyMethod == CN_RECEIVE); }
    void 	UseCnReceive(BOOL CnReceive = TRUE) 
    		{  
    		   if (CnReceive) m_nNotifyMethod = CN_RECEIVE;
      		   else m_nNotifyMethod = CN_EVENT;
    		}
    BOOL 	IsCnPollInUse(void)
    		{  return(m_nNotifyMethod == CN_POLL); }
    void 	UseCnPoll(BOOL CnPoll = TRUE)
    		{
      		   if (CnPoll) m_nNotifyMethod = CN_POLL;
      		   else m_nNotifyMethod = CN_EVENT;
    		} 
    BOOL 	IsCnEventInUse(void)
    		{  return(m_nNotifyMethod == CN_EVENT); } 
    void	OnComPortSettings(void);
    int 	PrepareCommPort();
    int 	PrepareCommPort(char *pszCOMn, char *szBuildString);
    int 	CloseCommPort(FLUSHQ flushQ);
    int 	EnableComms(void);
    int 	ReadCommBytes(int nMaxRequested, int *pnActualRead, BYTE *pbyBuffer);
    void 	ProcessCommError(void);
    int 	SendCommBytes(BYTE *pBytes, int nSize, int *pnNumSent);
	int 	SendCommBytes(BYTE *pBytes, int nSize);
    int 	PauseComms();
    int 	ResumeComms();
    int 	ResetComms();
    BOOL 	IsPortConfigured() { return(m_bConfiguredFlag); } 
    int 	GetPortID(); 
    int		GetDeviceId(void) { return(m_DCB.Id); } 
    const char *pszGetPortBuildString(void)
    		{  return(m_pszBuildString); }
    BOOL 	ConfirmDeviceId(UINT DeviceId) { return(m_DCB.Id == DeviceId); } 
    UINT 	GetCommEvent();
    BOOL 	IsRxDataAvailable(void);
    void 	HandleUnsolicitedData(void);
    int 	FlushReceiveQueue(void); 
    ICommViewSocket 	*AttachCommViewPlug(ICommViewSocket *pICommViewPlug);
    void	DetachCommViewPlug(ICommViewSocket *pICommViewPlug = NULL)
            {  m_pICommViewPlug = pICommViewPlug; }
  private:
    void GetCommErrorString(CString &msg);
    void GetComstatCodes(CString &msg); 
  public:
    #ifdef _DEBUG 
    void AssertValid() const;
    void Dump(CDumpContext& dc) const;
    #endif
  friend class CComPortSettings;    // settings configuration dialog
};


  
  
#endif // _WIN16COMM_

      