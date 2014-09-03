// MatchingEventDlg.h : header file
//

#pragma once

#include "EventDlg.h"
//#include "..\MMMatchDlg.h"
///////////////////////////////////////////////////////////////////////////////
// MatchingEventDlg dialog

class MatchingEventDlg : public EventDlg
{
// Construction
public:
	MatchingEventDlg(bool bAllNew, CObList *pList, CObList *pBkgList, CWnd* pParent = NULL);	// standard constructor
	~MatchingEventDlg();


// Dialog Data
	//{{AFX_DATA(MatchingEventDlg)

	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MatchingEventDlg)
protected:
	//}}AFX_VIRTUAL

// Implementation
protected:

	virtual void FillListCtrl(XListCtrl& list);
	virtual void PopulateBasicRow(XListCtrl& list, int iItem, int iData, MEvent* p, const bool bNew = true);
	void ImportedEntryRow(XListCtrl& list, int iItem, int iData, MEvent* p);
	void ExistingEntryRow(XListCtrl& list, int iItem, int iData, MEvent* p);
	virtual void SaveChanges();

	void PopulateComputedValues(int iEntry);

	CString GetMatchingMeasurementImage(int index, MEvent* p);

	// Generated message map functions
	//{{AFX_MSG(MatchingEventDlg)
	//}}AFX_MSG


	DECLARE_MESSAGE_MAP()
	//afx_msg LRESULT OnFatToolTip(WPARAM w, LPARAM l);
	afx_msg void OnBnClickedAnalyzeImp();
	afx_msg void OnBnClickedFit();

public:
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
};
