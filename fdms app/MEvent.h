//////////////////////////////////////////////////////////////////////////
// Copyright 2005, The Regents Of the University of California.  This
// software was produced under U.S. Government contract (W-7405-ENG-36) at
// the Los Alamos National Laboratory by the Safeguards Science and
// Technology Group (N-1).  All Rights Reserved.
//////////////////////////////////////////////////////////////////////////
// $History: MEvent.h $
// 
// *****************  Version 7  *****************
// User: 186846       Date: 4/04/08    Time: 6:02p
// Updated in $/FDMS/FDMSNET
// 
// *****************  Version 6  *****************
// User: 186846       Date: 1/30/08    Time: 4:33p
// Updated in $/FDMS/FDMS Application/FDMSNET
// 447/448/399
// 
// *****************  Version 5  *****************
// User: Longo        Date: 3/11/05    Time: 10:35a
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
//////////////////////////////////////////////////////////////////////////
#pragma once
#include "..\AppUL\radeventlist.h"
#include ".\externalmeasurements.h"


class MEvent: public ExternalMeasurements
{
public:
	MEvent();

	MEvent(RADEvent* _pRADEvent,
		ULONG ulCycles, short FacilityID, long StationID,DATE Start,	DATE End,
		CString szFacilityLongName, CString szFacilityShortName,
		CString szStationLongName, CString szStationShortName,
		CString szInstrumentType);

	~MEvent();
	
	void SetBkgPointer(MEvent* p) {m_pBkg = p;}
	MEvent* GetBkgPointer() {return (MEvent*)m_pBkg;}
	void SetRadEvent(RADEvent* p);
	RADEvent* GetRadEvent() {return pRADEvent;}
	void SetAccepted(bool b) {m_bAccepted = b;}
	bool GetAccepted() const {return m_bAccepted;}
	void SetFit(bool b) {m_bFit = b;}
	bool GetFit() const {return m_bFit;}


	bool GetMark() { return pRADEvent->m_bMark;}
	void SetMark(bool v)  {pRADEvent->m_bMark = v;}

	CString Image();

	void Match();
	void Initialize();
	void SetAssayType();
	
	CString m_ID;

	void UpdateMeasurementInfo(int iCID, LPCSTR pRawString, CString& szDisplayableResult);



protected:
	RADEvent* pRADEvent;  // the rad event info
	ExternalMeasurements* m_pBkg;
	bool m_bAccepted, m_bFit;


}; 