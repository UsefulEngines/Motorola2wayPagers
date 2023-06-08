//*************************************************************
//  File name: W16COMM.CPP
//
//  Description:  
//      Emplementation file for the CWin16Comm class.
//      
//      
//
//  History:    Date       Author               Comment
//              3/12/96    P. Pennington        Created
//
// Written by Motorola AMSD, PMU Engineering.
// Legacy code for reference only.
//*************************************************************

#include "StdAfx.h"
#include "..\pps.h"
#include "w16comm.h"
#include "comports.h" 


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

extern CPpsApp theApp;

 
IMPLEMENT_DYNAMIC(CWin16Comm, CObject)

#ifdef _DEBUG
void CWin16Comm::AssertValid() const
{
    CObject::AssertValid();
}

void CWin16Comm::Dump(CDumpContext& dc) const
{
  CObject::Dump(dc);
  dc << "\nm_pszCOMn=" 		   << (const char *)m_pszCOMn 
     << "\tm_pszBuildString="  << (const char *)m_pszBuildString 
     << "\tm_nOperation=" 	   << (int)m_nOperation 
     << "\tm_nStatusCode=" 	   << (unsigned int)m_nStatusCode 
     << "\tm_bConfiguredFlag=" << (int)m_bConfiguredFlag;
}
#endif


// *** Registry Info 
// Loaded from CWin16Comm::FindFirstAvailPort
// Saved from CWin16Comm::CloseCommPort
static const char szCommHeading[] = "CommSettings";
static const char szCommPortNumber[] = "PortNumber";
static const char szCommBuildString[] = "BuildString";
static const char szCommDriverOption[] = "DriverOption";

// *** ICommViewSocket Info
// Use these strings for interaction with any class
// plugged-into the ICommViewSocket.
static const char szRcvData[] = "R<-";
static const char szTmtData[] = "W->";
static const char szErrState[] = "ERR";



// default constructor
CWin16Comm::CWin16Comm()
{
  lstrcpyn(m_pszCOMn, "COM1", sizeof(m_pszCOMn));
  lstrcpyn(m_pszBuildString, "COM1:9600:n,8,1", sizeof(m_pszBuildString));
  m_nStatusCode = 0;
  m_bConfiguredFlag = FALSE;
  m_nNotifyMethod = CN_EVENT; 
  m_pICommViewPlug = NULL;
}

// alternative constructor
CWin16Comm::CWin16Comm(const char *szCOMn, const char *szBuildString)
{
  lstrcpyn(m_pszCOMn, szCOMn, sizeof(m_pszCOMn));
  lstrcpyn(m_pszBuildString, szBuildString, sizeof(m_pszBuildString));
  m_nStatusCode = 0;
  m_bConfiguredFlag = FALSE; 
  m_nNotifyMethod = CN_EVENT; 
  m_pICommViewPlug = NULL;
}

// copy constructor
CWin16Comm::CWin16Comm(CWin16Comm &rCWin16Comm) 
{ 
  lstrcpyn(m_pszCOMn, rCWin16Comm.m_pszCOMn, sizeof(m_pszCOMn));
  lstrcpyn(m_pszBuildString, rCWin16Comm.m_pszBuildString, sizeof(m_pszBuildString));
  memcpy(&m_DCB, &rCWin16Comm.m_DCB, sizeof(DCB)); 
  m_nOperation = rCWin16Comm.m_nOperation;
  m_nStatusCode = rCWin16Comm.m_nStatusCode;
  memcpy(&m_COMSTAT, &rCWin16Comm.m_COMSTAT, sizeof(COMSTAT));
  m_bConfiguredFlag = rCWin16Comm.m_bConfiguredFlag;
  m_nNotifyMethod = rCWin16Comm.m_nNotifyMethod;
  m_pICommViewPlug = rCWin16Comm.m_pICommViewPlug;
}



int CWin16Comm::GetPortID()
{
  if (m_bConfiguredFlag == FALSE)
    return(0);
    
  switch(m_pszCOMn[3])
  {
    case '1': return(1);
    case '2': return(2);
    case '3': return(3);
    case '4': return(4);
    default : return(0);
  }
  return(0);
}


    
static PortDefaults Defaults[] =
	{
		{ "COM1", "COM1:9600,n,8,1" },
		{ "COM2", "COM2:9600,n,8,1" },
		{ "COM3", "COM3:9600,n,8,1" },
		{ "COM4", "COM4:9600,n,8,1" }
	};
		  

int CWin16Comm::FindFirstAvailPort(void)
{
   TRACE("\nCWin16Comm::FindFirstAvailPort\n");
   #ifdef _DEBUG 
     Dump(afxDump);
   #endif
   int lcv;

   // Note:  if an available port is found and
   // 'prepared', it must still be enabled by
   // a call to EnableComms. 

   // First, check to see if a port is specified in the Window's Registry.
   CString strText;
   strText = AfxGetApp()->GetProfileString(szCommHeading, szCommPortNumber);
   if (!strText.IsEmpty())
   { 
     lstrcpyn(m_pszCOMn, (LPCTSTR)strText, sizeof(m_pszCOMn));
     m_pszCOMn[(sizeof(m_pszCOMn)-1)] = '\0'; 
     // now get the build string
     strText = AfxGetApp()->GetProfileString(szCommHeading, szCommBuildString);  
     if (!strText.IsEmpty()) 
     {
       lstrcpyn(m_pszBuildString, (LPCTSTR)strText, sizeof(m_pszBuildString));
       m_pszBuildString[(sizeof(m_pszBuildString)-1)] = '\0';
     } 
     // get the comm notification method, force to CN_EVENT if not 
     // CN_EVENT or CN_POLL.
     m_nNotifyMethod = (UINT)(AfxGetApp()->GetProfileInt(szCommHeading, szCommDriverOption, CN_EVENT));
     if ((m_nNotifyMethod != CN_EVENT) && (m_nNotifyMethod != CN_POLL))
       m_nNotifyMethod = CN_EVENT;
     return(PrepareCommPort());
   }   
   
   // next cycle through the default ports for an open one.
   for (lcv = 0; lcv < (sizeof(Defaults)/sizeof(PortDefaults)); ++lcv)
   {
     if (IsPortAvailable(Defaults[lcv].pszPortName))
       return(PrepareCommPort(Defaults[lcv].pszPortName, Defaults[lcv].pszBuildString));
   }
   return (1);
}



BOOL CWin16Comm::IsPortAvailable(char *pszCOMn)
{
  TRACE("\nCWin16Comm::IsPortAvailable : %s\n", pszCOMn);
  // Determine if the port is in use (open) or available (closed) 
  // by calling OpenComm with the send and receive queue sizes specified as zero.
  // The ports will not be opened, but the returned status will indicate availability.
  // Note: This method may not give accurate results if the hardware is not
  // present.
  
  // do not attempt operation if port is currently in use by this class
  if (m_bConfiguredFlag && (pszCOMn[3] == m_pszCOMn[3]))
    return(TRUE);

  m_nOperation = QUERYPORT;
  m_nStatusCode = ::OpenComm(pszCOMn, 0, 0);
  
  TRACE("\nCWin16Comm::IsPortAvailable : port=%s "
        "m_nStatusCode = %d, IE_OPEN=%d, "
        "IE_HARDWARE=%d, IE_MEMORY=%d, IE_NOPEN=%d\n", 
        pszCOMn, m_nStatusCode, IE_OPEN, IE_HARDWARE, IE_MEMORY, IE_NOPEN);
  

  // IE_OPEN indicates that the port is already opened by another program
  // IE_HARDWARE indicates that the hardware is locked by another device 
  // IE_MEMORY indicates port is avail but opening failed due to queue sizes
  // IE_NOPEN indicates that the port is not open
  if ((m_nStatusCode == IE_MEMORY) || (m_nStatusCode == IE_NOPEN)) 
    return TRUE; 
  else
    return FALSE;
}



// ** ONCOMPORTSETTINGS - configure the port via a dialog window.
void CWin16Comm::OnComPortSettings(void)
{   
  CComPortSettings dlgComSet;
  
  if (dlgComSet.DoModal(this) == IDOK)
  {
    if (IsPortAvailable(m_pszCOMn) == FALSE)
    {
      ProcessCommError();
      return;
    }
    if (PrepareCommPort())
    {
      ProcessCommError();
      return;
    }
    if (EnableComms())
    {
      ProcessCommError();
      return;
    }    
  }
  
  return;
}

    
// ** PREPARECOMMPORT - open and configure the specified comm device.
int CWin16Comm::PrepareCommPort(char *szCOMn, char *szBuildString)
{
  strncpy(m_pszCOMn, szCOMn, 4);
  strncpy(m_pszBuildString, szBuildString, 15);
  return(PrepareCommPort());
}


// ** PREPARECOMMPORT - open and configure the current comm device.
int CWin16Comm::PrepareCommPort()
{ 
  TRACE("\nCWin16Comm::PrepareCommPort : m_pszCOMn=%s : m_pszBuildString=%s\n", 
         m_pszCOMn, m_pszBuildString);
  int idDevice;
  
  // reset port if currently configured. 
  if (IsPortConfigured())
    CloseCommPort(FLUSH);
   
  // open the comm device.  if error then return.
  if ((idDevice = ::OpenComm(m_pszCOMn, INQUESIZE, OUTQUESIZE)) < 0) 
  {
    m_nOperation = OPENPORT;
    m_nStatusCode = idDevice;
    return(1);
  }
  
  // configure the DCB struct.  if error then return.
  ::FlushComm(idDevice, 0);	// ensure the output queue is empty
  ::FlushComm(idDevice, 1);	// ensure the input queue is empty  
  m_nStatusCode = ::BuildCommDCB(m_pszBuildString, &m_DCB);
  if (m_nStatusCode < 0) 
  {
    m_nOperation = BUILDDCB;
    return(1);
  }
  
  // set the device ID
  m_DCB.Id = (BYTE)idDevice;
  
  // ensure hardware flow control is OFF
  m_DCB.fOutxDsrFlow = FALSE;
  m_DCB.DsrTimeout = 0;
  m_DCB.fOutxCtsFlow = FALSE;
  m_DCB.CtsTimeout = 0 ;

  // ensure software flow control is OFF
  m_DCB.fInX = FALSE;
  m_DCB.fOutX = FALSE;

  // other various settings
  m_DCB.fBinary = TRUE;
  m_DCB.fNull = FALSE;
  m_DCB.fParity = FALSE;
  m_DCB.fRtsDisable = TRUE;   
  m_DCB.fDtrDisable = FALSE;  // *** Some programming fixtures use this line for power
  
  // configure the port
  if ((m_nStatusCode = ::SetCommState(&m_DCB)) < 0) 
  {
    // setting the port failed, so close the port and return error.
    ::CloseComm(idDevice);
    m_nOperation = SETSTATE;
    return (1);
  }

  m_bConfiguredFlag = TRUE;
  
  #ifdef _DEBUG 
    Dump(afxDump);
  #endif
  return (0);
}





// ** CLOSECOMMPORT - closes the comm port
// ** flushQ - indicates request to flush the in and out queues before closing
int CWin16Comm::CloseCommPort(FLUSHQ flushQ)
{       
  TRACE("\nCWin16Comm::CloseCommPort\n");

  if (IsPortConfigured() == FALSE)
    return(0);
    
  #ifdef _DEBUG 
    int nActualRead = 0;
    BYTE byReadBuff[16];
    TRACE("\tUnRead Input Queue Contents -->> \n\t");
    do
    {
      if (ReadCommBytes(16, &nActualRead, byReadBuff))
        break;
      for (int lcv = 0; lcv < nActualRead; ++lcv)
      {
        TRACE("%02X ", byReadBuff[lcv]);
        if (lcv+1 == nActualRead)
          TRACE("\n\t"); 
      }
    } while(nActualRead);
    TRACE("\n");
  #endif
 
  if (flushQ) 
  {
    ::FlushComm(m_DCB.Id, 0);	// ensure the output queue is empty
    ::FlushComm(m_DCB.Id, 1);	// ensure the input queue is empty
  }  

  // Disable event notification.  Using a NULL hWnd tells
  // the COMM.DRV to disable future notifications.
  EnableCommNotification(m_DCB.Id, NULL, -1, -1 ) ;

  if ((m_nStatusCode = ::CloseComm(m_DCB.Id)) < 0) 
  {
    m_nOperation = CLOSEPORT;
    return (1);
  }
  m_bConfiguredFlag = FALSE;
  
  // save the configuration string into the application '.ini' file
  AfxGetApp()->WriteProfileString(szCommHeading, szCommPortNumber, m_pszCOMn);
  AfxGetApp()->WriteProfileString(szCommHeading, szCommBuildString, m_pszBuildString);
  AfxGetApp()->WriteProfileInt(szCommHeading, szCommDriverOption, m_nNotifyMethod);
  
  return (0);
}




// ** ENABLE COMMS - enable the generation of WM_COMNOTIFY messages.  This routine
// ** enables windows to send a received-data notification upon receipt of 1 byte. 
// ** Additional bytes will probably be received before the WM_COMNOTIFY message is
// ** processed.  Consequently, you should call ReadComm() repeatedly until windows
// ** returns 0 as the number of bytes received.  This routine disables notification
// ** of when the transmit queue reaching a minimum number of bytes.  Additionally, notification
// ** upon receipt of special events may be initiated by separate calls to SetCommEventMask(). 
int CWin16Comm::EnableComms(void)
{                                
  TRACE("\nCWin16Comm::EnableComms\n");
   
  // NOTE: There are two methods of generating notification of received data
  // to the application.  Both notifications occur via the WM_COMNOTIFY message.
  // However, the LOWORD(lParam) attribute of WM_COMNOTIFY may indicate
  // CN_RECEIVE or CN_EVENT.  We may choose to receive either or both
  // notifications.  If we use CN_EVENT, then the application must call
  // CWin16Comm::GetCommEvent() to reenable receipt notification after
  // each event.  It appears that the use of CN_EVENT is more robust.
  // See the constructors above, as well as the inline GetCommEvent() 
  // function, and the CPager::OnCommNotify() function.
  
  // NOTE: CN_POLL is another notification scheme which has been added just
  // for this application. If selected as the notification method, then the
  // function CWin16Comm::IsRxDataAvailable() must be called at some timed
  // interval in order to the application to receive notification of received
  // data.
  
  //if (m_nNotifyMethod == CN_RECEIVE)    // enable notification via CN_RECEIVE on 1 char
  //  m_nStatusCode = ::EnableCommNotification(m_DCB.Id, theApp.m_pMainWnd->m_hWnd, 1, -1);
  //else                                 // enable notification via CN_EVENT or CN_POLL only
    m_nStatusCode = ::EnableCommNotification(m_DCB.Id, theApp.m_pMainWnd->m_hWnd, -1, -1);

  if (!(m_nStatusCode)) 
  {
    m_nOperation = ENABLEPORT;
    return (1);
  }  
  
  // set comm event masks so they can be cleared via GetCommEvent
  // if the event occurs. 
  if (m_nNotifyMethod == CN_EVENT)
    ::SetCommEventMask(m_DCB.Id, EV_RXCHAR | EV_ERR);   // CN_EVENT
  else
    ::SetCommEventMask(m_DCB.Id, EV_ERR);    			// CN_POLL, CN_RECEIVE

  return (0);
}



// ** GETCOMMEVENT - Call this function upon receipt of a WM_COMMNOTIFY message
// ** with the CN_EVENT notification flag.  This function resets the comm.drv
// ** event handling mechanism as set via the ::SetCommEventMask() function
// ** call above.
UINT CWin16Comm::GetCommEvent()
{  
  if (m_nNotifyMethod == CN_EVENT) 
    return(::GetCommEventMask(m_DCB.Id, EV_RXCHAR | EV_ERR));
  else 
    return(::GetCommEventMask(m_DCB.Id, EV_ERR));
}



// ** READCOMMBYTES - reads up to the maximum requested number of bytes.  Returns (0) upon
// ** success.  Sets pnActualRead to the number of bytes read.  If pnActualRead is less than
// ** nMaxRequested (more available), then repeatedly call ReadCommBytes() until pnActualRead returns 0.
// ** Returns (1) upon any communications API error.  The m_nStatusCode member contains 
// ** the physical error condition code returned by GetCommError().  The m_COMSTAT
// ** struct contains additional information obtained by the GetCommError() call.
// ** Call ProcessCommError immediately upon an error condition in order for a debug
// ** message box to be displayed.  
int CWin16Comm::ReadCommBytes(int nMaxRequested, int *pnActualRead, BYTE *pbyBuffer)
{                                
  TRACE("\nCWin16Comm::ReadCommBytes :");
  ASSERT(nMaxRequested >= 0);
  
  m_nOperation = RECEIVE;
  int nError = 0;
  
  *pnActualRead = ::ReadComm(m_DCB.Id, pbyBuffer, nMaxRequested);
  
  // ReadComm returns negative #bytes read on error
  if (*pnActualRead < 0)
  { 
    nError = 1;
    *pnActualRead *= (-1);
  }
    
  TRACE("\tnActualRead=%d : cbInQue=%d : nStatus=%04X\n", *pnActualRead, m_COMSTAT.cbInQue, m_nStatusCode);

  if ((m_pICommViewPlug != NULL) && (*pnActualRead))
    m_pICommViewPlug->CommNote(m_pszCOMn, szRcvData, pbyBuffer, *pnActualRead);

  // clear any possible error condition
  m_nStatusCode = ::GetCommError(m_DCB.Id, &m_COMSTAT);
  
  // if zero, assume no real error and reset comm notification
  if ((nError == 0) && (m_nStatusCode == 0)) 
     return(0);
  return(1);
}



// ** IsRxDataAvailable - Polls the serial port input queue for received data.
BOOL CWin16Comm::IsRxDataAvailable(void)
{ 

  // obtain the current serial port state
  ::GetCommError(m_DCB.Id, &m_COMSTAT);
  
  // use this occassion to ensure receive notification is sent to the application.
  if ((m_COMSTAT.cbInQue > 0) && (m_nNotifyMethod == CN_POLL))
    theApp.m_pMainWnd->PostMessage(WM_COMMNOTIFY, m_DCB.Id, CN_POLL); 
  
  TRACE("\nCWin16Comm::IsRxDataAvailable : %d\n", m_COMSTAT.cbInQue);
  GetCommEvent();    // reset event notification mechanism           
  
  return(m_COMSTAT.cbInQue > 0);
  
  /***** alternate implementation
  BYTE byCh;
  int nActualRead;
  
  if (ReadCommBytes(1, &nActualRead, &byCh))
    return(FALSE);  // on error
  
  if (nActualRead)  // data available
  {
    // replace character into the RxQueue
    if (::UngetCommChar(m_DCB.Id, byCh) < 0)
    {
      m_nStatusCode = ::GetCommError(m_DCB.Id, &m_COMSTAT);
      return(FALSE);
    }
    GetCommEvent();    // reset event notification mechanism
    return(TRUE);  // data available, no error
  }
  GetCommEvent();    // reset event notification mechanism
  return(FALSE);   // no data available, no error
  ****/  
}




// ** SENDCOMMBYTES - transmits up to the maximum requested number of bytes.  Depending
// ** upon specified parameters, the bytes are sent either one-at-a-time using the API
// ** function TransmitCommChar(), or simply placed in the send queue for transfer at
// ** windows convenience using the API function WriteComm().  Returns (0) upon success
// ** and (1) upon any failure.  The variable pnNumSent returns the number of bytes 
// ** actually sent or placed in the send queue.  An (1) condition may simply indicate 
// ** that the transmit queue is full or that all requested number of bytes could not
// ** be placed in the transmit queue.  The queue status will be indicated in the
// ** m_COMSTAT struct.  
int CWin16Comm::SendCommBytes(BYTE *pBytes, int nSize)
{ 
  ASSERT(pBytes != NULL);
  ASSERT(nSize >= 0);

  m_nOperation = TRANSMIT;

  ::WriteComm(m_DCB.Id, pBytes, nSize);

  if (m_pICommViewPlug != NULL)
    m_pICommViewPlug->CommNote(m_pszCOMn, szTmtData, pBytes, nSize);

  m_nStatusCode = ::GetCommError(m_DCB.Id, &m_COMSTAT);
  if (m_nStatusCode)
    return(1);
  return(0);
}

int CWin16Comm::SendCommBytes(BYTE *pBytes, int nSize, int *pnNumSent)
{
  ASSERT(pBytes != NULL);
  ASSERT(nSize >= 0);
  TRACE("\nCWin16Comm::SendCommBytes :  nSize=%d\n", nSize);
  register int lcv;
  register BYTE	*pb; 
  
  m_nOperation = TRANSMIT;
  pb = pBytes;
  for (lcv = 0; lcv < nSize; ++lcv) 
  {
    ::TransmitCommChar(m_DCB.Id, *pb); 
    m_nStatusCode = ::GetCommError(m_DCB.Id, &m_COMSTAT);
    if (m_nStatusCode)
      return(1);
    if (m_pICommViewPlug != NULL)
      m_pICommViewPlug->CommNote(m_pszCOMn, szTmtData, pb, 1);
    *pnNumSent++;
    pb++;
  }
  return(0);  
}  
 


// ** PAUSECOMMS
int CWin16Comm::PauseComms()
{ 
  TRACE("\nCWin16Comm::PauseComms\n");

  if ((m_nStatusCode = ::SetCommBreak(m_DCB.Id)) < 0) 
  {
    m_nOperation = PAUSE;
    return (1);
  }
  return (0);
}


// ** RESUMECOMMS
int CWin16Comm::ResumeComms()
{
  TRACE("\nCWin16Comm::ResumeComms\n");

  if ((m_nStatusCode = ::ClearCommBreak(m_DCB.Id)) < 0) 
  { 
    m_nOperation = RESUME;
    return (1);
  }                                        

  GetCommEvent();    // reset event notification mechanism
  return (0);
}




//static const char *szUnsolicitedData = "Unsolicited serial port data received.  "
//                                       "Receive queue flushed.  Press F1 for help.";
void CWin16Comm::HandleUnsolicitedData(void)    
{
  //static BOOL bMsgShowing = FALSE;
  
  // is this in response to an out-of-date WM_COMMNOTIFY msg?
  ::GetCommError(m_DCB.Id, &m_COMSTAT);
  if (m_COMSTAT.cbInQue == 0)
  {  
    GetCommEvent();    // reset event notification mechanism
    return;
  }
  
  if (FlushReceiveQueue())
    ResetComms(); 

  GetCommEvent();    // reset event notification mechanism
  
  if (m_pICommViewPlug != NULL)
    m_pICommViewPlug->CommError(m_pszCOMn, "Unsolicited Data", m_nStatusCode);
  
  /*** IGNORE UNSOLICITED DATA.  May be PING from pager if pager
       has Y-Command Protocol support.  7/30/97 PGP.  
  if (!bMsgShowing)
  { 
    bMsgShowing = TRUE;
    DWORD dwHelpID = HID_BASE_RESOURCE + IDP_UNSOLICITED_DATA;
    int nMsgRet = AfxMessageBox(szUnsolicitedData, MB_OK, (UINT)dwHelpID); 
    if (nMsgRet == MB_OK)
      bMsgShowing = FALSE;
  }
  */
  return;
}       



// ** FLUSH
int CWin16Comm::FlushReceiveQueue(void)
{

  // *** DIAGNOSTIC STUFF 
  if (m_pICommViewPlug != NULL)
  {
    int nActualRead = 0;
    BYTE byReadBuff[16];
    do
    { 
      if (m_COMSTAT.cbInQue == 0)
        break;
      ReadCommBytes(16, &nActualRead, byReadBuff);
      m_nStatusCode = ::GetCommError(m_DCB.Id, &m_COMSTAT);
    } while((nActualRead > 0) && (m_nStatusCode == 0));
  }
  // *** DIAGNOSTIC STUFF

  m_nStatusCode = ::FlushComm(m_DCB.Id, 1);	
  
  GetCommEvent();    // reset event notification mechanism

  if (m_nStatusCode > 0)
  {
    m_nOperation = FLUSH_RCV_QUE;
    return(1);
  }
  return(0);   
}           



//** RESETCOMMS
int CWin16Comm::ResetComms()
{
  TRACE("\nCWin16Comm::ResetComms\n");

  m_nOperation = RESET;
  m_nStatusCode = ::GetCommError(m_DCB.Id, &m_COMSTAT);
  
  // *** DIAGNOSTIC STUFF
  int nActualRead = 0;
  BYTE byReadBuff[16];
  TRACE("\nCWin16Comm::ResetComms : UnRead Input Queue Contents:\n\t");
  do
  { 
    if (m_COMSTAT.cbInQue == 0)
      break;
    ReadCommBytes(16, &nActualRead, byReadBuff);
    #ifdef _DEBUG
      for (int lcv = 0; lcv < nActualRead; ++lcv)
      {
        TRACE("%02X ", byReadBuff[lcv]);
        if (lcv+1 == nActualRead)
          TRACE("\n\t"); 
      }
    #endif
    m_nStatusCode = ::GetCommError(m_DCB.Id, &m_COMSTAT);
  } while((nActualRead > 0) && (m_nStatusCode == 0));
  TRACE("\n");   
  // *** DIAGNOSTIC STUFF
  
  if (m_nStatusCode)
    ProcessCommError();
    
  if (m_nStatusCode & CE_BREAK)
    ResumeComms();
    
  ::FlushComm(m_DCB.Id, 0);	// ensure the output queue is empty
  ::FlushComm(m_DCB.Id, 1);	// ensure the input queue is empty

  // note: this call actually does nothing with the serial port.
  // see the documentation of EscapeCommFunction.
  //if ((m_nStatusCode = ::EscapeCommFunction(m_DCB.Id, RESETDEV)) < 0) 
  //{
  //  m_nOperation = RESET;
  //  return (1);
  //}
  
  return(::GetCommError(m_DCB.Id, &m_COMSTAT));
}





// ** PROCESS ERROR - handle errors that may occur with the commo functions 
// ** note: this could be cleaned up with a string table resource if anyone had the time.
void CWin16Comm::ProcessCommError(void)
{
  static BOOL bErrMsgShowing = FALSE;
  TRACE("\nCWin16Comm::ProcessCommError : bErrMsgShowing=%d\n", bErrMsgShowing);
  
  if (bErrMsgShowing)  // display only 1 error msg at a time
    return;
  
  CString msg;
  
  msg += "Serial Port Error : operation = "; 
  
  switch(m_nOperation) 
  {
    case QUERYPORT : 
    	msg +=  "QUERY : Device not Available.";
    	break;
    case OPENPORT : 
    	msg +=  "OPENING PORT : "; 
    	switch(m_nStatusCode) 
    	{
     	  case IE_BADID : msg +=  "An invalid device name."; break;
     	  case IE_BAUDRATE : msg +=  "Unsupported baud rate."; break;
     	  case IE_BYTESIZE : msg +=  "Unsupported byte size."; break;
     	  case IE_DEFAULT : msg +=  "Error in the default parameters."; break;
     	  case IE_HARDWARE : msg +=  "Hardware not available."; break;
     	  case IE_MEMORY : msg +=  "Unable to allocate memory for the queues."; break;
     	  case IE_NOPEN : msg +=  "Not able to open the device."; break;
     	  case IE_OPEN : msg +=  "Device already open, locked, or not available."; break;
     	  default : msg +=  "ERROR UNKNOWN"; break;
     	} 
    	break;
    case BUILDDCB : 
    	msg +=  "BUILDING DCB : Unsuccessful call to API function BuildCommDCB()."; 
    	break;
    case SETSTATE : 
    	msg +=  "SETTING STATE : Unsuccessful call to API function SetCommState()."; 
    	break;
    case ENABLEPORT : 
    	msg +=  "ENABLING PORT : Unsuccessful call to API function EnableCommNotification()."; 
    	break;
    case TRANSMIT : 
    	msg +=  "TRANSMITTING DATA : ";
    	GetCommErrorString(msg);
    	GetComstatCodes(msg);
    	break;
    case RECEIVE : 
    	msg +=  "RECEIVING DATA : ";
    	GetCommErrorString(msg);
    	GetComstatCodes(msg); 
    	break;
    case CLOSEPORT : 
    	msg +=  "CLOSING PORT : Unsuccessful call to API function CloseComm()."; 
    	break;
    case PAUSE : 
    	msg +=  "PAUSE COMMS : Unsuccessful call to API function SetCommBreak()."; 
    	break;
    case RESUME : 
    	msg +=  "RESUMING COMMS : Unsuccessful call to API function ClearCommBreak()."; 
    	break;
    case RESET : 
    	msg +=  "RESET COMM PORT : \n"; 
    	GetCommErrorString(msg);
    	GetComstatCodes(msg); 
    	break; 
    case FLUSH_RCV_QUE :
        msg +=  "FLUSH RECEIVE QUE : \n";
    	GetCommErrorString(msg);
    	GetComstatCodes(msg); 
    	break; 
    default :  
      msg +=  "OP UNKNOWN : ERROR UNKNOWN";  
      GetComstatCodes(msg);
      break;
  } 
  
  msg += "\nPress F1 for help.\n";
  if (!bErrMsgShowing)
  { 
    //bErrMsgShowing = TRUE;
    DWORD dwHelpID = HID_BASE_RESOURCE + IDP_SERIAL_PORT_ERROR;   
    int nMsgRtn = AfxMessageBox((LPCTSTR)msg, MB_OK, (UINT)dwHelpID);
    if (nMsgRtn == MB_OK)
      bErrMsgShowing = FALSE;
  }
  return;
}


  
// ** local helper function for ProcessError() and readability
void CWin16Comm::GetCommErrorString(CString &msg)
{
  TRACE("\nCWin16Comm::GetCommErrorString : %04X\n", m_nStatusCode);

  if (m_nStatusCode & CE_BREAK)  msg +=  "A break condition was detected.\n";  
  if (m_nStatusCode & CE_CTSTO)  msg +=  "Clear-to-Send timeout.\n";  
  if (m_nStatusCode & CE_DNS)    msg +=  "Parallel device not selected.\n";  
  if (m_nStatusCode & CE_DSRTO)  msg +=  "Data-set-ready timeout.\n";  
  if (m_nStatusCode & CE_FRAME)  msg +=  "Hardware framing error detected.\n";  
  if (m_nStatusCode & CE_IOE)    msg +=  "Input/Output error on a parallel device.\n";  
  if (m_nStatusCode & CE_MODE)   msg +=  "Requested comm mode not supported or invalid device Id.\n";  
  if (m_nStatusCode & CE_OOP)    msg +=  "The parallel device is out of paper.\n";  
  if (m_nStatusCode & CE_OVERRUN) msg +=  "Data arrived before the last data could be read. The data is lost.\n";  
  if (m_nStatusCode & CE_PTO)    msg +=  "Parallel device time out.\n";  
  if (m_nStatusCode & CE_RLSDTO) msg +=  "Receive-line-signal-detect time out.\n";  
  if (m_nStatusCode & CE_RXOVER) msg +=  "The receive queue overflowed.\n";  
  if (m_nStatusCode & CE_RXPARITY) msg +=  "A parity error was detected.\n";  
  if (m_nStatusCode & CE_TXFULL)  msg +=  "The transmit queue is full.\n";  
  
  if (m_pICommViewPlug != NULL)
      m_pICommViewPlug->CommNote(m_pszCOMn, szErrState, (LPCTSTR)msg);

  return;
}




// ** helper function for ProcessError() and readability
void CWin16Comm::GetComstatCodes(CString &msg)
{
  TRACE("\nCWin16Comm::GetComstatCodes\n");
  char string[200];
  LONG lPort = ::EscapeCommFunction(m_DCB.Id, GETBASEIRQ);
  
  sprintf(string, "\nPORT STATS:"
                  "\nCOM%d : IRQ=%04X : BASE_ADDR=%04X"
                  "\nCTS hold=%d"
  		          "\nDSR hold=%d"
  			      "\nRLSD hold=%d"
  			      "\nXOFF hold=%d"
    		      "\nXOFF sent=%d"
    		      "\nEOF found=%d"
    		      "\nXMT waiting=%d"
    		      "\nRxQue=%d"
    		      "\nTxQue=%d\n",
    		      (m_DCB.Id+1), HIWORD(lPort), LOWORD(lPort),
    		      (m_COMSTAT.status & CSTF_CTSHOLD),
    		      (m_COMSTAT.status & CSTF_DSRHOLD),
    		      (m_COMSTAT.status & CSTF_RLSDHOLD),
    		      (m_COMSTAT.status & CSTF_XOFFHOLD),
    		      (m_COMSTAT.status & CSTF_XOFFSENT),
    		      (m_COMSTAT.status & CSTF_EOF),
    		      (m_COMSTAT.status & CSTF_TXIM),
    		      m_COMSTAT.cbInQue,
    		      m_COMSTAT.cbOutQue);
  msg += string; 
  return;
}


// ** This function may be called by another class which implements the
// ** ICommViewSocket interface.  The function sets the m_pICommViewPlug
// ** if the corresponding function parameter is not NULL and returns
// ** the previous pointer.  This allows the new plug to use the previous
// ** pointer in a chaining fashion if required.  That is, new plugs can
// ** attach themselves in a stack-like list of plug-in's.  Each attached
// ** plug-in is responsible for passing callback's on to the next
// ** plug-in in the stack.  This approach can be dangerous if any 
// ** plug-in in the call stack is deleted from memory.  Additionally,
// ** any call to DetachCommViewPlug() detaches all plug-in's from the
// ** CWin16Comm class. 
ICommViewSocket *CWin16Comm::AttachCommViewPlug(ICommViewSocket *pICommViewPlug)
{
  ICommViewSocket *pTemp = m_pICommViewPlug;
   
  if (pICommViewPlug != NULL) 
    m_pICommViewPlug = pICommViewPlug;
     
  return(pTemp); 
} 

  