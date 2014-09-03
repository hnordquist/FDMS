// NewEventDlg.h : header file
//

#pragma once

#include "EventDlg.h"


///////////////////////////////////////////////////////////////////////////////
// NewEventDlg dialog

class NewEventDlg : public EventDlg
{
// Construction
public:
	NewEventDlg(bool bAllNew, CObList *pList, CObList *pBkgList, CWnd* pParent = NULL);	// standard constructor
	~NewEventDlg();


// Dialog Data
	//{{AFX_DATA(NewEventDlg)

	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NewEventDlg)
protected:
	//}}AFX_VIRTUAL

// Implementation
protected:

	virtual void FillListCtrl(XListCtrl& list);
	virtual void PopulateBasicRow(XListCtrl& list, int iItem, int iData, MEvent* p, const bool bNew = true);
	virtual bool UpdateValue(int iEntry, int iColumnID, LPCSTR pRawString, CString& szDisplayableResult);
	virtual void SaveChanges();
	void PopulateComputedValues(int iEntry);

	// Generated message map functions
	//{{AFX_MSG(NewEventDlg)
	//}}AFX_MSG


	DECLARE_MESSAGE_MAP()

	afx_msg void OnBnClickedAnalyzeImp();
	afx_msg void OnBnClickedFit();

	bool m_mbMeasTypeChoice;  // 00399 default false, no combo box choice. TODO: this should become a user config value, added to the list in the tIniNames enum



};


