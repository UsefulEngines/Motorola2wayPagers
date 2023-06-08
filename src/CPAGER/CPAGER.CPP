//*************************************************************
//  File name: CPAGER.CPP
//
//  Description:  
//      Emplementation file for the CPager class.
//      
//      
//
//  History:    Date       Author               Comment
//              3/13/96    P. Pennington        Created
//
// Written by Motorola AMSD, PMU Engineering.
// Legacy code for reference only.
//*************************************************************

#include "StdAfx.h"
#include "..\pps.h"
#include "..\mainfrm.h"
#include "..\w16comm\w16comm.h"
#include "ciooper.h"
#include "cpager.h" 
 
// external references 
extern CWin16Comm 		theSerialPort;  
extern CPpsApp    		theApp; 
extern CIoController	theIoController;


IMPLEMENT_DYNAMIC(CPager, CObject)


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif




#ifdef _DEBUG 
void CPager::AssertValid() const
{
    CObject::AssertValid();
}

void CPager::Dump(CDumpContext& dc) const
{
  CObject::Dump(dc); 
  
}
#endif


// GENERAL DEFINES FOR READABILITY
// 
#define SIZEOF(IoCmdArray)	(sizeof(IoCmdArray)/sizeof(IoCommand))
#define TICKS(y)			y    // operation times-out in (100ms * y) seconds


//  IO OPERATION IDENTIFIERS
// 
#define HANDSHAKE_OP			1 

//  IOOP CALLBACK IDENTIFIERS
//
#define IS_HANDSHAKE_ACKED		9 


//  PROTOCOL IO OPERATION COMMAND ARRAYS and DATA BUFFERS
//
static BYTE byHandShakeCMD[1] = { 0xAA };  
static BYTE byHandShakeAckBUFF[1];  
//
static IoCommand 	HandShakeIOOP[] = 
 {	
   { IO_REPEATED_WRITE, byHandShakeCMD, 1 },   // repeat until comm event or timeout
   { IO_READ, byHandShakeAckBUFF, 1 },
   { IO_CALLBACK, NULL, IS_HANDSHAKE_ACKED } 
 };
// Ex: theIoController.AddTail(new CSerialIoOperation(&theSerialPort, HandShakeIOOP, 
//  											   SIZEOF(HandShakeIOOP), 
//  										       HANDSHAKE_OP, 
//  										       TICKS(10),
//												   &m_IoCallBackInterface));



/////////////////////////////////////////////////////////////////////////
//  InitiateRead()
//
//  This function is call upon a 'Device-Read' user event.  The actual
//  device type cannot be determined until the device responds to the
//  "universal" AA-protocol handshake.  When the device responds, the
//  returned device identifier is verified with theApp, and theApp selects
//  the appropriate CPager derived class to handle the subsequent
//  protocol transactions.
int	CPager::InitiateRead(void) 
{
  TRACE("\nCPager::InitiateRead\n");
  int nMaxTime = 0;
 
  // construct the sequence of IoOperations required to initiate handshaking
  // see the definition of a CIoOperation in file 'ciooper.h' for more info.
  
  if (theIoController.IsOperationInProgress())  // don't allow reads if other 	
    return(1);                                  // action in progress
  
  theIoController.AddTail(new CSerialIoOperation(&theSerialPort, HandShakeIOOP, 
  											     SIZEOF(HandShakeIOOP), 
  										         HANDSHAKE_OP, 
  										         TICKS(10),
											     &m_IoCallBackInterface));

  nMaxTime += 1;
  m_nCurrentState |= READ_REQUESTED;
  return(CPager::ExecuteIoOpList(nMaxTime, "READ"));                                           
}



/////////////////////////////////////////////////////////////////////////
//  ExecuteIoOpList()
//
//  This class is called upon return from CPager derived class functions
//  after construction of an IoOperation list.  The sequence of IoOperations
//  is initiated.
int CPager::ExecuteIoOpList(int nTimeInSecs, const char *pszIoOp)
{ 
  TRACE("\nCPager::ExecuteIoOpList\n");
  
  ASSERT(nTimeInSecs >= 0);
  m_fTotalIoOpTime = nTimeInSecs;    // relative time for progress indicator
  m_fCompletedIoOpTime = 0;
  m_szCurrentOpString = pszIoOp;
  
  if (theSerialPort.IsPortConfigured() == FALSE)
  {
    if (ConfigurePort())
    { 
      DisConnect(); 
      return(1);
    }                
  }
  
  // If no IoOperation is in progress, the serial port may be buffering unread 
  // data.  Flush the receive queue in this case. 
  if ((theIoController.IsOperationInProgress() == FALSE) &&
      (theSerialPort.IsRxDataAvailable() == TRUE))
    theSerialPort.HandleUnsolicitedData(); 
    
  return(theIoController.ExecuteIoOpList());
}



///////////////////////////////////////////////////////////////////////////
//  IsOperationInProgress()
//
BOOL CPager::IsOperationInProgress(void) 
{ 
  return(theIoController.IsOperationInProgress()); 
}




////////////////////////////////////////////////////////////////////
//  OnOpCompletion()
//
//  This function receives IoOperation completion notification from
//  the CPager derived class upon completion of an IoOperation. 
// 
int CPager::OnOpCompletion(UINT nStatus, UINT nIoOpId)
{
  TRACE("\nCPager::OnOpCompletion : nStatus=%04X : nIoOpId=%X\n", nStatus, nIoOpId);

  if (nStatus == IO_FAILED)    // indicates IoOperation failed
  {                          // notify the currently active window
    OnOpFailure("\nCurrent serial port operation failed.  "
                "First check all connections as well as the port selection.  "
                "Press F1 for additional help.\n");
  } 
  
  CSerialIoOperation *pIoOp = (CSerialIoOperation *)theIoController.GetCurrentIoOperation();
  ASSERT(pIoOp != NULL);
  
  // update the relative timespan data for the status bar progress indicator                     
  m_fCompletedIoOpTime += pIoOp->GetTotalTimeInSeconds();    
                                             
  return(nStatus);
}



///////////////////////////////////////////////////////////////////////
// GetPercentComplete()
//
int CPager::GetPercentComplete(void)
{ 
  CSerialIoOperation *pIoOp = (CSerialIoOperation *)theIoController.GetCurrentIoOperation();

  if ((m_fTotalIoOpTime <= 0) || (pIoOp == NULL))
    return(0);
  int nProgress = (int)((m_fCompletedIoOpTime + pIoOp->GetElapsedTimeInSeconds()) / m_fTotalIoOpTime * 100.0F);
  //TRACE("\nCPager::GetPercentComplete : %d\n", nProgress);
  return(nProgress);
}



////////////////////////////////////////////////////////////////////
//  OnOpFailure()
//
//  Provides a central function for notifying the user of a
//  application/pager interface failure.
//
void CPager::OnOpFailure(const char *pszFailString)
{ 
  TRACE("\nCPager::OnOpFailure : %s\n", pszFailString);
    
  m_fTotalIoOpTime = 0;
  m_fCompletedIoOpTime = 0; 
  theSerialPort.ResetComms(); 
  theSerialPort.ResumeComms();
  m_nCurrentState = NO_STATE;
  
  if (m_szDisplayOnFailure.IsEmpty())       // if error string empty, show default
    m_szDisplayOnFailure += pszFailString;
  DWORD dwHelpId = HID_BASE_RESOURCE + IDP_SERIAL_PORT_ERROR;    
  AfxMessageBox(m_szDisplayOnFailure, 
                MB_ICONINFORMATION | MB_APPLMODAL | MB_OK,
                (UINT)dwHelpId); 
  m_szDisplayOnFailure.Empty();
  return;
}


////////////////////////////////////////////////////////////////////////////
//  ConfigurePort()
// 
//  The default (first available) serial port is automatically
//  configured unless the user has configured another port.
//     
int CPager::ConfigurePort()
{
  TRACE("\nCPager::ConfigurePort\n");

  if (theSerialPort.IsPortConfigured() == FALSE)
  {
     if (theSerialPort.FindFirstAvailPort()) 
     {
       theSerialPort.ProcessCommError();
       return(1);
     }
  }  
  // enable the port for generation of WM_COMMNOTIFY messages
  if (theSerialPort.EnableComms())
  {
    theSerialPort.ProcessCommError();
    return(1);
  }      
  return(0);
}




/////////////////////////////////////////////////////////////////////
//  DisConnect()
//
//  This function closes the serial port and is called automatically
//  via the ~CPager destructor.
//
int CPager::DisConnect()
{
  TRACE("\nCPager::DisConnect\n");

  theIoController.EmptyIoOperationList();   // quit any IoOperation

  if (theSerialPort.IsPortConfigured() == TRUE)  // close the port
  {
     if (theSerialPort.CloseCommPort(FLUSH)) 
     {
       theSerialPort.ProcessCommError();
       return(1);
     }
  }
  
  m_nCurrentState = NO_STATE; 
  return(0);
}





/////////////////////////////////////////////////////////////////////
//  IoCallBackInterface::OnOpCallBack()
//
//  This function recieves a callback from the currently executing
//  IoOperation when an IO_CALLBACK command is encountered.  The
//  nCallBackId parameter indicates the reason for the callback
//  which may be an intermediate verification of data received in
//  a previous IoCommand.  The function returns 0 on success or
//  a 1 on error.  If 1 is returned, then the IoOperation ends
//  with a FAILED status and a WM_USER_IO_DONE message is 
//  generated.  Derived CPager based classes should override this
//  function as necessary.  Note that the function should not 
//  delete the IoOperation or otherwise attempt to terminate the
//  IoOperation since this would tightly couple the heap allocated
//  IoOperation to the CPager based class.  Allow the IoOperation to
//  end via the OnOpCompletion messaging sequence.
int CPager::IoCallBackInterface::OnIoOperationCallBack(int nCallBackId)
{
  TRACE("\nCPager::IoCallBackInterface::OnIoOperationCallBack : nCallBackId=%d", nCallBackId);
  METHOD_PROLOGUE(CPager, m_IoCallBackInterface)  // makes pThis (see 'ciooper.h')
  
  switch(nCallBackId)
  {  
    case IS_HANDSHAKE_ACKED:
      if (pThis->HandleIsHandshakeAcked())
        return(1);
      break;
        
    default:
      ASSERT(0);
      break;
  }
  return(0);
}



////////////////////////////////////////////////////////////////////
//  IoCallBackInterface::OnIoOperationComplete()
//
//  This function receives status notification from
//  the current CIoController::OnIoOpCompletion() function 
//  completion of the current IoOperation.  The function returns 0 
//  on success and a 1 on error.  If 1 is returned, then the current
//  list of IoOperations is deleted by the CIoController and a
//  WM_IO_OPLIST_DONE failed message is generated. 
//  
int CPager::IoCallBackInterface::OnIoOperationComplete(UINT nStatus, UINT nIoOpId) 
{
  TRACE("\nCPager::IoCallBackInterface::OnIoOperationComplete : nStatus=%04X : "
          "nIoOpId=%04X\n", nStatus, nIoOpId);

  METHOD_PROLOGUE(CPager, m_IoCallBackInterface)  // makes pThis (see 'ciooper.h')
  
  // handle op completion based on IoOperation nIOOPid indicator
  switch(nIoOpId)
  {
    case HANDSHAKE_OP:
      TRACE("\tIoOperation = HANDSHAKE_OP\n");
      if (nStatus)  // Operation Failed ???
        pThis->m_szDisplayOnFailure += "\nDevice not responding.\n"
                                       "Press F1 for Help.\n";         
      break;
      
    default:	
      // move on to next IoOperation in sequence
      break;
  }
  
  return(pThis->OnOpCompletion(nStatus, nIoOpId)); 
}


 
 
/////////////////////////////////////////////////////////////////////
//  Handle...() 
// 


int CPager::HandleIsHandshakeAcked(void)
{
  TRACE("\nCPager::HandleIsHandshakeAcked\n");
  
  if (theApp.IsPagerSupported(byHandShakeAckBUFF[0]) == FALSE)
  {
    char szTemp[80];
    sprintf(szTemp, "\nUnsupported device ID (0x%02X) received.\n", 
            byHandShakeAckBUFF[0]);
    m_szDisplayOnFailure += szTemp;   
    return(1);      // unsuccessful, abort read operation
  }
  
  // valid pager ID
  m_byPagerId = byHandShakeAckBUFF[0]; 
  theApp.SelectThePager(m_byPagerId);   
  return(theApp.GetThePager()->ReadCodePlug());  // continue read operation via derived class
}


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////





