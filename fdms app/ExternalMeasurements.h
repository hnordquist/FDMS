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
// $History: ExternalMeasurements.h $
// 
// *****************  Version 9  *****************
// User: 186846       Date: 4/04/08    Time: 6:02p
// Updated in $/FDMS/FDMSNET
// 
// *****************  Version 8  *****************
// User: 186846       Date: 4/01/08    Time: 3:40p
// Updated in $/FDMS/FDMSNET
// Mar. 29, fix for 398, partial for 447
// 
// *****************  Version 7  *****************
// User: Longo        Date: 3/17/05    Time: 9:32a
// Updated in $/FDMS/FDMS Application/FDMSNET
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
// User: Longo        Date: 2/07/05    Time: 10:19a
// Updated in $/FDMS/FDMS Application/FDMSNET
// checkpoint checkin
// 
// *****************  Version 2  *****************
// User: Longo        Date: 1/07/05    Time: 3:53p
// Updated in $/FDMS/FDMS Application/FDMSNET
// for CObList use
// 
// *****************  Version 1  *****************
// User: Longo        Date: 11/15/04   Time: 11:31a
// Created in $/FDMS/FDMS Application/FDMSNET
// But wait, there's more! If you are one of the first 50 callers we will
// add in tehse 4 source files, absolutely free, that's right! FREE!
#pragma once
#include "meacol.h"
#include ".\vmeasurement.h"
#include <afxtempl.h>

class ExternalMeasurements: public CObject
{
public:
	ExternalMeasurements();
	ExternalMeasurements(ULONG ulCycles);
	ExternalMeasurements(
		ULONG ulCycles, short FacilityID,	long StationID,	DATE Start,	DATE End,
		CString szFacilityLongName, CString szFacilityShortName,
		CString szStationLongName, CString szStationShortName,
		CString szInstrumentType		
		);
	~ExternalMeasurements(void);

	short sFacilityID;
	long lStationID;
	DATE StartTime;
	DATE EndTime;
	ULONG cycles;

	CString FacilityLongName, FacilityShortName;
	CString StationLongName, StationShortName;
	CString InstrumentType;

	double *pdData[MaxChannels];

	//int m_iSubsetSelector;
	int m_iGIndex[eisNum];	// non-zero when matched with an existing MP/DB entry
private:
	tAssayTypes m_tAssayType;  // bkg or verification
public:
	bool m_bExactMatch[eisNum];

	int StartNewDeclaration();
	void FinishNewDeclaration(int iItem);

	void ComputeMeasurement();
	void ComputeBackgroundMeasurement();

	bool CreateNewMeasurement();
	bool AddMeasurement();

	static void AgeStatus(const int iLastInde);
	static void PersistChanges(const bool bDBSave, CArray<int, int&>& measurementIndexes);

protected:
	void PopulateGlobals();

	void UpdateMPGlobal(int iIndex, int iCID);  // copies measured value on VirituaMeasurement to the matching global array
	void UpdateDBGlobal(int iIndex, int iCID);  // copies measured value on VirituaMeasurement to the matching global array
public:

	void CopyFromExistingGlobals(int iIndex);
	void SetAssayType(const CString& s);
	LPCTSTR GetAssayTypeString() const;
	tAssayTypes GetAssayType() const;
	VirtualMeasurement  m_vr;  // the measured and computed results from the event info

	bool m_bRM;
	bool GetRetentionMark() { return m_bRM;}
	void SetRetentionMark(bool v)  {m_bRM = v;}

	CString Image();

};
