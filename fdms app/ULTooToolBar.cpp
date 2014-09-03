//////////////////////////////////////////////////////////////////////////
// Copyright 2005, The Regents Of the University of California.  This
// software was produced under U.S. Government contract (W-7405-ENG-36) at
// the Los Alamos National Laboratory by the Safeguards Science and
// Technology Group (N-1).  All Rights Reserved.
//////////////////////////////////////////////////////////////////////////
// $History: ULTooToolBar.cpp $
// 
// *****************  Version 11  *****************
// User: Longo        Date: 3/17/05    Time: 2:20p
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 10  *****************
// User: Longo        Date: 3/16/05    Time: 2:21p
// Updated in $/FDMS/FDMS Application/FDMSNET
// nearing the deadlines ...
// 
// *****************  Version 9  *****************
// User: Longo        Date: 3/09/05    Time: 3:01p
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 8  *****************
// User: Longo        Date: 2/28/05    Time: 10:00a
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 7  *****************
// User: Longo        Date: 2/07/05    Time: 10:19a
// Updated in $/FDMS/FDMS Application/FDMSNET
// checkpoint checkin
// 
// *****************  Version 6  *****************
// User: Longo        Date: 1/06/05    Time: 10:05a
// Updated in $/FDMS/FDMS Application/FDMSNET
// threaded import processing for UI interaction
// 
// *****************  Version 5  *****************
// User: Longo        Date: 1/05/05    Time: 5:31p
// Updated in $/FDMS/FDMS Application/FDMSNET
// import button implementation
// 
// *****************  Version 4  *****************
// User: Longo        Date: 11/24/04   Time: 3:48p
// Updated in $/FDMS/FDMS Application/FDMSNET
// * bring up select measurement dialog upon requests from UL
// * added matching class with logic: uses 182 day window to seek for
// match
// * compiler warning removal
// 
// *****************  Version 3  *****************
// User: Longo        Date: 11/22/04   Time: 3:00p
// Updated in $/FDMS/FDMS Application/FDMSNET
// matching with detector id and timestamp to external tool
//
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FDMS.h"
#include "ULTooToolBar.h"
#include "ImportProcessor.h"



bool ULTooToolBar::m_bImportActive = false;

// ULTooToolBar
#define TIMER_CHECKIMP 871957

IMPLEMENT_DYNAMIC(ULTooToolBar, CToolBar)

ULTooToolBar::ULTooToolBar() : m_bULToolsAvailable(false), m_bImportSourceAvailable(false)
{
}

ULTooToolBar::~ULTooToolBar()
{
}

#define COMBOBOX_INDEX 0
#define COMBOBOX_WIDTH 150
#define COMBOBOX_HEIGHT 150
#define COMBOBOX_BITMAP 1


void ULTooToolBar::InitControlWithULInfo(CWnd* pParentWnd)
{

	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();

	Create(pParentWnd);
	LoadToolBar(IDR_TOOLBAR1);
	DWORD sty = GetBarStyle();
	SetBarStyle(sty | CBRS_TOOLTIPS | CBRS_FLYBY);
	EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
	SetWindowText(_T("Integrated Tools"));

	CRect rect(-COMBOBOX_WIDTH, -COMBOBOX_HEIGHT, 0, 0);

	// The ID of the ComboBox is important for two reasons.  One, so you
	// can receive notifications from the control.  And also for ToolTips.
	// During HitTesting if the ToolBar sees that the mouse is one a child
	// control, the toolbar will lookup the controls ID and search for a
	// string in the string table with the same ID to use for ToolTips
	// and StatusBar info.
	if (m_ToolBox.Create(WS_CHILD | CBS_DROPDOWN |
		CBS_AUTOHSCROLL | WS_VSCROLL | CBS_HASSTRINGS, rect, this,
		ID_TOOLCOMBOBOXID))
	{
		HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		if (hFont == NULL)
			hFont = (HFONT)GetStockObject(ANSI_VAR_FONT);

		m_ToolBox.SendMessage(WM_SETFONT, (WPARAM)hFont);


		// determine if UL is available.

		UINT num_tools = 0;


		if (pApp->m_pRT_Init != NULL)
			num_tools = pApp->m_pRT_Init->Get_Tools_Num();

		if (num_tools > 0)
		{
			const char * tool_name;
			unsigned int i, j;
			BOOL tool_status;
			m_bULToolsAvailable = true;
			j = 0;
			for (i=0; i<num_tools; i++)
			{
				tool_status = pApp->m_pRT_Init->Get_Tool_Installed (i);
				if (tool_status == FALSE)
					continue;
				tool_status = pApp->m_pRT_Init->Get_Data_Match_Tool (i);
				if (tool_status == FALSE)
					continue;
				tool_name = pApp->m_pRT_Init->Get_Tool_Name (i);
				m_ToolBox.InsertString(j, tool_name);
				m_ToolBox.SetItemData(j, i);
				j++;
			}
		}
		else
			m_ToolBox.InsertString(0, "None Available");

		if (ULSupport::ULIRSDataPath.GetLength() > 5) // need something with suffix at least
		{
			// no UL no IRS no FacMgr or whtever so don't bother to check
			CheckForImportFileExistence();
			SetTimer(TIMER_CHECKIMP,21000,0);  // check every 21 seconds
		}
	}

	if(!SetHorizontal())
		return;
}

#include "io.h"
void ULTooToolBar::CheckForImportFileExistence()
{
	if (_access(ULSupport::ULIRSDataPath,06)==-1)
	{
		m_bImportSourceAvailable = false;
	}
	else
	{
		m_bImportSourceAvailable = true;
	}
}

#include "FieldDate.h"
#include ".\ultootoolbar.h"

void ULTooToolBar::StartTheTool()
{
	int i = m_ToolBox.GetCurSel();
	StartTheTool(i);
}
void ULTooToolBar::StartTheTool(int item)
{

	int id = (int) m_ToolBox.GetItemData(item);
	const char * tool_name = NULL;

	if (item < 0 || item < 0)
		return;

	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();

	tool_name = pApp->m_pRT_Init->Get_Tool_Name (id);

	if (tool_name == NULL)
		return;
	/* build time stamp for last measurement reviewed */

	int iData = g_iMPIndexMostRecentMeasurement-1;
	if (iData < 0)
		iData  = 0;  // just for fun


	char _ts[23];
	char _dl[MAX_NAME_LENGTH];
	CString _tcs;
	ZeroMemory(_ts, sizeof(char)*23);
	ZeroMemory(_dl, sizeof(char)*MAX_NAME_LENGTH);

	CFieldDate::ConvertToTimestampStr(g_iMPMeasurementDateYear[iData], g_iMPMeasurementDateMonth[iData], g_iMPMeasurementDateDay[iData], _tcs),
	strncpy(_dl, g_szMPDetectorIDs[iData], MAX_NAME_LENGTH - 1);
	strncpy(_ts, _tcs.GetBuffer(), 22);

	pApp->m_pRT_Review->RT_Display_Other_Data (tool_name, _ts, _dl);

}


void ULTooToolBar::ImportFromTool()
{

	if (ULTooToolBar::m_bImportActive)
	{
		dvaprintf("ImportProcesser thread active, skipping\n");
		return;
	}

	CWaitCursor wait;

   AfxBeginThread(RUNTIME_CLASS(ImportProcessor));

}

BEGIN_MESSAGE_MAP(ULTooToolBar, CToolBar)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_IMPORT, OnUpdateImportButton)
	ON_WM_TIMER()
END_MESSAGE_MAP()

void ULTooToolBar::OnUpdateImportButton(CCmdUI *pCmdUI)
{
    if (m_bImportSourceAvailable)
		pCmdUI->Enable(); 
	else
		pCmdUI->Enable(FALSE);
}


// ULTooToolBar message handlers

BOOL ULTooToolBar::SetHorizontal()
{
	m_bVertical = FALSE;

	SetBarStyle((GetBarStyle() & ~CBRS_ALIGN_ANY) | CBRS_ALIGN_TOP);

	SetButtonInfo(COMBOBOX_INDEX, ID_TOOLCOMBOBOXID, TBBS_SEPARATOR, COMBOBOX_WIDTH);

	if (m_ToolBox.m_hWnd != NULL)
	{
		CRect rect;
		GetItemRect(COMBOBOX_INDEX, rect);

		m_ToolBox.SetWindowPos(NULL, rect.left, rect.top, 0, 0, SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOCOPYBITS);
		m_ToolBox.ShowWindow(SW_SHOW);
	}

	return TRUE;
}


BOOL ULTooToolBar::SetVertical()
{
	m_bVertical = TRUE;

	SetButtonInfo(COMBOBOX_INDEX, ID_EDIT_FIND, TBBS_BUTTON, COMBOBOX_BITMAP);

	if (m_ToolBox.m_hWnd != NULL)
		m_ToolBox.ShowWindow(SW_HIDE);

	return TRUE;
}


CSize ULTooToolBar::CalcDynamicLayout(int nLength, DWORD dwMode)
{
	// if we're committing set the buttons appropriately
	if (dwMode & LM_COMMIT)
	{
		if (dwMode & LM_VERTDOCK)
		{
			if (!m_bVertical)
				SetVertical();
		}
		else
		{
			if (m_bVertical)
				SetHorizontal();
		}

		return CToolBar::CalcDynamicLayout(nLength, dwMode);
	}
	else
	{
		BOOL bOld = m_bVertical;
		BOOL bSwitch = (dwMode & LM_HORZ) ? bOld : !bOld;

		if (bSwitch)
		{
			if (bOld)
				SetHorizontal();
			else
				SetVertical();
		}


		CSize sizeResult = CToolBar::CalcDynamicLayout(nLength, dwMode);

		if (bSwitch)
		{
			if (bOld)
				SetHorizontal();
			else
				SetVertical();
		}

		return sizeResult;
	}
}

void ULTooToolBar::OnTimer(UINT nIDEvent)
{
	if (nIDEvent == TIMER_CHECKIMP)
	{
		CheckForImportFileExistence();
	}
	CToolBar::OnTimer(nIDEvent);
}
