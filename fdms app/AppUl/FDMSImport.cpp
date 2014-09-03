//////////////////////////////////////////////////////////////////////////
// Copyright 2004, The Regents Of the University of California.  This
// software was produced under U.S. Government contract (W-7405-ENG-36) at
// the Los Alamos National Laboratory by the Safeguards Science and
// Technology Group (N-1).  All Rights Reserved.
//////////////////////////////////////////////////////////////////////////
// $History: FDMSImport.cpp $
// 
// *****************  Version 4  *****************
// User: Longo        Date: 11/30/04   Time: 4:23p
// Updated in $/FDMS/FDMS Application/FDMSNET/AppUl
// working on import integration
// 
// *****************  Version 3  *****************
// User: Longo        Date: 11/24/04   Time: 3:43p
// Updated in $/FDMS/FDMS Application/FDMSNET/AppUl
// Some good things are working now

#include "stdafx.h"
#include "..\FDMS.h"  //REQT:  Change this to use the new application header.
#include "FDMSImport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FDMSImport::FDMSImport()
{

}

FDMSImport::~FDMSImport()
{

}

RTImport::IMPORT_OPERATION FDMSImport::RT_Import_Begin(BOOL /*bBypass_Import_Prompt*/) 
{
	Set_Messages_On_Hold(FALSE);	// Allow messages received via pipe commands to be processed.

	bAPP_Import_Done = IMPORT_REVIEW;	// default to normal exit

    ImpReimpBegin (FALSE); // todo: jfl check here to see if app should exit or go into review mode

	return bAPP_Import_Done;
}

BOOL FDMSImport::RT_Import_Exit(void) 
{
	Set_Messages_On_Hold(TRUE);	// Allow messages received via pipe commands to be processed.

	return TRUE;

}

void FDMSImport::SetMessagesOnHold(bool ProcessMsgs)
{
	Set_Messages_On_Hold(ProcessMsgs);	// Allow messages received via pipe commands to be processed.

}