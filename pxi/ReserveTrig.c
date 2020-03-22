/**************************************************************
 * This example opens a session to a PXI BACKPLANE resource and 
 * reserves trigger lines for an identified PXI system. 
 * The PXI BACKPLANE resource will follow the format: 
 *	PXI0::?::BACKPLANE 
 * where ? is the number of your PXI chassis in MAX. 
 * 
 * Note that the unreserve call only executes if the reservation 
 * was successfully made. You must use caution when unreserving 
 * triggers as you could remove automatic reservations made by 
 * drivers such as DAQmx which could result in hardware damage.
 * 
 * For more information on PXI Timing and Triggering, please see 
 * KB 3TCCT2D8 at ni.com/support
 ***************************************************************/


/* Includes */
#define NIVISA_PXI /* to include PXI VISA Attributes */

#include <visa.h>

/* Prototypes */
int ReserveTrigger (int myTrigger, int myBusSegment);

/* Definitions */
int main (void)
{
	ViStatus status;

	status = ReserveTrigger (0, 1);
	
	return status;
}

int ReserveTrigger (int myTrigger, int myBusSegment) 
{
	ViSession RsrcManager;
	ViSession vi;
	ViStatus status = VI_SUCCESS;
	
	/* Open VISA Resource Manager and PXI Backplane Resource */
	status = viOpenDefaultRM (&RsrcManager);
	if (status < VI_SUCCESS)
		goto ReserveDone;

	status = viOpen (RsrcManager, "PXI0::2::BACKPLANE", 0, 0, &vi);
	if (status < VI_SUCCESS)
		goto CloseRMSession;
	
	/* Set the Trigger ID and the Trigger Bus Attributes */
	status = viSetAttribute (vi, VI_ATTR_TRIG_ID, myTrigger);
	if (status < VI_SUCCESS)
		goto CloseBackplaneSession;

	status = viSetAttribute (vi, VI_ATTR_PXI_TRIG_BUS, myBusSegment);
	if (status < VI_SUCCESS)
		goto CloseBackplaneSession;
	
	/* Reserve the Trigger */
	status = viAssertTrigger (vi, VI_TRIG_PROT_RESERVE);
	if (status < VI_SUCCESS)
		goto CloseBackplaneSession;
	
	/* TODO: Enter code that requires the trigger reserved here */

	/* UnReserve the Trigger - only do this if we succeeded in Reserving */
	status = viAssertTrigger (vi, VI_TRIG_PROT_UNRESERVE);
	
CloseBackplaneSession:
	/* Close the VISA Session to the PXI Backplane */
	viClose (vi);
CloseRMSession:
	viClose (RsrcManager);
ReserveDone:	
	return (status);
}

