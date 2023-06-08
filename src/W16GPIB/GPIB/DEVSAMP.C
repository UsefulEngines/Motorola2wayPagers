/*
 * ===========================================================================
 * C Example Program -- NI-488 device level functions                      
 *
 *  This sample program is for reference only. It can only be expected to
 *  function with a Fluke 45 Digital Multimeter.
 *
 *  This program reads 10 measurements from the Fluke 45 and averages
 *  the sum.
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*  WINDECL.H contains constants, declarations, and function prototypes.    */

#include "windecl.h"

void gpiberr(char *msg);                 /* Error routine                   */
void dvmerr(char *msg, char spr);        /* Fluke 45 error routine          */ 

  char     rd[512],                      /* read data buffer                */
           spr;                          /* serial poll response byte       */
  int      dvm,                          /* device number                   */
           m;                            /* FOR loop counter                */
  double   sum;                          /* Accumulator of measurements     */


void main() {

    printf("Read 10 measurements from the Fluke 45...\n");
    printf("\n");

/*
 *  Assign a unique identifier to the Fluke 45 and store in the variable
 *  DVM.  IBDEV opens an available device and assigns it to access GPIB0
 *  with a primary address of 1, a secondary address of 0, a timeout of
 *  10 seconds, the END message enabled, and the EOS mode disabled.
 *  If DVM is less than zero, call GPIBERR with an error message.
 */

    dvm = ibdev (0, 1, 0, T10s, 1, 0);
    if (dvm < 0) gpiberr("ibdev Error");

/*  Clear the internal or device functions of the Fluke 45.             */

    ibclr (dvm);
    if (ibsta & ERR) gpiberr("ibclr Error");

/*
 *  Reset the Fluke 45 by issuing the reset (*RST) command.  Instruct the 
 *  Fluke 45 to measure the volts alternating current (VAC) using auto-ranging
 *  (AUTO), to wait for a trigger from the GPIB interface board (TRIGGER 2),
 *  and to assert the IEEE-488 Service Request line, SRQ, when the measurement
 *  has been completed and the Fluke 45 is ready to send the result (*SRE 16). 
 */

    ibwrt (dvm,"*RST; VAC; AUTO; TRIGGER 2; *SRE 16", 35L);
    if (ibsta & ERR) gpiberr("ibwrt Error");

/*  Initialize the accumulator of the 10 measurements to zero.              */

    sum = 0.0;

/*
 *  Establish FOR loop to read the 10 measuements.  The variable m will
 *  serve as the counter of the FOR loop.
 */

    for (m=0; m < 10 ; m++) {

     /*  Trigger the Fluke 45.                                       */

         ibtrg (dvm);
         if (ibsta & ERR) gpiberr("ibtrg Error");

     /*
      *  Request the triggered measurement by sending the instruction
      *  "VAL1?". 
      */

         ibwrt (dvm,"VAL1?", 5L);
         if (ibsta & ERR) gpiberr("ibwrt Error");

     /*
      *  Wait for the Fluke 45 to request service (RQS) or wait for the 
      *  Fluke 45 to timeout(TIMO).  The default timeout period is 10 seconds. 
      *  RQS is detected by bit position 11 (hex 800).   TIMO is detected
      *  by bit position 14 (hex 4000).  These status bits are listed under
      *  the NI-488 function IBWAIT in the Software Reference Manual.  
      */

         ibwait (dvm,TIMO|RQS);
         if (ibsta & (ERR|TIMO)) gpiberr("ibwait Error");

     /*  Read the Fluke 45 serial poll status byte.                  */

         ibrsp (dvm, &spr);
         if (ibsta & ERR) gpiberr("ibrsp Error");

     /*
      *  If the returned status byte is hex 50, the Fluke 45 has valid data to
      *  send; otherwise, it has a fault condition to report.  If the status
      *  byte is not hex 50, call DVMERR with an error message.
      */

         if (spr != 0x50) dvmerr("Fluke 45 Error", spr);
     
     /*  Read the Fluke 45 measurement.                              */
         
         ibrd (dvm,rd,10L);
         if (ibsta & ERR) gpiberr("ibrd Error");

     /*
      *  Use the null character to mark the end of the data received
      *  in the array RD.  Print the measurement received from the
      *  Fluke 45.  
      */

         rd[ibcnt] = '\0';
         printf("Reading :  %s\n", rd);

     /*  Convert the variable RD to its numeric value and add to the
      *  accumulator.    
      */

         sum = sum + atof(rd);

    }  /*  Continue FOR loop until 10 measurements are read.   */

/*  Print the average of the 10 readings.                                  */

    printf("   The average of the 10 readings is : %f\n", sum/10);

    ibonl (dvm,0);         /* Disable the hardware and software            */

}


void gpiberr(char *msg) {

    printf ("%s\n", msg);
 
    printf ("ibsta = &H%x  <", ibsta);
    if (ibsta & ERR )  printf (" ERR");
    if (ibsta & TIMO)  printf (" TIMO");
    if (ibsta & END )  printf (" END");
    if (ibsta & SRQI)  printf (" SRQI");
    if (ibsta & RQS )  printf (" RQS");
    if (ibsta & SPOLL) printf (" SPOLL");
    if (ibsta & EVENT) printf (" EVENT");
    if (ibsta & CMPL)  printf (" CMPL");
    if (ibsta & LOK )  printf (" LOK");
    if (ibsta & REM )  printf (" REM");
    if (ibsta & CIC )  printf (" CIC");
    if (ibsta & ATN )  printf (" ATN");
    if (ibsta & TACS)  printf (" TACS");
    if (ibsta & LACS)  printf (" LACS");
    if (ibsta & DTAS)  printf (" DTAS");
    if (ibsta & DCAS)  printf (" DCAS");
    printf (" >\n");             
 
    printf ("iberr = %d", iberr);
    if (iberr == EDVR) printf (" EDVR <DOS Error>\n");
    if (iberr == ECIC) printf (" ECIC <Not CIC>\n");
    if (iberr == ENOL) printf (" ENOL <No Listener>\n");
    if (iberr == EADR) printf (" EADR <Address error>\n");
    if (iberr == EARG) printf (" EARG <Invalid argument>\n");
    if (iberr == ESAC) printf (" ESAC <Not Sys Ctrlr>\n");
    if (iberr == EABO) printf (" EABO <Op. aborted>\n");
    if (iberr == ENEB) printf (" ENEB <No GPIB board>\n");
    if (iberr == EOIP) printf (" EOIP <Async I/O in prg>\n");
    if (iberr == ECAP) printf (" ECAP <No capability>\n");
    if (iberr == EFSO) printf (" EFSO <File sys. error>\n");
    if (iberr == EBUS) printf (" EBUS <Command error>\n");
    if (iberr == ESTB) printf (" ESTB <Status byte lost>\n");
    if (iberr == ESRQ) printf (" ESRQ <SRQ stuck on>\n");
    if (iberr == ETAB) printf (" ETAB <Table Overflow>\n");
 
    printf ("ibcnt = %d\n", ibcnt);
    printf ("\n");

    ibonl (dvm,0);               /* Disable the hardware and software.  */

    exit(1);                     /* Abort program.                      */

}


void dvmerr(char *msg,char spr) {

    printf ("%s\n", msg);
    printf("Status byte = %x\n", spr);

    ibonl (dvm,0);               /* Disable the hardware and software.  */

    exit(1);                     /* Abort program.                      */

}



