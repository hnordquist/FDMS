// $History: FDMSSetup.cpp $ 
// 
// *****************  Version 2  *****************
// User: Longo        Date: 11/30/04   Time: 4:23p
// Updated in $/FDMS/FDMS Application/FDMSNET/AppUl
// working on import integration
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\FDMS.h"  //REQT:  Change this to use the new application header.
#include "FDMSSetup.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FDMSSetup::FDMSSetup() 
{
	bAPP_Setup_Done = RTSetup::SETUP_REVIEW;
}

FDMSSetup::~FDMSSetup()
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Required through RTSetup to provide the Setup Begin procedure which is specific to the APPLICATION
////////////////////////////////////////////////////////////////////////////////////////////////////////

RTSetup::SETUP_OPERATION  FDMSSetup::RT_Setup_Begin()
 {
	return bAPP_Setup_Done;
}

 void FDMSSetup::SetMessagesOnHold(bool ProcessMsgs)
{
	Set_Messages_On_Hold(ProcessMsgs);	// Allow messages received via pipe commands to be processed.

}



// required by RT Review
BOOL FDMSSetup::RT_Setup_Exit() 
{ 
	return TRUE;
}