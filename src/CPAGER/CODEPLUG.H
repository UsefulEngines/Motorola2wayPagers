//*************************************************************
//  File name: CODEPLUG.H
//
//  Description: 
//		Codeplug mapping description file 
//
//
//  Comments:
//		Based upon Pegasus Codeplug Description Document
//		AMSD-TX72_SRD_9510014, Rev 3.0, 15 May 1996 and later
//		
// Written by Motorola AMG, PMU Engineering.
// Legacy code for reference only.
//*************************************************************

#ifndef _CODEPLUG_
 #define _CODEPLUG_

// *** SUPPORTED DEVICE ID's - see also cpager derived .h files
// 
#define PEGASUS			0x50
#define CENTAUR 		0x80
#define PEGASUS_ACK     0x81
#define PAGEFINDER      0x82
#define ASPEN   		0x83
//#define VAIL			0x84  // not supported herein 
#define EXPANDAUR       0x85
#define LYNX			0x86
 
// data base record structure
typedef struct 
{ 
  BYTE		byPagerId;
  BYTE 		byCodeplugVersion;
  int 		nFieldOffset;
  int 		nFieldLength;
  DWORD		dwOrMask;
  BYTE		byShiftFactor;       
} CpRecord;



class CpField
{
  private:
    int			nNumRecords;
    CpRecord	*pRecords;
  public:
    CpField(CpRecord *pRecs, int nNumRecs);
    // attributes  
    int  	Offset(void);    
    int  	Length(void);    
    BYTE 	byOrMask(void);  
    UINT 	nOrMask(void);
    DWORD   dwOrMask(void);
    BYTE 	byAndMask(void); 
    UINT 	nAndMask(void);
    DWORD	dwAndMask(void);  
    int  	Shift(void);
    // operations    
    static DWORD dwByteSwap(DWORD ulOriginal);
    static UINT  nByteSwap(UINT nOriginal); 
    // from document operations
    void 	bSetFromDoc(BOOL *bDest, int nOffset=0);
    void 	bySetFromDoc(BYTE *byDest, int nOffset=0);
    void 	nSetFromDoc(UINT *nDest, int nOffset=0);
    void 	dwSetFromDoc(DWORD *dwDest, int nOffset=0);
    void 	szSetFromDoc(char *szDest, int nNumChars, int nOffset=0); 
    // to document operations
    void 	bSetToDoc(BOOL bSource, int nOffset=0);
    void 	bySetToDoc(BYTE bySource, int nOffset=0);
    void 	nSetToDoc(UINT nSource, int nOffset=0);
    void 	dwSetToDoc(DWORD dwSource, int nOffset=0);
    void 	szSetToDoc(const char *szSource, int nNumChars, int nOffset=0);    
  private:
    int  	Search(void);
    CPpsDoc *GetTheDocument(void);
    void	CpFieldError(int nIndex, int nNumbytes);  
};  // CpField




    

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
// *** Codeplug CpField Names - defined in file codeplug.cpp

// *** NOTE: sub-fields are indented and sub-field names are no-initial caps.
// *** You may have to set the Linker option which specifies use of case-sensitive 
// *** name resolution.

	//	Identification and Control Block : Sub-Fields are indented 
	extern CpField  CP_Serial_Number;
	extern CpField  CP_Version;
	extern CpField  CP_OTA_Protected_H;   // pager id
	extern CpField  CP_OTA_Protected_L;
     extern CpField  CP_rf_programming_disabled;
	extern CpField  CP_Model_Opts; 
	 extern CpField  CP_send_msg_complete;
	 extern CpField  CP_IOD_msg_capable;
	 extern CpField  CP_multi_response_capable;
	extern CpField  CP_OTA_Subheader;
     extern CpField  CP_OTA_revision;
	extern CpField  CP_OTA_Password;
	extern CpField  CP_DL_Password;
	extern CpField  CP_Release_Password;
	extern CpField  CP_System_Type_1_L;
     extern CpField  CP_reflex50_enable;
     extern CpField  CP_dl_security;	
	extern CpField  CP_AP_Powerup;
	extern CpField  CP_Personal_Addrs;
	 extern CpField  CP_Personal_addrs;    
	extern CpField  CP_InfoSvc_Addrs;
	 extern CpField  CP_infosvc_addrs_disable; 
	 extern CpField  CP_infosvc_addrs;
	extern CpField  CP_Active_SubAddrs;
	 extern CpField  CP_active_subaddrs;

	//  Protocol Control Block - Common to Reflex50 and Reflex25
    extern CpField  CP_Control_Channel; 
     extern CpField  CP_CtrlChanDisable;
     extern CpField  CP_CtrlChanScanDelay;
     extern CpField  CP_CtrlChanNumber;
    
    //  Protocol-Specific Control Block - Reflex25
    extern CpField  CP_Service_Provider_Ids;
     extern CpField  CP_ServiceProviderDisable;
     extern CpField  CP_ServiceProviderAncillary;
     extern CpField  CP_ServiceProviderId; 
     extern CpField  CP_SpidZoneIndicator;  // new format
     extern CpField  CP_SpidAncPrimIndicator;
     extern CpField  CP_SpidNumber;
     extern CpField  CP_SpidZoneNumber;
    extern CpField  CP_Home_Index;
    extern CpField  CP_RegFailRestTime; 
    
    //  Protocol-Specific Control Block - Reflex50
	extern CpField  CP_Electronic_Serial_Number;  
    extern CpField  CP_InfoFrame;
    extern CpField  CP_PagerFrame;
    extern CpField  CP_HomeLogicalNum;
    extern CpField  CP_PagerCollapse;
     extern CpField  CP_PersonalCollapse;
     extern CpField  CP_InfoServiceCollapse;
    extern CpField  CP_RevChannelMap;
     extern CpField  CP_RevChannelDisable;
     extern CpField  CP_RevChannelNum;
    extern CpField  CP_R50_Misc;
     extern CpField  CP_channel_id;
     extern CpField  CP_allow_auto_reg;
     extern CpField  CP_notify_tx_change;
    extern CpField  CP_IncommunicadoTmr;
    extern CpField  CP_CommunicadoTmr;
    extern CpField  CP_AlohaTimeout;
    extern CpField  CP_Retry_Cnt; 
    extern CpField  CP_AlohaRndInterval;
    extern CpField  CP_AggReg_Attack;
    extern CpField  CP_AggReg_Ceiling;
    extern CpField  CP_AggReg_RetryTmr;
    extern CpField  CP_AggReg_RetryCnt;

	//  Host Control Block
	extern CpField  CP_Inventory_Control_Number;
	extern CpField  CP_Programmable_Pleasing_Alert;
     extern CpField  CP_volume;
     extern CpField  CP_FrequencyIndex; 
	extern CpField  CP_Alert_Frequency;
	extern CpField  CP_Format_Delimiter;
	extern CpField  CP_Delimiter_Location;
	 extern CpField  CP_delimiter_location;
	extern CpField  CP_Function1_L; 
     extern CpField  CP_oor_indicator;
     extern CpField  CP_mult_choice_enable;
     extern CpField  CP_IOD_msg_enable;
     extern CpField  CP_send_msg_enable;
     extern CpField  CP_non_deaf_pager;
     extern CpField  CP_enable_all_alerts;
     extern CpField  CP_msg_read_by_user;
	extern CpField  CP_Memory_Opts_L; 
     extern CpField  CP_periodic_ram_check;
     extern CpField  CP_retain_errored_alpha;
     extern CpField  CP_retain_errored_numeric;
     extern CpField  CP_retain_errored_transparent;
     extern CpField  CP_retain_errored_2way;
     extern CpField  CP_power_on_ram_check;
	extern CpField  CP_Memory_Opts_H; 
     extern CpField  CP_duplication;
	extern CpField  CP_Canned_Msgs;
	 extern CpField  CP_canned_msg;
	extern CpField  CP_OutOfRange_Prompt;
	extern CpField  CP_OneWay_Prompt;
	extern CpField  CP_TwoWay_Prompt;
	extern CpField  CP_Div_Fwd_Rev;
    
    //  Hardware Control Block
    extern CpField  CP_StartSpNormalPatch;
    extern CpField  CP_SizeSpNormalPatch;
    
	//  Transceiver Control Block 
	extern CpField	CP_TxWarmupTimes;
	 extern CpField	 CP_TxWarmupTime;
	extern CpField  CP_SymbolTxLevel;
	 extern CpField  CP_Neg2400SymbolTxLevel;
	 extern CpField  CP_Neg800SymbolTxLevel;
	 extern CpField  CP_Pos2400SymbolTxLevel;
	 extern CpField  CP_Pos800SymbolTxLevel;
	extern CpField  CP_BalanceTune;
	extern CpField  CP_AfcMake;
	extern CpField  CP_InOutOfRangeThreshold;
     extern CpField  CP_OutOfRangeSyncs;
     extern CpField  CP_InRangeSyncs;
	extern CpField  CP_AsyncSleepTime;
	extern CpField  CP_TxIdleSymbol;
	extern CpField  CP_Test_Freqs;
	 extern CpField  CP_Rx1_Test_Freq; 
	 extern CpField  CP_Tx1_Test_Freq; 
	
	//  DSP Miscellaneous Codeplug Block
	extern CpField  CP_AckMsgPctg;
	
	//  Option (User Read/Write; and Expansion Block 
	extern CpField  CP_Alert_Opts_1; 
     extern CpField  CP_memory_full;
     extern CpField  CP_vibrator_enabled;
     extern CpField  CP_silent_update;
     extern CpField  CP_flap_alert;
     extern CpField  CP_low_cell_alert;
     extern CpField  CP_alert_duration;
	extern CpField  CP_Reminder_Alert_Interval;
	extern CpField  CP_Reminder_Timeout_Period;
	extern CpField  CP_Read_Timeout;
	extern CpField  CP_Read_Scroll_Time;
	extern CpField  CP_DataFlag;
	extern CpField  CP_Display_L;
	 extern CpField  CP_oor_icon;
	 extern CpField  CP_oor_prompt;
	 extern CpField  CP_one_way_prompt;
	 extern CpField  CP_two_way_prompt;
     extern CpField  CP_international_date;
     extern CpField  CP_display_timestamp;
    extern CpField	CP_MsgUpdateTimeout;
	extern CpField  CP_HostPatchLevel;
	extern CpField  CP_SPPatchLevel;
	extern CpField  CP_Div_Iod;
	extern CpField  CP_Reply_Attempt_Limit;
	
	// Patch Block
	extern CpField  Cp_Codeplug_Patch; 
	
    
/////////////////////////////////////////////////////////////////////////


 
#endif // _CODEPLUG_


