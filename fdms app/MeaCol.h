#pragma once

#include "field.h"

#include "SelectField.h"

class CMeaCol:
	public CObject
{
public:
	CMeaCol(void);
	~CMeaCol(void);

	// column types
	typedef enum {
		eItem, eFacility, eID,
		eMeasType, eEnrichment, eBurnUp,
		eDischDate, eCycle,  eMeasDate, eCoolingTime,
		eNeutronA, eNeutronB, eNeutronC,
		eGamma1, eGamma2, eDetector, eThresholdA,
		eThresholdB, eInternalIndex, eColumnCount
	} ColumnID;

	BOOL m_bAscending[eColumnCount];
	bool m_bColumnRWMap[eColumnCount];

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

	static const UINT m_fileheadermap[efhHeaderCount];


};
