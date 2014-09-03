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
