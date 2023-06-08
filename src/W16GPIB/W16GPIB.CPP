//*************************************************************
//  File name: W16GPIB.CPP
//
//  Description:  
//      Emplementation file for the CWin16Gpib class.
//      
//      
//
//  History:    Date       Author               Comment
//
// Written by Motorola AMSD, PMU Engineering.
// Legacy code for reference only.
//*************************************************************

#include "StdAfx.h"
#include "..\pps.h"
#include "gpib\windecl.h"
#include "w16gpib.h" 


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


#ifndef DISABLE_TUNING    // conditional compilation of this file


extern CPpsApp theApp;

 
IMPLEMENT_DYNAMIC(CWin16Gpib, CObject)

#ifdef _DEBUG
void CWin16Gpib::AssertValid() const
{
    CObject::AssertValid();
}

void CWin16Gpib::Dump(CDumpContext& dc) const
{
  CObject::Dump(dc); 
  dc << "\nm_nCardId=" 		    << m_nCardId 
     << "\tm_bCardConfigured="  << (int)m_bCardConfigured; 
}
#endif


// *** Registry Info 
// Loaded from CWin16Gpib::ConfigureCard
// Saved from CWin16Gpib::~CWin16Gpib
static const char szGpibHeading[] = "GpibSettings";
static const char szCardAddress[] = "CardAddress";
                              
                              
// *** IGpibViewSocket strings
static const char *pszPort = "GPIB";
static const char *pszRead = "R<-";
static const char *pszWrite = "W->";
static const char *pszErrState = "E";


// default constructor
CWin16Gpib::CWin16Gpib() 
{  
   m_bCardConfigured = FALSE;
   m_nCardId = 0;
   m_pIGpibViewPlug = NULL;
}

// copy constructor
CWin16Gpib::CWin16Gpib(CWin16Gpib &rCWin16Gpib)
{  
   m_bCardConfigured = rCWin16Gpib.m_bCardConfigured;
   m_nCardId = rCWin16Gpib.m_nCardId;
   m_pIGpibViewPlug = rCWin16Gpib.m_pIGpibViewPlug;
}



// destructor
CWin16Gpib::~CWin16Gpib()
{ 
   // disable card  
   if (m_bCardConfigured == TRUE) 
   {
     EnableCard(FALSE);
     AfxGetApp()->WriteProfileInt(szGpibHeading, szCardAddress, m_nCardId);
   }   
}       
 

/////////////////////////////////////////////////////////////////////////
//  ConfigureCard()
//
int	CWin16Gpib::ConfigureCard(void)
{ 
   TRACE("\nCWin16Gpib::ConfigureCard\n"); 

   // initialize device address from saved .ini entry, if available
   m_nCardId = AfxGetApp()->GetProfileInt(szGpibHeading, szCardAddress, 0);

   // make the card the Controller-in-Charge.
   ::SendIFC(m_nCardId);
   if (ibsta & ERR)
   {
     m_bCardConfigured = FALSE;
     return(1);
   }

   m_bCardConfigured = TRUE;  
   return(0);  // return OK
}

 

/////////////////////////////////////////////////////////////////////////
//  GetGpibError()
//
// 
void CWin16Gpib::GetGpibError(CString &msg) 
{ 
    char szTemp[40];
    
    sprintf(szTemp, "GPIB%d : ibsta = 0x%X", m_nCardId, ibsta);
    msg = szTemp;
    if (ibsta & ERR )  msg += "\n\tERR <Error detected>";
    if (ibsta & TIMO)  msg += "\n\tTIMO <Timeout";
    if (ibsta & END )  msg += "\n\tEND <EOI or EOS detected>";
    if (ibsta & SRQI)  msg += "\n\tSRQI <SRQ detected by CIC>";
    if (ibsta & RQS )  msg += "\n\tRQS <Device needs service>";
    if (ibsta & SPOLL) msg += "\n\tSPOLL <Board has been serially polled>";
    if (ibsta & EVENT) msg += "\n\tEVENT <An event has occured>";
    if (ibsta & CMPL)  msg += "\n\tCMPL <I/O completed>";
    if (ibsta & LOK )  msg += "\n\tLOK <Local lockout state>";
    if (ibsta & REM )  msg += "\n\tREM <Remote state>";
    if (ibsta & CIC )  msg += "\n\tCIC <Controller-in-Charge>";
    if (ibsta & ATN )  msg += "\n\tATN <Attention asserted>";
    if (ibsta & TACS)  msg += "\n\tTACS <Talker active>";
    if (ibsta & LACS)  msg += "\n\tLACS <Listener active>";
    if (ibsta & DTAS)  msg += "\n\tDTAS <Device trigger state>";
    if (ibsta & DCAS)  msg += "\n\tDCAS <Device clear state>";
 
    sprintf(szTemp, "\nGPIB%d : iberr = 0x%X", m_nCardId, iberr);
    msg += szTemp;
    if (iberr == EDVR) msg += "\n\tEDVR <System Error>";
    if (iberr == ECIC) msg += "\n\tECIC <Not CIC>";
    if (iberr == ENOL) msg += "\n\tENOL <No Listener>";
    if (iberr == EADR) msg += "\n\tEADR <Address error>";
    if (iberr == EARG) msg += "\n\tEARG <Invalid argument>";
    if (iberr == ESAC) msg += "\n\tESAC <Not Sys Ctrlr>";
    if (iberr == EABO) msg += "\n\tEABO <Op. aborted>";
    if (iberr == ENEB) msg += "\n\tENEB <No GPIB board>";
    if (iberr == EOIP) msg += "\n\tEOIP <Async I/O in prg>";
    if (iberr == ECAP) msg += "\n\tECAP <No capability>";
    if (iberr == EFSO) msg += "\n\tEFSO <File sys. error>";
    if (iberr == EBUS) msg += "\n\tEBUS <Command error>";
    if (iberr == ESTB) msg += "\n\tESTB <Status byte lost>";
    if (iberr == ESRQ) msg += "\n\tESRQ <SRQ stuck on>";
    if (iberr == ETAB) msg += "\n\tETAB <Table Overflow>";
     
    sprintf(szTemp, "\nGPIB%d : ibcnt = %d\n\n", m_nCardId, ibcnt);
    msg += szTemp; 
    
    // diagnostic output...
    if ((m_pIGpibViewPlug != NULL))
      m_pIGpibViewPlug->GpibNote(pszPort, pszErrState, (LPCTSTR)msg);

  return;
}


/////////////////////////////////////////////////////////////////////////////
//  Open()
//  
//  See GPIB function ibdev descriptions.  This command selects an unopened
//  device, opens it, and initializes it.  The function returns a descriptor
//  of the first unopened user-configurable device that it finds.  The 
//  descriptor can remain open across invocations of the application, so be
//  sure to return the device descriptor to the pool of available devices
//  by calling EnableDevice(nDvm, FALSE) when finished using the device.
//  Otherwise, the device will not be available for the next Open() call. 
//
int	CWin16Gpib::Open(int nPad, int nSad, int nTmo, int nEot, int nEos, int *nDvm)
{
  *nDvm = ::ibdev(m_nCardId, nPad, nSad, nTmo, nEot, nEos);
  if ((*nDvm < 0) || (ibsta & ERR))
   return(1);
  return(0);
}
  



//////////////////////////////////////////////////////////////////////////////
//  Clear()
//
int	CWin16Gpib::Clear(int nDvm)
{
  ::ibclr(nDvm);
  if (ibsta & ERR)
    return(1);
  return(0);
}



////////////////////////////////////////////////////////////////////////////
//  Write()
//
int CWin16Gpib::Write(int nDvm, char *pszString, long lCount) 
{
  ::ibwrt(nDvm, pszString, lCount);
  if (ibsta & ERR)
    return(1);

  // diagnostic output...
  if ((m_pIGpibViewPlug != NULL))
    m_pIGpibViewPlug->GpibNote(pszPort, pszWrite, pszString);

  return(0);
}


///////////////////////////////////////////////////////////////////////////
//  Trigger()
//
int	CWin16Gpib::Trigger(int nDvm)
{
  ::ibtrg(nDvm);
  if (ibsta & ERR)
    return(1);
  return(0);
}



///////////////////////////////////////////////////////////////////////////
//  Wait()  
//
int	CWin16Gpib::Wait(int nDvm, int nMask)
{ 
  ::ibwait(nDvm, nMask);
  if (ibsta & ERR)
    return(1);
  return(0);
}


///////////////////////////////////////////////////////////////////////////
//  ReturnSerialPoll() 
//
int	CWin16Gpib::ReturnSerialPoll(int nDvm, char *pcResponse)
{
  ::ibrsp(nDvm, pcResponse);
  if (ibsta & ERR)
    return(1);
  return(0);
}



///////////////////////////////////////////////////////////////////////////
//  Read() 
//
int	CWin16Gpib::Read(int nDvm, BYTE *pbyBuffer, long lCount)
{ 
  ::ibrd(nDvm, pbyBuffer, lCount);
  
  // mark the end of data 
  pbyBuffer[ibcntl] = '\0';
  
  // diagnostic output...
  if ((m_pIGpibViewPlug != NULL) && (ibcntl))
    m_pIGpibViewPlug->GpibNote(pszPort, pszRead, (const char *)pbyBuffer);

  if (ibsta & ERR)
    return(1);
  return(0);
}


///////////////////////////////////////////////////////////////////////////
//  EnableCard() 
// 
//  After a device or an interface board is taken offline, the handle (nDvm)
//  is no longer valid.  Before accessing the board or device again, you
//  must re-execute an ibfind or ibdev call to open the board or device.
//
int	CWin16Gpib::EnableCard(BOOL bEnable)
{ 
  ::ibonl(m_nCardId, (int)bEnable);
  if (ibsta & ERR)
    return(1);
  return(0);
}


///////////////////////////////////////////////////////////////////////////
//  EnableDevice() 
//
//  After a device or an interface board is taken offline, the handle (nDvm)
//  is no longer valid.  Before accessing the board or device again, you
//  must re-execute an ibfind or ibdev call to open the board or device.
//
int	CWin16Gpib::EnableDevice(int nDvm, BOOL bEnable)
{ 
  ::ibonl(nDvm, (int)bEnable);
  if (ibsta & ERR)
    return(1);
  return(0);
}



///////////////////////////////////////////////////////////////////////////
//  GetLastCount() 
//
long CWin16Gpib::GetLastCount(void)
{ 
  return(ibcntl);
}






///////////////////////////////////////////////////////////////////////////
//  FindDevice() 
//
//
int	CWin16Gpib::FindDevice(const char *pszIDN, Addr4882_t *pAddr4882_t)
{  
  int         num_listeners;     // Number of listeners on GPIB
  int		  loop;    
  Addr4882_t  instruments[32];   // Array of primary addresses      
  Addr4882_t  result[31];        // Array of found listen addresses  
  char		  szInstrument[51];  // Name of instrument 
  
  TRACE("\nCWin16Gpib::FindDevice : szIDN=%s\n", pszIDN);              
  
  // first ensure the GPIB card is configured...
  if (IsCardConfigured() == FALSE) 
  {
    if (ConfigureCard())    // error
      return(1);
  }
  
  // first look for device at address, if specified.
  if (*pAddr4882_t > 0)
  {
    ::Send(m_nCardId, *pAddr4882_t, "*IDN?", 5L, NLend);
    if ((ibsta & ERR) == 0)
    {
      ::Receive(m_nCardId, *pAddr4882_t, szInstrument, 50L, STOPend);
      if ((ibsta & ERR) == 0)
      { 
        szInstrument[ibcnt] = '\0'; 
        TRACE("\tCWin16Gpib::FindDevice : szInstrument=%s\n", szInstrument);              
        if (::strncmp(szInstrument, pszIDN, ::lstrlen(pszIDN)) == 0) 
         return(0);  // device found at specified address
      }
    }
  }
  
  // Create an array containing all valid GPIB primary addresses.  
  for (loop = 0; loop <= 30; loop++)
  {
    instruments[loop] = (Addr4882_t)loop;
  }
  instruments[31] = NOADDR;    

  // Store the listen addresses in the array RESULT.
  ::FindLstn(m_nCardId, &instruments[1], result, 31);
  if (ibsta & ERR) 
    return(1);

  // Assign the value of IBCNT to the variable NUM_LISTENERS.
  num_listeners = ibcnt;
  
  // Send the *IDN? command to each device that was found.  
  // Query the device for a matching device name.
  for (loop = 0; loop <= num_listeners; loop++) 
  {
    /*
     Send the identification query to each listen address in the
     array RESULT.  The constant NLend, defined in DECL.H, instructs
     the function Send to append a linefeed character with EOI asserted
     to the end of the message.
    */
    ::Send(m_nCardId, result[loop], "*IDN?", 5L, NLend);
    if (ibsta & ERR) 
      return(1);

    /*
     Read the name identification response returned from each device.  
     Store the response in the array szInstrument.  The constant STOPend,
     defined in WINDECL.H, instructs the function Receive to terminate
     the read when END is detected.
    */
    ::Receive(m_nCardId, result[loop], szInstrument, 50L, STOPend);
    if (ibsta & ERR) 
      return(1);
    
    /*
     Determine if the received device name is the requested device
     identifier.
    */
    szInstrument[ibcnt] = '\0'; 
    TRACE("\tCWin16Gpib::FindDevice : szInstrument=%s\n", szInstrument);              
    if (::strncmp(szInstrument, pszIDN, ::lstrlen(pszIDN)) == 0)
    {
      *pAddr4882_t = result[loop];
      break;
    }    
  }      /*  End of FOR loop */
   
  if ((loop >= num_listeners) || (ibsta & ERR))
    return(1);              
    
  return(0);
}





// ** This function may be called by another class which implements the
// ** IGpibViewSocket interface.  The function sets the m_pIGpibViewPlug
// ** if the corresponding function parameter is not NULL and returns
// ** the previous pointer.  This allows the new plug to use the previous
// ** pointer in a chaining fashion if required.  That is, new plugs can
// ** attach themselves in a stack-like list of plug-in's.  Each attached
// ** plug-in is responsible for passing callback's on to the next
// ** plug-in in the stack.  This approach can be dangerous if any 
// ** plug-in in the call stack is deleted from memory.  Additionally,
// ** any call to DetachGpibViewPlug() detaches all plug-in's from the
// ** CWin16Gpib class. 
IGpibViewSocket *CWin16Gpib::AttachGpibViewPlug(IGpibViewSocket *pIGpibViewPlug)
{
  IGpibViewSocket *pTemp = m_pIGpibViewPlug;
   
  if (pIGpibViewPlug != NULL) 
    m_pIGpibViewPlug = pIGpibViewPlug;
     
  return(pTemp); 
} 






#endif   // DISABLE_TUNING - conditional compilation of this file


  