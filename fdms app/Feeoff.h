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
