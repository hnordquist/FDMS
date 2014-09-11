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
// Copyright 2005, The Regents Of the University of California.  This
// software was produced under U.S. Government contract (W-7405-ENG-36) at
// the Los Alamos National Laboratory by the Safeguards Science and
// Technology Group (N-1).  All Rights Reserved.
//////////////////////////////////////////////////////////////////////////
// $History: FieldDate.h $
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
#pragma once
#include "Field.h"

class CFieldDate :
	 public CField
{
public:
	CFieldDate(LPCTSTR pName, LPCTSTR pLow, LPCTSTR pHigh, LPCTSTR pFormat, LPCTSTR pInputFormat, DWORD datetime = 0);
	~CFieldDate(void);

	COleDateTime dHigh, dLow, dVal;
	DWORD dDatetimeFlag;
	bool bSatisfiesConstraints(LPCTSTR pVal);
	CString szGetConstraintDescription(LPCTSTR pVal);

	static const CString DateFormat;
	static const CString TimeFormat;
	static const CString DateTimeFormat;
	static const CString DateTimeOutputFormat;

	static void FormatForOutput(COleDateTime od, CString& rs);

	static COleDateTime ConvertX(LPCTSTR pSomeVal);
	static COleDateTime ConvertDefaultX(LPCTSTR pSomeVal, DWORD dwFlags = 0);
	static void XConvert3(LPCTSTR pSomeVal, int& y, int& m, int& d);

	
	
	// time stamp format:	sssssYYYYMMDDHHMMSSmmm
    // where:
    //	            sssss	Station number (not used, always 0)
    //	         YYYYMMDD	Date
    //		   DDHHMM	Time
    //		      mmm	milliseconds (always 0)
	static void ConvertToTimestampStr (int y, int m, int d, CString& time_stamp);


protected:
	void Convert();

};
