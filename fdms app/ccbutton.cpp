//CColorButton Class
#include "stdafx.h"
#include "ccbutton.h"

//extern HANDLE g_hSMS_Slot;
//extern bool g_bDoingNetwork;
//the "real" work of displaying the owner draw button
//this function is called when the button is redrawn
void 
CColorButton::DrawItem( LPDRAWITEMSTRUCT lpDI ) {
	CRect r1(lpDI->rcItem);//rectangle for up/down effect
	CRect r2(lpDI->rcItem);//rectangle for fill color
	CRect r3(lpDI->rcItem);//rectangle for focus 
	CRect r4(lpDI->rcItem);//rectangle for flags
	CRect r5(0,0,0,0);     //temp working rectangle 
	CRect r6(0,0,0,0);     //temp working rectangle 
	r1.InflateRect(-1,-1); //rectangle for up/down effect
	r2.InflateRect(-2,-2); //rectangle for fill color
	r3.InflateRect(-5,-5); //rectangle for focus 
	r4.InflateRect(-3,-3); //rectangle for flags
	char text[255];

	//Draw whole button
	if ((lpDI->itemAction & ODA_DRAWENTIRE) ||
		(lpDI->itemAction & ODA_SELECT))
	{

		//get a dc from handle hDC
		CDC dc;
		dc.m_hDC = lpDI->hDC;

		//save the current state
		int saveddc = dc.SaveDC();

		//create the text 
		CButton::GetWindowText(text,sizeof(text)-1);
//		if (base > 0)
//		{
//			char tempstr[17];
//			itoa(numeric,tempstr,base);
//			strcat(text,"\n");
//			strcat(text,m_szPreNum);
//			strcat(text,tempstr);
//		}

		//force black but could enhance to settable color
		dc.SetTextColor(text_color);
		//make sure the background shows through the text
		dc.SetBkMode(TRANSPARENT);

		//create and select a solid brush at the current color
		CBrush sb, *rent;
		if (m_bFlashState && m_bFlash)
			sb.CreateSolidBrush(RGB(255,255,255));
		else
			sb.CreateSolidBrush(face_color);
		rent = dc.SelectObject(&sb);

		//draw an outline in black
		if (lpDI->itemState & ODS_FOCUS)
			dc.Draw3dRect(&lpDI->rcItem,RGB(0,0,0),RGB(0,0,0));
		else
			dc.Draw3dRect(&lpDI->rcItem,::GetSysColor(COLOR_BTNHILIGHT),::GetSysColor(COLOR_BTNSHADOW));
			

		//draw the raised or lowered edge of button
		if (lpDI->itemState & ODS_SELECTED)
		{
		  dc.Draw3dRect(r1,::GetSysColor(COLOR_BTNSHADOW),
		                   ::GetSysColor(COLOR_BTNHILIGHT));
//		  dc.Draw3dRect(r2,::GetSysColor(COLOR_BTNSHADOW),
//		                   ::GetSysColor(COLOR_BTNHILIGHT));
		}
		else
		{
		  dc.Draw3dRect(r1,::GetSysColor(COLOR_BTNHILIGHT), 
		                   ::GetSysColor(COLOR_BTNSHADOW));
//		  dc.Draw3dRect(r2,::GetSysColor(COLOR_BTNHILIGHT), 
//		                   ::GetSysColor(COLOR_BTNSHADOW));
		}

		//fill with the desired brush/color
		dc.FillRect(r2,&sb);
		//calculate the bounding box of the text
		//dc.DrawText(text, strlen(text), &r6, DT_CALCRECT);

		//center/top the box in the button
//		CRect r5((r2.right-r6.right)/2,
//			3,
//			(r2.right-r6.right)/2+r6.right,
//			3+r6.bottom);
		CRect r5;
		r5 = r4;


		//offset the position of the text box if selected
		if (lpDI->itemState & ODS_SELECTED) {
			r5.right += 2;
			r5.bottom += 2;
			r5.top += 2;
			r5.left += 2;
		}

		//if the BBM: text will fit then use it
//		if ((r6.right-r6.left)<(r3.right-r3.left))
			//draw the button's text
//			dc.DrawText(text, strlen(text), &r5, DT_CENTER|DT_SINGLELINE   |DT_VCENTER   );
			dc.DrawText(text, strlen(text), &r5,
				(bMultiline ? DT_TOP : DT_SINGLELINE) | DT_CENTER |	DT_VCENTER | DT_WORDBREAK);
//		else//DT_WORDBREAK
//			dc.DrawText(alttext,strlen(alttext), &r5, DT_CENTER|DT_WORDBREAK);
		if (lpDI->itemState & ODS_DISABLED) {
			r4 = r5;
			r4.InflateRect(-2,-2,2,2);
			dc.SetTextColor(RGB(255,255,255));
			dc.DrawText(text,strlen(text),&r4,
				(bMultiline ? DT_TOP : DT_SINGLELINE) | DT_CENTER |	DT_VCENTER | DT_WORDBREAK);
			r4.InflateRect(1,1,-1,-1);
			dc.SetTextColor(face_color);
			dc.DrawText(text,strlen(text),&r4,
				(bMultiline ? DT_TOP : DT_SINGLELINE) | DT_CENTER |	DT_VCENTER | DT_WORDBREAK);
		}

		//draw the button's special flags
		//concat all the selected flags
//		if (flag_flag || flag_bell || flag_time || flag_othr || flag_disk) {
//			int j = 0;
//			if (flag_flag)
//			{
//				text[j]=flag;
//				j++;
//			}
//			if (flag_bell)
//			{
//				text[j]=bell;
//				j++;
//			}
//			if (flag_time)
//			{
//				text[j]=time;
//				j++;
//			}
//			if (flag_disk)
//			{
//				text[j]=disk;
//				j++;
//			}
//			if (flag_othr)
//			{
//				text[j]=othr;
//				j++;
//			}
//			text[j] = NULL;
//
//			dc.SelectObject(pflag_font);
//			if (face_color==flag_color)
//				dc.SetTextColor(RGB(0,0,0));
//			else
//				dc.SetTextColor(flag_color);
//
//			dc.DrawText(text,j,
//				CRect(r4.left,r4.bottom-flag_size,
//				r4.right,r4.bottom),
//				DT_CENTER);
//
//		}

		dc.SelectObject(rent);
		//restore all of the dc's condition
		dc.RestoreDC(saveddc);
		if (lpDI->itemState & ODS_FOCUS)
			DrawFocusRect(lpDI->hDC,r3);

		//make sure our dc never gets used
		dc.m_hDC = NULL;
		return;
	}

	//Focus changing
	if (lpDI->itemAction & ODA_FOCUS) {
		//only need to add or drop the focus rectangle
		//if it is already displayed redrawing it will
		//remove it.
		DrawFocusRect(lpDI->hDC, r3);
		return;
	}
}

//set the "flag" icon 
//used to indicate comm error occured
void
CColorButton::SetFlag(bool set, bool redraw) {
	flag_flag = set;
	if (redraw) Redraw(false);
}

void
CColorButton::SetPreNum(char *txt, bool redraw) {
	if (strlen(txt) < sizeof(m_szPreNum))
		strcpy(m_szPreNum,txt);
	if (redraw) Redraw(false);
}

void
CColorButton::SetMultiLine(bool multi)
{
	bMultiline = multi;
}

void
CColorButton::Redraw(BOOL redraw)
{
/*
	//send packet
	typedef struct senddata {
		int BBM;
		int Index;
		int	TotalNetButton;
		int Base;
		COLORREF FaceColor;
		bool Flash;
		bool Flag;
		bool Bell;
		bool Time;
		bool Other;
		bool Disk;
		char FlagC;
		char BellC;
		char TimeC;
		char OtherC;
		char DiskC;
		char Title[32];
		char PreNum[32];
	} SendDataType;

	if (g_bDoingNetwork && m_bParticipateInNetwork )
	{
		SendDataType SendData;
		memset(&SendData,NULL,sizeof(SendData));

		SendData.BBM = numeric;
		SendData.Index = m_dNetIndex;
		SendData.TotalNetButton = m_dNetCount;
		SendData.Base = base;
		SendData.FaceColor = face_color;
		SendData.Flash = m_bFlash;
		SendData.Flag = flag_flag;
		SendData.Bell = flag_bell;
		SendData.Time = flag_time;
		SendData.Other= flag_othr;
		SendData.Disk = flag_disk;
		SendData.FlagC = flag;
		SendData.BellC = bell;
		SendData.TimeC = time;
		SendData.OtherC= othr;
		SendData.DiskC = disk;

		//limit the number of characters we try
		//to place in SendData.Title to 32 or less!
		CButton::GetWindowText(SendData.Title,31);
		if (base <=0)
			SendData.PreNum[0] = NULL;			
		else
			strncpy(SendData.PreNum,m_szPreNum,31);

		s_sAddr_Rem.sin_family = PF_INET;
		s_sAddr_Rem.sin_port = (unsigned short)(htons((unsigned short)s_dDefPort));
		char temp1[4];
		char temp2[32];
		int i = 1;
		itoa(i,temp1,10);
		*!GetPrivatProfileString("NETWORK",temp1,"",temp2,sizeof(temp2),g_cIniFile);
		while (strlen(temp2) > 0)
		{
			s_sAddr_Rem.sin_addr.s_addr = inet_addr(temp2);
			sendto(s_sd_dg,(char*)&SendData,sizeof(SendData),0,
				(struct sockaddr FAR *) &s_sAddr_Rem,sizeof(s_sAddr_Rem));
			i++;
			itoa(i,temp1,10);
			*!GetPrivatProfileString("NETWORK",temp1,"",temp2,sizeof(temp2),g_cIniFile);
		}

		if (g_hSMS_Slot)
		{
			DWORD NumBytesWritten;
			WriteFile(g_hSMS_Slot,
				&SendData, sizeof(SendData),
				&NumBytesWritten,
				(LPOVERLAPPED) NULL);
		}

	}
*/
	//invalidate the rectangle
	Invalidate(redraw);
}

//set the "disk" icon 
//used to indicate cannot write to disk
void
CColorButton::SetDisk(bool set, bool redraw){
	flag_disk = set;
	if (redraw) Redraw(false);
}

//set the "bell" icon
//used to indicate currently in MII
void
CColorButton::SetBell(bool set, bool redraw) {
	flag_bell = set;
	if (redraw) Redraw(false);
}

//set the "clock" icon
//used to indicate difference between MIC and instrument 
//is larger than acceptable
void
CColorButton::SetTime(bool set, bool redraw) {
	flag_time = set;
	if (redraw) Redraw(false);
}

//set the "frown face" icon
//used to indicate problem with memory or power
void
CColorButton::SetOthr(bool set, bool redraw) {
	flag_othr = set;
	if (redraw) Redraw(false);
}

//set the base of the numeric display
void
CColorButton::SetBase(int ibase, bool redraw) {
	base = ibase;
	if (redraw) Redraw(false);
}

//turn on or off flashing button
void
CColorButton::SetFlash(bool set)
{
	if (set)
	{
		m_bFlash = true;
		m_bFlashState = true;
		SetTimer(1,500,NULL);
		Redraw(true);
	}
	else
	{
		KillTimer(1);
		m_bFlash = false;
		m_bFlashState = false;
		Redraw(true);
	}
}

//constructor
CColorButton::CColorButton():CButton(){
	//set default face and text
	face_color = GetSysColor(COLOR_BTNFACE);
	text_color = GetSysColor(COLOR_BTNTEXT);
	flag_color = RGB(255,0,0);
	flag_size  = 32;
	flag_bell  = false;
	flag_time  = false;
	flag_flag  = false;
	flag_othr  = false;
	flag_disk  = false;
	m_bFlash   = false;
//	m_bParticipateInNetwork = ParticipateInNet;
//	if (ParticipateInNet)
//		m_dNetIndex = m_dNetCount++;
	pflag_font = NULL;

	flag = 'O';//flag character in wingdings
	bell = '%';//bell character in wingdings
	time = '�';//clock character in wingdings
	othr = 'L';//other character in wingdings(frown face)
	disk = '<';//disk character in wingdings
	space = ' ';

	numeric = 0;
	base = 10;
	bMultiline = false;
}

int
CColorButton::GetState()
{
	int ch = NULL;
	if (flag_bell) ch = ch | BELL_BIT;
	if (flag_time) ch = ch | TIME_BIT;
	if (flag_flag) ch = ch | FLAG_BIT;
	if (flag_othr) ch = ch | OTHR_BIT;
	if (flag_disk) ch = ch | DISK_BIT;
	if (m_bFlash)  ch = ch | FLASH_BIT;
	return ch;
}

//void
//CColorButton::ReCount(bool reset)
//{
//	//called during recounting of network
//	//participating buttons
//	if (reset) 
//		m_dNetCount = 0;
//	else if (m_bParticipateInNetwork)
//		m_dNetIndex = m_dNetCount++;
//}

//create and initialize
BOOL 
CColorButton::Create(LPCTSTR lpszCaption, 
		  DWORD dwStyle, 
		  const RECT& rect, 
		  CWnd* pParentWnd, 
		  UINT nID ) 
{
	//get wingdings font for special flags
	pflag_font = new CFont;
	pflag_font->CreateFont(flag_size,0,0,0,FW_BOLD,
		0,0,0,1,0,0,0,0,"Wingdings");

	strcpy(m_szPreNum,"BBM: ");

	//create the button and force OWNER DRAW
	return CButton::Create(lpszCaption, dwStyle | BS_OWNERDRAW, 
		rect, pParentWnd, nID );
}

//set the text color and redraw the button
COLORREF
CColorButton::SetTextColor(UINT color, bool redraw) 
{
	COLORREF temp = text_color;
	text_color = color;
	if (redraw) Redraw(false);
	return temp;
}

//set the face color and redraw the button
COLORREF
CColorButton::SetFaceColor(UINT color, bool redraw) 
{
	COLORREF temp;
	try
	{
		temp = face_color;
		face_color = color;
		if (redraw) Redraw(false);
	}
	catch(...)
	{}
	return temp;
}

//get the current face color
COLORREF
CColorButton::GetFaceColor(){
	return face_color;
}

//set the face color and redraw the button
COLORREF
CColorButton::SetFlagColor(UINT color, bool redraw) {
	COLORREF temp = flag_color;
	flag_color = color;
	if (redraw) Redraw(false);
	return temp;
}

//set the flag size
void
CColorButton::SetFlagSize(UINT size, bool redraw) {
	flag_size = size;
	//get wingdings font for special flags
	if (pflag_font) 
	{
		delete pflag_font;
		pflag_font = new CFont;
		pflag_font->CreateFont(flag_size,0,0,0,FW_BOLD,
			0,0,0,1,0,0,0,0,"Wingdings");
	}
	if (redraw) Redraw(false);
}

//set the numeric value
void
CColorButton::SetNumeric(int number, bool redraw) {
	numeric = number;
	if (redraw) Redraw(false);
}

//get the current numberic value
int
CColorButton::GetNumeric()
{
	return numeric;
}

//set the "flag" character to something other
//than the current (default) setting
void 
CColorButton::SetFlagChar(char cflag, bool redraw){
	flag = cflag;
	if (redraw) Redraw(false);
}

//set the "disk" character to someting other
//than the current (default) setting
void
CColorButton::SetDiskChar(char cdisk, bool redraw){
	disk = cdisk;
	if (redraw) Redraw(false);
}

//set the "bell" character to someting other
//than the current (default) setting
void 
CColorButton::SetBellChar(char cbell, bool redraw) {
	bell = cbell;
	if (redraw) Redraw(false);
}

//set the "clock" character to someting other
//than the current (default) setting
void 
CColorButton::SetTimeChar(char ctime, bool redraw) {
	time = ctime;
	if (redraw) Redraw(false);
}

//set the "frown face" character to someting other
//than the current (default) setting
void 
CColorButton::SetOthrChar(char cothr, bool redraw) {
	othr = cothr;
	if (redraw) Redraw(false);
}


/////////////////////////////////////////////////////////////////////////////
// CColorButton

CColorButton::~CColorButton()
{
	delete pflag_font;
}


BEGIN_MESSAGE_MAP(CColorButton, CButton)
	//{{AFX_MSG_MAP(CColorButton)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorButton message handlers

void CColorButton::OnTimer(UINT nIDEvent) 
{
	//redraw the button at the flash rate
	m_bFlashState = !m_bFlashState;	
	Invalidate(true);
	CButton::OnTimer(nIDEvent);
}

