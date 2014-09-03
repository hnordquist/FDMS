//////////////////////////////////////////////////////////////////////////
// Copyright 2004, The Regents Of the University of California.  This
// software was produced under U.S. Government contract (W-7405-ENG-36) at
// the Los Alamos National Laboratory by the Safeguards Science and
// Technology Group (N-1).  All Rights Reserved.
//////////////////////////////////////////////////////////////////////////
// $History: FDMSReanalyze.h $ 
// 
// *****************  Version 2  *****************
// User: Longo        Date: 11/30/04   Time: 4:23p
// Updated in $/FDMS/FDMS Application/FDMSNET/AppUl
// working on import integration
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "common\RTREANALYZE.H"

class FDMSReanalyze : public RTReanalyze  
{
public:
	FDMSReanalyze();
	virtual ~FDMSReanalyze();

	// Required through RTReanalyze to provide the Begin procedure which is specific to the integrated tool
	REANALYZE_OPERATION RT_Reanalyze_Begin();
	
	// Required through RTReanalyze to provide the Import Exit procedure which is specific to the integrated tool
	BOOL RT_Reanalyze_Exit(void); 

	virtual void ImpReimpBegin (BOOL bReimport) = 0;

	void SetMessagesOnHold(bool ProcessMsgs);

};

