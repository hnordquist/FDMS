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
// XListCtrlTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\FDMS.h"
#include "XListCtrlTestDlg.h"

#ifdef _XJDEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define INVALIDATETIMER 2500

///////////////////////////////////////////////////////////////////////////////
// XListCtrlDlg dialog

BEGIN_MESSAGE_MAP(XListCtrlDlg, CDialog)
//{{AFX_MSG_MAP(XListCtrlDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_DESTROY()
	//ON_NOTIFY(NM_CLICK, IDC_LIST1, OnClick)
	//ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblClick)
	//ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, OnColumnClick)
	//ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnItemChanged)
	ON_MESSAGE( MY_SUBEDIT_FINISHED,  OnSubEditEnd)
	ON_MESSAGE( MY_BOX_DBLCLICK,  OnBoxDblClick)
	//}}AFX_MSG_MAP
	//ON_REGISTERED_MESSAGE(WM_XLISTCTRL_COMBO_SELECTION, OnComboSelection)
	//ON_REGISTERED_MESSAGE(WM_XLISTCTRL_CHECKBOX_CLICKED, OnCheckbox)
	ON_BN_CLICKED(ID_ANALYZE_IMP, OnBnClickedAnalyzeImp)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	//ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)

END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// ctor
XListCtrlDlg::XListCtrlDlg(bool bAllNew, CObList *pList, CWnd* pParent /*=NULL*/) 
: CDialog(XListCtrlDlg::IDD, pParent), m_bAllNew(bAllNew), m_pEvList(pList), m_iSelected(-1), m_bInitialized(false), 
m_StatusIconIdx(3), m_NQStatusIconIdx(11)  // IMAGE WORK, all dialogs of this class use a 'checkbox on the left' scheme, all status indices must be shifted by three (3)?
{
	//{{AFX_DATA_INIT(XListCtrlDlg)
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

///////////////////////////////////////////////////////////////////////////////
// DoDataExchange
void XListCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	//
	// comment out the DDX_Control line to create XListCtrl dynamically -
	// remember also to include lines in OnInitDialog to create XListCtrl,
	// and remove the list control from IDD_LISTCTRLTEST_DIALOG in the rc file
	//
	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(XListCtrlDlg)
	DDX_Control(pDX, IDC_LIST1, m_List);	// comment out this line if creating dynamically
	//}}AFX_DATA_MAP
}

ImpEditCol* XListCtrlDlg::m_pSelectColumns;

CField* XListCtrlDlg::GetColScheme(ImpEditCol::ColumnID iColumnID)
{
	CField* pf = ((CField*)(m_pSelectColumns->GetAt(iColumnID)));
	return pf;
}

void XListCtrlDlg::MakeFieldMap()
{
	if (m_pSelectColumns == NULL)
	{
		m_pSelectColumns = new ImpEditCol();
		m_pSelectColumns->Reconstruct();
	}
}

///////////////////////////////////////////////////////////////////////////////
// OnInitDialog
BOOL XListCtrlDlg::OnInitDialog()
{
	TRACE0("in XListCtrlDlg::OnInitDialog\n");

	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//	when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE); 		// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	//
	// include the following lines to create XListCtrl dynamically -
	// remember also to comment out DDX line, and remove the list control
	// from IDD_LISTCTRLTEST_DIALOG in the rc file
	//
	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

#if 0  // -----------------------------------------------------------
	DWORD dwStyle = LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS |
		WS_CHILD | WS_VISIBLE | WS_GROUP | WS_TABSTOP;

	CRect rect, rectbutton;
	GetClientRect(&rect);
	GetDlgItem(IDC_BUTTON1)->GetClientRect(&rectbutton);
	rect.DeflateRect(5, 5);
	rect.top = rectbutton.bottom + 15;

	TRACE("   calling CreateEx\n");

	// you may also use Create(), but I prefer WS_EX_CLIENTEDGE style
	VERIFY(m_List.CreateEx(WS_EX_CLIENTEDGE, _T("SysListView32"), _T(""),
						dwStyle, rect, this, IDC_LIST, NULL));
	CFont *pFont = GetFont();
	if (pFont)
		m_List.SetFont(pFont);
#endif // ----------------------------------------------------------e-

	// XListCtrl must have LVS_EX_FULLROWSELECT if combo boxes are used
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_BORDERSELECT);

	myfont.CreateFont(
		18,                        // nHeight // TODO: font choice should be for user
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"Times");

	// call EnableToolTips to enable tooltip display
	m_List.EnableToolTips(TRUE);

	InitListCtrl(m_List);

	m_List.SetFont(&myfont);
	FillListCtrl(m_List);

	GetClientRect(&m_OldRect);
	m_bInitialized = true;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

///////////////////////////////////////////////////////////////////////////////
// OnSysCommand
void XListCtrlDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
}

///////////////////////////////////////////////////////////////////////////////
// OnPaint
void XListCtrlDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

///////////////////////////////////////////////////////////////////////////////
// OnQueryDragIcon
HCURSOR XListCtrlDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

///////////////////////////////////////////////////////////////////////////////
// OnTimer
void XListCtrlDlg::OnTimer(UINT nIDEvent)
{
	//TRACE0("in XListCtrlDlg::OnTimer\n");
	static int nPct = 0;
	static int nItem = -1;
	static int nBlink = 0;

	if (nIDEvent == 1)
	{
		KillTimer(nIDEvent);
		m_List.SetProgress(1, 3);
		nPct = 0;
		SetTimer(2, 80, NULL);
		SetTimer(4, 400, NULL);
	}
	else if (nIDEvent == 2)
	{
		nPct += 10;
		m_List.UpdateProgress(1, 3, nPct);
		if (nPct >= 100)
		{
			KillTimer(nIDEvent);
			SetTimer(3, 2000, NULL);
		}
	}
	else if (nIDEvent == 3)
	{
		CString str;
		str = m_List.GetComboText(0, 2);

		KillTimer(nIDEvent);
		m_List.DeleteProgress(1, 3);
		SetTimer(1, 2000, NULL);
	}
	else if (nIDEvent == 4)
	{
		if (nItem != -1)
		{
			// remove previous color & bold from columns 1 and 5
			m_List.SetItemText(nItem, 5, NULL, (COLORREF)-1, (COLORREF)-1);
			m_List.SetBold(nItem, 1, FALSE);
		}

		// blink item 4
		if (nItem == 4)
		{
			nBlink++;
			if (nBlink >= 12)
			{
				nBlink = 0;
				nItem++;
				m_List.SetItemText(nItem, 5, NULL, RGB(255,255,255), RGB(255,0,0));
				m_List.SetBold(nItem, 1, TRUE);
			}
			else
			{
				if (nBlink & 1)		// blink every other time
				{
					m_List.SetItemText(nItem, 5, NULL, RGB(255,255,255), RGB(255,0,0));
					m_List.SetBold(nItem, 1, TRUE);
				}
			}
		}
		else
		{
			nItem++;
			if (nItem >= m_List.GetItemCount())
				nItem = 0;
			m_List.SetItemText(nItem, 5, NULL, RGB(255,255,255), RGB(255,0,0));
			m_List.SetBold(nItem, 1, TRUE);
		}
	}
	else if(nIDEvent == INVALIDATETIMER)
	{
		Invalidate(TRUE);
		KillTimer(INVALIDATETIMER);
	}
}

void XListCtrlDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	if(m_bInitialized && cx > 0 && cy > 130)
	{
		KillTimer(INVALIDATETIMER);
		int iY_Offset = cy - m_OldRect.bottom;
		int iX_Offset = cx - m_OldRect.right;

		CRect rect;
		CWnd *item;
		item = GetDlgItem(IDC_LIST1);
		item->GetWindowRect(&rect);
		ScreenToClient(&rect);
		item->MoveWindow(rect.left, rect.top,rect.Width() + iX_Offset, 
			rect.Height() + iY_Offset,TRUE);

		item = GetDlgItem(IDOK);
		item->GetWindowRect(&rect);
		ScreenToClient(&rect);
		item->MoveWindow(rect.left + iX_Offset, rect.top + iY_Offset, rect.Width(),
			rect.Height(),TRUE);

		item = GetDlgItem(IDCANCEL);
		item->GetWindowRect(&rect);
		ScreenToClient(&rect);
		item->MoveWindow(rect.left + iX_Offset, rect.top + iY_Offset,rect.Width(),
			rect.Height(),TRUE);

		item = GetDlgItem(ID_ANALYZE_IMP);
		item->GetWindowRect(&rect);
		ScreenToClient(&rect);
		item->MoveWindow(rect.left, rect.top + iY_Offset,rect.Width(),
			rect.Height(),TRUE);

		GetClientRect(&m_OldRect);
		//invalidate
		SetTimer(INVALIDATETIMER,100,NULL);
	}

}
///////////////////////////////////////////////////////////////////////////////
// OnButton1
void XListCtrlDlg::OnButton1()
{
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	FillListCtrl(m_List);
	m_List.SetEnabled(3, FALSE);
	m_List.SetItemText(3, 1, _T("This row is disabled"));

#if 0  // -----------------------------------------------------------
	// demonstrate handling of WM_SYSCOLORCHANGE
	int nElement = COLOR_WINDOW;
	COLORREF crWindow = ::GetSysColor(nElement);
	if (::SetSysColors(1,				// number of elements
					   &nElement,		// array of elements
					   &crWindow))		// array of RGB values
	{
		TRACE(_T("SetSysColors ok\n"));
	}
	else
	{
		TRACE(_T("SetSysColors failed\n"));
	}
#endif // -----------------------------------------------------------

	SetTimer(1, 1000, NULL);
}

///////////////////////////////////////////////////////////////////////////////
// OnDestroy
void XListCtrlDlg::OnDestroy()
{
	KillTimer(1);
	KillTimer(2);
	KillTimer(3);
	KillTimer(4);
	CDialog::OnDestroy();
}

///////////////////////////////////////////////////////////////////////////////
// InitListCtrl
void XListCtrlDlg::InitListCtrl(XListCtrl& list)
{
	// set column width according to window rect
	CRect rect;
	list.GetWindowRect(&rect);

	list.ModifyStyle(0, LVS_EDITLABELS | LVS_NOLABELWRAP | LVS_SHOWSELALWAYS /* | LVS_SINGLESEL */ );
	//int w = rect.Width() - 2;
	int i;
	//int total_cx = 0;
	LV_COLUMN lvcolumn;
	memset(&lvcolumn, 0, sizeof(lvcolumn));

	// add columns
	for (i = 0;  i < m_pSelectColumns->count() ; i++)
	{
		int w = list.GetStringWidth(m_pSelectColumns->m_szFieldData[i].name);
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = m_pSelectColumns->m_szFieldData[i].cfmt;
		lvcolumn.pszText = m_pSelectColumns->m_szFieldData[i].name;
		lvcolumn.iSubItem = i;
		lvcolumn.cx = 4 + w + m_pSelectColumns->m_szFieldData[i].xwidth;
		//total_cx += lvcolumn.cx;
		list.InsertColumn(i, &lvcolumn);
	}


	// create the image list from bitmap resource
	VERIFY(list.m_cImageList.Create(IDB_CHECKBOXBMP, 18, 0, RGB(255, 0, 255))); // IMAGE WORK, all dialogs of this class use a 'checkbox on the left' scheme, all status indices must be shifted by three (3)
	list.m_HeaderCtrl.SetImageList(&list.m_cImageList);
	list.SetImageList(&list.m_cImageList,LVSIL_SMALL);

	list.m_HeaderCtrl.SetSelectColumns(m_pSelectColumns);

	// iterate through header items and attach the image list
	HDITEM hditem;

	for (i = 0; i < list.m_HeaderCtrl.GetItemCount(); i++)
	{
		memset(&hditem, 0, sizeof(hditem));
		hditem.mask = HDI_IMAGE | HDI_FORMAT | HDI_LPARAM;
		list.m_HeaderCtrl.GetItem(i, &hditem);
		hditem.fmt |=  HDF_IMAGE;
		if (i == ImpEditCol::eCheck)
			hditem.iImage = XHEADERCTRL_UNCHECKED_IMAGE;
		else
			hditem.iImage = XHEADERCTRL_NO_IMAGE;
		hditem.lParam = (LPARAM)(m_pSelectColumns->m_szFieldData[i].bMutable);
		list.m_HeaderCtrl.SetItem(i, &hditem);
	}

	memset(&lvcolumn, 0, sizeof(lvcolumn));

	// set the format again - must do this twice or first column does not get set
	for (i = 0; i < m_pSelectColumns->count(); i++)
	{
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM;
		lvcolumn.fmt = m_pSelectColumns->m_szFieldData[i].cfmt;
		lvcolumn.iSubItem = i;
		list.SetColumn(i, &lvcolumn);
	}
}

///////////////////////////////////////////////////////////////////////////////
// OnClick
//
// This method shows how to handle NM_CLICK messages from XListCtrl
//
void XListCtrlDlg::OnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = -1;
	int nSubItem = -1;
	if (pNMIA)
	{
		nItem = pNMIA->iItem;
		nSubItem = pNMIA->iSubItem;
	}
	TRACE(_T("in XListCtrlDlg::OnClick:  %d, %d\n"), nItem, nSubItem);

	*pResult = 0;
}

///////////////////////////////////////////////////////////////////////////////
// OnDblClick
//
// This method shows how to handle NM_DBLCLK messages from XListCtrl
//
void XListCtrlDlg::OnDblClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = -1;
	int nSubItem = -1;
	if (pNMIA)
	{
		nItem = pNMIA->iItem;
		nSubItem = pNMIA->iSubItem;
	}
	TRACE(_T("in XListCtrlDlg::OnDblClick:  %d, %d\n"), nItem, nSubItem);

	*pResult = 0;
}

///////////////////////////////////////////////////////////////////////////////
// OnColumnClick
//
// This method shows how to handle LVN_COLUMNCLICK messages from XListCtrl
//
void XListCtrlDlg::OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
//	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
//	int nItem = -1;
//	int nSubItem = -1;
//	if (pNMLV)
//	{
//		nItem = pNMLV->iItem;
//		nSubItem = pNMLV->iSubItem;
//	}
//	TRACE(_T("in XListCtrlDlg::OnColumnClick:  %d, %d\n"), nItem, nSubItem);
//
	*pResult = 0;
}

///////////////////////////////////////////////////////////////////////////////
// OnItemChanged
//
// This method shows how to handle LVN_ITEMCHANGED messages from XListCtrl
//
void XListCtrlDlg::OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int nItem = -1;
	int nSubItem = -1;
	if (pNMLV)
	{
		nItem = pNMLV->iItem;
		nSubItem = pNMLV->iSubItem;
	}
	//TRACE(_T("in XListCtrlDlg::OnItemChanged:  %d, %d\n"), nItem, nSubItem);

	if (pNMLV && (pNMLV->uNewState == (UINT)(LVIS_FOCUSED|LVIS_SELECTED)))
	{
		TRACE(_T("item has changed:  %d, %d\n"), nItem, nSubItem);
	}

	*pResult = 0;
}



LRESULT XListCtrlDlg::OnSubEditEnd(WPARAM , LPARAM p) 
{
	LV_DISPINFO *dispinfo = (LV_DISPINFO*)p;

	if (dispinfo->item.pszText != NULL)
	{
		TCHAR bar[512];
		dispinfo->item.cchTextMax = CLAMP(dispinfo->item.cchTextMax, 0, 510); // leave trailing blank space
		CString::CopyChars(bar,dispinfo->item.pszText, dispinfo->item.cchTextMax);
		bar[dispinfo->item.cchTextMax] = 0;
		CString szDisplayableResult;
		if (UpdateValue(dispinfo->item.iItem , dispinfo->item.iSubItem, (PTCHAR)&bar, szDisplayableResult))
			m_List.SetItemText(dispinfo->item.iItem, dispinfo->item.iSubItem, szDisplayableResult);
		else
			m_List.SetItemText(dispinfo->item.iItem, dispinfo->item.iSubItem, szDisplayableResult, -1, WARNINGPINK);
	}

	return 0;
}


///////////////////////////////////////////////////////////////////////////////
// OnComboSelection
//
// This method shows how to handle WM_XLISTCTRL_COMBO_SELECTION messages 
// from XListCtrl
//
LRESULT XListCtrlDlg::OnComboSelection(WPARAM nItem, LPARAM nSubItem)
{
	TRACE(_T("in XListCtrlDlg::OnComboSelection:  %d, %d\n"), nItem, nSubItem);
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// OnCheckbox
//
// This method shows how to handle WM_XLISTCTRL_CHECKBOX_CLICKED messages 
// from XListCtrl
//
LRESULT XListCtrlDlg::OnCheckbox(WPARAM nItem, LPARAM nSubItem)
{
	TRACE(_T("in XListCtrlDlg::OnCheckbox:  %d, %d\n"), nItem, nSubItem);
	return 0;
}
void XListCtrlDlg::CleanUpStatics()
{
	if (m_pSelectColumns != NULL)
	{
		try
		{
			m_pSelectColumns->ClearOut();
			delete m_pSelectColumns;
		}
		catch (...)
		{
			//e.ReportError();
		}
	}
	m_pSelectColumns = NULL;
}

#include ".\XCalDlg.h"
#include ".\xlistctrltestdlg.h"

LRESULT XListCtrlDlg::OnBoxDblClick(WPARAM w, LPARAM l) 
{
	try
	{
		int nItem = (int)w;
		int nSubItem = (int)l;
		CRect rect;
		m_List.GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rect);
		CString id = m_List.GetItemText(nItem, ImpEditCol::eID);
		CField* pField = m_pSelectColumns->GetAt(nSubItem);  // field name

		CString cstr;
		cstr.Format("%s for item # %d, id %s",pField->name, nItem, id);

		XCalDlg datetimedlg(rect.left, rect.top, cstr, this);

		datetimedlg.m_date =  m_List.GetDateTime(nItem, nSubItem); // init with proper time and date
		datetimedlg.m_time =  m_List.GetDateTime(nItem, nSubItem); // init with proper time and date
		
		INT_PTR r = datetimedlg.DoModal();
		if (r==IDOK && datetimedlg.bDataChanged())
		{
			COleDateTime ldfh;
			ldfh.SetDateTime(
				datetimedlg.m_date.GetYear(), datetimedlg.m_date.GetMonth(), datetimedlg.m_date.GetDay(),
				datetimedlg.m_time.GetHour(), datetimedlg.m_time.GetMinute(),datetimedlg.m_time.GetSecond());
			
			CFieldDate::FormatForOutput(ldfh, cstr); 

			m_List.SetCalCtrl(nItem, nSubItem, ldfh, cstr);
			// recover new time and date and set it

			CString szDisplayableResult;
			UpdateValue(nItem, nSubItem, cstr, szDisplayableResult);
		}
	}
	catch (...)
	{
	}

	return 0;
}

LRESULT XListCtrlDlg::OnFatToolTip(WPARAM w, LPARAM l) 
{
	return 0;
}

void XListCtrlDlg::OnBnClickedAnalyzeImp()
{
}

void XListCtrlDlg::OnBnClickedOk()
{
	OnOK();
}


void XListCtrlDlg::ReplaceSelection(LPTSTR lptstr)
{
	// get first line in text up to \r\n
	//	for each tab section in the first line
			// try to match string with a header name
			// if not, determine current selection and use that column

	// implemented in those subclasses that need it

	//int n = m_List.GetItemCount();
	//for (int i = 0; i < n; i++)
	//{
	//	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) m_List.GetItemData(i);

	//}

}

BOOL XListCtrlDlg::PreTranslateMessage(MSG* pMsg)
{
	// implement Ctrl-X and Ctrl-A too (see GetNextSelectedItem)

	if (pMsg->message == WM_CHAR)
	{
		WORD c = LOWORD(pMsg->wParam);
		if (c == 3) // Ctrl-C
		{
			bool r = CopyToClipboard(false, true);
			return r;
		}
		else if (c == 22) // Ctrl-V
		{
			bool r = CopyFromClipboard(true);
			return r;
		}
		else if (c == 24) // Ctrl-X
		{
			bool r = CutToClipboard(false);
			return r;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}
bool XListCtrlDlg::CopyFromClipboard(bool bReplaceSelected)
{
	// column paste
	// full paste
	// row paste
	// and cell paste

	if (!::IsClipboardFormatAvailable(CF_TEXT)) 
            return true; 
	if (!OpenClipboard()) 
            return true; 
    HGLOBAL   hglb;
	LPTSTR    lptstr; 
    hglb = GetClipboardData(CF_TEXT); 
    if (hglb != NULL) 
    { 
        lptstr = (LPSTR)GlobalLock(hglb); 
        if (lptstr != NULL) 
        { 
            // Call the application-defined ReplaceSelection 
            // function to insert the text and repaint the 
            // window. 

            ReplaceSelection(lptstr); 
            GlobalUnlock(hglb); 
        } 
    } 
    CloseClipboard(); 

	return true;
}
bool XListCtrlDlg::CutToClipboard(bool /*bAll*/)
{

	return true;
}

bool XListCtrlDlg::CopyToClipboard(bool bAll, bool bHeaders)
{
	const PTCHAR  _cediteol = "\r\n";
	if ( !OpenClipboard() )
	{
		AfxMessageBox( "Cannot open the Clipboard" );
		return true;
	}
	// Remove the current Clipboard contents
	if( !EmptyClipboard() )
	{
		AfxMessageBox( "Cannot empty the Clipboard" );
		return true;
	}
	// ...
	// Get the currently selected data
	int nItem, j, tlen, headeroffset;
	const int ic = m_List.GetItemCount();
	const int cc = m_List.GetHeaderCtrl()->GetItemCount();
	CStringArray a;
	a.SetSize(ic);
	tlen = 0;
	headeroffset = 0;
	if (bHeaders)
	{
		CString s;
		for (int xi = 0;  xi < m_pSelectColumns->count() ; xi++)
		{
			if (!(m_pSelectColumns->m_szFieldData[xi].bContent))  // take only desired content from a column
				continue;
			s.Append(m_pSelectColumns->m_szFieldData[xi].name);
			s.AppendChar('\t');
		}
		s.Append(_cediteol);
		a.SetAtGrow(0, s);
		tlen += s.GetLength();
		headeroffset = 1;
	}

	// now for the rows
	for (nItem = 0; nItem < ic; nItem++)
	{
		if (!bAll && !m_List.GetItemState(nItem, LVIS_SELECTED))
			continue;

		CString s = m_List.GetItemText(nItem,0);
		for (j = 1; j < cc; j++)
		{
			if (!(m_pSelectColumns->m_szFieldData[j].bContent))
				continue;
			s.Append(m_List.GetItemText(nItem,j));  // todo: account for text to numeric translation here
			s.AppendChar('\t');
		}
		s.Append(_cediteol);
		a.SetAtGrow(nItem + headeroffset, s);
		tlen += s.GetLength();
	}

    // Allocate a global memory object for the text. 
	LPTSTR  lptstrCopy; 
	HGLOBAL hglbCopy; 
    hglbCopy = GlobalAlloc(GMEM_MOVEABLE, 
        (tlen + 1) * sizeof(TCHAR)); 
    if (hglbCopy == NULL) 
    { 
        CloseClipboard(); 
        return true; 
    } 

    // Lock the handle and copy the text to the buffer. 
    lptstrCopy = (LPSTR)GlobalLock(hglbCopy);
    lptstrCopy[0] = (TCHAR) 0;    // null character 
	for (nItem = 0; nItem < a.GetCount(); nItem++)
	{
		LPSTR b = a[nItem].GetBuffer();
		size_t l = a[nItem].GetLength() *  sizeof(TCHAR);
		strncat(lptstrCopy, b, l);
	}
    lptstrCopy[tlen] = (TCHAR) 0;    // null character 
    GlobalUnlock(hglbCopy); 

    // Place the handle on the clipboard. 

	// For the appropriate data formats...
	if ( ::SetClipboardData( CF_TEXT, hglbCopy ) == NULL )
	{
		AfxMessageBox( "Unable to set Clipboard data" );
		CloseClipboard();
		return true;
	}
	// ...
	CloseClipboard();
	return true;
}



void XListCtrlDlg::OnEditCopy()
{
	CopyToClipboard(false);
}

void XListCtrlDlg::OnUpdateEditCopy(CCmdUI *pCmdUI)
{
}

void XListCtrlDlg::OnEditPaste()
{
	CopyFromClipboard(true);
}

void XListCtrlDlg::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	if (!::IsClipboardFormatAvailable(CF_TEXT)) 
       pCmdUI->Enable(FALSE);
	else
       pCmdUI->Enable(TRUE);
}
void XListCtrlDlg::OnUpdateEditCut(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

