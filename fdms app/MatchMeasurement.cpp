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
// $History: MatchMeasurement.cpp $
// 
// *****************  Version 9  *****************
// User: 186846       Date: 1/30/08    Time: 4:33p
// Updated in $/FDMS/FDMS Application/FDMSNET
// 447/448/399
// 
// *****************  Version 8  *****************
// User: Longo        Date: 3/16/05    Time: 2:21p
// Updated in $/FDMS/FDMS Application/FDMSNET
// nearing the deadlines ...
// 
// *****************  Version 7  *****************
// User: Longo        Date: 3/09/05    Time: 3:01p
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 6  *****************
// User: Longo        Date: 2/28/05    Time: 10:00a
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 5  *****************
// User: Longo        Date: 2/10/05    Time: 3:46p
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
// User: Longo        Date: 11/24/04   Time: 3:48p
// Updated in $/FDMS/FDMS Application/FDMSNET
// * bring up select measurement dialog upon requests from UL
// * added matching class with logic: uses 182 day window to seek for
// match
// * compiler warning removal
// 
// *****************  Version 1  *****************
// User: Longo        Date: 11/23/04   Time: 4:32p
// Created in $/FDMS/FDMS Application/FDMSNET
// matching class for IR tool timestamp match

#include "stdafx.h"
#include ".\FDMS.h"
#include ".\matchmeasurement.h"
#include ".\dconfig.h"

MatchMeasurement::MatchMeasurement(const char *pszLocation, tMatchTimeGranularity timeGranularity, tMatchIdentGranularity idGranularity) :
	m_tgranularity(timeGranularity),	
	m_igranularity(idGranularity),	
	m_seconds( GETPROFINT(eMATCHSECONDS) )
{

	for (int _i = eisMP; _i < eisNum; _i++)
	{
		tMatchIdentitySource s = (tMatchIdentitySource)_i;
		m_bEQ[s] = false;
		m_bIdentityMatch[s]  = false;
		m_measurementmatch[s]  = false;
		m_identitymatch[s]  = false;
	}


	m_days = GETPROFINT(eMATCHDAYS);
	m_minutes = GETPROFINT(eMATCHMINUTES);
	m_identitysource = eisMP;

	m_detector = pszLocation;

	m_detector.Trim();
}

MatchMeasurement::MatchMeasurement(const char *pszFacility, const char *pszLocation, tMatchTimeGranularity timeGranularity, tMatchIdentGranularity idGranularity) :
	m_tgranularity(timeGranularity),	
	m_igranularity(idGranularity),	
	m_seconds( GETPROFINT(eMATCHSECONDS) )
{
	m_days = GETPROFINT(eMATCHDAYS);
	m_minutes = GETPROFINT(eMATCHMINUTES);
	m_identitysource = eisMP;
	for (int _i = eisMP; _i < eisNum; _i++)
	{
		tMatchIdentitySource s = (tMatchIdentitySource)_i;
		m_bEQ[s] = false;
		m_bIdentityMatch[s]  = false;
		m_measurementmatch[s] =  false;
		m_identitymatch[s]  = false;
	}
	this->m_detector = pszLocation;
	this->m_facility = pszFacility;

	m_detector.Trim();
	m_facility.Trim();
}

MatchMeasurement::~MatchMeasurement(void)
{
}


bool MatchMeasurement::Matched(tMatchIdentitySource src)
{
	return m_measurementmatch[src] >= 0;// || m_dischargematch >= 0;
}

//bool MatchMeasurement::DischargeMatched() const
//{
//	return m_dischargematch >= 0;
//}
bool MatchMeasurement::MeasurementMatched(tMatchIdentitySource src) const
{
	return m_measurementmatch[src] >= 0;
}


void MatchMeasurement::Match()
{
	try
	{
		m_identitysource = eisMP;
		MatchDate(g_iMPMeasurementDateYear, g_iMPMeasurementDateMonth, g_iMPMeasurementDateDay, g_dMPMeasurementTime, m_measurementmatch[eisMP], m_measurementdate);
		m_identitysource = eisDB;
		float nothing = 0.0;
		MatchDate(g_iMeasurementDateYear, g_iMeasurementDateMonth, g_iMeasurementDateDay, &nothing, m_measurementmatch[eisDB], m_measurementdate);
		//MatchDate(g_iMPDischargeDateYear, g_iMPDischargeDateMonth, g_iMPDischargeDateDay, g_dMPMeasurementTime, m_dischargematch, m_dischargedate);
	}
	catch (...)
	{
	}
}

static const char* it(tMatchIdentitySource s, int i)
{
	switch (s)
	{
	case eisDB:
		return g_szItemIDs[i];
		break;

	case eisMP:
//	case eisM:  // with M, we find all in MP that have been measured!
	default:
		return g_szMPItemIDs[i];
		break;
	}
}
static const char* dn(tMatchIdentitySource s, int i)
{
	switch (s)
	{
	case eisDB:
		return g_szDetectorIDs[i];
		break;

	case eisMP:
//	case eisM:  // with M, we find all in MP that have been measured!
	default:
		return g_szMPDetectorIDs[i];
		break;
	}
}
static const char* fn(tMatchIdentitySource s,int i)
{
	switch (s)
	{
	case eisDB:
		return g_szFacilityNames[i];
		break;

	case eisMP:
//	case eisM:  // with M, we find all in MP that have been measured!
	default:
		return g_szMPFacilityNames[i];
		break;
	}
}

bool MatchMeasurement::CheckIdentityMatch(int i)
{
	const char * f= fn(m_identitysource, i);
	const char *d = dn(m_identitysource, i);
	const char *items = it(m_identitysource, i);

	bool res = true;
	switch (this->m_igranularity)
	{
	case eigNothing:		// always return true for a match request
		break;
	case eigFacility:
		if (m_facility.CompareNoCase(f) != 0)
			res = false;
		break;
	case eigDetector:  
		if (m_detector.CompareNoCase(d) != 0)
			res = false;
		break;
	case eigItemID:  
		if (m_item.CompareNoCase(items) != 0)
			res = false;
		break;
	case eigDetectorItemID: 
		if ((m_detector.CompareNoCase(d) != 0)
			|| (m_item.CompareNoCase(items) != 0))
			res = false;
		break;
	case eigFacilityItemID: 
		if ((m_facility.CompareNoCase(f) != 0)
			|| (m_item.CompareNoCase(items) != 0))
			res = false;
		break;
	case eigFacilityDetector: 
		if ((m_facility.CompareNoCase(f) != 0)
			|| (m_detector.CompareNoCase(d) != 0))
			res = false;
		break;
	case eigFacilityDetectorItemID: 
		if ((m_facility.CompareNoCase(f) != 0)
			|| ((strlen(d) > 0) && (m_detector.CompareNoCase(d) != 0))
			|| (m_item.CompareNoCase(items) != 0))
			res = false;
		else
			res = true;
		break;
	}
	return res;
}

void MatchMeasurement::MatchDate(const int *year, const int *month, const int *day, const float *_dayseconds, int& rmatch, COleDateTime& rv)
{
	int i;
	int matched = -1;
	int cydiff = INT_MAX, cmdiff = INT_MAX, cddiff = INT_MAX;
	int chdiff = INT_MAX, cMdiff = INT_MAX, csdiff = INT_MAX;

	int d = m_matchenddate.GetDay();
	int	m = m_matchenddate.GetMonth();
	int	y = m_matchenddate.GetYear();
	int	h = m_matchenddate.GetHour();
	int	M = m_matchenddate.GetMinute();
	int	s = m_matchenddate.GetSecond();

	int maxindex = -1;
	switch (m_identitysource)
	{
	case eisDB:
		maxindex = g_iEntriesInDB;
		break;

	case eisMP:
	//case eisM:  // with M, we find all in MP that have been measured!
		maxindex = g_iMPEntriesIn;
		break;
	}

	// find the closest date time stamp entry that matches the request constrained by an identity match 
	for (i = 0; i < maxindex; i++)
	{
		if (!CheckIdentityMatch(i))
			continue;

		COleDateTimeSpan x;
		x.SetDateTimeSpan(0,0,0,(int) (m_identitysource == eisMP ? _dayseconds[i] : *_dayseconds));

		const int qh = x.GetHours();
		const int qM = x.GetMinutes();
		const int qs = x.GetSeconds();

		int ydiff = abs(year[i] - y);
		int mdiff = abs(month[i] -  m);
		int ddiff = abs(day[i] -  d);
		int hdiff = abs(qh - h);
		int Mdiff = abs(qM - M)  + (hdiff *60);
		int sdiff = abs(qs - s);

		if ((ydiff <= cydiff) && (mdiff <= cmdiff) && (ddiff <= cddiff))
		{
			if (matched < 0)
				matched = i; // got a date match at least

			// now check time too
			if ((Mdiff <= cMdiff) && (sdiff <= csdiff))
			{
				matched = i;
				chdiff = hdiff;
				cMdiff = Mdiff;
				csdiff = sdiff;
			}
			cydiff = ydiff;
			cmdiff = mdiff;
			cddiff = ddiff;
		}
	}

	// now see if this match is close enough
	rmatch = matched;

	if (matched < 0)  // basic identity match failed
		return;

	m_bIdentityMatch[m_identitysource] = true;
	m_identitymatch[m_identitysource] = rmatch;

	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
	pApp->AppendToLogF("id match: %s %d [%d]",m_item, m_identitysource, rmatch);
	
		// now check for the timespan range matching constraints
	COleDateTimeSpan mss;
	mss.SetDateTimeSpan(0,0,0,(int) (m_identitysource == eisMP ? _dayseconds[matched] : *_dayseconds));

	const int qmh = mss.GetHours();
	const int qmM = mss.GetMinutes();
	const int qms = mss.GetSeconds();
	rv.SetDateTime(year[matched], month[matched], day[matched], qmh, qmM, qms);
	COleDateTimeSpan span = m_matchenddate - rv;

	switch (this->m_tgranularity)
	{
		case etgClosest:		// always return true for a match request
			break;
		case etgDaysOnly: // use the number of days only, a crude match
			if (abs(span.GetDays()) > m_days)
				rmatch = -1;
			break;
		case etgMinutesOnly:  
			if (abs(span.GetDays()) > m_days || abs(span.GetMinutes()) > m_minutes)
				rmatch = -1;
			break;
		case etgSecondsOnly:  
			if (abs(span.GetDays()) > m_days || abs(span.GetSeconds()) > m_seconds)
				rmatch = -1;
			break;
		case etgMinutesSeconds:   // todo: jfl this is screwed up, rewrite it
			if (abs(span.GetDays()) > 1)  // not a day match
				rmatch = -1;
			else if (abs(span.GetMinutes()) > m_minutes) // not a minute match on the day
				rmatch = -1;
				// || abs(span.GetSeconds()) > m_seconds)
			break;
		case etgDaysMinutesSeconds: 
			if (abs(span.GetDays()) != m_days && abs(span.GetMinutes()) != m_minutes && abs(span.GetSeconds()) != m_seconds)
				rmatch = -1;
			break;
	}
	pApp->AppendToLogF("time match: %d [%d]",m_identitysource, rmatch);

	return;
}

void MatchMeasurement::MatchIdentical(tMatchIdentitySource src, int i)
{
	m_bEQ[src] = (m_facility.CompareNoCase(g_szMPFacilityNames[i]) == 0)
			//&& (m_detector.CompareNoCase(g_szMPDetectorIDs[i]) == 0)
			&& (m_item.CompareNoCase(g_szMPItemIDs[i]) == 0);
	m_bEQ[src] = m_bEQ[src]
		&& g_iMPMeasurementDateYear[i] == m_matchenddate.GetYear()
		&& g_iMPMeasurementDateMonth[i] == m_matchenddate.GetMonth()
		&& g_iMPMeasurementDateDay[i] == m_matchenddate.GetDay();

	if (m_bEQ[src])
	{
		COleDateTimeSpan ms;
		ms.SetDateTimeSpan(0,0,0,(int)g_dMPMeasurementTime[i]);
		m_bEQ[src] = (m_matchenddate.GetHour() == ms.GetHours())
					&&
				(m_matchenddate.GetMinute() == ms.GetMinutes())
					&&
				(m_matchenddate.GetSecond() == ms.GetSeconds());
	}
}
bool MatchMeasurement::bIdentityMatch(tMatchIdentitySource src) const
{
	return m_bIdentityMatch[src];
}
bool MatchMeasurement::bIdentical(tMatchIdentitySource src) const
{
	return m_bEQ[src];
}