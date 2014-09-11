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
