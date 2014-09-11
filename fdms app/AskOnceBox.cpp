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
// $History: AskOnceBox.cpp $ 
// 
// *****************  Version 3  *****************
// User: 186846       Date: 12/18/07   Time: 4:34p
// Updated in $/FDMS/FDMS Application/FDMSNET
// VS 2005 changes completed
// 
// *****************  Version 2  *****************
// User: Longo        Date: 11/24/04   Time: 3:48p
// Updated in $/FDMS/FDMS Application/FDMSNET
// * bring up select measurement dialog upon requests from UL
// * added matching class with logic: uses 182 day window to seek for
// match
// * compiler warning removal
//

#include "stdafx.h"
#include "FDMS.h"

#include "regwrapper.h"
#include ".\askoncebox.h"

const PTCHAR CAskOnceBox::ASK_ONCE_KEY_NAME = "ao";


#define GUI_NONE 0x0

const CString CAskOnceBox::DefaultTitle = "Warning";
const CString CAskOnceBox::DefaultMessage = "Do you want this change to occur?";

// CAskOnceBox dialog

IMPLEMENT_DYNAMIC(CAskOnceBox, CDialog)
CAskOnceBox::CAskOnceBox(CWnd* pParent /*=NULL*/, LPCTSTR pMsgText, LPCTSTR pTitle)
: CDialog(CAskOnceBox::IDD, pParent)
, m_DoNotAskAgain(FALSE)
{
	if (pTitle)
	{
		Title = pTitle;
	}
	else
	{
		Title = DefaultTitle;
	}
	if (pMsgText)
	{
		MessageText = pMsgText;
	}
	else
	{
		MessageText = DefaultMessage;
	}
}

CAskOnceBox::~CAskOnceBox()
{
}



BEGIN_MESSAGE_MAP(CAskOnceBox, CDialog)
	ON_BN_CLICKED(IDNO, OnBnClickedNo)
	ON_BN_CLICKED(IDYES, OnBnClickedYes)
END_MESSAGE_MAP()


// CAskOnceBox message handlers

int  CAskOnceBox::AskOnce ( )
{
	LONG lRecalledState = 0;
	TCHAR ebu[36];

	GenHash(MessageText, ebu);
	//LONG lRes = 
		CRegWrapper::GetPrivateIniInt(ASK_ONCE_KEY_NAME,(LPCTSTR)ebu,&lRecalledState);
	//   if (lRes != 0)
	//       DisplayErrorText(ASK_ONCE_KEY_NAME, lRes);

	if (lRecalledState == GUI_NONE)  // never asked before, or previously user did not check "do not ask this again box"
	{

		TCHAR zebu[64];
		TCHAR szlmr[1024];
		strcpy(szlmr, MessageText.GetBuffer());
		strcpy(zebu,ebu);
		strcat(zebu,"txt");
		// LONG lRes = 
			CRegWrapper::SetPrivateIniString(ASK_ONCE_KEY_NAME,(LPCTSTR)zebu,szlmr);

		BOOL bSelected = 0;
		INT_PTR nRet = -1;
		int nMappedResult = GUI_NONE;

		nRet = this->DoModal();

		// Handle the return value from DoModal
		switch ( nRet )
		{
		case -1: 
			AfxMessageBox("Dialog box could not be created!");
			break;
		case IDYES:
		case IDOK:
			bSelected = m_DoNotAskAgain;
			nMappedResult = IDYES;
			break;
		case IDNO:
		case IDCANCEL:
			bSelected = m_DoNotAskAgain;
			nMappedResult = IDNO;
			break;
		};

		lRecalledState = nMappedResult;

		if (bSelected && nRet != 0) // save return value no matter what the value is, iff dont ask check box is checked
		{
			CRegWrapper::SetPrivateIniInt(ASK_ONCE_KEY_NAME,(LPCTSTR)ebu,nMappedResult);
		}

	}

	return lRecalledState;
}


// make a hash value from the input string
void CAskOnceBox::SimpleGenHash(PTCHAR p, int maxLength)
{
	PTCHAR key = p;
	ULONG nHash = 0;
	int i = 0;
	while (*key && i < maxLength)
	{
		i++;
		nHash = (nHash<<5) + nHash + *key++;     // hints from MSFT
	}
	//convert uint to a string rep now
	_ultoa (nHash, p, 10);
}

void CAskOnceBox::GenHash(const char *source, PTCHAR hash)
{
	static TCHAR shash[1024];
	ZeroMemory(&shash, 1024 * sizeof(TCHAR));

	memcpy(shash,  source, strlen(source));
	SimpleGenHash(shash, strlen(shash));
	strcpy(hash, shash);

}



void CAskOnceBox::ResetAllAskOnceVlaues(void)
{

	//LONG lRes = 
	CRegWrapper::DeleteAppSection(ASK_ONCE_KEY_NAME);

}


BOOL CAskOnceBox::OnInitDialog()
{
	CDialog::OnInitDialog();

	this->SetDlgItemText(IDC_MSGBOX_TEXT,MessageText);
	this->SetWindowText(Title );

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CAskOnceBox::OnBnClickedNo()
{
	m_DoNotAskAgain = IsDlgButtonChecked(IDC_MSGBOX_NOTAGAIN);
	OnCancel();
}

void CAskOnceBox::OnBnClickedYes()
{
	m_DoNotAskAgain = IsDlgButtonChecked(IDC_MSGBOX_NOTAGAIN);
	OnOK();
}