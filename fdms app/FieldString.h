#pragma once
#include "Field.h"

class CFieldString :
	public CField
{
public:
	CFieldString(LPCTSTR pName, LPCTSTR pLow, LPCTSTR pHigh, LPCTSTR pFormat, LPCTSTR pInputFormat, long lLen);
	~CFieldString(void);

	CString lVal;
	long m_length;
	bool bSatisfiesConstraints(LPCTSTR pVal);
	CString szGetConstraintDescription(LPCTSTR pVal);

protected:
	void Convert();

};
