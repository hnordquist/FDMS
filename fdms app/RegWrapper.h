#pragma once

class CRegWrapper
{
public:
    CRegWrapper(void);
    ~CRegWrapper(void);

    static LONG GetPrivateIniInt(char * pSection, LPCTSTR pName, LONG* plValue);
    static LONG SetPrivateIniInt(char * pSection, LPCTSTR pName, LONG lValue);

    static LONG GetPrivateIniString(char * pSection, LPCTSTR pName, PTCHAR pValue);
    static LONG SetPrivateIniString(char * pSection, LPCTSTR pName, PTCHAR pValue);

    static LONG GetAppKeyForWriting(const char* pSection, HKEY& hKey);
    static LONG CloseAppKey(HKEY hKey);
    static LONG DeleteAppSection(const char* pSection);

protected:

    static PTCHAR szAppRegKey;

    static LONG SetPrivateIniValue(char * pSection, LPCTSTR pName, DWORD dwType, BYTE *pValue, DWORD dwBufLen);
    static LONG GetPrivateIniValue(char * pSection, LPCTSTR pName, BYTE *pValue, DWORD dwBufLen);
    static LONG GetPrivateIniSZValue(char * pSection, LPCTSTR pName, BYTE *pValue, DWORD dwBufLen);


    static void QueryKey(HKEY hKey);

};
