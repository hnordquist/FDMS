#pragma once
#include "field.h"

class CFieldDouble :
	 public CField
{
public:
	CFieldDouble(LPCTSTR pName, LPCTSTR pLow, LPCTSTR pHigh, LPCTSTR pFormat, LPCTSTR pInputFormat);
	~CFieldDouble(void);

	double dHigh, dLow, dVal;
	bool bSatisfiesConstraints(LPCTSTR pVal);
	CString szGetConstraintDescription(LPCTSTR pVal);

protected:
	void Convert();

};
