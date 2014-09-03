//////////////////////////////////////////////////////////////////////////
// Copyright 2004, The Regents Of the University of California.  This
// software was produced under U.S. Government contract (W-7405-ENG-36) at
// the Los Alamos National Laboratory by the Safeguards Science and
// Technology Group (N-1).  All Rights Reserved.
//////////////////////////////////////////////////////////////////////////
// $History: SelectMeasurement.h $
// 
// *****************  Version 9  *****************
// User: Longo        Date: 3/11/05    Time: 10:35a
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 8  *****************
// User: Longo        Date: 2/10/05    Time: 3:46p
// Updated in $/FDMS/FDMS Application/FDMSNET
// checkpoint checkin
// 
// *****************  Version 7  *****************
// User: Longo        Date: 2/07/05    Time: 10:27a
// Updated in $/FDMS/FDMS Application/FDMSNET
// checkpoint checkin
// 
// *****************  Version 6  *****************
// User: Longo        Date: 1/07/05    Time: 2:25p
// Updated in $/FDMS/FDMS Application/FDMSNET
// adding the secondary import editor dilaog
// 
// *****************  Version 5  *****************
// User: Longo        Date: 11/30/04   Time: 4:22p
// Updated in $/FDMS/FDMS Application/FDMSNET
// added code to auto-select the matching row in the measeur dialog list,
// when involkd by IR tool review message
// 
// *****************  Version 4  *****************
// User: Longo        Date: 11/24/04   Time: 3:48p
// Updated in $/FDMS/FDMS Application/FDMSNET
// * bring up select measurement dialog upon requests from UL
// * added matching class with logic: uses 182 day window to seek for
// match
// * compiler warning removal
// 
// *****************  Version 3  *****************
// User: Longo        Date: 11/19/04   Time: 10:28a
// Updated in $/FDMS/FDMS Application/FDMSNET
// toolbar creation
// 
// *****************  Version 2  *****************
// User: Longo        Date: 10/20/04   Time: 1:10p
// Updated in $/FDMS/FDMS Application/FDMSNET
// registry failed today, saving all work



#pragma once

#include "mlistctl.h"
#include "meacol.h"
#include "feeoff.h"


/////////////////////////////////////////////////////////////////////////////
// CSelectMeasurement dialog


class CSelectMeasurement : public CDialog
{
// Construction
public:


	CSelectMeasurement(CWnd* pParent = NULL);   // standard constructor

	int m_iSelected;
	int m_iSubsetSelector;
	int m_preIndex;
	bool m_bETStat, m_bWriteMP, m_bBlock;

	static CField* GetColScheme(UINT iColumnID);
	
	void UpdateRow(bool bNew, int iItem, int iData);

	void ConstructList();


	static void CleanUpStatics();
	static void MakeFieldMap();

// Dialog Data
	//{{AFX_DATA(CSelectMeasurement)
	enum { IDD = IDD_DIALOG_NEWMEASURE };
	CButton	m_cOK;
	CButton	m_cCancel;
	CButton	m_cEdit;
	CButton	m_cAdd;
	CMyListCtrl	m_cListCtrl;
	//}}AFX_DATA
	CImageList m_pImageList;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectMeasurement)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:


	static CMeaCol* m_pSelectColumns;

	BOOL SortTextItems( int nCol, BOOL bAscending, 
					int low = 0, int high = -1);

	void AddMeasurement(const ChannelCounts& dAve, int iIndex);

	int StartNewDeclaration();
	void FinishNewDeclaration(bool bRetain, int iItem);

	void AddToGlobals(int iItem);
	bool RemoveFromGlobals(int row);
	void UpdateGlobals(int iEntry, int iCID, PTCHAR p, CString& szDisplayableResult);
	void UpdateTheGraphsAndGlobals(int iIndex = -1);
	void UpdateIconsAndIndex();

	int GetInternalIndex(int row);

	// Generated message map functions
	//{{AFX_MSG(CSelectMeasurement)
	virtual BOOL OnInitDialog();
	afx_msg void OnColumnclickList1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnEditToggle();
	afx_msg void OnAddNewDeclaration();
	afx_msg void OnRemoveDeclaration();
	afx_msg LRESULT OnEditRow(WPARAM w, LPARAM pResult);
	afx_msg void OnBnClickedDelSelM();
	afx_msg void OnBnClickedEditToggle3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CRect m_OldRect;
	BOOL m_bInitialized;

	int Selected;

public:
	int EditRow(int nItem, 	FieldTypeCheck::_EditableRowInfo& ri);

	virtual void OnCancel();
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	void SelectItem(int rindex);
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.