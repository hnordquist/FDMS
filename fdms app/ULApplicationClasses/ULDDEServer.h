// ULDDEServer.h: interface for the ULDDEServer class.
// $History: ULDDEServer.h $ 
// 
// *****************  Version 2  *****************
// User: Longo        Date: 9/17/04    Time: 3:35p
// Updated in $/FDMS/FDMS Application/FDMSNET/ULApplicationClasses
// First integration of the MFC Upper Layer code
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ULDDESERVER_H__F7462B0E_0072_418B_A721_4502BA4A205C__INCLUDED_)
#define AFX_ULDDESERVER_H__F7462B0E_0072_418B_A721_4502BA4A205C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <ddeml.h>
#include "ULCallback.h"

typedef bool (ULCallBack::*UL_CB_RequestServerFn)(CString ItemName, CString *pReturnData);
typedef bool (ULCallBack::*UL_CB_ExecuteServerFn)(CString Cmd);


//Ugly scope.  Needs to be cleaned up.  These are from sample code in the DDE book.
void server_errfunc(int code);
typedef struct
  {
  char *p;
  unsigned len;
  } DATABUF;

typedef struct
  {
  HSZ item;
  int type;
  int fmt;
  union
    {
    HDDEDATA (*fn)(HSZ,HSZ);
    char *str;
    HDDEDATA ddedata;
    DATABUF *data;
    } action;
  } ITEM;

typedef struct
  {
  HSZ topic;
  ITEM * items;
  } TOPIC;

class ULDDEServer  
{
public:
	ULDDEServer(	const char *szServerName,	BOOL *pSuccess ,	CString *pErrMsg );
	virtual ~ULDDEServer();

	//Supporting methods.
	//Server callback function.
/*	HDDEDATA FAR PASCAL _export dde_cbk(UINT type,UINT fmt,
					HCONV conv, HSZ hsz1, HSZ hsz2,  HDDEDATA data,DWORD data1,    
					DWORD data2);
*/
	bool CreateTopic(const char *szTopicName, // Input: topic name known to prospective client
							  bool (*pRequestServerFn)(CString ItemName, CString* pReturnData),
							  bool (*pExecuteServerFn)(CString Cmd));
	bool CreateTopic( const char *szTopicName,	// I Topic name known to prospective client.
			ULCallBack *pCB_Obj,  //pointer to a callback class
			UL_CB_RequestServerFn pRequestServerFn,  //pointer to a REQUEST callback function in the callback class m_pCB_Obj
			UL_CB_ExecuteServerFn pExecuteServerFn); //pointer to a EXECUTE callback function in the callback class m_pCB_Obj

	/*static*/ bool FindCmd(CString FullCmd, char *naszCmds[], UINT *puiCmdIndex,
				CString *pCmdArgs = NULL,	BOOL bCaseSensitive = FALSE);

	//Not the best scope.  Should really have a function to get access to these names. <<FIX LATER>>
	bool (*m_pRequestServerFn)(CString ItemName, CString *pReturnData);
	bool (*m_pExecuteServerFn)(CString Cmd);

/*	static ULCallBack *m_pCB_Obj;
	static UL_CB_RequestServerFn m_pCBO_RequestServerFn;
	static UL_CB_ExecuteServerFn m_pCBO_ExecuteServerFn;
*/
private:
			BOOL mbValid;
//			UINT muiServerCount;  //should be static but could not get to compile/link
//kdm			static zDdeServer *mpDDE_Server;
//kdm	/*static*/ DWORD  dde_inst;  //from DDE book example
//	static FARPROC ddecb;  //pointer to the callback function for the server.
//kdm - from example code	static TOPIC *m_servertopics;

};

#endif // !defined(AFX_ULDDESERVER_H__F7462B0E_0072_418B_A721_4502BA4A205C__INCLUDED_)
