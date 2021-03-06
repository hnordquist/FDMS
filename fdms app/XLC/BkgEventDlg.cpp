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
�	Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
disclaimer. 
�	Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
disclaimer in the documentation and/or other materials provided with the distribution. 
�	Neither the name of Los Alamos National Security, LLC, Los Alamos National Laboratory, LANL, the U.S. Government, 
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
#include "BkgEventDlg.h"
#include "..\datacolouring.h"

#ifdef _XJDEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



///////////////////////////////////////////////////////////////////////////////
// BkgEventDlg dialog

BEGIN_MESSAGE_MAP(BkgEventDlg, EventDlg)
	ON_BN_CLICKED(ID_ANALYZE_IMP, OnBnClickedAnalyzeImp)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// ctor
BkgEventDlg::BkgEventDlg(bool bAllNew, CObList *pList,CObList *pBkgList, CWnd* pParent /*=NULL*/) 
: EventDlg(bAllNew, pList, pBkgList, pParent)
{
	m_DClass = "BKG";
}

BkgEventDlg::~BkgEventDlg()
{
}



///////////////////////////////////////////////////////////////////////////////
// FillListCtrl
void BkgEventDlg::FillListCtrl(XListCtrl& list)
{
	EventDlg::FillListCtrl(list);
	// hide unused columns
	for (int i = 0; i < m_pSelectColumns->count(); i++)
	{
		if (
		(i == ImpEditCol::eItem)
		||
		(i >= ImpEditCol::eMeasType && i <= ImpEditCol::eCycle)
		||
		(i == ImpEditCol::eCoolingTime)
		||
		(i >= ImpEditCol::eThresholdA && i <= ImpEditCol::eInternalIndex))
		{
			LVCOLUMN col;

			col.mask = LVCF_WIDTH;
			if (m_List.GetColumn(i, &col))
			{
				col.cx = 0;
				m_List.SetColumn(i, &col);
			}
		}
	}

	SetWindowText("New Background Measurements");

	list.LockWindowUpdate();	// ***** lock window updates while filling list *****

	try
	{
		int iNewItem = 0;
		POSITION pos = m_pEvList->GetHeadPosition();
		while( pos != NULL )
		{
			MEvent* p = (MEvent*)m_pEvList->GetNext( pos );
			if (p->GetAssayType() == eBackgroundAssay) // it is a background measurement
			{
				PopulateBasicRow(list, iNewItem, -1, p);  // create entry with all known info, let user fill the rest in and affirm on each row
				iNewItem++;
				AddOutputLine(p->Image());
			}
		}
	}
	catch (...)
	{
	}
	list.UnlockWindowUpdate();	// ***** unlock window updates *****
}

void BkgEventDlg::PopulateBasicRow(XListCtrl& list, int iItem, int iData, MEvent* p, const bool bNew)
{
	CString cTemp;
	try
	{
		list.InsertItem(iItem," ");  // item index, text, image
		list.SetCheckbox(iItem, ImpEditCol::eCheck, XHEADERCTRL_NO_IMAGE);
		list.SetItemToolTipText(iItem, ImpEditCol::eCheck, GetColScheme(ImpEditCol::eCheck)->tooltipTemplate);


		cTemp.Format("--");
		list.SetItemText(iItem,ImpEditCol::eItem, cTemp);  // item index, text, image, text color, bg color
		list.SetItemToolTipText(iItem, ImpEditCol::eItem, GetColScheme(ImpEditCol::eItem)->tooltipTemplate);

		list.SetComboBox(iItem, ImpEditCol::eFacility, 1, &m_f, 5, m_f.GetIndexOf(p->FacilityShortName));
		list.SetItemToolTipText(iItem, ImpEditCol::eFacility, GetColScheme(ImpEditCol::eFacility)->tooltipTemplate);

		list.SetItemText(iItem,ImpEditCol::eID,p->m_ID);
		list.SetItemToolTipText(iItem, ImpEditCol::eID, GetColScheme(ImpEditCol::eID)->tooltipTemplate);

		CFieldDate::FormatForOutput (p->GetRadEvent()->m_Times[RADEvent::eEndTimestamp], cTemp);
		list.SetCalCtrl(iItem,ImpEditCol::eMeasDateTime,p->GetRadEvent()->m_Times[RADEvent::eEndTimestamp], cTemp);
		list.SetItemToolTipText(iItem, ImpEditCol::eMeasDateTime, GetColScheme(ImpEditCol::eMeasDateTime)->tooltipTemplate);

		cTemp.Format(GetColScheme(ImpEditCol::eNeutronA)->format,0.0);
		list.SetItemText(iItem,ImpEditCol::eNeutronA,cTemp);
		list.SetItemToolTipText(iItem, ImpEditCol::eNeutronA, GetColScheme(ImpEditCol::eNeutronA)->tooltipTemplate);

		cTemp.Format(GetColScheme(ImpEditCol::eNeutronB)->format,0.0);
		list.SetItemText(iItem,ImpEditCol::eNeutronB,cTemp);
		list.SetItemToolTipText(iItem, ImpEditCol::eNeutronB, GetColScheme(ImpEditCol::eNeutronB)->tooltipTemplate);

		cTemp.Format(GetColScheme(ImpEditCol::eNeutronC)->format,0.0);
		list.SetItemText(iItem,ImpEditCol::eNeutronC,cTemp);
		list.SetItemToolTipText(iItem, ImpEditCol::eNeutronC, GetColScheme(ImpEditCol::eNeutronC)->tooltipTemplate);

		cTemp.Format(GetColScheme(ImpEditCol::eGamma1)->format,0.0);
		list.SetItemText(iItem,ImpEditCol::eGamma1,cTemp);
		list.SetItemToolTipText(iItem, ImpEditCol::eGamma1, GetColScheme(ImpEditCol::eGamma1)->tooltipTemplate);

		cTemp.Format(GetColScheme(ImpEditCol::eGamma2)->format,0.0);
		list.SetItemText(iItem,ImpEditCol::eGamma2,cTemp);
		list.SetItemToolTipText(iItem, ImpEditCol::eGamma2, GetColScheme(ImpEditCol::eGamma2)->tooltipTemplate);

		list.SetComboBox(iItem, ImpEditCol::eDetector, 1, &m_d, 5, m_d.GetIndexOf(p->StationShortName));
		list.SetItemToolTipText(iItem, ImpEditCol::eDetector, GetColScheme(ImpEditCol::eDetector)->tooltipTemplate);

		list.SetItemData(iItem, (DWORD)p);  // the event, the external info, the virtual measurement info and results
	}
	catch (...)
	{
	}
}

void BkgEventDlg::PopulateComputedValues(int iEntry)
{
	MEvent* pm = (MEvent*)m_List.GetItemData(iEntry);

	const int m = iEntry % 2;
	COLORREF text, bkg;

	text = DataColouring::GoodValuesText(m);
	bkg = DataColouring::GoodValuesBkg(m);


	// set validated color across the row
	for (int ic = 0; ic < m_pSelectColumns->count(); ic++)
	{
		m_List.SetItemText(iEntry,ic,NULL, text, bkg);
	}

	// the 5 channels
	CString szDisplayableResult;


	szDisplayableResult.Format(GetColScheme(ImpEditCol::eNeutronC)->format,pm->m_vr._fMPNChanC);
	m_List.SetItemText(iEntry, ImpEditCol::eNeutronC, szDisplayableResult, text, bkg);

	szDisplayableResult.Format(GetColScheme(ImpEditCol::eNeutronB)->format,pm->m_vr._fMPNChanB);
	m_List.SetItemText(iEntry, ImpEditCol::eNeutronB, szDisplayableResult,  text, bkg);

	szDisplayableResult.Format(GetColScheme(ImpEditCol::eNeutronA)->format,pm->m_vr._fMPNChanA);
	m_List.SetItemText(iEntry, ImpEditCol::eNeutronA, szDisplayableResult,  text, bkg);

	szDisplayableResult.Format(GetColScheme(ImpEditCol::eGamma1)->format,pm->m_vr._fMPGDose1);
	m_List.SetItemText(iEntry, ImpEditCol::eGamma1, szDisplayableResult, text, bkg);

	szDisplayableResult.Format(GetColScheme(ImpEditCol::eGamma2)->format,pm->m_vr._fMPGDose2);
	m_List.SetItemText(iEntry, ImpEditCol::eGamma2, szDisplayableResult,  text, bkg);


}


//the measurement results need to be written into the fields after this operation for the user to see

void BkgEventDlg::OnBnClickedAnalyzeImp()
{
	bool deselected = false;
	for (int nItem = 0; nItem < m_List.GetItemCount(); nItem++)
	{
		if (!m_List.GetCheckbox(nItem,ImpEditCol::eCheck)) 
			continue;

		MEvent* pm = (MEvent*)m_List.GetItemData(nItem);
		if (!pm)
			continue;

		if (!deselected)
		{
			m_List.SelectAll(false);
			deselected = true;
		}

		pm->ComputeMeasurement();
		pm->ComputeBackgroundMeasurement();
		PopulateComputedValues(nItem);
		m_BkgList->AddTail(pm);
		
		AddOutputLine("Computed ", pm->Image());

		GetDlgItem(IDOK)->EnableWindow(true);
    }
}

void BkgEventDlg::OnBnClickedOk()
{
	EventDlg::OnBnClickedOk();
}
