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