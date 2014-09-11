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
// XListCtrlTestDlg.h : header file
//

#ifndef XLISTCTRLTESTDLG_H
#define XLISTCTRLTESTDLG_H

#include "XListCtrl.h"

#include "..\ImpEditCol.h"
#include "..\feeoff.h"
#include "..\mevent.h"
#include "afxwin.h"


///////////////////////////////////////////////////////////////////////////////
// XListCtrlDlg dialog

class XListCtrlDlg : public CDialog
{
// Construction
public:
	XListCtrlDlg(bool bAllNew, CObList *pList, CWnd* pParent = NULL);	// standard constructor
	static void MakeFieldMap();
	static void CleanUpStatics();

// Dialog Data
	//{{AFX_DATA(XListCtrlDlg)
	enum { IDD = IDD_DIALOG_IMPEDMEASURE };
	XListCtrl	m_List;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(XListCtrlDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_iSelected;
	bool m_bInitialized;
	CObList* m_pEvList;
	bool m_bAllNew;
	HICON m_hIcon;
	CStringArray m_sa1, m_sa2;
	CFont myfont;
	CRect m_OldRect;

	static ImpEditCol* m_pSelectColumns;
	const int m_StatusIconIdx;
	const int m_NQStatusIconIdx;
	CField* GetColScheme(ImpEditCol::ColumnID iColumnID);
	virtual void FillListCtrl(XListCtrl& list) = 0;
	virtual void InitListCtrl(XListCtrl& list);

	virtual bool UpdateValue(int iEntry, int iColumnID, LPCSTR pRawString, CString& szDisplayableResult) = 0;

	// Generated message map functions
	//{{AFX_MSG(XListCtrlDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButton1();
	afx_msg void OnDestroy();
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnSubEditEnd(WPARAM w, LPARAM pResult);
	afx_msg LRESULT OnBoxDblClick(WPARAM w, LPARAM l);
	afx_msg LRESULT OnFatToolTip(WPARAM w, LPARAM l);
	//}}AFX_MSG
	afx_msg LRESULT OnComboSelection(WPARAM, LPARAM);
	afx_msg LRESULT OnCheckbox(WPARAM, LPARAM);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAnalyzeImp();
	afx_msg void OnBnClickedOk();

	virtual bool CutToClipboard(bool bAll);
	virtual bool CopyToClipboard(bool bAll, bool bHeaders = false);
	virtual bool CopyFromClipboard(bool bReplaceSelected);
    virtual void ReplaceSelection(LPTSTR lptstr); 

	bool GetColumnWriteFlag(int column) { return m_List.GetColumnWriteFlag(column); }
	void SetColumnWriteFlag(bool b, int column) { m_List.SetColumnWriteFlag(b, column); }


	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI *pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditCut(CCmdUI *pCmdUI);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //XLISTCTRLTESTDLG_H
