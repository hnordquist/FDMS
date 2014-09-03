// XListCtrlTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\FDMS.h"
#include "NewEventDlg.h"
#include "..\datacolouring.h"

#ifdef _XJDEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



///////////////////////////////////////////////////////////////////////////////
// NewEventDlg dialog

BEGIN_MESSAGE_MAP(NewEventDlg, EventDlg)
	ON_BN_CLICKED(ID_ANALYZE_IMP, OnBnClickedAnalyzeImp)
	ON_BN_CLICKED(IDOK, OnBnClickedFit)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// ctor
NewEventDlg::NewEventDlg(bool bAllNew, CObList *pList, CObList *pBkgList, CWnd* pParent /*=NULL*/) 
: EventDlg(bAllNew, pList, pBkgList, pParent), m_mbMeasTypeChoice(true)
{
	m_DClass = "NEW";
}

NewEventDlg::~NewEventDlg()
{
}

bool NewEventDlg::UpdateValue(int iEntry, int iColumnID, LPCSTR pRawString, CString& szDisplayableResult)
{

	EventDlg::UpdateValue(iEntry, iColumnID, pRawString, szDisplayableResult); // column id is ImpEd enum

	MEvent* pm = (MEvent*)m_List.GetItemData(iEntry);
	if (pm)
	{
		// catch id update for UI when changing cooling time/disch date for an assembly verification
		if (CMeaCol::eCoolingTime == ImpEditCol::m_meacolmap[iColumnID])// && pm->m_vr._iMPMeasurementType == AssemblyVerification)
		{
			CString cTemp;

			cTemp.Format(CFieldDate::DateFormat,
				pm->m_vr._iMPDischargeDateYear,
				pm->m_vr._iMPDischargeDateMonth,
				pm->m_vr._iMPDischargeDateDay);

			m_List.SetCalCtrl(iEntry,ImpEditCol::eDischDate,pm->m_vr.GetDischargeDateTime(), cTemp);

		}
		else if (CMeaCol::eDischDate == ImpEditCol::m_meacolmap[iColumnID])// && pm->m_vr._iMPMeasurementType == AssemblyVerification)
		{
			CString cTemp;

			cTemp.Format(GetColScheme(ImpEditCol::eCoolingTime)->format, pm->m_vr._fMPCoolingTime);
			m_List.SetItemText(iEntry,ImpEditCol::eCoolingTime,cTemp);
			
			cTemp.Format(CFieldDate::DateFormat,
				pm->m_vr._iMPDischargeDateYear,
				pm->m_vr._iMPDischargeDateMonth,
				pm->m_vr._iMPDischargeDateDay);

			m_List.SetCalCtrl(iEntry,ImpEditCol::eDischDate,pm->m_vr.GetDischargeDateTime(), cTemp);

		}
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////
// FillListCtrl
void NewEventDlg::FillListCtrl(XListCtrl& list)
{
	EventDlg::FillListCtrl(list);

	// hide unused columns
	for (int i = 0; i < m_pSelectColumns->count(); i++)
	{
		if (i == ImpEditCol::eItem) 
		{
			LVCOLUMN col;

			col.mask = LVCF_WIDTH;
			if (m_List.GetColumn(i, &col))
			{
				col.cx = 2 + 18 + 6;
				m_List.SetColumn(i, &col);
			}
		}
		else if ((i >= ImpEditCol::eThresholdA && i <= ImpEditCol::eInternalIndex))
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
				(!p->GetMark()) && p->m_iGIndex[eisMP] < 0) // it is an unknown measurement, matching
			{
				PopulateBasicRow(list, iNewItem, -1, p);  // create entry with all known info, let user fill the rest in and affirm on each row
				iNewItem++;
			}
		}
	}
	catch (...)
	{
	}
	list.UnlockWindowUpdate();	// ***** unlock window updates *****
}

extern PTCHAR tMeasurementTypeImage(tMeasurementType t);

void NewEventDlg::PopulateBasicRow(XListCtrl& list, int iItem, int iData, MEvent* p, const bool bNew)
{
	CString cTemp;
	try
	{
		const int m = iItem % 2;
		COLORREF bkgclr = DataColouring::EditableBkg(m);
		COLORREF textclr = DataColouring::EditableText(m);

		if (bNew)
		{

		list.InsertItem(iItem," ");  // item index, text, image
		list.SetCheckbox(iItem, ImpEditCol::eCheck, XHEADERCTRL_NO_IMAGE);
		list.SetItemToolTipText(iItem, ImpEditCol::eCheck, GetColScheme(ImpEditCol::eCheck)->tooltipTemplate);

		cTemp.Format(" ");
		list.SetItemText(iItem,ImpEditCol::eItem, cTemp);  // item index, text, image, text color, bg color
		list.SetItemImage(iItem,ImpEditCol::eItem, m_NQStatusIconIdx + m_StatusIconIdx); // neutral yellow ? mark icon see UpdateIconsAndIndex, need to add idx to the update icon index everywhere
		list.SetItemToolTipText(iItem, ImpEditCol::eItem, GetColScheme(ImpEditCol::eItem)->tooltipTemplate);

		list.SetComboBox(iItem, ImpEditCol::eFacility, 1, &m_f, 5, m_f.GetIndexOf(p->FacilityShortName), bkgclr);
		list.SetItemToolTipText(iItem, ImpEditCol::eFacility, GetColScheme(ImpEditCol::eFacility)->tooltipTemplate);

		int idlen = p->m_ID.GetLength();
		if (idlen > 0)
			list.SetItemText(iItem,ImpEditCol::eID,p->m_ID,textclr, bkgclr);
		else
			list.SetItemText(iItem,ImpEditCol::eID,m_NoId, textclr, DataColouring::MustEditBkg(m));
		list.SetItemToolTipText(iItem, ImpEditCol::eID, GetColScheme(ImpEditCol::eID)->tooltipTemplate);

		if (m_mbMeasTypeChoice) // 00399 allow choice only if set, default to Assembly
			list.SetComboBox(iItem, ImpEditCol::eMeasType, 1, &m_m, 5, AssemblyVerification, bkgclr);
		else // else force assembly only
			list.SetItemText(iItem, ImpEditCol::eMeasType, tMeasurementTypeImage(AssemblyVerification), BLACK, bkgclr);
		list.SetItemToolTipText(iItem, ImpEditCol::eMeasType, GetColScheme(ImpEditCol::eMeasType)->tooltipTemplate);

		cTemp.Format(GetColScheme(ImpEditCol::eEnrichment)->format,0.0);
		list.SetItemText(iItem,ImpEditCol::eEnrichment,cTemp, textclr, bkgclr);
		list.SetItemToolTipText(iItem, ImpEditCol::eEnrichment, GetColScheme(ImpEditCol::eEnrichment)->tooltipTemplate);

		cTemp.Format(GetColScheme(ImpEditCol::eBurnUp)->format,0.0);
		list.SetItemText(iItem,ImpEditCol::eBurnUp,cTemp, textclr, bkgclr);
		list.SetItemToolTipText(iItem, ImpEditCol::eBurnUp, GetColScheme(ImpEditCol::eBurnUp)->tooltipTemplate);

		cTemp.Format(CFieldDate::DateFormat, 1950,1,1); 
		list.SetCalCtrl(iItem,ImpEditCol::eDischDate,ImpEditCol::m_DefaultStartDate, cTemp,bkgclr);
		list.SetItemToolTipText(iItem, ImpEditCol::eDischDate, GetColScheme(ImpEditCol::eDischDate)->tooltipTemplate);

		list.SetComboBox(iItem, ImpEditCol::eCycle, 1, &m_c, 5, 0, bkgclr);
		list.SetItemToolTipText(iItem, ImpEditCol::eCycle, GetColScheme(ImpEditCol::eCycle)->tooltipTemplate);


		cTemp.Format(GetColScheme(ImpEditCol::eThresholdA)->format,0.0);
		list.SetItemText(iItem,ImpEditCol::eThresholdA,cTemp);
		list.SetItemToolTipText(iItem, ImpEditCol::eThresholdA, GetColScheme(ImpEditCol::eThresholdA)->tooltipTemplate);

		cTemp.Format(GetColScheme(ImpEditCol::eThresholdB)->format,0.0);
		list.SetItemText(iItem,ImpEditCol::eThresholdB,cTemp);
		list.SetItemToolTipText(iItem, ImpEditCol::eThresholdB, GetColScheme(ImpEditCol::eThresholdB)->tooltipTemplate);

		CFieldDate::FormatForOutput (p->GetRadEvent()->m_Times[RADEvent::eEndTimestamp], cTemp);
		list.SetCalCtrl(iItem,ImpEditCol::eMeasDateTime,p->GetRadEvent()->m_Times[RADEvent::eEndTimestamp], cTemp);
		list.SetItemToolTipText(iItem, ImpEditCol::eMeasDateTime, GetColScheme(ImpEditCol::eMeasDateTime)->tooltipTemplate);

		cTemp.Format(GetColScheme(ImpEditCol::eCoolingTime)->format,0.0);
		list.SetItemText(iItem,ImpEditCol::eCoolingTime,cTemp,  textclr, bkgclr);
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

		list.SetComboBox(iItem, ImpEditCol::eDetector, 1, &m_d, 5, m_d.GetIndexOf(p->StationShortName));
		list.SetItemToolTipText(iItem, ImpEditCol::eDetector, GetColScheme(ImpEditCol::eDetector)->tooltipTemplate);

		list.SetItemData(iItem, (DWORD)p);  // the event, the external info, the virtual measurement info and results
		}
		else // setting only the bkg colors for deselection state
		{
		}
	}
	catch (...)
	{
	}
}

// 477 update here for BKG
void NewEventDlg::PopulateComputedValues(int iEntry)
{
	MEvent* pm = (MEvent*)m_List.GetItemData(iEntry);
	//what is computed?

	const int m = iEntry % 2;
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

void NewEventDlg::OnBnClickedAnalyzeImp()
{
	bool deselected = false;
	for (int nItem = 0; nItem < m_List.GetItemCount(); nItem++)
	{
		if (!m_List.GetCheckbox(nItem,ImpEditCol::eCheck)) 
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


void NewEventDlg::OnBnClickedFit()
{
	bool bUpdate = false;
    int iLastIndex = -1;

	InitMIndex();

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

		pm->StartNewDeclaration();  // get the next available index

		// next copy all the computed values into the global arrays
		iLastIndex = pm->m_vr._iMPEntryIndex;
		bUpdate = pm->CreateNewMeasurement();
		if (bUpdate)
			pm->SetFit(true); // an actual round trip through the analysis DLL occurs in the next step below
		m_idxa[nItem] = iLastIndex;
		pm->SetAccepted(true);
	}

	if (bUpdate)
	{
		ExternalMeasurements::AgeStatus(iLastIndex);  // does the round trip operation

		for (int nItem=0;nItem <= m_idxa.GetUpperBound();nItem++)  // refresh the icon state
		{
			const int m = nItem % 2;
			const int mi = m_idxa[nItem];
			if (mi >= 0) {
				// update this item row
				const int statusidx = g_iMPStatus[mi] + m_StatusIconIdx;
				COLORREF bkg = DataColouring::AnalyzedRowBkg(m, statusidx);  // the g_iMPStatus index 
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
				// PopulateComputedValues(nItem);
				//m_List.SetItemImage(nItem,ImpEditCol::eItem, m_NQStatusIconIdx + m_StatusIconIdx); 
			}
		}
	}
	m_WaitingForFit = false;
	SetOKButtonIfValidContent(true);
}

void NewEventDlg::SaveChanges()
{
	ExternalMeasurements::PersistChanges(false, m_idxa);
	m_WaitingForPersist = false;
}
