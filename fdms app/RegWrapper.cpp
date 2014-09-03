//////////////////////////////////////////////////////////////////////////
// Copyright 2004, The Regents Of the University of California.  This
// software was produced under U.S. Government contract (W-7405-ENG-36) at
// the Los Alamos National Laboratory by the Safeguards Science and
// Technology Group (N-1).  All Rights Reserved.
//////////////////////////////////////////////////////////////////////////
// $History: RegWrapper.cpp $
// 
// *****************  Version 3  *****************
// User: Longo        Date: 11/24/04   Time: 3:48p
// Updated in $/FDMS/FDMS Application/FDMSNET
// * bring up select measurement dialog upon requests from UL
// * added matching class with logic: uses 182 day window to seek for
// match
// * compiler warning removal
// 
// *****************  Version 2  *****************
// User: Longo        Date: 10/20/04   Time: 1:10p
// Updated in $/FDMS/FDMS Application/FDMSNET
// registry failed today, saving all work
//////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "regwrapper.h"

PTCHAR CRegWrapper::szAppRegKey = "SOFTWARE\\LANL\\FDMS\\";

CRegWrapper::CRegWrapper(void)
{
}

CRegWrapper::~CRegWrapper(void)
{
}


// test software not used in product:
// QueryKey - Enumerates the subkeys of key, and the associated 
//    values, then copies the information about the keys and values 
//    into a pair of edit controls and list boxes. 
// hDlg - Dialog box that contains the edit controls and list boxes.
// hKey - Key whose subkeys and values are to be enumerated.
#define MAX_VALUE_NAME 256
void CRegWrapper::QueryKey(HKEY hKey) 
{ 
	CHAR     achKey[MAX_PATH]; 
	CHAR     achClass[MAX_PATH] = "";  // buffer for class name 
	DWORD    cchClassName = MAX_PATH;  // size of class string 
	DWORD    cSubKeys;                 // number of subkeys 
	DWORD    cbMaxSubKey;              // longest subkey size 
	DWORD    cchMaxClass;              // longest class string 
	DWORD    cValues;              // number of values for key 
	DWORD    cchMaxValue;          // longest value name 
	DWORD    cbMaxValueData;       // longest value data 
	DWORD    cbSecurityDescriptor; // size of security descriptor 
	DWORD    cbName;
	FILETIME ftLastWriteTime;      // last write time 

	DWORD i, j; 
	DWORD retCode, retValue; 

	CHAR  achValue[MAX_VALUE_NAME]; 
	DWORD cchValue = MAX_VALUE_NAME; 
	CHAR  achBuff[80]; 

	// Get the class name and the value count. 
	RegQueryInfoKey(hKey,        // key handle 
		achClass,                // buffer for class name 
		&cchClassName,           // size of class string 
		NULL,                    // reserved 
		&cSubKeys,               // number of subkeys 
		&cbMaxSubKey,            // longest subkey size 
		&cchMaxClass,            // longest class string 
		&cValues,                // number of values for this key 
		&cchMaxValue,            // longest value name 
		&cbMaxValueData,         // longest value data 
		&cbSecurityDescriptor,   // security descriptor 
		&ftLastWriteTime);       // last write time 

	//    SetDlgItemText(hDlg, IDE_CLASS, achClass); 
	//   SetDlgItemInt(hDlg, IDE_CVALUES, cValues, FALSE); 

	//   SendMessage(GetDlgItem(hDlg, IDL_LISTBOX), 
	//       LB_ADDSTRING, 0, (LONG) ".."); 

	// Enumerate the child keys, until RegEnumKeyEx fails. Then 
	// get the name of each child key and copy it into the list box. 

	SetCursor(LoadCursor(NULL, IDC_WAIT)); 
	for (i = 0, retCode = ERROR_SUCCESS; 
		retCode == ERROR_SUCCESS; i++) 
	{ 
		retCode = RegEnumKeyEx(hKey, 
			i, 
			achKey, 
			&cbName, 
			NULL, 
			NULL, 
			NULL, 
			&ftLastWriteTime); 
		if (retCode == (DWORD)ERROR_SUCCESS) 
		{
			//         SendMessage(GetDlgItem(hDlg, IDL_LISTBOX), 
			//             LB_ADDSTRING, 0, (LONG) achKey); 
		}
	} 
	SetCursor(LoadCursor (NULL, IDC_ARROW)); 

	// Enumerate the key values. 
	SetCursor(LoadCursor(NULL, IDC_WAIT)); 

	if (cValues) 
	{
		for (j = 0, retValue = ERROR_SUCCESS; 
			j < cValues; j++) 
		{ 
			cchValue = MAX_VALUE_NAME; 
			achValue[0] = '\0'; 
			DWORD dwType = 0;
			BYTE bData = 0;
			DWORD bcData = 0;
			retValue = RegEnumValue(hKey, j, achValue, 
				&cchValue, 
				NULL, 
				&dwType, 
				&bData, 
				&bcData );

			if (retValue == (DWORD) ERROR_SUCCESS ) 
			{ 
				achBuff[0] = '\0'; 

				// Add each value to a list box. 
				if (!lstrlen(achValue)) 
					lstrcpy(achValue, "<NO NAME>"); 
				wsprintf(achBuff, "%d) %s ", j, achValue); 
				//                SendMessage(GetDlgItem(hDlg,IDL_LISTBOX2), 
				//                    LB_ADDSTRING, 0, (LONG) achBuff); 
			} 
		}
	}

	SetCursor(LoadCursor(NULL, IDC_ARROW)); 
}


LONG CRegWrapper::GetPrivateIniValue(char * pSection, LPCTSTR pName, BYTE *pValue, DWORD dwBufLen)
{

	HKEY hKey;
	char szSub[256];
	LONG lRet;

	strcpy(szSub,szAppRegKey);
	strcat(szSub,pSection);

	lRet = RegOpenKeyEx( HKEY_CURRENT_USER,
		szSub, 0, KEY_READ, &hKey );
	if( lRet != ERROR_SUCCESS )
	{
		lRet = RegCreateKey(HKEY_CURRENT_USER, 
			szSub, &hKey);
		if( lRet != ERROR_SUCCESS )
			goto LBL_EXIT__;
	}

	//HKEY_CURRENT_USER\Software\LANL\INCC\ao

	//DWORD dwType;

	ZeroMemory(pValue, dwBufLen);

	lRet = RegQueryValueEx(hKey,pName,
		NULL,
		NULL,
		pValue,
		&dwBufLen);

	if( lRet != ERROR_SUCCESS )
	{
		RegCloseKey( hKey );
		lRet = RegOpenKeyEx( HKEY_CURRENT_USER,
			szSub, 0, KEY_WRITE, &hKey );
		if( lRet != ERROR_SUCCESS )
			goto LBL_EXIT__;

		lRet = RegSetValueEx(hKey,pName,
			NULL,
			REG_DWORD,  // type mismaytch, palce this in typed caller insated, case swtuich on return code ifor nonexisitn key
			pValue,
			dwBufLen);
		if( lRet != ERROR_SUCCESS )
			goto LBL_EXIT__;
	}

LBL_EXIT__:

	RegCloseKey( hKey );

	return lRet;

}

LONG CRegWrapper::GetPrivateIniSZValue(char * pSection, LPCTSTR pName, BYTE *pValue, DWORD dwBufLen)
{

	HKEY hKey;
	char szSub[256];
	LONG lRet;

	strcpy(szSub,szAppRegKey);
	strcat(szSub,pSection);

	lRet = RegOpenKeyEx( HKEY_CURRENT_USER,
		szSub, 0, KEY_READ, &hKey );
	if( lRet != ERROR_SUCCESS )
	{
		lRet = RegCreateKey(HKEY_CURRENT_USER, 
			szSub, &hKey);
		if( lRet != ERROR_SUCCESS )
			goto LBL_EXIT__;
	}

	//HKEY_CURRENT_USER\Software\LANL\INCC\ao

	//DWORD dwType;

	ZeroMemory(pValue, dwBufLen);

	lRet = RegQueryValueEx(hKey,pName,
		NULL,
		NULL,
		pValue,
		&dwBufLen);

	if( lRet != ERROR_SUCCESS )
	{
		RegCloseKey( hKey );
		lRet = RegOpenKeyEx( HKEY_CURRENT_USER,
			szSub, 0, KEY_WRITE, &hKey );
		if( lRet != ERROR_SUCCESS )
			goto LBL_EXIT__;

		lRet = RegSetValueEx(hKey,pName,
			NULL,
			REG_SZ,  // type mismaytch, palce this in typed caller insated, case swtuich on return code ifor nonexisitn key
			pValue,
			dwBufLen);
		if( lRet != ERROR_SUCCESS )
			goto LBL_EXIT__;
	}

LBL_EXIT__:

	RegCloseKey( hKey );

	return lRet;

}

LONG CRegWrapper::DeleteAppSection(const char* pSection)
{
	HKEY hKey;
	LONG lRes = GetAppKeyForWriting(NULL, hKey);
	if (lRes == ERROR_SUCCESS)
		lRes = RegDeleteKey(hKey,pSection);
	return lRes;
}

LONG CRegWrapper::GetAppKeyForWriting(const char* pSection, HKEY& hKey)
{
	char szSub[256];
	LONG lRet;

	strcpy(szSub,szAppRegKey);
	if (pSection != NULL)
		strcat(szSub,pSection);

	lRet = RegOpenKeyEx( HKEY_CURRENT_USER,
		szSub, 0, KEY_WRITE, &hKey );

	return lRet;
}

LONG CRegWrapper::CloseAppKey(HKEY hKey)
{
	return RegCloseKey( hKey );

}

LONG CRegWrapper::SetPrivateIniValue(char * pSection, LPCTSTR pName, DWORD dwType, BYTE *pValue, DWORD dwBufLen)
{

	HKEY hKey;
	char szSub[256];
	LONG lRet;

	strcpy(szSub,szAppRegKey);
	strcat(szSub,pSection);

	lRet = RegOpenKeyEx( HKEY_CURRENT_USER,
		szSub, 0, KEY_WRITE, &hKey );
	if( lRet != ERROR_SUCCESS )
	{
		lRet = RegCreateKey(HKEY_CURRENT_USER, 
			szSub, &hKey);
		if( lRet != ERROR_SUCCESS )
			goto LBL_EXIT__;
	}

	lRet = RegSetValueEx(hKey,pName,
		NULL,
		dwType,
		pValue,
		dwBufLen);

LBL_EXIT__:

	RegCloseKey( hKey );

	return lRet;
}

LONG CRegWrapper::GetPrivateIniInt(char * pSection, LPCTSTR pName, LONG* plValue)
{
	LONG lRet;
	DWORD dwBufLen=sizeof(LONG);
	*plValue = 0;
	lRet = GetPrivateIniValue(pSection, pName, (LPBYTE)plValue, dwBufLen);
	return lRet;
}

LONG CRegWrapper::SetPrivateIniInt(char * pSection, LPCTSTR pName, LONG lValue)
{
	LONG lRet;
	DWORD dwBufLen=sizeof(LONG);
	lRet = SetPrivateIniValue(pSection, pName, REG_DWORD, (LPBYTE)&lValue, dwBufLen);
	return lRet;
}

LONG CRegWrapper::GetPrivateIniString(char * pSection, LPCTSTR pName, PTCHAR pValue)
{
	LONG lRet;
	DWORD dwBufLen=strlen(pValue);
	*pValue = 0;
	lRet = GetPrivateIniValue(pSection, pName, (LPBYTE)pValue, dwBufLen);
	return lRet;
}
LONG CRegWrapper::SetPrivateIniString(char * pSection, LPCTSTR pName, PTCHAR pValue)
{
	LONG lRet;
	DWORD dwBufLen=strlen(pValue);
	lRet = SetPrivateIniValue(pSection, pName, REG_SZ, (LPBYTE)pValue, dwBufLen);
	return lRet;
}


