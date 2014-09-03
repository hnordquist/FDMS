//////////////////////////////////////////////////////////////////////////
// Copyright 2004, The Regents Of the University of California.  This
// software was produced under U.S. Government contract (W-7405-ENG-36) at
// the Los Alamos National Laboratory by the Safeguards Science and
// Technology Group (N-1).  All Rights Reserved.
//////////////////////////////////////////////////////////////////////////
// $History: FDMSImpReimp.h $ 
// 
// *****************  Version 2  *****************
// User: Longo        Date: 12/10/04   Time: 4:44p
// Updated in $/FDMS/FDMS Application/FDMSNET/AppUl
// 
// *****************  Version 1  *****************
// User: Longo        Date: 11/29/04   Time: 2:01p
// Created in $/FDMS/FDMS Application/FDMSNET/AppUl
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "FDMSReanalyze.h"
#include "FDMSImport.h"

class FDMSImpReimp : public FDMSImport, public FDMSReanalyze
{
	public:
		
    FDMSImpReimp (void); 
    ~FDMSImpReimp (void); 

	void ImpReimpBegin (BOOL bReimport);

	void SetMessagesOnHold(bool ProcessMsgs);

};
