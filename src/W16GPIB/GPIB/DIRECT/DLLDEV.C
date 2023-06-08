/* ===========================================================================
 *  C Example Program : NI-488 Direct-Entry Points
 *
 *  This sample program is for reference only. It can only be expected to
 *  function with a Fluke 45 Digital Multimeter.
 *
 *  This program reads 10 measurements from the Fluke 45 and averages
 *  the sum.
 * ===========================================================================
 */

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "..\\windecl.h"

#ifdef __cplusplus
extern "C" {
#endif

int ibsta, iberr;                      /* NI-488.2 global status variables */
long ibcntl;

int _far _pascal DLLibclr (int ud, int _far *ibsta, int _far *iberr,
			  long _far *ibcntl);
int _far _pascal DLLibdev (int ud, int pad, int sad, int tmo, int eot,
			  int eos, int _far *ibsta, int _far *iberr,
			  long _far *ibcntl);
int _far _pascal DLLibonl (int ud, int v, int _far *ibsta, int _far *iberr,
			  long _far *ibcntl);
int _far _pascal DLLibrd  (int ud, char _far *rdbuf, long cnt,
			  int _far *ibsta, int _far *iberr, long _far *ibcntl);
int _far _pascal DLLibrsp (int ud, char _far *spr, int _far *ibsta,
			  int _far *iberr, long _far *ibcntl);
int _far _pascal DLLibtrg (int ud, int _far *ibsta, int _far *iberr,
			  long _far *ibcntl);
int _far _pascal DLLibwait (int ud, int mask, int _far *ibsta,int _far *iberr,
			  long _far *ibcntl);
int _far _pascal DLLibwrt (int ud, char _far *result,long cnt,int _far *ibsta,
			  int _far *iberr, long _far *ibcntl);

static HINSTANCE GpibLib = NULL;

#ifdef __cplusplus
}
#endif

void gpiberr(char *msg);               /* Error function                  */
void dvmerr(char *msg, char spr);      /* Fluke 45 error function         */

char     rd[512],                      /* read data buffer                */
	 spr;                          /* serial poll response byte       */
int      dvm,                          /* device number                   */
	 m;                            /* FOR loop counter                */
double   sum;                          /* Accumulator of measurements     */

/*      CODE TO ACCESS GPIB.DLL */

static int(_far _pascal *Pibclr)(int ud, int _far *ibsta, int _far *iberr,
				 long _far *ibcntl);
static int(_far _pascal *Pibdev)(int BdIndx, int pad, int sad, int tmo, 
				 int eot, int eos, int _far *ibsta, 
				 int _far *iberr, long _far *ibcntl);
static int(_far _pascal *Pibonl)(int ud, int v, int _far *ibsta, 
				 int _far *iberr, long _far *ibcntl);
static int(_far _pascal *Pibrd)(int ud, void _far *rdbuf, long cnt,
				int _far *ibsta, int _far *iberr,
				long _far *ibcntl);
static int(_far _pascal *Pibrsp)(int ud, char _far *spr, int _far *ibsta, 
				 int _far *iberr, long _far *ibcntl);
static int(_far _pascal *Pibtrg)(int ud, int _far *ibsta, int _far *iberr,
				 long _far *ibcntl);
static int(_far _pascal *Pibwait)(int ud, int mask, int _far *ibsta, 
				  int _far *iberr, long _far *ibcntl);
static int(_far _pascal *Pibwrt)(int ud, void _far *wrtbuf, long cnt,
				 int _far *ibsta, int _far *iberr, 
				 long _far *ibcntl);


static BOOL LoadDll (void){

	/* Call LoadLibrary to load the GPIB DLL. Save the handle into
		the global GpibLib */

	GpibLib=LoadLibrary("GPIB.DLL");
	if (!GpibLib) {
		/* The LoadLibrary call failed, return with an error. */
		return FALSE;
	}
	/* The GPIB library is now loaded. We will now get a pointer to
		each function. If the GetProcAddress call fails, we will
		return with an error. */

	Pibclr=(int (_far _pascal *)(int,int _far *,int _far *,long _far *))
		GetProcAddress(GpibLib,(LPCSTR)"DLLibclr");
	Pibdev=(int (_far _pascal *)(int, int, int, int, int, int, int _far*, 
		      int _far *, long _far *))GetProcAddress(GpibLib,
		      (LPCSTR)"DLLibdev");
	Pibonl=(int (_far _pascal *)(int, int, int _far *, int _far *, 
		      long _far *))GetProcAddress(GpibLib,(LPCSTR)"DLLibonl");
	Pibrd=(int (_far _pascal *)(int, void _far *, long, int _far *, 
		     int _far *, long _far *))GetProcAddress(GpibLib,
		     (LPCSTR)"DLLibrd");
	Pibrsp=(int (_far _pascal *)(int,char _far *, int _far *, int _far *, 
		      long _far *))GetProcAddress(GpibLib,(LPCSTR)"DLLibrsp");
	Pibtrg=(int (_far _pascal *)(int,int _far *,int _far *,long _far *))
		      GetProcAddress(GpibLib,(LPCSTR)"DLLibtrg");
	Pibwait=(int (_far _pascal *)(int, int, int _far *, int _far *, 
		       long _far *))GetProcAddress(GpibLib,
		       (LPCSTR)"DLLibwait");
	Pibwrt=(int (_far _pascal *)(int, void _far *, long, int _far *, 
		      int _far *, long _far *))GetProcAddress(GpibLib,
		      (LPCSTR)"DLLibwrt");


	if ((Pibclr==NULL)||
		 (Pibdev==NULL)||
		 (Pibonl==NULL)||
		 (Pibrd==NULL)||
		 (Pibrsp==NULL)||
		 (Pibtrg==NULL)||
		 (Pibwait==NULL)||
		 (Pibwrt==NULL)) {
		FreeLibrary(GpibLib);
		GpibLib=NULL;
		return FALSE;
	}
	else {
		return TRUE;
	}
} /*end of LoadDll*/

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
	printf("Read 10 measurements from the Fluke 45...\n");
	printf("\n");

	/* Assign a unique identifier to the Fluke 45 and store in the variable
		DVM.  IBDEV opens an available device and assigns it to access GPIB0
		with a primary address of 1, a secondary address of 0, a timeout of
		10 seconds, the END message enabled, and the EOS mode disabled.
      If DVM is less than zero, call GPIBERR with an error message. */

	dvm = (*Pibdev) (0, 1, 0, T10s, 1, 0, &ibsta, &iberr, &ibcntl);
	if (dvm < 0) {
		gpiberr("ibdev Error");
	}


	/* Clear the internal or device functions of the Fluke 45. */

   (*Pibclr) (dvm, &ibsta, &iberr, &ibcntl);
   if (ibsta & ERR) {
		gpiberr("ibclr Error");
	}

	/* Reset the Fluke 45 by issuing the reset (*RST) command.  Instruct the
      Fluke 45 to measure the volts alternating current (VAC) using auto-ranging
      (AUTO), to wait for a trigger from the GPIB interface board (TRIGGER 2),
      and to assert the IEEE-488 Service Request line, SRQ, when the measurement
      has been completed and the Fluke 45 is ready to send the result (*SRE 16). */

	(*Pibwrt) (dvm, "*RST; VAC; AUTO; TRIGGER 2; *SRE 16", 35L, &ibsta, &iberr, &ibcntl);
	if (ibsta & ERR) {
		gpiberr("ibwrt Error");
	}

	/* Initialize the accumulator of the 10 measurements to zero. */

	sum = 0.0;

	/* Establish FOR loop to read the 10 measuements.  The variable m will
	   serve as the counter of the FOR loop. */

	for (m=0; m < 10 ; m++) {
   
		/*  Trigger the Fluke 45. */

	(*Pibtrg) (dvm, &ibsta, &iberr, &ibcntl);
      if (ibsta & ERR) {
			gpiberr("ibtrg Error");
		}

     /*  Request the triggered measurement by sending the instruction
	 "VAL1?". */

		(*Pibwrt) (dvm, "VAL1?", 5L, &ibsta, &iberr, &ibcntl);
		if (ibsta & ERR) {
			gpiberr("ibwrt Error");
		}

      /* Wait for the Fluke 45 to request service (RQS) or wait for the
	  Fluke 45 to timeout(TIMO).  The default timeout period is 10 seconds.
	RQS is detected by bit position 11 (hex 800).   TIMO is detected
	by bit position 14 (hex 4000).  These status bits are listed under
	the NI-488 function IBWAIT in the Software Reference Manual. */

		(*Pibwait) (dvm, TIMO|RQS, &ibsta, &iberr, &ibcntl);
      if (ibsta & (ERR|TIMO)) {
			gpiberr("ibwait Error");
		}

		/* Read the Fluke 45 serial poll status byte. */

		(*Pibrsp) (dvm, &spr, &ibsta, &iberr, &ibcntl);
      if (ibsta & ERR) {
			gpiberr("ibrsp Error");
		}

		/* If the returned status byte is hex 50, the Fluke 45 has valid data
	to send; otherwise, it has a fault condition to report. */

		if (spr != 0x50) {
			dvmerr("Fluke 45 Error", spr);
		}

      /* Read the Fluke 45 measurement. */

      (*Pibrd) (dvm, rd, 10L, &ibsta, &iberr, &ibcntl);
		if (ibsta & ERR) {
			gpiberr("ibrd Error");
		}

     /* Use the null character to mark the end of the data received
	in the array RD.  Print the measurement received from the
	Fluke 45. */

		rd[(short)ibcntl] = '\0';
      printf("Reading :  %s\n", rd);

		/* Convert the variable RD to its numeric value and add to the
	 accumulator. */

		sum = sum + atof(rd);

	}  /*  Continue FOR loop until 10 measurements are read.   */

	/* Print the average of the 10 readings. */

   printf("   The average of the 10 readings is : %f\n", sum/10);

	/* Call the ibonl function to disable the hardware and software. */

   (*Pibonl) (dvm, 0, &ibsta, &iberr, &ibcntl);
	
	FreeDll();
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


void dvmerr(char *msg,char spr) {

	printf ("%s\n", msg);
   printf("Status byte = %x\n", spr);

	/* Disable hardware and software */
   (*Pibonl) (0, 0, &ibsta, &iberr, &ibcntl);
	
	FreeDll();
	
	/* Abort program */
   exit(1);
}



