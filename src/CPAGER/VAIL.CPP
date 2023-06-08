//*************************************************************
//  File name: vail.cpp
//
//  Description:  
//      Emplementation file for the CVail class.
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
#include "..\mainfrm.h"
#include "..\ppsdoc.h"
#include "ciooper.h"
#include "pegasus.h"
#include "vail.h" 
#include "codeplug.h" 
#include "..\w16comm\w16comm.h"
 
// external references
extern CPpsApp    	    theApp;
extern CWin16Comm	    theSerialPort;
extern CIoController	theIoController;
extern BYTE 			byHandShakeAckBUFF[1];  // declared in CPegasus.cpp

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif




IMPLEMENT_DYNAMIC(CVail, CObject)

#ifdef _DEBUG 
void CVail::AssertValid() const
{
    CObject::AssertValid();
}

void CVail::Dump(CDumpContext& dc) const
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
#define HANDSHAKE_OP		1
 

//  IOOP CALLBACK IDENTIFIERS
//
#define IS_HANDSHAKE_ACKED	9 





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
int CVail::IoCallBackInterface::OnIoOperationCallBack(int nCallBackId)
{
  TRACE("\nCVail::IoCallBackInterface::OnIoOperationCallBack : nCallBackId=%d", nCallBackId);
  METHOD_PROLOGUE(CVail, m_IoCallBackInterface)  // makes pThis (see 'ciooper.h')
  
  // Handle any CVail specific IoOperation functionality herein.
  // Otherwise, pass the event upon the derivation hierarchy.
  
  // handle CVail specific IoOp callbacks based on IoOperation nCallBackId indicator
  switch(nCallBackId)
  {  
    case IS_HANDSHAKE_ACKED:
      if (pThis->HandleIsHandshakeAcked())
        return(1);
      break;

    //*** Add any CVail specific cases here...
        
    default:
      ASSERT(0);   // Bad Code.  All call-backs defined should be recognized.
      return(1);
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
int CVail::IoCallBackInterface::OnIoOperationComplete(UINT nStatus, UINT nIoOpId) 
{
  TRACE("\nCVail::IoCallBackInterface::OnIoOperationComplete : nStatus=%04X : "
          "nIoOpId=%04X\n", nStatus, nIoOpId);

  METHOD_PROLOGUE(CVail, m_IoCallBackInterface)  // makes pThis (see 'ciooper.h')

  // Handle any CVail specific IoOperation functionality herein.
  // Otherwise, pass the event upon the derivation hierarchy.
  
  // handle CVail specific IoOp completion based on IoOperation nIOOPid indicator
  switch(nIoOpId)
  {
   
    //*** Add any CVail specific cases here...
    
    case HANDSHAKE_OP:    // fall thru for now...
    default:	
      // move on to next IoOperation in sequence
      break;
  }
  
  return(pThis->OnOpCompletion(nStatus, nIoOpId)); // return via base class
}



 
/////////////////////////////////////////////////////////////////////
//  Handle...() 
// 

int CVail::HandleIsHandshakeAcked(void)
{
  TRACE("\nCVail::HandleIsHandshakeAcked\n");
  
  // if OK  
  if (byHandShakeAckBUFF[0] == VAIL)   
  { 
    m_byPagerId = byHandShakeAckBUFF[0];
    return(0);
  }
    
  char szTemp[80];
  sprintf(szTemp, "\nUnexpected device ID (0x%02X) received.\n", 
          byHandShakeAckBUFF[0]);
  m_szDisplayOnFailure += szTemp;
   
  return(1);
}


 
////////////////////////////////////////////////////////////////////
// GetPagerIdBlockInfo()
// 
// Builds an ASCII string of the pager info block. 
//   
void CVail::GetPagerIdBlockInfo(CString& rString)
{ 
  char  cBuffer[30];
  BYTE	byTemp;
  
  // clear the current string contents
  rString.Empty();
  
  if (!(GetPagerID(&byTemp)))
  {
    sprintf(cBuffer, "DeviceID=%02X", byTemp);
    rString += cBuffer;
    rString += "    ";
  }
  
  if (!(GetPagerType(&byTemp)))
  {
    rString += "Type="; 
    if (byTemp == 'A')        
      rString += "Reflex25";  
    else if (byTemp == 'M')
      rString += "Reflex50";
    else
    {
      sprintf(cBuffer, "%c", (char)byTemp);
      rString += cBuffer;
    }
    rString += "    ";
  }
  
  if (!(GetPagerAPC((BYTE*)&cBuffer[0])))
  {
    cBuffer[3] = '\0';
    rString += "APC=";
    if (lstrcmp(cBuffer, "83A") == 0)   //saurin 7-7-97 Add Vail APC
      rString += "CreataLink";               //saurin 7-24-97 Display CreataLink on the
    else                                     //Inventory Screen within the Info Block Contents.
      rString += cBuffer;
    rString += "    ";
  }
  
  if (!(GetPagerDerivative((BYTE*)&cBuffer[0])))
  {
    cBuffer[3] = '\0';
    rString += "Derivative=";
    rString += cBuffer;
    rString += "    ";
  }
  
  
  if (!(GetPagerMajorReleaseNumber((BYTE*)&cBuffer[0])))
  {
    cBuffer[2] = '\0';
    rString += "ReleaseNum=";
    rString += cBuffer;
    
    if (!(GetPagerMinorReleaseNumber((BYTE*)&cBuffer[0])))
    {
      cBuffer[2] = '\0';
      rString += ".";
      rString += cBuffer;
    }
    rString += "    ";
  }
  
  if (!(GetPagerSerialNumber((BYTE*)&cBuffer[0])))
  { 
    cBuffer[10] = '\0';
    rString += "SerialNum=";
    rString += cBuffer;
    rString += "    ";
  }

#ifndef VAIL_INTEGRATOR_PPS
  
  if (!(GetPagerReleaseType((BYTE*)&cBuffer[0])))
  {
    cBuffer[2] = '\0';
    rString += "ReleaseType=";
    if ((lstrcmp(cBuffer, " D") == 0) ||
        (lstrcmp(cBuffer, " d") == 0))
      rString += "development";
    else if ((lstrcmp(cBuffer, " B") == 0) ||
             (lstrcmp(cBuffer, " b") == 0))
      rString += "beta";
    else if ((lstrcmp(cBuffer, " R") == 0) ||
             (lstrcmp(cBuffer, " r") == 0))
      rString += "production";
    else if ((lstrcmp(cBuffer, "SD") == 0) ||
             (lstrcmp(cBuffer, "sd") == 0))
      rString += "special_development";
    else if ((lstrcmp(cBuffer, "SB") == 0) ||
             (lstrcmp(cBuffer, "sb") == 0))
      rString += "special_beta";
    else if ((lstrcmp(cBuffer, " A") == 0) ||
             (lstrcmp(cBuffer, " a") == 0))
      rString += "alpha";
    else
      rString += cBuffer;
    rString += "    ";
  }

  
  if (!(GetPagerICN((BYTE*)&cBuffer[0])))
  {
    cBuffer[10] = '\0';
    rString += "InventoryNum=";
    rString += cBuffer;
    rString += "    ";
  }
  
  if (!(GetPagerStatus(&byTemp)))
  { 
    rString += "Status=";
    if (byTemp == 'U')
      rString += "unprogrammed_codeplug";
    else if (byTemp == 'D')
      rString += "download_disabled";
    else if (byTemp == 'Y')
      rString += "password_required";
    else if (byTemp == 'O')
      rString += "pager_disabled";
    else if (byTemp == 'N')
      rString += "no_password_required";
    else
    {
      sprintf(cBuffer, "%02X", byTemp);
      rString += cBuffer;
    }
    rString += "    ";
  }
  
#endif
  
  return;
}



void CVail::GetProductName(CString& rString)
{
  char  cAPCBuffer[30];
  BYTE	byTemp;
  
  if ((!(GetPagerAPC((BYTE*)&cAPCBuffer[0]))) &&
      (!(GetPagerType(&byTemp))))
  { 
    cAPCBuffer[3] = '\0';
	if (lstrcmp(cAPCBuffer, "83A") == 0) //saurin 7-7-97  Add Vail APC
      rString += "CreataLink";                //saurin 7-24-97 Display CreataLink on Logo screen
  }
  return;
}  


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

  

