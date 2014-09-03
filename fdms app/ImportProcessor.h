//////////////////////////////////////////////////////////////////////////
// Copyright 2005, The Regents Of the University of California.  This
// software was produced under U.S. Government contract (W-7405-ENG-36) at
// the Los Alamos National Laboratory by the Safeguards Science and
// Technology Group (N-1).  All Rights Reserved.
//////////////////////////////////////////////////////////////////////////
// $History: ImportProcessor.h $
// 
// *****************  Version 7  *****************
// User: Longo        Date: 11/14/05   Time: 3:44p
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 6  *****************
// User: Longo        Date: 3/16/05    Time: 2:21p
// Updated in $/FDMS/FDMS Application/FDMSNET
// nearing the deadlines ...
// 
// *****************  Version 5  *****************
// User: Longo        Date: 2/28/05    Time: 10:00a
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 4  *****************
// User: Longo        Date: 2/07/05    Time: 10:19a
// Updated in $/FDMS/FDMS Application/FDMSNET
// checkpoint checkin
// 
// *****************  Version 3  *****************
// User: Longo        Date: 1/07/05    Time: 2:25p
// Updated in $/FDMS/FDMS Application/FDMSNET
// adding the secondary import editor dilaog
// 
// *****************  Version 2  *****************
// User: Longo        Date: 1/06/05    Time: 10:05a
// Updated in $/FDMS/FDMS Application/FDMSNET
// threaded import processing for UI interaction
// 
// *****************  Version 1  *****************
// User: Longo        Date: 1/05/05    Time: 5:25p
// Created in $/FDMS/FDMS Application/FDMSNET
// initial import code
//////////////////////////////////////////////////////////////////////////
#pragma once

#import "..\Coms\CoGrandImport.dll"   named_guids
#include ".\mevent.h"

#define STATION_ID_TYPE long
#define OLD_STATION_ID_TYPE short

// ImportProcessor

class ImportProcessor : public CWinThread
{
	DECLARE_DYNCREATE(ImportProcessor)

protected:
	ImportProcessor();           // protected constructor used by dynamic creation
	virtual ~ImportProcessor();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()

	void Import();

private:
	RADEventsList* pRADEventsList;
	short mFacilityID;
	STATION_ID_TYPE mStationID;
	CString mStationShortName, mInstrumentType;
	CString mFacilityLongName, mFacilityShortName;

	CObList* pM;
	CObList* pB;

	void ComposeMeasurement(
		RADEvent* pRADEvent, COGRANDIMPORTLib::IGrandImportPtr& pIGrandImport,
		PTCHAR StationLongName);

	void ProcessImportedMeasurements();
};


