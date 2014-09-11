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
//////////////////////////////////////////////////////////////////////////
// Copyright 2004, The Regents Of the University of California.  This
// software was produced under U.S. Government contract (W-7405-ENG-36) at
// the Los Alamos National Laboratory by the Safeguards Science and
// Technology Group (N-1).  All Rights Reserved.
//////////////////////////////////////////////////////////////////////////
// $History: AllOtherDlg.h $ 
// 
// *****************  Version 4  *****************
// User: Longo        Date: 2/07/05    Time: 10:27a
// Updated in $/FDMS/FDMS Application/FDMSNET
// checkpoint checkin
// 
// *****************  Version 1  *****************
// User: Longo        Date: 1/18/05    Time: 9:43a
// Created in $/FDMS/FDMS Application/FDMSNET/FDMSNET
// new list ctrl code
// 
// *****************  Version 3  *****************
// User: Longo        Date: 11/30/04   Time: 4:22p
// Updated in $/FDMS/FDMS Application/FDMSNET
// added code to auto-select the matching row in the measeur dialog list,
// when involkd by IR tool review message
//
//////////////////////////////////////////////////////////////////////////
#pragma once

#include "CCButton.h"
/////////////////////////////////////////////////////////////////////////////
// CAllOtherDlg dialog
extern HWND g_hAllOtherDlg;

class CAllOtherDlg : public CDialog
{
// Construction
public:
	CAllOtherDlg(CWnd* pParent = NULL);   // standard constructor

	void OpenSelectMeasurement(int iSelectItem);

// Dialog Data
	//{{AFX_DATA(CAllOtherDlg)
	enum { IDD = IDD_DIALOG_ALLOTHER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAllOtherDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool FindGraph(CString cString);
	CColorButton m_cButtonMeasure;
	CColorButton m_cButtonIncludeGood;
	CColorButton m_cButtonIncludeSuspect;
	CColorButton m_cButtonAcceptGood;
	CColorButton m_cButtonNvsBU;
	CColorButton m_cButtonGBUvsTime;
	CColorButton m_cButtonNvsG;
	CColorButton m_cButtonReturn;
	bool m_bIncludeGood;
	bool m_bIncludeSuspect;
	int m_preIndex;
public:
	// Generated message map functions
	//{{AFX_MSG(CAllOtherDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnMeasure();
	afx_msg void OnIncludeGood();
	afx_msg void OnIncludeSuspect();
	afx_msg void OnAcceptGood();
	afx_msg void OnNeutronVsBU();
	afx_msg void OnGammaBUVsTime();
	afx_msg void OnNeutronVsGamma();
	afx_msg void OnReturn();
	afx_msg LRESULT OnMyShow(WPARAM WParam, LPARAM lParam);
	//}}AFX_MSG
	afx_msg LRESULT OnMySelect(WPARAM WParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


