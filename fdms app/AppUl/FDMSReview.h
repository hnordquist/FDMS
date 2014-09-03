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
