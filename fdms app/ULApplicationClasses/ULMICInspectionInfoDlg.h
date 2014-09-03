#if !defined(AFX_ULMICINSPECTIONINFODLG_H__7FDB0B0A_0482_48FF_8883_584A44186BDF__INCLUDED_)
#define AFX_ULMICINSPECTIONINFODLG_H__7FDB0B0A_0482_48FF_8883_584A44186BDF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ULMICInspectionInfoDlg.h : header file
// $History: ULMICInspectionInfoDlg.h $ 
// 
// *****************  Version 2  *****************
// User: Longo        Date: 9/17/04    Time: 3:35p
// Updated in $/FDMS/FDMS Application/FDMSNET/ULApplicationClasses
// First integration of the MFC Upper Layer code
//
#include "..\resource.h"
#include "RTInit.h"
/////////////////////////////////////////////////////////////////////////////
// ULMICInspectionInfoDlg dialog

class ULMICInspectionInfoDlg : public CDialog
{
// Construction
public:
	ULMICInspectionInfoDlg(CString InspecINIFileName, CString DlgWndTitle, 
		RTInit* pCommonParams, int IconResID = 0, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ULMICInspectionInfoDlg)
	enum { IDD = IDD_RT_MIC_LOAD_INSPEC_INFO };
	CListBox	m_MICInspecListCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ULMICInspectionInfoDlg)
	public:
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ULMICInspectionInfoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int m_IconID;
	CString m_Title;
	CString m_InspecINIFile;
	RTInit* m_pCommonParams;

//The following items were taken from the RTTitle code.  Names
//left as is until verified that the dialog is working properly.
private: 
		char *pszMIC_Facility;// = NULL, 
		char *pszMIC_Previous;// = NULL, 
		char *pszMIC_Date_Time; // = NULL, 
		char *pszMIC_Inspector_Name;// = NULL, 
		char *pszMIC_Inspector_ID;// = NULL, 
		char *pszMIC_Inspection_Number;// = NULL, 
//		char	*pszMIC_Full_Filename = NULL;
		bool m_FailedInit;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ULMICINSPECTIONINFODLG_H__7FDB0B0A_0482_48FF_8883_584A44186BDF__INCLUDED_)
