//*************************************************************
//  File name: CIOOPER.CPP
//
//  Description:  
//      Implementation file for the CIoOperation class.
//      
//      
//
//  History:    Date       Author               Comment
//              3/26/96    P. Pennington        Created
//
// Written by Motorola AMSD, PMU Engineering.
// Legacy code for reference only.
//*************************************************************

#include "StdAfx.h"
#include "..\pps.h"
#include "..\w16comm\w16comm.h"
#include "ciooper.h" 
#include "cpager.h"
 
// external references 
extern CPpsApp    theApp;


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CIoOperation, CObject)


#ifdef _DEBUG 
void CIoOperation::AssertValid() const
{
    CObject::AssertValid(); 
    ASSERT((pCurCmd->nCommand == IO_READ) ||
           (pCurCmd->nCommand == IO_WRITE) ||
           (pCurCmd->nCommand == IO_REPEATED_WRITE) ||
           (pCurCmd->nCommand == IO_CALLBACK) ||
           (pCurCmd->nCommand == IO_NOOP));
    ASSERT(nNumCmds > 0);
    ASSERT(nIOOPid != 0);
    ASSERT(nNumTicks >= 0);
    ASSERT((nCountDown >= 0) && (nCountDown <= nNumTicks));
}

void CIoOperation::Dump(CDumpContext& dc) const
{
  int lcv;
  CObject::Dump(dc); 
  dc << "\n\tpCmds="      << pCmds     << "\n"
     << "\tpCurCmd="      << pCurCmd   << "\n"
     << "\tnNumCmds="     << nNumCmds    << "\n"
     << "\tnIOOPid="      << nIOOPid   << "\n"
     << "\tnNumTicks="    << nNumTicks   << "\n"
     << "\tnCountDown="   << nCountDown;
  for(lcv=0; lcv < nNumCmds; ++lcv) 
  {
    dc << "\n\t\tpCurCmd[" << lcv << "]= " << (pCurCmd[lcv]).nCommand 
       << " : " << (pCurCmd[lcv]).pData << " : " << (pCurCmd[lcv]).nBytes;
  }
}
#endif


//////////////////////////////////////////////////////////////////////////////
//	BeginOperation()
//
//  This function is called any time after the CIoOperation object has been
//  constructed.  The function calls the StartTimer function to ensure
//  the IoCommand sequence times-out appropriately.  Afterwards, the
//  IoCommands are executed via the DoIoCommandLoop(). 
int CIoOperation::BeginOperation(void)
{
  TRACE("\nCIoOperation::BeginOperation : \n");
  #ifdef _DEBUG
    Dump(afxDump);
    AssertValid();
  #endif
  
  nAccumulatedRead = 0;
  pCurCmd = pCmds; 
  
  if (StartTimer())
    return(EndOperation(IO_FAILED));
  
  return(DoIoCommandLoop());
} 



///////////////////////////////////////////////////////////////////////////
//  StartTimer()
//
//  This function provides a centralized timer initiation function in which
//  the user is notified of any failure to acquire a system timer.  The
//  timer is identified by the IoOperation nIOOPid parameter.
int CIoOperation::StartTimer(void)
{
  TRACE("\nCIoOperation::StartTimer : nIOOPid = %04X : nNumTicks=%d\n", nIOOPid, nNumTicks);
    
  // if no time-out associated with the IoOperation ignore request to
  // start timer.  
  if (!(nNumTicks))
    return(1);
    
  if (theApp.m_pMainWnd->SetTimer(nIOOPid, TIMETICK, NULL) == 0)
  {
     AfxMessageBox("ERROR : Failed to acquire system timer.\n"
                   "Exit this and other windows applications. Then, restart this application.",
                    MB_ICONINFORMATION | MB_APPLMODAL | MB_OK);
     return(1);    
  }
  nCountDown = nNumTicks; 
  return(0);
}
                 


////////////////////////////////////////////////////////////////////////
//  OnTimerEvent()
//
//  This function receives the WM_TIMER event from CIoController::OnTimer()
//  function.  The purpose is to decrement the IoOperation nCountDown
//  counter as a IoOperation time-out check.  Additionally, if the 
//  current IoCommand is of type IO_REPEATED_WRITE, then the IoCommand
//  will be repeated at each WM_TIMER event.  This is the method of
//  issuing the periodic HANDSHAKE protocol function.  
//  This function returns 0L if the event is handled herein, 1L otherwise.
LONG CIoOperation::OnTimerEvent(UINT nIdEvent, BOOL bOverrideRepeat)
{ 
  TRACE("\nCIoOperation::OnTimerEvent : nIOOPid = %04X"
          "  nIdEvent=%04X  nCountDown=%d  bOverrideRepeat=%d\n", 
           nIOOPid, nIdEvent, nCountDown, bOverrideRepeat);

  // Old message?  Ignore.
  if (nIdEvent != nIOOPid)
  { 
    TRACE("\nCIoOperation::OnTimerEvent : Unknown Timer?\n");
    return(1L);   // this event not handled herein...
  } 
  
  if ((bThisIoOpCompleted) ||
      (pCurCmd >= &pCmds[nNumCmds]))   
  { 
    TRACE("\nCIoOperation::OnTimerEvent : Stale Timer?\n");
    return(0L);   // stale timer event?
  } 
                                            
  // Has the current IoOperation timed-out? 
  nCountDown -= 1;
  if (nCountDown <= 0)
  { 
    if ((pCurCmd->nCommand == IO_NOOP) && (pCurCmd->nBytes == 0)) 
      EndOperation(IO_COMPLETED); // IO_NOOP cmd timeout may be a success 
    else 
      EndOperation(IO_FAILED);    // any other cmd timeout is a failure
  } 

  // IO_REPEATED_WRITE IoCommand is repeated upon timer events.  The amount
  // of processing during this operation must be limited in order not to 
  // interfer with WM_TIMER operations.  The bOverrideRepeat parameter may
  // be used to override the repeating process.  This may be useful if data
  // is known to have arrived and the repeat should not occur.
  else if ((pCurCmd->nCommand == IO_REPEATED_WRITE) && (bOverrideRepeat == FALSE))
  { 
    if (DoIoWrite())
      EndOperation(IO_FAILED);
  }

  // if the current command is a NOOP command, decrement the NoOp counter
  // until specified delay is met.  If delay was specified as 0, then the
  // command times-out upon the CIoOperation timeout above.  Note the use
  // of WM_COMMNOTIFY to force further CIoOperation execution via the 
  // OnCommEvent() function.  Since processing should be limited in 
  // response to WM_TIMER messages, this technique allows the execution
  // of a chain of IoCommands to proceed without time constraints.  The
  // WM_COMMNOTIFY message is simply used as an event to continue IoCommand
  // processing after termination of this WM_TIMER execution thread.
  if ((pCurCmd->nCommand == IO_NOOP) && (pCurCmd->nBytes != 0))
  {
    nNoOpTickCounter -= 1;
    if (nNoOpTickCounter <= 0)
      theApp.m_pMainWnd->PostMessage(WM_COMMNOTIFY, (WPARAM)GetChannelId(), CN_POLL); 
  }

  // *** NOTE: Be sure to limit the processing invoked during a WM_TIMER event.
  // *** The system or application will lock-up if execution isn't returned
  // *** to the system in a timely manner. 
    
  return(0L);   // event handled   
}


///////////////////////////////////////////////////////////////////////
//  OnCommEvent
//
//  This function receives the WM_COMMNOTIFY message from 
//  thePager::OnCommNofity() function. The message indicates that data
//  has been received from the pager and is available in the serial 
//  port receive buffer.  The function first skips any IO_OPT_REPEAT optioned
//  IoCommand in process and then loops to process subsequent IoCommands.
//  The loop exits when no more data is present or the IoCommand sequence
//  completes.  Returns 0L if event handled herein, 1L otherwise.
LONG CIoOperation::OnCommEvent(void) 
{
  TRACE("\nCIoOperation::OnCommEvent :\n");

  // more data may be in receive buffer but next operation not yet
  // in progress.  ignore comm event and let initiation of 
  // next operation read the received data.
  if ((bThisIoOpCompleted) || (pCurCmd >= &pCmds[nNumCmds]))         
    return(0L);

  // Proceed to next command if in a NOOP state.
  if ((pCurCmd->nCommand == IO_NOOP) || (pCurCmd->nCommand == IO_REPEATED_WRITE))
    pCurCmd++;
  
  DoIoCommandLoop(); 
  return(0L);       // event handled
} 



/////////////////////////////////////////////////////////////////////
//  DoIoCommandLoop
//
//  Process IoCommands in the pCmds array until all the commands 
//  complete or completion of a command depends upon another event
//  or a command fails.  Returns 1 on error, 0 otherwise.  
int CIoOperation::DoIoCommandLoop(void)
{
  TRACE("\nCIoOperation::DoIoCommandLoop : Entering\n");

  // iterate thru the IoCommand array for this
  // IoOperation until all IoCommands are completed, or
  // an IoCommand fails, or when completion of the IoCommand
  // depends upon a subsequent TimerEvent or CommEvent.
  while (pCurCmd < &pCmds[nNumCmds])
  {
    #ifdef _DEBUG
      AssertValid();
    #endif
          
    // IO_NOOP completes on TimerEvent only.  The IoCommand nBytes
    // field is used to indicate how may ticks to delay.  If the value
    // is zero, then the delay will last until the CIoOperation 
    // times-out.  Any instructions after the IO_NOOP command will not
    // be executed in that case.  
    if (pCurCmd->nCommand == IO_NOOP)
    {
      TRACE("\nCIoOperation::DoIoCommandLoop : nCommand=IO_NOOP\n"); 
      nNoOpTickCounter = pCurCmd->nBytes;
      return(0);
    }
      
    // IO_WRITE completes in this loop.
    else if (pCurCmd->nCommand == IO_WRITE)
    { 
      TRACE("\nCIoOperation::DoIoCommandLoop : nCommand=IO_WRITE\n");
      if (DoIoWrite()) 
        return(EndOperation(IO_FAILED));
    }
    
    // IO_REPEATED_WRITE completes on comm event only and are repeated
    // upon each timer tick until the comm event.
    else if (pCurCmd->nCommand == IO_REPEATED_WRITE)
    { 
      TRACE("\nCIoOperation::DoIoCommandLoop : nCommand=IO_REPEATED_WRITE\n");
      if (DoIoWrite()) 
        return(EndOperation(IO_FAILED));
      return(0);
    }

    // IO_READ may or may not complete in this loop
    else if (pCurCmd->nCommand == IO_READ) 
    { 
      TRACE("\nCIoOperation::DoIoCommandLoop : nCommand=IO_READ\n");
      // Note: a read command will probably not find all
      // of its data in the serial port input buffers initially.
      // This is due to the fact that serial IO is relatively
      // slow compared to the execution of this program.  
      // Consequently, a static accumulator is used to 
      // incremently fill the READ IoCommand pData buffer.  WM_COMMNOTIFY
      // messages will be generated at each filling of of the
      // serial port input buffer.  It is important to continuously
      // read the input queue until no more data is detected since
      // the COM.DRV may not reliably re-issue WM_COMMNOTIFY messages. 
      int nActualRead;
      do
      { 
        nActualRead = 0;
        if (DoIoRead(&nActualRead, nAccumulatedRead))
          return(EndOperation(IO_FAILED));
        nAccumulatedRead += nActualRead;
      }while((nAccumulatedRead < pCurCmd->nBytes) && (nActualRead)); 
      // not all expected data arrived, wait until next WM_COMMNOTIFY
      if (nAccumulatedRead < pCurCmd->nBytes)
        return(0);
    }
    
    //  IO_CALLBACK performs intermediate call to the ICallBack derived
    //  class OnCallBack() function.  The nBytes field of this
    //  IoCommand is used as the nCallBackId parameter.  
    //  This IoCommand completes in this loop.
    else if (pCurCmd->nCommand == IO_CALLBACK)
    {
      TRACE("\nCIoOperation::DoIoCommandLoop : nCommand=IO_CALLBACK\n"); 
      ASSERT(pICallBack != NULL);  
      if (pICallBack->OnIoOperationCallBack(pCurCmd->nBytes)) 
        return(EndOperation(IO_FAILED));
    }
   
    // advance to next command 
    pCurCmd++;
    nAccumulatedRead = 0;
  }  // end while
  
  // all IoCommands completed
  return(EndOperation(IO_COMPLETED));
}

         



/////////////////////////////////////////////////////////////////////////
//  EndOperation()
//
//  This function is called upon completion of all the IoCommands in this
//  IoOperation.  Also, it is called upon a failure in the execution of
//  any IoCommand in the IoCommand sequence.  The function posts a 
//  WM_IO_OPERATION message indicating the IoOperation status.  The
//  CIoController::OnIoOpComplete member function then makes a callback
//  to the IoOperation owner.
int CIoOperation::EndOperation(UINT nStatus)
{
  if (bThisIoOpCompleted)   // IoOp already completed
    return(nStatus);

  TRACE("CIoOperation::EndOperation : nStatus=%04X nIOOPid=%04X nCountDown=%d\n", 
         nStatus, nIOOPid, nCountDown);
    
  if (theApp.m_pMainWnd->KillTimer(nIOOPid) == 0)  // failed to kill timer
    AfxMessageBox("ERROR : Failed to stop IoOperation timer.\n"
                  "Exit and restart this application.",
                  MB_ICONINFORMATION | MB_APPLMODAL | MB_OK);

  bThisIoOpCompleted = TRUE;    // mark this CIoOperation as completed   
  
  // Post a completion message in the applications message queue.
  // NOTE:  Do not use a completion callback.  Since this function
  // may be called from the OnTimer function above, it must return
  // quickly and a callback mechanism might take too long. 
  UINT nChannelId = GetChannelId();
  LPARAM lParam = MAKELONG(nIOOPid, nChannelId);
  theApp.m_pMainWnd->PostMessage(WM_IO_OPERATION, (WPARAM)nStatus, lParam); 
  return(nStatus);
}



static char *szIoCommandNames[] = { "UNKNOWN", 
									"IO_READ", 
									"IO_WRITE", 
									"IO_CALLBACK", 
									"IO_NOOP"
									"IO_REPEATED_WRITE" };
const char *CIoOperation::GetCurrentCmdName(void)
{ 
  if ((pCurCmd >= &pCmds[nNumCmds]) || (pCurCmd == NULL))
    return(szIoCommandNames[0]);
        
  if (pCurCmd->nCommand == IO_READ) 
    return(szIoCommandNames[1]);
  else if (pCurCmd->nCommand == IO_WRITE)   
    return(szIoCommandNames[2]);
  else if (pCurCmd->nCommand == IO_CALLBACK)
    return(szIoCommandNames[3]);
  else if (pCurCmd->nCommand == IO_NOOP)
    return(szIoCommandNames[4]);
  else if (pCurCmd->nCommand == IO_REPEATED_WRITE)
    return(szIoCommandNames[5]);
  else
    return(szIoCommandNames[0]);
}

 



///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//  CSerialIoOperation derived class specific member functions
//

IMPLEMENT_DYNAMIC(CSerialIoOperation, CIoOperation)

#ifdef _DEBUG 
void CSerialIoOperation::AssertValid() const
{
    CObject::AssertValid();
    CIoOperation::AssertValid();
}

void CSerialIoOperation::Dump(CDumpContext& dc) const
{
  CObject::Dump(dc); 
  CIoOperation::Dump(dc);
}
#endif


//////////////////////////////////////////////////////////////////////////
//  CSerialIoOperation::GetChannelId()
//
//  Return the device specific identifier..
//
UINT CSerialIoOperation::GetChannelId(void)
{
  return((UINT)m_pCWin16Comm->GetDeviceId());
}   


////////////////////////////////////////////////////////////////////////
//  CSerialIoOperation::OnTimerEvent()
//
//  The purpose is to decrement the IoOperation nCountDown
//  counter as a IoOperation time-out check.  Additionally, if the 
//  current IoCommand is of type IO_REPEATED_WRITE, then the IoCommand
//  will be repeated at each WM_TIMER event.
LONG CSerialIoOperation::OnTimerEvent(UINT nIdEvent, BOOL bPoll)
{ 
  TRACE("\nCSerialIoOperation::OnTimerEvent : nIOOPid = %04X"
          "  nIdEvent=%04X  nCountDown=%d\n", 
           nIOOPid, nIdEvent, nCountDown);
    
  // Query the serial port.  If CN_POLL WM_COMMNOTIFY event notification is
  // enabled, a CN_POLL event will be issued.  If data has arrived and the
  // IO_REPEATED_WRITE command is active, the poll status will stop the
  // repeating write operation in CIoOperation::OnTimerEvent().
  BOOL bNewPoll = m_pCWin16Comm->IsRxDataAvailable();

  // *** NOTE: Be sure to limit the processing invoked during a WM_TIMER event.
  // *** The system or application will lock-up if execution isn't returned
  // *** to the system in a timely manner. 
  
  return(CIoOperation::OnTimerEvent(nIdEvent, bNewPoll));
}


/////////////////////////////////////////////////////////////////////////
//  CSerialIoOperation::OnCommNotify()
//
//  Returns 0L if the event has been handled herein, 1L otherwise.
//  
LONG CSerialIoOperation::OnCommNotify(WPARAM wParam, LPARAM lParam)
{
  TRACE("\nCSerialIoOperation::OnCommNotify : wParam=%04X : lParam=%08X\n",
        wParam, lParam);

  // verify port id, is message from current port?
  if (wParam != GetChannelId()) 
    return(1L);

  WORD event = LOWORD(lParam); 
  UINT nSpEvent;
  
  // is port in break status or other event notification
  if (event & CN_EVENT)
  {
    nSpEvent = m_pCWin16Comm->GetCommEvent();  // reset event notification
    //
    TRACE("\tCSerialIoOperation::OnCommNotify : CN_EVENT : %04X\n", nSpEvent); 
    //
    if (nSpEvent & EV_RXCHAR) 
      OnCommEvent(); 
    else if (nSpEvent & EV_ERR)  // error event
    {
      m_pCWin16Comm->ProcessCommError();
      return(0L); 
    }
    else  // unknown event.  See windows.h for all possible events.
      return(1L); 
  }
  
  // is this a poll notification of received data
  else if (event & CN_POLL)
  {
    TRACE("\tCSerialIoOperation::OnCommNotify : CN_POLL\n");
    OnCommEvent();
  }

  // is event a received data notification?
  else if (event & CN_RECEIVE)
  {
    TRACE("\tCSerialIoOperation::OnCommNotify : CN_RECEIVE\n"); 
    OnCommEvent(); 
  }
    
  // is transmit queue empty - ignore
  else if (event & CN_TRANSMIT)
    return(0L);
   
  else   // unknown event
    return(1L); 

  return(0L);  // event handled
}



/////////////////////////////////////////////////////////////////////////
//  CSerialIoOperation::DoIoWrite()
//
//  This function serves to encapsulate the interaction of the 
//  CIoOperation class with theSerialPort object.  All the data
//  transfers may be viewed on the debug window.  Returns 1 on 
//  error, 0 otherwise.
int CSerialIoOperation::DoIoWrite(void)
{
  TRACE("\nCSerialIoOperation::DoIoWrite : nBytes=%d\n", pCurCmd->nBytes);
  #ifdef _DEBUG
    for (int lcv = 0; lcv < pCurCmd->nBytes; ++lcv)
    {
      TRACE("%02X ", *(pCurCmd->pData+lcv));
      if (((lcv + 1) % 16) == 0)
        TRACE("\n");
    }
    TRACE("\n");
  #endif    
  
  if (m_pCWin16Comm->SendCommBytes(pCurCmd->pData, pCurCmd->nBytes)) 
  {
    m_pCWin16Comm->ProcessCommError();  
    return(1);
  } 
  
  return(0);
}


/////////////////////////////////////////////////////////////////////////
//  CSerialIoOperation::DoIoRead()
//
//  This function serves to encapsulate the interaction of the 
//  CIoOperation class with theSerialPort object.  All the data
//  transfers may be viewed on the debug window.  The nOffset
//  parameter allows the IO_READ IoCommand pData buffer to be
//  filled incrementally over sequential WM_COMMNOTIFY events. 
//  Returns 1 on error, 0 otherwise.
int CSerialIoOperation::DoIoRead(int *nActualRead, int nOffset)
{                                       
  TRACE("\nCSerialIoOperation::DoIoRead : nBytes=%d\n", pCurCmd->nBytes);
    
  // has data already arrived for this command?  
  if ((pCurCmd->nBytes - nOffset) == 0) 
  {
    *nActualRead = 0;
    return(0);
  }
    
  // Read data from the comm port receive queue.  
  if (m_pCWin16Comm->ReadCommBytes(pCurCmd->nBytes - nOffset, 
                                  nActualRead, 
                                  pCurCmd->pData + nOffset))
  {
    m_pCWin16Comm->ProcessCommError();
    return(1);
  }
   
  #ifdef _DEBUG
    for (int lcv = 0; lcv < *nActualRead; ++lcv)
    {
      TRACE("%02X ", *(pCurCmd->pData+nOffset+lcv));
      if (((lcv + 1) % 16) == 0)
        TRACE("\n");
    }
  #endif    
  TRACE("\nCIoOperation::DoIoRead : nActualRead=%d  "
        "nAccumulatedRead=%d\n", *nActualRead, 
         (nAccumulatedRead+(*nActualRead)));
  return(0);
}  


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
 


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//  CGpibIoOperation derived class specific member functions
//
#ifndef DISABLE_TUNING

IMPLEMENT_DYNAMIC(CGpibIoOperation, CIoOperation)

#ifdef _DEBUG 
void CGpibIoOperation::AssertValid() const
{
    CObject::AssertValid();
    CIoOperation::AssertValid();
}

void CGpibIoOperation::Dump(CDumpContext& dc) const
{
  CObject::Dump(dc); 
  CIoOperation::Dump(dc);
}
#endif


//////////////////////////////////////////////////////////////////////////
//  GetChannelId()
//
//  Return the device specific identifier..
//
UINT CGpibIoOperation::GetChannelId(void)
{
  return((UINT)m_pCWin16Gpib->GetCardId());
}   


/////////////////////////////////////////////////////////////////////////
//  CGpibIoOperation::DoIoWrite()
//
//  This function serves to encapsulate the interaction of the 
//  CIoOperation class with theGpibPort object.  All the data
//  transfers may be viewed on the debug window.  Returns 1 on error,
//  0 otherwise.
int CGpibIoOperation::DoIoWrite(void)
{
  TRACE("\nCGpibIoOperation::DoIoWrite : nBytes=%d\n", pCurCmd->nBytes);
  #ifdef _DEBUG
    for (int lcv = 0; lcv < pCurCmd->nBytes; ++lcv)
    {
      TRACE("%02X ", *(pCurCmd->pData+lcv));
      if (((lcv + 1) % 16) == 0)
        TRACE("\n");
    }
    TRACE("\n");
  #endif    
  
  // NOTE:  The asynchronous read/write capability of the gpib.lib 
  // (i.e. ibrda, ibwrta) are not used herein.  These functions
  // may serve this application better than the synchronous
  // calls made herein.  If implemented, corresponding changes
  // will be required in files 'w16gpib.h' and 'w16gpib.cpp'.

  if (m_pCWin16Gpib->Write(m_nDvm, (char *)pCurCmd->pData, (long)pCurCmd->nBytes)) 
  {
    return(1);
  } 
  
  return(0);
}


/////////////////////////////////////////////////////////////////////////
//  CGpibIoOperation::DoIoRead()
//
//  This function serves to encapsulate the interaction of the 
//  CIoOperation class with theGpibPort object.  All the data
//  transfers may be viewed on the debug window.  The nOffset
//  parameter allows the IO_READ IoCommand pData buffer to be
//  filled incrementally over sequential Gpib events.
//  Returns 1 on error, 0 otherwise.  
int CGpibIoOperation::DoIoRead(int *nActualRead, int nOffset)
{                                       
  TRACE("\nCGpibIoOperation::DoIoRead : nBytes=%d\n", pCurCmd->nBytes);
    
  // has data already arrived for this command?  
  if ((pCurCmd->nBytes - nOffset) == 0) 
  {
    *nActualRead = 0;
    return(0);
  }
  
  // NOTE:  The asynchronous read/write capability of the gpib.lib 
  // (i.e. ibrda, ibwrta) are not used herein.  These functions
  // may serve this application better than the synchronous
  // calls made herein.  If implemented, corresponding changes
  // will be required in files 'w16gpib.h' and 'w16gpib.cpp'.
   
  // Read data from the gpib port.  
  if (m_pCWin16Gpib->Read(m_nDvm, pCurCmd->pData + nOffset, (long)pCurCmd->nBytes - nOffset))
  {
    return(1);
  }
  
  *nActualRead = (int)m_pCWin16Gpib->GetLastCount(); 
  
  #ifdef _DEBUG
    for (int lcv = 0; lcv < *nActualRead; ++lcv)
    {
      TRACE("%02X ", *(pCurCmd->pData+nOffset+lcv));
      if (((lcv + 1) % 16) == 0)
        TRACE("\n");
    }
  #endif    
  TRACE("\nCGpibIoOperation::DoIoRead : nActualRead=%d  "
        "nAccumulatedRead=%d\n", *nActualRead, 
         (nAccumulatedRead+(*nActualRead)));
  return(0);
}  

#endif  // DISABLE_TUNING





///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//  CIoController class member function definition... 


IMPLEMENT_DYNAMIC(CIoController, CObject)

#ifdef _DEBUG 
void CIoController::AssertValid() const
{
    CObject::AssertValid();
}

void CIoController::Dump(CDumpContext& dc) const
{
  CObject::Dump(dc); 
}
#endif



/////////////////////////////////////////////////////////////////////////
//  CIoController::AddTail()
//
//  Add a CIoOperation derived object to the IoOpList.
//
void CIoController::AddTail(CIoOperation *pCIoOperation)
{                                   
  ASSERT_VALID(pCIoOperation);
  m_IoOpList.AddTail(pCIoOperation);
  return;
}


/////////////////////////////////////////////////////////////////////////
//  CIoController::AddHead()
//
//  Add a CIoOperation derived object ot the IoOpList.
//
void CIoController::AddHead(CIoOperation *pCIoOperation) 
{                                   
  ASSERT_VALID(pCIoOperation);
  m_IoOpList.AddHead(pCIoOperation);
  return;
}



/////////////////////////////////////////////////////////////////////////
//  CIoController::ExecuteIoOpList()
//
//  The sequence of IoOperations is initiated. Returns 1 on error, 0
//  otherwise.
//
int CIoController::ExecuteIoOpList(void)
{ 
  TRACE("\nCIoController::ExecuteIoOpList\n");
  
  if (IsOperationInProgress())		// ignore simultaneous requests.  new 
    return(0);                      // request will be initiated after current.
  
  ASSERT(!(IsOpListEmpty()));
  if (IsOpListEmpty())  // error - user class should have constructed  
    return(1);          // an IoOperation list before calling here
    
  m_pIoOp = (CIoOperation *)m_IoOpList.RemoveHead();
  return(m_pIoOp->BeginOperation());
}




/////////////////////////////////////////////////////////////////////////////
//  OnTimer()
// 
//  This function is called from CMainFrame::OnTimer() upon receipt of a 
//  WM_TIMER message.  If an IoOperation is not in progress, then the 
//  timer event does not belong to this object.  
//  Otherwise, the event is passed to the current
//  IoOperation::OnTimerEvent() function for IoOperation specific 
//  processing.  Returns 0L if the event has been handled, 1L otherwise. 
LONG CIoController::OnTimer(UINT nIdEvent)
{
  TRACE("\nCIoController::OnTimer : nIDEvent=%04X\n", nIdEvent);
  
  // return if this event is not expected.
  if (IsOperationInProgress() == FALSE)
    return(1L);
  
  return(m_pIoOp->OnTimerEvent(nIdEvent));    
}




//////////////////////////////////////////////////////////////////////////////
//  OnCommNotify()
// 
//  This function is called from the CMainFrame::OnCommNotify() function.
//  Returns 0L if the event has been handled, 1L otherwise.
//
LONG CIoController::OnCommNotify(UINT wParam, LONG lParam)
{
  TRACE("\nCIoController::OnCommNotify : wParam=%04X lParamHI=%04X lParamLO=%04X\n", 
         wParam, HIWORD(lParam), LOWORD(lParam)); 

  WORD event = LOWORD(lParam); 
  
  // return if this event is not expected.
  if (IsOperationInProgress() == FALSE)
    return(1L);

  return(m_pIoOp->OnCommNotify(wParam, lParam)); 
}
  


////////////////////////////////////////////////////////////////////
//  CIoController::OnIoOpCompletion()
//
//  This function receives IoOperation completion notification from
//  the CMainFrame::OnWmIoOperation() upon completion of an IoOperation.  
//  If any verification or wrap-up processing is required, 
//  it is performed prior to deletion of the IoOperation
//  and execution of the next IoOperation.  Return 0L if event
//  handled herein, 1L otherwise.  
//    wParam = nStatus (0=IO_COMPLETED, 1=IO_FAILED, 2=IO_NOOPTIMEOUT)
//	  HIWORD(lParam) = nDeviceId
//    LOWORD(lParam) = CIoOperaiton ID 
//
LONG CIoController::OnIoOpCompletion(WPARAM wParam, LPARAM lParam)
{
  TRACE("\nCIoController::OnIoOpCompletion : wParam=%04X : lParam=%08X\n",
         wParam, lParam);
  CWnd *pCWnd = NULL;
   
  if (IsOperationInProgress() == FALSE)   // IoOperation already killed
    return(1L);
  
  UINT nStatus = wParam;
  UINT nIoOpId = LOWORD(lParam);
   
  // does IoOperation Id match current IoOpId
  ASSERT(m_pIoOp->GetIoOpId() == nIoOpId);
  if (nIoOpId != m_pIoOp->GetIoOpId())
    return(1L);
      
  // give the IoOperation owner a chance to handle the event
  if (m_pIoOp->pICallBack->OnIoOperationComplete(nStatus, nIoOpId))
  {
    EmptyIoOperationList(1);    // kill list if error return 
    nStatus = IO_FAILED;
  }

  // move on to next operation in list or end IoOperation sequence 
  if (m_pIoOp != NULL)
  {
    delete m_pIoOp;
    m_pIoOp = NULL;
  }
    
  if (m_IoOpList.IsEmpty()) 
  { 
    TRACE("\nCIoController::OnOpCompletion : m_IoOpList is empty\n");
    pCWnd = theApp.m_pMainWnd->GetActiveWindow();
    ASSERT(pCWnd != NULL);
    m_pIoOp = NULL;		// end of list
    if (pCWnd)
      pCWnd->PostMessage(WM_IO_OPLIST_DONE, nStatus, (LPARAM)nIoOpId); 
  }
  else   // initiate next IoOperation 
  {
    m_pIoOp = (CIoOperation *)m_IoOpList.RemoveHead();
    m_pIoOp->BeginOperation();
  }
  return(0L);
}



////////////////////////////////////////////////////////////////////
//  CIoController::EmptyIoOperationList()
//
//	Removes all IoOperations from the IoOpList.  This function is
//  called upon an IoOperation error. 
//
void CIoController::EmptyIoOperationList(int nStatus)
{ 
  TRACE("\nCIoController::EmptyIoOperationList\n");

  if ((IsOperationInProgress() == TRUE) || (IsOpListEmpty() == FALSE))
  { 
    // ensure disable any system timers if operation active
    if (IsOperationInProgress() == TRUE)
      m_pIoOp->EndOperation(nStatus); 
    // operation not active and oplist not empty, grab first CIoOperation
    if ((m_pIoOp == NULL) && (IsOpListEmpty() == FALSE))
      m_pIoOp = (CIoOperation *)m_IoOpList.RemoveHead();
    // delete all CIoOperations from memory. 
    while(m_pIoOp != NULL)           
    {
      delete m_pIoOp;
      m_pIoOp = NULL;
      if (!(m_IoOpList.IsEmpty()))    
        m_pIoOp = (CIoOperation *)m_IoOpList.RemoveHead();
    }
  }
  return;
}



///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
        

