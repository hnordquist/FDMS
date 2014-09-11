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
// EventDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\FDMS.h"
#include "EventDlg.h"
#include "..\combos.h"
#include ".\eventdlg.h"

#ifdef _XJDEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



///////////////////////////////////////////////////////////////////////////////
// EventDlg dialog

BEGIN_MESSAGE_MAP(EventDlg, XListCtrlDlg)
	ON_BN_CLICKED(ID_ANALYZE_IMP, OnBnClickedAnalyzeImp)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_REGISTERED_MESSAGE(WM_XLISTCTRL_CHECKBOX_CLICKED, OnCheckbox)
	ON_REGISTERED_MESSAGE(WM_XLISTCTRL_COMBO_SELECTION, OnComboSelection)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, OnColumnClick)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	//ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	//ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	//ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_WM_DESTROY()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// ctor
EventDlg::EventDlg(bool bAllNew, CObList *pList, CObList *pBkgList, CWnd* pParent /*=NULL*/) 
: XListCtrlDlg(bAllNew, pList, pParent), m_BkgList(pBkgList), m_WaitingForFit(false), m_WaitingForPersist(false)
{

}

EventDlg::~EventDlg()
{

}

void EventDlg::DoDataExchange(CDataExchange* pDX)
{
	XListCtrlDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(EventDlg)
	//}}AFX_DATA_MAP
}

void EventDlg::OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if (pNMLV)
	{
		if (ImpEditCol::eCheck ==  pNMLV->iSubItem) 
			OnCheckbox(1, pNMLV->iSubItem);
	}
	*pResult = 0;
}

LRESULT EventDlg::OnCheckbox(WPARAM nItem, LPARAM nSubItem)
{
	int c =m_List.CountCheckedItems(nSubItem);
	if (c < 1)
	{
		GetDlgItem(ID_ANALYZE_IMP)->EnableWindow(false);
		GetDlgItem(IDOK)->EnableWindow(false);
	}
	else
	{
		GetDlgItem(ID_ANALYZE_IMP)->EnableWindow(true);
		CString s;
		s.LoadString(ID_ANALYZE_IMP);
		if (c > 1)
			s.Append("ies");
		else
			s.AppendChar('y');
		SetDlgItemText(ID_ANALYZE_IMP, s);

		SetOKButtonIfValidContent(true);
	}

	return 0;
}

void EventDlg::SetOKButtonIfValidContent(bool bAcceptanceCheck)
{
	int good = 0;
	for (int nItem = 0; nItem < m_List.GetItemCount(); nItem++)
	{
		if (!m_List.GetCheckbox(nItem,ImpEditCol::eCheck)) 
			continue;

		MEvent* pm = (MEvent*)m_List.GetItemData(nItem);
		if (!pm)
			continue;

		if (pm->m_vr.ValidContent())
		{
			if ((!bAcceptanceCheck) || // don't care or 
				(bAcceptanceCheck && (!pm->GetAccepted()))) // catch those worth accepting
				good++;
		}
	}

	if (good < 1)
	{
		m_WaitingForFit = false;
		GetDlgItem(IDOK)->EnableWindow(false);
		SetDlgItemText(IDCANCEL, TEXT("Save Results"));
	}
	else
	{
		CString s;
		s = "Fit Selected Measurement";
		if (good > 1)
			s.Append("s");
		SetDlgItemText(IDOK, s);
		GetDlgItem(IDOK)->EnableWindow(true);
		SetDlgItemText(IDCANCEL, TEXT("Cancel"));
		m_WaitingForFit = true;
	}
}
BOOL EventDlg::PreTranslateMessage(MSG* pMsg)
{

	// implement Ctrl-X and Ctrl-A too (see GetNextSelectedItem)

	if (pMsg->message == WM_CHAR)
	{
		WORD c = LOWORD(pMsg->wParam);
		if (c == 1) // Ctrl-A
		{
			int nCheckedState = m_List.GetHeaderCheckedState(ImpEditCol::eCheck);

			// 0 = no checkbox
			if (nCheckedState != XHEADERCTRL_NO_IMAGE)
			{
				/*for (int nItem = 0; nItem < m_List.GetItemCount(); nItem++)
				{
					XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) m_List.GetItemData(nItem);
					if (!pXLCD)
					{
						continue;
					}

					if (pXLCD[ImpEditCol::eCheck].nCheckedState != -1)
					{
						pXLCD[ImpEditCol::eCheck].nCheckedState;
					}
				}
				*/
				OnCheckbox(1, ImpEditCol::eCheck);
			}
			m_List.SelectAll(true);
			return true;
		}
	}
	return XListCtrlDlg::PreTranslateMessage(pMsg);
}
LRESULT EventDlg::OnComboSelection(WPARAM nItem, LPARAM nSubItem)
{
	CString sle = m_List.GetComboText(nItem, nSubItem);
	MEvent* pm = (MEvent*)m_List.GetItemData(nItem);
	if (pm)
	{
		CString szDisplayableResult;
		pm->UpdateMeasurementInfo(ImpEditCol::m_meacolmap[nSubItem], sle, szDisplayableResult);
	}	

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// OnInitDialog
BOOL EventDlg::OnInitDialog()
{
	XListCtrlDlg::OnInitDialog();

	GetDlgItem(IDOK)->EnableWindow(false);
	GetDlgItem(ID_ANALYZE_IMP)->EnableWindow(false);
	return TRUE;  // return TRUE  unless you set the focus to a control
}


///////////////////////////////////////////////////////////////////////////////
// FillListCtrl
void EventDlg::FillListCtrl(XListCtrl& list)
{
	m_f.Build(m_pEvList);
	m_d.Build(m_pEvList);
	m_c.Build(m_pEvList);
	m_m.Build(m_pEvList);
}


bool EventDlg::UpdateValue(int iEntry, int iColumnID, LPCSTR pRawString, CString& szDisplayableResult)  // column id is ImpEd enum
{
	// get the list entry (MEvent)

	// update the relevant field on the MEvent

	// return false if the value does not match the allowed data ranges

	// otherwise return true so that the text can be inserted with the correct formatting in the list row subitem

	MEvent* pm = (MEvent*)m_List.GetItemData(iEntry);
	if (pm)
	{
		pm->UpdateMeasurementInfo(ImpEditCol::m_meacolmap[iColumnID], pRawString, szDisplayableResult);
	}	
	return true;
}

//the measurement results need to be written into the fields after this operation for the user to see

void EventDlg::OnBnClickedAnalyzeImp()
{
 // see subclass
}

void EventDlg::OnBnClickedOk()
{
	OnOK();
}

void EventDlg::OnBnClickedCancel()
{
	int cont = IDYES;
	if (m_WaitingForFit)
		cont = AfxMessageBox("Cancel this dialog and discard the selected measurement events?\r\nSelect Yes to discard the events and close.\r\nSelect No to go back to the events dialog.",
		MB_YESNO);

	if (m_WaitingForPersist)
	{
		SaveChanges();
	}


	if (cont == IDYES)
	{
		OnCancel();
	}
}


MEvent* EventDlg::LookupBkg(MEvent *m)
{
	MEvent * p = NULL;
	MEvent * q = NULL;
	POSITION pos = m_BkgList->GetHeadPosition();
	while( pos != NULL )
	{
		p = (MEvent*)m_BkgList->GetNext( pos );
		if (p->StationShortName.CompareNoCase(m->StationShortName) != 0)
			continue;	// not the same detector
		
		if (p->EndTime <= m->StartTime)
		{
			if (!q)
			{
				q = p;
			}
			else
			{
				DATE test = m->StartTime - p->EndTime;
				DATE cur = m->StartTime - q->EndTime;
				if (test < cur)
				{
					q = p;
				}
			}
		}
	}
	return q;
}

const CString EventDlg::m_NoId = " ID ? ";

void EventDlg::AddOutputLine(LPCTSTR a, LPCTSTR b)
{
//	CString x;
//	x.Format("%s %s%s", m_DClass, a, b);
//	m_OList.AddString(x);

	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
	pApp->AppendToLogF("%s %s%s", m_DClass, a, b);

}
void EventDlg::AddOutputLine(LPCTSTR s)
{
	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
	pApp->AppendToLogF("%s %s", m_DClass, s);
}
void EventDlg::OnDestroy()
{
//	int l = m_OList.GetCount();
//	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();

//	CString str;
//	for (int i = 0; i < l; i++)
//	{
//		m_OList.GetText(i, str);
//		pApp->AppendToLog(str);
//	}
	XListCtrlDlg::OnDestroy();

}

void EventDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	/*
	int index, colnum;
	if( ( index = m_List.HitTestEx( point, &colnum )) != -1 )
	{
		if (GetColumnWriteFlag(colnum))
		 // todo: if this hits over an editable cell, do a popup menu that has an operation to fill a column
		{
			CMenu* menu_bar = AfxGetMainWnd()->GetMenu();
			CMenu* file_menu = menu_bar->GetSubMenu(2);    
			ASSERT(file_menu);

			file_menu->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, point.x, point.y, this);
		}

	}
	else
	*/
		DefWindowProc(WM_CONTEXTMENU, (WPARAM)pWnd, (LPARAM)&point);
}

extern tMeasurementType tImageToMeasurementType(LPCSTR s);

bool EventDlg::CopyToClipboard(bool bAll, bool bHeaders)
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
	// Get the currently selected data

	int nItem, j, tlen, headeroffset;
	const int ic = m_List.GetItemCount();
	CStringArray a;
	a.SetSize(ic);
	tlen = 0;
	headeroffset = 0;
	if (bHeaders)
	{
		CString s;
		for (j = efhFacility;  j < efhHeaderCount; j++)
		{
			UINT icol = j;  // direct mapping from header id to header string
			s.Append(CFDMSApp::GetFileHeader(CSVFileHeaderIDs(icol)));
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
		for (j = efhFacility; j < efhHeaderCount; j++)
		{
			if (j == efhDischMonth || j == efhDischYear)  // blend of three columns into one
				continue;
			if (j == efhMeasMonth || j == efhMeasYear)  // blend
				continue;
			UINT icol = ImpEditCol::m_fileheadermap[j];
			if (j == efhDischDay)  //  build combined disch date
			{
				COleDateTime dt = m_List.GetDateTime(nItem, icol);
				s.Append(dt.Format("%d\t%m\t%Y"));
			}
			else
			if (j == efhMeasDay)  // build combined meas date
			{
				COleDateTime dt = m_List.GetDateTime(nItem, icol);
				s.Append(dt.Format("%d\t%m\t%Y"));				
			}
			else
			if (j == efhStatus)  // get status from the related globals
			{
				s.Append("0");  // the status of all entries in an import dialog is always unmeasured
			}
			else
			if (j == efhMeasType)  // convert string to number from the item text
			{
				CString cs;
				cs.Format("%d", tImageToMeasurementType(m_List.GetItemText(nItem,icol)));
				s.Append(cs);
			}
			else
				s.Append(m_List.GetItemText(nItem,icol));
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
	if ( SetClipboardData( CF_TEXT, hglbCopy ) == NULL )
	{
		AfxMessageBox( "Unable to set Clipboard data" );
		CloseClipboard();
		return true;
	}
	CloseClipboard();
	return true;
}

extern PTCHAR tMeasurementTypeImage(tMeasurementType t);

void EventDlg::UpdateColumnEntry(int iItem, int hcol, LPTSTR val)
{
	UINT icol =ImpEditCol::m_fileheadermap[hcol];  // see this garbage? a result of poor design
	CString s;

	if (ImpEditCol::eMeasType == icol)   // for string to numeric internal maps, handle the update differently
	{
		MEvent* pm = (MEvent*)m_List.GetItemData(iItem);
		if (pm)
		{
			CString szDisplayableResult;
			pm->UpdateMeasurementInfo(ImpEditCol::m_meacolmap[icol], val, szDisplayableResult);
			m_List.SetItemText(iItem,ImpEditCol::eMeasType,tMeasurementTypeImage((tMeasurementType)pm->m_vr._iMPMeasurementType));
		}	
	}
	else
	{
		if (UpdateValue(iItem , icol, val, s)) // icol is ImpEd enum
		{
			m_List.SetItemText(iItem , icol, s);
		}
	}


}

void EventDlg::ReplaceSelection(LPTSTR lptstr)
{
	// get first line in text up to \r\n 0xd0xa
	//	for each tab section in the first line
			// try to match string with a header name
			// if not, determine current selection and use that column
	const PTCHAR basedate = "1950.01.01";

	try
	{
		bool  columns[efhHeaderCount];
		
		int  columnmatch;
		const CHAR septabnl[]   = "\t\r\n";
		CHAR *ltoken;
		int i;
		columnmatch = 0;
		for (i = 0; i < efhHeaderCount; i++)
		{
			columns[i] = false;
		}
		bool datecolfound = false, date3colsfound = false;
		// get header line
		ltoken = strtok( lptstr, septabnl );

		for (i = 0; i < efhHeaderCount; i++)
		{
			if (i == efhDischDay) // extra processing, looser matching, for three-column source dates
			{
				bool found = (0 == _strnicmp(ltoken, CFDMSApp::GetFileHeader(CSVFileHeaderIDs(i), 12), 12)); // "discharge da"
				if (found)
				{
					const PTCHAR combocol = "discharge date";
					datecolfound = (0 == _stricmp(ltoken, combocol)); // "discharge date"

					if (!datecolfound)
					{
						// I'm looking for two more column names here
						ltoken = strtok( NULL, septabnl );  // should be "month"
						if (ltoken)
						{
							bool y = 0;
							bool m = (0 == _stricmp(ltoken, CFDMSApp::GetFileHeader(efhDischMonth)));

							ltoken = strtok( NULL, septabnl ); // should be "year"
							if (ltoken)
								y = (0 == _stricmp(ltoken, CFDMSApp::GetFileHeader(efhDischYear)));

							date3colsfound = (y && m);
						}
					}
					columns[efhDischDay] = found;
					if (found) 
						columnmatch++;
				}
			}
			else
			{
				if (0 == _stricmp(ltoken, CFDMSApp::GetFileHeader(CSVFileHeaderIDs(i))))
				{
					columns[i] = true;
					columnmatch++;
				}
			}
			if (columns[i])
				break;

		}
		if (columnmatch == 0)
			return;

		// put ltoken in the item text
		// start at current row, and paste down until end of list row
		BOOL enabled = FALSE;
		for (int j = 0; (ltoken != NULL) &&
			(j < m_List.GetItemCount()); j++)
		{
			enabled = m_List.GetEnabled(j);
			if (enabled) // paste is mapped only to enabled rows
			{
				if (datecolfound) //
				{
					// test token for dateness; use some heuristics on locale and IAEA formats
					ltoken = strtok( NULL, septabnl );
					if (ltoken)
					try {
						CHAR buff[256];
						ZeroMemory(buff, sizeof(buff));
						COleDateTime candidate = CFieldDate::ConvertDefaultX(ltoken,VAR_DATEVALUEONLY);
						if (candidate.GetStatus() == COleDateTime::valid)
							strcpy(buff, ltoken);
						else
							sprintf(buff, basedate);
						UpdateColumnEntry(j, i, buff);
					}
					catch(...)
					{
					}
				}
				else if (date3colsfound && columns[efhDischDay]) 
				{
					CHAR *day = NULL, *month = NULL, *year = NULL;
					// grab the three tokens, form a date out of it and paste the IAEA formatted date
					int m = 0, d = 0, y = 0;
					int res = 0;
					
					day = strtok( NULL, septabnl );  // should be "day"
					if (day)
						sscanf(day, "%d", &d);
					if (day)
						month = strtok( NULL, septabnl );  // should be "month"
					if (month)
						sscanf(month, "%d", &m);
					if (month)
						year = strtok( NULL, septabnl ); // should be "year"
					if (year)
						sscanf(year, "%d", &y);

					if (day && month && year)
					{
						CHAR buff[256];
						ZeroMemory(buff, sizeof(buff));
						try
						{
							COleDateTime candidate;
							res = candidate.SetDate(y,m,d);
							if (res == 0 && (candidate.GetStatus() == COleDateTime::valid))
								sprintf(buff, CFieldDate::DateFormat, y, m, d);
							else
								sprintf(buff, basedate);
						}
						catch(...)
						{
						}
						UpdateColumnEntry(j, i, buff);
					}
					else
					{
						UpdateColumnEntry(j, i, basedate);
					}
				}
				else
				{
					ltoken = strtok( NULL, septabnl );
					if (ltoken)
						UpdateColumnEntry(j, i, ltoken);
				}
			}
		}
	}
	catch (...)
	{
	}
}

void EventDlg::InitMIndex()
{
	if (m_idxa.GetCount() <= 0)
	{
		m_idxa.SetSize(m_List.GetItemCount());
		for (int nItem = 0; nItem < m_List.GetItemCount(); nItem++)
		{
			m_idxa[nItem] = -1;
		}
	}
}