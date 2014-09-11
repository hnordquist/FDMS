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
//CColorButton Class Header File

#if !defined(COLORBUTTON)
#define COLORBUTTON

#include <winsock.h>

#define BELL_BIT	0x01
#define TIME_BIT	0x02
#define FLAG_BIT	0x04
#define OTHR_BIT	0x08
#define DISK_BIT	0x10
#define FLASH_BIT	0x20

static	WORD s_wVer;
//static	WSADATA s_WSAData;
//static	SOCKET s_sd_dg;
//static	struct sockaddr_in s_sAddr_Loc;
//static	struct sockaddr_in s_sAddr_Rem;
//static	int s_dDefPort;
//static  int s_iRc;
//static int m_dNetCount;
 
class CColorButton: public CButton {
public:
	CColorButton();//:CButton(){};
	BOOL Create( LPCTSTR lpszCaption, DWORD dwStyle, 
		         const RECT& rect, CWnd* pParentWnd, UINT nID );
	COLORREF SetFaceColor(UINT, bool redraw = true);
	COLORREF GetFaceColor();
	COLORREF SetTextColor(UINT, bool redraw = true);
	COLORREF SetFlagColor(UINT, bool redraw = true);
	int  GetNumeric();
	void SetFlash(bool set);
	void SetNumeric(int, bool redraw = true);
	void SetFlagSize(UINT, bool redraw = true);
	void SetFlag(bool set, bool redraw = true);
	void SetBell(bool set, bool redraw = true);
	void SetTime(bool set, bool redraw = true);
	void SetOthr(bool set, bool redraw = true);
	void SetDisk(bool set, bool redraw = true);
	void SetBase(int, bool redraw = true);
	void SetFlagChar(char ch, bool redraw = true);
	void SetBellChar(char ch, bool redraw = true);
	void SetTimeChar(char ch, bool redraw = true);
	void SetOthrChar(char ch, bool redraw = true);
	void SetDiskChar(char ch, bool redraw = true);
	void SetPreNum(char *txt, bool redraw = true);
	int GetState();
//	void ReCount(bool reset = false);
	void Redraw(BOOL redraw);

	void SetMultiLine(bool multi = false);
private:
	COLORREF face_color;
	COLORREF text_color;
	COLORREF flag_color;
	UINT     flag_size;
	char	 m_szPreNum[32];
	CFont    flag_font;
	CFont	*pflag_font;
	int      numeric;
//	int		 m_dFontSize;
	int		 base;
	char	 flag;
	char	 bell;
	char	 time;
	char	 othr;
	char	 space;
	char	 disk;
	bool	 m_bFlash;
	bool	 m_bFlashState;
	bool	 flag_flag;
	bool	 flag_bell;
	bool	 flag_time;
	bool	 flag_othr;
	bool	 flag_disk;
	bool	 m_bParticipateInNetwork;
	bool	 bMultiline;

	int		 m_dNetIndex;
	
	void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );
//};

//#endif
/////////////////////////////////////////////////////////////////////////////
// CColorButton window

//class CColorButton : public CButton
//{
// Construction
//public:
//	CColorButton();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColorButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorButton)
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
#endif
/////////////////////////////////////////////////////////////////////////////
