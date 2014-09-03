//////////////////////////////////////////////////////////////////////////
// Copyright 2005, The Regents Of the University of California.  This
// software was produced under U.S. Government contract (W-7405-ENG-36) at
// the Los Alamos National Laboratory by the Safeguards Science and
// Technology Group (N-1).  All Rights Reserved.
//////////////////////////////////////////////////////////////////////////
// $History: ImpEditCol.cpp $
// 
// *****************  Version 11  *****************
// User: 186846       Date: 4/04/08    Time: 6:02p
// Updated in $/FDMS/FDMSNET
// 
// *****************  Version 10  *****************
// User: 186846       Date: 2/26/08    Time: 8:54a
// Updated in $/FDMS/FDMS Application/FDMSNET
// checkpoint
// 
// *****************  Version 9  *****************
// User: 186846       Date: 1/29/08    Time: 5:46p
// Updated in $/FDMS/FDMS Application/FDMSNET
// tweakings
// 
// *****************  Version 8  *****************
// User: Longo        Date: 3/16/05    Time: 2:21p
// Updated in $/FDMS/FDMS Application/FDMSNET
// nearing the deadlines ...
// 
// *****************  Version 7  *****************
// User: Longo        Date: 3/11/05    Time: 10:35a
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 6  *****************
// User: Longo        Date: 3/09/05    Time: 3:01p
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 5  *****************
// User: Longo        Date: 2/28/05    Time: 10:00a
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 4  *****************
// User: Longo        Date: 2/10/05    Time: 3:46p
// Updated in $/FDMS/FDMS Application/FDMSNET
// checkpoint checkin
// 
// *****************  Version 3  *****************
// User: Longo        Date: 2/07/05    Time: 10:19a
// Updated in $/FDMS/FDMS Application/FDMSNET
// checkpoint checkin
// 
// *****************  Version 2  *****************
// User: Longo        Date: 1/07/05    Time: 3:54p
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 1  *****************
// User: Longo        Date: 1/07/05    Time: 2:26p
// Created in $/FDMS/FDMS Application/FDMSNET
// support for new import edit dialog
//////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "FDMS.h"
#include ".\impeditcol.h"
#include ".\meacol.h"
#include ".\dconfig.h"


#ifdef _XJDEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const  COleDateTime ImpEditCol::m_DefaultStartDate = COleDateTime(1950, 1,1,0,0,0);

// MS Facility	ID	Measurement type	Status	Enrichment (%)	Burnup (GWd/MT)	Discharge day	month	year	cycle #	Thres A	Thres B	 Measurement day	month	year	Cooling Time (years)	 NA	 NB	 NC	 G1	 G2 	Detector      
/*
		efhFacility, efhID,
		efhMeasType, efhStatus,
		efhEnrichment, efhBurnUp,
		efhDischDay,efhDischMonth,efhDischYear,
		efhCycle,
		efhThresholdA,		efhThresholdB, 
		efhMeasDay,efhMeasMonth,efhMeasYear,
		efhCoolingTime,
		efhNeutronA, efhNeutronB, efhNeutronC,
		efhGamma1, efhGamma2, efhDetector, efhHeaderCount
		*/
const UINT ImpEditCol::m_fileheadermap[] =
{
	ImpEditCol::eFacility, ImpEditCol::eID, 
	ImpEditCol::eMeasType, -1,
	ImpEditCol::eEnrichment, ImpEditCol::eBurnUp,
	ImpEditCol::eDischDate, ImpEditCol::eDischDate, ImpEditCol::eDischDate,
	ImpEditCol::eCycle,
	ImpEditCol::eThresholdA,	ImpEditCol::eThresholdB,
	ImpEditCol::eMeasDateTime, ImpEditCol::eMeasDateTime, ImpEditCol::eMeasDateTime,
	ImpEditCol::eCoolingTime,
	ImpEditCol::eNeutronA, ImpEditCol::eNeutronB, ImpEditCol::eNeutronC,
	ImpEditCol::eGamma1, ImpEditCol::eGamma2, ImpEditCol::eDetector
};


const UINT ImpEditCol::m_meacolmap[] =
{
		/*eCheck,*/ 	CMeaCol::eItem, 
		/*eItem,*/ 	CMeaCol::eItem, 
		/*eFacility,*/ 	CMeaCol::eFacility, 
	CMeaCol::eID, 
	CMeaCol::eMeasType,CMeaCol:: eEnrichment, CMeaCol::eBurnUp,
	CMeaCol::eDischDate, CMeaCol::eCycle, CMeaCol::eMeasDate, CMeaCol::eCoolingTime,
	CMeaCol::eNeutronA, CMeaCol::eNeutronB, CMeaCol::eNeutronC,
	CMeaCol::eGamma1, CMeaCol::eGamma2, CMeaCol::eDetector, 
	CMeaCol::eThresholdA,	CMeaCol::eThresholdB,
	CMeaCol::eInternalIndex
};
// MP Facility	ID	Measurement type	Status	Enrichment (%)	Burnup (GWd/MT)	Discharge day	month	year	cycle #	Thres A	Thres B	 Measurement day	month	year	Cooling Time (years)	 NA	 NB	 NC	 G1	 G2 	Detector      
// DB Facility	ID	Measurement type	Status	Enrichment (%)	Burnup (GWd/MT)	Discharge day	month	year	cycle #	Thres A	Thres B	 Measurement day	month	year	Cooling Time (years)	 NA	 NB	 NC	 G1	 G2 	Detector
// MS Facility	ID	Measurement type	Status	Enrichment (%)	Burnup (GWd/MT)	Discharge day	month	year	cycle #	Thres A	Thres B	 Measurement day	month	year	Cooling Time (years)	 NA	 NB	 NC	 G1	 G2 	Detector      

ImpEditCol::ImpEditCol(void)
{
	for (int i = 0; i < eColumnCount; i++)
	{
		m_bAscending[i] = TRUE;
		columns[i] = NULL;
	}
}

ImpEditCol::~ImpEditCol(void)
{
}

static const CString curdatenowtime = (COleDateTime::GetCurrentTime()).Format(VAR_DATEVALUEONLY, LANG_USER_DEFAULT );

CField::FieldDesc ImpEditCol::m_szFieldData[ImpEditCol::eColumnCount] =
{
	{CField::eImageField, "  ",		"0",				"128", false, "", "", 24, LVCFMT_CENTER, "", false},
	{CField::eString, "Item",		"0",				"128", false, "%05d", "", 40, LVCFMT_CENTER, "!!!", true},
	{CField::eString, "Facility",		"0",			"128", true, "%05d", "", 40, LVCFMT_LEFT, "Facility", true},
	{CField::eString, "ID",			"0",				"128", true, "", "", 60, LVCFMT_LEFT, "Item Identification", true},
	{CField::eLong,	"Meas. Type",		"1",			"3",   true, "%d", "%d", 25, LVCFMT_LEFT, "", true},
	{CField::eDouble, "Enrichment",	"0.0",				"100.0", true, "%.2lf", "%g", 0, LVCFMT_RIGHT, "Enrichment %", true},
	{CField::eDouble, "Burnup",	"0.0",					"100.0", true, "%.2lf", "%g", 25, LVCFMT_RIGHT, "Burnup %", true},
	{CField::eDate, "Disch. Date",	"1 January 1950",	curdatenowtime, true, "", "", 75, LVCFMT_CENTER, "Declared Discharge Date", true},
	{CField::eLong, "Cycle #",		"0",				"9", true, "%d", "%d", 0, LVCFMT_CENTER, "Cycle #", true},
	{CField::eDateTime, "Meas. Date","1 January 1950",	curdatenowtime, false, "", "", 75, LVCFMT_CENTER, "Measurement Date and Time", true},
	{CField::eDouble, "Cooling Time","0.0",				"1e6", true, "%.2lf", "%g", 0, LVCFMT_RIGHT, "Cooling Time: Declared or Calculated", true},
	{CField::eDouble, "Neutron A",	"0.0",				"1e9", false, "%.2lf", "%g", 0, LVCFMT_RIGHT, "Calculated Neutron", true}, //%#6.5lf
	{CField::eDouble, "Neutron B",	"0.0",				"1e9", false, "%.2lf", "%g", 0, LVCFMT_RIGHT, "Calculated Neutron", true}, //%#6.5lf
	{CField::eDouble, "Neutron C",	"0.0",				"1e9", false, "%.2lf", "%g", 0, LVCFMT_RIGHT, "Calculated Neutron", true}, //%#7.lf
	{CField::eDouble, "Gamma 1",	"0.0",				"1e9", false, "%.2lf", "%g", 0, LVCFMT_RIGHT, "Calculated Gamma", true},
	{CField::eDouble, "Gamma 2",	"0.0",				"1e9", false, "%.2lf", "%g", 0, LVCFMT_RIGHT, "Calculated Gamma", true},
	{CField::eString, "Detector",	"0",				"128", false, "", "", 55, LVCFMT_CENTER, "Detector or Station ID", true},
	{CField::eDouble, "A (Threshold)",	"0.00",			"1e6", false, "%.2lf", "%g", 0, LVCFMT_RIGHT, "", true},
	{CField::eDouble, "B (Threshold)",	"0.00",			"1e6", false, "%.2lf", "%g", 0, LVCFMT_RIGHT, "", true},
	{CField::eLong, "internal",		"0",				"9999", false, "%d", "%d", 5, LVCFMT_CENTER, "*", false},

};

void ImpEditCol::ClearOut()
{
	for (int i = 0; i < eColumnCount; i++)
	{
		CField* p = NULL;
		p = columns[i];
		if (p)
			delete p;
	}
}

void ImpEditCol::SetAt(int i, CField* p)
{
	columns[i] = p;
}

CField* ImpEditCol::GetAt(int i)
{
	return columns[i];
}

bool ImpEditCol::isDateTime(ImpEditCol::ColumnID cid)
{
	return (m_szFieldData[cid]._type == CField::eDate) || 
				(m_szFieldData[cid]._type == CField::eDateTime) ;
}

void ImpEditCol::LoadRangesFromIniData()
{
	CString Low, High;
	for (int i = eItem_RANGE;
		i < (eItem_RANGE + (eInternalIndex + 1));
		i++)
	{
		GETRANGEVALUES((tIniNames)i, Low, High);
		int index = i - eItem_RANGE;

		Low.Trim(); High.Trim();
		m_szFieldData[ index ].low = Low;
		m_szFieldData[ index ].high = High;

		switch(m_szFieldData[ index ]._type)
		{
			case CField::eString:
				break;
			case CField::eDouble:
				{
					float dLow, dHigh;
					sscanf(Low, "%g", &dLow);
					sscanf(High, "%g", &dHigh);
					m_szFieldData[ index ].low.Format(m_szFieldData[ index ].format, dLow);
					m_szFieldData[ index ].high.Format(m_szFieldData[index ].format, dHigh);
				}
				break;
			case CField::eLong:
				break;
			case CField::eDate:
				break;
			case CField::eTime:
				break;
			case CField::eDateTime:
				break;
			case CField::eBoolean:
				break;
			default:
				break;
		}
	}
}

void ImpEditCol::Reconstruct()
{
	LoadRangesFromIniData();
	for (int i = 0; i < eColumnCount; i++)
	{
		CField* p = NULL;
		switch(m_szFieldData[i]._type)
		{
			case CField::eString:
			case CField::eImageField:
				p = new CFieldString(m_szFieldData[i].name,m_szFieldData[i].low, m_szFieldData[i].high, m_szFieldData[i].format, m_szFieldData[i].inputFormat,128);
				SetAt(i, p);
				break;
			case CField::eDouble:
				p = new CFieldDouble(m_szFieldData[i].name,m_szFieldData[i].low, m_szFieldData[i].high, m_szFieldData[i].format, m_szFieldData[i].inputFormat);
				SetAt(i, p);
				break;
			case CField::eLong:
				p = new CFieldLong(m_szFieldData[i].name,m_szFieldData[i].low, m_szFieldData[i].high, m_szFieldData[i].format, m_szFieldData[i].inputFormat);
				SetAt(i, p);
				break;
			case CField::eDate:
				p = new CFieldDate(m_szFieldData[i].name,m_szFieldData[i].low, m_szFieldData[i].high, m_szFieldData[i].format, m_szFieldData[i].inputFormat, VAR_DATEVALUEONLY);
				SetAt(i,p);
				break;
			case CField::eTime:
				p = new CFieldDate(m_szFieldData[i].name,m_szFieldData[i].low, m_szFieldData[i].high, m_szFieldData[i].format, m_szFieldData[i].inputFormat, VAR_TIMEVALUEONLY);
				SetAt(i, p);
				break;
			case CField::eDateTime:
				p = new CFieldDate(m_szFieldData[i].name,m_szFieldData[i].low, m_szFieldData[i].high, m_szFieldData[i].format, m_szFieldData[i].inputFormat);
				SetAt(i,p);
				break;
			case CField::eBoolean:
				p = new CFieldBoolean(m_szFieldData[i].name,m_szFieldData[i].low, m_szFieldData[i].high, m_szFieldData[i].format, m_szFieldData[i].inputFormat);
				SetAt(i,p);
				break;
			default:
				p = NULL;
				break;
		}
		if (p)
			p->tooltipTemplate = m_szFieldData[i].tooltipTemplate;
	}
}


int ImpEditCol::count() const
{
		return eColumnCount;
}