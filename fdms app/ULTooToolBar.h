//////////////////////////////////////////////////////////////////////////
// Copyright 2005, The Regents Of the University of California.  This
// software was produced under U.S. Government contract (W-7405-ENG-36) at
// the Los Alamos National Laboratory by the Safeguards Science and
// Technology Group (N-1).  All Rights Reserved.
//////////////////////////////////////////////////////////////////////////
// $History: ULTooToolBar.h $
// 
// *****************  Version 4  *****************
// User: Longo        Date: 3/09/05    Time: 3:01p
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 3  *****************
// User: Longo        Date: 1/06/05    Time: 10:05a
// Updated in $/FDMS/FDMS Application/FDMSNET
// threaded import processing for UI interaction
// 
// *****************  Version 2  *****************
// User: Longo        Date: 1/05/05    Time: 5:31p
// Updated in $/FDMS/FDMS Application/FDMSNET
// import button implementation
//////////////////////////////////////////////////////////////////////////
#pragma once


// ULTooToolBar

class ULTooToolBar : public CToolBar
{
	DECLARE_DYNAMIC(ULTooToolBar)

public:
	ULTooToolBar();
	virtual ~ULTooToolBar();

	void StartTheTool();
	void StartTheTool(int item);

	void InitControlWithULInfo(CWnd* pParentWnd);

	void ImportFromTool();

	CComboBox m_ToolBox;
	BOOL m_bVertical;
	BOOL m_bColor;
	bool m_bULToolsAvailable;
	bool m_bImportSourceAvailable;
	static bool m_bImportActive;

	BOOL SetColor(BOOL bColor);
	BOOL SetHorizontal();
	BOOL SetVertical();


	void CheckForImportFileExistence();

	virtual CSize CalcDynamicLayout(int nLength, DWORD dwMode);

	afx_msg void OnUpdateImportButton(CCmdUI *pCmdUI);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT nIDEvent);
};


