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
#pragma once

#include "..\AppUL\radeventlist.h"

typedef enum
//	eafMeasurementType = ImpEditCol::eMeasType,
//	eafEnrichment = ImpEditCol::eEnrichment,
//	eafBurnUp = ImpEditCol::eBurnUp,
//	eafDischargeDate = ImpEditCol::eDischDate,
//	eafCycle = ImpEditCol::eCycle
//	eafCycle = ImpEditCol::eCoolingTime
{
	eafId,
	eafMeasurementType,
	eafEnrichment,
	eafBurnUp,
	eafDischargeDate,
	eafCycle,
	eafCoolingTime,
	eafEnumCount
} tAdvisedEditableFields;

struct PROCESSDATA
{
	PROCESSDATA()
	{
		_iMPEntryIndex = -1; 

		_iMPStatus = 0;
		_iMPItemIndex = 0;
		_szMPFacilityName[0] = 0;
		_szMPItemID[0] = 0;
		_iMPMeasurementType = 2; //2 default to Assembly Verification, see tMeasurementType, see DR 399
		_fMPEnrichmentPercent= 0.0;

		_fMPBurnUp= 0.0;
		_iMPDischargeDateYear = 1950;
		_iMPDischargeDateMonth = 1;
		_iMPDischargeDateDay= 1;
		_iMPCycle= 0;

		_fMPNChanAThresh= 0.0;
		_fMPNChanBThresh= 0.0;

		_iMPMeasurementDateYear= 0;
		_iMPMeasurementDateMonth= 0;
		_iMPMeasurementDateDay= 0;

		_fMPCoolingTime = 0.0;
		_fMPLogCoolingTime= 0.0;
		_fMPNChanA= 0.0;
		_fMPNChanB= 0.0;
		_fMPNChanC= 0.0;

		_fMPGDose1= 0.0;
		_fMPGDose2= 0.0;
		_szMPDetectorID[0] = 0;
	
		_fMPLogBurnup = 0.0;
		_fMPLogDecayCorTotNeutrons = 0.0;
		_fMPLogNoverG = 0.0;

		//AnalysisRec _sScanRecs[MAXIMUM_CYCLES_AUTHORIZED];
		_dScanRecMax = 0;
		for (int cht = NeutA; cht < MaxChannels; cht++)
		{
			_fBkgMeasurements[cht] = 0.0;
		}

		for (int eaf = eafMeasurementType; eaf < eafEnumCount; eaf++)
		{
			m_bEAFChanged[eaf] = false;
		}

		//cBKGTime

	}

	int		_iMPEntryIndex;

	int		_iMPStatus;
	int		_iMPItemIndex;
	char	_szMPFacilityName[MAX_NAME_LENGTH];
	char	_szMPItemID[MAX_ID_LENGTH];
	int		_iMPMeasurementType;
	float	_fMPEnrichmentPercent;

	float	_fMPBurnUp;
	int		_iMPDischargeDateYear;
	int		_iMPDischargeDateMonth;
	int		_iMPDischargeDateDay;
	int		_iMPCycle;

	float _fMPNChanAThresh;
	float _fMPNChanBThresh;

	int _iMPMeasurementDateYear;
	int _iMPMeasurementDateMonth;
	int _iMPMeasurementDateDay;

	float _fMPCoolingTime;
	float _fMPLogCoolingTime;
	float _fMPNChanA;
	float _fMPNChanB;
	float _fMPNChanC;

	float _fMPGDose1;
	float _fMPGDose2;
	char _szMPDetectorID[MAX_NAME_LENGTH];

	float	_fMPLogGammaDose;

	float	_fMPLogBurnup;
	float	_fMPLogDecayCorTotNeutrons;
	float	_fMPLogNoverG;

	AnalysisRec _sScanRecs[MAXIMUM_CYCLES_AUTHORIZED];
	int		_dScanRecMax;

	float	_fBkgMeasurements[MaxChannels];
	COleDateTime cBkgTime;
	COleDateTime _iMPMeasurementDateTime;
	
	ChannelCounts _dAve;

	bool m_bEAFChanged[eafEnumCount];


};

class VirtualMeasurement: public PROCESSDATA
{
public:
	VirtualMeasurement(void);
	~VirtualMeasurement(void);

	void UpdateMeasurementInfo(int iCID, LPCSTR pRawString, CString& szDisplayableResult);
	void CalcCoolingTime();
	void ApplyAdjustments();
	void SetDetectorID();
	void UpdateDischargeDate();
	void ComputeMeasurement();
	void FinishMeasurement(COleDateTime odtMTime);

	void SetInfo(RADEvent* p, const CString& id, const CString& detect, const CString& facname, const COleDateTime& mEndTime);

	void Certify();

	void SetBackgroundValues(double  bkg[], COleDateTime bkgtime);
	void SetBackgroundValues(float  bkg[], COleDateTime bkgtime);

	bool ValidContent();
	void MarkInfoAsEdited(int iCID);
	int CompareDischargeDate(COleDateTime dtime);
	int CompareMeasurementDate(COleDateTime mtime);
	COleDateTime GetDischargeDateTime()  const;

	bool m_bComputed;



};
