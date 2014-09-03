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
