//////////////////////////////////////////////////////////////////////////
// Copyright 2004, The Regents Of the University of California.  This
// software was produced under U.S. Government contract (W-7405-ENG-36) at
// the Los Alamos National Laboratory by the Safeguards Science and
// Technology Group (N-1).  All Rights Reserved.
//////////////////////////////////////////////////////////////////////////
// $History: FieldLong.cpp $
// 
// *****************  Version 2  *****************
// User: Longo        Date: 1/05/05    Time: 5:30p
// Updated in $/FDMS/FDMS Application/FDMSNET
//////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include ".\FieldLong.h"

CFieldLong::CFieldLong(LPCTSTR pName, LPCTSTR pLow, LPCTSTR pHigh, LPCTSTR pFormat, LPCTSTR pInputFormat)
			: CField(pName, pLow, pHigh, pFormat, pInputFormat)
{
		m_type = eLong;
		Convert();
}

CFieldLong::~CFieldLong(void)
{
}


bool CFieldLong::bSatisfiesConstraints(LPCTSTR pVal)
{
	if (m_cstate == eInvalid)
		return false;

	long iInt;
	if (1 != sscanf(pVal, "%ld", &iInt))
	{
		m_cstate = eFormat;
	}
	else
	{
		lVal = iInt;
		if (lLow <= lVal && lHigh >= lVal)
			m_cstate = eOK;
		else
			m_cstate = eRange;
	}
	return (m_cstate == eOK);
}

void CFieldLong::Convert()
{
	long iInt;
	bool bLowOK = (1 == sscanf(low, "%ld", &iInt));
	lLow = iInt;
	bool bHighOK = (1 == sscanf(high, "%ld", &iInt));
	lHigh = iInt;

	if (bLowOK && bHighOK)
	{
		m_cstate = eOK;
		low.Format(format, lLow);
		high.Format(format, lHigh);
	}
	else
		m_cstate = eFormat;
}

CString CFieldLong::szGetConstraintDescription(LPCTSTR pVal)
{
	CString s;
	if (m_cstate == eOK)
	{
		s = this->szStatusDesc[m_cstate];
	}
	if (m_cstate == eFormat)
	{
		s.Format("'%s' is not a long integer, use a number like '%s' instead.", pVal, low);
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

