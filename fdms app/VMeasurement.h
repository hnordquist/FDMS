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
