#include "stdafx.h"
#include ".\Fielddouble.h"

CFieldDouble::CFieldDouble(LPCTSTR pName, LPCTSTR pLow, LPCTSTR pHigh, LPCTSTR pFormat, LPCTSTR pInputFormat)
			: CField(pName, pLow, pHigh, pFormat, pInputFormat)
{
	m_type = eDouble;
	Convert();
}

CFieldDouble::~CFieldDouble(void)
{
}


bool CFieldDouble::bSatisfiesConstraints(LPCTSTR pVal)
{
	if (m_cstate == eInvalid)
		return false;

	float dloat;
	if (1 != sscanf(pVal, "%g", &dloat))
	{
			m_cstate = eFormat;
	}
	else
	{
		dVal = dloat;
		if (dLow <= dVal && dHigh >= dVal)
			m_cstate = eOK;
		else
			m_cstate = eRange;
	}
	return (m_cstate == eOK);
}

void CFieldDouble::Convert()
{
	float flote;
	bool bLowOK =  (1 == sscanf(low, "%g", &flote));
	dLow = flote;
	bool bHighOK = (1 == sscanf(high, "%g", &flote));
	dHigh = flote;

	if (bLowOK && bHighOK)
	{
		m_cstate = eOK;
		low.Format(format, dLow);
		high.Format(format, dHigh);
	}
	else
		m_cstate = eFormat;
}

CString CFieldDouble::szGetConstraintDescription(LPCTSTR pVal)
{
	CString s;
	if (m_cstate == eOK)
	{
		s = this->szStatusDesc[m_cstate];
	}
	if (m_cstate == eFormat)
	{
		s.Format("'%s' is not a number, use a format like '%s' instead.", pVal, low);
	}
	else if (m_cstate == eRange)
	{
		s.Format("'%s' is out of range %s to %s.", pVal, low, high);
	}
	else // invalid
	{
		s = this->szStatusDesc[m_cstate];
	}
	return s;
}

