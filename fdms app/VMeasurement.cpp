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
// $History: VMeasurement.cpp $
// 
// *****************  Version 7  *****************
// User: 186846       Date: 12/19/07   Time: 2:42p
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 6  *****************
// User: Longo        Date: 3/16/05    Time: 2:21p
// Updated in $/FDMS/FDMS Application/FDMSNET
// nearing the deadlines ...
// 
// *****************  Version 5  *****************
// User: Longo        Date: 3/11/05    Time: 10:35a
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 4  *****************
// User: Longo        Date: 3/09/05    Time: 3:01p
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 3  *****************
// User: Longo        Date: 2/28/05    Time: 10:00a
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 2  *****************
// User: Longo        Date: 2/10/05    Time: 3:46p
// Updated in $/FDMS/FDMS Application/FDMSNET
// checkpoint checkin
// 
// *****************  Version 1  *****************
// User: Longo        Date: 2/07/05    Time: 10:22a
// Created in $/FDMS/FDMS Application/FDMSNET
// new measuement class, used on RAD events


#include "stdafx.h"
#include ".\FDMS.h" //REQT:  Change this to use the new application header.
#include "dconfig.h"
#include "meacol.h"
#include "impeditcol.h"
#include ".\vmeasurement.h"
#include ".\selectmeasurement.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

PTCHAR tMeasurementTypeStrings[MeasurementTypeCount] = {
	"Unspecified",
	"Cycle Verification",
	"Assembly Verification",
	"Rates Only",
};
tMeasurementType tImageToMeasurementType(LPCSTR s)
{

	if (_strnicmp(s,tMeasurementTypeStrings[0], 11) == 0)
		return UnspecifiedVerification;

	if (_strnicmp(s,tMeasurementTypeStrings[1], 5) == 0)
		return CycleVerification;

	if (_strnicmp(s,tMeasurementTypeStrings[2], 8) == 0)
		return AssemblyVerification;

//	if (strnicmp(s,tMeasurementTypeStrings[3], 5) == 0)
//		return RatesOnly;

	return UnspecifiedVerification;


}

PTCHAR tMeasurementTypeImage(tMeasurementType t)
{
	switch (t)
	{
	case UnspecifiedVerification:
	case CycleVerification:
	case AssemblyVerification:
//	case RatesOnly:
		return tMeasurementTypeStrings[t];
	}
	return "";
}


VirtualMeasurement::VirtualMeasurement(void) : 	m_bComputed(false)
{
}

VirtualMeasurement::~VirtualMeasurement(void)
{
}



static void l_szUtilCopy(char* t, LPCSTR x, const int el_maxo)
{
	int len = strlen(x);
	len = (len < el_maxo ? len : el_maxo - 1);
	memset(t, _T('\0'), el_maxo);
	strncpy(t, x, len);
	t[el_maxo - 1] = 0;
}



void VirtualMeasurement::UpdateMeasurementInfo(int iCID, LPCSTR pRawString, CString& szDisplayableResult)
{
	CMeaCol::ColumnID iColumnID = (CMeaCol::ColumnID)iCID;
	CField* pf = CSelectMeasurement::GetColScheme(iColumnID);
	bool bMsgBox = false;

	switch (iColumnID)
	{
		case CMeaCol::eItem: 
			break;
		case CMeaCol::eFacility: 
			if (pf->bSatisfiesConstraints(pRawString))
			{
				l_szUtilCopy(_szMPFacilityName, pRawString, MAX_NAME_LENGTH);
			}
			else
			{
				bMsgBox = true;
			}
			szDisplayableResult = _szMPFacilityName;
			break;
		case CMeaCol::eID:
			if (pf->bSatisfiesConstraints(pRawString))
			{
				l_szUtilCopy(_szMPItemID, pRawString, MAX_ID_LENGTH);
			}
			else
			{
				bMsgBox = true;
			}
			szDisplayableResult = _szMPItemID;
			break;
		case CMeaCol::eMeasType:
			_iMPMeasurementType = tImageToMeasurementType(pRawString);
			szDisplayableResult.Format(pf->format, _iMPMeasurementType);
			break;
		case CMeaCol::eEnrichment:
			if (pf->bSatisfiesConstraints(pRawString))
			{
				sscanf(pRawString, CSelectMeasurement::GetColScheme(iColumnID)->inputFormat, &_fMPEnrichmentPercent);
			}
			else
			{
				bMsgBox = true;
			}
			szDisplayableResult.Format(pf->format, _fMPEnrichmentPercent);
			break;
		case CMeaCol::eBurnUp:
			if (pf->bSatisfiesConstraints(pRawString))
			{
				CString sp = pRawString;
				sscanf(pRawString, CSelectMeasurement::GetColScheme(iColumnID)->inputFormat, &_fMPBurnUp);
				if (_fMPBurnUp == 0.0)
				{
					_fMPLogBurnup = DEFAULTBADVALUE;
				}
				else
				{
					_fMPLogBurnup = log10(_fMPBurnUp);
				}
			}
			else
			{
				bMsgBox = true;
			}
			szDisplayableResult.Format(pf->format, _fMPBurnUp);
			break;
		case CMeaCol::eCycle:
			if (pf->bSatisfiesConstraints(pRawString))
			{
				sscanf(pRawString, CSelectMeasurement::GetColScheme(iColumnID)->inputFormat, &_iMPCycle); 
			}
			else
			{
				bMsgBox = true;
			}
			szDisplayableResult.Format(pf->format, _iMPCycle);
			break;
		case CMeaCol::eThresholdA:
			if (pf->bSatisfiesConstraints(pRawString))
			{
				sscanf(pRawString, CSelectMeasurement::GetColScheme(iColumnID)->inputFormat, &_fMPNChanAThresh);
			}
			else
			{
				bMsgBox = true;
			}
			szDisplayableResult.Format(pf->format, _fMPNChanAThresh);		
			break;
		case CMeaCol::eThresholdB:
			if (pf->bSatisfiesConstraints(pRawString))
			{
				sscanf(pRawString, CSelectMeasurement::GetColScheme(iColumnID)->inputFormat, &_fMPNChanBThresh);
			}
			else
			{
				bMsgBox = true;
			}
			szDisplayableResult.Format(pf->format, _fMPNChanBThresh);				
			break;
		case CMeaCol::eDischDate:	
			if (pf->bSatisfiesConstraints(pRawString))
			{
				CFieldDate::XConvert3(pRawString,_iMPDischargeDateYear,	_iMPDischargeDateMonth,_iMPDischargeDateDay);
			}
			else
			{
				bMsgBox = true;
			}
			szDisplayableResult.Format(CFieldDate::DateFormat,
				_iMPDischargeDateYear,
				_iMPDischargeDateMonth,
				_iMPDischargeDateDay);		

			// recompute cooling time and log o c t
	
			CalcCoolingTime();
			ApplyAdjustments();

			break;
		case CMeaCol::eMeasDate:
			if (pf->bSatisfiesConstraints(pRawString))
			{
				CFieldDate::XConvert3(pRawString,_iMPMeasurementDateYear,	_iMPMeasurementDateMonth,_iMPMeasurementDateDay);
			}
			else
			{
				bMsgBox = true;
			}
			szDisplayableResult.Format(CFieldDate::DateFormat,
				CLAMP(_iMPMeasurementDateYear,1950,2102),
				CLAMP(_iMPMeasurementDateMonth,1,12),
				CLAMP(_iMPMeasurementDateDay,1,31));
			break;
		case CMeaCol::eCoolingTime:
			if (pf->bSatisfiesConstraints(pRawString))
			{
				sscanf(pRawString,CSelectMeasurement::GetColScheme(iColumnID)->inputFormat, &_fMPCoolingTime);
				if (_fMPCoolingTime < 0.0f)
						_fMPCoolingTime = 0.0f;
				if (_fMPCoolingTime == 0.0f)  // added 
				{
					_fMPLogCoolingTime = DEFAULTBADVALUE;
				}
				else
					_fMPLogCoolingTime =  (float)log10(_fMPCoolingTime); // 11

				UpdateDischargeDate();
			}
			else
			{
				bMsgBox = true;
			}
			szDisplayableResult.Format(pf->format, _fMPCoolingTime);				
			break;

		case CMeaCol::eNeutronA:
			if (pf->bSatisfiesConstraints(pRawString))
			{
				sscanf(pRawString, CSelectMeasurement::GetColScheme(iColumnID)->inputFormat, &_fMPNChanA);
			}
			else
			{
				bMsgBox = true;
			}
			szDisplayableResult.Format(pf->format, _fMPNChanA);				
			break;
		case CMeaCol::eNeutronB:
			if (pf->bSatisfiesConstraints(pRawString))
			{
				sscanf(pRawString, CSelectMeasurement::GetColScheme(iColumnID)->inputFormat, &_fMPNChanB);
			}
			else
			{
				bMsgBox = true;
			}
			szDisplayableResult.Format(pf->format, _fMPNChanB);				
			break;
		case CMeaCol::eNeutronC:
			if (pf->bSatisfiesConstraints(pRawString))
			{
				sscanf(pRawString, CSelectMeasurement::GetColScheme(iColumnID)->inputFormat, &_fMPNChanC);
			}
			else
			{
				bMsgBox = true;
			}
			szDisplayableResult.Format(pf->format, _fMPNChanC);						
			break;
		case CMeaCol::eGamma1:
			if (pf->bSatisfiesConstraints(pRawString))
			{
				sscanf(pRawString, CSelectMeasurement::GetColScheme(iColumnID)->inputFormat, &_fMPGDose1);
			}
			else
			{
				bMsgBox = true;
			}
			szDisplayableResult.Format(pf->format, _fMPGDose1);						
			break;
		case CMeaCol::eGamma2:
			if (pf->bSatisfiesConstraints(pRawString))
			{
				sscanf(pRawString,  CSelectMeasurement::GetColScheme(iColumnID)->inputFormat, &_fMPGDose2);
			}
			else
			{
				bMsgBox = true;
			}
			szDisplayableResult.Format( pf->format, _fMPGDose2);						
			break;
		case CMeaCol::eDetector:
			if (pf->bSatisfiesConstraints(pRawString))
			{
				l_szUtilCopy(_szMPDetectorID, pRawString, MAX_NAME_LENGTH);
			}
			else
			{
				bMsgBox = true;
			}
			szDisplayableResult = _szMPDetectorID;						
			break;
		default:
			break;
	}

	if (bMsgBox)
	{
		CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
		pApp->MyMessageBox( pf->szGetConstraintDescription(pRawString), MB_OK | MB_ICONEXCLAMATION, (UINT)-1, pf->name);
	}
	else
		MarkInfoAsEdited(iCID);
}

COleDateTime VirtualMeasurement::GetDischargeDateTime()  const
{
	COleDateTime dd(_iMPDischargeDateYear,_iMPDischargeDateMonth , _iMPDischargeDateDay,0,0,0);
	return dd;
}
void  VirtualMeasurement::UpdateDischargeDate()
{
	//if (_iMPMeasurementType == AssemblyVerification)
	{
		if (_fMPCoolingTime < 0.0f)
			_fMPCoolingTime = 0.0;

		// change discharge date to conform to the cooling time
		// cooling time is in years
		COleDateTimeSpan coolings;
		int secs = int(24.0*60.0*60.0 * 365.0 * _fMPCoolingTime);
		coolings.SetDateTimeSpan(0, 0, 0, secs);

		COleDateTime dd = _iMPMeasurementDateTime - coolings;
		_iMPDischargeDateYear = dd.GetYear();
		_iMPDischargeDateMonth = dd.GetMonth();
		_iMPDischargeDateDay = dd.GetDay();

	}
}
void  VirtualMeasurement::CalcCoolingTime()
{
	//if (_fMPCoolingTime[i] == 0.0f) jfl todo: check this, should be able to specify coolingtime directly and ignore the discharge date
	//{
	_fMPCoolingTime = 	(float)
			(_iMPMeasurementDateYear - _iMPDischargeDateYear) +
		((_iMPMeasurementDateMonth - _iMPDischargeDateMonth)/12.0f) +
		((_iMPMeasurementDateDay - _iMPDischargeDateDay)/365.0f);

	if (_fMPCoolingTime < 0.0f)
		_fMPCoolingTime = 0.0f;
	//}
	if (_fMPCoolingTime == 0.0f)
	{
		_fMPLogCoolingTime = DEFAULTBADVALUE;
	}
	else
	{
		_fMPLogCoolingTime = (float)log10(_fMPCoolingTime);
	}
}


void  VirtualMeasurement::ApplyAdjustments()
{
		switch(_iMPMeasurementType)
		{
		case CycleVerification:
			if ((_fMPNChanA == 0.0f) && (_fMPNChanB == 0.0f))
			{
				_fMPLogNoverG = DEFAULTBADVALUE;
			}
			else
			{
				_fMPLogNoverG = (float)log10((_fMPNChanA+ _fMPNChanB)/_fMPGDose1);
			}
			break;
		case AssemblyVerification:
			if ((_fMPNChanA == 0.0f) && (_fMPNChanB == 0.0f))
			{
				_fMPLogDecayCorTotNeutrons = DEFAULTBADVALUE;
			}
			else
			{
				_fMPLogDecayCorTotNeutrons = (float)
						log10((_fMPNChanA+ _fMPNChanB) * exp(0.03828* _fMPCoolingTime) );
			}
			break;
		}
		if (_fMPGDose1 == 0.0f)
		{
			_fMPLogGammaDose = DEFAULTBADVALUE;
		}
		else
		{
			_fMPLogGammaDose = (float)log10(_fMPGDose1);
		}

}

void VirtualMeasurement::SetDetectorID()
{
	char cdid[128];
	GETPROFSTR(eDETECTOR_ID,cdid,sizeof(cdid));

	l_szUtilCopy(_szMPDetectorID, cdid, MAX_NAME_LENGTH);
 
}
void VirtualMeasurement::SetBackgroundValues(double bkg[], COleDateTime bkgtime)
{
	cBkgTime = bkgtime;
	int cht;
	for (cht = NeutA; cht < MaxChannels; cht++)
	{
		_fBkgMeasurements[cht] = (float)bkg[cht];
	}
}
void VirtualMeasurement::SetBackgroundValues(float bkg[], COleDateTime bkgtime)
{
	int cht;
	for (cht = NeutA; cht < MaxChannels; cht++)
	{
		_fBkgMeasurements[cht] = bkg[cht];
	}
}
void VirtualMeasurement::ComputeMeasurement()
{
	m_bComputed = false;
	int i, cht;
	for (cht = NeutA; cht < MaxChannels; cht++)
		_dAve.val[cht] = 0.0f;

	if (_dScanRecMax > 0)
	{
		for (i = 0; i < _dScanRecMax; i++)
		{
			_dAve.val[NeutA] += _sScanRecs[i].dNeutronA;
			_dAve.val[NeutB] += _sScanRecs[i].dNeutronB;
			_dAve.val[NeutC] += _sScanRecs[i].dNeutronC;
			_dAve.val[Gamma1] += _sScanRecs[i].dGrossGammaAve1;
			_dAve.val[Gamma2] += _sScanRecs[i].dGrossGammaAve2;
		}
		for (cht = NeutA; cht < MaxChannels; cht++)
		{
			_dAve.val[cht]/= _dScanRecMax;
		}

		CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();

		if (!pApp->m_bDemoVersion) // subtract bkg if live, but not for demo version
		{
			for (cht = NeutA; cht < MaxChannels; cht++)
			{
					_dAve.val[cht] -= _fBkgMeasurements[cht];
			}
		}

		for (cht = NeutA; cht <= NeutC; cht++)
		{
			_dAve.val[cht] = (_dAve.val[cht] < 0.01f ? 0.01f : _dAve.val[cht]);
		}
		for (cht = Gamma1; cht <= Gamma2; cht++)
		{
			_dAve.val[cht] = (_dAve.val[cht] < 0.0001f ? 0.0001f : _dAve.val[cht]);
		}
	}

}
int VirtualMeasurement::CompareMeasurementDate(COleDateTime mtime)
{
	if (
(CLAMP(_iMPMeasurementDateYear,1950,2102)  == mtime.GetYear()) &&
(CLAMP(_iMPMeasurementDateMonth,1,12) == mtime.GetMonth()) &&
(CLAMP(_iMPMeasurementDateDay,1,31) == mtime.GetDay())
		)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}
int VirtualMeasurement::CompareDischargeDate(COleDateTime dtime)
{
	
	if ( (_iMPDischargeDateYear == dtime.GetYear()) &&
		 (_iMPDischargeDateMonth == dtime.GetMonth()) &&
		 (_iMPDischargeDateDay == dtime.GetDay()))
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

void VirtualMeasurement::Certify()
{
	m_bComputed = true;
}

void VirtualMeasurement::FinishMeasurement(COleDateTime odtMTime)
{

	_iMPMeasurementDateYear = odtMTime.GetYear();
	_iMPMeasurementDateMonth = odtMTime.GetMonth();
	_iMPMeasurementDateDay = odtMTime.GetDay();

	_fMPNChanA = (float)_dAve.val[NeutA];
	_fMPNChanB = (float)_dAve.val[NeutB];
	_fMPNChanC = (float)_dAve.val[NeutC];
	_fMPGDose1 = (float)_dAve.val[Gamma1];
	_fMPGDose2 = (float)_dAve.val[Gamma2];

	COleDateTimeSpan delta = odtMTime;
	delta -= delta.GetDays();

	_fMPNChanBThresh = (float)delta.GetTotalSeconds();

	CalcCoolingTime();
	ApplyAdjustments();
	//SetDetectorID();

	Certify();
}


void VirtualMeasurement::SetInfo(RADEvent* p, const CString& id, const CString& detect, const CString& facname, const COleDateTime& mEndTime)
{
	_iMPMeasurementDateTime = mEndTime;
	_iMPMeasurementDateYear = p->m_Times[RADEvent::eEndTimestamp].GetYear();
	_iMPMeasurementDateMonth = p->m_Times[RADEvent::eEndTimestamp].GetMonth();
	_iMPMeasurementDateDay = p->m_Times[RADEvent::eEndTimestamp].GetDay();

	_iMPCycle = 1; // just defaults to 1

	l_szUtilCopy(_szMPFacilityName, facname, MAX_NAME_LENGTH);
	l_szUtilCopy(_szMPDetectorID, detect, MAX_NAME_LENGTH);
	l_szUtilCopy(_szMPItemID, id, MAX_ID_LENGTH);

}


bool VirtualMeasurement::ValidContent()
{
	bool res = false;

	if (_iMPMeasurementType == CycleVerification)
	{
		res = true;
	}
	else if (_iMPMeasurementType == AssemblyVerification)
	{
		res = (_fMPEnrichmentPercent > 0.0);
		res = res && (_fMPBurnUp > 0.0);
	}

	if (res && CompareDischargeDate(ImpEditCol::m_DefaultStartDate) == 0)
	{
		res = false;
	}

	if (res && strlen(_szMPItemID) == 0)
	{
		res = false;
	}
	
	return res;

}


void VirtualMeasurement::MarkInfoAsEdited(int iCID)
{
	CMeaCol::ColumnID iColumnID = (CMeaCol::ColumnID)iCID;

	switch (iColumnID)
	{
		case CMeaCol::eID:
			m_bEAFChanged[eafId] = true;
			break;
		case CMeaCol::eMeasType:
			m_bEAFChanged[eafMeasurementType] = true;
			break;
		case CMeaCol::eEnrichment:
			m_bEAFChanged[eafEnrichment] = true;
		break;
		case CMeaCol::eBurnUp:
			m_bEAFChanged[eafBurnUp] = true;
				break;
		case CMeaCol::eCycle:
			m_bEAFChanged[eafCycle] = true;
			break;
		case CMeaCol::eDischDate:	
			m_bEAFChanged[eafDischargeDate] = true;
			break;
		case CMeaCol::eCoolingTime:
			m_bEAFChanged[eafCoolingTime] = true;
			break;
		default:
			break;
	}

}