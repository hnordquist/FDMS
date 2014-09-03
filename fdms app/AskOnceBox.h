#pragma once


// CAskOnceBox dialog

class CAskOnceBox : public CDialog
{
	DECLARE_DYNAMIC(CAskOnceBox)

public:
	CAskOnceBox(CWnd* pParent = NULL, LPCTSTR pMsgText = NULL, LPCTSTR pTitle = NULL);   // standard constructor
	virtual ~CAskOnceBox();

	int  AskOnce (); // may call DoModal, or return immediately with an answer
			// returns IDYES or IDNO, retains 'do not ask' state of question

	static void ResetAllAskOnceVlaues(void);

	
	static const PTCHAR ASK_ONCE_KEY_NAME;

// Dialog Data
	enum { IDD = IDD_MSGBOX };

protected:

	CString Title, MessageText;
	static const CString DefaultTitle;
	static const CString DefaultMessage;

	static void GenHash(const char *source, PTCHAR hash);
	static void SimpleGenHash(PTCHAR p, int maxLength);

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_DoNotAskAgain;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedNo();
	afx_msg void OnBnClickedYes();
	afx_msg void OnBnClickedMsgboxNotagain();
};
