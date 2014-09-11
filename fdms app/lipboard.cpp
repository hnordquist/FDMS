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
#include ".\lipboard.h"

Clipboard::Clipboard(void)
{
}

Clipboard::~Clipboard(void)
{
}


void Clipboard::CopyToClipboard(HWND h)
{
	if ( !OpenClipboard(h) )
	{
		MessageBox(NULL,  "Cannot open the Clipboard", "Error", MB_OK);
		return;
	}
	// Remove the current Clipboard contents
	if( !EmptyClipboard() )
	{
		MessageBox(NULL, "Cannot empty the Clipboard", "Error", MB_OK);
		return;
	}
	// ...
	// Get the currently selected data

/*************
	INCCRunTimeEnvironment::tEnvInfo info;
	GetEnvironmentInfoList(info);
	int tlen = 0;
	int i;


	const int ic = eMAXPATHCFGNAMES/2 + eMAXAPPINFONAMES;

	PTCHAR *foo = new PTCHAR[ic];
	for (i = 0; i < ic; i++)
	{
		foo[i] = new TCHAR[512];
		foo[i][0] = 0;
	}

	for (i = 0; i < (eMAXPATHCFGNAMES/2); i++)
	{
		sprintf(foo[i],"%s\t\t%s\t\t%s\t\t%s\r\n",
			info.pIniFile->m_Entry[i].name, 
			info.pIniFile->m_Entry[i].iniValue,  
			info.pIniFile->m_Entry[i].defIniValue,  
			info.pIniFile->m_Entry[i].szDesc);
		tlen += strlen(foo[i]);
	}
	for (i = 0; i < (eMAXAPPINFONAMES); i++)
	{
		sprintf(foo[i + (eMAXPATHCFGNAMES/2)],"%s\t\t%s\t\t%s\t\t%s\r\n",
			info.info[i].name, 
			info.info[i].iniValue,  
			info.info[i].defIniValue,  
			info.info[i].szDesc);    
		tlen += strlen(foo[i]);
	}



	// Allocate a global memory object for the text. 
	LPTSTR  lptstrCopy; 
	HGLOBAL hglbCopy; 
	hglbCopy = GlobalAlloc(GMEM_MOVEABLE, (tlen + 1) * sizeof(TCHAR)); 
	if (hglbCopy == NULL) 
	{ 
		goto cleanup;
	} 

	// Lock the handle and copy the text to the buffer. 
	lptstrCopy = (LPSTR)GlobalLock(hglbCopy);
	lptstrCopy[0] = (TCHAR) 0;    // null character 
	for (i = 0; i < ic; i++)
	{
		LPSTR b = foo[i];
		size_t l = strlen(b);
		strncat(lptstrCopy, b, l);
	}
	lptstrCopy[tlen] = (TCHAR) 0;    // null character 
	GlobalUnlock(hglbCopy); 

	// Place the handle on the clipboard. 

	// For the appropriate data formats...
	if ( ::SetClipboardData( CF_TEXT, hglbCopy ) == NULL )
	{
		MessageBox(NULL, "Unable to set Clipboard data", "Error", MB_OK);
		goto cleanup;
	}

cleanup:
	CloseClipboard();

	for (i = 0; i < ic; i++)
	{
		delete [] foo[i];
	}
	delete [] foo;
		*/
}