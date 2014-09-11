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
// $History: FDMSReview.h $ 
// 
// *****************  Version 4  *****************
// User: Longo        Date: 11/30/04   Time: 4:22p
// Updated in $/FDMS/FDMS Application/FDMSNET/AppUl
// added code to auto-select the matching row in the measeur dialog list,
// when involkd by IR tool review message
// 
// *****************  Version 3  *****************
// User: Longo        Date: 11/24/04   Time: 3:43p
// Updated in $/FDMS/FDMS Application/FDMSNET/AppUl
// Some good things are working now
// 
// *****************  Version 2  *****************
// User: Longo        Date: 9/17/04    Time: 3:32p
// Updated in $/FDMS/FDMS Application/FDMSNET/AppUl
// First integration of the MFC Upper Layer code
// FDMSReview.h: interface for the FDMSReview class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APPREVIEW_H__801C26A1_E8B1_407A_965D_8B814CDE1881__INCLUDED_)
#define AFX_APPREVIEW_H__801C26A1_E8B1_407A_965D_8B814CDE1881__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RTREVIEW.H"

class FDMSReview : public RTReview  
{
public:
	void SetMessagesOnHold(bool ProcessMsgs);
	FDMSReview();
	virtual ~FDMSReview();

	// Required through RTReview to provide the member functions to save all data
	BOOL RT_Review_My_Save(void);
				
	// Required through RTReview to provide the Review Exit procedure which is specific to operator review
	BOOL RT_Review_My_Exit(void); 

	// Required through RTReview to provide the Review Begin procedure which is specific to operator review
	RTReview::REVIEW_OPERATION RT_Review_Begin(void);

	//These methods must be overridden in order to process messages received by the upper layer for this application.
	BOOL RT_Display_My_Data(UINT uiEvent_Number);
	BOOL RT_Display_My_Data(const char *pszTool_Name, const char *pszTimestamp);
	BOOL RT_Display_My_Data(const char *pszTool_Name, const char *pszTimestamp, const char *pszLocation, BOOL *pbLocation_Failed);

	bool ActivateMeasurementDialog(int mindex);

};

#endif // !defined(AFX_APPREVIEW_H__801C26A1_E8B1_407A_965D_8B814CDE1881__INCLUDED_)
