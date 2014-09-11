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
#include "MatchingEventDlg.h"
#include "..\datacolouring.h"

#ifdef _XJDEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



///////////////////////////////////////////////////////////////////////////////
// MatchingEventDlg dialog

BEGIN_MESSAGE_MAP(MatchingEventDlg, EventDlg)
	ON_BN_CLICKED(ID_ANALYZE_IMP, OnBnClickedAnalyzeImp)
	ON_BN_CLICKED(IDOK, OnBnClickedFit)

	ON_WM_DESTROY()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// ctor
MatchingEventDlg::MatchingEventDlg(bool bAllNew, CObList *pList,CObList *pBkgList, CWnd* pParent /*=NULL*/) 
: EventDlg(bAllNew, pList, pBkgList, pParent)
{
//	m_pModeless = NULL;
	m_DClass = "EXA";

}

MatchingEventDlg::~MatchingEventDlg()
{
}

///////////////////////////////////////////////////////////////////////////////
// FillListCtrl
void MatchingEventDlg::FillListCtrl(XListCtrl& list)
{
	EventDlg::FillListCtrl(list);

	SetWindowText("Existing Measurements");

	// hide unused columns
	for (int i = 0; i < m_pSelectColumns->count(); i++)
	{
		if (
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

	list.LockWindowUpdate();	// ***** lock window updates while filling list *****

	try
	{
		int iNewItem = 0;
		POSITION pos = m_pEvList->GetHeadPosition();
		while( pos != NULL )
		{
			MEvent* p = (MEvent*)m_pEvList->GetNext( pos );
			if ((p->GetAssayType() == eNormalAssay) &&
				p->GetMark() && !p->m_bExactMatch[eisMP]) // it is a known measurement, matching something that already exists, but not something that is exactly the same
			{
				// look up known measurement info here
				PopulateBasicRow(list, iNewItem, p->m_iGIndex[eisMP], p);  // create entry with all known info, let user fill the rest in and affirm on each row
				iNewItem++;
			}
		}
	}
	catch (...)
	{
	}
	list.UnlockWindowUpdate();	// ***** unlock window updates *****

//	m_pModeless = new MMMatchDlg(this);
//	if (m_pModeless->Create() == TRUE)
		GetDlgItem(IDOK)->EnableWindow(FALSE);
}

void MatchingEventDlg::PopulateBasicRow(XListCtrl& list, int iItem, int iData, MEvent* p, const bool bNew)
{
	// global existing m data must be assoicated with the event match do this here, or at match time or in another place
	p->CopyFromExistingGlobals(iData);

	// existing description row, disabled
	ExistingEntryRow(list, iItem , iData, p);
	// updated measurement row
	ImportedEntryRow(list, iItem, iData, p);
}

extern PTCHAR tMeasurementTypeImage(tMeasurementType t);

void MatchingEventDlg::ImportedEntryRow(XListCtrl& list, int iItem, int iData, MEvent* p)
{
	CString cTemp;
	try
	{
		iItem *= 2;
		iItem++;
		const int m = (iItem+1) % 2;
		COLORREF bkgclr = DataColouring::EditableBkg(m);
		COLORREF textclr = DataColouring::EditableText(m);

		list.InsertItem(iItem," ");  // item index, text, image
		list.SetCheckbox(iItem, ImpEditCol::eCheck, XHEADERCTRL_NO_IMAGE);
		list.SetItemToolTipText(iItem, ImpEditCol::eCheck, _T("Imported Measurement"));

		cTemp = " "; //.Format(GetColScheme(ImpEditCol::eItem)->format,iData+1);
		list.SetItemText(iItem,ImpEditCol::eItem, cTemp);  // item index, text, image
		list.SetItemImage(iItem,ImpEditCol::eItem, m_NQStatusIconIdx + m_StatusIconIdx); // IMAGE WORK. neutral yellow ? mark icon
		list.SetItemToolTipText(iItem, ImpEditCol::eItem, GetColScheme(ImpEditCol::eItem)->tooltipTemplate);

		list.SetComboBox(iItem, ImpEditCol::eFacility, 1, &m_f, 5, m_f.GetIndexOf(g_szMPFacilityNames[iData]), bkgclr);
		list.SetItemToolTipText(iItem, ImpEditCol::eFacility, GetColScheme(ImpEditCol::eFacility)->tooltipTemplate);

		list.SetItemText(iItem,ImpEditCol::eID,g_szMPItemIDs[iData], textclr, bkgclr);
		list.SetItemToolTipText(iItem, ImpEditCol::eID, GetColScheme(ImpEditCol::eID)->tooltipTemplate);

		list.SetItemText(iItem,ImpEditCol::eMeasType,tMeasurementTypeImage((tMeasurementType)g_iMPMeasurementType[iData]));
		list.SetItemToolTipText(iItem, ImpEditCol::eMeasType, GetColScheme(ImpEditCol::eMeasType)->tooltipTemplate);

		cTemp.Format(GetColScheme(ImpEditCol::eEnrichment)->format,g_fMPEnrichmentPercent[iData]);
		list.SetItemText(iItem,ImpEditCol::eEnrichment,cTemp, textclr, bkgclr);  
		list.SetItemToolTipText(iItem, ImpEditCol::eEnrichment, GetColScheme(ImpEditCol::eEnrichment)->tooltipTemplate);

		cTemp.Format(GetColScheme(ImpEditCol::eBurnUp)->format,g_fMPBurnUp[iData]);
		list.SetItemText(iItem,ImpEditCol::eBurnUp,cTemp, textclr, bkgclr);
		list.SetItemToolTipText(iItem, ImpEditCol::eBurnUp, GetColScheme(ImpEditCol::eBurnUp)->tooltipTemplate);

		cTemp.Format(CFieldDate::DateFormat, g_iMPDischargeDateYear[iData],g_iMPDischargeDateMonth[iData] ,g_iMPDischargeDateDay[iData]); 
		COleDateTime x(g_iMPDischargeDateYear[iData],g_iMPDischargeDateMonth[iData] ,g_iMPDischargeDateDay[iData], 0,0,0);
		list.SetCalCtrl(iItem,ImpEditCol::eDischDate,x, cTemp, bkgclr);
		list.SetItemToolTipText(iItem, ImpEditCol::eDischDate, GetColScheme(ImpEditCol::eDischDate)->tooltipTemplate);
		
		list.SetComboBox(iItem, ImpEditCol::eCycle, 1, &m_c, 5,g_iMPCycle[iData]- 1, bkgclr); // 0-based comb box list
		list.SetItemToolTipText(iItem, ImpEditCol::eCycle, GetColScheme(ImpEditCol::eCycle)->tooltipTemplate);

		cTemp.Format(GetColScheme(ImpEditCol::eThresholdA)->format,g_fMPNChanAThresh[iData]);
		list.SetItemText(iItem,ImpEditCol::eThresholdA,cTemp);
		list.SetItemToolTipText(iItem, ImpEditCol::eThresholdA, GetColScheme(ImpEditCol::eThresholdA)->tooltipTemplate);

		cTemp.Format(GetColScheme(ImpEditCol::eThresholdB)->format,g_fMPNChanBThresh[iData]);
		list.SetItemText(iItem,ImpEditCol::eThresholdB,cTemp);
		list.SetItemToolTipText(iItem, ImpEditCol::eThresholdB, GetColScheme(ImpEditCol::eThresholdB)->tooltipTemplate);

		CFieldDate::FormatForOutput (p->GetRadEvent()->m_Times[RADEvent::eEndTimestamp], cTemp);
		list.SetCalCtrl(iItem,ImpEditCol::eMeasDateTime,p->GetRadEvent()->m_Times[RADEvent::eEndTimestamp], cTemp);
		list.SetItemToolTipText(iItem, ImpEditCol::eMeasDateTime, GetColScheme(ImpEditCol::eMeasDateTime)->tooltipTemplate);

		cTemp.Format(GetColScheme(ImpEditCol::eCoolingTime)->format,0.0);
		list.SetItemText(iItem,ImpEditCol::eCoolingTime,cTemp, textclr, bkgclr);
		list.SetItemToolTipText(iItem, ImpEditCol::eCoolingTime, GetColScheme(ImpEditCol::eCoolingTime)->tooltipTemplate);

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

		list.SetComboBox(iItem, ImpEditCol::eDetector, 1, &m_d,  5, m_d.GetIndexOf(g_szMPDetectorIDs[iData]));
		list.SetItemToolTipText(iItem, ImpEditCol::eDetector, GetColScheme(ImpEditCol::eDetector)->tooltipTemplate);

		list.SetItemData(iItem, (DWORD)p);  // the event, the external info, the virtual measurement info and results
	}
	catch (...)
	{
	}
}
void MatchingEventDlg::ExistingEntryRow(XListCtrl& list, int iItem, int iData, MEvent* p)
{
	CString cTemp;
	try
	{
		iItem *= 2;
		//const int m = iItem % 2;
		//COLORREF bkgclr = DataColouring::EditableBkg(m);
		//COLORREF textclr = DataColouring::EditableText(m);
		//CString tt = GetMatchingMeasurementImage(iData, p);

		list.InsertItem(iItem," ");  // item index, text, image
		list.SetCheckbox(iItem, ImpEditCol::eCheck, -1); // ?? XHEADERCTRL_NO_IMAGE

		cTemp.Format(GetColScheme(ImpEditCol::eItem)->format,iData+1);
		list.SetItemText(iItem,ImpEditCol::eItem, cTemp);  // item index, text, image
		list.SetItemImage(iItem,ImpEditCol::eItem, g_iMPStatus[iData] + m_StatusIconIdx); // IMAGE WORK

		list.SetItemText(iItem,ImpEditCol::eFacility,g_szMPFacilityNames[iData]);

		list.SetItemText(iItem,ImpEditCol::eID,g_szMPItemIDs[iData]);

		list.SetItemText(iItem,ImpEditCol::eMeasType,tMeasurementTypeImage((tMeasurementType)g_iMPMeasurementType[iData]));

		cTemp.Format(GetColScheme(ImpEditCol::eEnrichment)->format,g_fMPEnrichmentPercent[iData]);
		list.SetItemText(iItem,ImpEditCol::eEnrichment,cTemp);  

		cTemp.Format(GetColScheme(ImpEditCol::eBurnUp)->format,g_fMPBurnUp[iData]);
		list.SetItemText(iItem,ImpEditCol::eBurnUp,cTemp);

		cTemp.Format(CFieldDate::DateFormat, g_iMPDischargeDateYear[iData],g_iMPDischargeDateMonth[iData] ,g_iMPDischargeDateDay[iData]); 
		COleDateTime x(g_iMPDischargeDateYear[iData],g_iMPDischargeDateMonth[iData] ,g_iMPDischargeDateDay[iData], 0,0,0);
		list.SetCalCtrl(iItem,ImpEditCol::eDischDate,x, cTemp);
				
		list.SetItemText(iItem,ImpEditCol::eCycle,m_c.GetAt(g_iMPCycle[iData]-1 ));

		cTemp.Format(GetColScheme(ImpEditCol::eThresholdA)->format,g_fMPNChanAThresh[iData]);
		list.SetItemText(iItem,ImpEditCol::eThresholdA,cTemp);

		cTemp.Format(GetColScheme(ImpEditCol::eThresholdB)->format,g_fMPNChanBThresh[iData]);
		list.SetItemText(iItem,ImpEditCol::eThresholdB,cTemp);

		cTemp.Format(CFieldDate::DateFormat, g_iMPMeasurementDateYear[iData],g_iMPMeasurementDateMonth[iData] ,g_iMPMeasurementDateDay[iData]); 
		COleDateTime y(g_iMPMeasurementDateYear[iData],g_iMPMeasurementDateMonth[iData] ,g_iMPMeasurementDateDay[iData], 0,0,0);
		if (g_iMPMeasurementDateYear[iData] == 0)
		{
			list.SetItemText(iItem,ImpEditCol::eMeasDateTime,"");
		}
		else
		{
			list.SetItemText(iItem,ImpEditCol::eMeasDateTime,cTemp);
		}

		cTemp.Format(GetColScheme(ImpEditCol::eCoolingTime)->format,g_fMPCoolingTime[iData]);
		list.SetItemText(iItem,ImpEditCol::eCoolingTime,cTemp);

		cTemp.Format(GetColScheme(ImpEditCol::eNeutronA)->format,g_fMPNChanA[iData]);
		list.SetItemText(iItem,ImpEditCol::eNeutronA,cTemp);

		cTemp.Format(GetColScheme(ImpEditCol::eNeutronB)->format,g_fMPNChanB[iData]);
		list.SetItemText(iItem,ImpEditCol::eNeutronB,cTemp);

		cTemp.Format(GetColScheme(ImpEditCol::eNeutronC)->format,g_fMPNChanC[iData]);
		list.SetItemText(iItem,ImpEditCol::eNeutronC,cTemp);

		cTemp.Format(GetColScheme(ImpEditCol::eGamma1)->format,g_fMPGDose1[iData]);
		list.SetItemText(iItem,ImpEditCol::eGamma1,cTemp);

		cTemp.Format(GetColScheme(ImpEditCol::eGamma2)->format,g_fMPGDose2[iData]);
		list.SetItemText(iItem,ImpEditCol::eGamma2,cTemp);

		list.SetItemText(iItem, ImpEditCol::eDetector, g_szMPDetectorIDs[iData]);
	
		list.SetEnabled(iItem, FALSE);
		list.SetItemData(iItem, (DWORD)p);  // the event, the external info, the virtual measurement info and results

	}
	catch (...)
	{
	}

}
	// TODO NEXT MEvent* pm = (MEvent*)m_List.GetItemData(iEntry);  // use the item data to get the icon status index and update every row in the dialog, do this in the other dialog too (NewEvent)

void MatchingEventDlg::PopulateComputedValues(int iEntry)
{
	MEvent* pm = (MEvent*)m_List.GetItemData(iEntry);
	//what is computed?

	const int m = (iEntry+1) % 2;
	COLORREF text, bkg;
	int ic;

	// clear bold
	for (ic = ImpEditCol::eFacility; ic <= ImpEditCol::eColumnCount; ic++)
	{
		if (GetColumnWriteFlag(ic))
			m_List.SetBold(iEntry,ic,FALSE);
	}

	// now color the row based on the actual current state
	if (pm->m_vr.ValidContent())
	{
		// set the 8 columns of interest to the typical bkg color
		for (ic = ImpEditCol::eFacility; ic < ImpEditCol::eColumnCount; ic++)
		{
			if (GetColumnWriteFlag(ic))
				m_List.SetItemText(iEntry,ic,NULL, DataColouring::EditableText(m), DataColouring::EditableBkg(m));
		}

		text = DataColouring::GoodValuesText(m);
		bkg = DataColouring::GoodValuesBkg(m);

		// set validated color across the row
		for (ic = 0; ic < m_pSelectColumns->count(); ic++)
		{
			m_List.SetItemText(iEntry,ic,NULL, text, bkg);
		}

		// cycle is advisable
		if (!pm->m_vr.m_bEAFChanged[eafCycle])
		{
			m_List.SetItemText(iEntry,ImpEditCol::eCycle,NULL, DataColouring::EditAdvisableText(m), bkg);
		}

		m_List.SetBold(iEntry,ImpEditCol::eID, TRUE);  // highlight the ID as an indicator, too.


		/*
		if (pm->m_vr._iMPMeasurementType == CycleVerification)
		{
		//  you dont have to enter value for these, but it might be advisable
		if (!pm->m_vr.m_bEAFChanged[eafEnrichment] || !(pm->m_vr._fMPEnrichmentPercent > 0.0))
		{
		m_List.SetItemText(iEntry,ImpEditCol::eEnrichment,NULL, DataColouring::EditAdvisableText(m), bkg);
		}
		if (!pm->m_vr.m_bEAFChanged[eafBurnUp] || !(pm->m_vr._fMPEnrichmentPercent > 0.0))
		{
		m_List.SetItemText(iEntry,ImpEditCol::eBurnUp,NULL, DataColouring::EditAdvisableText(m), bkg);
		}
		}
		*/

	}
	else
	{
		text = DataColouring::StaticText(m);
		bkg = DataColouring::StaticBkg(m);

		// set non-validated color bkg across the row
		for (ic = 0; ic < m_pSelectColumns->count(); ic++)
		{
			m_List.SetItemText(iEntry,ic,NULL, text, bkg);
		}
		// set the 8 columns of interest to the typical bkg color
		for (ic = ImpEditCol::eFacility; ic <= ImpEditCol::eColumnCount; ic++)
		{
			if (GetColumnWriteFlag(ic))
				m_List.SetItemText(iEntry,ic,NULL, DataColouring::EditableText(m), DataColouring::EditableBkg(m));
		}
		// now do the notification highlights
		if (pm->m_vr._iMPMeasurementType == AssemblyVerification) 
		{
			// highlight the fields that need to be dealt with differently in WARNINGPINK
			// in this specific situation, the user must enter burnup and enrichment
			if (!(pm->m_vr._fMPEnrichmentPercent > 0.0))
			{
				m_List.SetBold(iEntry,ImpEditCol::eEnrichment, TRUE);
				m_List.SetItemText(iEntry,ImpEditCol::eEnrichment,NULL, DataColouring::MustEditText(m), DataColouring::MustEditBkg(m));
			}
			if (!(pm->m_vr._fMPBurnUp > 0.0))
			{
				m_List.SetBold(iEntry,ImpEditCol::eBurnUp, TRUE);
				m_List.SetItemText(iEntry,ImpEditCol::eBurnUp,NULL, DataColouring::MustEditText(m), DataColouring::MustEditBkg(m));
			}

			// assembly verification: disch date, coolingtime
			bool bNullDisch =  (pm->m_vr.CompareDischargeDate(ImpEditCol::m_DefaultStartDate) == 0);
			bool bNullCT = !(pm->m_vr._fMPCoolingTime > 0.0);
			bool bHighLightDisch = false;
			bool bHighLightCT = false;
			bool bAugmentDisch = false;
			bool bAugmentCT = false;
			if (bNullDisch  && bNullCT)
			{
				bHighLightDisch = bHighLightCT = true;
			}
			else if (bNullDisch)
			{
				bAugmentDisch = true;
			}
			else if (bNullCT)
			{
				bAugmentCT = true;
			}
			if (bHighLightDisch) // no cooling time; must specify disch date 
			{
				// bold
				m_List.SetBold(iEntry,ImpEditCol::eDischDate, TRUE);
				m_List.SetItemText(iEntry,ImpEditCol::eDischDate,NULL, DataColouring::MustEditText(m), DataColouring::MustEditBkg(m));
			}
			if (bAugmentDisch)
				m_List.SetItemText(iEntry,ImpEditCol::eDischDate,NULL, DataColouring::EditAdvisableText(m), DataColouring::EditableBkg(m));
			if (bHighLightCT) // no cooling time; must specify disch date 
			{
				// bold
				m_List.SetBold(iEntry,ImpEditCol::eCoolingTime, TRUE);
				m_List.SetItemText(iEntry,ImpEditCol::eCoolingTime,NULL, DataColouring::MustEditText(m), DataColouring::MustEditBkg(m));
			}
			if (bAugmentCT)
				m_List.SetItemText(iEntry,ImpEditCol::eCoolingTime,NULL, DataColouring::EditAdvisableText(m), DataColouring::EditableBkg(m));
		}  // assembly
		else
		{
			if (pm->m_vr.CompareDischargeDate(ImpEditCol::m_DefaultStartDate) == 0)
			{
				m_List.SetBold(iEntry,ImpEditCol::eDischDate, TRUE);
				m_List.SetItemText(iEntry,ImpEditCol::eDischDate,NULL, DataColouring::MustEditText(m), DataColouring::MustEditBkg(m));
			}		
			if (pm->m_vr._iMPMeasurementType == CycleVerification)
			{
				// you dont have to enter value for these, but it might be advisable
				if (!pm->m_vr.m_bEAFChanged[eafEnrichment] || !(pm->m_vr._fMPEnrichmentPercent > 0.0))
				{
					m_List.SetItemText(iEntry,ImpEditCol::eEnrichment,NULL, DataColouring::EditAdvisableText(m), DataColouring::EditAdvisableBkg(m));
				}
				if (!pm->m_vr.m_bEAFChanged[eafBurnUp] || !(pm->m_vr._fMPEnrichmentPercent > 0.0))
				{
					m_List.SetItemText(iEntry,ImpEditCol::eBurnUp,NULL, DataColouring::EditAdvisableText(m), DataColouring::EditAdvisableBkg(m));
				}
			}
			else
			{
				if (pm->m_vr._iMPMeasurementType == UnspecifiedVerification)
				{
					m_List.SetBold(iEntry,ImpEditCol::eMeasType, TRUE);
					m_List.SetItemText(iEntry,ImpEditCol::eMeasType,NULL, DataColouring::MustEditText(m), DataColouring::MustEditBkg(m));
				}
			}
		}
		int idlen = pm->m_ID.GetLength();
		if (idlen <= 0)
		{
			m_List.SetBold(iEntry,ImpEditCol::eID, TRUE);
			m_List.SetItemText(iEntry,ImpEditCol::eID, m_NoId, DataColouring::MustEditText(m), DataColouring::MustEditBkg(m));
		}
	}
	// specifying a measurement type is always required no matter what
	if (pm->m_vr._iMPMeasurementType == UnspecifiedVerification)
	{
		m_List.SetBold(iEntry,ImpEditCol::eMeasType, TRUE);
		m_List.SetItemText(iEntry,ImpEditCol::eMeasType,NULL, DataColouring::MustEditText(m), DataColouring::MustEditBkg(m));
	}

	// the 5 channels
	CString szDisplayableResult;
	szDisplayableResult.Format(GetColScheme(ImpEditCol::eCoolingTime)->format,pm->m_vr._fMPCoolingTime);
	m_List.SetItemText(iEntry, ImpEditCol::eCoolingTime, szDisplayableResult, text, bkg);

	szDisplayableResult.Format(GetColScheme(ImpEditCol::eNeutronC)->format,pm->m_vr._fMPNChanC);
	m_List.SetItemText(iEntry, ImpEditCol::eNeutronC, szDisplayableResult, text, bkg);

	szDisplayableResult.Format(GetColScheme(ImpEditCol::eNeutronB)->format,pm->m_vr._fMPNChanB);
	m_List.SetItemText(iEntry, ImpEditCol::eNeutronB, szDisplayableResult,text, bkg);

	szDisplayableResult.Format(GetColScheme(ImpEditCol::eNeutronA)->format,pm->m_vr._fMPNChanA);
	m_List.SetItemText(iEntry, ImpEditCol::eNeutronA, szDisplayableResult,text, bkg);

	szDisplayableResult.Format(GetColScheme(ImpEditCol::eGamma1)->format,pm->m_vr._fMPGDose1);
	m_List.SetItemText(iEntry, ImpEditCol::eGamma1, szDisplayableResult, text, bkg);

	szDisplayableResult.Format(GetColScheme(ImpEditCol::eGamma2)->format,pm->m_vr._fMPGDose2);
	m_List.SetItemText(iEntry, ImpEditCol::eGamma2, szDisplayableResult, text, bkg);

}

//the measurement results need to be written into the fields after this operation for the user to see
void MatchingEventDlg::OnBnClickedAnalyzeImp()
{
	bool deselected = false;
	// every second row is an active import row, so we loop through those directly
	for (int nItem = 1; nItem < m_List.GetItemCount(); nItem = nItem + 2)
	{
		if (!m_List.GetCheckbox(nItem,ImpEditCol::eCheck)) 
			continue;
		if (!m_List.GetEnabled(nItem)) 
			continue;

		if (!deselected)
		{
			m_List.SelectAll(false);
			deselected = true;
		}

		MEvent* pm = (MEvent*)m_List.GetItemData(nItem);
		if (!pm)
			continue;
		//if (pm->GetFit()) // already performed with current values
		//	continue;

		pm->SetAccepted(false); // set up button state
		MEvent* pBkg = LookupBkg(pm);
		if (pBkg)
		{
			pm->m_vr.SetBackgroundValues(pBkg->m_vr._fBkgMeasurements, pBkg->EndTime);
			pm->SetBkgPointer(pBkg);
			AddOutputLine("Using BKG ", pBkg->Image());
		}
		pm->ComputeMeasurement();
		PopulateComputedValues(nItem);

		AddOutputLine("Computed ", pm->Image());
	}

	SetOKButtonIfValidContent(true);
}

#include "..\acceptreject.h"

void MatchingEventDlg::OnBnClickedFit()
{

	bool bUpdate = false;
    int iLastIndex = -1;
	
	InitMIndex();

	CAcceptReject cAcceptReject;
	cAcceptReject.m_cDialogLabel = "Background Corrected Results";
	cAcceptReject.m_cLabelNew	 = "New";
	cAcceptReject.m_cLabelPrev	 = "Previous";
	cAcceptReject.m_cLabel[NeutA]	= "Neutron Chan A:";
	cAcceptReject.m_cLabel[NeutB]	= "Neutron Chan B:";
	cAcceptReject.m_cLabel[NeutC]	= "Neutron Chan C:";
	cAcceptReject.m_cLabel[Gamma1]	= "Gamma Chan 1:";
	cAcceptReject.m_cLabel[Gamma2]	= "Gamma Chan 2:";

	// need to skip exact matches here, or filter them out at the onset

	for (int nItem = 0; nItem < m_List.GetItemCount(); nItem++)
	{
		if (!m_List.GetCheckbox(nItem,ImpEditCol::eCheck)) 
			continue;

		MEvent* pm = (MEvent*)m_List.GetItemData(nItem);
		if (!pm)
			continue;
		if (!pm->m_vr.m_bComputed) 
			continue;
		if (!pm->m_vr.ValidContent()) 
			continue;
		//if (pm->GetFit()) // already performed with current values
		//	continue;

		COleDateTime t(pm->EndTime);
		cAcceptReject.m_cLabelTitle.Format("Item '%.16s' measured at %.24s.",
			pm->m_vr._szMPItemID, t.Format(), pm->StationShortName);

		//set up the previous measurement's data
		cAcceptReject.m_cPrev[NeutA].Format("%.3lf",g_fMPNChanA[pm->m_vr._iMPItemIndex]);
		cAcceptReject.m_cPrev[NeutB].Format("%.3lf",g_fMPNChanB[pm->m_vr._iMPItemIndex]);
		cAcceptReject.m_cPrev[NeutC].Format("%.3lf",g_fMPNChanC[pm->m_vr._iMPItemIndex]);
		cAcceptReject.m_cPrev[Gamma1].Format("%.3lf",g_fMPGDose1[pm->m_vr._iMPItemIndex]);
		cAcceptReject.m_cPrev[Gamma2].Format("%.3lf",g_fMPGDose2[pm->m_vr._iMPItemIndex]);

		cAcceptReject.m_cNew[NeutA].Format("%.3lf",pm->m_vr._fMPNChanA);
		cAcceptReject.m_cNew[NeutB].Format("%.3lf",pm->m_vr._fMPNChanB);
		cAcceptReject.m_cNew[NeutC].Format("%.3lf",pm->m_vr._fMPNChanC);
		cAcceptReject.m_cNew[Gamma1].Format("%.3lf",pm->m_vr._fMPGDose1);
		cAcceptReject.m_cNew[Gamma2].Format("%.3lf",pm->m_vr._fMPGDose2);

		//ask the user if accept or reject
		if (cAcceptReject.DoModal()==IDOK)
		{
			// assign the matched MPindex entry number to the m_vr struct
			pm->m_vr._iMPEntryIndex = pm->m_iGIndex[eisMP];
	
			if (pm->m_iGIndex[eisMP] < 0)
				continue;

			// if there are new background measurements applied to this measurement, and said measurements have not been saved out, do so now
			MEvent* pBkg;
			if (pBkg = pm->GetBkgPointer())
			{
				if (!pBkg->GetRetentionMark())
				{
					// save bkg measurement here
					CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
					pApp->UpdateMeasurementsVBG(pBkg->m_vr.cBkgTime, pBkg->m_vr._fBkgMeasurements, pm->m_vr._szMPDetectorID);
					pBkg->SetRetentionMark(true);
				}
			}

			// next copy all the computed values into the global arrays and save the data out to the files
			iLastIndex = pm->m_vr._iMPEntryIndex;
			bUpdate = pm->AddMeasurement();
			if (bUpdate)
				pm->SetFit(true); // an actual round trip through the analysis DLL occurs in the next step below
			m_idxa[nItem] = iLastIndex;
			pm->SetAccepted(true);
		}
    }

	if (bUpdate)
	{
		ExternalMeasurements::AgeStatus(iLastIndex);

		//ExternalMeasurements::Persist(bDBSave, m_idxa);
		for (int nItem=0;nItem <= m_idxa.GetUpperBound();nItem++)  // refresh the icon state for every row
		{
			int mi = m_idxa[nItem];
			// save the individual measurement event to the log file
			if (mi >= 0) // update this item row
			{
				const int statusidx = g_iMPStatus[mi] + m_StatusIconIdx;
				COLORREF bkg = DataColouring::AnalyzedRowBkg(0, statusidx);  // the g_iMPStatus index 
				m_List.SetItemImage(nItem,ImpEditCol::eItem, statusidx, bkg);
				// set specific bkg color across the row
				for (int ic = 0; ic < m_pSelectColumns->count(); ic++)
				{
					m_List.SetBackground(nItem,ic,bkg);
				}
				m_WaitingForPersist = true;
			}
			else
			{
				//MEvent* pm = (MEvent*)m_List.GetItemData(nItem);
				//if (pm)
				//	m_List.SetItemImage(nItem,ImpEditCol::eItem, g_iMPStatus[pm->m_iGIndex[eisMP] + m_StatusIconIdx]);
			}
		}
	}
	m_WaitingForFit = false;
	SetOKButtonIfValidContent(true);
}
void MatchingEventDlg::SaveChanges()
{
	ExternalMeasurements::PersistChanges(false, m_idxa);
	m_WaitingForPersist = false;
}


CString MatchingEventDlg::GetMatchingMeasurementImage(int iData, MEvent* p)
{
	CString cTemp;
	CString s;

	cTemp.Format(GetColScheme(ImpEditCol::eItem)->format,iData+1);
	s.Append("Index: ");
	s.Append(cTemp);
	s.Append("\r\n");

	s.Append("Facility: ");
	s.Append(g_szMPFacilityNames[iData]);
	s.Append("\r\n");

	s.Append("ID: ");
	s.Append(g_szMPItemIDs[iData]);
	s.Append("\r\n");

	s.Append("Type: ");
	s.Append(tMeasurementTypeImage((tMeasurementType)g_iMPMeasurementType[iData]));
	s.Append("\r\n");

	cTemp.Format(GetColScheme(ImpEditCol::eEnrichment)->format,g_fMPEnrichmentPercent[iData]);
	s.Append("Enrich: ");
	s.Append(cTemp);
	s.Append("\r\n");

	cTemp.Format(GetColScheme(ImpEditCol::eBurnUp)->format,g_fMPBurnUp[iData]);
	s.Append("Burnup: ");
	s.Append(cTemp);
	s.Append("\r\n");


	cTemp.Format(CFieldDate::DateFormat,
			g_iMPDischargeDateYear[iData],
			g_iMPDischargeDateMonth[iData],
			g_iMPDischargeDateDay[iData]);
	s.Append("Discharged: ");
	s.Append(cTemp);
	s.Append("\r\n");


	cTemp.Format(GetColScheme(ImpEditCol::eCycle)->format,g_iMPCycle[iData]);
	s.Append("Cycle: ");
	s.Append(cTemp);
	s.Append("\r\n");

	/*cTemp.Format(GetColScheme(ImpEditCol::eThresholdA)->format,g_fMPNChanAThresh[iData]);
	s.Append("TA: ");
	s.Append(cTemp);
	s.Append("\r\n");

	cTemp.Format(GetColScheme(ImpEditCol::eThresholdB)->format,g_fMPNChanBThresh[iData]);
	s.Append("TB: ");
	s.Append(cTemp);
	s.Append("\r\n");
*/
	if (
		(CLAMP(g_iMPMeasurementDateYear[iData],1950,2102)  == ImpEditCol::m_DefaultStartDate.GetYear()) &&
		(CLAMP(g_iMPMeasurementDateMonth[iData],1,12) == ImpEditCol::m_DefaultStartDate.GetMonth()) &&
		(CLAMP(g_iMPMeasurementDateDay[iData],1,31) == ImpEditCol::m_DefaultStartDate.GetDay()))
	{
		s.Append("Never Measured");
	}
	else
	{
		cTemp.Format(CFieldDate::DateFormat,
				CLAMP(g_iMPMeasurementDateYear[iData],1950,2102),
				CLAMP(g_iMPMeasurementDateMonth[iData],1,12),
				CLAMP(g_iMPMeasurementDateDay[iData],1,31));
		s.Append("Measured Date: ");
		s.Append(cTemp);
	}

	s.Append("\r\n");
	

	cTemp.Format(GetColScheme(ImpEditCol::eCoolingTime)->format,g_fMPCoolingTime[iData]);
	s.Append("Cooling: ");
	s.Append(cTemp);
	s.Append("\r\n");

	cTemp.Format(GetColScheme(ImpEditCol::eNeutronA)->format,g_fMPNChanA[iData]);
	s.Append("Neutron A: ");
	s.Append(cTemp);
	s.Append("\r\n");

	cTemp.Format(GetColScheme(ImpEditCol::eNeutronB)->format,g_fMPNChanB[iData]);
	s.Append("Neutron B: ");
	s.Append(cTemp);
	s.Append("\r\n");

	cTemp.Format(GetColScheme(ImpEditCol::eNeutronC)->format,g_fMPNChanC[iData]);
	s.Append("Neutron C: ");
	s.Append(cTemp);
	s.Append("\r\n");

	cTemp.Format(GetColScheme(ImpEditCol::eGamma1)->format,g_fMPGDose1[iData]);
	s.Append("Gamma 1: ");
	s.Append(cTemp);
	s.Append("\r\n");

	cTemp.Format(GetColScheme(ImpEditCol::eGamma2)->format,g_fMPGDose2[iData]);
	s.Append("Gamma 2: ");
	s.Append(cTemp);
	s.Append("\r\n");

	s.Append("Detector: ");
	s.Append(g_szMPDetectorIDs[iData]);
	s.Append("\r\n");

	cTemp.Format(GetColScheme(ImpEditCol::eInternalIndex)->format,g_iMPItemIndices[iData]);
	s.Append(cTemp);

	return s;
}
//LRESULT MatchingEventDlg::OnFatToolTip(WPARAM w, LPARAM l) 
//{
//	m_pModeless->SetXText((LPCSTR)l);
//	return 0;
//}
/*
void MatchingEventDlg::OnMMHover(NMHDR* pNMHDR, LRESULT* pResult)
{

	m_List.OnToolTipText(0, pNMHDR, pResult);
	
	LVHITTESTINFO lvhitTestInfo;
	
	CPoint point;
	::GetCursorPos(&point);
	ScreenToClient(&point);
	lvhitTestInfo.pt = point;
	
	int nItem = ListView_SubItemHitTest(m_List->m_hWnd, &lvhitTestInfo);

	UINT nFlags = lvhitTestInfo.flags;

	// nFlags is 0 if the SubItemHitTest fails
	// Therefore, 0 & <anything> will equal false
	if (nFlags & LVHT_ONITEMLABEL)
	{

	}
	
	*pResult = 0;
}
*/



BOOL MatchingEventDlg::OnInitDialog()
{
	SetColumnWriteFlag(false, ImpEditCol::eMeasType);

	EventDlg::OnInitDialog();
	return TRUE;  
}

void MatchingEventDlg::OnDestroy()
{
	SetColumnWriteFlag(true, ImpEditCol::eMeasType);
	EventDlg::OnDestroy();
}
