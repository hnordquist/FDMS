#pragma once
#include "Field.h"

class CFieldLong :
	public CField
{
public:
	CFieldLong(LPCTSTR pName, LPCTSTR pLow, LPCTSTR pHigh, LPCTSTR pFormat, LPCTSTR pInputFormat);
	~CFieldLong(void);

	long lHigh, lLow, lVal;
	bool bSatisfiesConstraints(LPCTSTR pVal);
	CString szGetConstraintDescription(LPCTSTR pVal);

protected:
	void Convert();

};
