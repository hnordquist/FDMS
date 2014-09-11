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
//////////////////////////////////////////////////////////////////////////
// Copyright 2004, The Regents Of the University of California.  This
// software was produced under U.S. Government contract (W-7405-ENG-36) at
// the Los Alamos National Laboratory by the Safeguards Science and
// Technology Group (N-1).  All Rights Reserved.
//////////////////////////////////////////////////////////////////////////
// $History: SelectMeasurement.cpp $
// 
// *****************  Version 18  *****************
// User: 186846       Date: 2/26/08    Time: 8:54a
// Updated in $/FDMS/FDMS Application/FDMSNET
// checkpoint
// 
// *****************  Version 17  *****************
// User: Longo        Date: 12/18/07   Time: 12:32p
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 16  *****************
// User: Longo        Date: 3/16/05    Time: 2:21p
// Updated in $/FDMS/FDMS Application/FDMSNET
// nearing the deadlines ...
// 
// *****************  Version 15  *****************
// User: Longo        Date: 3/11/05    Time: 10:35a
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 14  *****************
// User: Longo        Date: 2/28/05    Time: 10:00a
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 13  *****************
// User: Longo        Date: 2/10/05    Time: 3:46p
// Updated in $/FDMS/FDMS Application/FDMSNET
// checkpoint checkin
// 
// *****************  Version 12  *****************
// User: Longo        Date: 2/07/05    Time: 10:27a
// Updated in $/FDMS/FDMS Application/FDMSNET
// checkpoint checkin
// 
// *****************  Version 11  *****************
// User: Longo        Date: 1/07/05    Time: 2:25p
// Updated in $/FDMS/FDMS Application/FDMSNET
// adding the secondary import editor dilaog
// 
// *****************  Version 10  *****************
// User: Longo        Date: 12/21/04   Time: 4:14p
// Updated in $/FDMS/FDMS Application/FDMSNET
// added a few things
// 
// *****************  Version 9  *****************
// User: Longo        Date: 12/10/04   Time: 6:25p
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 8  *****************
// User: Longo        Date: 12/10/04   Time: 4:44p
// Updated in $/FDMS/FDMS Application/FDMSNET
// Combining the GrandCom demo class wirth the SerialCom live detector
// control class
// 
// *****************  Version 7  *****************
// User: Longo        Date: 11/30/04   Time: 4:22p
// Updated in $/FDMS/FDMS Application/FDMSNET
// added code to auto-select the matching row in the measeur dialog list,
// when involkd by IR tool review message
// 
// *****************  Version 6  *****************
// User: Longo        Date: 11/24/04   Time: 3:48p
// Updated in $/FDMS/FDMS Application/FDMSNET
// * bring up select measurement dialog upon requests from UL
// * added matching class with logic: uses 182 day window to seek for
// match
// * compiler warning removal
// 
// *****************  Version 5  *****************
// User: Longo        Date: 11/22/04   Time: 3:04p
// Updated in $/FDMS/FDMS Application/FDMSNET
// block UL while ediitng plan entries
// 
// *****************  Version 4  *****************
// User: Longo        Date: 11/19/04   Time: 10:28a
// Updated in $/FDMS/FDMS Application/FDMSNET
// toolbar creation
// 
// *****************  Version 3  *****************
// User: Longo        Date: 10/20/04   Time: 1:10p
// Updated in $/FDMS/FDMS Application/FDMSNET
// registry failed today, saving all work

#include "stdafx.h"
#include <math.h>
#include "fdms.h"

#include ".\selectmeasurement.h"

#include "doassay.h"
#include "DemoDoAssay.h"

#include "acceptreject.h"
#include "dconfig.h"
#include "feeoff.h"

#include ".\processmeasurement.h"

#ifdef _XJDEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define INVALIDATETIMER 2500
/////////////////////////////////////////////////////////////////////////////
// CSelectMeasurement dialog


CSelectMeasurement::CSelectMeasurement(CWnd* pParent /*=NULL*/)
: CDialog(CSelectMeasurement::IDD, pParent)
{
	m_iSelected = -1;
	m_preIndex = -1;
	m_iSubsetSelector = UnspecifiedVerification;
	m_bInitialized = FALSE;
	m_bETStat = false;
	m_bWriteMP = false;
}

void CSelectMeasurement::CleanUpStatics()
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


void CSelectMeasurement::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectMeasurement)
	DDX_Control(pDX, IDOK, m_cOK);
	DDX_Control(pDX, IDCANCEL, m_cCancel);
	DDX_Control(pDX, ID_ADD_NEW_M, m_cAdd);
	DDX_Control(pDX, IDC_LIST1, m_cListCtrl);
	//}}AFX_DATA_MAP
}

//	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnItemchangedList1)

BEGIN_MESSAGE_MAP(CSelectMeasurement, CDialog)
	//{{AFX_MSG_MAP(CSelectMeasurement)
	ON_BN_CLICKED(ID_EDIT_TOGGLE, OnEditToggle)
	ON_BN_CLICKED(ID_ADD_NEW_M, OnAddNewDeclaration)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, OnColumnclickList1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnItemchangedList1)
	ON_MESSAGE( MY_ENTEM_EDIT,  OnEditRow)
	ON_WM_SIZE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(ID_DEL_SEL_M, OnBnClickedDelSelM)
	ON_BN_CLICKED(ID_EDIT_TOGGLE3, OnBnClickedEditToggle3)
	ON_WM_DESTROY()
	ON_WM_CREATE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectMeasurement message handlers
//Facility	ID	Measurement type	Enrichment (%)	Burnup (GWd/MT)	Discharge date (dd-mm-year)	cycle #	Thres A	Thres B	 Measurement date (dd-mm-year)	Cooling Time (years)	 NA	 NB	 NC	 G1	 G2  Detector



void CSelectMeasurement::MakeFieldMap()
{
	if (m_pSelectColumns == NULL)
	{
		m_pSelectColumns = new CMeaCol();
		m_pSelectColumns->Reconstruct();
	}
}

void CSelectMeasurement::ConstructList()
{
	m_pImageList.Create( IDB_BMP_ALL, 18, 1, RGB(192,192,192));
	for (int iData = 0; iData < m_pSelectColumns->count(); iData++)
	{
		int cw = m_cListCtrl.GetStringWidth(m_pSelectColumns->m_szFieldData[iData].name);
		m_cListCtrl.InsertColumn(
			iData,
			m_pSelectColumns->m_szFieldData[iData].name,
			m_pSelectColumns->m_szFieldData[iData].cfmt,
			cw + m_pSelectColumns->m_szFieldData[iData].xwidth);
	}	// hide unused columns
	for (int i = 0; i < m_pSelectColumns->count(); i++)
	{
		if (
		(i >= CMeaCol::eThresholdA && i <= CMeaCol::eThresholdB))
		{
			LVCOLUMN col;

			col.mask = LVCF_WIDTH;
			if (m_cListCtrl.GetColumn(i, &col))
			{
				col.cx = 0;
				m_cListCtrl.SetColumn(i, &col);
			}
		}
	}
	m_cListCtrl.ModifyStyle(0, LVS_NOLABELWRAP | LVS_SINGLESEL | LVS_SHOWSELALWAYS );
	m_cListCtrl.SetImageList(&m_pImageList,LVSIL_SMALL);

	CString cTemp;
	int iNewItem = 0;
	for (int iData = 0; iData < g_iMPEntriesIn; iData++)
	{
		if (g_iMPMeasurementType[iData] == m_iSubsetSelector || m_iSubsetSelector == UnspecifiedVerification)
		{
			UpdateRow(true, iNewItem, iData);
			if (m_preIndex>= 0 && iData == m_preIndex)  // create selection marker for end of initdialog
			{
				m_preIndex = iNewItem;
			}
			iNewItem++;
		}
	}
	m_cListCtrl.SetROColumns((bool*)m_pSelectColumns->m_bColumnRWMap);
	m_cListCtrl.SetExtendedStyle(m_cListCtrl.GetExtendedStyle()|LVS_EX_HEADERDRAGDROP | LVS_EX_FULLROWSELECT);
}

BOOL CSelectMeasurement::OnInitDialog() 
{
	CDialog::OnInitDialog();
	extern PTCHAR tMeasurementTypeImage(tMeasurementType t);
	CString titlestring = tMeasurementTypeImage((tMeasurementType)m_iSubsetSelector);
	titlestring.Append(" Measurements");
	SetWindowText(titlestring);

	ConstructList();

	GetDlgItem(IDOK)->EnableWindow(false);

	GetClientRect(&m_OldRect);
	//CRect initRect;

	//initRect.left = m_OldRect.right - GetSystemMetrics(SM_CXHSCROLL);
	//initRect.top = m_OldRect.bottom - GetSystemMetrics(SM_CYVSCROLL);
	//m_Grip.Create(WS_CHILD | SBS_SIZEBOX | SBS_SIZEBOXBOTTOMRIGHTALIGN | 
	//	SBS_SIZEGRIP | WS_VISIBLE, initRect, this, AFX_IDW_SIZE_BOX);
	m_bInitialized = TRUE;

	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
	pApp->SetMeasurementDialogPtr(this);

	if (m_preIndex >= 0)
	{
		this->m_cListCtrl.SelectRow(m_preIndex);
	}

	return TRUE; 
}

void CSelectMeasurement::OnRemoveDeclaration() 
{
	if ((m_iSelected = m_cListCtrl.GetSelectionMark()) >= 0)
	{
		CString cTemp;
		cTemp.Format(_T("Remove Declaration %s ?"), m_cListCtrl.GetItemText(m_iSelected, CMeaCol::eItem));
		if (AfxMessageBox(cTemp, MB_YESNO) == IDYES)
		{
			m_bWriteMP = RemoveFromGlobals(m_iSelected);

			if (m_bWriteMP)
			{
				//UpdateInternalIndices(m_iSelected);

				//step through all of the rows greater than the removed one
				// for each row get the internal index, and decremnt it, and replac eit
				for (int j = m_iSelected + 1; j < m_cListCtrl.GetItemCount(); j++)
				{
					int iIndex = GetInternalIndex(j);
					iIndex--;
					// now need to set the proper internal index for this row entry
					cTemp.Format(GetColScheme(CMeaCol::eInternalIndex)->format,iIndex);
					m_cListCtrl.SetItemText(j,CMeaCol::eInternalIndex,cTemp);
				}

				m_cListCtrl.DeleteItem(m_iSelected);
				UpdateTheGraphsAndGlobals();
			}
		}
	}
}

bool CSelectMeasurement::RemoveFromGlobals(int row)
{
	// internal array index value from the indices array value as stored in the eInternalIndex row element

	int iIndex = GetInternalIndex(row);

	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
	return pApp->RemoveGlobalMPEntry(iIndex);

}

int CSelectMeasurement::EditRow(int nItem, FieldTypeCheck::_EditableRowInfo& ri)
{

	for (int x = ri.eFacility; x < ri.eEditableItems; x++)
	{
		ri.data[x].b = false;
		ri.data[x].val = m_cListCtrl.GetItemText(nItem,  ri.map[x]);
	}

	ri.number = m_cListCtrl.GetItemText(nItem,CMeaCol::eItem);
	ri.id = m_cListCtrl.GetItemText(nItem,CMeaCol::eID);

	int edited;
	FieldTypeCheck fee(ri);
	edited = fee.DoModal();

	if (edited == IDOK)
	{
		if (ri.bHasChangedData())
		{
			CString szDisplayableResult;
			for (int x = ri.eFacility; x < ri.eEditableItems; x++)
			{
				if (ri.data[x].b)
				{
					UpdateGlobals(nItem, ri.map[x], ri.data[x].val.GetBuffer(), szDisplayableResult);
					m_cListCtrl.SetItemText(nItem, ri.map[x], szDisplayableResult);
				}
			}
			UpdateTheGraphsAndGlobals();
			m_bWriteMP = true;
		}
	}
	return edited;
}

void CSelectMeasurement::UpdateTheGraphsAndGlobals(int iIndex)
{

	//call the fortran dll
	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
	pApp->UpdateGraphData();

	if (iIndex >= 0)
	{
		// refresh the row data
		UpdateRow(false, m_iSelected, iIndex);
	}

	//update the colored icons in the list box
	UpdateIconsAndIndex();
	m_cListCtrl.UpdateWindow();

	//now refresh all of the graphs
	pApp->UpdateAllDocs();

}

LRESULT CSelectMeasurement::OnEditRow(WPARAM w, LPARAM) 
{
	((CFDMSApp*)AfxGetApp())->SetAllMessagesOnHold(true);
	m_bBlock = true;

	try
	{
		int nItem = (int)w;
		FieldTypeCheck::_EditableRowInfo ri;
		ri.bNew = false;
		EditRow(nItem, ri);
	}
	catch (...)
	{
	}

	m_bBlock = false;
	((CFDMSApp*)AfxGetApp())->SetAllMessagesOnHold(false);
	return 0;
}

void CSelectMeasurement::AddToGlobals(int iItem)
{
	RowText x;

	x[CMeaCol::eFacility] = m_cListCtrl.GetItemText(iItem, CMeaCol::eFacility);
	x[CMeaCol::eID] = m_cListCtrl.GetItemText(iItem, CMeaCol::eID);
	x[CMeaCol::eMeasType] = m_cListCtrl.GetItemText(iItem, CMeaCol::eMeasType);
	x[CMeaCol::eEnrichment] = m_cListCtrl.GetItemText(iItem, CMeaCol::eEnrichment);
	x[CMeaCol::eBurnUp] = m_cListCtrl.GetItemText(iItem, CMeaCol::eBurnUp);
	x[CMeaCol::eDischDate] = m_cListCtrl.GetItemText(iItem, CMeaCol::eDischDate);
	x[CMeaCol::eCycle] = m_cListCtrl.GetItemText(iItem, CMeaCol::eCycle);
	x[CMeaCol::eThresholdA] = m_cListCtrl.GetItemText(iItem, CMeaCol::eThresholdA);
	x[CMeaCol::eThresholdB] = m_cListCtrl.GetItemText(iItem, CMeaCol::eThresholdB);
	x[CMeaCol::eMeasDate] = m_cListCtrl.GetItemText(iItem, CMeaCol::eMeasDate);
	x[CMeaCol::eCoolingTime] = m_cListCtrl.GetItemText(iItem, CMeaCol::eCoolingTime);
	x[CMeaCol::eNeutronA] = m_cListCtrl.GetItemText(iItem, CMeaCol::eNeutronA);
	x[CMeaCol::eNeutronB] = m_cListCtrl.GetItemText(iItem, CMeaCol::eNeutronB);
	x[CMeaCol::eNeutronC] = m_cListCtrl.GetItemText(iItem, CMeaCol::eNeutronC);
	x[CMeaCol::eGamma1] = m_cListCtrl.GetItemText(iItem, CMeaCol::eGamma1);
	x[CMeaCol::eGamma2] = m_cListCtrl.GetItemText(iItem, CMeaCol::eGamma2);
	x[CMeaCol::eDetector] = m_cListCtrl.GetItemText(iItem, CMeaCol::eDetector);

	ProcessMeasurement::AddToGlobals(x);
}

void CSelectMeasurement::UpdateGlobals(int iEntry, int iCID, PTCHAR p, CString& szDisplayableResult)
{
	int iIndex = GetInternalIndex(iEntry);
	ProcessMeasurement::UpdateGlobals(iIndex, iCID, p, szDisplayableResult);
}

int CSelectMeasurement::StartNewDeclaration() 
{
	if (!m_bETStat)  // editable state is available
	{
		return -1;
	}

	/* may not be active */
	m_iSelected = m_cListCtrl.GetSelectionMark();

	CString cTemp;

	const bool bNewEntry = (m_iSelected < 0);
	int iItem = m_cListCtrl.GetItemCount();

	// create new row with unique incremented sequence #
	cTemp.Format(GetColScheme(CMeaCol::eItem)->format, g_iMPEntriesIn+1);
	m_cListCtrl.InsertItem(iItem, cTemp,11);  // <<= image # 11

	// copy data from selected row
	for (int ii =  CMeaCol::eFacility; ii < m_pSelectColumns->count(); ii++)
	{
		CMeaCol::ColumnID iColumn = (CMeaCol::ColumnID)ii;
		if (iColumn == CMeaCol::eID)
		{
			cTemp.Format(GetColScheme(iColumn)->format,g_iMPEntriesIn);  // force unique ....
		}
		else if (iColumn == CMeaCol::eDetector)
		{
			cTemp = "";
		}
		else if (iColumn == CMeaCol::eMeasType)
		{
			cTemp.Format(GetColScheme(iColumn)->format, m_iSubsetSelector);
		}
		else if (iColumn == CMeaCol::eInternalIndex)
		{
			cTemp.Format(GetColScheme(iColumn)->format, g_iMPEntriesIn);
		}
		else if (iColumn > CMeaCol::eCoolingTime)
		{
			cTemp = m_cListCtrl.GetItemText(m_iSelected, iColumn);
		}
		else if (bNewEntry || (iColumn > CMeaCol::eCycle && iColumn <= CMeaCol::eGamma2)) // use zeroed element
		{
			if ( m_pSelectColumns->m_szFieldData[iColumn]._type == CField::eDate || 
				m_pSelectColumns->m_szFieldData[iColumn]._type == CField::eDateTime)
			{
				cTemp = "";
			}
			else if ( m_pSelectColumns->m_szFieldData[iColumn]._type == CField::eString)
			{
				cTemp = m_pSelectColumns->m_szFieldData[iColumn].name;
			}
			else
			{
				// CFiled* pf = GetColScheme(iColumn);
				cTemp = m_pSelectColumns->m_szFieldData[iColumn].low;
			}
		}
		else						// copy the element
		{
			cTemp = m_cListCtrl.GetItemText(m_iSelected, iColumn);
		}
		m_cListCtrl.SetItemText(iItem, iColumn, cTemp);
	}
	return iItem;

}



void CSelectMeasurement::FinishNewDeclaration(bool bRetain, int iItem)
{

	if (bRetain)
	{
		AddToGlobals(iItem);
		m_cListCtrl.EnsureVisible(iItem, false);
		UpdateTheGraphsAndGlobals();
		m_bWriteMP = true;
	}
	else
	{
		m_cListCtrl.DeleteItem(iItem);
	}

}

void CSelectMeasurement::OnAddNewDeclaration() 
{
	if (!m_bETStat)  // editable state is available
	{
		return;
	}

	int iItem = StartNewDeclaration();

	FieldTypeCheck::_EditableRowInfo ri;
	ri.bNew = true;
	int ret = EditRow(iItem, ri);

	FinishNewDeclaration (ret == IDOK, iItem);

}

// TODO 447/448 
// 1: create global with RGB colours mapping to the graphs bitmap colors: RGB		g_MPStatusColour[COUNT]; count them in IDB_BMP_ALL, put this in DataColouring.h
// 2: create global enum mapping the named states into the IDB_BMP_ALL indexed images (maybe this already exists somewhere?)
// 3: set list BKG color for each row in code based on cMyLIstCtrl and EventDlg, take into accoutn teh alternate shading used
void CSelectMeasurement::UpdateRow(bool bNew, int iItem, int iData)
{
	CString cTemp;

	// TODO 447/448 See NewEventDlg::NewEntryRow for use of bkg and text color approach

	cTemp.Format(GetColScheme(CMeaCol::eItem)->format,iData+1);
	if (bNew)
		m_cListCtrl.InsertItem(iItem,cTemp,g_iMPStatus[iData]);  // item index, text, image

	m_cListCtrl.SetItemData(iItem,iData);

	m_cListCtrl.SetItemText(iItem,CMeaCol::eFacility,g_szMPFacilityNames[iData]);
	m_cListCtrl.SetItemText(iItem,CMeaCol::eID,g_szMPItemIDs[iData]);

	cTemp.Format(GetColScheme(CMeaCol::eMeasType)->format,g_iMPMeasurementType[iData]);
	m_cListCtrl.SetItemText(iItem,CMeaCol::eMeasType,cTemp);

	cTemp.Format(GetColScheme(CMeaCol::eEnrichment)->format,g_fMPEnrichmentPercent[iData]);
	m_cListCtrl.SetItemText(iItem,CMeaCol::eEnrichment,cTemp);

	cTemp.Format(GetColScheme(CMeaCol::eBurnUp)->format,g_fMPBurnUp[iData]);
	m_cListCtrl.SetItemText(iItem,CMeaCol::eBurnUp,cTemp);

	if (g_iMPDischargeDateYear[iData] != 0 && 
		g_iMPDischargeDateMonth[iData] != 0 && 
		g_iMPDischargeDateDay[iData] != 0)
	{
		cTemp.Format(CFieldDate::DateFormat,
			g_iMPDischargeDateYear[iData],
			g_iMPDischargeDateMonth[iData],
			g_iMPDischargeDateDay[iData]);
		m_cListCtrl.SetItemText(iItem,CMeaCol::eDischDate,cTemp);
	}

	cTemp.Format(GetColScheme(CMeaCol::eCycle)->format,g_iMPCycle[iData]);
	m_cListCtrl.SetItemText(iItem,CMeaCol::eCycle,cTemp);


	if (g_iMPMeasurementDateYear[iData] != 0 &&
		g_iMPMeasurementDateMonth[iData] != 0 &&
		g_iMPMeasurementDateDay[iData] != 0)
	{
		cTemp.Format(CFieldDate::DateFormat,
			CLAMP(g_iMPMeasurementDateYear[iData],1950,2102),
			CLAMP(g_iMPMeasurementDateMonth[iData],1,12),
			CLAMP(g_iMPMeasurementDateDay[iData],1,31));
		m_cListCtrl.SetItemText(iItem,CMeaCol::eMeasDate,cTemp);
	}

	cTemp.Format(GetColScheme(CMeaCol::eCoolingTime)->format,g_fMPCoolingTime[iData]);
	m_cListCtrl.SetItemText(iItem,CMeaCol::eCoolingTime,cTemp);

	cTemp.Format(GetColScheme(CMeaCol::eNeutronA)->format,g_fMPNChanA[iData]);
	m_cListCtrl.SetItemText(iItem,CMeaCol::eNeutronA,cTemp);

	cTemp.Format(GetColScheme(CMeaCol::eNeutronB)->format,g_fMPNChanB[iData]);
	m_cListCtrl.SetItemText(iItem,CMeaCol::eNeutronB,cTemp);

	cTemp.Format(GetColScheme(CMeaCol::eNeutronC)->format,g_fMPNChanC[iData]);
	m_cListCtrl.SetItemText(iItem,CMeaCol::eNeutronC,cTemp);

	cTemp.Format(GetColScheme(CMeaCol::eGamma1)->format,g_fMPGDose1[iData]);
	m_cListCtrl.SetItemText(iItem,CMeaCol::eGamma1,cTemp);

	cTemp.Format(GetColScheme(CMeaCol::eGamma2)->format,g_fMPGDose2[iData]);
	m_cListCtrl.SetItemText(iItem,CMeaCol::eGamma2,cTemp);

	m_cListCtrl.SetItemText(iItem,CMeaCol::eDetector,g_szMPDetectorIDs[iData]);


	cTemp.Format(GetColScheme(CMeaCol::eThresholdA)->format,g_fMPNChanAThresh[iData]);
	m_cListCtrl.SetItemText(iItem,CMeaCol::eThresholdA,cTemp);

	cTemp.Format(GetColScheme(CMeaCol::eThresholdB)->format,g_fMPNChanBThresh[iData]);
	m_cListCtrl.SetItemText(iItem,CMeaCol::eThresholdB,cTemp);

	cTemp.Format(GetColScheme(CMeaCol::eInternalIndex)->format,g_iMPItemIndices[iData]);
	m_cListCtrl.SetItemText(iItem,CMeaCol::eInternalIndex,cTemp);
}

int CSelectMeasurement::GetInternalIndex(int row)
{
	CString cString;
	int iIndex;
	cString = m_cListCtrl.GetItemText(row, CMeaCol::eInternalIndex);
	iIndex = atoi(cString);

	return iIndex;
}

void CSelectMeasurement::SelectItem(int rindex)
{
	LVFINDINFO info;
	info.flags =  LVFI_PARAM;
	info.lParam = rindex;
	int i = m_cListCtrl.FindItem(&info);
	m_cListCtrl.SelectRow(i);
}
// TODO 447/448 
// 1: create global with RGB colors mapping to the graphs bitmap colors: RGB		g_MPStatusColour[COUNT]; count them in IDB_BMP_ALL, add to DataColouring.h
// 2: create global enum mapping the named states into the IDB_BMP_ALL indexed images (maybe this already exists somewhere?)
// 3: set list BKG color foreach row in code based on cMyLIstCtrl and EventDlg, take into account the alternate shading used


void CSelectMeasurement::UpdateIconsAndIndex() {
	//step through all of the display and modify the colored icons if needed
	for (int j = 0; j < m_cListCtrl.GetItemCount(); j++)
	{
		int iIndex = GetInternalIndex(j);
		LVITEM lvItem;
		lvItem.mask = LVIF_IMAGE;
		lvItem.iItem = j;
		lvItem.iSubItem = 0;
		m_cListCtrl.GetItem(&lvItem);
		//if (lvItem.iImage != g_iMPStatus[iIndex])
		//{
		m_cListCtrl.SetItem(j, 0,LVIF_IMAGE, NULL, g_iMPStatus[iIndex], NULL, NULL, NULL);
		//}
	}
};

void CSelectMeasurement::OnColumnclickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	//this is the column selected to sort on.

	Selected = -1;
	SortTextItems(pNMListView->iSubItem,m_pSelectColumns->m_bAscending[pNMListView->iSubItem]);
	if (Selected >= 0)
		m_cListCtrl.SetSelectionMark(Selected);

	LVCOLUMN col;
	char szText[64];
	for (int i = 0; i < m_pSelectColumns->count(); i++)
	{
		if (i == pNMListView->iSubItem)
		{
			if (m_pSelectColumns->m_bAscending[i])
				strcpy(szText,"^");
			else
				strcpy(szText,"v");
			strcat(szText,m_pSelectColumns->m_szFieldData[i].name);
		}
		else
			strcpy(szText,m_pSelectColumns->m_szFieldData[i].name);

		col.mask = LVCF_TEXT;
		col.pszText = szText;
		m_cListCtrl.SetColumn(i,&col);
	}
	m_pSelectColumns->m_bAscending[pNMListView->iSubItem] = !m_pSelectColumns->m_bAscending[pNMListView->iSubItem];

	*pResult = 0;
}



// SortTextItems	- Sort the list based on column text
// Returns		- Returns true for success
// nCol			- column that contains the text to be sorted
// bAscending		- indicate sort order
// low			- row to start scanning from - default row is 0
// high			- row to end scan. -1 indicates last row
BOOL CSelectMeasurement::SortTextItems( int nCol, BOOL bAscending, 
									   int low /*=0*/, int high/*=-1*/ )
{
	if( nCol >= ((CHeaderCtrl*)(m_cListCtrl.GetDlgItem(0)))->GetItemCount() )
		return FALSE;

	if( high == -1 ) high = m_cListCtrl.GetItemCount() - 1;

	int lo = low;
	int hi = high;
	CString midItem;

	if( hi <= lo ) return FALSE;

	midItem = m_cListCtrl.GetItemText( (lo+hi)/2, nCol );

	// loop through the list until indices cross
	while( lo <= hi )
	{
		// rowText will hold all column text for one row
		CStringArray rowText;

		// find the first element that is greater than or equal to 
		// the partition element starting from the left Index.
		if( bAscending )
			while( ( lo < high ) && ( m_cListCtrl.GetItemText(lo, nCol) < midItem ) )
				++lo;
		else
			while( ( lo < high ) && ( m_cListCtrl.GetItemText(lo, nCol) > midItem ) )
				++lo;

		// find an element that is smaller than or equal to 
		// the partition element starting from the right Index.
		if( bAscending )
			while( ( hi > low ) && ( m_cListCtrl.GetItemText(hi, nCol) > midItem ) )
				--hi;
		else
			while( ( hi > low ) && ( m_cListCtrl.GetItemText(hi, nCol) < midItem ) )
				--hi;

		// if the indexes have not crossed, swap
		// and if the items are not equal
		if( lo <= hi )
		{
			// swap only if the items are not equal
			if( m_cListCtrl.GetItemText(lo, nCol) != m_cListCtrl.GetItemText(hi, nCol))
			{
				// swap the rows
				LV_ITEM lvitemlo, lvitemhi;
				int nColCount = 
					((CHeaderCtrl*)(m_cListCtrl.GetDlgItem(0)))->GetItemCount();
				rowText.SetSize( nColCount );
				int i;
				for( i=0; i<nColCount; i++)
					rowText[i] = m_cListCtrl.GetItemText(lo, i);
				lvitemlo.mask = LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
				lvitemlo.iItem = lo;
				lvitemlo.iSubItem = 0;
				lvitemlo.stateMask = LVIS_CUT | LVIS_DROPHILITED | 
					LVIS_FOCUSED |  LVIS_SELECTED | 
					LVIS_OVERLAYMASK | LVIS_STATEIMAGEMASK;

				lvitemhi = lvitemlo;
				lvitemhi.iItem = hi;

				m_cListCtrl.GetItem( &lvitemlo );
				m_cListCtrl.GetItem( &lvitemhi );
				if (lvitemlo.state & LVIS_SELECTED)
					Selected = hi;
				if (lvitemhi.state & LVIS_SELECTED)
					Selected = lo;

				for( i=0; i<nColCount; i++)
					m_cListCtrl.SetItemText(lo, i, m_cListCtrl.GetItemText(hi, i));

				lvitemhi.iItem = lo;
				m_cListCtrl.SetItem( &lvitemhi );

				for( i=0; i<nColCount; i++)
					m_cListCtrl.SetItemText(hi, i, rowText[i]);

				lvitemlo.iItem = hi;
				m_cListCtrl.SetItem( &lvitemlo );
			}

			++lo;
			--hi;
		}
	}

	// If the right index has not reached the left side of array
	// must now sort the left partition.
	if( low < hi )
		SortTextItems( nCol, bAscending , low, hi);

	// If the left index has not reached the right side of array
	// must now sort the right partition.
	if( lo < high )
		SortTextItems( nCol, bAscending , lo, high );

	return TRUE;
}
void CSelectMeasurement::OnCancel()
{
	if (m_bWriteMP)
	{
		CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
		pApp->WriteMeasurementPlan(); // write updated data to plan file
	}
	CDialog::OnCancel();
}

void CSelectMeasurement::OnOK() 
{
	m_bBlock = true;
	ShowWindow(SW_HIDE);

	CDoAssay* pDoAssay = NULL;

	try
	{
		CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
		if (pApp->m_bDemoVersion)
			pDoAssay = new DemoDoAssay();
		else
			pDoAssay = new CDoAssay();

		pDoAssay->m_iMode = eNormalAssay;//NORMAL
		m_iSelected = -1;
		if ((m_iSelected = m_cListCtrl.GetSelectionMark()) >= 0)
		{
			CString cString;
			cString = m_cListCtrl.GetItemText(m_iSelected,CMeaCol::eID);

			int iIndex = GetInternalIndex(m_iSelected);

			int iTakeNewMeasurement = IDYES;

			if (g_iMPStatus[iIndex] != eUnmeasured)  // > 0 < 8
			{

				iTakeNewMeasurement = AfxMessageBox(
					"This item has already been measured!\n\n"
					"Do you want to remeasure it?\n\n"
					"Click on \"Yes\" to remeasure it.\n"
					"Click on \"No\" to set it as the most\n"
					"most recent measurement but don't remeasure.\n"
					"Click on \"Cancel\" to return.",MB_YESNOCANCEL|MB_ICONSTOP);
			}

			if (iTakeNewMeasurement == IDNO)
			{

				for (int i = 0; i < g_iMPEntriesIn; i++)
				{
					if (g_iMPStatus[i] == eMostRecentNoFits
						||	g_iMPStatus[i] == eMostRecentGood
						|| g_iMPStatus[i] == eMostRecentSuspect)
					{
						g_iMPStatus[i] += 3;
					}

				}
				g_iMPIndexMostRecentMeasurement = iIndex+1;
				//set the status of the new measurement
				g_iMPStatus[iIndex] = eMostRecent;
				g_bPlotMostRecent = true;

				UpdateTheGraphsAndGlobals();

				((CFDMSApp*)AfxGetApp())->WriteMeasurementPlan(); // write updated data to plan file now, not later
				m_bWriteMP = false;

			}
			else if (iTakeNewMeasurement == IDYES)
			{

				// jfl: this is the entry point for a review operation invoked from an external source

				//tell assay what we want done
				SMeasurementData sMeasurementData;
				sMeasurementData.iResult = 0;
				sMeasurementData.iType = 0;
				sMeasurementData.iTime = GETPROFINT(eCYCLETIME);
				sMeasurementData.iIonHV = GETPROFINT(eICHVBIAS);
				sMeasurementData.iHeHV = GETPROFINT(eHVBIAS);
				sMeasurementData.iPoints = GETPROFINT(eCYCLES);

				//give the data to doassay
				pDoAssay->m_psMeasurementData = &sMeasurementData;

				CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
				if (pApp->m_bDemoVersion)
				{
					char szTemp[16];
					GETPROFSTR(ePASSIVE_SYM,szTemp,sizeof(szTemp));
					if (toupper(szTemp[0]) == 'N')
						pDoAssay->m_dAssayIndex = iIndex;
				}


				//bring up the dialog and take a measurement
				if (pDoAssay->DoModal(cString)==IDOK)
				{
					if (pDoAssay->m_bPass)
					{
						//set up to ask if accept or reject
						int cht;
						//set up the dialog's text
						CAcceptReject cAcceptReject;
						cAcceptReject.m_cDialogLabel = "Background Corrected Results";
						cAcceptReject.m_cLabelNew	 = "New";
						cAcceptReject.m_cLabelPrev	 = "Previous";
						cAcceptReject.m_cLabel[NeutA]	= "Neutron Chan A:";
						cAcceptReject.m_cLabel[NeutB]	= "Neutron Chan B:";
						cAcceptReject.m_cLabel[NeutC]	= "Neutron Chan C:";
						cAcceptReject.m_cLabel[Gamma1]	= "Gamma Chan 1:";
						cAcceptReject.m_cLabel[Gamma2]	= "Gamma Chan 2:";

						//setup the previous measurement's data
						CString cTemp;
						double dTemp;
						for (cht = NeutA; cht < MaxChannels; cht++)
						{
							cTemp = m_cListCtrl.GetItemText( m_iSelected, CMeaCol::eNeutronA + cht );
							sscanf(cTemp, "%lf", &dTemp);
							cAcceptReject.m_cPrev[cht].Format("%.3lf",dTemp);
						}

						//average across all of the measurements
						ChannelCounts dAve;
						ProcessMeasurement::ComputeMeasurement(dAve);

						//format and setup the new measurements
						if (pApp->m_bDemoVersion)// put the original stuff into the current slots!
						{
							for (cht = NeutA; cht < MaxChannels; cht++)
							{
									sscanf((LPCTSTR)(cAcceptReject.m_cPrev[cht]), "%lf", &dTemp);
									dAve.val[cht] = dTemp;
									cTemp.Format("%.3lf",dAve.val[cht]);
									cAcceptReject.m_cNew[cht] = cTemp;
							}
						}

						//ask the user if accept or reject
						if (cAcceptReject.DoModal()==IDOK)
						{
							AddMeasurement(dAve, iIndex);
						}
						else
						{
							//						g_iMPMeasurementStat[iIndex] = eUnmeasured;
						}
					}
				}
				else
				{
					COleDateTime cNow = COleDateTime::GetCurrentTime();
					// assay cancelled or failed
				}
			}
		}
	}
	catch (...)
	{
	}

	if (pDoAssay)
		delete pDoAssay;
	m_bBlock = false;
	ShowWindow(SW_SHOW);
}


void CSelectMeasurement::AddMeasurement(const ChannelCounts& dAve, int iIndex)
{
	COleDateTime cNow = COleDateTime::GetCurrentTime();
	CString cNowString;
	cNowString = cNow.Format("%Y.%m.%d");
	m_iSelected = m_cListCtrl.GetSelectionMark();
	m_cListCtrl.SetItemText(m_iSelected,CMeaCol::eMeasDate,cNowString);

	ProcessMeasurement::AddMeasurement(dAve, iIndex, cNow);

	UpdateTheGraphsAndGlobals(iIndex);

	((CFDMSApp*)AfxGetApp())->WriteMeasurementPlan(); // write updated data to plan file now, not later
	m_bWriteMP = false;

	// save the individual measurement event to the log file
	((CFDMSApp*)AfxGetApp())->UpdateMeasurements(g_iMPIndexMostRecentMeasurement);

}

void CSelectMeasurement::OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if (pNMListView->uChanged & LVIF_STATE)
	{
		if (pNMListView->uNewState != 0)
			GetDlgItem(IDOK)->EnableWindow(true);
		else
			GetDlgItem(IDOK)->EnableWindow(false);
	}
	*pResult = 0;
}

void CSelectMeasurement::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	if(m_bInitialized && cx > 0 && cy > 130)
	{
		KillTimer(INVALIDATETIMER);
		int iY_Offset = cy - m_OldRect.bottom;
		int iX_Offset = cx - m_OldRect.right;

		CRect rect, rect2, rect3;
		CRect CurRect;
		CWnd *item;

		// stop leftwardmovement at edge of IDOK button
		item = GetDlgItem(ID_EDIT_TOGGLE);
		item->GetWindowRect(&rect2);
		ScreenToClient(&rect2);

		item = GetDlgItem(ID_EDIT_TOGGLE3);
		item->GetWindowRect(&rect3);
		ScreenToClient(&rect3);

		item = GetDlgItem(IDOK);
		item->GetWindowRect(&rect);
		ScreenToClient(&rect);

	//	if ((rect.right >= (rect2.left + iX_Offset)) && (iX_Offset < 0))
	//		iX_Offset = 0;

	//	GetClientRect(&CurRect);
	//	ScreenToClient(&CurRect);
	//	if ((CurRect.left <= rect3.right)) // do not let the right hand edge cover the rightmost button
	//		iX_Offset = 0;

		item->MoveWindow(rect.left, rect.top + iY_Offset, rect.Width(),
			rect.Height(),TRUE);


		item = GetDlgItem(ID_EDIT_TOGGLE);
		item->GetWindowRect(&rect);
		ScreenToClient(&rect);
		item->MoveWindow(rect.left + iX_Offset, rect.top + iY_Offset,rect.Width(),
			rect.Height(),TRUE);


		item = GetDlgItem(IDC_LIST1);
		item->GetWindowRect(&rect);
		ScreenToClient(&rect);
		item->MoveWindow(rect.left, rect.top,rect.Width() + iX_Offset, 
			rect.Height() + iY_Offset,TRUE);

	
		item = GetDlgItem(IDCANCEL);
		item->GetWindowRect(&rect);
		ScreenToClient(&rect);
		item->MoveWindow(rect.left, rect.top + iY_Offset,rect.Width(),
			rect.Height(),TRUE);


		item = GetDlgItem(ID_EDIT_TOGGLE3);
		item->GetWindowRect(&rect);
		ScreenToClient(&rect);
		item->MoveWindow(rect.left + iX_Offset,rect.top + iY_Offset,rect.Width(),
			rect.Height(),TRUE);

		item = GetDlgItem(ID_ADD_NEW_M);
		item->GetWindowRect(&rect);
		ScreenToClient(&rect);
		item->MoveWindow(rect.left + iX_Offset, rect.top + iY_Offset,rect.Width(),
			rect.Height(),TRUE);

		item = GetDlgItem(ID_DEL_SEL_M);
		item->GetWindowRect(&rect);
		ScreenToClient(&rect);
		item->MoveWindow(rect.left + iX_Offset, rect.top + iY_Offset,rect.Width(),
			rect.Height(),TRUE);

		GetClientRect(&m_OldRect);
		//invalidate
		SetTimer(INVALIDATETIMER,100,NULL);
	}

}

void CSelectMeasurement::OnTimer(UINT nIDEvent) 
{

	if(nIDEvent == INVALIDATETIMER)
	{
		Invalidate(TRUE);
		KillTimer(INVALIDATETIMER);
	}

	CDialog::OnTimer(nIDEvent);
}

CField* CSelectMeasurement::GetColScheme(UINT iColumnID)
{
	CField* pf = ((CField*)(m_pSelectColumns->GetAt(iColumnID)));
	return pf;
}

CMeaCol* CSelectMeasurement::m_pSelectColumns;




void CSelectMeasurement::OnBnClickedDelSelM()
{
	if (m_bETStat)  // editable state is available
	{
		OnRemoveDeclaration();
	}
}

void CSelectMeasurement::OnEditToggle() 
{
	if ((m_iSelected = m_cListCtrl.GetSelectionMark()) >= 0)
		OnEditRow(m_iSelected, 0);
}



void CSelectMeasurement::OnBnClickedEditToggle3()
{
	m_bETStat = !m_bETStat;
	int show;
	if (m_bETStat)
	{
		show = SW_SHOW;
		SetDlgItemText(ID_EDIT_TOGGLE3, "Do Not Edit Declarations");
		m_cListCtrl.ModifyStyle(0, LVS_EDITLABELS);
		m_cListCtrl.ModifyStyleEx(0, WS_EX_CLIENTEDGE | WS_EX_DLGMODALFRAME);	
	}
	else
	{
		show = SW_HIDE;
		SetDlgItemText(ID_EDIT_TOGGLE3, "Edit Declarations");
		m_cListCtrl.ModifyStyle(LVS_EDITLABELS, 0);
		m_cListCtrl.ModifyStyleEx(WS_EX_CLIENTEDGE | WS_EX_DLGMODALFRAME, 0);
	}
	GetDlgItem(ID_ADD_NEW_M)->ShowWindow(show);
	GetDlgItem(ID_EDIT_TOGGLE)->ShowWindow(show);
	GetDlgItem(ID_DEL_SEL_M)->ShowWindow(show);}



void CSelectMeasurement::OnDestroy()
{
	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
	pApp->SetMeasurementDialogPtr(NULL);
	CDialog::OnDestroy();
}

int CSelectMeasurement::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}
