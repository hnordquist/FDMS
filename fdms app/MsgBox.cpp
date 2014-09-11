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
#include "FDMS.h"
#include "msgbox.h"
#include "regwrapper.h"



IMPLEMENT_DYNAMIC(CMsgBox, CDialog)
CMsgBox::CMsgBox(CWnd* pParent /*=NULL*/)
	: CDialog(CMsgBox::IDD, pParent)
	, m_facility(_T(""))
	, m_id(_T(""))
	, m_enrichment(0)
	, m_burnup(0)
	, m_discharge_date(COleDateTime::GetCurrentTime())
	, m_cycle_number(0)
{
}
CMsgBox::~CMsgBox(void)
{
}


#define ASK_ONCE_KEY_NAME "ao"

#define GUI_NONE 0xf
#define GUI_YES 1
#define GUI_NO 0

int  CMsgBox::AskOnce (const char *MsgText)
{
    LONG lRecalledState = 0;
    TCHAR ebu[36];
    BOOL bSelected = 0;

    GenHash(MsgText, ebu);
    LONG lRes = CRegWrapper::GetPrivateIniInt(ASK_ONCE_KEY_NAME,(LPCTSTR)ebu,&lRecalledState);
    if (lRes != 0)
        DisplayErrorText(ASK_ONCE_KEY_NAME, lRes);

    if (lRecalledState == GUI_NONE)  // never asked before, or previously user did not check "do not ask this again box"
    {
	    int ret = AfxMessageBox(cString,MB_YESNO|MB_ICONQUESTION);
            lRecalledState = ret;

            if (bSelected && ret != 0) // save return value no matter what the value is, iff dont ask check box is checked
            {
                    CRegWrapper::SetPrivateIniInt(ASK_ONCE_KEY_NAME,(LPCTSTR)ebu,button);
            }

        }

    }
    
    return (GUI_ACTION)lRecalledState;
}

int  CMsgBox::Plain (const char *MsgText)
{
   return GUI_MsgBox(MsgText, Icon, Btn);
}

// make a hash value from the input string
void CMsgBox::SimpleGenHash(PTCHAR p, int maxLength)
{
	PTCHAR key = p;
	ULONG nHash = 0;
	int i = 0;
	while (*key && i < maxLength)
	{
		i++;
		nHash = (nHash<<5) + nHash + *key++;     // hints from MSFT
	}
	//convert uint to a string rep now
	ultoa (nHash, p, 10);
}

void CMsgBox::GenHash(const char *source, PTCHAR hash)
{
	static TCHAR shash[1024];
	ZeroMemory(&shash, 1024 * sizeof(TCHAR));

	memcpy(shash,  source, strlen(source));
	SimpleGenHash(shash, strlen(shash));
        strcpy(hash, shash);

}


void
CMsgBox::DisplayErrorText(
char* lpszFunction,
DWORD val
    )
{
    DWORD dwLastError = val;
    if (val == 0)
        val = GetLastError(); 
    HMODULE hModule = NULL; // default to system source
    LPSTR MessageBuffer;
    DWORD dwBufferLength;

    DWORD dwFormatFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_IGNORE_INSERTS |
        FORMAT_MESSAGE_FROM_SYSTEM ;

    //
    // If dwLastError is in the network range, 
    //  load the message source.
    //

/*    if(dwLastError >= NERR_BASE && dwLastError <= MAX_NERR) {
        hModule = LoadLibraryEx(
            TEXT("netmsg.dll"),
            NULL,
            LOAD_LIBRARY_AS_DATAFILE
            );

        if(hModule != NULL)
            dwFormatFlags |= FORMAT_MESSAGE_FROM_HMODULE;
    }
*/
    //
    // Call FormatMessage() to allow for message 
    //  text to be acquired from the system 
    //  or from the supplied module handle.
    //

    if(dwBufferLength = FormatMessageA(
        dwFormatFlags,
        hModule, // module to get message from (NULL == system)
        dwLastError,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // default language
        (LPSTR) &MessageBuffer,
        0,
        NULL
        ))
    {
        DWORD dwBytesWritten;

        //
        // Output message string on stderr.
        //
        WriteFile(
            GetStdHandle(STD_ERROR_HANDLE),
            MessageBuffer,
            dwBufferLength,
            &dwBytesWritten,
            NULL
            );

		char szBuf[512];
	    sprintf(szBuf, "%s failed: %s", 
        lpszFunction, MessageBuffer); 
    MessageBox(NULL, szBuf, "Error", MB_OK);

        //
        // Free the buffer allocated by the system.
        //
        LocalFree(MessageBuffer);
    }

    //
    // If we loaded a message source, unload it.
    //
    if(hModule != NULL)
        FreeLibrary(hModule);
}

void CMsgBox::ResetAllAskOnceVlaues(void)
{

    LONG lRes = CRegWrapper::DeleteAppSection(ASK_ONCE_KEY_NAME);
 
}

void reset_askonce(void)
{
    CMsgBox::ResetAllAskOnceVlaues();
}