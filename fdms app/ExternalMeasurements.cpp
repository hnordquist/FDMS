//////////////////////////////////////////////////////////////////////////
// Copyright 2005, The Regents Of the University of California.  This
// software was produced under U.S. Government contract (W-7405-ENG-36) at
// the Los Alamos National Laboratory by the Safeguards Science and
// Technology Group (N-1).  All Rights Reserved.
//////////////////////////////////////////////////////////////////////////
// $History: ExternalMeasurements.cpp $
// 
// *****************  Version 13  *****************
// User: 186846       Date: 4/04/08    Time: 6:02p
// Updated in $/FDMS/FDMSNET
// 
// *****************  Version 12  *****************
// User: 186846       Date: 4/01/08    Time: 3:40p
// Updated in $/FDMS/FDMSNET
// Mar. 29, fix for 398, partial for 447
// 
// *****************  Version 11  *****************
// User: 186846       Date: 1/29/08    Time: 5:46p
// Updated in $/FDMS/FDMS Application/FDMSNET
// tweakings
// 
// *****************  Version 10  *****************
// User: Longo        Date: 3/17/05    Time: 9:32a
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 9  *****************
// User: Longo        Date: 3/16/05    Time: 2:21p
// Updated in $/FDMS/FDMS Application/FDMSNET
// nearing the deadlines ...
// 
// *****************  Version 8  *****************
// User: Longo        Date: 2/28/05    Time: 11:34a
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
// User: Longo        Date: 1/07/05    Time: 2:25p
// Updated in $/FDMS/FDMS Application/FDMSNET
// adding the secondary import editor dilaog
// 
// *****************  Version 3  *****************
// User: Longo        Date: 1/05/05    Time: 5:28p
// Updated in $/FDMS/FDMS Application/FDMSNET
// init data
// 
// *****************  Version 2  *****************
// User: Longo        Date: 11/24/04   Time: 3:48p
// Updated in $/FDMS/FDMS Application/FDMSNET
// * bring up select measurement dialog upon requests from UL
// * added matching class with logic: uses 182 day window to seek for
// match
// * compiler warning removal
// 
// *****************  Version 1  *****************
// User: Longo        Date: 11/15/04   Time: 11:31a
// Created in $/FDMS/FDMS Application/FDMSNET
// But wait, there's more! If you are one of the first 50 callers we will
// add in tehse 4 source files, absolutely free, that's right! FREE!

#include "stdafx.h"
#include "FDMS.h"
#include "DoAssay.h"

#include ".\externalmeasurements.h"
#include ".\selectmeasurement.h"
#include ".\VMeasurement.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



ExternalMeasurements::ExternalMeasurements(void) : cycles(0), 	sFacilityID(0),	lStationID(0),	StartTime(0),
	EndTime(0), m_tAssayType(eBackgroundAssay), m_bRM(false)

{
	for (int _i = eisMP; _i < eisNum; _i++)
	{
		tMatchIdentitySource s = (tMatchIdentitySource)_i;
		m_iGIndex[s] = -1;
		m_bExactMatch[s] = false;
	}
	for (int i = NeutA; i < MaxChannels; i++)
	{
		pdData[i] = NULL;
	}

}
ExternalMeasurements::ExternalMeasurements(ULONG ulCycles) : cycles(ulCycles),  sFacilityID(0),	lStationID(0),	StartTime(0),
	EndTime(0),  m_tAssayType(eBackgroundAssay), m_bRM(false)
{
	for (int _i = eisMP; _i < eisNum; _i++)
	{
		tMatchIdentitySource s = (tMatchIdentitySource)_i;
		m_iGIndex[s] = -1;
		m_bExactMatch[s] = false;
	}
	for (int i = NeutA; i < MaxChannels; i++)
	{
		pdData[i] = new double[ulCycles];
		for (ULONG j = 0; j < ulCycles; j++)
			pdData[i][j] = 0.0;
	}

}

ExternalMeasurements::ExternalMeasurements(
		ULONG ulCycles, short FacilityID,	long StationID,	DATE Start,	DATE End,
		CString szFacilityLongName, CString szFacilityShortName,
		CString szStationLongName, CString szStationShortName,
		CString szInstrumentType)
	: cycles(ulCycles),  sFacilityID(FacilityID),	lStationID(StationID),	StartTime(Start),	EndTime(End),
		FacilityLongName(szFacilityLongName),
		FacilityShortName(szFacilityShortName),
		StationLongName(szStationLongName),
		StationShortName(szStationShortName),
		InstrumentType(szInstrumentType),
		m_tAssayType(eBackgroundAssay), m_bRM(false)
{

	for (int _i = eisMP; _i < eisNum; _i++)
	{
		tMatchIdentitySource s = (tMatchIdentitySource)_i;
		m_iGIndex[s] = -1;
		m_bExactMatch[s] = false;
	}
	for (int i = NeutA; i < MaxChannels; i++)
	{
		pdData[i] = new double[ulCycles];
		for (ULONG j = 0; j < ulCycles; j++)
			pdData[i][j] = 0.0;
	}

}

	
ExternalMeasurements::~ExternalMeasurements(void)
{
	for (int i = NeutA; i < MaxChannels; i++)
	{
		if (pdData[i])
			delete [] pdData[i];
	}
}
// rad uses these strings (Unknown, Channel, Measurement or Background)
void ExternalMeasurements::SetAssayType(const CString& s)
{
	if (s.CompareNoCase("Channel") == 0)
		m_tAssayType = eNormalAssay;
	else if (s.CompareNoCase("Background") == 0)
		m_tAssayType = eBackgroundAssay;
	//else if (s.CompareNoCase("Measurement") == 0)
	//	m_tAssayType = eNormalAssay;
	else 
		m_tAssayType = eUnknownAssay;
}

// rad uses these strings (Unknown, Channel, Measurement or Background)
LPCTSTR ExternalMeasurements::GetAssayTypeString() const
{
	if (m_tAssayType == eNormalAssay)
		return "Channel";
	else if (m_tAssayType == eBackgroundAssay)
		return "Background";
	else
		return "Unknown";
}
tAssayTypes ExternalMeasurements::GetAssayType() const
{
	return m_tAssayType;
}

void ExternalMeasurements::ComputeBackgroundMeasurement()
{
	m_vr.SetBackgroundValues(m_vr._dAve.val, this->EndTime);
}

void ExternalMeasurements::ComputeMeasurement()
{

	// accumulate the results of the measurement cycles
	ULONG i;
	for (i = 0; i < cycles && i < MAXIMUM_CYCLES_AUTHORIZED;i++)
	{
		m_vr._sScanRecs[i].dGrossGammaAve1	= pdData[Gamma1][i];
		m_vr._sScanRecs[i].dGrossGammaAve2	= pdData[Gamma2][i];
		m_vr._sScanRecs[i].dNeutronA	= pdData[NeutA][i];
		m_vr._sScanRecs[i].dNeutronB	= pdData[NeutB][i];
		m_vr._sScanRecs[i].dNeutronC	= pdData[NeutC][i];
	}

	m_vr._dScanRecMax = i;

	//average across all of the measurements
	m_vr.ComputeMeasurement();

	// compute relevant values for the measurement
	m_vr.FinishMeasurement(this->EndTime);
}




static void l_szUtilCopy(char* t, char* x, const int el_maxo)
{
	int len = strlen(x);
	len = (len < el_maxo ? len : el_maxo - 1);
	memset(t, _T('\0'), el_maxo);
	strncpy(t, x, len);
	t[el_maxo - 1] = 0;
}

void ExternalMeasurements::CopyFromExistingGlobals(int iIndex)
{

	// these were computed in 'ApplyAdjustments'
	m_vr._fMPLogNoverG = g_fMPLogNoverG[iIndex];
	g_fMPLogDecayCorTotNeutrons[iIndex] = m_vr._fMPLogDecayCorTotNeutrons;
	g_fMPLogGammaDose[iIndex] = m_vr._fMPLogGammaDose;

	m_vr._iMPItemIndex = g_iMPItemIndices[iIndex];
	l_szUtilCopy(m_vr._szMPFacilityName, g_szMPFacilityNames[iIndex], MAX_NAME_LENGTH);
	l_szUtilCopy( m_vr._szMPItemID,g_szMPItemIDs[iIndex], MAX_ID_LENGTH);
	m_vr._iMPMeasurementType = g_iMPMeasurementType[iIndex] ;
	m_vr._fMPEnrichmentPercent = g_fMPEnrichmentPercent[iIndex];
	m_vr._fMPBurnUp = g_fMPBurnUp[iIndex];
	m_vr._fMPLogBurnup = g_fMPLogBurnup[iIndex];
	m_vr._iMPCycle = g_iMPCycle[iIndex];
	m_vr._fMPNChanAThresh = g_fMPNChanAThresh[iIndex];	
	m_vr._fMPNChanBThresh = g_fMPNChanBThresh[iIndex];		
	m_vr._iMPDischargeDateYear = g_iMPDischargeDateYear[iIndex];
	m_vr._iMPDischargeDateMonth = g_iMPDischargeDateMonth[iIndex];
	m_vr._iMPDischargeDateDay = g_iMPDischargeDateDay[iIndex];
	l_szUtilCopy(m_vr._szMPDetectorID, g_szMPDetectorIDs[iIndex], MAX_NAME_LENGTH);				
}

void ExternalMeasurements::UpdateMPGlobal(int iIndex, int iCID)
{
	CMeaCol::ColumnID iColumnID = (CMeaCol::ColumnID)iCID;

	switch (iColumnID)
	{
		case CMeaCol::eItem: 
			// these were computed in 'ApplyAdjustments'
			g_fMPLogNoverG[iIndex] = m_vr._fMPLogNoverG;
			g_fMPLogDecayCorTotNeutrons[iIndex] = m_vr._fMPLogDecayCorTotNeutrons;
			g_fMPLogGammaDose[iIndex] = m_vr._fMPLogGammaDose;

			g_iMPItemIndices[iIndex] = iIndex;  /// jfl might need this later in a row update operation?
			break;
		case CMeaCol::eFacility: 
			l_szUtilCopy(g_szMPFacilityNames[iIndex],  m_vr._szMPFacilityName, MAX_NAME_LENGTH);
			break;
		case CMeaCol::eID:
			l_szUtilCopy(g_szMPItemIDs[iIndex], m_vr._szMPItemID, MAX_ID_LENGTH);
			break;
		case CMeaCol::eMeasType:
			g_iMPMeasurementType[iIndex] = m_vr._iMPMeasurementType;
			break;
		case CMeaCol::eEnrichment:
			g_fMPEnrichmentPercent[iIndex] = m_vr._fMPEnrichmentPercent;
			break;
		case CMeaCol::eBurnUp:
			g_fMPBurnUp[iIndex] = m_vr._fMPBurnUp;
			g_fMPLogBurnup[iIndex] = m_vr._fMPLogBurnup;
			break;
		case CMeaCol::eCycle:
			g_iMPCycle[iIndex] = m_vr._iMPCycle;
			break;
		case CMeaCol::eThresholdA:
			g_fMPNChanAThresh[iIndex] = m_vr._fMPNChanAThresh;	
			break;
		case CMeaCol::eThresholdB:
			g_fMPNChanBThresh[iIndex] = m_vr._fMPNChanBThresh;		
			break;
		case CMeaCol::eDischDate:	
			g_iMPDischargeDateYear[iIndex] =	m_vr._iMPDischargeDateYear;
			g_iMPDischargeDateMonth[iIndex] = m_vr._iMPDischargeDateMonth;
			g_iMPDischargeDateDay[iIndex] = m_vr._iMPDischargeDateDay;
			break;
		case CMeaCol::eMeasDate:
			g_iMPMeasurementDateYear[iIndex] = m_vr._iMPMeasurementDateYear;
			g_iMPMeasurementDateMonth[iIndex] = m_vr._iMPMeasurementDateMonth;
			g_iMPMeasurementDateDay[iIndex] = m_vr._iMPMeasurementDateDay;
			break;
		case CMeaCol::eCoolingTime:
			g_fMPCoolingTime[iIndex] = m_vr._fMPCoolingTime;
			g_fMPLogCoolingTime[iIndex] =  m_vr._fMPLogCoolingTime;
			break;
		case CMeaCol::eNeutronA:
			g_fMPNChanA[iIndex] = m_vr._fMPNChanA;
			break;
		case CMeaCol::eNeutronB:
			g_fMPNChanB[iIndex] = m_vr._fMPNChanB;
			break;
		case CMeaCol::eNeutronC:
			g_fMPNChanC[iIndex] = m_vr._fMPNChanC;
			break;
		case CMeaCol::eGamma1:
			g_fMPGDose1[iIndex] =  m_vr._fMPGDose1;
			break;
		case CMeaCol::eGamma2:
			g_fMPGDose2[iIndex] =  m_vr._fMPGDose2;
			break;
		case CMeaCol::eDetector:
			l_szUtilCopy(g_szMPDetectorIDs[iIndex], m_vr._szMPDetectorID, MAX_NAME_LENGTH);				
			break;
		default:
			break;
	}
}

void ExternalMeasurements::UpdateDBGlobal(int iIndex, int iCID)
{
	CMeaCol::ColumnID iColumnID = (CMeaCol::ColumnID)iCID;

	switch (iColumnID)
	{
		case CMeaCol::eItem: 
			// these were computed in 'ApplyAdjustments'
			g_fLogNoverG[iIndex] = m_vr._fMPLogNoverG;
			g_fLogDecayCorTotNeutrons[iIndex] = m_vr._fMPLogDecayCorTotNeutrons;
			g_fLogGammaDose[iIndex] = m_vr._fMPLogGammaDose;

//			g_iItemIndices[iIndex] = iIndex;  /// jfl might need this later in a row update operation?
			break;
		case CMeaCol::eFacility: 
			l_szUtilCopy(g_szFacilityNames[iIndex],  m_vr._szMPFacilityName, MAX_NAME_LENGTH);
			break;
		case CMeaCol::eID:
			l_szUtilCopy(g_szItemIDs[iIndex], m_vr._szMPItemID, MAX_ID_LENGTH);
			break;
		case CMeaCol::eMeasType:
			g_iMeasurementType[iIndex] = m_vr._iMPMeasurementType;
			break;
		case CMeaCol::eEnrichment:
			g_fEnrichmentLevel[iIndex] = m_vr._fMPEnrichmentPercent;
			break;
		case CMeaCol::eBurnUp:
			g_fBurnUp[iIndex] = m_vr._fMPBurnUp;
			g_fLogBurnup[iIndex] = m_vr._fMPLogBurnup;
			break;
		case CMeaCol::eCycle:
			g_iCycle[iIndex] = m_vr._iMPCycle;
			break;
		case CMeaCol::eThresholdA:
			g_fNChanAThresh[iIndex] = m_vr._fMPNChanAThresh;	
			break;
		case CMeaCol::eThresholdB:
			g_fNChanBThresh[iIndex] = m_vr._fMPNChanBThresh;		
			break;
		case CMeaCol::eDischDate:	
			g_iDischargeDateYear[iIndex] =	m_vr._iMPDischargeDateYear;
			g_iDischargeDateMonth[iIndex] = m_vr._iMPDischargeDateMonth;
			g_iDischargeDateDay[iIndex] = m_vr._iMPDischargeDateDay;
			break;
		case CMeaCol::eMeasDate:
			g_iMeasurementDateYear[iIndex] = m_vr._iMPMeasurementDateYear;
			g_iMeasurementDateMonth[iIndex] = m_vr._iMPMeasurementDateMonth;
			g_iMeasurementDateDay[iIndex] = m_vr._iMPMeasurementDateDay;
			break;
		case CMeaCol::eCoolingTime:
			g_fCoolingTime[iIndex] = m_vr._fMPCoolingTime;
			g_fLogCoolingTime[iIndex] =  m_vr._fMPLogCoolingTime;
			break;
		case CMeaCol::eNeutronA:
			g_fNChanA[iIndex] = m_vr._fMPNChanA;
			break;
		case CMeaCol::eNeutronB:
			g_fNChanB[iIndex] = m_vr._fMPNChanB;
			break;
		case CMeaCol::eNeutronC:
			g_fNChanC[iIndex] = m_vr._fMPNChanC;
			break;
		case CMeaCol::eGamma1:
			g_fGDose1[iIndex] =  m_vr._fMPGDose1;
			break;
		case CMeaCol::eGamma2:
			g_fGDose2[iIndex] =  m_vr._fMPGDose2;
			break;
		case CMeaCol::eDetector:
			// not used in DB l_szUtilCopy(g_szDetectorIDs[iIndex], m_vr._szMPDetectorID, MAX_NAME_LENGTH);				
			break;
		default:
			break;
	}
}
int ExternalMeasurements::StartNewDeclaration() 
{
	m_vr._iMPEntryIndex = g_iMPEntriesIn;
	return 0;
}

#include ".\impeditcol.h"

void ExternalMeasurements::PopulateGlobals()
{
	for (int j = (int)ImpEditCol::eCheck; j < ImpEditCol::eColumnCount; j++)
	{
		if (!(ImpEditCol::m_szFieldData[j].bContent))
			continue;

    	// get the internal index
		UpdateMPGlobal(m_vr._iMPEntryIndex,ImpEditCol::m_meacolmap[j]);
	 }

	// nothing in the DB, add it
	if ((m_iGIndex[eisDB] < 0) && (!m_bExactMatch[eisDB]))
	{
		for (int j = (int)ImpEditCol::eCheck; j < ImpEditCol::eColumnCount; j++)
		{
			if (!(ImpEditCol::m_szFieldData[j].bContent))
				continue;
			UpdateDBGlobal(g_iEntriesInDB,ImpEditCol::m_meacolmap[j]);
		}
		g_iEntriesInDB++;
	}

}

void ExternalMeasurements::AgeStatus(const int iLastIndex)
{

	//age the state of all of the measurement plan
	for (int i = 0; i < g_iMPEntriesIn; i++)
	{
		if (//g_iMPStatus[i] == eMostRecent ||
			g_iMPStatus[i] == eMostRecentNoFits ||
			g_iMPStatus[i] == eMostRecentGood ||
			g_iMPStatus[i] == eMostRecentSuspect)
		{
			g_iMPStatus[i] += 3;
		}
	}
	//get ready to tell the fortran dll
	//add the "1" 'cause fortran dll will use this
	//as an index and it is "1" based not "0" based
	//like the rest of the world
	g_iMPIndexMostRecentMeasurement = iLastIndex+1;
	//set the status of the new measurement
	g_iMPStatus[iLastIndex] = eMostRecent;
	g_bPlotMostRecent = true;

	//call the fortran dll
	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
	pApp->UpdateGraphData();
 
	//now refresh all of the graphs
	pApp->UpdateAllDocs();

	g_iMPIndexMostRecentMeasurement = iLastIndex+1;
}

void ExternalMeasurements::PersistChanges(const bool bDBSave, CArray<int, int&>& measurementIndexes)
{
	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();

	//if ((m_iGIndex[eisDB] < 0) && (!m_bExactMatch[eisDB]))
	if (bDBSave)
	{
		pApp->WriteDatabase(); // write updated data to DB file now, not later
	}

	pApp->WriteMeasurementPlan(); // write updated data to plan file now, not later

	for (int i=0;i <= measurementIndexes.GetUpperBound();i++)
	{
		int mi = measurementIndexes[i];
		// save the individual measurement event to the log file
		if (mi >= 0)
			pApp->UpdateMeasurements(mi+1); //bump the index for the fortran array indexing!
	}

}

bool ExternalMeasurements::AddMeasurement()  // updating an existing mp entry
{
	ASSERT(m_vr._iMPEntryIndex >= 0 && m_vr._iMPEntryIndex < g_iMPEntriesIn);

	PopulateGlobals();
	//AgeStatus();
	//PersistChanges();
	return true;
}

bool ExternalMeasurements::CreateNewMeasurement()
{

	/// all of the required values exist as text in the list cells and as typed entities on PROCESSDATA struct fields
	// formatting and parsing has already occurred
	// soooo ....
	// I can copy these values directly into the global data structures from the PROCESSDATA

	// but any that were not changed have not been updated, so we need an init pass on the PROCESSDATA that sets every field with the same value that was stuck into PROCESSDATA
	// g_MPEntriesIn is the next empty slot in the global arrays, we bump it after we copy the data

	// do this by calling 
    //		UpdateGlobal(int iIndex, int iCID)
	// per column item
	
	ASSERT(m_vr._iMPEntryIndex >= 0);

	PopulateGlobals();
	
	// globals have been updated, but global states have NOT been recomputed yet,
	// we wait for matching declarations to be processed and only after both sets of global data have been processed then do we do the full state update and write the files

	// this is an accumulation step only!
	// it might be that we do not want to update the globals until after the matching dialog processing has been accumulated? consider this as a change

	// add the measurement results now, this writes the data to the external csv files

	// this global state modification is also supposed to happen during a live measurement:

	// here we apparently we have successfully filled in a new MP entry, increment the global counter ....
	g_iMPEntriesIn++;

	//AgeStatus();
	//PersistChanges();

	return true;
}


CString ExternalMeasurements::Image()
{
	CString s;
	COleDateTime t(EndTime);
	s.Format("%.16s %.24s %.10s :%s,%s", m_vr._szMPItemID,t.Format(), GetAssayTypeString(), StationShortName, FacilityShortName);
	return s;
}
