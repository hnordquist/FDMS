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
