//*************************************************************
//  File name: PEGASUS.CPP
//
//  Description:  
//      Emplementation file for the CPegasus class.
//      
//      
//
//  History:    Date       Author               Comment
//              4/01/96    P. Pennington        Created
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
#include "codeplug.h" 
#include "..\w16comm\w16comm.h"
 
// external references
extern CPpsApp    	    theApp;
extern CWin16Comm	    theSerialPort;
extern CIoController	theIoController;


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif




IMPLEMENT_DYNAMIC(CPegasus, CObject)

#ifdef _DEBUG 
void CPegasus::AssertValid() const
{
    CObject::AssertValid();
}

void CPegasus::Dump(CDumpContext& dc) const
{
  CObject::Dump(dc);  
}
#endif




////////////////////////////////////////////////////////////////////////////////
//  protocol data
//
//  The following data elements and structures serve to define the protocol
//  between the application and the pager as detailed in the Pegasus Factory
//  Download/Test Document AMSD-TX72-SRE-95K0021.  Each protocol transaction
//  can be considered a sequence of IO_READs and IO_WRITES.  An array of 
//  IoCommand structs defines the sequence for each protocol transaction.
//  Compound pager operations, such as 'ReadCodePlug' can be implemented
//  by combining the IoCommand arrays into an IoOperation list.  See the
//  ReadCodePlug() function below for an example.
//
//  That is:
//		- A protocol activity such as 'ReadCodePlug' consists of a 
//		  sequence of IoOperations.
//		- An IoOperation consists of sequence of IoCommands.
//		- An IoCommand dictates a basic read or write of data to or from
//		  the serial port.



// GENERAL DEFINES FOR READABILITY
// 
#define SIZEOF(IoCmdArray)	(sizeof(IoCmdArray)/sizeof(IoCommand))
#define TICKS(y)			y    // operation times-out in (100ms * y) seconds


//  IO OPERATION IDENTIFIERS
// 
#define HANDSHAKE_OP			1 
#define READ_CP_OP				2    
#define UPLOAD_INFO_OP			3
#define WRITE_CP_OP				4
#define DELAY_OP            	6
#define ENTER_TEST_MODE_OP  	7
#define START_TEST_OP			8
#define REGISTER_DNLOAD_OP		9
#define END_REGISTER_DNLOAD_OP	10
#define EXIT_TEST_MODE_OP		11
#define SOFTWARE_RESET_OP		13
#define HARDWARE_RESET_OP		14
#define CANCEL_PREV_TEST_OP		15
#define WRITE_VERIFY_OP			16
#define PREWRITE_VERIFY_OP		17

//  IOOP CALLBACK IDENTIFIERS
//
#define	IS_PASSWORD_REQUIRED				1
#define IS_PASSWORD_ACCEPTED				2
#define IS_CHECKSUM_ACKED					4
#define IS_CP_WRITE_ACKED					5
#define IS_TEST_MODE_ACKED					6
#define IS_REG_DATA_ACKED					7
#define IS_TEST_MODE_ENTRY_ACKED			8
#define IS_HANDSHAKE_ACKED					9 
#define IS_CODEPLUG_UNPROGRAMMED			10
#define IS_CODEPLUG_VERSION_VALID			11


//  PROTOCOL IO OPERATION COMMAND ARRAYS and DATA BUFFERS
//
static BYTE byHandShakeCMD[1] = { 0xAA };  
BYTE byHandShakeAckBUFF[1];     // non-static so that CPegasus derived classes may re-use
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




// this command sequence forces the pager to send the pager info block (36 bytes), 
// followed by the pager download control byte (1 byte),
// followed by the CodePlug (768 bytes) and then to perform a hardware
// reset.  If the pager requires a password, the password download is
// inserted between receipt of the info block and the download control byte.
// received data is stored in the single array 'byInfoAndCodePlugBUFF'.  The 
// buffer is accessed by the various member pointers via offsets.  The first 36 bytes
// are the PagerIdBlock, SN, ICN, and status.  The remaining 768 bytes is the CodePlug.
static BYTE byUploadInfoAndCodePlugCMD[] = { 0x86,0x01,0x0D,0x00,0x00,0x00,0x03,0x00 };
static BYTE byInfoBUFF[35];
static BYTE byStatusBUFF[1];
static BYTE byDownloadControlBUFF[1];
static BYTE byCodePlugBUFF[768];
static BYTE byPasswordBUFF[11];
static BYTE byEncodedPasswordBUFF[10];
//
static IoCommand 	ReadCodePlugIOOP[] =
 {	
   { IO_WRITE, byUploadInfoAndCodePlugCMD, sizeof(byUploadInfoAndCodePlugCMD) },
   { IO_READ, byInfoBUFF, 35 }, 
   { IO_READ, byStatusBUFF, 1 },
   { IO_CALLBACK, NULL, IS_PASSWORD_REQUIRED }, 
   { IO_WRITE, byEncodedPasswordBUFF, sizeof(byEncodedPasswordBUFF) }, 
   { IO_READ, byDownloadControlBUFF, 1 },
   { IO_CALLBACK, NULL, IS_PASSWORD_ACCEPTED }, 
   { IO_READ, byCodePlugBUFF, 768 } 
 };
// Ex: theIoController.AddTail(new CSerialIoOperation(&theSerialPort, ReadCodePlugIOOP, 
//  											   SIZEOF(ReadCodePlugIOOP), 
//  										       READ_CP_OP, 
//  										       TICKS(50),
//												   &m_IoCallBackInterface));




// this command uploads the info and status block (36 bytes) as well as 
// the codeplug version and device model options.  Note, 3 other bytes 
// separate these 2 fields so they are uploaded as well in order to save
// from having to perform 2 separate IOOP's.  
static BYTE byUploadInfoCMD[] = { 0x86,0x01,0x0D,0x0E,0x00,0x00,0x00,0x05 };
static BYTE byCodeplugVersion;      // CP_Version
static BYTE byJustFillerBytes[3];
static BYTE byDeviceModelOptions;   // CP_OTA_Subheader[0]
// 
static IoCommand	UploadInfoIOOP[] = 
 {
   { IO_WRITE, byUploadInfoCMD, sizeof(byUploadInfoCMD) }, 
   { IO_READ, byInfoBUFF, 35 },
   { IO_READ, byStatusBUFF, 1 },
   { IO_CALLBACK, NULL, IS_PASSWORD_REQUIRED }, 
   { IO_WRITE, byEncodedPasswordBUFF, sizeof(byEncodedPasswordBUFF) }, 
   { IO_READ, byDownloadControlBUFF, 1 },
   { IO_CALLBACK, NULL, IS_PASSWORD_ACCEPTED }, 
   { IO_READ, &byCodeplugVersion, 1 },
   { IO_READ, byJustFillerBytes, 3 },
   { IO_READ, &byDeviceModelOptions, 1 }
 };                
// Ex: theIoController.AddTail(new CSerialIoOperation(&theSerialPort, UploadInfoIOOP, 
//  											   SIZEOF(UploadInfoIOOP), 
//  										       UPLOAD_INFO_OP, 
//  										       TICKS(20),
//												   &m_IoCallBackInterface));




// This IoCommand array forces the pager to receive the CodePlug block.  
// All 768 bytes of the CodePlug are downloaded.  
// There is a relatively long delay between the checksum ack and the ack of
// the download operation (45 secs).
static BYTE byDownloadCodePlugCMD[] = { 0x18,0x02,0x0D,0x00,0x00,0x00,0x03,0x00 };
static BYTE byChecksumBUFF[1];
static BYTE byChecksumAckBUFF[1];
static BYTE byWriteCpAckBUFF[1];
//
static IoCommand	WriteCodePlugIOOP[] = 
 {
   { IO_WRITE, byDownloadCodePlugCMD, sizeof(byDownloadCodePlugCMD) },
   { IO_READ, byStatusBUFF, 1 },
   { IO_CALLBACK, NULL, IS_PASSWORD_REQUIRED },
   { IO_WRITE, byEncodedPasswordBUFF, sizeof(byEncodedPasswordBUFF) }, 
   { IO_READ, byDownloadControlBUFF, 1 },
   { IO_CALLBACK, NULL, IS_PASSWORD_ACCEPTED }, 
   { IO_WRITE, byCodePlugBUFF, 768 }, 
   { IO_WRITE, byChecksumBUFF, 1 },                                                                     
   { IO_READ, byChecksumAckBUFF, 1 },                                                                   
   { IO_CALLBACK, NULL, IS_CHECKSUM_ACKED },
 };                                                            
// Ex: theIoController.AddTail(new CSerialIoOperation(&theSerialPort, WriteCodePlugIOOP, 
//  											   SIZEOF(WriteCodePlugIOOP), 
//  										       WRITE_CP_OP, 
//  										       TICKS(50),
//												   &m_IoCallBackInterface));



//  This IoOperation may be used prior to writing modifications to the 
//  device codeplug in order to verify the codeplug version, etc..
static IoCommand	PreWriteVerifyIOOP[] = 
 {
   { IO_CALLBACK, NULL, IS_CODEPLUG_VERSION_VALID },
   { IO_CALLBACK, NULL, IS_CODEPLUG_UNPROGRAMMED },
 };                
// Ex: theIoController.AddTail(new CSerialIoOperation(&theSerialPort, PreWriteVerifyIOOP, 
//  											   SIZEOF(PreWriteVerifyIOOP), 
//  										       PREWRITE_VERIFY_OP, 
//  										       TICKS(10),
//												   &m_IoCallBackInterface));



// This IoCommand array may be used to download subsets of the codeplug
// array.  
static BYTE byWriteSegmentCMD[] = { 0x80,0x02,0x00,0x00,0x00,0x00,0x00,0x00 };
static UINT *pWriteSegmentDest = (UINT*)&byWriteSegmentCMD[2];
static UINT *pWriteSegmentNumBytes = (UINT*)&byWriteSegmentCMD[6];
//
static IoCommand	WriteCodePlugSegmentIOOP[] = 
 { 
   { IO_WRITE, byWriteSegmentCMD, sizeof(byWriteSegmentCMD) },
   { IO_READ, byStatusBUFF, 1 },
   { IO_CALLBACK, NULL, IS_PASSWORD_REQUIRED },
   { IO_WRITE, byEncodedPasswordBUFF, sizeof(byEncodedPasswordBUFF) }, 
   { IO_READ, byDownloadControlBUFF, 1 },
   { IO_CALLBACK, NULL, IS_PASSWORD_ACCEPTED }, 
   { IO_WRITE, NULL, 0 },              // *** NOTE: this command filled on the fly : Ensure index is correct.
   { IO_WRITE, byChecksumBUFF, 1 },                                          //    |
   { IO_READ, byChecksumAckBUFF, 1 },                                        //    |
   { IO_CALLBACK, NULL, IS_CHECKSUM_ACKED },                                 //    |
 };                                                                          //    |
//                                                                           //    |
static IoCommand *pWriteSegmentIoCommand = &WriteCodePlugSegmentIOOP[6];     // <--+
// Ex: theIoController.AddTail(new CSerialIoOperation(&theSerialPort, WriteCodePlugSegmentIOOP, 
//  											   SIZEOF(WriteCodePlugSegmentIOOP), 
//  										       WRITE_CP_OP, 
//  										       TICKS(50),
//												   &m_IoCallBackInterface));


static IoCommand	WriteVerificationAckIOOP[] = 
 {
   { IO_READ, byWriteCpAckBUFF, 1 },
   { IO_CALLBACK, NULL, IS_CP_WRITE_ACKED },
 };
// Ex: theIoController.AddTail(new CSerialIoOperation(&theSerialPort, WriteVerificationAckIOOP, 
//  											   SIZEOF(WriteVerificationAckIOOP), 
//  										       WRITE_VERIFY_OP, 
//  										       TICKS(450),
//												   &m_IoCallBackInterface));


// Use this CIoOperation to force delays in the interface protocol between
// CIoOperation invocations.  The IO_NOOP IoCommand may also be used to 
// insert delays between IoCommands.  See file CIOOPER.H.
static IoCommand	DelayIOOP[] = 
 {
   { IO_NOOP, NULL, 0 },
 };
// Ex: theIoController.AddTail(new CSerialIoOperation(&theSerialPort, DelayIOOP, 
//  											   SIZEOF(DelayIOOP), 
//  										       DELAY_OP, 
//  										       TICKS(50),
//												   &m_IoCallBackInterface));


// Use this CIoOperation to attempt a software reset with memory intact.
static BYTE bySoftwareResetCMD[] = { 0x89,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
static IoCommand	SoftwareResetIOOP[] = 
 {
   { IO_WRITE, bySoftwareResetCMD, sizeof(bySoftwareResetCMD) }, 
   { IO_READ, byStatusBUFF, 1 },
 };                
// Ex: theIoController.AddTail(new CSerialIoOperation(&theSerialPort, SoftwareResetIOOP, 
//  											   SIZEOF(SoftwareResetIOOP), 
//  										       SOFTWARE_RESET_OP, 
//  										       TICKS(30),
//												   &m_IoCallBackInterface));


// Use this CIoOperation to attempt a hardware reset.
static BYTE byHardwareResetCMD[] = { 0x19,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
static IoCommand	HardwareResetIOOP[] = 
 {
   { IO_WRITE, byHardwareResetCMD, sizeof(byHardwareResetCMD) }, 
   { IO_READ, byStatusBUFF, 1 },
 };                
// Ex: theIoController.AddTail(new CSerialIoOperation(&theSerialPort, SoftwareResetIOOP, 
//  											   SIZEOF(HardwareResetIOOP), 
//  										       HARDWARE_RESET_OP, 
//  										       TICKS(40),
//												   &m_IoCallBackInterface));



///////////////////////////////////////////////////////////////////////////////
//  SetPagerCodePlug()
//  
//  Call this function prior to a WriteCodePlug() request in order to ensure
//  that the downloaded information matches the document contents.
//
int	CPegasus::SetPagerCodePlug(const CByteArray& rSource)        		  
{  
  m_pbyPagerCodePlug = &byCodePlugBUFF[0];
  
  // don't allow the codeplug buffer to be overwritten if any operation in progress.
  if (IsOperationInProgress())
    return(1);
    
  // error if the document is not the correct size
  if ((rSource.GetSize() != CP_SIZE)) return(1);  
  
  // force a status read in case a new pager has been connected
  m_pbyPagerStatus = NULL;
  
  // copy the rSource data
  for (int lcv = 0; lcv < CP_SIZE; ++lcv)
    m_pbyPagerCodePlug[lcv] = rSource[lcv];
  return(0);
}



//////////////////////////////////////////////////////////////////////////////
//  ReadCodePlug()
//  
//  This function is called from the OnFileNew function of the CPpsDoc class.
//  The sequence of IoOperations required to read the CodePlug from the pager
//  is constructed and the base class CPager::ReadCodePlug() function is 
//  called to initiate the first IoOperation.
int CPegasus::ReadCodePlug(void)
{
  TRACE("\nCPegasus::ReadCodePlug\n");
  int nMaxTime = 3;
  int nNumTicks = TICKS(30);
  
  // construct the sequence of IoOperations required to ReadCodePlug
  // see the definition of a CIoOperation in file 'ciooper.h' for more info.
  
  if (theSerialPort.GetBaudRate() <= 2400) 
  { 
    TRACE("\nCPegasus::ReadCodePlug : SLOW BAUD\n");
    nNumTicks = TICKS(90);    // allow more time for slow baud rate
    nMaxTime = 9;
  }
    
  theIoController.AddTail(new CSerialIoOperation(&theSerialPort, ReadCodePlugIOOP, 
  											  SIZEOF(ReadCodePlugIOOP), 
  										      READ_CP_OP, 
  										      nNumTicks, 
											  &m_IoCallBackInterface));

  m_nCurrentState |= READ_REQUESTED;
  return(CPager::ExecuteIoOpList(nMaxTime, "READ"));                                           
}




//////////////////////////////////////////////////////////////////////////////
//  WriteCodePlug()
//
//  Downloads the entire codeplug to the pager.
//  
int CPegasus::WriteCodePlug(const CByteArray& rPlug)
{
  TRACE("\nCPegasus::WriteCodePlug\n");
  CIoOperation *pIoOp = NULL;
  int lcv;
  UINT nSum = 0;
  int nMaxTime = 0;
  
  // don't allow buffer overwrite if another write operation in progress.
  if (IsWriteRequested())   
    return(1);
    
  // over-write the read buffer
  if (SetPagerCodePlug(rPlug))
    return(1);
      
  // calculate the checksum
  for (lcv=0; lcv < CP_SIZE; ++lcv)
    nSum += byCodePlugBUFF[lcv];
  byChecksumBUFF[0] = (BYTE)nSum;

  // construct the sequence of IoOperations required to WriteCodePlug
  // see the definition of a CIoOperation in file 'ciooper.h' for more info.

  theIoController.AddTail(new CSerialIoOperation(&theSerialPort, HandShakeIOOP, 
  											  SIZEOF(HandShakeIOOP), 
  										      HANDSHAKE_OP, 
  										      TICKS(10), 
											  &m_IoCallBackInterface));
  										      
  theIoController.AddTail(new CSerialIoOperation(&theSerialPort, UploadInfoIOOP, 
 											  SIZEOF(UploadInfoIOOP), 
  										      UPLOAD_INFO_OP, 
  										      TICKS(20), 
											  &m_IoCallBackInterface));

  theIoController.AddTail(new CSerialIoOperation(&theSerialPort, HandShakeIOOP, 
  											  SIZEOF(HandShakeIOOP), 
  										      HANDSHAKE_OP, 
  										      TICKS(10), 
											  &m_IoCallBackInterface));

  theIoController.AddTail(new CSerialIoOperation(&theSerialPort, WriteCodePlugIOOP, 
  											  SIZEOF(WriteCodePlugIOOP), 
  										      WRITE_CP_OP, 
  										      TICKS(60),
											  &m_IoCallBackInterface));
  nMaxTime += 10;
  m_nCurrentState |= WRITE_REQUESTED;
  return(CPager::ExecuteIoOpList(nMaxTime, "WRITE"));                                           
}




//////////////////////////////////////////////////////////////////////////////
//  WriteCodePlug()
//
//  downloads select portions of the codeplug to the pager.  Be sure to
//  make a call to SetPagerCodePlug() prior to initially calling this 
//  function since the data to be written is taken from a byte array
//  which is local to this file and separate from the document object
//  which the user is making changes to.  See file 'CPpsDoc.cpp'. 
//  
int	CPegasus::WriteCodePlug(int nIndex, int nNumBytes)
{ 
  TRACE("\nCPegasus::WriteCodePlug : nIndex=%04X : nNumBytes=%04X\n", nIndex, nNumBytes);

  CIoOperation *pIoOp = NULL;
  int nMaxTime = 0;
  
  // don't overwrite if another write operation in progress.
  if (IsWriteRequested())   
    return(1);

  if (m_pbyPagerStatus == NULL)   // Do we know if the pager is unprogrammed?
  {  							  // If not, first get pager info-block.			        
    theIoController.AddTail(new CSerialIoOperation(&theSerialPort, HandShakeIOOP, 
  											  SIZEOF(HandShakeIOOP), 
  										      HANDSHAKE_OP, 
  										      TICKS(10), 
											  &m_IoCallBackInterface));
  										      
    theIoController.AddTail(new CSerialIoOperation(&theSerialPort, UploadInfoIOOP, 
 											  SIZEOF(UploadInfoIOOP), 
  										      UPLOAD_INFO_OP, 
  										      TICKS(20), 
											  &m_IoCallBackInterface));
											  
    theIoController.AddTail(new CSerialIoOperation(&theSerialPort, PreWriteVerifyIOOP, 
  											  SIZEOF(PreWriteVerifyIOOP), 
  										      PREWRITE_VERIFY_OP, 
  										      TICKS(10),
											  &m_IoCallBackInterface));

    theIoController.AddTail(new CSerialIoOperation(&theSerialPort, DelayIOOP, 
  											  SIZEOF(DelayIOOP), 
 										      DELAY_OP, 
  										      TICKS(10),
											  &m_IoCallBackInterface));
	nMaxTime = 5;
  }
  
  ASSERT(nIndex >= 0);
  ASSERT(nNumBytes > 0);
  ASSERT((nIndex + nNumBytes) <= (BASE_ADDRESS + CP_SIZE));
  
  // set the IoOperation data
  *pWriteSegmentDest = (UINT)(BASE_ADDRESS + nIndex); 
  *pWriteSegmentDest = CpField::nByteSwap(*pWriteSegmentDest);
  *pWriteSegmentNumBytes = (UINT)nNumBytes;
  *pWriteSegmentNumBytes = CpField::nByteSwap(*pWriteSegmentNumBytes);
  pWriteSegmentIoCommand->pData = &byCodePlugBUFF[nIndex];
  pWriteSegmentIoCommand->nBytes = nNumBytes;
  // calculate the checksum
  UINT nSum = 0;
  for (int lcv=0 ; lcv < nNumBytes; ++lcv)
    nSum += byCodePlugBUFF[(nIndex + lcv)];
  byChecksumBUFF[0] = (BYTE)nSum;
    
  theIoController.AddTail(new CSerialIoOperation(&theSerialPort, HandShakeIOOP, 
  											  SIZEOF(HandShakeIOOP), 
  										      HANDSHAKE_OP, 
  										      TICKS(10), 
											  &m_IoCallBackInterface));

  theIoController.AddTail(new CSerialIoOperation(&theSerialPort, WriteCodePlugSegmentIOOP, 
  											  SIZEOF(WriteCodePlugSegmentIOOP), 
  										      WRITE_CP_OP, 
  										      TICKS(60),
											  &m_IoCallBackInterface));
											  
  nMaxTime += 7;
  m_nCurrentState |= WRITE_REQUESTED;
  return(CPager::ExecuteIoOpList(nMaxTime, "WRITE"));
}





//////////////////////////////////////////////////////////////////////////
//  DoSoftwareReset()
//
int	CPegasus::DoSoftwareReset(BYTE byWhen)
{ 
  TRACE("\nCPegasus::DoSoftwareReset\n");
  CIoOperation *pIoOp = NULL;
  int nMaxTime = 0;

  if (byWhen == NOW) 
  {
    theIoController.AddHead(new CSerialIoOperation(&theSerialPort, DelayIOOP, 
  											    SIZEOF(DelayIOOP), 
  										        DELAY_OP, 
  										        TICKS(40),
											    &m_IoCallBackInterface));
    theIoController.AddHead(new CSerialIoOperation(&theSerialPort, SoftwareResetIOOP, 
 											    SIZEOF(SoftwareResetIOOP), 
  										        SOFTWARE_RESET_OP, 
  										        TICKS(30),
											    &m_IoCallBackInterface));
    theIoController.AddHead(new CSerialIoOperation(&theSerialPort, HandShakeIOOP, 
  											    SIZEOF(HandShakeIOOP), 
  										        HANDSHAKE_OP, 
  										        TICKS(10),
											    &m_IoCallBackInterface));
    return(0);
  }
  else  // LATER
  {
    theIoController.AddTail(new CSerialIoOperation(&theSerialPort, HandShakeIOOP, 
  											    SIZEOF(HandShakeIOOP), 
  										        HANDSHAKE_OP, 
  										        TICKS(10),
											    &m_IoCallBackInterface));
    theIoController.AddTail(new CSerialIoOperation(&theSerialPort, SoftwareResetIOOP, 
 											    SIZEOF(SoftwareResetIOOP), 
  										        SOFTWARE_RESET_OP, 
  										        TICKS(30), 
											    &m_IoCallBackInterface));
    theIoController.AddTail(new CSerialIoOperation(&theSerialPort, DelayIOOP, 
  											    SIZEOF(DelayIOOP), 
  										        DELAY_OP, 
  										        TICKS(30),
											    &m_IoCallBackInterface));
	nMaxTime += 7;
  }
  return(CPager::ExecuteIoOpList(nMaxTime, "RESET"));                                           
}




//////////////////////////////////////////////////////////////////////////
//  DoHardwareReset()
//
int	CPegasus::DoHardwareReset(BYTE byWhen) 
{ 
  TRACE("\nCPegasus::DoHardwareReset\n");
  CIoOperation *pIoOp = NULL;
  int nMaxTime = 0;
  
  if (byWhen == NOW) 
  {
    theIoController.AddHead(new CSerialIoOperation(&theSerialPort, DelayIOOP, 
  											    SIZEOF(DelayIOOP), 
  										        DELAY_OP, 
  										        TICKS(30),
											    &m_IoCallBackInterface));
    theIoController.AddHead(new CSerialIoOperation(&theSerialPort, HardwareResetIOOP, 
 											    SIZEOF(HardwareResetIOOP), 
  										        HARDWARE_RESET_OP, 
  										        TICKS(30),
											    &m_IoCallBackInterface));
    theIoController.AddHead(new CSerialIoOperation(&theSerialPort, HandShakeIOOP, 
  											    SIZEOF(HandShakeIOOP), 
  										        HANDSHAKE_OP, 
  										        TICKS(10),
											    &m_IoCallBackInterface));  										        
    return(0);
  }
  else  // LATER
  {
    theIoController.AddTail(new CSerialIoOperation(&theSerialPort, HandShakeIOOP, 
  											    SIZEOF(HandShakeIOOP), 
  										        HANDSHAKE_OP, 
  										        TICKS(10),
											    &m_IoCallBackInterface));
    theIoController.AddTail(new CSerialIoOperation(&theSerialPort, HardwareResetIOOP, 
 										        SIZEOF(HardwareResetIOOP), 
  										        HARDWARE_RESET_OP, 
  										        TICKS(30),
											    &m_IoCallBackInterface));
    theIoController.AddTail(new CSerialIoOperation(&theSerialPort, DelayIOOP, 
  											    SIZEOF(DelayIOOP), 
  										        DELAY_OP, 
  										        TICKS(30),
											    &m_IoCallBackInterface));
    nMaxTime += 6;
  }
  return(CPager::ExecuteIoOpList(nMaxTime, "RESET"));
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
int CPegasus::IoCallBackInterface::OnIoOperationCallBack(int nCallBackId)
{
  TRACE("\nCPegasus::IoCallBackInterface::OnIoOperationCallBack : nCallBackId=%d", nCallBackId);
  METHOD_PROLOGUE(CPegasus, m_IoCallBackInterface)  // makes pThis (see 'ciooper.h')
  
  switch(nCallBackId)
  {  
    case IS_HANDSHAKE_ACKED:
      if (pThis->HandleIsHandshakeAcked())
        return(1);
      break;
        
    case IS_PASSWORD_REQUIRED :
      if (pThis->HandleIsPasswordRequired())
        return(1); 
      break;
      
    case IS_PASSWORD_ACCEPTED:
      if (pThis->HandleIsPasswordAccepted())
        return(1);
      break;
      
    case IS_CHECKSUM_ACKED:
      if (pThis->HandleIsCheckSumAcked())
        return(1);
      break;
    
    case IS_CP_WRITE_ACKED:
      if (pThis->HandleIsCodePlugWriteAcked())
        return(1);
      break;

    case IS_TEST_MODE_ACKED:
      if (pThis->HandleIsTestModeAcked())
        return(1);
      break;

    case IS_REG_DATA_ACKED:
      if (pThis->HandleIsTestRegisterDataAcked())
        return(1);
      break;
    
    case IS_TEST_MODE_ENTRY_ACKED:
      if (pThis->HandleIsTestModeAcked())
        return(1);
      else
        pThis->m_nCurrentState |= TEST_MODE;
      break;
    
    case IS_CODEPLUG_UNPROGRAMMED:
      if (pThis->HandleIsCodePlugUnprogrammed())
        return(1);
      break;
      
    case IS_CODEPLUG_VERSION_VALID:
      if (pThis->HandleIsCodePlugVersionValid())
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
int CPegasus::IoCallBackInterface::OnIoOperationComplete(UINT nStatus, UINT nIoOpId) 
{
  TRACE("\nCPegasus::IoCallBackInterface::OnIoOperationComplete : nStatus=%04X : "
          "nIoOpId=%04X\n", nStatus, nIoOpId);

  METHOD_PROLOGUE(CPegasus, m_IoCallBackInterface)  // makes pThis (see 'ciooper.h')
  
  // handle op completion based on IoOperation nIOOPid indicator
  switch(nIoOpId)
  {
    case HANDSHAKE_OP:
      TRACE("\tIoOperation = HANDSHAKE_OP\n");
      if (nStatus)  // Operation Failed ???
        pThis->m_szDisplayOnFailure += "\nDevice not responding.\n"
                                       "Press F1 for Help.\n";         
      break;
      
    case READ_CP_OP:
      TRACE("\tIoOperation = READ_CP_OP\n");
      pThis->m_nCurrentState &= (~(READ_REQUESTED));
      if (nStatus)  // Operation Failed ???           
      {
        theApp.m_pMainWnd->PostMessage(WM_READ_CP_DONE, 1, 0L); 
        pThis->m_szDisplayOnFailure += "\n'Device-Read' Operation Failed.\n";
        break;
      }              
      // establish the CPegasus member data pointers
      pThis->m_pbyPagerIdBlock = &byInfoBUFF[0];
      pThis->m_pbyPagerSerialNum = &byInfoBUFF[15];
      pThis->m_pbyPagerICN = &byInfoBUFF[25];
      pThis->m_pbyPagerStatus = &byStatusBUFF[0];
      pThis->m_pbyPagerCodePlug = &byCodePlugBUFF[0];
      theApp.m_pMainWnd->PostMessage(WM_READ_CP_DONE, 0, 0L); 
      break;
      
    case UPLOAD_INFO_OP:
      TRACE("\tIoOperation = UPLOAD_INFO_OP\n");         
      if (nStatus)  // Operation Failed ???           
      {
        pThis->m_szDisplayOnFailure += "\nFailure on 'Upload-Info' operation.\n";
        break;
      }              
      // establish the CPegasus member data pointers
      pThis->m_pbyPagerIdBlock = &byInfoBUFF[0];
      pThis->m_pbyPagerSerialNum = &byInfoBUFF[15];
      pThis->m_pbyPagerICN = &byInfoBUFF[25];
      pThis->m_pbyPagerStatus = &byStatusBUFF[0];
      break;
    
    case PREWRITE_VERIFY_OP:
      TRACE("\tIoOperation = PREWRITE_VERIFY_OP\n");
      if (nStatus)  // Operation Failed ???           
        pThis->m_szDisplayOnFailure += "\n'Device-Write' Operation Failed.\n"; 
      break;
            
    case WRITE_CP_OP: 
      TRACE("\tIoOperation = WRITE_CP_OP\n");         
      if (nStatus)  // Operation Failed ???           
        pThis->m_szDisplayOnFailure += "\n'Device-Write' Operation Failed.\n"; 
      // NOTE:  Write operation is not completed until WRITE_VERIFY_OP completes
      break;
    
    case WRITE_VERIFY_OP:
      TRACE("\tIoOperation = WRITE_VERIFY_OP\n");
      pThis->m_nCurrentState &= (~(WRITE_REQUESTED));
      if (nStatus)  // Operation Failed ???           
        pThis->m_szDisplayOnFailure += "\nWrite Verification Operation Failed.\n";
      theApp.m_pMainWnd->PostMessage(WM_WRITE_CP_DONE, nStatus, 0L);
      break;
      
    case ENTER_TEST_MODE_OP : 
      TRACE("\tIoOperation = ENTER_TEST_MODE_OP\n"); 
      if (nStatus)  // Operation Failed ???           
        pThis->m_szDisplayOnFailure += "\nEnter Test Mode Operation Failed.\n";
      pThis->m_nCurrentState |= TEST_MODE;
      pThis->m_nCurrentState &= (~(TEST_MODE_REQUESTED)); 
      break;
    
    case START_TEST_OP:
      TRACE("\tIoOperation = START_TEST_OP\n");
      if (nStatus)  // Operation Failed ???           
        pThis->m_szDisplayOnFailure += "\nStart Test Operation Failed.\n";
      break;
    
    case REGISTER_DNLOAD_OP:
      TRACE("\tIoOperation = REGISTER_DNLOAD_OP\n");
      if (nStatus)  // Operation Failed ???           
        pThis->m_szDisplayOnFailure += "\nTest Mode Register Download Operation Failed.\n";
      break; 
          
    case END_REGISTER_DNLOAD_OP : 
      TRACE("\tIoOperation = END_REGISTER_DNLOAD_OP\n");
      if (nStatus)  // Operation Failed ???           
         pThis->m_szDisplayOnFailure += "\nEnd Test Mode Register Download Operation Failed.\n";
      break; 
      
    case EXIT_TEST_MODE_OP:
      TRACE("\tIoOperation = EXIT_TEST_MODE_OP\n");
      if (nStatus)  // Operation Failed ???           
        pThis->m_szDisplayOnFailure += "\nExit Test Mode Operation Failed.\n";
      pThis->m_nCurrentState &= (~(TEST_MODE));
      break;
      
    case SOFTWARE_RESET_OP:
      TRACE("\tIoOperation = SOFTWARE_RESET_OP\n");
      if (nStatus)  // Operation Failed ???           
        pThis->m_szDisplayOnFailure += "\nSoft-Reset Operation Failed.\n";
      break;
      
    case HARDWARE_RESET_OP:
      TRACE("\tIoOperation = HARDWARE_RESET_OP\n");
      if (nStatus)  // Operation Failed ???           
        pThis->m_szDisplayOnFailure += "\nHard-Reset Operation Failed.\n";
      break;
      
    case CANCEL_PREV_TEST_OP:
      TRACE("\tIoOperation = CANCEL_PREV_TEST_OP\n");
      if (nStatus)  // Operation Failed ???           
        pThis->m_szDisplayOnFailure += "\nCancel Previous Test Operation Failed.\n";
      break;
    
    case DELAY_OP:  
      TRACE("\tIoOperation = DELAY_OP\n");
      if (nStatus)  // Operation Failed ???           
        pThis->m_szDisplayOnFailure += "\nI/O Delay Operation Operation Failed.\n";
      break;
    
    default:	
      // move on to next IoOperation in sequence
      break;
  }
  
  return(pThis->OnOpCompletion(nStatus, nIoOpId)); // return via base class
}


 
 
/////////////////////////////////////////////////////////////////////
//  Handle...() 
// 


int CPegasus::HandleIsHandshakeAcked(void)
{
  TRACE("\nCPegasus::HandleIsHandshakeAcked\n");
  
  // if OK  
  if ((byHandShakeAckBUFF[0] == PEGASUS_ALPHA) ||
      (byHandShakeAckBUFF[0] == CENTAUR) ||
      (byHandShakeAckBUFF[0] == PEGASUS_ACK) ||
      (byHandShakeAckBUFF[0] == PAGEFINDER) ||
      (byHandShakeAckBUFF[0] == ASPEN) ||
      (byHandShakeAckBUFF[0] == EXPANDAUR) ||
      (byHandShakeAckBUFF[0] == LYNX))
  { 
    m_byPagerId = byHandShakeAckBUFF[0];
    return(0);
  }
    
  char szTemp[80];  
  // if the Ack byte is the same as the Handshake byte,
  // the user may be using a NULL-modem cable.  
  if (byHandShakeAckBUFF[0] == byHandShakeCMD[0])
  {                               
    sprintf(szTemp, "\nCOM port echo received.  Check the cable and all connectors.\n");
  }
  else
  {
    sprintf(szTemp, "\nUnsupported device ID (0x%02X) received.\n", 
            byHandShakeAckBUFF[0]); 
  }
  m_szDisplayOnFailure += szTemp;
   
  return(1);
}


 
int CPegasus::HandleIsPasswordRequired(void)
{
  TRACE("\nCPegasus::HandleIsPasswordRequired\n");

  // verify pager status
  m_pbyPagerStatus = &byStatusBUFF[0];

  if (*m_pbyPagerStatus == 0x59)  // 'Y' == password required
  {
    if (m_pbyPagerPassword == NULL)  // no password entered by user
    {
      m_szDisplayOnFailure += "\nThe device requires a password."
                              "\nEnter a password via the 'Device-Password' menu and "
                              "re-initiate the device access operation.\n"; 
      return(1);
    }
  }  
  else if (*m_pbyPagerStatus == 0x44) // 'D' == Pager Download Disabled
  {
    m_szDisplayOnFailure += "\nSupport for external programming of this device has been disabled.  "
                            "Further attempts to configure the device will fail.  "
                            "Contact customer support for assistance.\n"; 
    return(1);
  } 
  else
  {
    CIoOperation *pIoOp = (CSerialIoOperation *)theIoController.GetCurrentIoOperation();
    ASSERT(pIoOp != NULL);
    pIoOp->SkipIoCommand(1);   // don't send password
  }
  return(0);
}


int CPegasus::HandleIsPasswordAccepted(void)
{
  TRACE("\nCPegasus::HandleIsPasswordAccepted\n"); 
  char msg[300];

  m_pbyPagerDownloadControl = byDownloadControlBUFF;

  // the Download Control byte may be either 0xBB or a value between 0-7.
  if (*m_pbyPagerDownloadControl != 0xBB)
  {
    sprintf(msg, "\nThe device access password was not accepted.  "
                 "CAUTION:  After 8 unsuccessful access attempts, "
                 "device access will be permanently disabled.\n"
                 "Attempts remaining:  %d\n",
                 *m_pbyPagerDownloadControl); 
    m_szDisplayOnFailure += msg;
    return(1);
  } 
  return(0);
}


/////////////////////////////////////////////////////////////////////////////
// HandleIsCheckSumAcked
//
// Callback from WriteCodePlug operation.  If success, insert IoOperation
// to receive second ACK from pager.  This will take about 40 seconds due
// the time required to re-write the entire EEPROM.  If only writing a 
// segment of the codeplug, the time will be less.
//
int CPegasus::HandleIsCheckSumAcked(void)
{
  TRACE("\nCPegasus::HandleIsCheckSumAcked\n");
  CIoOperation *pIoOp = NULL;
  
  if (byChecksumAckBUFF[0] != 0x06)
  {
    m_szDisplayOnFailure += "\nThe checksum of the downloaded data was not accepted.\n";
    return(1);
  }
  										      
  theIoController.AddTail(new CSerialIoOperation(&theSerialPort, WriteVerificationAckIOOP, 
   											  SIZEOF(WriteVerificationAckIOOP), 
  										      WRITE_VERIFY_OP, 
  										      TICKS(450), 
											  &m_IoCallBackInterface));
  return(CPager::ExecuteIoOpList(45, "WRITE"));
}



///////////////////////////////////////////////////////////////////////////
//  HandleIsCodePlugWriteAcked
//
//  Callback from WriteVerificationAckIOOP.
//
int CPegasus::HandleIsCodePlugWriteAcked(void)
{
  TRACE("\nCPegasus::HandleIsCodePlugWriteAcked\n");
  
  if (byWriteCpAckBUFF[0] != 0x06) 
  {
    m_szDisplayOnFailure += "\nThe downloaded codeplug data was not accepted "
      			            "by the device.\n";
    return(1);
  }
  return(0);
}

 
///////////////////////////////////////////////////////////////////////////
//  HandleIsCodePlugUnprogrammed()
//
//  Callback from PreWriteVerifyIOOP
//
int CPegasus::HandleIsCodePlugUnprogrammed(void)
{ 
  TRACE("\nCPegasus::HandleIsCodePlugUnprogrammed\n"); 
                                                     
  // ensure entire codeplug is written if device is unprogrammed
  if (IsWriteRequested() && IsCodePlugUnprogrammed())
  {
    // reset the IoOperation data
    *pWriteSegmentDest = (UINT)BASE_ADDRESS;
    *pWriteSegmentNumBytes = (UINT)CP_SIZE;
    pWriteSegmentIoCommand->pData = &byCodePlugBUFF[0];
    pWriteSegmentIoCommand->nBytes = CP_SIZE;
    // calculate the checksum
    UINT nSum = 0;
    for (int lcv=0 ; lcv < CP_SIZE; ++lcv)
      nSum += byCodePlugBUFF[lcv];
    byChecksumBUFF[0] = (BYTE)nSum;
  }
  
  return(0);
}


//////////////////////////////////////////////////////////////////////////
//  HandleIsCodePlugVersionValid()
//
//  Callback from UploadInfoIOOP
//  This function validates that the current codeplug about to be
//  written matches the codeplug version already on the pager.  If
//  the two don't match, then the operation fails.  Allow the write
//  if the pager has a previously unprogrammed codeplug. 
//
int CPegasus::HandleIsCodePlugVersionValid(void)
{  
  // Verify that the current configuration device ID matches that 
  // of the attached device.
  if (byHandShakeAckBUFF[0] != byCodePlugBUFF[0x10])
  {
    m_szDisplayOnFailure += "\nAttempt to write a configuration with a device ID which does "
                            "not correspond to the attached device ID.  Operation Failed."
                            "\nFirst perform a 'Device-Read' operation on the "
                            "connected device, make any configuration changes, "
                            "and then perform a 'Device-Write' operation.\n"; 
                             
    return(1);
  }
  
  // If device is unprogrammed, there is no sense in verfiying codeplug
  // version or device model since the codeplug is blank.
  if (IsCodePlugUnprogrammed())
    return(0);   // Validated
  
  // Verify that the current configuration codeplug version matches 
  // that of the attached device.  Note:  The DeviceModelOptions corresponds
  // to the CP_OTA_Subheader[0] codeplug field.    
  #ifdef DISABLE_HEXEDIT
  if ((byCodeplugVersion != byCodePlugBUFF[0x0E]) ||
      (byDeviceModelOptions != byCodePlugBUFF[0x12]))
  #else
  if (byCodeplugVersion != byCodePlugBUFF[0x0E]) 
  #endif
  {
    m_szDisplayOnFailure += "\nAttempt to over-write an existing codeplug with "
                            "a different version of codeplug data.  Operation failed."
                            "\nFirst perform a 'Device-Read' operation on the connected "
                            "device, make any configuration changes, and then perform "
                            "a 'Device-Write' operation.\n";
    return(1);
  } 
  
  // Validated.
  return(0);
}    


////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
// PEGASUS TUNING AND TEST MODE PROTOCOL ELEMENTS
//


static BYTE	byEnterTestModeCMD[] = { 0x80,0x09,0x00,0x00,0x00,0x00,0x00,0x00 };
static BYTE byTestResponseBUFF[1] = { 0x2E }; 
//
static IoCommand	EnterTestModeIOOP[] =	
 {
   { IO_WRITE, byEnterTestModeCMD, sizeof(byEnterTestModeCMD) }, 
   { IO_READ, byTestResponseBUFF, 1 },
   { IO_CALLBACK, NULL, IS_TEST_MODE_ENTRY_ACKED },
 };                
// Ex: theIoController.AddTail(new CSerialIoOperation(&theSerialPort, EnterTestModeIOOP, 
//  											   SIZEOF(EnterTestModeIOOP), 
//  										       ENTER_TEST_MODE_OP, 
//  										       TICKS(30),
//											       &m_IoCallBackInterface));



static BYTE byTestNumberBUFF[1] = { 0x00 };
static BYTE byRegisterIndicatorBUFF[1] = { 0x00 };// 0x00=use defaults register data; 
//												  // 0x01=override defaults with ...
static IoCommand	StartTestIOOP[] =             // 0x02=override with ... and look for more
 {
   { IO_WRITE, byTestNumberBUFF, 1 }, 
   { IO_READ, byTestResponseBUFF, 1 },
   { IO_CALLBACK, NULL, IS_TEST_MODE_ACKED },
   { IO_WRITE, byRegisterIndicatorBUFF, 1 },
   { IO_READ, byTestResponseBUFF, 1 },
   { IO_CALLBACK, NULL, IS_TEST_MODE_ACKED },
 };                
// Ex: theIoController.AddTail(new CSerialIoOperation(&theSerialPort, StartTestIOOP, 
//  											   SIZEOF(StartTestIOOP), 
//  										       START_TEST_OP, 
//  										       TICKS(40),
//											       &m_IoCallBackInterface));



static BYTE byEndRegisterDownloadBUFF[1] = { 0xFF };
//
static IoCommand	EndRegisterDownloadIOOP[] =
 { 
   { IO_WRITE, byEndRegisterDownloadBUFF, 1 },
   { IO_READ, byTestResponseBUFF, 1 },
   { IO_CALLBACK, NULL, IS_TEST_MODE_ACKED },
 };                
// Ex: theIoController.AddTail(new CSerialIoOperation(&theSerialPort, EndRegisterDownloadIOOP, 
//  											   SIZEOF(EndRegisterDownloadIOOP), 
//  										       END_REGISTER_DNLOAD_OP, 
//  										       TICKS(20),
//											       &m_IoCallBackInterface));



static BYTE byExitTestModeBUFF[1] = { 0x1F };
//
static IoCommand	ExitTestModeIOOP[] =
 {
   { IO_WRITE, byExitTestModeBUFF, 1 },
   { IO_READ, byTestResponseBUFF, 1 },       
   { IO_CALLBACK, NULL, IS_TEST_MODE_ACKED },
 };                
// Ex: theIoController.AddTail(new CSerialIoOperation(&theSerialPort, ExitTestModeIOOP, 
//  											   SIZEOF(ExitTestModeIOOP), 
//  										       EXIT_TEST_MODE_OP, 
//  										       TICKS(50),
//											       &m_IoCallBackInterface));


static BYTE byTestModeCtrlRegNumBUFF[1] = { 0xAE };
static BYTE byTestModeCtrlRegBUFF[6] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; 
static BYTE byTestModeCtrlRegCheckSumBUFF[1] = { 0x00 }; 
//
static IoCommand	TestModeCtrlRegIOOP[] =
 {
   { IO_WRITE, byTestModeCtrlRegNumBUFF, 1 },
   { IO_WRITE, byTestModeCtrlRegBUFF,  sizeof(byTestModeCtrlRegBUFF) },
   { IO_WRITE, byTestModeCtrlRegCheckSumBUFF, 1 }, 
   { IO_READ, byTestResponseBUFF, 1 },
   { IO_CALLBACK, NULL, IS_REG_DATA_ACKED },
 };                
// Ex: theIoController.AddTail(new CSerialIoOperation(&theSerialPort, TestModeCtrlRegIOOP, 
//  											   SIZEOF(TestModeCtrlRegIOOP), 
//  										       REGISTER_DNLOAD_OP, 
//  										       TICKS(20),
//											       &m_IoCallBackInterface));


static BYTE byTransmitRegNumBUFF[1] = { 0x40 };
static BYTE byTransmitRegBUFF[6] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; 
static BYTE byTransmitRegCheckSumBUFF[1] = { 0x00 };
//
static IoCommand	TransmitRegIOOP[] =
 {
   { IO_WRITE, byTransmitRegNumBUFF, 1 },
   { IO_WRITE, byTransmitRegBUFF,  sizeof(byTransmitRegBUFF) },
   { IO_WRITE, byTransmitRegCheckSumBUFF, 1 }, 
   { IO_READ, byTestResponseBUFF, 1 },
   { IO_CALLBACK, NULL, IS_REG_DATA_ACKED },
 };                
// Ex: theIoController.AddTail(new CSerialIoOperation(&theSerialPort, TransmitRegIOOP, 
//  											   SIZEOF(TransmitRegIOOP), 
//  										       REGISTER_DNLOAD_OP, 
//  										       TICKS(20),
//											       &m_IoCallBackInterface));


static BYTE byRxTuningRegNumBUFF[1] = { 0x42 };
static BYTE byRxTuningRegBUFF[4] = { 0x00, 0x00, 0x00, 0x00 }; 
static BYTE byRxTuningRegCheckSumBUFF[1] = { 0x00 };
//
static IoCommand	RxTuningRegIOOP[] =
 {
   { IO_WRITE, byRxTuningRegNumBUFF, 1 },
   { IO_WRITE, byRxTuningRegBUFF,  sizeof(byRxTuningRegBUFF) },
   { IO_WRITE, byRxTuningRegCheckSumBUFF, 1 }, 
   { IO_READ, byTestResponseBUFF, 1 },
   { IO_CALLBACK, NULL, IS_REG_DATA_ACKED },
 };                
// Ex: theIoController.AddTail(new CSerialIoOperation(&theSerialPort, RxTuningRegIOOP, 
//  											   SIZEOF(RxTuningRegIOOP), 
//  										       REGISTER_DNLOAD_OP, 
//  										       TICKS(20),
//											       &m_IoCallBackInterface));



static BYTE byTxWarmupRegNumBUFF[1] = { 0x46 };
static BYTE byTxWarmupRegBUFF[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; 
static BYTE byTxWarmupRegCheckSumBUFF[1] = { 0x00 };
//
static IoCommand	TxWarmupRegIOOP[] =
 {
   { IO_WRITE, byTxWarmupRegNumBUFF, 1 },
   { IO_WRITE, byTxWarmupRegBUFF,  sizeof(byTxWarmupRegBUFF) },
   { IO_WRITE, byTxWarmupRegCheckSumBUFF, 1 }, 
   { IO_READ, byTestResponseBUFF, 1 },
   { IO_CALLBACK, NULL, IS_REG_DATA_ACKED },
 };                
// Ex: theIoController.AddTail(new CSerialIoOperation(&theSerialPort, TxWarmupRegIOOP, 
//  											   SIZEOF(TxWarmupRegIOOP), 
//  										       REGISTER_DNLOAD_OP, 
//  										       TICKS(20),
//											       &m_IoCallBackInterface));


 
static BYTE byCancelPreviousTestBUFF[1] = { 0xFF };
//
static IoCommand	CancelPreviousTestIOOP[] = 
 {
   { IO_WRITE, byCancelPreviousTestBUFF, 1 }, 
   { IO_READ, byTestResponseBUFF, 1 },
   { IO_CALLBACK, NULL, IS_TEST_MODE_ACKED },
 };
// Ex: theIoController.AddTail(new CSerialIoOperation(&theSerialPort, CancelPreviousTestIOOP, 
//  											   SIZEOF(CancelPreviousTestIOOP), 
//  										       CANCEL_PREV_TEST_OP, 
//  										       TICKS(20),
//											       &m_IoCallBackInterface));



///////////////////////////////////////////////////////////////////////////
//	Handle...()
//
//	IO_CALLBACK handlers for the test mode protocol
//

int CPegasus::HandleIsTestModeAcked(void)
{ 
  TRACE("\nCPegasus::HandleIsTestModeAcked\n");
  
  if (byTestResponseBUFF[0] != 0x2E)
  {
    m_szDisplayOnFailure += "\nA test-mode command did not complete or was not accepted by the device.  "
      			            "Device test-mode operation failed.  Press 'Stop Test' to exit test mode.\n";
    return(1);
  }
  return(0);  
}



int CPegasus::HandleIsTestRegisterDataAcked(void)
{
  TRACE("\nCPegasus::HandleIsTestRegisterDataAcked\n");
  
  if (byTestResponseBUFF[0] != 0x2E)
  { 
    m_szDisplayOnFailure += "\nThe downloaded DSP/ASIC register data was not accepted by "
      			            "the device.  Device test-mode operation failed.  "
     			            "Press 'Stop Test' to exit test mode.\n";
    return(1);
  }
  return(0);  
}





//////////////////////////////////////////////////////////////////////////
//  EnterTestMode()
//
// the pager should be placed in test mode and will continue in test
// mode until the exit test mode operation is initiated.
//
int CPegasus::EnterTestMode(void)
{
  int nMaxTime = 0;
  
  if (IsTestModeRequested())  // prevent sequential enter test mode requests
    return(0);
    
  // insert handshake sequence
  theIoController.AddTail(new CSerialIoOperation(&theSerialPort, HandShakeIOOP, 
  											  SIZEOF(HandShakeIOOP), 
  										      HANDSHAKE_OP, 
  										      TICKS(10),
											  &m_IoCallBackInterface));

  theIoController.AddTail(new CSerialIoOperation(&theSerialPort, EnterTestModeIOOP, 
  											  SIZEOF(EnterTestModeIOOP), 
  										      ENTER_TEST_MODE_OP, 
  										      TICKS(30),
											  &m_IoCallBackInterface));

  // the pager should be placed in test mode and will continue in test
  // mode until the exit test mode operation is initiated.
  nMaxTime += 4;
  m_nCurrentState |= TEST_MODE_REQUESTED;  
  return(CPager::ExecuteIoOpList(nMaxTime, "TEST"));                                           
}




//////////////////////////////////////////////////////////////////////////
//  DoTestUseDefaultData()
//  
//  Initiate the desired test, but command the pager to use default 
//  DSP/ASIC register contents.
//
int	CPegasus::DoTestUseDefaultData(BYTE byTestId)
{
  TRACE("\nCPegasus::DoTestUseDefaultData : %02X\n", byTestId);
  int nMaxTime = 0;
  
  if (IsPagerInTestMode())   // prevent sequential start test requests
    return(1);

  if (IsOperationInProgress())		// insert time for pager to reset
  { 
    theIoController.AddTail(new CSerialIoOperation(&theSerialPort, DelayIOOP, 
  	  										    SIZEOF(DelayIOOP), 
  										        DELAY_OP, 
  										        TICKS(30),
											    &m_IoCallBackInterface));
	nMaxTime += 3;
  }
  // insert handshake sequence
  theIoController.AddTail(new CSerialIoOperation(&theSerialPort, HandShakeIOOP, 
  											  SIZEOF(HandShakeIOOP), 
  										      HANDSHAKE_OP, 
  										      TICKS(10),
											  &m_IoCallBackInterface));

  theIoController.AddTail(new CSerialIoOperation(&theSerialPort, EnterTestModeIOOP, 
  											  SIZEOF(EnterTestModeIOOP), 
  										      ENTER_TEST_MODE_OP, 
  										      TICKS(50),
											  &m_IoCallBackInterface));

  byTestNumberBUFF[0] = byTestId;	
  byRegisterIndicatorBUFF[0] = 0x00;	// use default register data
  theIoController.AddTail(new CSerialIoOperation(&theSerialPort, StartTestIOOP, 
  											  SIZEOF(StartTestIOOP), 
  										      START_TEST_OP, 
  										      TICKS(40),
											  &m_IoCallBackInterface));

  // the pager should be placed in test mode and will continue in test
  // mode until the stop test operation is initiated.
  nMaxTime += 10;
  return(CPager::ExecuteIoOpList(nMaxTime, "TEST"));                                           
}




//////////////////////////////////////////////////////////////////////////
//  ExitTestMode()
//
//  Note:  the byExitTestModeCommand is passed in as a parameter since the 
//  command itself may change between pager releases.  Refer to the
//  Factory Download/Test Document.
//
int	CPegasus::ExitTestMode(BYTE byExitTestModeCommand)
{
  TRACE("\nCPegasus::ExitTestMode\n");           
  int nMaxTime = 0;
  
  // pager must already be in test mode or requesting it.
  if (!((IsPagerInTestMode()) || (IsTestModeRequested())))   
    return(1);
    
  byExitTestModeBUFF[0] = byExitTestModeCommand;
  
  theIoController.AddTail(new CSerialIoOperation(&theSerialPort, ExitTestModeIOOP, 
  											  SIZEOF(ExitTestModeIOOP), 
  										      EXIT_TEST_MODE_OP, 
  										      TICKS(40),
											  &m_IoCallBackInterface));
											  
  // insert a delay to account for the pager reset time.
  theIoController.AddTail(new CSerialIoOperation(&theSerialPort, DelayIOOP, 
     										  SIZEOF(DelayIOOP), 
  										      DELAY_OP, 
  										      TICKS(40),
											  &m_IoCallBackInterface));
  nMaxTime += 8;											  
  return(CPager::ExecuteIoOpList(nMaxTime, "STOP"));
}



//////////////////////////////////////////////////////////////////////////
//  CancelPreviousTest()
//
//  Note:  the CancelPrevTestCmd is passed in as a parameter since the 
//  command itself may change between pager releases.  Refer to the
//  Factory Download/Test Document.
//
int	CPegasus::CancelPreviousTest(BYTE CancelPrevTestCmd)
{
  TRACE("\nCPegasus::CancelPreviousTest\n");           
  int nMaxTime = 0;
  
  // pager must already be in test mode or requesting it.
  if (!((IsPagerInTestMode()) || (IsTestModeRequested())))   
    return(1);
  
  byCancelPreviousTestBUFF[0] = CancelPrevTestCmd;
  
  theIoController.AddTail(new CSerialIoOperation(&theSerialPort, CancelPreviousTestIOOP, 
  											  SIZEOF(CancelPreviousTestIOOP), 
  										      CANCEL_PREV_TEST_OP, 
  										      TICKS(20),
											  &m_IoCallBackInterface));
  nMaxTime += 2;  										      
  return(CPager::ExecuteIoOpList(nMaxTime, "TEST"));
}



//////////////////////////////////////////////////////////////////////
//  DoTuningTest()
//  
//  See the Pegasus Factory Download/Test Document as well as the
//  DSP/ASIC Register Map Document.
//
//  The device must already have entered test mode 
//  (or entry has been requested) prior to calling this function.  
//  Additionally, the CancelPreviousTest or ExitTestMode/EnterTestMode
//  functions must be called prior to a second call to this function. 
//  
//  Beyond placing the pager into test mode, initiating a Transceiver 
//  Tuning test involves downloading the DSP/ASIC Test Mode Control
//  Register (0xAE) as well as the Transmit Register (0x40) and the
//  Receive Register (0x42).  The following register configuration applies:
//
//	Test Mode Control Register (0xAE):		
//				bit -> F  E  D  C  B  A  9  8  7  6  5  4  3  2  1  0
//				word1  -  -  -  -  -  -  -  -  -  -  -  ---- tm -----
//				word2  -  -  -  *  *  ------- nCenterFreq (tf) ------
//				word3  ------------- TestData (bf) ------------------
//				    
//					where:	tm = Test Mode                                                      
//							tf = Test Center Frequency
//							bf = test dependent data							
//					note: * is for Reflex 50 only - 00 = -5 khz offsets
//												  - 01 = 5 khz offsets
//												  - 10 = 15 kHz offsets
//												  - 11 = -15 kHz offsets
//
//	Transmit Register (0x40)
//				bit -> F  E  D  C  B  A  9  8  7  6  5  4  3  2  1  0
//				word1  -------- L1 ----------  -------- L2 ----------
//				word2  -------- L3 ----------  -------- L4 ----------
//				word3  -------- bat ---------  -------- L0 ----------
//
//					where:  L0 = CP_Tx_Idle_Symbol
//							bat = CP_Balance_Tune
//							L1 = CP_Symbol_Tx_Level[0] (-2400)
//							L2 = CP_Symbol_Tx_Level[1] (-800)
//							L3 = CP_Symbol_Tx_Level[2] (+2400)
//							L4 = CP_Symbol_Tx_Level[3] (+800)
// 
//
//	Rx Tuning Register (0x42)
//				bit -> F  E  D  C  B  A  9  8  7  6  5  4  3  2  1  0
//				word1  -------- At1 ---------  -------- At2 ---------
//				word2  -------- At3 ---------  -------- Rosc ---------
//
//                  where:  At1 = tuned antenna D/A setting at 929.00625 MHz
//						    At2 = tuned antenna D/A setting at 935 MHz
//							At3 = tuned antenna D/A setting at 940.5 MHz
//					        Rosc = CP_AfcMake value (reference oscillator warp value)
//
int	CPegasus::DoTuningTest(BYTE byTestId, BYTE byDspAsicDownloadMethod,
	                       CTestModeControlRegister &rTestModeControlRegister,
	                       CTransmitRegister &rTransmitRegister,
	                       CReceiveRegister &rReceiveRegister,
	                       CTransmitWarmupRegister &rTransmitWarmupRegister,
	                       BYTE byInsertCancelTest, int nDelayTicks,
	                       BOOL bIgnoreTxWarmupRegister) 
{
  TRACE("\nCPegasus::DoTuningTest\n");
  UINT	nSum = 0;
  BYTE  *pbyTmp = NULL; 
  int lcv;

  // pager must already be in test mode or requesting it.
  if (!((IsPagerInTestMode()) || (IsTestModeRequested())))   
    return(1);
                                 
  // Upon initiation of the tuning test, the test number is set along
  // with a Dsp/Asic Register download method inidicator.  The interpretation
  // is as follows:
  //   (1) if byDspAsicRegMethod == 1, then the pager will take only the following reg data
  //   (2) if byDspAsicRegMethod == 2, then the pager will take the folliwing reg data and
  //       remain receptive to additional reg data until the test is cancelled or exited.                               
  byTestNumberBUFF[0] = byTestId;
  byRegisterIndicatorBUFF[0] = byDspAsicDownloadMethod;	
  theIoController.AddTail(new CSerialIoOperation(&theSerialPort, StartTestIOOP, 
  											  SIZEOF(StartTestIOOP), 
  										      START_TEST_OP, 
  										      TICKS(20),
											  &m_IoCallBackInterface));


  // byte-swap and set the Test Mode Control Register data
  byTestModeCtrlRegBUFF[0] = (BYTE)(rTestModeControlRegister.nTestMode >> 8);										      
  byTestModeCtrlRegBUFF[1] = (BYTE)rTestModeControlRegister.nTestMode;
  byTestModeCtrlRegBUFF[2] = (BYTE)(rTestModeControlRegister.nTestFreq >> 8);
  byTestModeCtrlRegBUFF[3] = (BYTE)rTestModeControlRegister.nTestFreq;
  byTestModeCtrlRegBUFF[4] = (BYTE)(rTestModeControlRegister.nTestData >> 8);
  byTestModeCtrlRegBUFF[5] = (BYTE)rTestModeControlRegister.nTestData;
  // calculate the Test Mode Control Register data checksum
  pbyTmp = byTestModeCtrlRegBUFF;
  for (lcv = 0; lcv < sizeof(byTestModeCtrlRegBUFF); ++lcv) 
    nSum += pbyTmp[lcv];		
  byTestModeCtrlRegCheckSumBUFF[0] = (BYTE)nSum;
  // insert the Test Mode Control Register download operation										      
  theIoController.AddTail(new CSerialIoOperation(&theSerialPort, TestModeCtrlRegIOOP, 
 										      SIZEOF(TestModeCtrlRegIOOP), 
	  										  REGISTER_DNLOAD_OP, 
	  										  TICKS(20),
											  &m_IoCallBackInterface));

  // set the Transmit Register data
  byTransmitRegBUFF[0] = rTransmitRegister.byL1;     // L1
  byTransmitRegBUFF[1] = rTransmitRegister.byL2;	 // L2
  byTransmitRegBUFF[2] = rTransmitRegister.byL3;     // L3
  byTransmitRegBUFF[3] = rTransmitRegister.byL4;     // L4
  byTransmitRegBUFF[4] = rTransmitRegister.byBat;    // bat
  byTransmitRegBUFF[5] = rTransmitRegister.byL0;     // L0
  // calculate the Transmit Register data checksum
  nSum = 0;
  pbyTmp = byTransmitRegBUFF;
  for (lcv = 0; lcv < sizeof(byTransmitRegBUFF); ++lcv) 
    nSum += pbyTmp[lcv];		
  byTransmitRegCheckSumBUFF[0] = (BYTE)nSum;
  // insert the Transmit Register download operation
  theIoController.AddTail(new CSerialIoOperation(&theSerialPort, TransmitRegIOOP, 
  											  SIZEOF(TransmitRegIOOP), 
  										      REGISTER_DNLOAD_OP, 
  										      TICKS(20),
											  &m_IoCallBackInterface));

  // set the RxTuning Register data
  byRxTuningRegBUFF[0] = rReceiveRegister.byAt1;     // At1
  byRxTuningRegBUFF[1] = rReceiveRegister.byAt2;	 // At2
  byRxTuningRegBUFF[2] = rReceiveRegister.byAt3;     // At3
  byRxTuningRegBUFF[3] = rReceiveRegister.byRosc;    // ROSC
  // calculate the RxTuning Register data checksum
  nSum = 0;
  pbyTmp = byRxTuningRegBUFF;
  for (lcv = 0; lcv < sizeof(byRxTuningRegBUFF); ++lcv) 
    nSum += pbyTmp[lcv];		
  byRxTuningRegCheckSumBUFF[0] = (BYTE)nSum;
  // insert the RxTuning Register download operation
  theIoController.AddTail(new CSerialIoOperation(&theSerialPort, RxTuningRegIOOP, 
  											  SIZEOF(RxTuningRegIOOP), 
  										      REGISTER_DNLOAD_OP, 
  										      TICKS(20),
											  &m_IoCallBackInterface));
  
  // if the user is effecting an 'AutoTune' of the PMU, then the
  // transmit warmup register may be used to cause a period of 
  // silent carrier signal prior to beginning modulation. Use
  // the bIgnoreTxWarmupRegister parameter to over-ride default
  // behavior of 'not sending the register content'.
  if (bIgnoreTxWarmupRegister == FALSE)
  {
    // set the TxWarmup Register data 
    byTxWarmupRegBUFF[0] = rTransmitWarmupRegister.byTu1;	
    byTxWarmupRegBUFF[1] = rTransmitWarmupRegister.byTu2;	
    byTxWarmupRegBUFF[2] = rTransmitWarmupRegister.byTu3;	
    byTxWarmupRegBUFF[3] = rTransmitWarmupRegister.byTu4;	
    byTxWarmupRegBUFF[4] = rTransmitWarmupRegister.byTu5;	
    byTxWarmupRegBUFF[5] = rTransmitWarmupRegister.byTu6;	
    byTxWarmupRegBUFF[6] = rTransmitWarmupRegister.byTu7;	
    byTxWarmupRegBUFF[7] = rTransmitWarmupRegister.byTu8;	
    // calculate the TxWarmup Register data checksum
    nSum = 0;
    pbyTmp = byTxWarmupRegBUFF;
    for (lcv = 0; lcv < sizeof(byTxWarmupRegBUFF); ++lcv) 
      nSum += pbyTmp[lcv];		
    byTxWarmupRegCheckSumBUFF[0] = (BYTE)nSum;
    // insert the TxWarmup Register download operation  
    theIoController.AddTail(new CSerialIoOperation(&theSerialPort, TxWarmupRegIOOP, 
  	  										       SIZEOF(TxWarmupRegIOOP), 
  										           REGISTER_DNLOAD_OP, 
  										           TICKS(20),
											       &m_IoCallBackInterface));
  }
  
  // mark end of DSP/ASIC register download
  theIoController.AddTail(new CSerialIoOperation(&theSerialPort, EndRegisterDownloadIOOP, 
  											  SIZEOF(EndRegisterDownloadIOOP), 
  										      END_REGISTER_DNLOAD_OP, 
  										      TICKS(20),
											  &m_IoCallBackInterface));
  
  // The byInsertCancelTest parameter is used to effect a 'start-run-cancel' test
  // sequence.  It is useful to obtain an MDA triggering as well as conserve
  // NiCad voltage during the tuning process.  The value of byInsertCancelTest 
  // should equal the 'Cancel Test' test-mode command (which may change with
  // pager versions).  
  if (byInsertCancelTest > 0)
  {
    byCancelPreviousTestBUFF[0] = byInsertCancelTest;
    // insert a small delay to allow pager to initiate test...
    theIoController.AddTail(new CSerialIoOperation(&theSerialPort, DelayIOOP, 
  	  										    SIZEOF(DelayIOOP), 
  										        DELAY_OP, 
  										        TICKS(nDelayTicks),
											    &m_IoCallBackInterface));
    // now cancel the test to effect a trigger...
    theIoController.AddTail(new CSerialIoOperation(&theSerialPort, CancelPreviousTestIOOP, 
  											  SIZEOF(CancelPreviousTestIOOP), 
  										      CANCEL_PREV_TEST_OP, 
  										      TICKS(10),
											  &m_IoCallBackInterface));
  }   
   
  // the pager should be placed in test mode and will continue in test
  // mode until the stop test operation is initiated.
  
  return(CPager::ExecuteIoOpList(12, "TEST"));                                           
} 


//////////////////////////////////////////////////////////////////////////////
//  DoTuningUpdate()
//
//  If the Tuning Test was initiated with the byDspAsicDownloadMethod=2, then
//  this function may be called to download and update of the Transmit and 
//  Receive DSP/ASIC registers.
//
int CPegasus::DoTuningUpdate(CTransmitRegister &rTransmitRegister,
	                         CReceiveRegister &rReceiveRegister)                     
{
  TRACE("\nCPegasus::DoTuningTest\n");
  UINT	nSum = 0;
  BYTE  *pbyTmp = NULL; 
  int lcv;

  // pager must already be in test mode or requesting it.
  if (!((IsPagerInTestMode()) || (IsTestModeRequested())))   
    return(1);
                                 
  // set the Transmit Register data
  byTransmitRegBUFF[0] = rTransmitRegister.byL1;     // L1
  byTransmitRegBUFF[1] = rTransmitRegister.byL2;	   // L2
  byTransmitRegBUFF[2] = rTransmitRegister.byL3;     // L3
  byTransmitRegBUFF[3] = rTransmitRegister.byL4;     // L4
  byTransmitRegBUFF[4] = rTransmitRegister.byBat;    // bat
  byTransmitRegBUFF[5] = rTransmitRegister.byL0;     // L0
  // calculate the Transmit Register data checksum
  nSum = 0;
  pbyTmp = byTransmitRegBUFF;
  for (lcv = 0; lcv < sizeof(byTransmitRegBUFF); ++lcv) 
    nSum += pbyTmp[lcv];		
  byTransmitRegCheckSumBUFF[0] = (BYTE)nSum;
  // insert the Transmit Register download operation
  theIoController.AddTail(new CSerialIoOperation(&theSerialPort, TransmitRegIOOP, 
  											  SIZEOF(TransmitRegIOOP), 
  										      REGISTER_DNLOAD_OP, 
  										      TICKS(20),
											  &m_IoCallBackInterface));

  // set the RxTuning Register data
  byRxTuningRegBUFF[0] = rReceiveRegister.byAt1;     // At1
  byRxTuningRegBUFF[1] = rReceiveRegister.byAt2;	 // At2
  byRxTuningRegBUFF[2] = rReceiveRegister.byAt3;     // At3
  byRxTuningRegBUFF[3] = rReceiveRegister.byRosc;    // ROSC
  // calculate the RxTuning Register data checksum
  nSum = 0;
  pbyTmp = byRxTuningRegBUFF;
  for (lcv = 0; lcv < sizeof(byRxTuningRegBUFF); ++lcv) 
    nSum += pbyTmp[lcv];		
  byRxTuningRegCheckSumBUFF[0] = (BYTE)nSum;
  // insert the RxTuning Register download operation
  theIoController.AddTail(new CSerialIoOperation(&theSerialPort, RxTuningRegIOOP, 
  											  SIZEOF(RxTuningRegIOOP), 
  										      REGISTER_DNLOAD_OP, 
  										      TICKS(20),
											  &m_IoCallBackInterface));
  return(CPager::ExecuteIoOpList(4, "TEST"));                                           
} 
    





///////////////////////////////////////////////////////////////////////
//  The following download password encode/decode mechanism adopted
//  from the tango project.  It seemed appropriate to simply 
//  copy without modification. 

static char security[] = "Q^A_RFS`BaTGUbCcVHWdDeXIYfEgZ@[h0i\\j]";

int	CPegasus::SetPagerPassword(const char *pSource)
{
  TRACE("\nCPegasus::SetPagerPassword : %s\n", pSource); 
  BYTE byTmpBUFF[11];  
  
  m_pbyPagerPassword = byPasswordBUFF;
  _fmemcpy(m_pbyPagerPassword, (BYTE *)pSource, 11);
  _fmemcpy(byTmpBUFF, (BYTE *)pSource, 11);
  // note: the password dialog has been coded to convert all
  // character input to uppercase so pSource is already in caps. 
  //_strupr((char *)byTmpBUFF);
  padRSpace((char *)byTmpBUFF, 10);
  EncodePassword((char *)byTmpBUFF, 
                 (char *)byEncodedPasswordBUFF);
  TRACE("\tEncoded=%s\n", (char *)byEncodedPasswordBUFF);
  return(0);
}



void CPegasus::EncodePassword(const char pwd[], char epwd[])
{ 
  TRACE("\nCPegasus::EncodePassword : %s : %d\n", pwd, strlen(pwd));
  int i,j,l;

  l = lstrlen(security);
  for (i=0; i < (lstrlen(pwd)); i++) {
    j = (strloc(security, (char)(pwd[i] + 16))) + i + 1;
    if (j > l-1) j -= l;
    epwd[i] = (char)(security[j] - 16);
  }
  epwd[i] = '\0';
  return;                              
}



void CPegasus::DecodePassword(const char epwd[], char pwd[])
{
  TRACE("\nCPegasus::DecodePassword : %s\n", epwd);
  int i,j;

  for (i=0; i < (lstrlen(epwd)); i++) {
    j = strloc(security, (char)(epwd[i]+16));
    if (j > i) j -= i;
    else j += lstrlen(security) - i;
    pwd[i] = (char)(security[j-1] - 16);
  }
  pwd[i] = '\0';
  
  return;
}



void CPegasus::padRSpace(char s[], int length)
{
  int i,l;

  l = lstrlen(s);
  if (l < length)
  {
    for (i=l; i < length; i++)
	 s[i] = ' ';
    s[i] = '\0';
  }

  return;
}



int CPegasus::strloc(char s[], char c)
{ 
  int i;

  for (i=0;  i < (lstrlen(s)); i++) {
    if (s[i]==c) return i;
  }
  return -1;
}
 
 
 
 
////////////////////////////////////////////////////////////////////
// GetPagerIdBlockInfo()
// 
// Builds an ASCII string of the pager info block. 
//   
void CPegasus::GetPagerIdBlockInfo(CString& rString)
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
    if (lstrcmp(cBuffer, "255") == 0)
      rString += "Tango";
    else if (lstrcmp(cBuffer, "413") == 0)
      rString += "Pegasus";
    else if (lstrcmp(cBuffer, "50A") == 0)
      rString += "PageFinder";
    else if (lstrcmp(cBuffer, "51A") == 0)
      rString += "Pegasus";
    else if (lstrcmp(cBuffer, "52A") == 0)
      rString += "Aspen";
    else 
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
      rString += "OTA_disabled";
    else if (byTemp == 'N')
      rString += "no_password_required";
    else
    {
      sprintf(cBuffer, "%02X", byTemp);
      rString += cBuffer;
    }
    rString += "    ";
  }
  
  return;
}



void CPegasus::GetProductName(CString& rString)
{
  char  cAPCBuffer[30];
  BYTE	byTemp;
  
  if ((!(GetPagerAPC((BYTE*)&cAPCBuffer[0]))) &&
      (!(GetPagerType(&byTemp))))
  { 
    cAPCBuffer[3] = '\0';
    if (lstrcmp(cAPCBuffer, "255") == 0)
      rString += "Tango";
    else if (lstrcmp(cAPCBuffer, "413") == 0) 
    {
      if (byTemp == 'A')
        rString += "Pegasus";
      else if ((byTemp == 'B') || (byTemp == 'M'))
        rString += "Centaur";
    }  
    else if (lstrcmp(cAPCBuffer, "50A") == 0)
      rString += "PageFinder";  
    else if (lstrcmp(cAPCBuffer, "51A") == 0)
      rString += "Pegasus";  
    else if (lstrcmp(cAPCBuffer, "52A") == 0)
      rString += "Aspen";  
  }
  return;
}  


int	CPegasus::GetPagerSerialNumber(BYTE *pDest)
{
  if (m_pbyPagerSerialNum == NULL) return(1);
  _fmemcpy(pDest, m_pbyPagerSerialNum, 10);
  return(0);
}    		  


int	CPegasus::GetPagerICN(BYTE *pDest)
{
  if (m_pbyPagerICN == NULL) return(1);
  _fmemcpy(pDest, m_pbyPagerICN, 10);
  return(0);
}


int	CPegasus::GetPagerStatus(BYTE *pDest)
{
  if (m_pbyPagerStatus == NULL) return(1);
  _fmemcpy(pDest, m_pbyPagerStatus, 1);
  return(0);
}        		  


BOOL CPegasus::IsCodePlugUnprogrammed()
{ 
  if (m_pbyPagerStatus == NULL) return(FALSE);
  return(*m_pbyPagerStatus == 0x55);
}


BOOL CPegasus::IsPagerDownloadDisabled()
{ 
  if (m_pbyPagerStatus == NULL) return(FALSE);
  return(*m_pbyPagerStatus == 0x44);
}


BOOL CPegasus::IsPagerPasswordRequired()
{ 
  if (m_pbyPagerStatus == NULL) return(FALSE);
  return(*m_pbyPagerStatus == 0x59);
}


BOOL CPegasus::IsPagerOtaDisabled()
{ 
  if (m_pbyPagerStatus == NULL) return(FALSE);
  return(*m_pbyPagerStatus == 0x4F);
}


int	CPegasus::GetPagerCodePlug(CByteArray& rDest)
{
  if (m_pbyPagerCodePlug == NULL) return(1); 
  for (int lcv = 0; lcv < CP_SIZE; ++lcv)
    rDest.Add(m_pbyPagerCodePlug[lcv]);
  return(0);
}


int	CPegasus::GetPagerPassword(char *pDest)
{ 
  if (m_pbyPagerPassword == NULL) return(1);
  strncpy(pDest, (char *)m_pbyPagerPassword, 11);
  return(0);
}


int	CPegasus::GetPagerDownloadControlByte(BYTE *pDest)
{
  if (m_pbyPagerDownloadControl == NULL) return(1);
  *pDest = *m_pbyPagerDownloadControl;
  return(0);
}                                                


int	CPegasus::RetrieveFromPagerIdBlock(BYTE *pDest, int nOffset, int nSize)
{
  if (m_pbyPagerIdBlock == NULL) return(1);
	  _fmemcpy(pDest, &m_pbyPagerIdBlock[nOffset], nSize);
  return(0);
}
   
   

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////


  
    

  

