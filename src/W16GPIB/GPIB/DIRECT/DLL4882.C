/* ===========================================================================
 * C Example Program:   NI-488.2 Direct-Entry Points
 *
 *  This sample program is for reference only. It can only be expected to
 *  function with a Fluke 45 Digital Multimeter.
 *
 *  The following program determines if a Fluke 45 multimeter is a listener
 *  on the GPIB.  If the Fluke 45 is a listener, 10 measurements are read
 *  and the average of the sum of the measurements is calculated.
 *
 * ===========================================================================
 */

#include "..\\windecl.h"

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifdef __cplusplus
extern "C" {
#endif

int ibsta, iberr;                      /* NI-488.2 global status variables */
long ibcntl;

static HINSTANCE GpibLib=NULL;

static int (_far _pascal *PSendIFC)(int boardID, int _far *ibsta, 
				    int _far *iberr, long _far *ibcntl);
static int (_far _pascal *PFindLstn) (int boardID, Addr4882_t _far padlist[],
				      Addr4882_t _far resultlist[],int limit,
				      int _far *ibsta, int _far *iberr, 
				      long _far *ibcntl);
static int (_far _pascal *PSend) (int boardID, Addr4882_t address, 
				  void _far *buffer, long datacnt, 
				  int eotmode, int _far *ibsta, 
				  int _far *iberr, long _far *ibcntl);
static int (_far _pascal *PReceive) (int boardID, Addr4882_t address, 
				     void _far *buffer, long cnt, 
				     int termination, int _far *ibsta, 
				     int _far *iberr, long _far *ibcntl);
static int (_far _pascal *Pibonl) (int ud, int v, int _far *ibsta, 
				   int _far *iberr, long _far *ibcntl);
static int (_far _pascal *PDevClear) (int boardID, Addr4882_t address, 
				      int _far *ibsta, int _far *iberr,
				      long _far *ibcntl);
static int (_far _pascal *PWaitSRQ) (int boardID, short _far *result, 
				     int _far *ibsta, int _far *iberr,
				     long _far *ibcntl);
static int (_far _pascal *PReadStatusByte) (int boardID, Addr4882_t address, 
					    short _far *result, 
					    int _far *ibsta,int _far *iberr, 
					    long _far *ibcntl);

#ifdef __cplusplus
}
#endif

void found (unsigned int fluke);       /* Obtain readings from Fluke 45     */
void gpiberr(char *msg);               /* Error routine                     */

#define  MAVbit   0x10           /* Position of the Message Available bit.  */

char          buffer[101];       /* Data received from the Fluke 45         */
int           loop,              /* FOR loop counter and array index        */
	      m,                 /* FOR loop counter                        */
	      num_listeners,     /* Number of listeners on GPIB             */
	      pad;               /* Primary address of listener on GPIB     */
double        sum;               /* Accumulator of measurements             */
Addr4882_t    fluke,             /* Primary address of the Fluke 45         */
	      SRQasserted,       /* Set to indicate if SRQ is asserted      */
	      statusByte,        /* Serial Poll Response Byte               */
	      instruments[32],   /* Array of primary addresses              */
	      result[31];        /* Array of listen addresses               */


static BOOL LoadDll(void) {

	/* Call LoadLibrary to load the GPIB DLL. Save the handle into
		the global GpibLib */

	GpibLib=LoadLibrary("GPIB.DLL");
	if (!GpibLib) {
		/* The LoadLibrary call failed, return with an error.*/
		return FALSE;
	}
	/* The GPIB library is now loaded. We will now get a pointer to
		each function. If the GetProcAddress call fails, we will
		return with an error. */

	PSendIFC=(int (_far _pascal *)(int, int _far *, int _far *, 
		  long _far *))GetProcAddress(GpibLib,(LPCSTR)"DLLSendIFC");
	PFindLstn=(int (_far _pascal *)(int, Addr4882_t _far *, 
			 Addr4882_t _far *, int, int _far *, int _far *, 
			 long _far *))GetProcAddress(GpibLib,
			 (LPCSTR)"DLLFindLstn"); 
	PSend=(int (_far _pascal *)(int, Addr4882_t, void _far *, long, int, 
		     int _far *, int _far *, long _far *))
		     GetProcAddress(GpibLib,(LPCSTR)"DLLSend");
	PReceive=(int (_far _pascal *)(int, Addr4882_t, void _far *, long, 
			int, int _far *, int _far *, long _far *))
			GetProcAddress(GpibLib,(LPCSTR)"DLLReceive");
	Pibonl=(int (_far _pascal *)(int, int, int _far *, int _far *, 
		      long _far *))GetProcAddress(GpibLib,
		      (LPCSTR)"DLLibonl");
	PDevClear=(int (_far _pascal *)(int, Addr4882_t, int _far *, 
		   int _far *, long _far *))GetProcAddress(GpibLib,
		   (LPCSTR)"DLLDevClear");
	PWaitSRQ=(int (_far _pascal *)(int, short _far *, int _far *, 
			int _far *, long _far *))GetProcAddress(GpibLib,
			(LPCSTR)"DLLWaitSRQ");
	PReadStatusByte=(int (_far _pascal *)(int, Addr4882_t, short _far *, 
			 int _far *, int _far *, long _far *))
			 GetProcAddress(GpibLib,(LPCSTR)"DLLReadStatusByte");

	if ((PSendIFC==NULL)||
		 (PFindLstn==NULL)||
		 (PSend==NULL)||
		 (PReceive ==NULL)||
		 (Pibonl==NULL)||
		 (PDevClear==NULL)||
		 (PWaitSRQ==NULL)||
		 (PReadStatusByte==NULL)) {
		FreeLibrary(GpibLib);
		GpibLib=NULL;
		return FALSE;
	}
	else {
		return TRUE;
	}

}

static void FreeDll(void) {

	FreeLibrary(GpibLib);
	GpibLib=NULL;
	return;
}

void main() {

	if (!LoadDll()){
		printf("Unable to correctly access the GPIB DLL\n");
		return;
	}
	/* Your board needs to be the Controller-In-Charge in order to find all
		listeners on the GPIB.  To accomplish this, the function SendIFC is
		called. */

	(*PSendIFC)(0, &ibsta, &iberr, &ibcntl);
	if (ibsta & ERR) {
		gpiberr ("SendIFC Error");
	}

	/* Create an array containing all valid GPIB primary addresses.  Your GPIB
		interface board is at address 0 by default.  This  array (INSTRUMENTS)
		will be given to the function FindLstn to find all listeners. */

	for (loop = 0; loop <= 30; loop++) {
		instruments[loop] = loop;
	}
   instruments[31] = NOADDR;

	/* Print message to tell user that the program is searching for all active
		listeners.  Find all of the listeners on the bus.   Store the listen
		addresses in the array RESULT. */

	printf("Finding all listeners on the bus...\n");
	printf("\n");

	(*PFindLstn)(0, &instruments[1], result, 31, &ibsta, &iberr, &ibcntl);
	if (ibsta & ERR) {
		gpiberr("FindLstn Error");
	}

	/* Assign the value of IBCNT to the variable NUM_LISTENERS.
		Print the number of listeners found. */

	num_listeners = (short)ibcntl;

	printf("Number of instruments found = %d\n", num_listeners);

	/* Send the *IDN? command to each device that was found.
      Establish a FOR loop to determine if the Fluke 45 is a listener on the
      GPIB.  The variable LOOP will serve as a counter for the FOR loop and
	   as the index to the array RESULT. */

	for (loop = 0; loop <= num_listeners; loop++) {

	/*      Send the identification query to each listen address in the
      array RESULT.  The constant NLend instructs the function Send to
      append a linefeed character with EOI asserted to the end of the
      message. */

		(*PSend)(0, result[loop], "*IDN?", 5L, NLend, &ibsta, &iberr, &ibcntl);
      if (ibsta & ERR) {
			gpiberr("Send Error");
		}

		/* Read the name identification response returned from each device.
	 Store the response in the array BUFFER.  The constant STOPend
	 instructs the function Receive to terminate the read when END
	 is detected. */

		(*PReceive)(0, result[loop], buffer, 10L, STOPend, &ibsta, &iberr, &ibcntl);
      if (ibsta & ERR) {
			gpiberr("Receive Error");
		}

		/* The low byte of the listen address is the primary address.
	 Assign the variable PAD the primary address of the device.
	 The macro GetPAD returns the low byte of the listen address. */

		pad = GetPAD(result[loop]);

		/* Use the null character to mark the end of the data received
			in the array BUFFER.  Print the primary address and the name
			identification of the device. */

		buffer[(short)ibcntl] = '\0';
		printf("The instrument at address %d is a %s\n", pad, buffer);

		/* Determine if the name identification is the Fluke 45.  If it is
	 the Fluke 45, assign PAD to FLUKE,  print message that the
	 Fluke 45 has been found, call the function FOUND, and terminate
	 FOR loop. */

		if (strncmp(buffer, "FLUKE, 45", 9) == 0) {
			fluke = pad;
			printf("**** We found the Fluke ****\n");
			found(fluke);
			break;
		}

	}      /*  End of FOR loop */

	if (loop > num_listeners) {
		printf("Did not find the Fluke!\n");
	}

	/* Call the ibonl function to disable the hardware and software. */

    (*Pibonl) (0, 0, &ibsta, &iberr, &ibcntl);

	 FreeDll();

}


/* ===========================================================================
 *                      Function FOUND
 *  This function is called if the Fluke 45 has been identified as a listener
 *  in the array RESULT.  The variable FLUKE is the primary address of the
 *  Fluke 45.  Ten measurements are read from the fluke and the average of
 *  the sum is calculated.
 * ===========================================================================
 */

void found(unsigned int fluke) {

	/* Reset the Fluke 45 using the functions DevClear and Send.
		DevClear will send the GPIB Selected Device Clear (SDC) command message
		to the Fluke 45. */

	(*PDevClear)(0, fluke, &ibsta, &iberr, &ibcntl);
	if (ibsta & ERR) {
		gpiberr("DevClear Error");
	}

	/* Use the function Send to send the IEEE-488.2 reset command (*RST)
		to the Fluke 45.  The constant NLend, defined in DECL.H, instructs
		the function Send to append a linefeed character with EOI asserted
		to the end of the message. */

	(*PSend)(0, fluke, "*RST", 4L, NLend, &ibsta, &iberr, &ibcntl);
	if (ibsta & ERR){
		gpiberr("Send *RST Error");
	}

	/* Use the function Send to send device configuration commands to the
		Fluke 45.  Instruct the Fluke 45 to measure volts alternating current
		(VAC) using auto-ranging (AUTO), to wait for a trigger from the GPIB
		interface board (TRIGGER 2), and to assert the IEEE-488 Service Request
		line, SRQ, when the measurement has been completed and the Fluke 45 is
		ready to send the result (*SRE 16). */

	(*PSend)(0, fluke, "VAC; AUTO; TRIGGER 2; *SRE 16", 29L, NLend, &ibsta, &iberr, &ibcntl);
	if (ibsta & ERR) {
		gpiberr("Send Setup Error");
	}

	/*  Initialized the accumulator of the 10 measurements to zero.             */

	sum = 0.0;

	/* Establish FOR loop to read the 10 measurements.  The variable m will
		serve as the counter of the FOR loop. */

	for (m=0; m < 10 ; m++) {

		/*      Trigger the Fluke 45 by sending the trigger command (*TRG) and
	 request a measurement by sending the command "VAL1?". */

		(*PSend)(0, fluke, "*TRG; VAL1?", 11L, NLend, &ibsta, &iberr, &ibcntl);
      if (ibsta & ERR){
			gpiberr("Send Trigger Error");
		}

		/* Wait for the Fluke 45 to assert SRQ, meaning it is ready to send
	 a measurement.  If SRQ is not asserted within the timeout period,
	 call GPIBERR with an error message.  The timeout period by default
	 is 10 seconds. */

			(*PWaitSRQ)(0, &SRQasserted, &ibsta, &iberr, &ibcntl);
	 if (!SRQasserted) {
		printf("SRQ is not asserted.  The Fluke is not ready.\n");
	    exit(1);
	 }

		/* Read the serial poll status byte of the Fluke 45. */

		(*PReadStatusByte)(0, fluke, &statusByte, &ibsta, &iberr, &ibcntl);
		if (ibsta & ERR){
			gpiberr("ReadStatusByte Error");
		}

      /* Check if the Message Available Bit (bit 4) of the return status
	 byte is set.  If this bit is not set, print the status byte. */

		if (!(statusByte & MAVbit)) {
			printf(" Status Byte = 0x%x\n", statusByte);
	 gpiberr("Improper Status Byte");
      }

		/* Read the Fluke 45 measurement.  Store the measurement in the
			variable BUFFER.  The constant STOPend, defined in DECL.H,
	 instructs the function Receive to terminate the read when END
	 is detected. */

		(*PReceive)(0, fluke, buffer, 10L, STOPend, &ibsta, &iberr, &ibcntl);
      if (ibsta & ERR) {
			gpiberr("Receive Error");
		}

      /* Use the null character to mark the end of the data received
	 in the array BUFFER.  Print the measurement received from the
	 Fluke 45. */

		buffer[(short)ibcntl] = '\0';
      printf("Reading :  %s\n", buffer);

      /*  Convert the variable BUFFER to its numeric value and add to the
	  accumulator. */

		sum = sum + atof(buffer);

		}  /*  Continue FOR loop until 10 measurements are read.   */

	/*  Print the average of the 10 readings. */

	printf("   The average of the 10 readings is : %f\n", sum/10);

}


void gpiberr(char *msg) {

	printf ("%s\n", msg);
   printf ("iberr = %d \n", iberr);

	/* Disable hardware and software */
   (*Pibonl) (0, 0, &ibsta, &iberr, &ibcntl);
	FreeDll();

	/* Abort program */
   exit(1);                               
}
