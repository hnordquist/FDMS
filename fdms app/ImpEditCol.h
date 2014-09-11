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
// $History: ImpEditCol.h $
// 
// *****************  Version 7  *****************
// User: Longo        Date: 3/16/05    Time: 2:21p
// Updated in $/FDMS/FDMS Application/FDMSNET
// nearing the deadlines ...
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
#pragma once

#include "field.h"
#include "SelectField.h"


class ImpEditCol:
	public CObject
{
public:
	ImpEditCol(void);
	~ImpEditCol(void);

	// column types
	typedef enum {
		eCheck,
		eItem, eFacility, eID,
		eMeasType, eEnrichment, eBurnUp,
		eDischDate, eCycle, eMeasDateTime, eCoolingTime,
		eNeutronA, eNeutronB, eNeutronC,
		eGamma1, eGamma2, eDetector, eThresholdA,
		eThresholdB, eInternalIndex,eColumnCount
	} ColumnID;
	
	BOOL m_bAscending[eColumnCount];

	// initial data for constructing array of header definitions
	static CField::FieldDesc m_szFieldData[eColumnCount];

private:
	CField* columns[eColumnCount];
	void LoadRangesFromIniData();

public:
	void Reconstruct();
	int count() const;
	void ClearOut();

	void SetAt(int i, CField* p);

	CField* GetAt(int i);

	bool isDateTime(ColumnID cid);

	static const COleDateTime m_DefaultStartDate;

	static const UINT m_meacolmap[ImpEditCol::eColumnCount];

	static const UINT m_fileheadermap[efhHeaderCount];

};
