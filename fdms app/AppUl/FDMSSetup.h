// $History: FDMSSetup.h $ 
// 
// *****************  Version 2  *****************
// User: Longo        Date: 11/30/04   Time: 4:23p
// Updated in $/FDMS/FDMS Application/FDMSNET/AppUl
// working on import integration
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APPSETUP_H__568CE7E9_279B_42A9_9B82_3C8C9EE15620__INCLUDED_)
#define AFX_APPSETUP_H__568CE7E9_279B_42A9_9B82_3C8C9EE15620__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RTSETUP.H"

class FDMSSetup : public RTSetup  
{
public:
	FDMSSetup();
	virtual ~FDMSSetup();

	// Required through RTSetup to provide the Setup Begin procedure which is specific to operator review
	RTSetup::SETUP_OPERATION RT_Setup_Begin(void);
	
	// Required through RTSetup to provide the Setup Exit procedure which is specific to operator review
	BOOL RT_Setup_Exit(void); 

	void SetMessagesOnHold(bool ProcessMsgs);

private:
            RTSetup::SETUP_OPERATION bAPP_Setup_Done;                            

};

#endif // !defined(AFX_APPSETUP_H__568CE7E9_279B_42A9_9B82_3C8C9EE15620__INCLUDED_)
