#pragma once

class CMsgBox
{
public:
    CMsgBox(void);
    ~CMsgBox(void);

    static int  AskOnce (const char *MsgText);

    static int  Plain (const char *MsgText);

    static void ResetAllAskOnceVlaues(void);

    // utility, move this somewhere else ....
    static void DisplayErrorText(char* lpszFunction, DWORD val = 0);

protected:
    static void GenHash(const char *source, PTCHAR hash);
    static void SimpleGenHash(PTCHAR p, int maxLength);

};
