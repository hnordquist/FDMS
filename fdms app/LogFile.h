#pragma once


class CLogFile :
	public CStdioFile
{
public:
	CLogFile(tIniNames iFileCat);
	~CLogFile(void);

	tIniNames iCat;
	COleDateTime cNow;
	CString szCur;
	BOOL bGo;

	virtual void WriteString(LPCTSTR lpsz);

	static void SetLogLoc();
	
	static CString	szLogLocPath;


};
