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
