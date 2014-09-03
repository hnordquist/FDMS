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
