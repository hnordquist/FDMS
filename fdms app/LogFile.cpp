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
#include "stdafx.h"
#include "fdms.h"
#include "dconfig.h"
#include ".\logfile.h"

#ifdef _XJDEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString CLogFile::szLogLocPath = "\\\\.\\logs";

void CLogFile::SetLogLoc()
{
	char temp[MAX_PATH];
	int len = GETPROFSTR(eLOGLOC,temp,sizeof(temp));
	if (len > 0)
	{
		szLogLocPath = temp;
	}
}

CLogFile::CLogFile(tIniNames iFileCat) : bGo(false)
{

	iCat = iFileCat;
	m_bCloseOnDelete = TRUE;
	// get the file name from the init file entry
	TCHAR fpn[MAX_PATH+1];
	DWORD res = GETPROFSTR(iCat, fpn, sizeof(fpn) - 1);

	if (res)  // create file only if indicated by entry in ini file
	{
		cNow = COleDateTime::GetCurrentTime();
		CString cNowString;
		cNowString = cNow.Format("%y%m%d%H%M%s");	

		CString dfpn = szLogLocPath;
		dfpn = dfpn + "\\";
		dfpn = dfpn + cNowString;
		dfpn = dfpn + fpn;
		dfpn = dfpn + ".log";

		bGo = Open(dfpn,modeReadWrite | modeCreate | typeText | shareDenyNone);

	}

}

CLogFile::~CLogFile(void)
{
}

void CLogFile::WriteString(LPCTSTR lpsz)
{
	if (bGo)
	{
		//GetSystemTime(&cNow);
		cNow = COleDateTime::GetCurrentTime();
		CString cNowString;
		cNowString = cNow.Format("%d%H%M%s\n");	
		//if (szCur.Compare(cNowString))
		CStdioFile::WriteString(cNowString);
		CStdioFile::WriteString(lpsz);
		CStdioFile::WriteString("\n");
		Flush();
	}
}
