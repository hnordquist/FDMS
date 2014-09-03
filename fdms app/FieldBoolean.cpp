// $History: FieldBoolean.cpp $
// 
// *****************  Version 2  *****************
// User: Longo        Date: 11/24/04   Time: 3:48p
// Updated in $/FDMS/FDMS Application/FDMSNET
// * bring up select measurement dialog upon requests from UL
// * added matching class with logic: uses 182 day window to seek for
// match
// * compiler warning removal
#include "stdafx.h"
#include ".\FieldBoolean.h"

CFieldBoolean::CFieldBoolean(LPCTSTR pName, LPCTSTR pLow, LPCTSTR pHigh, LPCTSTR pFormat, LPCTSTR pInputFormat)
: CField(pName, pLow, pHigh, pFormat, pInputFormat)
{
	Convert();
}

CFieldBoolean::~CFieldBoolean(void)
{
}


bool CFieldBoolean::bSatisfiesConstraints(LPCTSTR)
{
	if (m_cstate == eInvalid)
		return false;
	else
		return true;
}

void CFieldBoolean::Convert()
{
	// if test 
	m_cstate = eOK;
	//else
	//	m_cstate = eFormat;
}

CString CFieldBoolean::szGetConstraintDescription(LPCTSTR pVal)
{
	CString s;
	if (m_cstate == eOK)
	{
		s = this->szStatusDesc[m_cstate];
	}
	if (m_cstate == eFormat)
	{
		s.Format("'%s' is not a boolean, try '%s' or '%s'.", pVal, low, high);
	}
	else if (m_cstate == eRange)
	{
		// should neverhappen s.Format("'%s' length is out of range 0 to %d.", pVal, m_length);
	}
	else // invalid
	{
		s = this->szStatusDesc[m_cstate];
	}
	return s;
}