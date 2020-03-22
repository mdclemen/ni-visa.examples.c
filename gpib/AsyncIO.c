/**************************************************************************/
/*                 Asynchronous I/O Completion Example                    */
/*                                                                        */
/*  This example shows how to use an asynchronous event handling function */
/*  that is called when an asynchronous input/output operation completes. */
/*  Compare this to viRead and viWrite which block the application until  */
/*  either the call returns successfully or a timeout occurs.  Read and   */
/*  write operations can be quite slow sometimes, so these asynchronous   */
/*  operations will allow you processor to perform other tasks.           */
/*  The code uses VISA functions and sets a flag in the callback upon     */
/*  completion of an asynchronous read from a GPIB device to break out of */
/*  an otherwise infinite loop.  The flow of the code is as follows:      */
/*                                                                        */
/*  Open A Session To The VISA Resource Manager                           */
/*  Open A Session To A GPIB Device                                       */
/*  Install A Handler For Asynchronous IO Completion Events               */
/*  Enable Asynchronous IO Completion Events                              */
/*  Write A Command To The Instrument                                     */
/*  Call The Asynchronous Read Command                                    */
/*  Start A Loop That Can Only Be Broken By A Handler Flag Or Timeout     */
/*  Print Out The Returned Data                                           */
/*  Close The Instrument Session                                          */
/*  Close The Resource Manager Session                                    */
/**************************************************************************/

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_DEPRECATE)
/* Functions like strcpy are technically not secure because they do */
/* not contain a 'length'. But we disable this warning for the VISA */
/* examples since we never copy more than the actual buffer size.   */
#define _CRT_SECURE_NO_DEPRECATE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "visa.h"

/*  Prototype for the event handler for asynchronous i/o completion     */
ViStatus _VI_FUNCH AsyncHandler(ViSession vi, ViEventType etype, ViEvent event, ViAddr userHandle);

#define READ_BUFFER_SIZE 4096

static ViJobId job;
static unsigned char data[READ_BUFFER_SIZE];
static ViAddr uhandle;
static ViStatus status, StatusSession;
static ViSession inst, Sessionparam;
static ViEventType EventTypeparam;
static ViAddr Addressparam;
static ViUInt32 BytesToWrite;
static ViSession defaultRM;
static ViUInt32 rcount, RdCount;
static volatile ViBoolean stopflag = VI_FALSE;
static int letter;
static char stringinput[256];

/*
 * The handler function. The instrument session, the type of event, and a 
 * handle to the event are passed to the function along with a user handle
 * which is basically a label that could be used to reference the handler.
 * The only thing done in the handler is to set a flag that allows the
 * program to finish.  Always return VI_SUCCESS from your handler.
 */
ViStatus _VI_FUNCH AsyncHandler(ViSession vi, ViEventType etype, ViEvent event, ViAddr userHandle)
{
   Sessionparam = vi;
   EventTypeparam = etype;
   Addressparam = userHandle;
   viGetAttribute (event, VI_ATTR_STATUS, &StatusSession);
   viGetAttribute (event, VI_ATTR_RET_COUNT, &RdCount);
   stopflag = VI_TRUE;
   return VI_SUCCESS;
}


int main (void)
{
   /*
    *  First we open a session to the VISA resource manager.  We are
    *  returned a handle to the resource manager session that we must
    *  use to open sessions to specific instruments.
    */
   status = viOpenDefaultRM (&defaultRM);
   if (status < VI_SUCCESS)
   {
      printf("Could not open a session to the VISA Resource Manager!\n");
      exit (EXIT_FAILURE);
   }
   
   /*
    *  Next we use the resource manager handle to open a session to a
    *  GPIB instrument at device 2.  A handle to this session is
    *  returned in the handle inst.  Please consult the NI-VISA User Manual 
    *  for the syntax for using other instruments.
    */
   status = viOpen (defaultRM, "GPIB::2::INSTR", VI_NULL, VI_NULL, &inst);
   
   /*
    *  Now we install the handler for asynchronous i/o completion events.
    *  To install the handler, we must pass our instrument session, the type of
    *  event to handle, the handler function name and a user handle
    *  which acts as a handle to the handler function.
    */
   status = viInstallHandler (inst, VI_EVENT_IO_COMPLETION, AsyncHandler, uhandle);
   
   /*  Now we must actually enable the I/O completion event so that our
    *  handler will see the events.  Note, one of the parameters is 
    *  VI_HNDLR indicating that we want the events to be handled by
    *  an asynchronous event handler.  The alternate mechanism for handling
    *  events is to queue them and read events off of the queue when
    *  you want to check them in your program.
    */
   status = viEnableEvent (inst, VI_EVENT_IO_COMPLETION, VI_HNDLR, VI_NULL);

   /* 
    *  Now the VISA write command is used to send a request to the
    *  instrument to generate a sine wave.  This demonstrates the 
    *   synchronous read operation that blocks the application until viRead()
    *   returns.  Note that the command syntax is instrument specific.
    */

   /*
    * Here you specify which string you wish to send to your instrument.
    * The command listed below is device specific. You may have to change
    * command to accommodate your instrument.
    */
   strcpy(stringinput,"SOUR:FUNC SIN; SENS: DATA?\n");
   BytesToWrite = (ViUInt32)strlen(stringinput);
   status = viWrite (inst, (ViBuf)stringinput,BytesToWrite, &rcount);

   /*
    *  Next the asynchronous read command is called to read back the 
    *  date from the instrument.  Immediately after this is called
    *  the program goes into a loop which will terminate
    *  on an i/o completion event triggering the asynchronous callback.
    *  Note that the asynchronous read command returns a job id that is
    *  a handle to the asynchronous command.  We can use this handle
    *  to terminate the read if too much time has passed.
    */
   status = viReadAsync (inst, data, READ_BUFFER_SIZE - 1, &job);
   
   printf("\n\nHit enter to continue.");
   letter = getchar();

   /* 
    *  If the asynchronous callback was called and the flag was set
    *  we print out the returned data otherwise we terminate the
    *  asynchronous job.
    */
   if (stopflag == VI_TRUE)
   {
      /* RdCount was set in the callback */
      /* Add a NULL terminator to the read buffer */
      data[RdCount] = '\0';
      printf ("Here is the data:  %s", data);
   }
   else
   {
      status = viTerminate (inst, VI_NULL, job);  
      printf("The asynchronous read did not complete.\n");
   }

    printf ("\nHit enter to continue.");
    fflush(stdin);
    getchar();

   /*
    *  Now we close the instrument session and the resource manager
    *  session to free up resources.
    */
   status = viClose(inst);
   status = viClose(defaultRM);
   
   return 0;
}

