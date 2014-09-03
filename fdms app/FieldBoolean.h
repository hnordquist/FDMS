#pragma once
#include "Field.h"

class CFieldBoolean :
	virtual public CField
{
public:
	CFieldBoolean(LPCTSTR pName, LPCTSTR pLow, LPCTSTR pHigh, LPCTSTR pFormat, LPCTSTR pInputFormat);
	~CFieldBoolean(void);

	bool lVal;
	bool bSatisfiesConstraints(LPCTSTR pVal);
	CString szGetConstraintDescription(LPCTSTR pVal);

protected:
	void Convert();

};
