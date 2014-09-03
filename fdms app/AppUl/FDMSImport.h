//////////////////////////////////////////////////////////////////////////
// Copyright 2004, The Regents Of the University of California.  This
// software was produced under U.S. Government contract (W-7405-ENG-36) at
// the Los Alamos National Laboratory by the Safeguards Science and
// Technology Group (N-1).  All Rights Reserved.
//////////////////////////////////////////////////////////////////////////
// $History: FDMSImport.h $ 
// 
// *****************  Version 2  *****************
// User: Longo        Date: 11/30/04   Time: 4:23p
// Updated in $/FDMS/FDMS Application/FDMSNET/AppUl
// working on import integration
//
//////////////////////////////////////////////////////////////////////////


#pragma once

#include "common\RTIMPORT.H"

class FDMSImport : public RTImport  
{
public:
	FDMSImport();
	virtual ~FDMSImport();

	// Required through RTImport to provide the Import Begin procedure which is specific to the integrated tool
	IMPORT_OPERATION RT_Import_Begin(BOOL bBypass_Import_Prompt);

	// Required through RTImport to provide the Import Exit procedure which is specific to the integrated tool
	BOOL RT_Import_Exit(void); 

	virtual void ImpReimpBegin (BOOL bReimport) = 0;

	void SetMessagesOnHold(bool ProcessMsgs);

private:
	RTImport::IMPORT_OPERATION bAPP_Import_Done;                            

};

