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
// $History: MatchMeasurement.h $
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
// User: Longo        Date: 2/10/05    Time: 3:46p
// Updated in $/FDMS/FDMS Application/FDMSNET
// checkpoint checkin
// 
// *****************  Version 3  *****************
// User: Longo        Date: 1/10/05    Time: 12:47p
// Updated in $/FDMS/FDMS Application/FDMSNET
// matching on date time AND facility, detector and item id
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
#pragma once
typedef enum
{
	etgClosest,		// always return the closest match, no matter how large the time difference, no constraints are applied
	etgDaysOnly, // use the number of days only, a crude match within the number of days tolerance
	etgMinutesOnly,  // find the closest match on the same day, within the minutes tolerance: must be on the same day
	etgSecondsOnly,  // find the closest match on the same day, within the seconds tolerance: must be on the same day, ignores minutes
	etgMinutesSeconds,  //  find the closest match on the same day, within the minutes and seconds tolerances: must be on the same day
	etgDaysMinutesSeconds,  // same as above, but must be exact to the second, not the closest
} tMatchTimeGranularity;


typedef enum
{
	eigNothing,		// ignore unique identifying information
	eigFacility,	// match facility
	eigDetector,  //match detector id, (station name)
	eigItemID,  // match Item id
	eigDetectorItemID,  //match detector id, (station name), and Item id
	eigFacilityDetector,  //match facility, detector id, (station name)
	eigFacilityItemID,  //match facility, and Item id
	eigFacilityDetectorItemID,  //match facility, detector id, (station name), and Item id
} tMatchIdentGranularity;

class MatchMeasurement
{
public:
	MatchMeasurement(const char *pszLocation, tMatchTimeGranularity timeGranularity, tMatchIdentGranularity idGranularity = eigNothing);
	MatchMeasurement(const char *pszFacility, const char *pszLocation, tMatchTimeGranularity timeGranularity, tMatchIdentGranularity idGranularity = eigNothing);
	~MatchMeasurement(void);

	void SetIDMatchSource(tMatchIdentitySource s)
	{
		m_identitysource = s;
	}

    
	void SetMatchDate(COleDateTime a, COleDateTime b)
	{
		m_match_startdate = a;
		m_matchenddate = b;
	}

	void SetMatchIdentification(const CString& fac, const CString& detector, const CString& item)
	{
		m_facility = fac;
		m_detector = detector;
		m_item = item;
		m_facility.Trim();
		m_item.Trim();
		m_detector.Trim();
	}
	
	void SetMatchFacility(const CString& detector)
	{
		m_detector = detector;
		m_detector.Trim();
	}
	
	void SetMatchDetector(const CString& fac)
	{
		m_facility = fac;
		m_facility.Trim();
	}

	void SetMatchItem(const CString& item)
	{
		m_item = item;
		m_item.Trim();
	}

	int GetMeasurementDateMatch(tMatchIdentitySource src)
	{
		return m_measurementmatch[src];
	}
	//int GetDischargeDateMatch()
	//{
	//	return m_dischargematch;
	//}
	int GetIdentityMatch(tMatchIdentitySource src)
	{
		return m_identitymatch[src];
	}
   
	bool Matched(tMatchIdentitySource src);
	bool DischargeMatched(tMatchIdentitySource src) const;
	bool MeasurementMatched(tMatchIdentitySource src) const;

	// find match based on date, item and identifiers
	// find the closest m and d dates to the target date
	void Match();

	void MatchIdentical(tMatchIdentitySource src, int withthis);
	bool bIdentical(tMatchIdentitySource src) const;

	bool bIdentityMatch(tMatchIdentitySource src) const;

private:
	CString m_detector, m_facility, m_item;

	COleDateTime m_match_startdate;
	COleDateTime m_matchenddate;
	COleDateTime m_measurementdate;
	//COleDateTime m_dischargedate;
	int m_measurementmatch[eisNum];
	//int m_dischargematch;
	int m_identitymatch[eisNum];
	tMatchIdentitySource m_identitysource;

	int m_days, m_minutes, m_seconds;

	tMatchTimeGranularity m_tgranularity;
	tMatchIdentGranularity m_igranularity;

	bool m_bEQ[eisNum];
	bool m_bIdentityMatch[eisNum];

	void MatchDate(const int *year, const int *month, const int *day, const float *_dayseconds, int& rmatch, COleDateTime& rv);

	// test current element against the identifier match specification
	bool CheckIdentityMatch(int iEntry);

};
