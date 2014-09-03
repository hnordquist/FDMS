//////////////////////////////////////////////////////////////////////////
// Copyright 2005, The Regents Of the University of California.  This
// software was produced under U.S. Government contract (W-7405-ENG-36) at
// the Los Alamos National Laboratory by the Safeguards Science and
// Technology Group (N-1).  All Rights Reserved.
//////////////////////////////////////////////////////////////////////////
// $History: MEvent.cpp $
// 
// *****************  Version 9  *****************
// User: 186846       Date: 4/04/08    Time: 6:02p
// Updated in $/FDMS/FDMSNET
// 
// *****************  Version 8  *****************
// User: 186846       Date: 1/30/08    Time: 4:33p
// Updated in $/FDMS/FDMS Application/FDMSNET
// 447/448/399
// 
// *****************  Version 7  *****************
// User: Longo        Date: 3/16/05    Time: 2:21p
// Updated in $/FDMS/FDMS Application/FDMSNET
// nearing the deadlines ...
// 
// *****************  Version 6  *****************
// User: Longo        Date: 3/11/05    Time: 10:35a
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 5  *****************
// User: Longo        Date: 3/09/05    Time: 3:01p
// Updated in $/FDMS/FDMS Application/FDMSNET
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
// User: Longo        Date: 2/07/05    Time: 10:19a
// Updated in $/FDMS/FDMS Application/FDMSNET
// checkpoint checkin
// 
// *****************  Version 1  *****************
// User: Longo        Date: 1/10/05    Time: 1:20p
// Created in $/FDMS/FDMS Application/FDMSNET
// combined RAD event and external measurement with matching behavior
///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"


#include ".\FDMS.h"
#include ".\mevent.h"
#include ".\matchmeasurement.h"


#ifdef _XJDEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


MEvent::MEvent() : m_bAccepted(false), m_bFit(false), pRADEvent(NULL), m_pBkg(NULL)
{
}

MEvent::~MEvent()
{
}

MEvent::MEvent(RADEvent* _pRADEvent,
		ULONG ulCycles, short FacilityID,	long StationID,	DATE Start,	DATE End,
		CString szFacilityLongName, CString szFacilityShortName,
		CString szStationLongName, CString szStationShortName,
		CString szInstrumentType):

		ExternalMeasurements(ulCycles,  FacilityID,	StationID,	Start,	End,
		szFacilityLongName,
		szFacilityShortName,
		szStationLongName,
		szStationShortName,
		szInstrumentType),
		m_bFit(false), m_bAccepted(false)
{
	SetRadEvent(_pRADEvent);
	m_pBkg = NULL;
	Initialize();
}

void MEvent::SetRadEvent(RADEvent* p)
	{
		pRADEvent = p;
		m_ID = pRADEvent->values[eRADEventID];
		m_ID.Trim();
	}

void MEvent::SetAssayType()
{
	CString s = pRADEvent->values[	eRADEventType]; // Type of event (Unknown, Channel, Measurement or Background)
	s.Trim();

	ExternalMeasurements::SetAssayType(s);

}

void MEvent::UpdateMeasurementInfo(int iCID, LPCSTR pRawString, CString& szDisplayableResult)
{
	m_vr.UpdateMeasurementInfo(iCID, pRawString, szDisplayableResult);

	// catch id update for UI
	if (iCID == CMeaCol::eID)
	{
		m_ID = m_vr._szMPItemID;
		m_ID.Trim();
	}
}
// match on closest dates that match from the same detector id
void MEvent::Match()
{
	//Load the date and time information into a COleDateTime object.
	MatchMeasurement m(FacilityShortName, StationShortName, etgMinutesSeconds, eigFacilityDetectorItemID);
	COleDateTime a = pRADEvent->m_Times[RADEvent::eStartTimestamp];
	COleDateTime b = pRADEvent->m_Times[RADEvent::eEndTimestamp];   // timestamps match on end time, as measurements are stamped when they are completed and accepted
	m.SetMatchDate(a, b);
	m.SetMatchItem(m_ID);
	m.Match();

	// an existing measurement is found by matching in the MP
	SetMark((m.MeasurementMatched(eisMP) || m.bIdentityMatch(eisMP)));

	for (int _mmm = eisMP; _mmm < eisNum; _mmm++)
	{
		tMatchIdentitySource mmm = (tMatchIdentitySource)_mmm;
		if (m.MeasurementMatched(mmm))
		{
			m_iGIndex[mmm]  = m.GetMeasurementDateMatch(mmm);// ties the ExternalMeasurement data to the matched global measurement data
		}
		else if (m.bIdentityMatch(mmm))
		{
			m_iGIndex[mmm] = m.GetIdentityMatch(mmm);// ties the ExternalMeasurement data to the matched global measurement data
		}
		if (m_iGIndex[mmm] >= 0)
		{
			m.MatchIdentical(mmm, m_iGIndex[mmm]); // check the general match for an identical match
			m_bExactMatch[mmm] = m.bIdentical(mmm);
		}
	}
}

void MEvent::Initialize()
{
	m_vr.SetInfo(pRADEvent, m_ID, StationShortName, FacilityShortName, EndTime);
	SetAssayType();
}

CString MEvent::Image()
{
	return ExternalMeasurements::Image();
}
