// BkgEventDlg.h : header file
//

#pragma once

#include "EventDlg.h"


///////////////////////////////////////////////////////////////////////////////
// BkgEventDlg dialog

class BkgEventDlg : public EventDlg
{
// Construction
public:
	BkgEventDlg(bool bAllNew, CObList *pList, CObList* pBkgList, CWnd* pParent = NULL);	// standard constructor
	~BkgEventDlg();


// Dialog Data
	//{{AFX_DATA(BkgEventDlg)

	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(BkgEventDlg)
protected:
	//}}AFX_VIRTUAL

// Implementation
protected:

	virtual void FillListCtrl(XListCtrl& list);
	virtual void PopulateBasicRow(XListCtrl& list, int iItem, int iData, MEvent* p, const bool bNew = true);

	void PopulateComputedValues(int iEntry);
	virtual void SaveChanges() {}; // no-op for bkg

	// Generated message map functions
	//{{AFX_MSG(BkgEventDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	afx_msg void OnBnClickedAnalyzeImp();
	afx_msg void OnBnClickedOk();
};

