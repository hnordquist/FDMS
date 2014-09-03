// $History: Feeoff.cpp $
// 
// *****************  Version 3  *****************
// User: Longo        Date: 1/07/05    Time: 2:25p
// Updated in $/FDMS/FDMS Application/FDMSNET
// adding the secondary import editor dilaog
// 
// *****************  Version 2  *****************
// User: Longo        Date: 11/24/04   Time: 3:48p
// Updated in $/FDMS/FDMS Application/FDMSNET
// * bring up select measurement dialog upon requests from UL
// * added matching class with logic: uses 182 day window to seek for
// match
// * compiler warning removal
//

#include "stdafx.h"
#include <afxdtctl.h>
#include ".\meacol.h"
#include ".\FDMS.h"
#include ".\feeoff.h"
#include ".\askoncebox.h"
#include ".\field.h"

#include "SelectMeasurement.h"

#include "IniFileProcessor.h"
// eFacility, eDetector, eID, eEnrichment, eBurnup, eDischargeDate, eCycleNumber, eEditableItems}
const UINT FieldTypeCheck::_EditableRowInfo::map[] = 
{
	CMeaCol::eFacility, CMeaCol::eDetector, CMeaCol::eID,
		CMeaCol::eEnrichment, CMeaCol::eBurnUp,
		CMeaCol::eDischDate, CMeaCol::eCycle
};

// FieldTypeCheck dialog

IMPLEMENT_DYNAMIC(FieldTypeCheck, CDialog)
FieldTypeCheck::FieldTypeCheck(FieldTypeCheck::_EditableRowInfo& ri, CWnd* pParent /*=NULL*/)
: 	m_RowInfo(ri), CDialog(FieldTypeCheck::IDD, pParent)
{

}

FieldTypeCheck::~FieldTypeCheck()
{
}

void FieldTypeCheck::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EM_FACILITY, m_RowInfo.data[m_RowInfo.eFacility].val);
	DDV_MaxChars(pDX, m_RowInfo.data[m_RowInfo.eFacility].val, 128);

	//	DDX_Text(pDX, IDC_EM_DETECTOR, m_RowInfo.data[m_RowInfo.eDetector].val);
	//	DDV_MaxChars(pDX,  m_RowInfo.data[m_RowInfo.eDetector].val, 128);

	DDX_Text(pDX, IDC_EM_ID, m_RowInfo.data[m_RowInfo.eID].val);
	DDV_MaxChars(pDX, m_RowInfo.data[m_RowInfo.eID].val, 128);

	DDX_Text(pDX, IDC_EM_ENRICHMENT, m_RowInfo.data[m_RowInfo.eEnrichment].val);
	//DDV_MinMaxDouble(pDX, m_RowInfo.m_enrichment, 0.0, 100.0);

	DDX_Text(pDX, IDC_EM_BURNUP, m_RowInfo.data[m_RowInfo.eBurnup].val);
	//DDV_MinMaxDouble(pDX, m_RowInfo.m_burnup, 0.0, 100.0);

	DDX_Text(pDX, IDC_EM_DISCHARGE_DATE, m_RowInfo.data[m_RowInfo.eDischargeDate].val);

	DDX_Text(pDX, IDC_EM_CYCLE, m_RowInfo.data[m_RowInfo.eCycleNumber].val);


}


BEGIN_MESSAGE_MAP(FieldTypeCheck, CDialog)
	ON_COMMAND(IDOK, OnOK)
	ON_EN_KILLFOCUS(IDC_EM_BURNUP, OnEnKillfocusEmBurnup)
	ON_NOTIFY(NM_KILLFOCUS, IDC_EM_DISCHARGE_DATE, OnNMKillfocusEmDischargeDate)
	ON_EN_KILLFOCUS(IDC_EM_ENRICHMENT, OnEnKillfocusEmEnrichment)
	ON_EN_KILLFOCUS(IDC_EM_FACILITY, OnEnKillfocusEmFacility)
	ON_EN_KILLFOCUS(IDC_EM_ID, OnEnKillfocusEmId)
	ON_EN_CHANGE(IDC_EM_BURNUP, OnEnChangeEmBurnup)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_EM_DISCHARGE_DATE, OnDtnDatetimechangeEmDischargeDate)
	ON_EN_CHANGE(IDC_EM_ENRICHMENT, OnEnChangeEmEnrichment)
	ON_EN_CHANGE(IDC_EM_FACILITY, OnEnChangeEmFacility)
	ON_EN_CHANGE(IDC_EM_ID, OnEnChangeEmId)
	ON_CBN_SELCHANGE(IDC_EM_CYCLE, OnCbnSelchangeEmCycle)
	ON_CBN_KILLFOCUS(IDC_EM_CYCLE, OnCbnKillfocusEmCycle)
	ON_CBN_EDITCHANGE(IDC_EM_CYCLE, OnCbnEditchangeEmCycle)
END_MESSAGE_MAP()


// FieldTypeCheck message handlers

void FieldTypeCheck::OnOK()
{
	if (m_RowInfo.bHasChangedData())
	{
		CString confirmation = L"Clicking 'Yes' will save any changes you have made to this entry, continue?";

		CAskOnceBox x (this, confirmation);

		INT_PTR nRet = -1;
		nRet = x.AskOnce();

		switch ( nRet )
		{
		case IDYES:
			break;
		case IDNO:
			return;
			break;
		default:
			return;
			break;
		};
	}
	CDialog::OnOK();
}

BOOL FieldTypeCheck::OnInitDialog()
{
	CDialog::OnInitDialog();

	// get the combo box selection range here
	CComboBox *pc = (CComboBox*)GetDlgItem(IDC_EM_CYCLE);

	if (pc)
	{
		CFieldLong* pdl = (CFieldLong*) CSelectMeasurement::GetColScheme((CMeaCol::ColumnID)m_RowInfo.map[m_RowInfo.eCycleNumber]);
		CString ent;
		int ret;
		for (int i = pdl->lLow; i <= pdl->lHigh; i++)
		{
			ent.Format(pdl->format, i);
			ret = pc->AddString(ent);
		}
		int fse = pc->FindStringExact(0,m_RowInfo.data[m_RowInfo.eCycleNumber].val);
		pc->SetCurSel(fse);
	}

	CDateTimeCtrl *pd = (CDateTimeCtrl *)GetDlgItem(IDC_EM_DISCHARGE_DATE);
	if (pd)
	{
		// set the date picker range
		CFieldDate* pdf = (CFieldDate*)CSelectMeasurement::GetColScheme(m_RowInfo.map[m_RowInfo.eDischargeDate]);
		if (pdf)
		{
			pd->SetRange(&pdf->dLow, &pdf->dHigh);
		}
		// set the date dialog value, and the date picker date, too.
		COleDateTime t = CFieldDate::ConvertX(m_RowInfo.data[m_RowInfo.eDischargeDate].val);
		if (t.GetStatus() == COleDateTime::valid)
			pd->SetTime(t);
	}

	CString s;
	if (m_RowInfo.bNew)
	{
		s.Format("Editing new entry %s, id %s", m_RowInfo.number, m_RowInfo.id);
	}
	else
	{
		s.Format("Editing entry %s, id %s", m_RowInfo.number, m_RowInfo.id);
	}
	SetWindowText(s);

	return TRUE;
}

void FieldTypeCheck::WarningMessageBox(CField* pf, CString& str)
{
	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
	CString s, msg;

	if (m_RowInfo.bNew)
	{
		s.Format("Error: new entry %s, id %s", m_RowInfo.number, m_RowInfo.id);
	}
	else
	{
		s.Format("Error: entry %s, id %s", m_RowInfo.number, m_RowInfo.id);
	}

	msg.Format("%s value\r\n%s", pf->name, pf->szGetConstraintDescription(str));

	pApp->MyMessageBox( msg, MB_OK | MB_ICONEXCLAMATION, (UINT)-1, s);

}

void FieldTypeCheck::OnEnKillfocusEmBurnup()
{
	if (!m_RowInfo.data[m_RowInfo.eBurnup].b)
		return;
	CString str;
	GetDlgItemText(IDC_EM_BURNUP, str);
	CField* pf = CSelectMeasurement::GetColScheme(m_RowInfo.map[m_RowInfo.eBurnup]);
	if (pf->bSatisfiesConstraints(str) == false)
	{
		WarningMessageBox(pf, str);
		SetDlgItemText(IDC_EM_BURNUP, m_RowInfo.data[m_RowInfo.eBurnup].val);
		m_RowInfo.data[m_RowInfo.eBurnup].b = false;
	}
	else
	{
		float f;
		CString s;
		sscanf(str, pf->inputFormat, &f);
		s.Format(pf->format, f);
		SetDlgItemText(IDC_EM_BURNUP, s);
	}
}

void FieldTypeCheck::OnNMKillfocusEmDischargeDate(NMHDR *, LRESULT *)
{
	if (!m_RowInfo.data[m_RowInfo.eDischargeDate].b)
		return;
	CString str;
	GetDlgItemText(IDC_EM_DISCHARGE_DATE, str);
	CField* pf = CSelectMeasurement::GetColScheme(m_RowInfo.map[m_RowInfo.eDischargeDate]);
	if (pf->bSatisfiesConstraints(str) == false)
	{
		WarningMessageBox(pf, str);
		SetDlgItemText(IDC_EM_DISCHARGE_DATE, m_RowInfo.data[m_RowInfo.eDischargeDate].val);
		m_RowInfo.data[m_RowInfo.eDischargeDate].b  = false;
	}
}

void FieldTypeCheck::OnEnKillfocusEmDetector()
{
	if (!m_RowInfo.data[m_RowInfo.eDetector].b)
		return;
	CString str;
	GetDlgItemText(IDC_EM_DETECTOR, str);
	CField* pf = CSelectMeasurement::GetColScheme(m_RowInfo.map[m_RowInfo.eDetector]);
	if (pf->bSatisfiesConstraints(str) == false)
	{
		WarningMessageBox(pf, str);
		SetDlgItemText(IDC_EM_DETECTOR, m_RowInfo.data[m_RowInfo.eDetector].val);
		m_RowInfo.data[m_RowInfo.eDetector].b = false;
	}
}

void FieldTypeCheck::OnEnKillfocusEmEnrichment()
{
	if (!m_RowInfo.data[m_RowInfo.eEnrichment].b)
		return;
	CString str;
	GetDlgItemText(IDC_EM_ENRICHMENT, str);
	CField* pf = CSelectMeasurement::GetColScheme(m_RowInfo.map[m_RowInfo.eEnrichment]);
	if (pf->bSatisfiesConstraints(str) == false)
	{
		WarningMessageBox(pf, str);
		SetDlgItemText(IDC_EM_ENRICHMENT, m_RowInfo.data[m_RowInfo.eEnrichment].val);
		m_RowInfo.data[m_RowInfo.eEnrichment].b = false;
	}
	else
	{
		float f;
		CString s;
		sscanf(str,  pf->inputFormat, &f);
		s.Format(pf->format, f);
		SetDlgItemText(IDC_EM_ENRICHMENT, s);
	}
}

void FieldTypeCheck::OnEnKillfocusEmFacility()
{
	if (!m_RowInfo.data[m_RowInfo.eFacility].b)
		return;
	CString str;
	GetDlgItemText(IDC_EM_FACILITY, str);
	CField* pf = CSelectMeasurement::GetColScheme(m_RowInfo.map[m_RowInfo.eFacility]);
	if (pf->bSatisfiesConstraints(str) == false)
	{
		WarningMessageBox(pf, str);
		SetDlgItemText(IDC_EM_FACILITY,m_RowInfo.data[m_RowInfo.eFacility].val);
		m_RowInfo.data[m_RowInfo.eFacility].b= false;
	}
}

void FieldTypeCheck::OnEnKillfocusEmId()
{
	if (!m_RowInfo.data[m_RowInfo.eID].b)
		return;
	CString str;
	GetDlgItemText(IDC_EM_ID, str);
	CField* pf = CSelectMeasurement::GetColScheme(m_RowInfo.map[m_RowInfo.eID]);
	if (pf->bSatisfiesConstraints(str) == false)
	{
		WarningMessageBox(pf, str);
		SetDlgItemText(IDC_EM_ID, m_RowInfo.data[m_RowInfo.eID].val);
		m_RowInfo.data[m_RowInfo.eID].b = false;
	}
}

void FieldTypeCheck::OnCbnKillfocusEmCycle()
{
	if (!m_RowInfo.data[m_RowInfo.eCycleNumber].b)
		return;
	CString str;
	GetDlgItemText(IDC_EM_CYCLE, str);
	CField* pf = CSelectMeasurement::GetColScheme(m_RowInfo.map[m_RowInfo.eCycleNumber]);
	if (pf->bSatisfiesConstraints(str) == false)
	{
		WarningMessageBox(pf, str);
		SetDlgItemText(IDC_EM_CYCLE, m_RowInfo.data[m_RowInfo.eCycleNumber].val);
		m_RowInfo.data[m_RowInfo.eID].b = false;
	}
}


void FieldTypeCheck::OnEnChangeEmDetector()
{
	m_RowInfo.data[m_RowInfo.eDetector].b =true;
}

void FieldTypeCheck::OnEnChangeEmBurnup()
{
	m_RowInfo.data[m_RowInfo.eBurnup].b = true;
}

void FieldTypeCheck::OnDtnDatetimechangeEmDischargeDate(NMHDR *, LRESULT *pResult)
{
	//	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	m_RowInfo.data[m_RowInfo.eDischargeDate].b = true;
	*pResult = 0;
}

void FieldTypeCheck::OnEnChangeEmEnrichment()
{
	m_RowInfo.data[m_RowInfo.eEnrichment].b = true;
}

void FieldTypeCheck::OnEnChangeEmFacility()
{
	m_RowInfo.data[m_RowInfo.eFacility].b = true;
}

void FieldTypeCheck::OnEnChangeEmId()
{
	m_RowInfo.data[m_RowInfo.eID].b = true;
}

void FieldTypeCheck::OnCbnSelchangeEmCycle()
{
	m_RowInfo.data[m_RowInfo.eCycleNumber].b = true;
}

void FieldTypeCheck::OnCbnEditchangeEmCycle()
{
	m_RowInfo.data[m_RowInfo.eCycleNumber].b = true;
}
