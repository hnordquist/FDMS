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