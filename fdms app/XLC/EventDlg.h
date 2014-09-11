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

#include "XListCtrlTestDlg.h"
#include "..\combos.h"


///////////////////////////////////////////////////////////////////////////////
// EventDlg dialog

class EventDlg : public XListCtrlDlg
{
// Construction
public:
	EventDlg(bool bAllNew, CObList *pList, CObList *pBkgList, CWnd* pParent = NULL);	// standard constructor
	~EventDlg();


// Dialog Data
	//{{AFX_DATA(EventDlg)

	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(EventDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	virtual void FillListCtrl(XListCtrl& list) = 0;
	virtual bool UpdateValue(int iEntry, int iColumnID, LPCSTR pRawString, CString& szDisplayableResult);
	
	void UpdateColumnEntry(int iItem, int hcol, LPTSTR val);

	virtual void PopulateBasicRow(XListCtrl& list, int iItem, int iData, MEvent* p, const bool bNew = true) = 0;

	virtual void ReplaceSelection(LPTSTR lptstr); 

	virtual void SaveChanges() = 0;


	// Generated message map functions
	//{{AFX_MSG(EventDlg)
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//}}AFX_MSG


	DECLARE_MESSAGE_MAP()

	afx_msg void OnBnClickedAnalyzeImp();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg LRESULT OnComboSelection(WPARAM, LPARAM);
	afx_msg LRESULT OnCheckbox(WPARAM, LPARAM);
	afx_msg void OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);

	FacilityCombo m_f;
	DetectorCombo m_d;
	CycleCombo    m_c;
	MeasTypeCombo m_m;

	CObList *m_BkgList;
	CString m_DClass;

	bool m_WaitingForFit; // true when a "fit" operation is due
	bool m_WaitingForPersist; // true when analsyis and "fit" operations have occurred and the data is waiting to be saved 
	CArray<int, int&> m_idxa;  // tracks measurements with good analysis and fit, used for final persistence

	void InitMIndex();
	MEvent* LookupBkg(MEvent *m);

	void AddOutputLine(LPCTSTR a, LPCTSTR b);
	void AddOutputLine(LPCTSTR s);

	static const CString m_NoId;


public:
	afx_msg void OnDestroy();
	void SetOKButtonIfValidContent(bool bAcceptanceCheck);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	virtual bool CopyToClipboard(bool bAll, bool bHeaders = false);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


