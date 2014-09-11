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
// $History: ImportProcessor.cpp $
// 
// *****************  Version 20  *****************
// User: 186846       Date: 4/02/08    Time: 10:41a
// Updated in $/FDMS/FDMSNET
// 397
// 
// *****************  Version 19  *****************
// User: 186846       Date: 4/01/08    Time: 3:40p
// Updated in $/FDMS/FDMSNET
// Mar. 29, fix for 398, partial for 447
// 
// *****************  Version 18  *****************
// User: 186846       Date: 2/27/08    Time: 4:08p
// Updated in $/FDMS/FDMSNET
// Visual update status now appears during import processing. Import
// processing is reallllly slow due to FM and GI COM implementations.
// 
// *****************  Version 17  *****************
// User: 186846       Date: 2/26/08    Time: 8:54a
// Updated in $/FDMS/FDMS Application/FDMSNET
// checkpoint
// 
// *****************  Version 16  *****************
// User: 186846       Date: 1/30/08    Time: 4:33p
// Updated in $/FDMS/FDMS Application/FDMSNET
// 447/448/399
// 
// *****************  Version 15  *****************
// User: 186846       Date: 1/28/08    Time: 4:06p
// Updated in $/FDMS/FDMS Application/FDMSNET
// use of new grand and FM
// 
// *****************  Version 14  *****************
// User: 186846       Date: 12/19/07   Time: 2:42p
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 13  *****************
// User: Longo        Date: 11/14/05   Time: 3:43p
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 12  *****************
// User: Longo        Date: 7/27/05    Time: 3:52p
// Updated in $/FDMS/FDMS Application/FDMSNET
// long staion id use
// fixed chanel off by 1 error on import
// 
// *****************  Version 11  *****************
// User: Longo        Date: 3/17/05    Time: 9:32a
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 10  *****************
// User: Longo        Date: 3/16/05    Time: 2:21p
// Updated in $/FDMS/FDMS Application/FDMSNET
// nearing the deadlines ...
// 
// *****************  Version 9  *****************
// User: Longo        Date: 3/11/05    Time: 10:35a
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 8  *****************
// User: Longo        Date: 3/09/05    Time: 3:47p
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 7  *****************
// User: Longo        Date: 2/28/05    Time: 10:00a
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 6  *****************
// User: Longo        Date: 2/10/05    Time: 3:46p
// Updated in $/FDMS/FDMS Application/FDMSNET
// checkpoint checkin
// 
// *****************  Version 5  *****************
// User: Longo        Date: 2/07/05    Time: 10:19a
// Updated in $/FDMS/FDMS Application/FDMSNET
// checkpoint checkin
// 
// *****************  Version 4  *****************
// User: Longo        Date: 1/10/05    Time: 12:47p
// Updated in $/FDMS/FDMS Application/FDMSNET
// matching on date time AND facility, detector and item id
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


#include "stdafx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#import "..\Coms\CoFacilityMgr.exe"  no_namespace named_guids

#include <msxml2.h>
#include <stdio.h>
#include <comutil.h>

#include "..\FDMS.h"
#include "..\externalmeasurements.h"

#include "..\doassay.h"
#include "..\selectmeasurement.h"
#include "..\dconfig.h"
#include ".\importprocessor.h"
#include "ULTooToolBar.h"
#include ".\matchmeasurement.h"


#include ".\xlc\MatchingEventDlg.h"
#include ".\xlc\NewEventDlg.h"
#include ".\xlc\BkgEventDlg.h"


// ImportProcessor

IMPLEMENT_DYNCREATE(ImportProcessor, CWinThread)

ImportProcessor::ImportProcessor()
{
	pM = new CObList();
	pB = new CObList();
}

ImportProcessor::~ImportProcessor()
{
	if (pM)
	{
		POSITION pos = pM->GetHeadPosition();
		while( pos != NULL )
		{
			MEvent* p = (MEvent*)pM->GetNext( pos );
			if (p)
				delete p;
		}
		pM->RemoveAll();
		delete pM;
	}
	if (pB)
		delete pB;
}

BOOL ImportProcessor::InitInstance()
{
	ULTooToolBar::m_bImportActive = true;
	CoInitialize(NULL);
	dvaprintf("start ImportProcessor thread\n");
	//Sleep(10000);
	Import();

	ProcessImportedMeasurements();

	if (pRADEventsList)
	{
		delete pRADEventsList;
		pRADEventsList = NULL;
	}

	return FALSE;
}

int ImportProcessor::ExitInstance()
{
	ULTooToolBar::m_bImportActive = false;
	dvaprintf("end ImportProcessor thread\n");
	CoUninitialize();
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(ImportProcessor, CWinThread)
END_MESSAGE_MAP()



// Macro that calls a COM method returning HRESULT value:
#define HRCALL(a, errmsg) \
do { \
    hr = (a); \
    if (FAILED(hr)) { \
        dvaprintf( "%s:%d  HRCALL Failed: %s\n  0x%.8x = %s\n", \
                __FILE__, __LINE__, errmsg, hr, #a ); \
        goto clean; \
    } \
} while (0)


void handlecomerr(const _com_error& comerr)
{
	WCHAR wszMessage[1024] = {0};

	if (_snwprintf(
			wszMessage, 
			(sizeof(wszMessage)/sizeof(wszMessage[0])) - 1, 
			L"Error Code: 0x%X\nError Description: %s\n", 
			comerr.Error(),
			(WCHAR*)comerr.Description()
			) < 0)
	{
		wprintf(L"The message is too long for the buffer specified.\n");
	}
	else
	{
		wszMessage[(sizeof(wszMessage)/sizeof(wszMessage[0])) - 1] = L'\0';
		MessageBoxW(NULL, wszMessage, L"FDMS Event Import", MB_OK);
	}
}

#include "StartupImpDlg.h"

void ImportProcessor::ProcessImportedMeasurements()
{
	try
	{
		//	These are all the events that have data we want to check out
		StartupImpDlg StartUpDlg;
		StartUpDlg.DoIt("Find New Measurements . . .");

		// now, must identify those that are new, mark them
		POSITION pos = pM->GetHeadPosition();
		while( pos != NULL )
		{
			MEvent* p = (MEvent*)pM->GetNext( pos );
			p->Match();  // finds the closest match in time that matches the identity settings
		}

		/* case 1:	marked events match measurement timestamps found in the database
					but do they match the detector, or item id?
		   case 2:	unmarked events do not match on timestamp, may match on facility, mtype (B or (V,C))item id (or not)	

		   put up an editable dialog that shows all of these events with the matching components and unknown or unmatching
		   then for each row, allow these operations
		       fill in missing data for a matching entry (if entry)
			   
			   subclasses of data probably require some subtlety

			   for a new measurement, require a declaration to be entered into the database and what about the measurement plan?

			   The primary step now is to build the UI component and get it to work

			   use the original editable init file id class to get this going

		 */

		// put up import editor dialog that lets each measurement event be identified, then process them.

		bool bNewToProcess = false;
		bool bExistingToUpdate = false;
		bool bBkgToProcess = false;
		pos = pM->GetHeadPosition();
		CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
		StartUpDlg.SetCount(pM->GetCount());
		StartUpDlg.BumpCount();

		while( pos != NULL )
		{
			MEvent* p = (MEvent*)pM->GetNext( pos );
			if (p->GetMark())
			{
				if (!p->m_bExactMatch[eisMP])
				{
					bExistingToUpdate = true;
					pApp->AppendToLogF("* existing %s", p->Image());
				}
				else
				{
					pApp->AppendToLogF("== exact %s", p->Image());
				}
			}
			else if (p->GetAssayType() == eBackgroundAssay)
			{
				pApp->AppendToLogF("background %s [BKG]", p->Image());
				bBkgToProcess = true;
				StartUpDlg.BumpCount();
				Sleep(150);
			}
			else // 
			{
				pApp->AppendToLogF("new      %s [%d]", p->Image(), p->m_iGIndex[eisMP]);
				bNewToProcess = true;
				StartUpDlg.BumpCount();
				Sleep(150);
			}
		}
		StartUpDlg.DestroyWindow();
		if (!bExistingToUpdate && !bNewToProcess)
		{
			CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
			CString s, msg;
			s = "Imported Events from Radiation Review";
			msg = "No new or updated measurements were found in the imported events from Radiation Review.";
			pApp->MyMessageBox( msg, MB_OK | MB_ICONEXCLAMATION, (UINT)-1, s);
			bBkgToProcess = false;
		}

		if (bBkgToProcess)
		{
			BkgEventDlg x(true, pM, pB, AfxGetMainWnd());  // display all the bkg events, user operates upon and chooses the events to add to the MP
			x.DoModal();
		}
		if (bNewToProcess)
		{
			NewEventDlg y(true, pM, pB, AfxGetMainWnd());  // display all the new events in a dialog, user operates upon and chooses the events to add to the MP
			y.DoModal();
		}
		if (bExistingToUpdate)
		{
			MatchingEventDlg z(false, pM, pB, AfxGetMainWnd()); // display all the events that had a real match, user operates and chooses said same self serk
			z.DoModal();
		}
		// alternate technique: gather the matching items and the new items from the pM list and do the final storage here
	}
	catch( CMemoryException* theException )
	{
		theException->Delete();
	}
	catch (const _com_error& comerr) 
	{
		handlecomerr(comerr);				
	}
	catch (...)
	{
	}

}


void ImportProcessor::ComposeMeasurement(
		RADEvent* pRADEvent, COGRANDIMPORTLib::IGrandImportPtr& pIGrandImport,
		PTCHAR StationLongName)
{

	// Get the relevant data statistics for the {facility, station} combo
	// on the time span, per channel.
	//
	ULONG ulNumPts = 0;
	ULONG ulMaxPts = 0;
	DATE dActualStart = 0.0;
	DATE dActualEnd = 0.0;
	DATE dStart = pRADEvent->m_Times[RADEvent::eStartTimestamp];  // get from Event object
	DATE dEnd = pRADEvent->m_Times[RADEvent::eEndTimestamp];	// get from Event object

	for (short chan = 1; chan <= 5; chan++)
	{
		long lNumPts;
		VARIANT_BOOL bStatus = ((COGRANDIMPORTLib::IGrandImportPtr)pIGrandImport)->GetDataStatistics(mFacilityID,mStationID, chan, dStart, dEnd, &dActualStart, &dActualEnd, &lNumPts);
		if (bStatus)
		{ //extract info
			ulNumPts = lNumPts;
			ulMaxPts = max(ulNumPts, ulMaxPts);
			// NOTE! FDMS restricts the maximum cycle count to 128
		}
		else
		{
			continue;  // skip this channel of the measurement
		}
	}

	if (ulMaxPts > 0)
	{
		if (ulMaxPts > MAXIMUM_CYCLES_AUTHORIZED) // jfl todo: what to do if it is more?
			ulMaxPts = MAXIMUM_CYCLES_AUTHORIZED;
		VARIANT GrandVarStruct;
		GrandVarStruct.lVal = ulMaxPts;
		GrandVarStruct.vt = VT_INT;
		int iStatus = 0;
		try
		{
			iStatus = ((COGRANDIMPORTLib::IGrandImportPtr)pIGrandImport)->GetDataByStation(
				mFacilityID,mStationID, dStart, dEnd, &GrandVarStruct);

			if (iStatus &&	(GrandVarStruct.vt == (VT_VARIANT | VT_ARRAY)))
			{
				CString szStationLongName(StationLongName);

				MEvent* pE = new MEvent(pRADEvent,
					ulMaxPts,mFacilityID,mStationID, dStart, dEnd, 
					mFacilityLongName,	mFacilityShortName,
					szStationLongName,	mStationShortName,
					mInstrumentType);
				pM->AddTail(pE);

				ulNumPts = min(ulNumPts, MAXIMUM_CYCLES_AUTHORIZED);
				for (ULONG i = 0; i < ulNumPts; i++)
				{
					for (ULONG j = 0; j < 5; j++)  // 0 .. 4 are na, nb, nc, g1, g2
					{
						bool bSuccess = false;
						long alIndex[2];
						VARIANT Var;
						alIndex[0] = i;
						alIndex[1] = j+2;
						HRESULT hr = SafeArrayGetElement(GrandVarStruct.parray, alIndex, &Var);
						if (hr == S_OK) 
							pE->pdData[j][i] = (float)Var.dblVal;
					}
				}

				SafeArrayDestroy(GrandVarStruct.parray);
			}
		}
		catch( CMemoryException* theException )
		{
			theException->Delete();
		}
		catch (const _com_error& comerr) 
		{
			handlecomerr(comerr);				
		}
		catch (...)
		{
		}
	} //there are some data points
	else
	{
		CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
		pApp->AppendToLogF("%s data not found at time:%s d:%s id:%s",   pRADEvent->values[eRADEventType], pRADEvent->values[eRADEndTimestamp], pRADEvent->values[eRADStationName], pRADEvent->values[eRADEventID]);

	}
}  

void 	ImportProcessor::Import()
{
	//Let the user know that application is starting
	StartupImpDlg StartUpDlg;
	StartUpDlg.DoIt("Importing Measurements . . .");

	try
	{

		CIniFileProcessor::GetIRSFacilityID(mFacilityID);

		pRADEventsList = new RADEventsList();

		// The external location and name of the RAD to FDMS data interchange file are predefined as .\radfdms.xml,

		pRADEventsList->SetFullFilePath(ULSupport::ULIRSDataPath.GetBuffer());

		// Find the file named "ULSupport::ULIRSDataPath\rfname", open the file, and parse the XML found in the file.
		// For each event foud in the XML, create a RADEvent object and insert it on the RADEvents list
		pRADEventsList->Populate();

		pRADEventsList->CullSingletons();

	}
	catch (...)
	{
	}

	StartUpDlg.SetCount(pRADEventsList->GetCount());

	// pRADEventsList now has a list of 0 or more event objects.
	// Each event object has string representation and typed representations of the 15 values

	// This block uses the information in each RADEvent event object to obtain the measurement data for each event.

	if (pRADEventsList->GetHeadPosition() != NULL) // there are some events to process
	try
	{

		CString FacilityDirectory;

		// Using the Facility Manager, obtain info on the current facility
		IFacilityMgrDataPtr pIFacMgrData(CLSID_FacilityMgrData);
		mFacilityLongName = pIFacMgrData->GetFacilityLongName(mFacilityID).GetBSTR();
		mFacilityShortName = pIFacMgrData->GetFacilityShortName(mFacilityID).GetBSTR();
		FacilityDirectory = pIFacMgrData->GetFacilityDirectory(mFacilityID).GetBSTR();

		// Get the data using the current facility id and station id, as well as the time slice data and
		// The station id is also known as the Detector ID. Use the RADEvent value for eRADStationName to obtain the station id.

		// note: must get data by channel,
		// Channels a, b and c for neutron counts 
		// are apparently ids 1, 2 & 3 respectively
		// Gamma1 and Gamma2 are thus 4 and 5. 

		COGRANDIMPORTLib::IGrandImportPtr pIGrandImport(__uuidof(COGRANDIMPORTLib::GrandImport));

		int iStatus;
		mStationID = 0;

		// set up the import COM with the Facility ID
		iStatus = pIGrandImport->OpenDataStore(mFacilityID);
		if (iStatus != 0)
		{
			POSITION pos = pRADEventsList->GetHeadPosition();
			while( pos != NULL )
			{
				RADEvent* pRADEvent = (RADEvent*)pRADEventsList->GetNext( pos );

				// Get the station info here, the station name is on each event record.
				// Get the numerical id, and the short name.
				// Get the instrument type, too. Get it from the long name call.
				_bstr_t bstrStationLongName = pRADEvent->values[eRADStationName];
				if (bstrStationLongName.length() <= 0) // skip if empty station name
					continue;

				mStationID = 0;
				_bstr_t bstrInstrumentType;
				HRESULT hr = pIFacMgrData->GetStationID(mFacilityID, bstrStationLongName, &mStationID, bstrInstrumentType.GetAddress());
				if (FAILED(hr))
				{
					continue;
				}
				StartUpDlg.BumpCount();

				mStationShortName = pIFacMgrData->GetStationShortName(mFacilityID, mStationID, bstrInstrumentType).GetBSTR();
				mInstrumentType = bstrInstrumentType.GetBSTR();

				ComposeMeasurement(
					pRADEvent, pIGrandImport,
					pRADEvent->values[eRADStationName]);

			}  // for each event
		} // import object was properly opened

		pIGrandImport->CloseDataStore();
		pIGrandImport = NULL;
		pIFacMgrData = NULL;

	} // process events on XML list
	catch (const _com_error& comerr) 
	{
		handlecomerr(comerr);				
	}
	catch (...)
	{
	}
	StartUpDlg.FlashWindow(FALSE);
	StartUpDlg.DestroyWindow();
}