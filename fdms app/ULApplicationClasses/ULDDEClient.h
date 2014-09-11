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
•	Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
disclaimer. 
•	Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
disclaimer in the documentation and/or other materials provided with the distribution. 
•	Neither the name of Los Alamos National Security, LLC, Los Alamos National Laboratory, LANL, the U.S. Government, 
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

// ULDDEClient.h: interface for the ULDDEClient class.
// $History: ULDDEClient.h $ 
// 
// *****************  Version 2  *****************
// User: Longo        Date: 9/17/04    Time: 3:35p
// Updated in $/FDMS/FDMS Application/FDMSNET/ULApplicationClasses
// First integration of the MFC Upper Layer code
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ULDDECLIENT_H__FF97436E_9E66_4265_8E3F_68A25476662A__INCLUDED_)
#define AFX_ULDDECLIENT_H__FF97436E_9E66_4265_8E3F_68A25476662A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#include <windows.h>
#include <ddeml.h>

class ULDDEClient  
{
public:
	/*Constructor*/
	ULDDEClient(const char *szServerName, 	const char *szTopicName,	BOOL *pSuccess,	CString *pErrMsg);
	
	/*Destructor*/
	virtual ~ULDDEClient();
	
	//Supporting methods
	const char *GetServerName();
	BOOL Execute(const char *szKeyword,	const char *szParam, double dTimeOutSecs);
	BOOL Request(const char *szItemName, CString *pRetMsg,	double dTimeOutSecs);


private:
	BOOL mbValid;
	CString mServerName;
	HCONV  mpConversation;  
	HINSTANCE mhInst;
};

#endif // !defined(AFX_ULDDECLIENT_H__FF97436E_9E66_4265_8E3F_68A25476662A__INCLUDED_)
