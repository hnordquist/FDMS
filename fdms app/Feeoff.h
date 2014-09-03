#pragma once

// FieldTypeCheck dialog

class FieldTypeCheck : public CDialog
{
	DECLARE_DYNAMIC(FieldTypeCheck)

public:

	class _EditableRowInfo 
	{
	public:
		typedef enum
			{eFacility, eDetector, eID, eEnrichment, eBurnup, eDischargeDate, eCycleNumber, eEditableItems}
		iEditableItems;

        typedef struct
		{
			CString val;
			bool b;
			UINT id;
		} EditableFields;

		EditableFields data[eEditableItems];

		bool bNew;
		CString id, number;

		bool bHasChangedData() const
		{
			for (int x = eFacility; x < eEditableItems; x++)
				if (data[x].b)
					return true;
			return false;
		}
		static const UINT map[eEditableItems];

	};

	FieldTypeCheck(FieldTypeCheck::_EditableRowInfo& ri, CWnd* pParent = NULL);   // standard constructor
	virtual ~FieldTypeCheck();

// Dialog Data
	enum { IDD = IDD_EDENT };

protected:
	void WarningMessageBox(CField* pf, CString& str);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	_EditableRowInfo& m_RowInfo;

	afx_msg void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnEnKillfocusEmBurnup();
	afx_msg void OnNMKillfocusEmDischargeDate(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnKillfocusEmDetector();
	afx_msg void OnEnKillfocusEmEnrichment();
	afx_msg void OnEnKillfocusEmFacility();
	afx_msg void OnEnKillfocusEmId();
	afx_msg void OnEnChangeEmDetector();
	afx_msg void OnEnChangeEmBurnup();
	afx_msg void OnDtnDatetimechangeEmDischargeDate(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEmEnrichment();
	afx_msg void OnEnChangeEmFacility();
	afx_msg void OnEnChangeEmId();
	afx_msg void OnCbnSelchangeEmCycle();
	afx_msg void OnCbnKillfocusEmCycle();
	afx_msg void OnCbnEditchangeEmCycle();
};
