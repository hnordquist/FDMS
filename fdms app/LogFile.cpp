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
