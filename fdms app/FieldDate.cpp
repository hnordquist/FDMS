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
�	Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
disclaimer. 
�	Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
disclaimer in the documentation and/or other materials provided with the distribution. 
�	Neither the name of Los Alamos National Security, LLC, Los Alamos National Laboratory, LANL, the U.S. Government, 
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
// Copyright 2005, The Regents Of the University of California.  This
// software was produced under U.S. Government contract (W-7405-ENG-36) at
// the Los Alamos National Laboratory by the Safeguards Science and
// Technology Group (N-1).  All Rights Reserved.
//////////////////////////////////////////////////////////////////////////
// $History: FieldDate.cpp $
// 
// *****************  Version 5  *****************
// User: 186846       Date: 4/01/08    Time: 3:40p
// Updated in $/FDMS/FDMSNET
// Mar. 29, fix for 398, partial for 447
// 
// *****************  Version 4  *****************
// User: Longo        Date: 2/07/05    Time: 10:19a
// Updated in $/FDMS/FDMS Application/FDMSNET
// checkpoint checkin
// 
// *****************  Version 3  *****************
// User: Longo        Date: 1/14/05    Time: 2:52p
// Updated in $/FDMS/FDMS Application/FDMSNET
// time added
// 
// *****************  Version 2  *****************
// User: Longo        Date: 11/22/04   Time: 3:03p
// Updated in $/FDMS/FDMS Application/FDMSNET
// legacy timestamp string construction added
#include "stdafx.h"
#include ".\Fielddate.h"

CFieldDate::CFieldDate(LPCTSTR pName, LPCTSTR pLow, LPCTSTR pHigh, LPCTSTR pFormat, LPCTSTR pInputFormat,DWORD datetime)
			: CField(pName, pLow, pHigh, pFormat, pInputFormat)
{
	dDatetimeFlag = datetime;

	if (datetime == VAR_TIMEVALUEONLY)
	{
		m_type = eTime;
	}
	else if (datetime == VAR_DATEVALUEONLY)
	{
		m_type = eDate;
	}
	else
		m_type = eDateTime;

	Convert();
}

CFieldDate::~CFieldDate(void)
{
}


bool CFieldDate::bSatisfiesConstraints(LPCTSTR pVal)
{
	if (m_cstate == eInvalid)
		return false;

	bool bValOk = dVal.ParseDateTime(pVal, dDatetimeFlag);

	if (!bValOk) // check for screwy dot formatting
	{
		int y,m,d;
		if (3 == sscanf(pVal,CFieldDate::DateFormat, &y, &m, &d))
		{
			dVal.SetDate(y,m,d);
			bValOk = true;
		}
	}
	if (!bValOk)
	{
		m_cstate = eFormat;
	}
	else
	{
		if (dLow <= dVal && dHigh >= dVal)
			m_cstate = eOK;
		else
			m_cstate = eRange;
	}
	return (m_cstate == eOK);
}

CString CFieldDate::szGetConstraintDescription(LPCTSTR pVal)
{
	CString s;
	if (m_cstate == eOK)
	{
		s = this->szStatusDesc[m_cstate];
	}
	if (m_cstate == eFormat)
	{
		COleDateTime t = COleDateTime::GetCurrentTime();
		CString tef = t.Format(dDatetimeFlag, LANG_USER_DEFAULT);
		s.Format("'%s' is an invalid date or time, or in an unrecognized format, use values like '%s' instead.", pVal, tef);
	}
	else if (m_cstate == eRange)
	{
		s.Format("'%s' is out of range %s to %s.", pVal, dLow.Format(dDatetimeFlag, LANG_USER_DEFAULT), dHigh.Format(m_type, LANG_USER_DEFAULT));
	}
	else // invalid
	{
		s = this->szStatusDesc[m_cstate];
	}
	return s;
}
const CString CFieldDate::DateFormat = "%04d.%02d.%02d";
const CString CFieldDate::TimeFormat = "%02u:%02u:%02u";
const CString CFieldDate::DateTimeFormat = "%04d.%02d.%02d %02u:%02u:%02u";

const CString CFieldDate::DateTimeOutputFormat = "%Y.%m.%d %H:%M:%S";

void CFieldDate::FormatForOutput(COleDateTime od, CString& rs)
{
	rs.Format(DateTimeFormat,
	od.GetYear(),
	od.GetMonth(),
	od.GetDay(),
	od.GetHour(),
	od.GetMinute(),
	od.GetSecond());
}


void CFieldDate::Convert()
{

	bool bLowOK = dLow.ParseDateTime(low, dDatetimeFlag);
	bool bHighOK = dHigh.ParseDateTime(high, dDatetimeFlag);

	if (bLowOK && bHighOK)
		m_cstate = eOK;
	else
		m_cstate = eFormat;
}

COleDateTime CFieldDate::ConvertDefaultX(LPCTSTR pSomeVal, DWORD dwFlags )
{
	COleDateTime foo;

	bool bValOk = foo.ParseDateTime(pSomeVal, dwFlags);

	if (!bValOk) // check for screwy dot formatting
	{
		foo.SetDate(1950,1,1);
		int y,m,d;
		if (3 == sscanf(pSomeVal,CFieldDate::DateFormat, &y, &m, &d))
		{
			foo.SetDate(y,m,d);
			bValOk = true;
		}
	}

	return foo;
}

COleDateTime CFieldDate::ConvertX(LPCTSTR pSomeVal)
{
	COleDateTime foo;

	bool bValOk = foo.ParseDateTime(pSomeVal);

	if (!bValOk) // check for screwy dot formatting
	{
		int y,m,d;
		if (3 == sscanf(pSomeVal,CFieldDate::DateFormat, &y, &m, &d))
		{
			foo.SetDate(y,m,d);
			bValOk = true;
		}
	}

	return foo;
}

void CFieldDate::XConvert3(LPCTSTR pSomeVal, int& y, int& m, int& d)
{
	COleDateTime temp = ConvertX(pSomeVal);
	if (temp.GetStatus() == COleDateTime::valid)
	{
		d = temp.GetDay();
		m = temp.GetMonth();
		y = temp.GetYear();
	}
}



// time stamp format:	sssssYYYYMMDDHHMMSSmmm
// where:
//	            sssss	Station number (not used, always 0)
//	         YYYYMMDD	Date
//		   DDHHMM	Time
//		      mmm	milliseconds (always 0)
void CFieldDate::ConvertToTimestampStr (int y, int m, int d, CString& time_stamp)
{
	COleDateTime foo;
	foo.SetDate(y,m,d);
	time_stamp = foo.Format("00000%Y%m%d000000000");

}
