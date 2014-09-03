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