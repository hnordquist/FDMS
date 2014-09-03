// $History: FieldString.cpp $ 
// 
// *****************  Version 2  *****************
// User: Longo        Date: 11/24/04   Time: 3:48p
// Updated in $/FDMS/FDMS Application/FDMSNET
// * bring up select measurement dialog upon requests from UL
// * added matching class with logic: uses 182 day window to seek for
// match
// * compiler warning removal
#include "stdafx.h"
#include ".\FieldString.h"

CFieldString::CFieldString(LPCTSTR pName, LPCTSTR pLow, LPCTSTR pHigh, LPCTSTR pFormat, LPCTSTR pInputFormat, long lLen)
			: CField(pName, pLow, pHigh, pFormat, pInputFormat)
{
		m_type = eString;
		m_length = lLen;
		Convert();
}

CFieldString::~CFieldString(void)
{
}


bool CFieldString::bSatisfiesConstraints(LPCTSTR )
{
	if (m_cstate == eInvalid)
		return false;

	// truncate val with m_length

	return true;

}

void CFieldString::Convert()
{
		m_cstate = eOK;
}


CString CFieldString::szGetConstraintDescription(LPCTSTR pVal)
{
	CString s;
	if (m_cstate == eOK)
	{
		s = this->szStatusDesc[m_cstate];
	}
	if (m_cstate == eFormat)
	{
		s.Format("'%s' does not match the format '%s'.", pVal, low);
	}
	else if (m_cstate == eRange)
	{
		s.Format("'%s' length is out of range 0 to %d.", pVal, m_length);
	}
	else // invalid
	{
		s = this->szStatusDesc[m_cstate];
	}
	return s;
}