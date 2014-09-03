//////////////////////////////////////////////////////////////////////////
// Copyright 2004, The Regents Of the University of California.  This
// software was produced under U.S. Government contract (W-7405-ENG-36) at
// the Los Alamos National Laboratory by the Safeguards Science and
// Technology Group (N-1).  All Rights Reserved.
//////////////////////////////////////////////////////////////////////////
// $History: FDMSImpReimp.cpp $ 
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
#include "stdafx.h"
#include "..\FDMS.h"  //REQT:  Change this to use the new application header.
#include "FDMSImpReimp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FDMSImpReimp::FDMSImpReimp()
{

}

FDMSImpReimp::~FDMSImpReimp()
{

}

void FDMSImpReimp::ImpReimpBegin(BOOL /*bReimport*/) 
{
	CFDMSApp* pMe = ((CFDMSApp*)AfxGetApp());

}

void FDMSImpReimp::SetMessagesOnHold(bool ProcessMsgs)
{
	((FDMSImport*)this)->SetMessagesOnHold(ProcessMsgs);	// Allow messages received via pipe commands to be processed.
	((FDMSReanalyze*)this)->SetMessagesOnHold(ProcessMsgs);	// Allow messages received via pipe commands to be processed.

}


