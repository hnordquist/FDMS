/*
Copyright (c) 2014, Los Alamos National Security, LLC
All rights reserved.
Copyright 2014. Los Alamos National Security, LLC. This software was produced under U.S. Government contract 
DE-AC52-06NA25396 for Los Alamos National Laboratory (LANL), which is operated by Los Alamos National Security, 
LLC for the U.S. Department of Energy. The U.S. Government has rights to use, reproduce, and distribute this software.  
NEITHER THE GOVERNMENT NOR LOS ALAMOS NATIONAL SECURITY, LLC MAKES ANY WARRANTY, EXPRESS OR IMPLIED, 
OR ASSUMES ANY LIABILITY FOR THE USE OF THIS SOFTWARE.  If software is modified to produce derivative works, 
such modified software should be clearly marked, so as not to confuse it with the version available from LANL.

Additionally, redistribution and use in source and binary forms, with or without modification, are permitted provided 
that the following conditions are met:
•	Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
disclaimer. 
•	Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
disclaimer in the documentation and/or other materials provided with the distribution. 
•	Neither the name of Los Alamos National Security, LLC, Los Alamos National Laboratory, LANL, the U.S. Government, 
nor the names of its contributors may be used to endorse or promote products derived from this software without specific 
prior written permission. 
THIS SOFTWARE IS PROVIDED BY LOS ALAMOS NATIONAL SECURITY, LLC AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED 
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL LOS ALAMOS NATIONAL SECURITY, LLC OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
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

