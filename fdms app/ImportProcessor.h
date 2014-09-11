/*
Copyright (c) 2014, Los Alamos National Security, LLC
All rights reserved.
Copyright 2014. Los Alamos National Security, LLC. This software was produced under U.S. Government contract 
DE-AC52-06NA25396 for Los Alamos National Laboratory (LANL), which is operated by Los Alamos National Security, 
LLC for the U.S. Department of Energy. The U.S. Government has rights to use, reproduce, and distribute this software.  
NEITHER THE GOVERNMENT NOR LOS ALAMOS NATIONAL SECURITY, LLC MAKES ANY WARRANTY, EXPRESS OR IMPLIED, 
OR ASSUMES ANY LIABILITY FOR THE USE OF THIS SOFTWARE.  If software is modified to produce derivative works, 
such modified software should be clearly marked, so as not to confuse it with the version available from LANL.

Additionally, redistribution and use in source and binary forms, with or without modification, are permitted provided 
that the following conditions are met:
•	Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
disclaimer. 
•	Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
disclaimer in the documentation and/or other materials provided with the distribution. 
•	Neither the name of Los Alamos National Security, LLC, Los Alamos National Laboratory, LANL, the U.S. Government, 
nor the names of its contributors may be used to endorse or promote products derived from this software without specific 
prior written permission. 
THIS SOFTWARE IS PROVIDED BY LOS ALAMOS NATIONAL SECURITY, LLC AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED 
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL LOS ALAMOS NATIONAL SECURITY, LLC OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
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


