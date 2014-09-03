#include "stdafx.h"
#include ".\Field.h"


const PTCHAR CField::szStatusDesc[eStatusTypes+1] = 
{
	"Invalid", "OK", "Format", "Range", "@"
};


CField::CField(LPCTSTR pName, LPCTSTR pLow, LPCTSTR pHigh, LPCTSTR pFormat, LPCTSTR pInputFormat) : 
	low (pLow), high(pHigh), m_cstate(eInvalid),
	bMutable(false), name(pName), format(pFormat), inputFormat(pInputFormat) //, szState(szStatusDesc[eInvalid])
{
	m_type = eUnknownType;
}

CField::~CField(void)
{
}


void CField::Reset(LPCTSTR pLow, LPCTSTR pHigh)
{
	low = pLow;
	high= pHigh;
	Convert();
}


void CField::Convert()
{
}