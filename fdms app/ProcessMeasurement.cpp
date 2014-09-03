// $History: ProcessMeasurement.cpp $
// 
// *****************  Version 5  *****************
// User: Longo        Date: 3/16/05    Time: 2:21p
// Updated in $/FDMS/FDMS Application/FDMSNET
// nearing the deadlines ...
// 
// *****************  Version 4  *****************
// User: Longo        Date: 2/28/05    Time: 10:00a
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 3  *****************
// User: Longo        Date: 2/10/05    Time: 3:46p
// Updated in $/FDMS/FDMS Application/FDMSNET
// checkpoint checkin
// 
// *****************  Version 2  *****************
// User: Longo        Date: 2/07/05    Time: 10:27a
// Updated in $/FDMS/FDMS Application/FDMSNET
// checkpoint checkin
// 
// *****************  Version 1  *****************
// User: Longo        Date: 11/15/04   Time: 11:31a
// Created in $/FDMS/FDMS Application/FDMSNET
// But wait, there's more! If you are one of the first 50 callers we will
// add in tehse 4 source files, absolutely free, that's right! FREE!

#include "stdafx.h"
#include ".\FDMS.h" //REQT:  Change this to use the new application header.
#include "dconfig.h"
#include "meacol.h"
#include ".\processmeasurement.h"
#include ".\selectmeasurement.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


ProcessMeasurement::ProcessMeasurement(void)
{
}

ProcessMeasurement::~ProcessMeasurement(void)
{
}



static void l_szUtilCopy(char* t, char* x, const int el_maxo)
{
	int len = strlen(x);
	len = (len < el_maxo ? len : el_maxo - 1);
	memset(t, _T('\0'), el_maxo);
	strncpy(t, x, len);
	t[el_maxo - 1] = 0;
}


// remove the list util, replace with an array of strings
void ProcessMeasurement::AddToGlobals(RowText& Row)
{
	CString x;

	// g_iMPEntriesIn needs to be incremented, then write the data piece by piece into the g_iMP vars (see OnOk processing)
	int newEntry = g_iMPEntriesIn;

	g_iMPStatus[newEntry] = eUnmeasured; // make sure status is nothing to start with

	g_iMPItemIndices[newEntry] = newEntry;  // assign this 

	x = Row[CMeaCol::eFacility];
	l_szUtilCopy(g_szMPFacilityNames[newEntry], x.GetBuffer(), MAX_NAME_LENGTH);

	x = Row[CMeaCol::eID];
	l_szUtilCopy(g_szMPItemIDs[newEntry], x.GetBuffer(), MAX_ID_LENGTH);

	x = Row[CMeaCol::eMeasType];
	sscanf(x, CSelectMeasurement::GetColScheme(CMeaCol::eMeasType)->inputFormat, &g_iMPMeasurementType[newEntry]);

	x = Row[CMeaCol::eEnrichment];
	sscanf(x, ((CField*)CSelectMeasurement::GetColScheme(CMeaCol::eEnrichment))->inputFormat, &g_fMPEnrichmentPercent[newEntry]);

	x = Row[CMeaCol::eBurnUp];
	sscanf(x.GetBuffer(),((CField*)CSelectMeasurement::GetColScheme(CMeaCol::eBurnUp))->inputFormat, &g_fMPBurnUp[newEntry]);

	sscanf(	Row[CMeaCol::eDischDate],
		CFieldDate::DateFormat,
			&g_iMPDischargeDateYear[newEntry],
			&g_iMPDischargeDateMonth[newEntry],
			&g_iMPDischargeDateDay[newEntry]);

	x = Row[CMeaCol::eCycle];
	sscanf(x, ((CField*)CSelectMeasurement::GetColScheme(CMeaCol::eCycle))->inputFormat, &g_iMPCycle[newEntry]);

	x = Row[CMeaCol::eThresholdA];
	sscanf(x.GetBuffer(),(CSelectMeasurement::GetColScheme(CMeaCol::eThresholdA))->inputFormat, &g_fMPNChanAThresh[newEntry]);
	x = Row[CMeaCol::eThresholdB];
	sscanf(x.GetBuffer(),(CSelectMeasurement::GetColScheme(CMeaCol::eThresholdB))->inputFormat, &g_fMPNChanBThresh[newEntry]);


	sscanf(	Row[CMeaCol::eMeasDate],
		CFieldDate::DateFormat,
			&g_iMPMeasurementDateYear[newEntry],
			&g_iMPMeasurementDateMonth[newEntry],
			&g_iMPMeasurementDateDay[newEntry]);

	if (g_iMPMeasurementDateYear[newEntry] != 0 &&
		g_iMPMeasurementDateMonth[newEntry] != 0 &&
		g_iMPMeasurementDateDay[newEntry] != 0)
	{
		CLAMP(g_iMPMeasurementDateYear[newEntry],1945,2102);
		CLAMP(g_iMPMeasurementDateMonth[newEntry],1,12);
		CLAMP(g_iMPMeasurementDateDay[newEntry],1,31);
	}
	
	x = Row[CMeaCol::eCoolingTime];
	sscanf(x.GetBuffer(),CSelectMeasurement::GetColScheme(CMeaCol::eCoolingTime)->inputFormat, &g_fMPCoolingTime[newEntry]);
	if (g_fMPCoolingTime[newEntry] < 0.0f)
			g_fMPCoolingTime[newEntry] = 0.0f;
	if (g_fMPCoolingTime[newEntry] == 0.0f)  // added 
	{
		g_fMPLogCoolingTime[newEntry] = DEFAULTBADVALUE;
	}
	else
		g_fMPLogCoolingTime[newEntry] = (float)log10(g_fMPCoolingTime[newEntry]); // 11

	x = Row[CMeaCol::eNeutronA];
	sscanf(x.GetBuffer(),CSelectMeasurement::GetColScheme(CMeaCol::eNeutronA)->inputFormat, &g_fMPNChanA[newEntry]);
	x = Row[CMeaCol::eNeutronB];
	sscanf(x.GetBuffer(),CSelectMeasurement::GetColScheme(CMeaCol::eNeutronB)->inputFormat, &g_fMPNChanB[newEntry]);
	x = Row[CMeaCol::eNeutronC];
	sscanf(x.GetBuffer(),CSelectMeasurement::GetColScheme(CMeaCol::eNeutronC)->inputFormat, &g_fMPNChanC[newEntry]);

	x = Row[CMeaCol::eGamma1];
	sscanf(x.GetBuffer(), CSelectMeasurement::GetColScheme(CMeaCol::eGamma1)->inputFormat, &g_fMPGDose1[newEntry]);

	x = Row[CMeaCol::eGamma2];
	sscanf(x.GetBuffer(), CSelectMeasurement::GetColScheme(CMeaCol::eGamma2)->inputFormat, &g_fMPGDose2[newEntry]);

	x = Row[CMeaCol::eDetector];
	l_szUtilCopy(g_szMPDetectorIDs[newEntry], x.GetBuffer(), MAX_NAME_LENGTH);

	// redundant
	// x = Row[CMeaCol::eInternalIndex);
	// sscanf(x,  CSelectMeasurement::GetColScheme(CMeaCol::eInternalIndex)->inputFormat, &g_iMPItemIndices[newEntry]);


	// apparently we have successfully filled in a new entry, increment the global counter ....
	g_iMPEntriesIn++;

}


void ProcessMeasurement::UpdateGlobals(int iIndex, int iCID, PTCHAR p, CString& szDisplayableResult)
{
	CMeaCol::ColumnID iColumnID = (CMeaCol::ColumnID)iCID;
	CField* pf = CSelectMeasurement::GetColScheme(iColumnID);
	bool bMsgBox = false;

	//int iIndex = g_iMPEntriesIn;

	switch (iColumnID)
	{
		case CMeaCol::eItem: 
			break;
		case CMeaCol::eFacility: 
			if (pf->bSatisfiesConstraints(p))
			{
				l_szUtilCopy(g_szMPFacilityNames[iIndex], p, MAX_NAME_LENGTH);
			}
			else
			{
				bMsgBox = true;
			}
			szDisplayableResult = g_szMPFacilityNames[iIndex];
			break;
		case CMeaCol::eID:
			if (pf->bSatisfiesConstraints(p))
			{
				l_szUtilCopy(g_szMPItemIDs[iIndex], p, MAX_ID_LENGTH);
			}
			else
			{
				bMsgBox = true;
			}
			szDisplayableResult = g_szMPItemIDs[iIndex];
			break;
		case CMeaCol::eMeasType:
			if (pf->bSatisfiesConstraints(p))
			{
				sscanf(p, CSelectMeasurement::GetColScheme(iColumnID)->inputFormat, &g_iMPMeasurementType[iIndex]);
			}
			else
			{
				bMsgBox = true;
			}
			szDisplayableResult.Format(pf->format, g_iMPMeasurementType[iIndex]);
			break;
		case CMeaCol::eEnrichment:
			if (pf->bSatisfiesConstraints(p))
			{
				sscanf(p, CSelectMeasurement::GetColScheme(iColumnID)->inputFormat, &g_fMPEnrichmentPercent[iIndex]);
			}
			else
			{
				bMsgBox = true;
			}
			szDisplayableResult.Format(pf->format, g_fMPEnrichmentPercent[iIndex]);
			break;
		case CMeaCol::eBurnUp:
			if (pf->bSatisfiesConstraints(p))
			{
				CString sp = p;
				sscanf(p, CSelectMeasurement::GetColScheme(iColumnID)->inputFormat, &g_fMPBurnUp[iIndex]);
				if (g_fMPBurnUp[iIndex] == 0.0)
				{
					g_fMPLogBurnup[iIndex] = DEFAULTBADVALUE;
				}
				else
				{
					g_fMPLogBurnup[iIndex] = log10(g_fMPBurnUp[iIndex]);
				}
			}
			else
			{
				bMsgBox = true;
			}
			szDisplayableResult.Format(pf->format, g_fMPBurnUp[iIndex]);
			break;
		case CMeaCol::eCycle:
			if (pf->bSatisfiesConstraints(p))
			{
				sscanf(p, CSelectMeasurement::GetColScheme(iColumnID)->inputFormat, &g_iMPCycle[iIndex]); 
			}
			else
			{
				bMsgBox = true;
			}
			szDisplayableResult.Format(pf->format, g_iMPCycle[iIndex]);
			break;
		case CMeaCol::eThresholdA:
			if (pf->bSatisfiesConstraints(p))
			{
				sscanf(p, CSelectMeasurement::GetColScheme(iColumnID)->inputFormat, &g_fMPNChanAThresh[iIndex]);
			}
			else
			{
				bMsgBox = true;
			}
			szDisplayableResult.Format(pf->format, g_fMPNChanAThresh[iIndex]);		
			break;
		case CMeaCol::eThresholdB:
			if (pf->bSatisfiesConstraints(p))
			{
				sscanf(p, CSelectMeasurement::GetColScheme(iColumnID)->inputFormat, &g_fMPNChanBThresh[iIndex]);
			}
			else
			{
				bMsgBox = true;
			}
			szDisplayableResult.Format(pf->format, g_fMPNChanBThresh[iIndex]);				
			break;
		case CMeaCol::eDischDate:	
			if (pf->bSatisfiesConstraints(p))
			{
				CFieldDate::XConvert3(p,g_iMPDischargeDateYear[iIndex],	g_iMPDischargeDateMonth[iIndex],g_iMPDischargeDateDay[iIndex]);
			}
			else
			{
				bMsgBox = true;
			}
			szDisplayableResult.Format(CFieldDate::DateFormat,
				g_iMPDischargeDateYear[iIndex],
				g_iMPDischargeDateMonth[iIndex],
				g_iMPDischargeDateDay[iIndex]);		

			// recompute cooling time and log o c t
	
			CalcCoolingTime(iIndex);
			ApplyAdjustments(iIndex);

			{
				// chained causation, a display of cooling time update
				CString x;
				x.Format(pf->format, g_fMPCoolingTime[iIndex]);
				// jfl m_cListCtrl.SetItemText(iEntry, CMeaCol::eCoolingTime, x);
			}

			break;
		case CMeaCol::eMeasDate:
			if (pf->bSatisfiesConstraints(p))
			{
				CFieldDate::XConvert3(p,g_iMPMeasurementDateYear[iIndex],	g_iMPMeasurementDateMonth[iIndex],g_iMPMeasurementDateDay[iIndex]);
			}
			else
			{
				bMsgBox = true;
			}
			szDisplayableResult.Format(CFieldDate::DateFormat,
				CLAMP(g_iMPMeasurementDateYear[iIndex],1950,2102),
				CLAMP(g_iMPMeasurementDateMonth[iIndex],1,12),
				CLAMP(g_iMPMeasurementDateDay[iIndex],1,31));
			break;
		case CMeaCol::eCoolingTime:
			if (pf->bSatisfiesConstraints(p))
			{
				sscanf(p,CSelectMeasurement::GetColScheme(iColumnID)->inputFormat, &g_fMPCoolingTime[iIndex]);
				if (g_fMPCoolingTime[iIndex] < 0.0f)
						g_fMPCoolingTime[iIndex] = 0.0f;
				if (g_fMPCoolingTime[iIndex] == 0.0f)  // added 
				{
					g_fMPLogCoolingTime[iIndex] = DEFAULTBADVALUE;
				}
				else
					g_fMPLogCoolingTime[iIndex] =  (float)log10(g_fMPCoolingTime[iIndex]); // 11
			}
			else
			{
				bMsgBox = true;
			}
			szDisplayableResult.Format(pf->format, g_fMPCoolingTime[iIndex]);				
			break;

		case CMeaCol::eNeutronA:
			if (pf->bSatisfiesConstraints(p))
			{
				sscanf(p, CSelectMeasurement::GetColScheme(iColumnID)->inputFormat, &g_fMPNChanA[iIndex]);
			}
			else
			{
				bMsgBox = true;
			}
			szDisplayableResult.Format(pf->format, g_fMPNChanA[iIndex]);				
			break;
		case CMeaCol::eNeutronB:
			if (pf->bSatisfiesConstraints(p))
			{
				sscanf(p, CSelectMeasurement::GetColScheme(iColumnID)->inputFormat, &g_fMPNChanB[iIndex]);
			}
			else
			{
				bMsgBox = true;
			}
			szDisplayableResult.Format(pf->format, g_fMPNChanB[iIndex]);				
			break;
		case CMeaCol::eNeutronC:
			if (pf->bSatisfiesConstraints(p))
			{
				sscanf(p, CSelectMeasurement::GetColScheme(iColumnID)->inputFormat, &g_fMPNChanC[iIndex]);
			}
			else
			{
				bMsgBox = true;
			}
			szDisplayableResult.Format(pf->format, g_fMPNChanC[iIndex]);						
			break;
		case CMeaCol::eGamma1:
			if (pf->bSatisfiesConstraints(p))
			{
				sscanf(p, CSelectMeasurement::GetColScheme(iColumnID)->inputFormat, &g_fMPGDose1[iIndex]);
			}
			else
			{
				bMsgBox = true;
			}
			szDisplayableResult.Format(pf->format, g_fMPGDose1[iIndex]);						
			break;
		case CMeaCol::eGamma2:
			if (pf->bSatisfiesConstraints(p))
			{
				sscanf(p,  CSelectMeasurement::GetColScheme(iColumnID)->inputFormat, &g_fMPGDose2[iIndex]);
			}
			else
			{
				bMsgBox = true;
			}
			szDisplayableResult.Format( pf->format, g_fMPGDose2[iIndex]);						
			break;
		case CMeaCol::eDetector:
			if (pf->bSatisfiesConstraints(p))
			{
				l_szUtilCopy(g_szMPDetectorIDs[iIndex], p, MAX_NAME_LENGTH);
			}
			else
			{
				bMsgBox = true;
			}
			szDisplayableResult = g_szMPDetectorIDs[iIndex];						
			break;
		default:
			break;
	}

	if (bMsgBox)
	{
		CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
		pApp->MyMessageBox( pf->szGetConstraintDescription(p), MB_OK | MB_ICONEXCLAMATION, (UINT)-1, pf->name);
	}
}

void  ProcessMeasurement::CalcCoolingTime(int i)
{
	//if (g_fMPCoolingTime[i] == 0.0f)  jfl todo: check this, should be able to specify coolingtime directly and ignore the d dat
	//{
		g_fMPCoolingTime[i] = 	(float)
			 (g_iMPMeasurementDateYear[i] - g_iMPDischargeDateYear[i]) +
			((g_iMPMeasurementDateMonth[i] - g_iMPDischargeDateMonth[i])/12.0f) +
			((g_iMPMeasurementDateDay[i] - g_iMPDischargeDateDay[i])/365.0f);

		if (g_fMPCoolingTime[i] < 0.0f)
			g_fMPCoolingTime[i] = 0.0f;
	//}
	if (g_fMPCoolingTime[i] == 0.0f)
	{
		g_fMPLogCoolingTime[i] = DEFAULTBADVALUE;
	}
	else
	{
		g_fMPLogCoolingTime[i] = (float)log10(g_fMPCoolingTime[i]);
	}
}


void  ProcessMeasurement::ApplyAdjustments(int i)
{
		switch(g_iMPMeasurementType[i])
		{
		case CycleVerification:
			if ((g_fMPNChanA[i] == 0.0f) && (g_fMPNChanB[i] == 0.0f))
			{
				g_fMPLogNoverG[i] = DEFAULTBADVALUE;
			}
			else
			{
				g_fMPLogNoverG[i] = (float)log10((g_fMPNChanA[i]+ g_fMPNChanB[i])/g_fMPGDose1[i]);
			}
			break;
		case AssemblyVerification:
			if ((g_fMPNChanA[i] == 0.0f) && (g_fMPNChanB[i] == 0.0f))
			{
				g_fMPLogDecayCorTotNeutrons[i] = DEFAULTBADVALUE;
			}
			else
			{
				g_fMPLogDecayCorTotNeutrons[i] = (float)
						log10((g_fMPNChanA[i]+ g_fMPNChanB[i]) * exp(0.03828* g_fMPCoolingTime[i]) );
			}
			break;
		}
		if (g_fMPGDose1[i] == 0.0f)
		{
			g_fMPLogGammaDose[i] = DEFAULTBADVALUE;
		}
		else
		{
			g_fMPLogGammaDose[i] = (float)log10(g_fMPGDose1[i]);
		}

}

void ProcessMeasurement::SetDetectorID(int i)
{
	char cdid[128];
	GETPROFSTR(eDETECTOR_ID,cdid,sizeof(cdid));

	l_szUtilCopy(g_szMPDetectorIDs[i], cdid, MAX_NAME_LENGTH);
 
}

extern AnalysisRec g_sScanRecs[MAXIMUM_CYCLES_AUTHORIZED];  // change this to a class that guards overwriting, or use a dynamic array, something to use as a guard
extern int		g_dScanRecMax;


void ProcessMeasurement::ComputeMeasurement(ChannelCounts& dAve)
{
	int i, cht;
	for (cht = NeutA; cht < MaxChannels; cht++)
		dAve.val[cht] = 0.0f;

	if (g_dScanRecMax > 0)
	{
		for (i = 0; i < g_dScanRecMax; i++)
		{
			dAve.val[NeutA] += g_sScanRecs[i].dNeutronA;
			dAve.val[NeutB] += g_sScanRecs[i].dNeutronB;
			dAve.val[NeutC] += g_sScanRecs[i].dNeutronC;
			dAve.val[Gamma1] += g_sScanRecs[i].dGrossGammaAve1;
			dAve.val[Gamma2] += g_sScanRecs[i].dGrossGammaAve2;
		}
		for (cht = NeutA; cht < MaxChannels; cht++)
		{
			dAve.val[cht]/= g_dScanRecMax;
		}

		CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();

		if (!pApp->m_bDemoVersion) // subtract bkg if live, but not for demo version
		{
			for (cht = NeutA; cht < MaxChannels; cht++)
			{
					dAve.val[cht] -= g_fBkgMeasurements[cht];
			}
		}

		for (cht = NeutA; cht <= NeutC; cht++)
		{
			dAve.val[cht] = (dAve.val[cht] < 0.01f ? 0.01f : dAve.val[cht]);
		}
		for (cht = Gamma1; cht <= Gamma2; cht++)
		{
			dAve.val[cht] = (dAve.val[cht] < 0.0001f ? 0.0001f : dAve.val[cht]);
		}
	}

}

void ProcessMeasurement::AddMeasurement(const ChannelCounts& dAve, int iIndex, COleDateTime odtMTime)
{

	g_iMPMeasurementDateYear[iIndex] = odtMTime.GetYear();
	g_iMPMeasurementDateMonth[iIndex] = odtMTime.GetMonth();
	g_iMPMeasurementDateDay[iIndex] = odtMTime.GetDay();


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
	g_iMPIndexMostRecentMeasurement = iIndex+1;
	//set the status of the new measurement
	g_iMPStatus[iIndex] = eMostRecent;
	g_bPlotMostRecent = true;

	g_fMPNChanA[iIndex] = (float)dAve.val[NeutA];
	g_fMPNChanB[iIndex] = (float)dAve.val[NeutB];
	g_fMPNChanC[iIndex] = (float)dAve.val[NeutC];
	g_fMPGDose1[iIndex] = (float)dAve.val[Gamma1];
	g_fMPGDose2[iIndex] = (float)dAve.val[Gamma2];

	ProcessMeasurement::CalcCoolingTime(iIndex);
	ProcessMeasurement::ApplyAdjustments(iIndex);
	ProcessMeasurement::SetDetectorID(iIndex);

}

