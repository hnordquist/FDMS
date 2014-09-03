//////////////////////////////////////////////////////////////////////////
// Copyright 2005, The Regents Of the University of California.  This
// software was produced under U.S. Government contract (W-7405-ENG-36) at
// the Los Alamos National Laboratory by the Safeguards Science and
// Technology Group (N-1).  All Rights Reserved.
//////////////////////////////////////////////////////////////////////////
// $History: test.cpp $
// 
// *****************  Version 5  *****************
// User: Longo        Date: 2/07/05    Time: 10:20a
// Updated in $/FDMS/FDMS Application/FDMSNET/tests
// 
// *****************  Version 4  *****************
// User: Longo        Date: 12/23/04   Time: 2:59p
// Updated in $/FDMS/FDMS Application/FDMSNET/tests
// 
// *****************  Version 3  *****************
// User: Longo        Date: 12/14/04   Time: 9:27a
// Updated in $/FDMS/FDMS Application/FDMSNET/tests
// 
// *****************  Version 2  *****************
// User: Longo        Date: 12/09/04   Time: 12:55p
// Updated in $/FDMS/FDMS Application/FDMSNET/tests
// 
// *****************  Version 1  *****************
// User: Longo        Date: 10/20/04   Time: 1:11p
// Created in $/FDMS/FDMS Application/FDMSNET/tests
// registry failed today, baselining

#include "stdafx.h"

#include "..\AppUL\radeventlist.h"
#include ".\test.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


////////////////////////////////////////////////////////

void TestFDMSRAD()
{

	// create a new RAD Event List
	RADEventsList* pRadEventsList = new RADEventsList();

	// The external location and name of the RAD to FDMS data interchange file are predefined as .\radfdms.xml,
	// The file name and path can be manually set with these two operations

	pRadEventsList->SetLocationPath(".");		// create the external file in the current working directory
	pRadEventsList->SetFileName("jeeves.xml");  // chaage the name of the external file to 'jeeves.xml'

	// create a new event object
	RADEvent* pOneRADEvent = new RADEvent();

	// place string representations of each of the RAD values onto the RADEvent object
	pOneRADEvent->SetREValue(eRADSequenceNum,"0057");
	pOneRADEvent->SetREValue(eRADStartTimestamp,"0000020010826175602000");
	pOneRADEvent->SetREValue(eRADEndTimestamp,"0000020010826175827000");
	pOneRADEvent->SetREValue(eRADMaxTimestamp,"0000020010826175702000");
	
	pOneRADEvent->SetREValue(eRADIntegralSum,"10941.0");
	pOneRADEvent->SetREValue(eRADMaxValue,"84");
	pOneRADEvent->SetREValue(eRADAvgRealsRate,"8101.01");
	pOneRADEvent->SetREValue(eRADAvgTotalsRate,"8928.23l29029");
	
	pOneRADEvent->SetREValue(eRADStationType,"beowulf");
	pOneRADEvent->SetREValue(eRADStationName,L"GRAND 2");

	pOneRADEvent->SetREValue(eRADEventChannel,L"1");
	pOneRADEvent->SetREValue(eRADEventType,"Background");
	pOneRADEvent->SetREValue(eRADEventSubType,L"Normal Background");

	pOneRADEvent->SetREValue(eRADDataPointNum,L"32");
	pOneRADEvent->SetREValue(eRADEventID,L"CV-17");

	// add the RAD Event to the Event List
	pRadEventsList->AddToEnd(pOneRADEvent);

	// create a new event object
	pOneRADEvent = new RADEvent();

	// populate the object with the  event info strings
	pOneRADEvent->SetREValue(eRADSequenceNum,"0059");
	pOneRADEvent->SetREValue(eRADStartTimestamp,"0000020010828175612000");
	pOneRADEvent->SetREValue(eRADEndTimestamp,"0000020010828175812000");
	pOneRADEvent->SetREValue(eRADMaxTimestamp,"00000200108281757012000");
	pOneRADEvent->SetREValue(eRADIntegralSum,"55541.0");
	pOneRADEvent->SetREValue(eRADMaxValue,"99");

	pOneRADEvent->SetREValue(eRADAvgRealsRate,"100101.01");
	pOneRADEvent->SetREValue(eRADAvgTotalsRate,"938928.23l29029");
	
	pOneRADEvent->SetREValue(eRADStationType,"beowulf");
	pOneRADEvent->SetREValue(eRADStationName,L"GRAND 2");

	pOneRADEvent->SetREValue(eRADEventChannel,L"1");
	pOneRADEvent->SetREValue(eRADEventType,"Measurement");
	pOneRADEvent->SetREValue(eRADEventSubType,L"Totals");

	pOneRADEvent->SetREValue(eRADDataPointNum,L"32");
	pOneRADEvent->SetREValue(eRADEventID,L"CV-17");;

	// add the RAD Event to the Event List
	pRadEventsList->AddToEnd(pOneRADEvent);

	// Save the 2 element event list to an external location.
	// Persist is the operation that creates (or overwrites the pre-existing) ".\jeeves.xml".
	pRadEventsList->Persist();

	// Delete the RADEvents list object, thelist destructor also deletes each RADEvent object added to the list above
	delete pRadEventsList;
}


