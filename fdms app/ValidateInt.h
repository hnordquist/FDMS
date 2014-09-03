#pragma once
#include "validator.h"

class CValidateLong :
	virtual public CValidator
{
public:
	CValidateLong(LPCTSTR pLow, LPCTSTR pHigh);
	~CValidateLong(void);

	long lHigh, lLow, lVal;
	bool bCheckConstraints(LPCTSTR pVal);

protected:
	void Convert();

};
