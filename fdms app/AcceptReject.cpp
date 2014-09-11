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
// AcceptReject.cpp : implementation file
//

#include "stdafx.h"
#include "fdms.h"
#include "AcceptReject.h"

#ifdef _XJDEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAcceptReject dialog


CAcceptReject::CAcceptReject(CWnd* pParent /*=NULL*/)
	: CDialog(CAcceptReject::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAcceptReject)

	m_cLabelNew = _T("");
	m_cLabelPrev = _T("");
	m_cLabelTitle = _T("");
	for (int cht = NeutA; cht < MaxChannels; cht++)
	{
		m_cLabel[cht] = _T("");
		m_cNew[cht] = _T("");
		m_cPrev[cht] = _T("");
	}
	//}}AFX_DATA_INIT
}


void CAcceptReject::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAcceptReject)
	DDX_Text(pDX, IDC_STATIC_LABEL1, m_cLabel[NeutA]);
	DDX_Text(pDX, IDC_STATIC_LABEL2, m_cLabel[NeutB]);
	DDX_Text(pDX, IDC_STATIC_LABEL3, m_cLabel[NeutC]);
	DDX_Text(pDX, IDC_STATIC_LABEL4, m_cLabel[Gamma1]);
	DDX_Text(pDX, IDC_STATIC_LABEL5, m_cLabel[Gamma2]);
	DDX_Text(pDX, IDC_STATIC_LABELNEW, m_cLabelNew);
	DDX_Text(pDX, IDC_STATIC_LABELPREV, m_cLabelPrev);
	DDX_Text(pDX, IDC_STATIC_NEW1, m_cNew[NeutA]);
	DDX_Text(pDX, IDC_STATIC_NEW2, m_cNew[NeutB]);
	DDX_Text(pDX, IDC_STATIC_NEW3, m_cNew[NeutC]);
	DDX_Text(pDX, IDC_STATIC_NEW4, m_cNew[Gamma1]);
	DDX_Text(pDX, IDC_STATIC_NEW5, m_cNew[Gamma2]);
	DDX_Text(pDX, IDC_STATIC_PREV1, m_cPrev[NeutA]);
	DDX_Text(pDX, IDC_STATIC_PREV2, m_cPrev[NeutB]);
	DDX_Text(pDX, IDC_STATIC_PREV3, m_cPrev[NeutC]);
	DDX_Text(pDX, IDC_STATIC_PREV4, m_cPrev[Gamma1]);
	DDX_Text(pDX, IDC_STATIC_PREV5, m_cPrev[Gamma2]);
	DDX_Text(pDX, IDC_STATIC_ML, m_cLabelTitle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAcceptReject, CDialog)
	//{{AFX_MSG_MAP(CAcceptReject)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAcceptReject message handlers

BOOL CAcceptReject::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (m_cDialogLabel.GetLength())
		SetWindowText(m_cDialogLabel);
	return TRUE; 
}

void CAcceptReject::OnCancel() 
{
	CDialog::OnCancel();
}

void CAcceptReject::OnBnClickedOk()
{
	OnOK();
}
