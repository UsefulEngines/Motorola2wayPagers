/*
 *        Function Prototypes and Useful Defined Constants for the           
 *         Microsoft C and Borland C++ Language Interface for the                      
 *                  GPIB DLL (Dynamic Link Library)                          
 *                   for Microsoft Windows 3.0/3.1
 *
 */
 
#ifdef __cplusplus
extern "C" {
#endif

extern  int             ibsta;
extern  int             iberr;
extern  unsigned short  ibcnt; 
extern  long            ibcntl;


#define UNL  0x3f  /* GPIB unlisten command                */
#define UNT  0x5f  /* GPIB untalk command                  */
#define GTL  0x01  /* GPIB go to local                     */
#define SDC  0x04  /* GPIB selected device clear           */
#define PPC  0x05  /* GPIB parallel poll configure         */
#define GET  0x08  /* GPIB group execute trigger           */
#define TCT  0x09  /* GPIB take control                    */
#define LLO  0x11  /* GPIB local lock out                  */
#define DCL  0x14  /* GPIB device clear                    */
#define PPU  0x15  /* GPIB parallel poll unconfigure       */
#define SPE  0x18  /* GPIB serial poll enable              */
#define SPD  0x19  /* GPIB serial poll disable             */
#define PPE  0x60  /* GPIB parallel poll enable            */
#define PPD  0x70  /* GPIB parallel poll disable           */

/* GPIB status bit vector :                                */
/*       global variable ibsta and wait mask               */

#define ERR     (1<<15) /* Error detected                  */
#define TIMO    (1<<14) /* Timeout                         */
#define END     (1<<13) /* EOI or EOS detected             */
#define SRQI    (1<<12) /* SRQ detected by CIC             */
#define RQS     (1<<11) /* Device needs service            */
#define SPOLL   (1<<10) /* Board has been serially polled  */
#define EVENT   (1<<9)  /* An event has occured            */
#define CMPL    (1<<8)  /* I/O completed                   */
#define LOK     (1<<7)  /* Local lockout state             */
#define REM     (1<<6)  /* Remote state                    */
#define CIC     (1<<5)  /* Controller-in-Charge            */
#define ATN     (1<<4)  /* Attention asserted              */
#define TACS    (1<<3)  /* Talker active                   */
#define LACS    (1<<2)  /* Listener active                 */
#define DTAS    (1<<1)  /* Device trigger state            */
#define DCAS    (1<<0)  /* Device clear state              */

/* Error messages returned in global variable iberr        */

#define EDVR  0  /* System error                           */
#define ECIC  1  /* Function requires GPIB board to be CIC */
#define ENOL  2  /* Write function detected no Listeners   */
#define EADR  3  /* Interface board not addressed correctly*/
#define EARG  4  /* Invalid argument to function call      */
#define ESAC  5  /* Function requires GPIB board to be SAC */
#define EABO  6  /* I/O operation aborted                  */
#define ENEB  7  /* Non-existent interface board           */
#define EDMA  8  /* Error performing DMA                   */
#define EOIP 10  /* I/O operation started before previous  */
                 /* operation completed                    */
#define ECAP 11  /* No capability for intended operation   */
#define EFSO 12  /* File system operation error            */
#define EBUS 14  /* Command error during device call       */
#define ESTB 15  /* Serial poll status byte lost           */
#define ESRQ 16  /* SRQ remains asserted                   */
#define ETAB 20  /* The return buffer is full.             */
#define ELCK 21  /* Address or board is locked.            */
#define ECFG 24  /* Configuration warning, not error:
                       only returned by board-level ibfind -
                       a WARNING that the board's current 
                       configuration differs from the newly
                       requested configuration.            */

/* EOS mode bits                                           */

#define BIN  (1<<12) /* Eight bit compare                  */
#define XEOS (1<<11) /* Send END with EOS byte		        */
#define REOS (1<<10) /* Terminate read on EOS              */

/* Timeout values and meanings                             */

#define TNONE    0   /* Infinite timeout (disabled)        */
#define T10us    1   /* Timeout of 10 us (ideal)           */
#define T30us    2   /* Timeout of 30 us (ideal)           */
#define T100us   3   /* Timeout of 100 us (ideal)          */
#define T300us   4   /* Timeout of 300 us (ideal)          */
#define T1ms     5   /* Timeout of 1 ms (ideal)            */
#define T3ms     6   /* Timeout of 3 ms (ideal)            */
#define T10ms    7   /* Timeout of 10 ms (ideal)           */
#define T30ms    8   /* Timeout of 30 ms (ideal)           */
#define T100ms   9   /* Timeout of 100 ms (ideal)          */
#define T300ms  10   /* Timeout of 300 ms (ideal)          */
#define T1s     11   /* Timeout of 1 s (ideal)             */
#define T3s     12   /* Timeout of 3 s (ideal)             */
#define T10s    13   /* Timeout of 10 s (ideal)            */
#define T30s    14   /* Timeout of 30 s (ideal)            */
#define T100s   15   /* Timeout of 100 s (ideal)           */
#define T300s   16   /* Timeout of 300 s (ideal)           */
#define T1000s  17   /* Timeout of 1000 s (ideal)          */


/*  IBLN Constants                                         */
#define NO_SAD   0
#define ALL_SAD -1

/*  IBEVENT Constants                                      */
#define  EventDTAS   1
#define  EventDCAS   2
#define  EventIFC    3

/*
 *    GotoMultAddr (VXI) flags
 */
#define  MultAddrPrimary      0x00
#define  MultAddrSecondary    0x01

#define  MultAddrListen       0x00
#define  MultAddrTalk         0x01
#define  MultAddrSerialPoll   0x81


/*  The following constants are used for the second parameter of the 
 *  ibconfig function.  They are the "option" selection codes.
 */
#define  IbcPAD            0x0001   /* Primary Address                                           */
#define  IbcSAD            0x0002   /* Secondary Address                                         */
#define  IbcTMO            0x0003   /* Timeout Value                                             */
#define  IbcEOT            0x0004   /* Send EOI with last data byte?                             */
#define  IbcPPC            0x0005   /* Parallel Poll Configure                                   */
#define  IbcREADDR         0x0006   /* Repeat Addressing                                         */
#define  IbcAUTOPOLL       0x0007   /* Disable Auto Serial Polling                               */
#define  IbcCICPROT        0x0008   /* Use the CIC Protocol?                                     */
#define  IbcIRQ            0x0009   /* Use PIO for I/O                                           */
#define  IbcSC             0x000A   /* Board is System Controller?                               */
#define  IbcSRE            0x000B   /* Assert SRE on device calls?                               */
#define  IbcEOSrd          0x000C   /* Terminate reads on EOS                                    */
#define  IbcEOSwrt         0x000D   /* Send EOI with EOS character                               */
#define  IbcEOScmp         0x000E   /* Use 7 or 8-bit EOS compare                                */
#define  IbcEOSchar        0x000F   /* The EOS character.                                        */
#define  IbcPP2            0x0010   /* Use Parallel Poll Mode 2.                                 */
#define  IbcTIMING         0x0011   /* NORMAL, HIGH, or VERY_HIGH timing.                        */
#define  IbcDMA            0x0012   /* Use DMA for I/O                                           */
#define  IbcReadAdjust     0x0013   /* Swap bytes during an ibrd.                                */
#define  IbcWriteAdjust    0x0014   /* Swap bytes during an ibwrt.                               */
#define  IbcEventQueue     0x0015   /* Enable/disable the event queue.                           */
#define  IbcSPollBit       0x0016   /* Enable/disable the visibility of SPOLL.                   */
#define  IbcSendLLO        0x0017   /* Enable/disable the sending of LLO.                        */
#define  IbcSPollTime      0x0018   /* Set the timeout value for serial polls.                   */
#define  IbcPPollTime      0x0019   /* Set the parallel poll length period.                      */
#define  IbcEndBitIsNormal 0x001A   /* Remove EOS from END bit of IBSTA.                         */
#define  IbcUnAddr         0x001B   /* Enable/disable device unaddressing.                       */
#define  IbcSignalNumber   0x001C   /* Set UNIX signal number - unsupported                      */
#define  IbcBlockIfLocked  0x001D   /* Enable/disable blocking for locked boards/devices         */
#define  IbcHSCableLength  0x001F   /* Length of cable specified for high speed timing.          */
#define  IbcIst            0x0020   /* Set the IST bit.                                          */
#define  IbcRsv            0x0021   /* Set the RSV byte.                                         */
#define  IbcLON            0x0022   /* Enter listen only mode.                                   */

/*
 *    Constants that can be used (in addition to the ibconfig constants)
 *    when calling the ibask() function.
 */

#define  IbaPAD            IbcPAD            
#define  IbaSAD            IbcSAD            
#define  IbaTMO            IbcTMO            
#define  IbaEOT            IbcEOT            
#define  IbaPPC            IbcPPC            
#define  IbaREADDR         IbcREADDR         
#define  IbaAUTOPOLL       IbcAUTOPOLL       
#define  IbaCICPROT        IbcCICPROT        
#define  IbaIRQ            IbcIRQ            
#define  IbaSC             IbcSC             
#define  IbaSRE            IbcSRE            
#define  IbaEOSrd          IbcEOSrd          
#define  IbaEOSwrt         IbcEOSwrt         
#define  IbaEOScmp         IbcEOScmp         
#define  IbaEOSchar        IbcEOSchar        
#define  IbaPP2            IbcPP2            
#define  IbaTIMING         IbcTIMING         
#define  IbaDMA            IbcDMA            
#define  IbaReadAdjust     IbcReadAdjust     
#define  IbaWriteAdjust    IbcWriteAdjust    
#define  IbaEventQueue     IbcEventQueue     
#define  IbaSPollBit       IbcSPollBit       
#define  IbaSendLLO        IbcSendLLO        
#define  IbaSPollTime      IbcSPollTime      
#define  IbaPPollTime      IbcPPollTime      
#define  IbaEndBitIsNormal IbcEndBitIsNormal
#define  IbaUnAddr         IbcUnAddr         
#define  IbaSignalNumber   IbcSignalNumber
#define  IbaBlockIfLocked  IbcBlockIfLocked
#define  IbaHSCableLength  IbcHSCableLength
#define  IbaLON            IbcLON
#define  IbaIst            IbcIst
#define  IbaRsv            IbcRsv   

#define  IbaBNA                  0x0200   /* A device's access board.                  */
#define  IbaBaseAddr             0x0201   /* A GPIB board's base I/O address.          */
#define  IbaDmaChannel           0x0202   /* A GPIB board's DMA channel.               */
#define  IbaIrqLevel             0x0203   /* A GPIB board's IRQ level.                 */
#define  IbaBaud                 0x0204   /* Baud rate used to communicate to CT box.  */
#define  IbaParity               0x0205   /* Parity setting for CT box.                */
#define  IbaStopBits             0x0206   /* Stop bits used for communicating to CT.   */
#define  IbaDataBits             0x0207   /* Data bits used for communicating to CT.   */
#define  IbaComPort              0x0208   /* System COM port used for CT box.          */
#define  IbaComIrqLevel          0x0209   /* System COM port's interrupt level.        */
#define  IbaComPortBase          0x020A   /* System COM port's base I/O address.       */
#define  IbaDmaType              0x020B   /* Type of DMA transfer mode used.           */
#define  IbaSingleCycleDma       0x020B   /* Does the board use single cycle DMA?      */
#define  IbaSlotNumber           0x020C   /* Slot number, if appropriate.              */
#define  IbaSocketNumber         0x020C   /* PCMCIA only - socket number.              */
#define  IbaLPTNumber            0x020D   /* Parallel port number                      */
#define  IbaLPTType              0x020E   /* Parallel port protocol                    */


/* Values used by the Send 488.2 command. */
#define  NULLend 0x00  /* Do nothing at the end of a transfer.*/
#define  NLend   0x01  /* Send NL with EOI after a transfer.  */
#define  DABend  0x02  /* Send EOI with the last DAB.         */

/* Value used by the 488.2 Receive command.
 */
#define  STOPend     0x0100


/* Address type (for 488.2 calls) */

typedef short Addr4882_t;	/* System dependent: must be 16 bits */

/*
 *  This macro can be used to easily create an entry in address list
 *  that is required by many of the 488.2 functions. The primary address goes in the
 *  lower 8-bits and the secondary address goes in the upper 8-bits.
 */
#define  MakeAddr(pad, sad)   ((Addr4882_t)(((pad)&0xFF) | ((sad)<<8)))

/*
 *  This value is used to terminate an address list.  It should be
 *  assigned to the last entry.
 */
#define  NOADDR    (Addr4882_t)0xFFFF

/*
 *  The following two macros are used to "break apart" an address list
 *  entry.  They take an unsigned integer and return either the primary
 *  or secondary address stored in the integer.
 */
#define  GetPAD(val)    ((val) & 0xFF)
#define  GetSAD(val)    (((val) >> 8) & 0xFF)

/* iblines constants */

#define  ValidEOI   (short)0x0080
#define  ValidATN   (short)0x0040
#define  ValidSRQ   (short)0x0020
#define  ValidREN   (short)0x0010
#define  ValidIFC   (short)0x0008
#define  ValidNRFD  (short)0x0004
#define  ValidNDAC  (short)0x0002
#define  ValidDAV   (short)0x0001
#define  BusEOI     (short)0x8000
#define  BusATN     (short)0x4000
#define  BusSRQ     (short)0x2000
#define  BusREN     (short)0x1000
#define  BusIFC     (short)0x0800
#define  BusNRFD    (short)0x0400
#define  BusNDAC    (short)0x0200
#define  BusDAV     (short)0x0100

/* Function prototypes */

int _far _pascal ibask(int handle, int option, int _far *retval);
int _far _pascal ibbna(int handle, char _far *bdname);
int _far _pascal ibcac(int handle, int v);
int _far _pascal ibclr(int handle);
int _far _pascal ibcmd(int handle, void _far *buffer, long cnt);
int _far _pascal ibcmda(int handle, void _far *buffer, long cnt);
int _far _pascal ibconfig(int handle, int option, int value);
int _far _pascal ibdev(int boardID, int pad, int sad, int tmo, int eot, int eos);
int _far _pascal ibdma(int handle, int v);
int _far _pascal ibeos(int handle, int v);
int _far _pascal ibeot(int handle, int v);
int _far _pascal ibevent(int handle, short _far *event);
int _far _pascal ibfind(char _far *bdname);
int _far _pascal ibgts(int handle, int v);
int _far _pascal ibist(int handle, int v);
int _far _pascal iblines(int handle, short _far *lines);
int _far _pascal ibln(int handle, int padval, int sadval, short _far *listenflag);
int _far _pascal ibloc(int handle);
int _far _pascal ibonl(int handle, int v);
int _far _pascal ibpad(int handle, int v);
int _far _pascal ibpct(int handle);
int _far _pascal ibppc(int handle, int v);
int _far _pascal ibrd(int handle, void _far *buffer, long cnt);
int _far _pascal ibrda(int handle, void _far *buffer, long cnt);
int _far _pascal ibrdf(int handle, char _far *flname);
int _far _pascal ibrpp(int handle, char _far *ppr);
int _far _pascal ibrsc(int handle, int v);
int _far _pascal ibrsp(int handle, char _far *spr);
int _far _pascal ibrsv(int handle, int v);
int _far _pascal ibsad(int handle, int v);
int _far _pascal ibsic(int handle);
int _far _pascal ibsre(int handle, int v);
int _far _pascal ibstop(int handle);
int _far _pascal ibtmo(int handle, int v);
int _far _pascal ibtrg(int handle);
int _far _pascal ibwait(int handle, int mask);
int _far _pascal ibwrt(int handle, void _far *buffer, long cnt);
int _far _pascal ibwrta(int handle, void _far *buffer, long cnt);
int _far _pascal ibwrtf(int handle, char _far *flname);

int _far _pascal ibpoke(int handle, int option, int value);
int _far _pascal ibdiag(int handle, void _far *buffer, long cnt);
int _far _pascal ibxtrc(int handle, void _far *buffer, long cnt);

int _far _pascal ibwrtkey(int handle, void _far *buffer, int  cnt);
int _far _pascal ibrdkey(int handle, void _far *buffer, int cnt);

/*
 *  iblock and ibunlock are currently only supported by the GPIB-ENET.
 */
int _far _pascal iblock (int handle);
int _far _pascal ibunlock (int handle);


void _far _pascal AllSpoll(int boardID, Addr4882_t _far *addrlist, short _far *resultlist);
void _far _pascal DevClear(int boardID, Addr4882_t address);
void _far _pascal DevClearList(int boardID, Addr4882_t _far *addrlist);
void _far _pascal EnableLocal(int boardID, Addr4882_t _far *addrlist);
void _far _pascal EnableRemote(int boardID, Addr4882_t _far *addrlist);
void _far _pascal FindLstn(int boardID, Addr4882_t far *padlist, Addr4882_t  far *resultlist, int limit);
void _far _pascal FindRQS(int boardID, Addr4882_t _far *addrlist, short _far *result);
void _far _pascal PPoll(int boardID, short _far *result);
void _far _pascal PPollConfig(int boardID, Addr4882_t address, int dataLine, int lineSense);
void _far _pascal PPollUnconfig(int boardID, Addr4882_t _far *addrlist);
void _far _pascal PassControl(int boardID, Addr4882_t address);
void _far _pascal RcvRespMsg(int boardID, void _far *buffer, long cnt, int termination);
void _far _pascal ReadStatusByte(int boardID, Addr4882_t address, short _far *result);
void _far _pascal Receive(int boardID, Addr4882_t address, void _far *buffer, long cnt, int termination);
void _far _pascal ReceiveSetup(int boardID, Addr4882_t address);
void _far _pascal ResetSys(int boardID, Addr4882_t _far *addrlist);
void _far _pascal Send(int boardID, Addr4882_t address, void _far *buffer, long datacnt, int eotmode);
void _far _pascal SendCmds(int boardID, void _far *buffer, long cnt);
void _far _pascal SendDataBytes(int boardID, void _far *buffer, long cnt, int eotmode);
void _far _pascal SendIFC(int boardID);
void _far _pascal SendLLO(int boardID);
void _far _pascal SendList(int boardID, Addr4882_t _far *addrlist, void _far *buffer, long datacnt, int eotmode);
void _far _pascal SendSetup(int boardID, Addr4882_t _far *addrlist);
void _far _pascal SetRWLS(int boardID, Addr4882_t _far *addrlist);
void _far _pascal TestSRQ(int boardID, short _far *result);
void _far _pascal TestSys(int boardID, Addr4882_t _far *addrlist, short _far *resultlist);
void _far _pascal Trigger(int boardID, Addr4882_t address);
void _far _pascal TriggerList(int boardID, Addr4882_t _far *addrlist);
void _far _pascal WaitSRQ(int boardID, short _far *result);

void _far _pascal GotoMultAddr(int boardID,
                              unsigned short type,
                              unsigned short (_far *addrfunc)(),
                              unsigned short (_far *spollfunc)());
void _far _pascal GenerateREQT(int boardID, unsigned short addr);
void _far _pascal GenerateREQF(int boardID, unsigned short addr);


#ifdef __cplusplus
}
#endif


