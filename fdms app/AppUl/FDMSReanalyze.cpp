// $History: FDMSReanalyze.cpp $ 
// 
// *****************  Version 2  *****************
// User: Longo        Date: 11/30/04   Time: 4:23p
// Updated in $/FDMS/FDMS Application/FDMSNET/AppUl
// working on import integration
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\FDMS.h" //REQT:  Change this to use the new application header.
#include "FDMSReanalyze.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FDMSReanalyze::FDMSReanalyze()
{

}

FDMSReanalyze::~FDMSReanalyze()
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Required through RTReanalyze to provide a Begin procedure which is specific to TRAIN TRACKER
////////////////////////////////////////////////////////////////////////////////////////////////////////
RTReanalyze::REANALYZE_OPERATION  FDMSReanalyze::RT_Reanalyze_Begin()
{
	Set_Messages_On_Hold(FALSE);	// Allow messages received via pipe commands to be processed.

    ImpReimpBegin (TRUE); // todo: jfl check here to see if app should exit or go into review mode

	return REANALYZE_REVIEW;	// default to normal exit
}

void FDMSReanalyze::SetMessagesOnHold(bool ProcessMsgs)
{
	Set_Messages_On_Hold(ProcessMsgs);	// Allow messages received via pipe commands to be processed.
}

// required by RT Review
BOOL FDMSReanalyze::RT_Reanalyze_Exit()
 { 
	return TRUE;
}