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
�	Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
disclaimer. 
�	Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
disclaimer in the documentation and/or other materials provided with the distribution. 
�	Neither the name of Los Alamos National Security, LLC, Los Alamos National Laboratory, LANL, the U.S. Government, 
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


