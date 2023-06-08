//*************************************************************
//  File name: CODEPLUG.CPP
//
//  Description: 
//		Codeplug mapping description file 
//
//
//  Comments:
//		Based upon Pegasus Codeplug Description Document
//		AMSD-TX72_SRD_9510014, Rev 3.0, 15 May 1996 and later
//
//
// Written by Motorola AMG, PMU Engineering.
// Legacy code for reference only.
//*************************************************************


#include "StdAfx.h"
#include "..\pps.h"
#include "..\ppsdoc.h" 
#include "codeplug.h"


extern CPpsApp			theApp;		  // see files pps.h and pps.cpp

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
///
/// *** CODEPLUG FIELDS DATABASE - README PLEASE *** 
///
/// Only those fields which are accessed (read/write) by the PPS are maintained.
/// Each field has an associated array of CpRecord(s) which specify the DeviceID,
/// Codeplug Version, Field Offset, Field Length, Field Mask, and Field Bitwise-Shift.
/// If no Field Mask or Shift value applies for a field, then the enter a zero.
/// 
/// MAINTENANCE NOTE:  The RECORDS arrays are ordered by the PAGER# and CP_VER# fields
/// in increasing numerical order.  The tuple (PAGER#, CP_VER#) are the search keys.  
/// The search occurs last record to first.  Given a (DeviceId, CP_Version) request,
/// a match is returned on (PAGER# <= DeviceId) and (CP_VER# <= CP_Version). If no 
/// match occurs, the last record in the RECORDS array is returned.
///
/// When adding new records, place the entry into the array based upon (PAGER#, CP_VER#)
/// order.  It may be necessary to add entries around the new entry.  For instance,
/// given the following existing records array: 
///
///                RECORDS( CP_RevChannelMap ) 			
///                { { PEGASUS,    0x02, 0x0D9D,  7, 0x00, 0 } };      
///                FIELD( CP_RevChannelMap )
/// 
/// And, given the requirement to add support for the following change record:
///
///                  { PAGEFINDER, 0x0B, 0x0DAD,  7, 0x00, 0 },
/// 
/// The final records array will appear correctly as:
///
///                RECORDS( CP_RevChannelMap ) 			
///                { { PEGASUS,    0x02, 0x0D9D,  7, 0x00, 0 },
///                  { PAGEFINDER, 0x05, 0x0D9D,  7, 0x00, 0 },
///                  { PAGEFINDER, 0x0B, 0x0DAD,  7, 0x00, 0 },
///                  { ASPEN,      0x09, 0x0D9D,  7, 0x00, 0 } };      
///                FIELD( CP_RevChannelMap )
///
/// Notice that, before any change, the original CP_RevChannelMap records array 
/// inherently supported all PEGASUS, PAGEFINDER, and ASPEN pagers since this 
/// specific field was the same for all builds of each pager.  However, after 
/// the (PAGEFINDER, 0x0B) codeplug configuration changes, the new
/// PAGEFINDER entry cannot be simply added to the end of the records array.
/// This is because the search routine will fail to return the correct record
/// if an ASPEN unit is being used, for instance.  Consequently, 3 new entries 
/// must be made.
///
/// The first new entry continues support for all PAGEFINDER devices between
/// (PEGASUS, 0x02) and the new (PAGEFINDER, 0x0B) tuples.  The second entry
/// provides support for the new (PAGEFINDER, 0x0B+) tuples.  The third entry
/// continues support for all (ASPEN, 0x09+) tuples.  In this case (PAGEFINDER, 0x05)
/// is the first PAGEFINDER build. 
///
/// This mechanism allows the records arrays to be smaller since only changes
/// to existing records have to be added.  However, care must be taken in order
/// to ensure support for existing tuples is not lost when new tuples are added.
/// Especially, when the new tuple's PAGER# differs from the existing records
/// PAGER#.  Spend some time studying the CpField::Search routine below to 
/// understand the implications of the design.
///
  
 

#define RECORDS(name) static CpRecord name##_RECORDS[] =
 
#define FIELD(name) CpField name##(##name##_RECORDS, (sizeof(##name##_RECORDS)/sizeof(CpRecord)));
 

//	Identification and Control Block fields   
//  PAGER#   CP_VER#   OFFSET  LEN ORMASK  SHIFT               
RECORDS( CP_Serial_Number )
{ { PEGASUS,    0x01,  0x0D00, 10, 0x0000, 0 } }; 
FIELD( CP_Serial_Number )

RECORDS( CP_Version ) 			  //  NOTE:  this field should not change.		
{ { PEGASUS,    0x01, 0x0D0E,  1, 0x00, 0 } };   
FIELD( CP_Version )				

RECORDS( CP_OTA_Protected_L ) 			
{ { PEGASUS,    0x01, 0x0D0F,  1, 0x00, 0 } };
FIELD( CP_OTA_Protected_L ) 			

   // sub fields
   RECORDS( CP_rf_programming_disabled )
   { { PEGASUS,    0x01, 0x0D0F, 1, 0x80, 7 } };
   FIELD( CP_rf_programming_disabled )

RECORDS( CP_OTA_Protected_H )    //  NOTE:  this field should not change.
{ { PEGASUS,    0x01, 0x0D10,  1, 0x00, 0 } };
FIELD( CP_OTA_Protected_H ) 

RECORDS( CP_Model_Opts )
{ { PEGASUS,  0x01, 0x0D12, 1, 0x00, 0 } };
FIELD( CP_Model_Opts )
     
   // sub fields
   RECORDS( CP_send_msg_complete )
   { { PEGASUS, 0x01, 0x0D12, 1, 0x04, 2 } };
   FIELD( CP_send_msg_complete )
   
   RECORDS( CP_IOD_msg_capable )
   { { PEGASUS, 0x01, 0x0D12, 1, 0x02, 1 } };
   FIELD( CP_IOD_msg_capable )   
   
   RECORDS( CP_multi_response_capable )
   { { PEGASUS, 0x01, 0x0D12, 1, 0x01, 0 } };
   FIELD( CP_multi_response_capable )
		
RECORDS( CP_OTA_Subheader ) 			
{ { PEGASUS,    0x01, 0x0D12,  5, 0x00, 0 } };
FIELD( CP_OTA_Subheader ) 	

   RECORDS( CP_OTA_revision )
   { { PEGASUS,  0x01, 0x0D17, 1, 0x00, 0 } };
   FIELD( CP_OTA_revision )

RECORDS( CP_OTA_Password ) 				
{ { PEGASUS,    0x01, 0x0D17, 10, 0x00, 0 } };
FIELD( CP_OTA_Password ) 				

RECORDS( CP_DL_Password )  				
{ { PEGASUS,    0x01, 0x0D21, 10, 0x00, 0 } };
FIELD( CP_DL_Password )  				

RECORDS( CP_Release_Password ) 			
{ { PEGASUS,    0x01, 0x0D2B, 10, 0x00, 0 } };
FIELD( CP_Release_Password ) 			

RECORDS( CP_System_Type_1_L ) 			
{ { PEGASUS,    0x01, 0x0D35,  1, 0x00, 0 } };
FIELD( CP_System_Type_1_L ) 			

   // sub-fields
   RECORDS( CP_reflex50_enable )
   { { PEGASUS,    0x01, 0x0D35, 1, 0x40, 6 } };
   FIELD( CP_reflex50_enable )

   RECORDS( CP_dl_security )
   { { PEGASUS,    0x01, 0x0D35, 1, 0x0C, 2 } };
   FIELD( CP_dl_security )

RECORDS( CP_AP_Powerup ) 				
{ { PEGASUS,    0x01, 0x0D36, 20, 0x00, 0 } };
FIELD( CP_AP_Powerup ) 				

RECORDS( CP_Personal_Addrs ) 			
{ { PEGASUS,    0x01, 0x0D4A,  8, 0x00, 0 },
  { PAGEFINDER, 0x05, 0x0D4A,  8, 0x00, 0 },
  { PAGEFINDER, 0x80, 0x0D4A,  4, 0x00, 0 },
  { ASPEN,      0x09, 0x0D4A,  8, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0D4A,  8, 0x00, 0 },
  { EXPANDAUR,  0x80, 0x0D4A,  4, 0x00, 0 },
  { LYNX,		0x01, 0x0D4A,  8, 0x00, 0 } };
FIELD( CP_Personal_Addrs )

  RECORDS( CP_Personal_addrs )
  { { PEGASUS,    0x01, 0x0D4A, 4, 0x00, 0 } };
  FIELD( CP_Personal_addrs ) 			

RECORDS( CP_InfoSvc_Addrs ) 			
{ { PEGASUS,    0x01, 0x0D52, 24, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0D52, 24, 0x00, 0 },
  { EXPANDAUR,  0x80, 0x0D4E, 24, 0x00, 0 },
  { LYNX,       0x01, 0x0D52, 24, 0x00, 0 } };
FIELD( CP_InfoSvc_Addrs ) 
  
  RECORDS( CP_infosvc_addrs_disable )
  { { PEGASUS,    0x01, 0x0D52, 4, 0x80000000, 31 },
    { EXPANDAUR,  0x01, 0x0D52, 4, 0x80000000, 31 },
    { EXPANDAUR,  0x80, 0x0D4E, 4, 0x80000000, 31 },
    { LYNX,       0x01, 0x0D52, 4, 0x80000000, 31 } };
  FIELD( CP_infosvc_addrs_disable )
  
  RECORDS( CP_infosvc_addrs )
  { { PEGASUS,    0x01, 0x0D52, 4, 0x7FFFFFFF, 0 },
    { EXPANDAUR,  0x01, 0x0D52, 4, 0x7FFFFFFF, 0 },
    { EXPANDAUR,  0x80, 0x0D4E, 4, 0x7FFFFFFF, 0 },
    { LYNX,       0x01, 0x0D52, 4, 0x7FFFFFFF, 0 } };
  FIELD( CP_infosvc_addrs )
  
RECORDS( CP_Active_SubAddrs ) 			
{ { PEGASUS,    0x01, 0x0D6A, 24, 0x00, 0 } };
FIELD( CP_Active_SubAddrs )

  RECORDS( CP_active_subaddrs )
  { { PEGASUS,    0x01, 0x0D6A, 4, 0x00, 0 } };
  FIELD( CP_active_subaddrs ) 			



//  Protocol Control Block - Common  					
//  PAGER#   CP_VER#  OFFSET  LEN ORMASK  SHIFT               
RECORDS( CP_Control_Channel )			
{ { PEGASUS,    0x01, 0x0D85, 16, 0x00, 0 },
  { PAGEFINDER, 0x05, 0x0D85, 16, 0x00, 0 },
  { PAGEFINDER, 0x80, 0x0D85, 32, 0x00, 0 },
  { ASPEN,      0x09, 0x0D85, 16, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0D82, 16, 0x00, 0 },
  { EXPANDAUR,  0x80, 0x0D66, 32, 0x00, 0 },
  { LYNX,		0x01, 0x0D85, 16, 0x00, 0 } };   
FIELD( CP_Control_Channel )			

    // sub-fields
   RECORDS( CP_CtrlChanDisable )
   { { PEGASUS,    0x01, 0x0D85, 2, 0x8000, 15 },
     { EXPANDAUR,  0x80, 0x0D66, 2, 0x8000, 15 } };
   // this field used by Reflex25 PMU's only
   FIELD( CP_CtrlChanDisable )

   RECORDS( CP_CtrlChanScanDelay )
   { { PEGASUS,    0x01, 0x0D85, 2, 0x7000, 12 },
     { EXPANDAUR,  0x80, 0x0D66, 2, 0x7000, 12 } };
   // this field used by Reflex25 PMU's only
   FIELD( CP_CtrlChanScanDelay )

   RECORDS( CP_CtrlChanNumber )
   { { PEGASUS,    0x01, 0x0D85, 2, 0x07FF, 0 },
     { PEGASUS,    0x05, 0x0D85, 2, 0x0FFF, 0 },
     { EXPANDAUR,  0x01, 0x0D82, 2, 0x0FFF, 0 },
     { EXPANDAUR,  0x80, 0x0D66, 2, 0x0FFF, 0 },
     { LYNX,       0x01, 0x0D85, 2, 0x0FFF, 0 } };
   FIELD( CP_CtrlChanNumber )
 


//  Protocol-Specific Control Block - Reflex25		
//  PAGER#   CP_VER#  OFFSET  LEN ORMASK  SHIFT               
RECORDS( CP_Service_Provider_Ids )		
{ { PEGASUS,    0x01, 0x0D95,  8, 0x00, 0 },
  { PAGEFINDER, 0x05, 0x0D95,  8, 0x00, 0 },
  { PAGEFINDER, 0x80, 0x0DA5, 16, 0x00, 0 },
  { ASPEN,      0x09, 0x0D95,  8, 0x00, 0 },
  { EXPANDAUR,  0x80, 0x0D86, 16, 0x00, 0 } };
FIELD( CP_Service_Provider_Ids )		
          
   // Reflex25 PageFinder (0x09-) specific
   // sub-fields      
   RECORDS( CP_ServiceProviderDisable )
   { { PEGASUS,    0x01, 0x0D95, 2, 0x8000, 15 } };
   FIELD( CP_ServiceProviderDisable )

   RECORDS( CP_ServiceProviderAncillary )
   { { PEGASUS,    0x01, 0x0D95, 2, 0x4000, 14 } };
   FIELD( CP_ServiceProviderAncillary )

   RECORDS( CP_ServiceProviderId )
   { { PEGASUS,    0x01, 0x0D95, 2, 0x3FFF, 0 } };
   FIELD( CP_ServiceProviderId )  
   
   // Reflex25 PageFinder (0x80+) and Reflex25
   // Expandaur (0x80+) specific subfields
   RECORDS( CP_SpidZoneIndicator )
   { { PAGEFINDER, 0x80, 0x0DA5, 2, 0x8000, 15 },
     { EXPANDAUR,  0x80, 0x0D86, 2, 0x8000, 15 } };
   FIELD( CP_SpidZoneIndicator )
   
   RECORDS( CP_SpidAncPrimIndicator )
   { { PAGEFINDER, 0x80, 0x0DA5, 2, 0x4000, 14 },
     { EXPANDAUR,  0x80, 0x0D86, 2, 0x4000, 14 } };
   FIELD( CP_SpidAncPrimIndicator )
   
   RECORDS( CP_SpidNumber )
   { { PAGEFINDER, 0x80, 0x0DA5, 2, 0x3FFF, 0 },
     { EXPANDAUR,  0x80, 0x0D86, 2, 0x3FFF, 0 } };
   FIELD( CP_SpidNumber )
   
   RECORDS( CP_SpidZoneNumber )
   { { PAGEFINDER, 0x80, 0x0DA5, 2, 0x3FFF, 0 },
     { EXPANDAUR,  0x80, 0x0D86, 2, 0x3FFF, 0 } };
   FIELD( CP_SpidZoneNumber )

RECORDS( CP_Home_Index )				
{ { PEGASUS,    0x01, 0x0D9D,  4, 0x00, 0 },
  { PAGEFINDER, 0x05, 0x0D9D,  4, 0x00, 0 },
  { PAGEFINDER, 0x80, 0x0DB5,  4, 0x00, 0 },
  { ASPEN,      0x09, 0x0D9D,  4, 0x00, 0 },
  { EXPANDAUR,  0x80, 0x0D96,  4, 0x00, 0 } };
FIELD( CP_Home_Index )

RECORDS( CP_AckMsgPctg ) 
{ { PEGASUS,    0x02, 0x0F61,  1, 0x1F, 0 },
  { PAGEFINDER, 0x05, 0x0DA7,  1, 0x1F, 0 },
  { PAGEFINDER, 0x80, 0x0DBF,  1, 0x1F, 0 },
  { ASPEN,      0x09, 0x0DA7,  1, 0x1F, 0 },
  { EXPANDAUR,  0x80, 0x0DA0,  1, 0x1F, 0 },
  { EXPANDAUR,  0x81, 0x0D9A,  1, 0x1F, 0 } };    
FIELD( CP_AckMsgPctg )    

RECORDS( CP_RegFailRestTime )
{ { PAGEFINDER, 0x06, 0x0DA8, 1, 0x00, 0 },
  { PAGEFINDER, 0x80, 0x0DC0, 1, 0x00, 0 },
  { ASPEN,      0x09, 0x0DA8, 1, 0x00, 0 },
  { EXPANDAUR,  0x80, 0x0DA1, 1, 0x00, 0 },
  { EXPANDAUR,  0x81, 0x0D9D, 1, 0x00, 0 } };      
FIELD( CP_RegFailRestTime ) 				



//  Protocol-Specific Control Block - Reflex50         
//  PAGER#   CP_VER#  OFFSET  LEN ORMASK  SHIFT               
RECORDS( CP_Electronic_Serial_Number ) 	
{ { PEGASUS,    0x01, 0x0D0A,  4, 0x00, 0 }, 
  { PEGASUS,    0x02, 0x0D95,  4, 0x00, 0 },
  { PAGEFINDER, 0x05, 0x0D95,  4, 0x00, 0 },
  { PAGEFINDER, 0x0B, 0x0DA5,  4, 0x00, 0 },
  { PAGEFINDER, 0x0C, 0x0D95,  4, 0x00, 0 },
  { ASPEN,      0x09, 0x0D95,  4, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0D92,  4, 0x00, 0 },
  { LYNX,       0x01, 0x0D95,  4, 0x00, 0 } };    
FIELD( CP_Electronic_Serial_Number ) 	

RECORDS( CP_InfoFrame ) 				
{ { PEGASUS,    0x02, 0x0D99,  1, 0x00, 0 },
  { PAGEFINDER, 0x05, 0x0D99,  1, 0x00, 0 },
  { PAGEFINDER, 0x0B, 0x0DA9,  1, 0x00, 0 },
  { PAGEFINDER, 0x0C, 0x0D99,  1, 0x00, 0 },
  { ASPEN,      0x09, 0x0D99,  1, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0D96,  1, 0x00, 0 },
  { LYNX,       0x01, 0x0D99,  1, 0x00, 0 } };		 
FIELD( CP_InfoFrame ) 				

RECORDS( CP_PagerFrame ) 				
{ { PEGASUS,    0x02, 0x0D9A,  1, 0x00, 0 },
  { PAGEFINDER, 0x05, 0x0D9A,  1, 0x00, 0 },
  { PAGEFINDER, 0x0B, 0x0DAA,  1, 0x00, 0 },
  { PAGEFINDER, 0x0C, 0x0D9A,  1, 0x00, 0 },
  { ASPEN,      0x09, 0x0D9A,  1, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0D97,  1, 0x00, 0 },
  { LYNX,       0x01, 0x0D9A,  1, 0x00, 0 } };   	
FIELD( CP_PagerFrame ) 				             

RECORDS( CP_HomeLogicalNum ) 			
{ { PEGASUS,    0x02, 0x0D9B,  1, 0x00, 0 },
  { PAGEFINDER, 0x05, 0x0D9B,  1, 0x00, 0 },
  { PAGEFINDER, 0x0B, 0x0DAB,  1, 0x00, 0 },
  { PAGEFINDER, 0x0C, 0x0D9B,  1, 0x00, 0 },
  { ASPEN,      0x09, 0x0D9B,  1, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0D98,  1, 0x00, 0 },
  { LYNX,       0x01, 0x0D9B,  1, 0x00, 0 } };      
FIELD( CP_HomeLogicalNum ) 			

RECORDS( CP_PagerCollapse ) 			
{ { PEGASUS,    0x02, 0x0D9C,  1, 0x00, 0 },
  { PAGEFINDER, 0x05, 0x0D9C,  1, 0x00, 0 },
  { PAGEFINDER, 0x0B, 0x0DAC,  1, 0x00, 0 },
  { PAGEFINDER, 0x0C, 0x0D9C,  1, 0x00, 0 },
  { ASPEN,      0x09, 0x0D9C,  1, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0D99,  1, 0x00, 0 },
  { LYNX,       0x01, 0x0D9C,  1, 0x00, 0 } };      
FIELD( CP_PagerCollapse ) 			             

   // sub-fields
   RECORDS( CP_InfoServiceCollapse )
   { { PEGASUS,    0x02, 0x0D9C, 1, 0x70, 4 },
     { PAGEFINDER, 0x05, 0x0D9C, 1, 0x70, 4 },
     { PAGEFINDER, 0x0B, 0x0DAC, 1, 0x70, 4 },
     { PAGEFINDER, 0x0C, 0x0D9C, 1, 0x70, 4 },
     { ASPEN,      0x09, 0x0D9C, 1, 0x70, 4 },
     { EXPANDAUR,  0x01, 0x0D99, 1, 0x70, 4 },
     { LYNX,       0x01, 0x0D9C, 1, 0x70, 4 } };    
   FIELD( CP_InfoServiceCollapse )

   RECORDS( CP_PersonalCollapse )
   { { PEGASUS,    0x02, 0x0D9C, 1, 0x07, 0 },
     { PAGEFINDER, 0x05, 0x0D9C, 1, 0x07, 0 },
     { PAGEFINDER, 0x0B, 0x0DAC, 1, 0x07, 0 },
     { PAGEFINDER, 0x0C, 0x0D9C, 1, 0x07, 0 },
     { ASPEN,      0x09, 0x0D9C, 1, 0x07, 0 },
     { EXPANDAUR,  0x01, 0x0D99, 1, 0x07, 0 },
     { LYNX,       0x01, 0x0D9C, 1, 0x07, 0 } };    
   FIELD( CP_PersonalCollapse )

RECORDS( CP_RevChannelMap ) 			
{ { PEGASUS,    0x02, 0x0D9D,  7, 0x00, 0 },
  { PAGEFINDER, 0x05, 0x0D9D,  7, 0x00, 0 },
  { PAGEFINDER, 0x0B, 0x0DAD,  7, 0x00, 0 },
  { PAGEFINDER, 0x0C, 0x0D9D,  7, 0x00, 0 },
  { ASPEN,      0x09, 0x0D9D,  7, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0D9A,  7, 0x00, 0 },
  { LYNX,       0x01, 0x0D9D,  7, 0x00, 0 } };      
FIELD( CP_RevChannelMap )

  RECORDS( CP_RevChannelDisable )
  { { PEGASUS,    0x02, 0x0D9D, 1, 0x80, 7 },
    { PAGEFINDER, 0x05, 0x0D9D, 1, 0x80, 7 },
    { PAGEFINDER, 0x0B, 0x0DAD, 1, 0x80, 7 },
    { PAGEFINDER, 0x0C, 0x0D9D, 1, 0x80, 7 },
    { ASPEN,      0x09, 0x0D9D, 1, 0x80, 7 },
    { EXPANDAUR,  0x01, 0x0D9A, 1, 0x80, 7 },
    { LYNX,       0x01, 0x0D9D, 1, 0x80, 7 } };     
  FIELD( CP_RevChannelDisable )

  RECORDS( CP_RevChannelNum )
  { { PEGASUS,    0x02, 0x0D9D, 1, 0x7F, 0 },
    { PAGEFINDER, 0x05, 0x0D9D, 1, 0x7F, 0 },
    { PAGEFINDER, 0x0B, 0x0DAD, 1, 0x7F, 0 },
    { PAGEFINDER, 0x0C, 0x0D9D, 1, 0x7F, 0 },
    { ASPEN,      0x09, 0x0D9D, 1, 0x7F, 0 },
    { EXPANDAUR,  0x01, 0x0D9A, 1, 0x7F, 0 },
    { LYNX,       0x01, 0x0D9D, 1, 0x7F, 0 } };     
  FIELD( CP_RevChannelNum ) 			

RECORDS( CP_R50_Misc ) 					
{ { PEGASUS,    0x02, 0x0DA4,  1, 0x00, 0 },
  { PAGEFINDER, 0x05, 0x0DA4,  1, 0x00, 0 },
  { PAGEFINDER, 0x0B, 0x0DB4,  1, 0x00, 0 },
  { PAGEFINDER, 0x0C, 0x0DA4,  1, 0x00, 0 },
  { ASPEN,      0x09, 0x0DA4,  1, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0DA1,  1, 0x00, 0 },
  { LYNX,       0x01, 0x0DA4,  1, 0x00, 0 } };      
FIELD( CP_R50_Misc ) 					

   // sub_fields  
   RECORDS( CP_channel_id )
   { { PEGASUS,    0x02, 0x0DA4, 1, 0x04, 2 },
     { PAGEFINDER, 0x05, 0x0DA4, 1, 0x04, 2 },
     { PAGEFINDER, 0x0B, 0x0DB4, 1, 0x04, 2 },
     { PAGEFINDER, 0x0C, 0x0DA4, 1, 0x04, 2 },
     { ASPEN,      0x09, 0x0DA4, 1, 0x04, 2 },
     { EXPANDAUR,  0x01, 0x0DA1, 1, 0x04, 2 },
     { LYNX,       0x01, 0x0DA4, 1, 0x04, 2 } };    
   FIELD( CP_channel_id )
   
   RECORDS( CP_notify_tx_change )
   { { PEGASUS,    0x02, 0x0DA4, 1, 0x02, 1 },
     { PAGEFINDER, 0x05, 0x0DA4, 1, 0x02, 1 },
     { PAGEFINDER, 0x0B, 0x0DB4, 1, 0x02, 1 },
     { PAGEFINDER, 0x0C, 0x0DA4, 1, 0x02, 1 },
     { ASPEN,      0x09, 0x0DA4, 1, 0x02, 1 },
     { EXPANDAUR,  0x01, 0x0DA1, 1, 0x02, 1 },
     { LYNX,       0x01, 0x0DA4, 1, 0x02, 1 } };    
   FIELD( CP_notify_tx_change )

   RECORDS( CP_allow_auto_reg )
   { { PEGASUS,    0x02, 0x0DA4, 1, 0x01, 0 },
     { PAGEFINDER, 0x05, 0x0DA4, 1, 0x01, 0 },
     { PAGEFINDER, 0x0B, 0x0DB4, 1, 0x01, 0 },
     { PAGEFINDER, 0x0C, 0x0DA4, 1, 0x01, 0 },
     { ASPEN,      0x09, 0x0DA4, 1, 0x01, 0 },
     { EXPANDAUR,  0x01, 0x0DA1, 1, 0x01, 0 },
     { LYNX,       0x01, 0x0DA4, 1, 0x01, 0 } };    
   FIELD( CP_allow_auto_reg )

RECORDS( CP_IncommunicadoTmr ) 			
{ { PEGASUS,    0x02, 0x0DA5,  2, 0x00, 0 },
  { PAGEFINDER, 0x05, 0x0DA5,  2, 0x00, 0 },
  { PAGEFINDER, 0x0B, 0x0DB5,  2, 0x00, 0 },
  { PAGEFINDER, 0x0C, 0x0DA5,  2, 0x00, 0 },
  { ASPEN,      0x09, 0x0DA5,  2, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0DA2,  2, 0x00, 0 },
  { LYNX,       0x01, 0x0DA5,  2, 0x00, 0 } };    
FIELD( CP_IncommunicadoTmr ) 			

RECORDS( CP_CommunicadoTmr ) 			
{ { PEGASUS,    0x02, 0x0DA7,  2, 0x00, 0 },
  { PAGEFINDER, 0x05, 0x0DA7,  2, 0x00, 0 },
  { PAGEFINDER, 0x0B, 0x0DB7,  2, 0x00, 0 },
  { PAGEFINDER, 0x0C, 0x0DA7,  2, 0x00, 0 },
  { ASPEN,      0x09, 0x0DA7,  2, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0DA4,  2, 0x00, 0 },
  { LYNX,       0x01, 0x0DA7,  2, 0x00, 0 } };   
FIELD( CP_CommunicadoTmr ) 			

RECORDS( CP_AlohaTimeout ) 				
{ { PEGASUS,    0x02, 0x0DA9,  2, 0x00, 0 },
  { PAGEFINDER, 0x05, 0x0DA9,  2, 0x00, 0 },
  { PAGEFINDER, 0x0B, 0x0DB9,  2, 0x00, 0 },
  { PAGEFINDER, 0x0C, 0x0DA9,  2, 0x00, 0 },
  { ASPEN,      0x09, 0x0DA9,  2, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0DA6,  2, 0x00, 0 },
  { LYNX,       0x01, 0x0DA9,  2, 0x00, 0 } };   
FIELD( CP_AlohaTimeout ) 				

RECORDS( CP_Retry_Cnt ) 				
{ { PEGASUS,    0x02, 0x0DAB,  2, 0x00, 0 },
  { PAGEFINDER, 0x05, 0x0DAB,  2, 0x00, 0 },
  { PAGEFINDER, 0x0B, 0x0DBB,  2, 0x00, 0 },
  { PAGEFINDER, 0x0C, 0x0DAB,  2, 0x00, 0 },
  { ASPEN,      0x09, 0x0DAB,  2, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0DA8,  2, 0x00, 0 },
  { LYNX,       0x01, 0x0DAB,  2, 0x00, 0 } };    
FIELD( CP_Retry_Cnt )

RECORDS( CP_AlohaRndInterval )
{ { PEGASUS,    0x02, 0x0DAD,  1, 0x00, 0 },
  { PAGEFINDER, 0x05, 0x0DAD,  1, 0x00, 0 },
  { PAGEFINDER, 0x0B, 0x0DBD,  1, 0x00, 0 },
  { PAGEFINDER, 0x0C, 0x0DAD,  1, 0x00, 0 },
  { ASPEN,      0x09, 0x0DAD,  1, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0DAA,  1, 0x00, 0 },
  { LYNX,       0x01, 0x0DAD,  1, 0x00, 0 } };      
FIELD( CP_AlohaRndInterval ) 				

RECORDS( CP_AggReg_Attack ) 			
{ { PEGASUS,    0x02, 0x0DAE,  1, 0x00, 0 },
  { PAGEFINDER, 0x05, 0x0DAE,  1, 0x00, 0 },
  { PAGEFINDER, 0x0B, 0x0DBE,  1, 0x00, 0 },
  { PAGEFINDER, 0x0C, 0x0DAE,  1, 0x00, 0 },
  { ASPEN,      0x09, 0x0DAE,  1, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0DAB,  1, 0x00, 0 },
  { LYNX,       0x01, 0x0DAE,  1, 0x00, 0 } };      
FIELD( CP_AggReg_Attack ) 			

RECORDS( CP_AggReg_Ceiling ) 			
{ { PEGASUS,    0x02, 0x0DAF,  2, 0x00, 0 },
  { PAGEFINDER, 0x05, 0x0DAF,  2, 0x00, 0 },
  { PAGEFINDER, 0x0B, 0x0DBF,  2, 0x00, 0 },
  { PAGEFINDER, 0x0C, 0x0DAF,  2, 0x00, 0 },
  { ASPEN,      0x09, 0x0DAF,  2, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0DAC,  2, 0x00, 0 },
  { LYNX,       0x01, 0x0DAF,  2, 0x00, 0 } };    
FIELD( CP_AggReg_Ceiling ) 			

RECORDS( CP_AggReg_RetryTmr ) 			
{ { PEGASUS,    0x02, 0x0DB1,  2, 0x00, 0 },
  { PAGEFINDER, 0x05, 0x0DB1,  2, 0x00, 0 },
  { PAGEFINDER, 0x0B, 0x0DC1,  2, 0x00, 0 },
  { PAGEFINDER, 0x0C, 0x0DB1,  2, 0x00, 0 },
  { ASPEN,      0x09, 0x0DB1,  2, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0DAE,  2, 0x00, 0 },
  { LYNX,       0x01, 0x0DB1,  2, 0x00, 0 } };    
FIELD( CP_AggReg_RetryTmr ) 			

RECORDS( CP_AggReg_RetryCnt ) 			
{ { PEGASUS,    0x02, 0x0DB3,  2, 0x00, 0 },
  { PAGEFINDER, 0x05, 0x0DB3,  2, 0x00, 0 },
  { PAGEFINDER, 0x0B, 0x0DC3,  2, 0x00, 0 },
  { PAGEFINDER, 0x0C, 0x0DB3,  2, 0x00, 0 },
  { ASPEN,      0x09, 0x0DB3,  2, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0DB0,  2, 0x00, 0 },
  { LYNX,       0x01, 0x0DB3,  2, 0x00, 0 } };    
FIELD( CP_AggReg_RetryCnt ) 			


//  Host Control Block                                  
//  PAGER#   CP_VER#  OFFSET  LEN ORMASK  SHIFT               
RECORDS( CP_Inventory_Control_Number ) 	
{ { PEGASUS,    0x01, 0x0DA8, 10, 0x00, 0 },
  { PEGASUS,    0x02, 0x0DB8, 10, 0x00, 0 },
  { PAGEFINDER, 0x05, 0x0DB8, 10, 0x00, 0 },
  { PAGEFINDER, 0x0B, 0x0DC8, 10, 0x00, 0 },
  { PAGEFINDER, 0x0C, 0x0DB8, 10, 0x00, 0 },
  { PAGEFINDER, 0x80, 0x0DC8, 10, 0x00, 0 },
  { ASPEN,      0x09, 0x0DB8, 10, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0DB5, 10, 0x00, 0 },
  { LYNX,       0x01, 0x0DB8, 10, 0x00, 0 } };      
FIELD( CP_Inventory_Control_Number ) 	

RECORDS( CP_Programmable_Pleasing_Alert ) 
{ { PEGASUS,    0x01, 0x0DB2, 16, 0x00, 0 },
  { PEGASUS,    0x02, 0x0DC2, 16, 0x00, 0 },
  { PAGEFINDER, 0x05, 0x0DC2, 16, 0x00, 0 },
  { PAGEFINDER, 0x0B, 0x0DD2, 16, 0x00, 0 },
  { PAGEFINDER, 0x0C, 0x0DC2, 16, 0x00, 0 },
  { PAGEFINDER, 0x80, 0x0DD2, 16, 0x00, 0 },
  { ASPEN,      0x09, 0x0DC2, 16, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0DBF, 16, 0x00, 0 },
  { LYNX,       0x01, 0x0DC2, 16, 0x00, 0 } };      
FIELD( CP_Programmable_Pleasing_Alert ) 

   // sub-fields
   RECORDS( CP_volume )
   { { PEGASUS,    0x01, 0x0DB2, 1, 0xE0, 5 },
     { PEGASUS,    0x02, 0x0DC2, 1, 0xE0, 5 },
     { PAGEFINDER, 0x05, 0x0DC2, 1, 0xE0, 5 },
     { PAGEFINDER, 0x0B, 0x0DD2, 1, 0xE0, 5 },
     { PAGEFINDER, 0x0C, 0x0DC2, 1, 0xE0, 5 },
     { PAGEFINDER, 0x80, 0x0DD2, 1, 0xE0, 5 },
     { ASPEN,      0x09, 0x0DC2, 1, 0xE0, 5 },
     { EXPANDAUR,  0x01, 0x0DBF, 1, 0xE0, 5 },
     { LYNX,       0x01, 0x0DC2, 1, 0xE0, 5 } };    
   FIELD( CP_volume )

   RECORDS( CP_FrequencyIndex ) 
   { { PEGASUS,    0x01, 0x0DB2, 1, 0x1F, 0 },
     { PEGASUS,    0x02, 0x0DC2, 1, 0x1F, 0 },
     { PAGEFINDER, 0x05, 0x0DC2, 1, 0x1F, 0 },
     { PAGEFINDER, 0x0B, 0x0DD2, 1, 0x1F, 0 },
     { PAGEFINDER, 0x0C, 0x0DC2, 1, 0x1F, 0 },
     { PAGEFINDER, 0x80, 0x0DD2, 1, 0x1F, 0 },
     { ASPEN,      0x09, 0x0DC2, 1, 0x1F, 0 },
     { EXPANDAUR,  0x01, 0x0DBF, 1, 0x1F, 0 },
     { LYNX,       0x01, 0x0DC2, 1, 0x1F, 0 } };    
   FIELD( CP_FrequencyIndex ) 

RECORDS( CP_Alert_Frequency )			
{ { PEGASUS,    0x01, 0x0DC2,  1, 0x00, 0 }, 
  { PEGASUS,    0x02, 0x0DD2,  1, 0x00, 0 },
  { PAGEFINDER, 0x05, 0x0DD2,  1, 0x00, 0 },
  { PAGEFINDER, 0x0B, 0x0DE2,  1, 0x00, 0 },
  { PAGEFINDER, 0x0C, 0x0DD2,  1, 0x00, 0 },
  { PAGEFINDER, 0x80, 0x0DE2,  1, 0x00, 0 },
  { ASPEN,      0x09, 0x0DD2,  1, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0DCF,  1, 0x00, 0 },
  { LYNX,       0x01, 0x0DD2,  1, 0x00, 0 } };     
FIELD( CP_Alert_Frequency )			

RECORDS( CP_Format_Delimiter ) 			
{ { PEGASUS,    0x01, 0x0DCF,  1, 0x00, 0 },
  { PEGASUS,    0x02, 0x0DDF,  1, 0x00, 0 },
  { PEGASUS,    0x04, 0x0DD3,  1, 0x00, 0 },
  { PAGEFINDER, 0x05, 0x0DD3,  1, 0x00, 0 },
  { PAGEFINDER, 0x0B, 0x0DE3,  1, 0x00, 0 },
  { PAGEFINDER, 0x0C, 0x0DD3,  1, 0x00, 0 }, 
  { PAGEFINDER, 0x80, 0x0DE3,  1, 0x00, 0 },
  { ASPEN,      0x09, 0x0DD3,  1, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0DD0,  1, 0x00, 0 },
  { LYNX,       0x01, 0x0DD3,  1, 0x00, 0 } };      
FIELD( CP_Format_Delimiter ) 			

RECORDS( CP_Delimiter_Location ) 		
{ { PEGASUS,    0x01, 0x0DD0,  4, 0x00, 0 },
  { PEGASUS,    0x02, 0x0DE0,  4, 0x00, 0 },
  { PEGASUS,    0x04, 0x0DD4,  4, 0x00, 0 },
  { PAGEFINDER, 0x05, 0x0DD4,  4, 0x00, 0 },
  { PAGEFINDER, 0x0B, 0x0DE4,  4, 0x00, 0 },
  { PAGEFINDER, 0x0C, 0x0DD4,  4, 0x00, 0 }, 
  { PAGEFINDER, 0x80, 0x0DE4,  4, 0x00, 0 },
  { ASPEN,      0x09, 0x0DD4,  4, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0DD1,  4, 0x00, 0 },
  { LYNX,       0x01, 0x0DD4,  4, 0x00, 0 } };    
FIELD( CP_Delimiter_Location )

  RECORDS( CP_delimiter_location )
  { { PEGASUS,    0x01, 0x0DD0,  1, 0x00, 0 },
    { PEGASUS,    0x02, 0x0DE0,  1, 0x00, 0 },
    { PEGASUS,    0x04, 0x0DD4,  1, 0x00, 0 },
    { PAGEFINDER, 0x05, 0x0DD4,  1, 0x00, 0 },
    { PAGEFINDER, 0x0B, 0x0DE4,  1, 0x00, 0 },
    { PAGEFINDER, 0x0C, 0x0DD4,  1, 0x00, 0 },
    { PAGEFINDER, 0x80, 0x0DE4,  1, 0x00, 0 },
    { ASPEN,      0x09, 0x0DD4,  1, 0x00, 0 },
    { EXPANDAUR,  0x01, 0x0DD1,  1, 0x00, 0 },
    { LYNX,       0x01, 0x0DD4,  1, 0x00, 0 } };    
  FIELD( CP_delimiter_location )

RECORDS( CP_Function1_L ) 				
{ { PEGASUS,    0x01, 0x0DD4,  1, 0x00, 0 },
  { PEGASUS,    0x02, 0x0DE4,  1, 0x00, 0 },
  { PEGASUS,    0x04, 0x0DD8,  1, 0x00, 0 },
  { PAGEFINDER, 0x05, 0x0DD8,  1, 0x00, 0 },  
  { PAGEFINDER, 0x0B, 0x0DE8,  1, 0x00, 0 },
  { PAGEFINDER, 0x0C, 0x0DD8,  1, 0x00, 0 },
  { PAGEFINDER, 0x80, 0x0DE8,  1, 0x00, 0 },
  { ASPEN,      0x09, 0x0DD8,  1, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0DD5,  1, 0x00, 0 },
  { LYNX,       0x01, 0x0DD8,  1, 0x00, 0 } };      
FIELD( CP_Function1_L ) 				

   // sub-fields
   RECORDS( CP_oor_indicator )
   { { PEGASUS,    0x01, 0x0DD4, 1, 0x80, 7 },
     { PEGASUS,    0x02, 0x0DE4, 1, 0x80, 7 } };
     // this bit field removed with CP_Version=4
     // replaced with 'oor_icon' in location CP_Display_L
   FIELD( CP_oor_indicator )

   RECORDS( CP_mult_choice_enable )
   { { PEGASUS,    0x01, 0x0DD4, 1, 0x40, 6 },
     { PEGASUS,    0x02, 0x0DE4, 1, 0x40, 6 },
     { PEGASUS,    0x04, 0x0DD8, 1, 0x40, 6 },
     { PAGEFINDER, 0x05, 0x0DD8, 1, 0x40, 6 },
     { PAGEFINDER, 0x0B, 0x0DE8, 1, 0x40, 6 },
     { PAGEFINDER, 0x0C, 0x0DD8, 1, 0x40, 6 },
     { PAGEFINDER, 0x80, 0x0DE8, 1, 0x40, 6 },
     { ASPEN,      0x09, 0x0DD8, 1, 0x40, 6 },
     { EXPANDAUR,  0x01, 0x0DD5, 1, 0x40, 6 },
     { LYNX,       0x01, 0x0DD8, 1, 0x40, 6 } };    
   FIELD( CP_mult_choice_enable )
   
   // Note:  this field new with (PAGEFINDER, 0x0B).
   // Now has meaning of Info-on-Demand capable. 9/18/97
   RECORDS( CP_IOD_msg_enable ) 
   { { PEGASUS,    0x01, 0x0DD4, 1, 0x20, 5 },
     { PEGASUS,    0x02, 0x0DE4, 1, 0x20, 5 },
     { PEGASUS,    0x04, 0x0DD8, 1, 0x20, 5 },
     { PAGEFINDER, 0x05, 0x0DD8, 1, 0x20, 5 },
     { PAGEFINDER, 0x0B, 0x0DE8, 1, 0x20, 5 },
     { PAGEFINDER, 0x0C, 0x0DD8, 1, 0x20, 5 },
     { PAGEFINDER, 0x80, 0x0DE8, 1, 0x20, 5 },
     { ASPEN,      0x09, 0x0DD8, 1, 0x20, 5 },
     { EXPANDAUR,  0x01, 0x0DD5, 1, 0x20, 5 },
     { LYNX,       0x01, 0x0DD8, 1, 0x20, 5 } };
   FIELD( CP_IOD_msg_enable )
   
   // Note:  this field swapped bits with 
   // (PAGEFINDER, 0x0B).  Now means Long-Reverse
   // capable.                                  9/18/97
   RECORDS( CP_send_msg_enable )
   { { PAGEFINDER, 0x05, 0x0DD8, 1, 0x20, 5 },
     { PAGEFINDER, 0x0B, 0x0DE8, 1, 0x10, 4 }, 
     { PAGEFINDER, 0x0C, 0x0DD8, 1, 0x10, 4 },
     { PAGEFINDER, 0x80, 0x0DE8, 1, 0x10, 4 },
     { ASPEN,      0x09, 0x0DD8, 1, 0x10, 4 },
     { EXPANDAUR,  0x01, 0x0DD5, 1, 0x10, 4 },
     { LYNX,       0x01, 0x0DD8, 1, 0x10, 4 } };    
   FIELD( CP_send_msg_enable )

   RECORDS( CP_non_deaf_pager )
   { { PEGASUS,    0x01, 0x0DD4, 1, 0x08, 3 },
     { PEGASUS,    0x02, 0x0DE4, 1, 0x08, 3 },
     { PEGASUS,    0x04, 0x0DD8, 1, 0x08, 3 },
     { PAGEFINDER, 0x05, 0x0DD8, 1, 0x08, 3 },
     { PAGEFINDER, 0x0B, 0x0DE8, 1, 0x08, 3 },
     { PAGEFINDER, 0x0C, 0x0DD8, 1, 0x08, 3 },
     { PAGEFINDER, 0x80, 0x0DE8, 1, 0x08, 3 },
     { ASPEN,      0x09, 0x0DD8, 1, 0x08, 3 },
     { EXPANDAUR,  0x01, 0x0DD5, 1, 0x08, 3 },
     { LYNX,       0x01, 0x0DD8, 1, 0x08, 3 } };    
   FIELD( CP_non_deaf_pager )

   RECORDS( CP_enable_all_alerts )
   { { PEGASUS,    0x01, 0x0DD4, 1, 0x04, 2 },
     { PEGASUS,    0x02, 0x0DE4, 1, 0x04, 2 },
     { PEGASUS,    0x04, 0x0DD8, 1, 0x04, 2 },
     { PAGEFINDER, 0x05, 0x0DD8, 1, 0x04, 2 },
     { PAGEFINDER, 0x0B, 0x0DE8, 1, 0x04, 2 },
     { PAGEFINDER, 0x0C, 0x0DD8, 1, 0x04, 2 },
     { PAGEFINDER, 0x80, 0x0DE8, 1, 0x04, 2 },
     { ASPEN,      0x09, 0x0DD8, 1, 0x04, 2 },
     { EXPANDAUR,  0x01, 0x0DD5, 1, 0x04, 2 },
     { LYNX,       0x01, 0x0DD8, 1, 0x04, 2 } };    
   FIELD( CP_enable_all_alerts )
                              
   RECORDS( CP_msg_read_by_user )
   { { PEGASUS,    0x01, 0x0DD4, 1, 0x03, 0 },
     { PEGASUS,    0x02, 0x0DE4, 1, 0x03, 0 },
     { PEGASUS,    0x04, 0x0DD8, 1, 0x03, 0 },
     { PAGEFINDER, 0x05, 0x0DD8, 1, 0x03, 0 },
     { PAGEFINDER, 0x0B, 0x0DE8, 1, 0x03, 0 },
     { PAGEFINDER, 0x0C, 0x0DD8, 1, 0x03, 0 },
     { PAGEFINDER, 0x80, 0x0DE8, 1, 0x03, 0 },
     { ASPEN,      0x09, 0x0DD8, 1, 0x03, 0 },
     { EXPANDAUR,  0x01, 0x0DD5, 1, 0x03, 0 },
     { LYNX,       0x01, 0x0DD8, 1, 0x03, 0 } };   
   FIELD( CP_msg_read_by_user )

RECORDS( CP_Memory_Opts_L ) 			
{ { PEGASUS,    0x01, 0x0DD5,  1, 0x00, 0 }, 
  { PEGASUS,    0x02, 0x0DE5,  1, 0x00, 0 },
  { PEGASUS,    0x04, 0x0DD9,  1, 0x00, 0 },
  { PAGEFINDER, 0x05, 0x0DD9,  1, 0x00, 0 },
  { PAGEFINDER, 0x0B, 0x0DE9,  1, 0x00, 0 },
  { PAGEFINDER, 0x0C, 0x0DD9,  1, 0x00, 0 },
  { PAGEFINDER, 0x80, 0x0DE9,  1, 0x00, 0 },
  { ASPEN,      0x09, 0x0DD9,  1, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0DD6,  1, 0x00, 0 },
  { LYNX,       0x01, 0x0DD9,  1, 0x00, 0 } };      
FIELD( CP_Memory_Opts_L ) 			

   // sub-fields
   RECORDS( CP_periodic_ram_check )
   { { PEGASUS,    0x01, 0x0DD5, 1, 0x80, 7 },
     { PEGASUS,    0x02, 0x0DE5, 1, 0x80, 7 },
     { PEGASUS,    0x04, 0x0DD9, 1, 0x80, 7 },
     { PAGEFINDER, 0x05, 0x0DD9, 1, 0x80, 7 },
     { PAGEFINDER, 0x0B, 0x0DE9, 1, 0x80, 7 },
     { PAGEFINDER, 0x0C, 0x0DD9, 1, 0x80, 7 }, 
     { PAGEFINDER, 0x80, 0x0DE9, 1, 0x80, 7 },
     { ASPEN,      0x09, 0x0DD9, 1, 0x80, 7 },
     { EXPANDAUR,  0x01, 0x0DD6, 1, 0x80, 7 },
     { LYNX,       0x01, 0x0DD9, 1, 0x80, 7 } };    
   FIELD( CP_periodic_ram_check )

   RECORDS( CP_retain_errored_alpha )
   { { PEGASUS,    0x01, 0x0DD5, 1, 0x40, 6 },
     { PEGASUS,    0x02, 0x0DE5, 1, 0x40, 6 },
     { PEGASUS,    0x04, 0x0DD9, 1, 0x40, 6 },
     { PAGEFINDER, 0x05, 0x0DD9, 1, 0x40, 6 },
     { PAGEFINDER, 0x0B, 0x0DE9, 1, 0x40, 6 },
     { PAGEFINDER, 0x0C, 0x0DD9, 1, 0x40, 6 },
     { PAGEFINDER, 0x80, 0x0DE9, 1, 0x40, 6 },
     { ASPEN,      0x09, 0x0DD9, 1, 0x40, 6 },
     { EXPANDAUR,  0x01, 0x0DD6, 1, 0x40, 6 },
     { LYNX,       0x01, 0x0DD9, 1, 0x40, 6 } };    
   FIELD( CP_retain_errored_alpha )

   RECORDS( CP_retain_errored_numeric )
   { { PEGASUS,    0x01, 0x0DD5, 1, 0x20, 5 },
     { PEGASUS,    0x02, 0x0DE5, 1, 0x20, 5 },
     { PEGASUS,    0x04, 0x0DD9, 1, 0x20, 5 },
     { PAGEFINDER, 0x05, 0x0DD9, 1, 0x20, 5 },
     { PAGEFINDER, 0x0B, 0x0DE9, 1, 0x20, 5 },
     { PAGEFINDER, 0x0C, 0x0DD9, 1, 0x20, 5 },
     { PAGEFINDER, 0x80, 0x0DE9, 1, 0x20, 5 },
     { ASPEN,      0x09, 0x0DD9, 1, 0x20, 5 },
     { EXPANDAUR,  0x01, 0x0DD6, 1, 0x20, 5 },
     { LYNX,       0x01, 0x0DD9, 1, 0x20, 5 } };    
   FIELD( CP_retain_errored_numeric )

   RECORDS( CP_retain_errored_transparent )
   { { PEGASUS,    0x01, 0x0DD5, 1, 0x10, 4 },
     { PEGASUS,    0x02, 0x0DE5, 1, 0x10, 4 },
     { PEGASUS,    0x04, 0x0DD9, 1, 0x10, 4 },
     { PAGEFINDER, 0x05, 0x0DD9, 1, 0x10, 4 },
     { PAGEFINDER, 0x0B, 0x0DE9, 1, 0x10, 4 },
     { PAGEFINDER, 0x0C, 0x0DD9, 1, 0x10, 4 },
     { PAGEFINDER, 0x80, 0x0DE9, 1, 0x10, 4 },
     { ASPEN,      0x09, 0x0DD9, 1, 0x10, 4 },
     { EXPANDAUR,  0x01, 0x0DD6, 1, 0x10, 4 },
     { LYNX,       0x01, 0x0DD9, 1, 0x10, 4 } };    
   FIELD( CP_retain_errored_transparent )

   RECORDS( CP_retain_errored_2way )
   { { PEGASUS,    0x01, 0x0DD5, 1, 0x08, 3 },
     { PEGASUS,    0x02, 0x0DE5, 1, 0x08, 3 },
     { PEGASUS,    0x04, 0x0DD9, 1, 0x08, 3 },
     { PAGEFINDER, 0x05, 0x0DD9, 1, 0x08, 3 },
     { PAGEFINDER, 0x0B, 0x0DE9, 1, 0x08, 3 },
     { PAGEFINDER, 0x0C, 0x0DD9, 1, 0x08, 3 },
     { PAGEFINDER, 0x80, 0x0DE9, 1, 0x08, 3 },
     { ASPEN,      0x09, 0x0DD9, 1, 0x08, 3 },
     { EXPANDAUR,  0x01, 0x0DD6, 1, 0x08, 3 },
     { LYNX,       0x01, 0x0DD9, 1, 0x08, 3 } };    
   FIELD( CP_retain_errored_2way )
   
   RECORDS( CP_power_on_ram_check )
   { { PEGASUS,    0x04, 0x0DD9, 1, 0x04, 2 },
     { PAGEFINDER, 0x05, 0x0DD9, 1, 0x04, 2 },
     { PAGEFINDER, 0x0B, 0x0DE9, 1, 0x04, 2 },
     { PAGEFINDER, 0x0C, 0x0DD9, 1, 0x04, 2 },
     { PAGEFINDER, 0x80, 0x0DE9, 1, 0x04, 2 },
     { ASPEN,	   0x09, 0x0DD9, 1, 0x04, 2 },
     { EXPANDAUR,  0x01, 0x0DD6, 1, 0x04, 2 },
     { LYNX,       0x01, 0x0DD9, 1, 0x04, 2 } };    
   FIELD( CP_power_on_ram_check )
   
RECORDS( CP_Memory_Opts_H ) 			
{ { PEGASUS,    0x01, 0x0DD6,  1, 0x00, 0 }, 
  { PEGASUS,    0x02, 0x0DE6,  1, 0x00, 0 },
  { PEGASUS,    0x04, 0x0DDA,  1, 0x00, 0 },
  { PAGEFINDER, 0x05, 0x0DDA,  1, 0x00, 0 },
  { PAGEFINDER, 0x0B, 0x0DEA,  1, 0x00, 0 },
  { PAGEFINDER, 0x0C, 0x0DDA,  1, 0x00, 0 },
  { PAGEFINDER, 0x80, 0x0DEA,  1, 0x00, 0 },
  { ASPEN, 		0x09, 0x0DDA,  1, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0DD7,  1, 0x00, 0 },
  { LYNX,       0x01, 0x0DDA,  1, 0x00, 0 } };      
FIELD( CP_Memory_Opts_H ) 			

   // sub-fields
   RECORDS( CP_duplication )
   { { PEGASUS,    0x01, 0x0DD6, 1, 0x80, 7 },
     { PEGASUS,    0x02, 0x0DE6, 1, 0x80, 7 },
     { PEGASUS,    0x04, 0x0DDA, 1, 0x80, 7 },
     { PAGEFINDER, 0x05, 0x0DDA, 1, 0x80, 7 },
     { PAGEFINDER, 0x0B, 0x0DEA, 1, 0x80, 7 },
     { PAGEFINDER, 0x0C, 0x0DDA, 1, 0x80, 7 },
     { PAGEFINDER, 0x80, 0x0DEA, 1, 0x80, 7 },
     { ASPEN, 	   0x09, 0x0DDA, 1, 0x80, 7 },
     { EXPANDAUR,  0x01, 0x0DD7, 1, 0x80, 7 },
     { LYNX,       0x01, 0x0DDA, 1, 0x80, 7 } };    
   FIELD( CP_duplication )

RECORDS( CP_Canned_Msgs ) 				
{ { PEGASUS,    0x01, 0x0DD8, 256, 0x00, 0 },
  { PEGASUS,    0x02, 0x0DE8, 256, 0x00, 0 },
  { PEGASUS,    0x04, 0x0DDC, 208, 0x00, 0 },
  { PAGEFINDER, 0x05, 0x0DDC, 208, 0x00, 0 },
  { PAGEFINDER, 0x0B, 0x0DEC, 192, 0x00, 0 },
  { PAGEFINDER, 0x0C, 0x0DDC, 208, 0x00, 0 },
  { PAGEFINDER, 0x80, 0x0DEC, 192, 0x00, 0 },
  { ASPEN,		0x09, 0x0DDC, 208, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0DD9, 208, 0x00, 0 },
  { LYNX,       0x01, 0x0DDC, 208, 0x00, 0 } };     
FIELD( CP_Canned_Msgs ) 

  RECORDS( CP_canned_msg )
  { { PEGASUS,    0x01, 0x0DD8, 16, 0x00, 0 },
    { PEGASUS,    0x02, 0x0DE8, 16, 0x00, 0 },
    { PEGASUS,    0x04, 0x0DDC, 16, 0x00, 0 },
    { PAGEFINDER, 0x05, 0x0DDC, 16, 0x00, 0 },
    { PAGEFINDER, 0x0B, 0x0DEC, 16, 0x00, 0 },
    { PAGEFINDER, 0x0C, 0x0DDC, 16, 0x00, 0 },
    { PAGEFINDER, 0x80, 0x0DEC, 16, 0x00, 0 },
    { ASPEN, 	  0x09, 0x0DDC, 16, 0x00, 0 },
    { EXPANDAUR,  0x01, 0x0DD9, 16, 0x00, 0 },
    { LYNX,       0x01, 0x0DDC, 16, 0x00, 0 } };    
  FIELD( CP_canned_msg )
  
RECORDS( CP_OutOfRange_Prompt ) 
{ { PEGASUS,    0x04, 0x0EAC, 20, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0EA9, 20, 0x00, 0 },
  { LYNX,       0x01, 0x0EAC, 20, 0x00, 0 } };   
FIELD( CP_OutOfRange_Prompt)

RECORDS( CP_OneWay_Prompt ) 
{ { PEGASUS,    0x04, 0x0EC0, 20, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0EBD, 20, 0x00, 0 },
  { LYNX,       0x01, 0x0EC0, 20, 0x00, 0 } };   
FIELD( CP_OneWay_Prompt)

RECORDS( CP_TwoWay_Prompt ) 
{ { PEGASUS,    0x04, 0x0ED4, 20, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0ED1, 20, 0x00, 0 },
  { LYNX,       0x01, 0x0ED4, 20, 0x00, 0 } };   
FIELD( CP_TwoWay_Prompt)

RECORDS( CP_Div_Fwd_Rev )
{ { PEGASUS,    0x01, 0x0EE8,   1, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0EE5,   1, 0x00, 0 },
  { LYNX,       0x01, 0x0EE8,   1, 0x00, 0 } };
FIELD( CP_Div_Fwd_Rev )
 				
                           
                         
                           
//  Hardware Control Block 
RECORDS( CP_StartSpNormalPatch )
{ { PEGASUS,    0x01, 0x0EEB,   2, 0x00, 0 },
  { PEGASUS,    0x02, 0x0EFB,   2, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0EF8,   2, 0x00, 0 },
  { LYNX,       0x01, 0x0EFB,   2, 0x00, 0 } };
FIELD( CP_StartSpNormalPatch );

RECORDS( CP_SizeSpNormalPatch )
{ { PEGASUS,    0x01, 0x0EED,   1, 0x00, 0 },
  { PEGASUS,    0x02, 0x0EFD,   1, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0EFA,   1, 0x00, 0 },
  { LYNX,       0x01, 0x0EFD,   1, 0x00, 0 } };
FIELD( CP_SizeSpNormalPatch );



//  Transceiver Control Block                           
//  PAGER#   CP_VER#  OFFSET  LEN ORMASK  SHIFT 
RECORDS( CP_TxWarmupTimes ) 			
{ { PEGASUS,    0x01, 0x0EFF,  8, 0x00, 0 }, 
  { PEGASUS,    0x02, 0x0F0F,  8, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0F0C,  8, 0x00, 0 },
  { LYNX,       0x01, 0x0F0F,  8, 0x00, 0 } };
FIELD( CP_TxWarmupTimes )

  // subfields
  RECORDS( CP_TxWarmupTime ) 			
  { { PEGASUS,    0x01, 0x0EFF, 1, 0x00, 0 }, 
    { PEGASUS,    0x02, 0x0F0F, 1, 0x00, 0 },
    { EXPANDAUR,  0x01, 0x0F0C, 1, 0x00, 0 },
    { LYNX,       0x01, 0x0F0F, 1, 0x00, 0 } };
  FIELD( CP_TxWarmupTime )
              
RECORDS( CP_SymbolTxLevel ) 			
{ { PEGASUS,    0x01, 0x0F0F,  4, 0x00, 0 }, 
  { PEGASUS,    0x02, 0x0F1F,  4, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0F1C,  4, 0x00, 0 },
  { LYNX,       0x01, 0x0F1F,  4, 0x00, 0 } };
FIELD( CP_SymbolTxLevel ) 			

  // subfields
  RECORDS( CP_Neg2400SymbolTxLevel ) 				
  { { PEGASUS,    0x01, 0x0F0F, 1, 0x00, 0 },
    { PEGASUS,    0x02, 0x0F1F, 1, 0x00, 0 },
    { EXPANDAUR,  0x01, 0x0F1C, 1, 0x00, 0 },
    { LYNX,       0x01, 0x0F1F, 1, 0x00, 0 } };
  FIELD( CP_Neg2400SymbolTxLevel ) 
  
  RECORDS( CP_Neg800SymbolTxLevel ) 				
  { { PEGASUS,    0x01, 0x0F0F+1, 1, 0x00, 0 },
    { PEGASUS,    0x02, 0x0F1F+1, 1, 0x00, 0 },
    { EXPANDAUR,  0x01, 0x0F1C+1, 1, 0x00, 0 },
    { LYNX,       0x01, 0x0F1F+1, 1, 0x00, 0 } };
  FIELD( CP_Neg800SymbolTxLevel ) 
  
  RECORDS( CP_Pos2400SymbolTxLevel ) 				
  { { PEGASUS,    0x01, 0x0F0F+2, 1, 0x00, 0 },
    { PEGASUS,    0x02, 0x0F1F+2, 1, 0x00, 0 },
    { EXPANDAUR,  0x01, 0x0F1C+2, 1, 0x00, 0 },
    { LYNX,       0x01, 0x0F1F+2, 1, 0x00, 0 } };
  FIELD( CP_Pos2400SymbolTxLevel ) 

  RECORDS( CP_Pos800SymbolTxLevel ) 				
  { { PEGASUS,    0x01, 0x0F0F+3, 1, 0x00, 0 },
    { PEGASUS,    0x02, 0x0F1F+3, 1, 0x00, 0 },
    { EXPANDAUR,  0x01, 0x0F1C+3, 1, 0x00, 0 },
    { LYNX,       0x01, 0x0F1F+3, 1, 0x00, 0 } };
  FIELD( CP_Pos800SymbolTxLevel ) 

RECORDS( CP_BalanceTune ) 				
{ { PEGASUS,    0x01, 0x0F13, 1, 0x00, 0 }, 
  { PEGASUS,    0x02, 0x0F23, 1, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0F20, 1, 0x00, 0 },
  { LYNX,       0x01, 0x0F23, 1, 0x00, 0 } };
FIELD( CP_BalanceTune ) 				

RECORDS( CP_AfcMake )					
{ { PEGASUS,    0x01, 0x0F14, 1, 0x00, 0 }, 
  { PEGASUS,    0x02, 0x0F24, 1, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0F21, 1, 0x00, 0 },
  { LYNX,       0x01, 0x0F24, 1, 0x00, 0 } };
FIELD( CP_AfcMake )					

RECORDS( CP_InOutOfRangeThreshold )		
{ { PEGASUS,    0x01, 0x0F16, 1, 0x00, 0 }, 
  { PEGASUS,    0x02, 0x0F26, 1, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0F23, 1, 0x00, 0 },
  { LYNX,       0x01, 0x0F26, 1, 0x00, 0 } };
FIELD( CP_InOutOfRangeThreshold )		

   // sub-fields
   RECORDS( CP_OutOfRangeSyncs )
   { { PEGASUS,    0x01, 0x0F16, 1, 0xF0, 4 },
     { PEGASUS,    0x02, 0x0F26, 1, 0xF0, 4 },
     { EXPANDAUR,  0x01, 0x0F23, 1, 0xF0, 4 },
     { LYNX,       0x01, 0x0F26, 1, 0xF0, 4 } };
   FIELD( CP_OutOfRangeSyncs )

   RECORDS( CP_InRangeSyncs )
   { { PEGASUS,    0x01, 0x0F16, 1, 0x0F, 0 },
     { PEGASUS,    0x02, 0x0F26, 1, 0x0F, 0 },
     { EXPANDAUR,  0x01, 0x0F23, 1, 0x0F, 0 },
     { LYNX,       0x01, 0x0F26, 1, 0x0F, 0 } };
   FIELD( CP_InRangeSyncs )

RECORDS( CP_AsyncSleepTime )
{ { PEGASUS,    0x01, 0x0F17, 1, 0x00, 0 },
  { PEGASUS,    0x02, 0x0F27, 1, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0F24, 1, 0x00, 0 },
  { LYNX,       0x01, 0x0F27, 1, 0x00, 0 } };
FIELD( CP_AsyncSleepTime )

RECORDS( CP_TxIdleSymbol ) 				
{ { PEGASUS,    0x01, 0x0F1B,  1, 0x00, 0 },
  { PEGASUS,    0x02, 0x0F2B,  1, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0F25,  1, 0x00, 0 },
  { LYNX,       0x01, 0x0F2B,  1, 0x00, 0 } };
FIELD( CP_TxIdleSymbol ) 

RECORDS( CP_Test_Freqs )
{ { PEGASUS,    0x01, 0x0F1C,  2, 0x00, 0 },
  { PEGASUS,    0x02, 0x0F2C,  2, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0F26,  2, 0x00, 0 },
  { LYNX,       0x01, 0x0F2C,  2, 0x00, 0 } };
FIELD( CP_Test_Freqs )

  RECORDS( CP_Rx1_Test_Freq )
  { { PEGASUS,    0x01, 0x0F1C,  2, 0x00, 0 },
    { PEGASUS,    0x02, 0x0F2C,  2, 0x00, 0 },
    { EXPANDAUR,  0x01, 0x0F26,  2, 0x00, 0 },
    { LYNX,       0x01, 0x0F2C,  2, 0x00, 0 } };
  FIELD( CP_Rx1_Test_Freq )

  RECORDS( CP_Tx1_Test_Freq )
  { { PEGASUS,    0x01, 0x0F1C+8,  2, 0x00, 0 },
    { PEGASUS,    0x02, 0x0F2C+8,  2, 0x00, 0 },
    { EXPANDAUR,  0x01, 0x0F26+8,  2, 0x00, 0 },
    { LYNX,       0x01, 0x0F2C+8,  2, 0x00, 0 } };
  FIELD( CP_Tx1_Test_Freq )
   				


//  DSP Miscellaneous Codeplug Block
//  PAGER#   CP_VER#  OFFSET  LEN ORMASK  SHIFT               
	
	

//  Option (User Read/Write) and Expansion Block        
//  PAGER#   CP_VER#  OFFSET  LEN ORMASK  SHIFT               
RECORDS( CP_Alert_Opts_1 ) 				
{ { PEGASUS,    0x01, 0x0F61,  1, 0x00, 0 },
  { PEGASUS,    0x02, 0x0F69,  1, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0F62,  1, 0x00, 0 },
  { LYNX,       0x01, 0x0F69,  1, 0x00, 0 } };
FIELD( CP_Alert_Opts_1 ) 				

  // sub-fields
  RECORDS( CP_memory_full )
  { { PEGASUS,    0x01, 0x0F61,  1, 0x80, 7 },
    { PEGASUS,    0x02, 0x0F69,  1, 0x80, 7 },
    { EXPANDAUR,  0x01, 0x0F62,  1, 0x80, 7 },
    { LYNX,       0x01, 0x0F69,  1, 0x80, 7 } };
  FIELD( CP_memory_full )
                                                   
  RECORDS( CP_vibrator_enabled )
  { { PEGASUS,    0x01, 0x0F61,  1, 0x40, 6 },
    { PEGASUS,    0x02, 0x0F69,  1, 0x40, 6 },
    { EXPANDAUR,  0x01, 0x0F62,  1, 0x40, 6 },
    { LYNX,       0x01, 0x0F69,  1, 0x40, 6 } };
  FIELD( CP_vibrator_enabled ) 
  
  RECORDS( CP_silent_update )
  { { PEGASUS,    0x01, 0x0F61,  1, 0x20, 5 },
    { PEGASUS,    0x02, 0x0F69,  1, 0x20, 5 },
    { PEGASUS,    0x03, 0x0F69,  1, 0x20, 5 },
    { EXPANDAUR,  0x01, 0x0F62,  1, 0x20, 5 },
    { LYNX,       0x01, 0x0F69,  1, 0x20, 5 } };
  FIELD( CP_silent_update )                    

  RECORDS( CP_low_cell_alert )
  { { PEGASUS,    0x01, 0x0F61,  1, 0x02, 1 },
    { PEGASUS,    0x02, 0x0F69,  1, 0x02, 1 },
    { EXPANDAUR,  0x01, 0x0F62,  1, 0x02, 1 },
    { LYNX,       0x01, 0x0F69,  1, 0x02, 1 } };
  FIELD( CP_low_cell_alert )

  RECORDS( CP_alert_duration )
  { { PEGASUS,    0x01, 0x0F61,  1, 0x01, 0 },
    { PEGASUS,    0x02, 0x0F69,  1, 0x01, 0 },
    { EXPANDAUR,  0x01, 0x0F62,  1, 0x01, 0 },
    { LYNX,       0x01, 0x0F69,  1, 0x01, 0 } };
  FIELD( CP_alert_duration )

RECORDS( CP_Reminder_Alert_Interval )	
{ { PEGASUS,    0x01, 0x0F64,  1, 0x00, 0 },
  { PEGASUS,    0x02, 0x0F6C,  1, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0F65,  1, 0x00, 0 },
  { LYNX,       0x01, 0x0F6C,  1, 0x00, 0 } };
FIELD( CP_Reminder_Alert_Interval )	

RECORDS( CP_Reminder_Timeout_Period )   
{ { PEGASUS,    0x01, 0x0F65,  1, 0x00, 0 },
  { PEGASUS,    0x02, 0x0F6D,  1, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0F66,  1, 0x00, 0 },
  { LYNX,       0x01, 0x0F6D,  1, 0x00, 0 } };
FIELD( CP_Reminder_Timeout_Period )   

RECORDS( CP_Read_Timeout ) 				
{ { PEGASUS,    0x01, 0x0F66,  1, 0x00, 0 },
  { PEGASUS,    0x02, 0x0F6E,  1, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0F67,  1, 0x00, 0 },
  { LYNX,       0x01, 0x0F6E,  1, 0x00, 0 } };
FIELD( CP_Read_Timeout ) 				

RECORDS( CP_Read_Scroll_Time )			
{ { PEGASUS,    0x01, 0x0F67,  1, 0x00, 0 },
  { PEGASUS,    0x02, 0x0F6F,  1, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0F68,  1, 0x00, 0 },
  { LYNX,       0x01, 0x0F6F,  1, 0x00, 0 } };
FIELD( CP_Read_Scroll_Time )			

RECORDS( CP_Display_L )					
{ { PEGASUS,    0x01, 0x0F6A,  1, 0x00, 0 },
  { PEGASUS,    0x02, 0x0F72,  1, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0F69,  1, 0x00, 0 },
  { LYNX,       0x01, 0x0F72,  1, 0x00, 0 } };
FIELD( CP_Display_L )					

   // sub-fields 
   RECORDS( CP_oor_icon )
   { { PEGASUS,    0x04, 0x0F72,  1, 0x80, 7 },
     { EXPANDAUR,  0x01, 0x0F69,  1, 0x80, 7 },
     { LYNX,       0x01, 0x0F72,  1, 0x80, 7 } };  
   FIELD( CP_oor_icon )
  
   RECORDS( CP_oor_prompt )
   { { PEGASUS,    0x04, 0x0F72,  1, 0x40, 6 },
     { EXPANDAUR,  0x01, 0x0F69,  1, 0x40, 6 },
     { LYNX,       0x01, 0x0F72,  1, 0x40, 6 } };  
   FIELD( CP_oor_prompt )

   RECORDS( CP_one_way_prompt )
   { { PEGASUS,    0x04, 0x0F72,  1, 0x20, 5 },
     { EXPANDAUR,  0x01, 0x0F69,  1, 0x20, 5 },
     { LYNX,       0x01, 0x0F72,  1, 0x20, 5 } };  
   FIELD( CP_one_way_prompt )

   RECORDS( CP_two_way_prompt )
   { { PEGASUS,    0x04, 0x0F72,  1, 0x10, 4 },
     { EXPANDAUR,  0x01, 0x0F69,  1, 0x10, 4 },
     { LYNX,       0x01, 0x0F72,  1, 0x10, 4 } };  
   FIELD( CP_two_way_prompt )
   
   RECORDS( CP_international_date )					
   { { PEGASUS,    0x01, 0x0F6A,  1, 0x10, 4 },
     { PEGASUS,    0x02, 0x0F72,  1, 0x10, 4 },
     { PEGASUS,    0x04, 0x0F72,  1, 0x08, 3 },
     { EXPANDAUR,  0x01, 0x0F69,  1, 0x08, 3 },
     { LYNX,       0x01, 0x0F72,  1, 0x08, 3 } };  
   FIELD( CP_international_date )
   
   RECORDS( CP_display_timestamp )
   { { PEGASUS,    0x01, 0x0F6A,  1, 0x01, 0 },
     { PEGASUS,    0x02, 0x0F72,  1, 0x01, 0 },
     { EXPANDAUR,  0x01, 0x0F69,  1, 0x01, 0 },
     { LYNX,       0x01, 0x0F72,  1, 0x01, 0 } };
   FIELD( CP_display_timestamp )
   
RECORDS( CP_MsgUpdateTimeout )
{ { PEGASUS,    0x01, 0x0F6D,  1, 0x00, 0 },      
  { PEGASUS,    0x02, 0x0F75,  1, 0x00, 0 },      
  { PEGASUS,    0x03, 0x0F76,  1, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0F6D,  1, 0x00, 0 },
  { LYNX,       0x01, 0x0F76,  1, 0x00, 0 } };
FIELD( CP_MsgUpdateTimeout )

RECORDS( CP_HostPatchLevel )
{ { PEGASUS,    0x01, 0x0F6F,  1, 0x00, 0 },
  { PEGASUS,    0x02, 0x0F77,  1, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0F6E,  1, 0x00, 0 },
  { LYNX,       0x01, 0x0F77,  1, 0x00, 0 } };
FIELD( CP_HostPatchLevel )

RECORDS( CP_SPPatchLevel )
{ { PEGASUS,    0x01, 0x0F70,  1, 0x00, 0 },
  { PEGASUS,    0x02, 0x0F78,  1, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0F6F,  1, 0x00, 0 },
  { LYNX,       0x01, 0x0F78,  1, 0x00, 0 } };
FIELD( CP_SPPatchLevel )

RECORDS( CP_Div_Iod )
{ { PEGASUS,    0x02, 0x0F79,  1, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0F70,  1, 0x00, 0 },
  { LYNX,       0x01, 0x0F79,  1, 0x00, 0 } };
FIELD( CP_Div_Iod ) 

RECORDS( CP_Reply_Attempt_Limit ) 
{ { PAGEFINDER, 0x0A, 0x0F7B, 1, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0F72, 1, 0x00, 0 },
  { LYNX,       0x01, 0x0F7B, 1, 0x00, 0 } };
FIELD( CP_Reply_Attempt_Limit )                  


// Patch Block
RECORDS( Cp_Codeplug_Patch )
{ { PEGASUS,    0x01, 0x0F7E, 127, 0x00, 0 },
  { PAGEFINDER, 0x05, 0x0F7E, 130, 0x00, 0 },
  { PAGEFINDER, 0x0B, 0x0F7D, 130, 0x00, 0 },
  { PAGEFINDER, 0x0C, 0x0F7E, 130, 0x00, 0 },
  { PAGEFINDER, 0x80, 0x0F7D, 130, 0x00, 0 },
  { ASPEN,      0x09, 0x0F7E, 130, 0x00, 0 },
  { EXPANDAUR,  0x01, 0x0F74, 140, 0x00, 0 },
  { LYNX,       0x01, 0x0F7E, 130, 0x00, 0 } };
FIELD( Cp_Codeplug_Patch ) 


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
// CpField class member function definitions


CpField::CpField(CpRecord *pRecs, int nNumRecs) 
{ 
  pRecords = pRecs; 
  nNumRecords = nNumRecs; 
}


int CpField::Offset(void)    
{ 
  return(pRecords[(Search())].nFieldOffset - (GetTheDocument()->GetDocBaseAddress())); 
}


int CpField::Length(void)    
{ 
  return(pRecords[(Search())].nFieldLength); 
}


BYTE CpField::byOrMask(void)  
{ 
  return((BYTE)(pRecords[(Search())].dwOrMask)); 
}


UINT CpField::nOrMask(void)   
{ 
  return((UINT)(pRecords[(Search())].dwOrMask));  
}
 

DWORD CpField::dwOrMask(void)
{
  return(pRecords[(Search())].dwOrMask);
}


BYTE CpField::byAndMask(void) 
{ 
  return((BYTE)(~(dwOrMask()))); 
}


UINT CpField::nAndMask(void)  
{ 
  return((UINT)(~(dwOrMask()))); 
}


DWORD CpField::dwAndMask(void)  
{ 
  return(~(dwOrMask())); 
}


int CpField::Shift(void)
{ 
  return((int)(pRecords[(Search())].byShiftFactor)); 
}


DWORD CpField::dwByteSwap(DWORD dwOriginal)
{
  DWORD dwTemp = 0L;
  
  dwTemp =  ((dwOriginal << 24) & 0xFF000000);
  dwTemp |= ((dwOriginal <<  8) & 0x00FF0000);
  dwTemp |= ((dwOriginal >>  8) & 0x0000FF00);
  dwTemp |= ((dwOriginal >> 24) & 0x000000FF);
  
  return(dwTemp);
}


UINT  CpField::nByteSwap(UINT nOriginal)
{
  UINT nTemp = 0x0000;
  
  nTemp = ((nOriginal << 8) & 0xFF00);
  nTemp |= ((nOriginal >> 8) & 0x00FF);
  
  return(nTemp);
}


void CpField::bSetFromDoc(BOOL *bDest, int nOffset) 
{ 
  ASSERT((Length() == sizeof(BYTE)) || 
         (Length() == sizeof(UINT)) ||
         (Length() == sizeof(DWORD)));
  int nIndex = Offset() + nOffset;
      
  if (Length() == sizeof(BYTE))
  {
    BYTE byTemp; 
    if (GetTheDocument()->Field(nIndex, 1, &byTemp))
      CpFieldError(nIndex, 1);
    *bDest = ((byTemp & (byOrMask())) > 0);
  }
  else if (Length() == sizeof(UINT))
  {
    UINT nTemp;
    if (GetTheDocument()->Field(nIndex, Length(), (BYTE *)&nTemp))
      CpFieldError(nIndex, Length());
    nTemp = nByteSwap(nTemp);
    *bDest = ((nTemp & (nOrMask())) > 0);
  }
  else if (Length() == sizeof(DWORD))
  {
    DWORD dwTemp;
    if (GetTheDocument()->Field(nIndex, Length(), (BYTE *)&dwTemp))
      CpFieldError(nIndex, Length());   
    dwTemp = dwByteSwap(dwTemp);
    *bDest = ((dwTemp & (dwOrMask())) > 0);
  } 
  TRACE("\t\tCpField::bSetFromDoc : %d\n", *bDest);
}


void CpField::bySetFromDoc(BYTE *byDest, int nOffset)
{
  ASSERT(Length() == sizeof(BYTE));
  int nIndex = Offset() + nOffset;

  if (GetTheDocument()->Field(nIndex, 1, byDest))
    CpFieldError(nIndex, 1);  
  if (byOrMask())
    *byDest &= (byOrMask());
  if (Shift())
    *byDest = (BYTE)(*byDest >> (Shift()));
  TRACE("\t\tCpField::bySetFromDoc : %02X\n", *byDest);
}


void CpField::nSetFromDoc(UINT *nDest, int nOffset)
{
  ASSERT(Length() == sizeof(UINT));
  int nIndex = Offset() + nOffset;

  if (GetTheDocument()->Field(nIndex, Length(), (BYTE*)nDest))
    CpFieldError(nIndex, Length());  
  *nDest = nByteSwap(*nDest);
  if (nOrMask())
    *nDest &= (nOrMask());
  if (Shift())
    *nDest = (*nDest >> (Shift()));
  TRACE("\t\tCpField::nSetFromDoc : %04X\n", *nDest);
}
  
 
void CpField::dwSetFromDoc(DWORD *dwDest, int nOffset)
{
  ASSERT(Length() == sizeof(DWORD));
  int nIndex = Offset() + nOffset;

  if (GetTheDocument()->Field(nIndex, Length(), (BYTE*)dwDest))
    CpFieldError(nIndex, Length());  
  *dwDest = dwByteSwap(*dwDest);
  if (dwOrMask())
    *dwDest &= (dwOrMask());
  if (Shift())
    *dwDest = (*dwDest >> (Shift()));
  TRACE("\t\tCpField::dwSetFromDoc : %08lX\n", *dwDest);
}
  

void CpField::szSetFromDoc(char *szDest, int nNumChars, int nOffset)
{
  ASSERT(Length() <= nNumChars);
  int nIndex = Offset();
  nIndex += nOffset;

  if (GetTheDocument()->Field(nIndex, nNumChars, (BYTE*)szDest))
    CpFieldError(nIndex, nNumChars);  
  szDest[nNumChars] = '\0';
  TRACE("\t\tCpField::szSetFromDoc : %s\n", szDest);
}
      
      
//////
void CpField::bSetToDoc(BOOL bSource, int nOffset)
{ 
  TRACE("\nCpField::bSetToDoc : %d\n", (int)(bSource));
  ASSERT((Length() == sizeof(BYTE)) || 
         (Length() == sizeof(UINT)) ||
         (Length() == sizeof(DWORD))); 
  BOOL bOrig;
  int nIndex = Offset() + nOffset;
  
  // check need to save modification
  bSetFromDoc(&bOrig, nOffset);  
  if (bOrig == bSource)  
    return;
    
  if (Length() == sizeof(BYTE))
  {
    BYTE byTemp; 
    if (GetTheDocument()->Field(nIndex, Length(), &byTemp))
      CpFieldError(nIndex, Length());
    if (bSource == TRUE)
      byTemp |= (byOrMask());
    else
      byTemp &= (byAndMask());
    if (GetTheDocument()->SetField(nIndex, Length(), &byTemp))
      CpFieldError(nIndex, Length());    
  }
  else if (Length() == sizeof(UINT))
  {
    UINT nTemp;
    if (GetTheDocument()->Field(nIndex, Length(), (BYTE *)&nTemp))
      CpFieldError(nIndex, Length());    
    nTemp = nByteSwap(nTemp);
    if (bSource == TRUE)
      nTemp |= (nOrMask());
    else
      nTemp &= (nAndMask());
    nTemp = nByteSwap(nTemp);
    if (GetTheDocument()->SetField(nIndex, Length(), (BYTE *)&nTemp))
      CpFieldError(nIndex, Length());    
  }
  else if (Length() == sizeof(DWORD))
  {
    DWORD dwTemp;
    if (GetTheDocument()->Field(nIndex, Length(), (BYTE *)&dwTemp))
      CpFieldError(nIndex, Length());
    dwTemp = dwByteSwap(dwTemp);
    if (bSource == TRUE)
      dwTemp |= (dwOrMask());
    else
      dwTemp &= (dwAndMask());
    dwTemp = dwByteSwap(dwTemp);
    if (GetTheDocument()->SetField(nIndex, Length(), (BYTE *)&dwTemp))
      CpFieldError(nIndex, Length());
  }      
  return;
}


void CpField::bySetToDoc(BYTE bySource, int nOffset)
{
  TRACE("\nCpField::bySetToDoc : %02X\n", bySource);
  ASSERT(Length() == sizeof(BYTE));
  BYTE byTemp, byOrig;
  int nIndex = Offset() + nOffset;
  
  if (GetTheDocument()->Field(nIndex, Length(), &byTemp))
    CpFieldError(nIndex, Length());   
  byOrig = byTemp;
  
  if (byOrMask())  // is field a bit-field
  {
    byTemp &= (byAndMask());
    byTemp |= (bySource << (Shift()));
  }
  else
    byTemp = bySource;
  
  // check need to save modification
  if (byOrig == byTemp)
    return;
  if (GetTheDocument()->SetField(nIndex, Length(), &byTemp))
    CpFieldError(nIndex, Length());  
  return;  
}


void CpField::nSetToDoc(UINT nSource, int nOffset)
{
  TRACE("\nCpField::nSetToDoc : %04X\n", nSource);
  ASSERT(Length() == sizeof(UINT));
  UINT nTemp, nOrig;
  int nIndex = Offset() + nOffset;
  
  // get original field from the document
  if (GetTheDocument()->Field(nIndex, Length(), (BYTE*)&nTemp))
    CpFieldError(nIndex, Length());  
  nOrig = nTemp;

  if (nOrMask())	// is field a bit-field
  {
    nTemp = nByteSwap(nTemp);
    nTemp &= (nAndMask());
    nTemp |= (nSource << (Shift()));
    nTemp = nByteSwap(nTemp);
  }
  else
  {
    nTemp = nByteSwap(nSource);
  }
       
  // check need to save modification
  if (nTemp == nOrig)
    return;
  if (GetTheDocument()->SetField(nIndex, Length(), (BYTE*)&nTemp))
    CpFieldError(nIndex, Length());    
  return;
}
    


void CpField::dwSetToDoc(DWORD dwSource, int nOffset)
{
  TRACE("\nCpField::dwSetToDoc : %08lX\n", dwSource);
  ASSERT(Length() == sizeof(DWORD));
  DWORD dwTemp, dwOrig;
  int nIndex = Offset() + nOffset;
  
  // get the original field from the document
  if (GetTheDocument()->Field(nIndex, Length(), (BYTE*)&dwTemp))
    CpFieldError(nIndex, Length());  
  dwOrig = dwTemp;
  
  if (dwOrMask())	// is field a bit-field
  { 
    dwTemp = dwByteSwap(dwTemp);
    dwTemp &= (dwAndMask());
    dwTemp |= (dwSource << (Shift()));
    dwTemp = dwByteSwap(dwTemp);
  }
  else
  {
    dwTemp = dwByteSwap(dwSource);
  }
  
  // check for need to save modification
  if (dwTemp == dwOrig)
    return;
  if (GetTheDocument()->SetField(nIndex, Length(), (BYTE*)&dwTemp))
    CpFieldError(nIndex, Length());  
  return;
}


void CpField::szSetToDoc(const char *szSource, int nNumChars, int nOffset)
{
  TRACE("\nCpField::szSetToDoc : %s\n", szSource);
  ASSERT(nNumChars <= Length());
  ASSERT(Length() >= lstrlen(szSource));
  
  int nIndex = Offset() + nOffset;
  ASSERT(Length() < 200);  // is there really a 200 byte 
  char szTemp[200];        // string in the codeplug?
   
  szSetFromDoc(szTemp, Length(), nOffset);
  if (lstrcmp(szTemp, szSource) == 0)  // check need to modify
    return; 
  
  // ensure the entire field is over-written
  lstrcpy(szTemp, szSource);
  int lcv = lstrlen(szSource);
  for ( ; lcv < Length(); ++lcv)
    szTemp[lcv] = '\0';
  
  if (GetTheDocument()->SetField(nIndex, Length(), (BYTE*)szTemp))
    CpFieldError(nIndex, Length());
  return;
}


/////////////////////////////////////////////////////////////////////////
// Search()
//
// This CpField member function performs a linear search backwards thru
// the data record array for a CpField.  The search keys are the current
// device ID and the current codeplug version.  An entry is considered 
// found if the device ID and/or the version indicator is less than
// or equal to the current device_id and codeplug version.  This saves from having to
// update each field in the data-base when new devices are supported and/or
// new codeplug versions are released.  This assumes that device id's
// and codeplug versions are issued in increasing order.   
//                                    
int CpField::Search(void)           
{                        		    
  CPpsDoc *pDoc = GetTheDocument();
  ASSERT(pDoc);
  
  for (int lcv = (nNumRecords - 1); lcv >= 0; lcv--)
  { 
    ASSERT(pRecords != NULL); 
    if (pRecords[lcv].byPagerId < (pDoc->GetDeviceId()))
      return(lcv);
    if ((pRecords[lcv].byPagerId <= (pDoc->GetDeviceId())) &&
        (pRecords[lcv].byCodeplugVersion <= (pDoc->GetCpVersion())))
      return(lcv);
  } 
  TRACE("\n**** CpField::Search : FIELD NOT FOUND ****\n");
  return(nNumRecords-1);   // default to the latest entry if not found
} 



CPpsDoc *CpField::GetTheDocument(void)
{
  #ifdef _DEBUG
    CPpsDoc *pDoc = (CPpsDoc*)((CFrameWnd *)theApp.m_pMainWnd)->GetActiveDocument();
    //ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(CPpsDoc)));
    return(pDoc);
  #else
    return((CPpsDoc*)((CFrameWnd *)theApp.m_pMainWnd)->GetActiveDocument());
  #endif
}


static char *szAccessError = "Configuration document access error.\n"
                             "Attempt to access document using an \n"
                             "invalid index or field length value.\n"
                             "Press F1 for additional details.\n"
                             "\nOffending pair: index=%d, length=%d";
void CpField::CpFieldError(int nIndex, int nNumbytes)
{  
  static BOOL bErrMsgShowing = FALSE;

  if (bErrMsgShowing)  // display only 1 error msg at a time
    return;

  if (!bErrMsgShowing)
  { 
    char szTemp[250];
    sprintf(szTemp, szAccessError, nIndex, nNumbytes); 
    bErrMsgShowing = TRUE;
    DWORD dwHelpID = HID_BASE_RESOURCE + IDP_CPFIELDACCESS_ERROR;   
    int nMsgRtn = AfxMessageBox(szTemp, MB_OK, (UINT)dwHelpID);
    if (nMsgRtn == MB_OK)
      bErrMsgShowing = FALSE;
  }
  return;
}  



/////////// ******* CODEPLUG.CPP

