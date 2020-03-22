/**************************************************************
 * This example opens a session to a PXI BACKPLANE resource and 
 * routes trigger lines for an identified PXI system. 
 * The PXI BACKPLANE resource will follow the format: 
 *	PXI0::?::BACKPLANE 
 * where ? is the number of your PXI chassis in MAX. 
 * 
 * Note that this program routes triggers from a source bus 
 * to a destination bus but does not reserve a trigger line 
 * in the source bus. This could allow multiple boards to drive 
 * a single trigger line which could damage your hardware. 
 * See the "Reserve PXI Trigger" example for a method of reserving 
 * the source trigger so automatic drivers such as DAQmx avoid 
 * using the same line.
 * 
 * For more information on PXI Timing and Triggering, please see 
 * KB 3TCCT2D8 at ni.com/support
 ***************************************************************/

/* Includes */
#define NIVISA_PXI /* to include PXI VISA Attributes */

#include <visa.h>

/* Prototypes */
int RouteTrigger (ViInt16 myTrigger, int mySourceSegment, int myDestinationSegment);

/* Definitions */
int main (void)
{
	ViStatus status;

	status = RouteTrigger (0, 1, 3);

	return status;
}

int RouteTrigger (ViInt16 myTrigger, int mySourceSegment, int myDestinationSegment) 
{
	ViSession RsrcManager;
	ViSession vi;
	ViStatus status = VI_SUCCESS;

	/* Open VISA Resource Manager and PXI Backplane Resource */
	status = viOpenDefaultRM (&RsrcManager);
	if (status < VI_SUCCESS)
		goto RouteDone;

	status = viOpen (RsrcManager, "PXI0::2::BACKPLANE", 0, 0, &vi);
	if (status < VI_SUCCESS)
		goto CloseRMSession;
	
	/* Set the Source and Destination bus PXI VISA Attributes */
	status = viSetAttribute (vi, VI_ATTR_PXI_SRC_TRIG_BUS, mySourceSegment);
	if (status < VI_SUCCESS)
		goto CloseBackplaneSession;

	status = viSetAttribute (vi, VI_ATTR_PXI_DEST_TRIG_BUS,myDestinationSegment);
	if (status < VI_SUCCESS)
		goto CloseBackplaneSession;
	
	/* Route the trigger */
	status = viMapTrigger (vi, myTrigger, myTrigger, VI_NULL);
	if (status < VI_SUCCESS)
		goto CloseBackplaneSession;
	
	/* TODO: Enter code that requires the trigger routed here */
	
	/* UnRoute the trigger - only do this if we succeeded in Routing */
	status = viUnmapTrigger (vi, myTrigger, myTrigger);
	
CloseBackplaneSession:
	/* Close your VISA Sessions */
	status = viClose (vi);
CloseRMSession:
	status = viClose (RsrcManager);
RouteDone:
	return (status);
}

