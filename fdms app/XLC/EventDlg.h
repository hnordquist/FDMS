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


