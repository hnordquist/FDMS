// LegText.cpp : implementation file
//


#include "stdafx.h"
#include "FDMS.h"
#include "LegText.h"
#include "about.h"





CLegText::CLegText() : CDialog(CLegText::IDD)
{

	//{{AFX_DATA_INIT(CLegText)
	//}}AFX_DATA_INIT


	clauses[0] = CString("Copyright (c) 2014, Los Alamos National Security, LLC\r\nAll rights reserved.");
	clauses[1] = CString("Copyright 2014. Los Alamos National Security, LLC. This software was produced under U.S. Government contract\r\nDE-AC52-06NA25396 for Los Alamos National Laboratory (LANL), which is operated by Los Alamos National Security,\r\nLLC for the U.S. Department of Energy. The U.S. Government has rights to use, reproduce, and distribute this software.\r\nNEITHER THE GOVERNMENT NOR LOS ALAMOS NATIONAL SECURITY, LLC MAKES ANY WARRANTY, EXPRESS OR IMPLIED,\r\nOR ASSUMES ANY LIABILITY FOR THE USE OF THIS SOFTWARE.  If software is modified to produce derivative works,\r\nsuch modified software should be clearly marked, so as not to confuse it with the version available from LANL.");
	clauses[2] = CString("Additionally, redistribution and use in source and binary forms, with or without modification, are permitted provided\r\nthat the following conditions are met:\r\n  *  Redistributions of source code must retain the above copyright notice, this list of conditions and the following\r\ndisclaimer.\r\n  *  Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following\r\ndisclaimer in the documentation and/or other materials provided with the distribution.\r\n  *  Neither the name of Los Alamos National Security, LLC, Los Alamos National Laboratory, LANL, the U.S. Government,\r\nnor the names of its contributors may be used to endorse or promote products derived from this software without specific\r\nprior written permission.");
	clauses[3] = CString("THIS SOFTWARE IS PROVIDED BY LOS ALAMOS NATIONAL SECURITY, LLC AND CONTRIBUTORS ""AS IS"" AND ANY EXPRESS OR IMPLIED\r\nARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR\r\nPURPOSE ARE DISCLAIMED. IN NO EVENT SHALL LOS ALAMOS NATIONAL SECURITY, LLC OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,\r\nINDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF\r\nSUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY\r\nTHEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING\r\nIN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.");


	}

void CLegText::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLegText)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLegText, CDialog)
	//{{AFX_MSG_MAP(CLegText)
	// No message handlers
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CLegText::OnBnClickedMore)
END_MESSAGE_MAP()

// My callback procedure that reads the rich edit control contents 
// from a file. 
static DWORD CALLBACK MyStreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
   CFile* pFile = (CFile*) dwCookie;
   *pcb = pFile->Read(pbBuff, cb);
   return 0;
}
BOOL CLegText::OnInitDialog()
{
	CDialog::OnInitDialog();
	//AfxInitRichEdit2();
	// the static vars named through CAboutDlg are loaded at FDMS app window frame init time
	CString s;
#ifdef _DEBUG
	s.Format(_T("FDMS %s (%s)"),
			(LPCTSTR)CAboutDlg::m_sVer, (LPCTSTR)CAboutDlg::m_sTime);
#else
	s.Format(_T("FDMS %s"),
			(LPCTSTR)CAboutDlg::m_sVer);
#endif 
	SetDlgItemText(IDC_VERSION_LABEL, s);

	//mTexts.Set
//	SetDlgItemText(IDC_LEGALESE,clauses.GetAt((POSITION)1));
	CRichEditCtrl *p = (CRichEditCtrl *)GetDlgItem(IDC_LEGALESE);
	CFile cFile(TEXT("license.rtf"), CFile::modeRead);
	EDITSTREAM es;
	es.dwCookie = (DWORD) &cFile;
	es.pfnCallback = MyStreamInCallback; 
	p->StreamIn(SF_RTF, es);

	return TRUE;
}




class CAboutDlg;
void CLegText::OnBnClickedMore()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
