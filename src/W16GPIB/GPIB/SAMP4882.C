/* ===========================================================================
 *
 *  C Example Program:   NI-488.2 functions                                 
 *
 *  This sample program is for reference only. It can only be expected to
 *  function with a Fluke 45 Digital Multimeter.
 * 
 *  The following program determines if a Fluke 45 multimeter is a listener 
 *  on the GPIB.  If the Fluke 45 is a listener, 10 measurements are read 
 *  and the average of the sum of the measurements is calculated.
 * ===========================================================================
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*  WINDECL.H contains constants, declarations, and function prototypes.     */

#include "windecl.h"

void found (unsigned int fluke); /* Fluke 45 listener on GPIB               */
void gpiberr(char *msg);         /* Error routine                           */

#define  MAVbit   0x10           /* Position of the Message Available bit.  */
       
char          buffer[101];       /* Data received from the Fluke 45         */
short         loop,              /* FOR loop counter and array index        */
              m,                 /* FOR loop counter                        */
              num_listeners,     /* Number of listeners on GPIB             */
              pad,               /* Primary address of listener on GPIB     */
              SRQasserted,       /* Set to indicate if SRQ is asserted      */
              statusByte;        /* Serial Poll Response Byte               */
double        sum;               /* Accumulator of measurements             */
Addr4882_t    fluke,             /* Primary address of the Fluke 45         */
              instruments[32],   /* Array of primary addresses              */
              result[31];        /* Array of listen addresses               */

void main() {


/*
 *  Your board needs to be the Controller-In-Charge in order to find all
 *  listeners on the GPIB.  To accomplish this, the function SendIFC is
 *  called. 
 */

    SendIFC(0);
    if (ibsta & ERR) gpiberr ("SendIFC Error");

/*
 *  Create an array containing all valid GPIB primary addresses.  Your GPIB
 *  interface board is at address 0 by default.  This  array (INSTRUMENTS)
 *  will be given to the function FindLstn to find all listeners.  The 
 *  constant NOADDR, defined in WINDECL.H, signifies the end of the array.
 */

    for (loop = 0; loop <= 30; loop++) {
       instruments[loop] = loop;
    }
    instruments[31] = NOADDR;    

/*
 *  Print message to tell user that the program is searching for all active
 *  listeners.  Find all of the listeners on the bus.   Store the listen 
 *  addresses in the array RESULT.
 */

    printf("Finding all listeners on the bus...\n");
    printf("\n");

    FindLstn(0, &instruments[1], result, 31);
    if (ibsta & ERR) gpiberr("FindLstn Error");

/*
 *  Assign the value of IBCNT to the variable NUM_LISTENERS. 
 *  Print the number of listeners found.
 */

    num_listeners = ibcnt;

    printf("Number of instruments found = %d\n", num_listeners);

/*
 *  Send the *IDN? command to each device that was found.
 *
 *  Establish a FOR loop to determine if the Fluke 45 is a listener on the
 *  GPIB.  The variable LOOP will serve as a counter for the FOR loop and
 *  as the index to the array RESULT. 
 */

    for (loop = 0; loop <= num_listeners; loop++) {

       /*
        *  Send the identification query to each listen address in the
        *  array RESULT.  The constant NLend, defined in DECL.H, instructs
        *  the function Send to append a linefeed character with EOI asserted
        *  to the end of the message.
        */

           Send(0, result[loop], "*IDN?", 5L, NLend);
           if (ibsta & ERR) gpiberr("Send Error");

       /*
        *  Read the name identification response returned from each device.  
        *  Store the response in the array BUFFER.  The constant STOPend,
        *  defined in WINDECL.H, instructs the function Receive to terminate
        *  the read when END is detected.
        */

           Receive(0, result[loop], buffer, 10L, STOPend);
           if (ibsta & ERR) gpiberr("Receive Error");

       /*
        *  The low byte of the listen address is the primary address.
        *  Assign the variable PAD the primary address of the device.
        *  The macro GetPAD, defined in WINDECL.H, returns the low byte
        *  of the listen address.
        */

           pad = GetPAD(result[loop]);

       /*
        *  Use the null character to mark the end of the data received
        *  in the array BUFFER.  Print the primary address and the name
        *  identification of the device.  
        */

           buffer[ibcnt] = '\0';
           printf("The instrument at address %d is a %s\n", pad, buffer);

       /*
        *  Determine if the name identification is the Fluke 45.  If it is
        *  the Fluke 45, assign PAD to FLUKE,  print message that the
        *  Fluke 45 has been found, call the function FOUND, and terminate 
        *  FOR loop.
        */

           if (strncmp(buffer, "FLUKE, 45", 9) == 0) {
              fluke = pad;
              printf("**** We found the Fluke ****\n");
              found(fluke);
              break;
           }

    }      /*  End of FOR loop */

    if (loop > num_listeners) printf("Did not find the Fluke!\n");

/*  Call the ibonl function to disable the hardware and software.           */

    ibonl (0,0);

}


void found(unsigned int fluke) {

/* 
 * Reset the Fluke 45 using the functions DevClear and Send.               
 *
 *  DevClear will send the GPIB Selected Device Clear (SDC) command message 
 *  to the Fluke 45.  If the error bit ERR is set in IBSTA, call GPIBERR with 
 *  an error message.
 */

    DevClear(0, fluke);
    if (ibsta & ERR) gpiberr("DevClear Error");

/*   
 *  Use the function Send to send the IEEE-488.2 reset command (*RST) 
 *  to the Fluke 45.  The constant NLend, defined in DECL.H, instructs
 *  the function Send to append a linefeed character with EOI asserted
 *  to the end of the message. 
 */

    Send(0, fluke, "*RST", 4L, NLend);
    if (ibsta & ERR) gpiberr("Send *RST Error");

/*
 *  Use the function Send to send device configuration commands to the 
 *  Fluke 45.  Instruct the Fluke 45 to measure volts alternating current 
 *  (VAC) using auto-ranging (AUTO), to wait for a trigger from the GPIB 
 *  interface board (TRIGGER 2), and to assert the IEEE-488 Service Request
 *  line, SRQ, when the measurement has been completed and the Fluke 45 is
 *  ready to send the result (*SRE 16).
 */

    Send(0, fluke, "VAC; AUTO; TRIGGER 2; *SRE 16", 29L, NLend);
    if (ibsta & ERR) gpiberr("Send Setup Error");

/*  Initialized the accumulator of the 10 measurements to zero.             */

    sum = 0.0;

/*
 *  Establish FOR loop to read the 10 measurements.  The variable m will
 *  serve as the counter of the FOR loop.
 */

    for (m=0; m < 10 ; m++) {

       /*
        *  Trigger the Fluke 45 by sending the trigger command (*TRG) and
        *  request a measurement by sending the command "VAL1?".
        */

           Send(0, fluke, "*TRG; VAL1?", 11L, NLend);
           if (ibsta & ERR) gpiberr("Send Trigger Error");

        /*
         *  Wait for the Fluke 45 to assert SRQ, meaning it is ready to send 
         *  a measurement.  If SRQ is not asserted within the timeout period,
         *  call GPIBERR with an error message.  The timeout period by default
         *  is 10 seconds.
         */

            WaitSRQ(0, &SRQasserted);
            if (!SRQasserted) {
                printf("SRQ is not asserted.  The Fluke is not ready.\n");
                exit(1);
            }

       /*
        *  Read the serial poll status byte of the Fluke 45.  If the error 
        *  bit ERR is set in IBSTA, call GPIBERR with an error message.
        */ 

           ReadStatusByte(0, fluke, &statusByte);
           if (ibsta & ERR) gpiberr("ReadStatusByte Error");

       /*
        *  Check if the Message Available Bit (bit 4) of the return status 
        *  byte is set.  If this bit is not set, print the status byte and 
        *  call GPIBERR with an error message.
        */

           if (!(statusByte & MAVbit)) {
               printf(" Status Byte = 0x%x\n", statusByte);
               gpiberr("Improper Status Byte");
           }

       /*
        *  Read the Fluke 45 measurement.  Store the measurement in the
        *  variable BUFFER.  The constant STOPend, defined in DECL.H, 
        *  instructs the function Receive to terminate the read when END
        *  is detected.  If the error bit ERR is set in IBSTA, call
        *  GPIBERR with an error message.
        */

           Receive(0, fluke, buffer, 10L, STOPend);
           if (ibsta & ERR) gpiberr("Receive Error");

       /*
        *  Use the null character to mark the end of the data received
        *  in the array BUFFER.  Print the measurement received from the
        *  Fluke 45.  
        */

           buffer[ibcnt] = '\0';
           printf("Reading :  %s\n", buffer);

       /*  Convert the variable BUFFER to its numeric value and add to the
        *  accumulator.    
        */

           sum = sum + atof(buffer);

    }  /*  Continue FOR loop until 10 measurements are read.   */

/*  Print the average of the 10 readings.                                   */

    printf("   The average of the 10 readings is : %f\n", sum/10);

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
 
    printf ("ibcnt = %d\n", ibcntl);
    printf ("\n");

    ibonl (0,0);      /* Disable the hardware and software.           */

    exit(1);          /* Abort program                                */

}
