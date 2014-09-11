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
// LoggingLog.cpp : implementation file
//

#include "stdafx.h"
#include "FDMS.h"
#include "LoggingLog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

// LoggingLog dialog

LoggingLog::LoggingLog(CWnd* pParent /*=NULL*/)
	: CDialog(LoggingLog::IDD, pParent)
{

	ASSERT(pParent != NULL);

	m_pParent = pParent;
	m_nID = LoggingLog::IDD;
}


void LoggingLog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OLIST, m_OList);

}



BEGIN_MESSAGE_MAP(LoggingLog, CDialog)
END_MESSAGE_MAP()


// LoggingLog message handlers
BOOL LoggingLog::OnInitDialog()
{
	CDialog::OnInitDialog();
	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
	for (int i = 0; i < pApp->m_MLog.GetCount(); i++)
	{
		m_OList.AddString(pApp->m_MLog.GetAt(i));
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}



BOOL LoggingLog::Create()
{
	return CDialog::Create(m_nID, m_pParent);
}

void LoggingLog::OnCancel()
{
	DestroyWindow();
}

void LoggingLog::PostNcDestroy()
{
	delete this;
}


BOOL LoggingLog::PreTranslateMessage(MSG* pMsg)
{

	// implement Ctrl-X and Ctrl-A too (see GetNextSelectedItem)

	if (pMsg->message == WM_CHAR)
	{
		WORD c = LOWORD(pMsg->wParam);
		if (c == 1) // Ctrl-A
		{
			//LRESULT r = m_OList.SelectAll();
			return true;
		}
		else if (c == 3) // Ctrl-C
		{
			bool r = CopyToClipboard(true);
			return r;
		}
		else if (c == 22) // Ctrl-V
		{
			bool r = CopyFromClipboard(true);
			return r;
		}
		else if (c == 24) // Ctrl-X
		{
			bool r = CutToClipboard(false);
			return r;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}
bool LoggingLog::CopyFromClipboard(bool bReplaceSelected)
{
	return true;
}
bool LoggingLog::CutToClipboard(bool bAll)
{
	return true;
}
bool LoggingLog::CopyToClipboard(bool /*bAll*/, bool bHeaders)
{
		const PTCHAR  _cediteol = "";
		if ( !OpenClipboard() )
		{
			AfxMessageBox( "Cannot open the Clipboard" );
			return TRUE;
		}
		// Remove the current Clipboard contents
		if( !EmptyClipboard() )
		{
			AfxMessageBox( "Cannot empty the Clipboard" );
			return TRUE;
		}
		// ...
		// Get the currently selected data

		int i, tlen;
		const int ic = m_OList.GetCount();
		CStringArray a;
		a.SetSize(ic);
		tlen = 0;
		for (i = 0; i < ic; i++)
		{
			CString s;
			m_OList.GetText(i, s);
			s.Append(_cediteol);
			a.SetAtGrow(i, s);
			tlen += s.GetLength();
		}

        // Allocate a global memory object for the text. 
		LPTSTR  lptstrCopy; 
	    HGLOBAL hglbCopy; 
        hglbCopy = GlobalAlloc(GMEM_MOVEABLE, 
            (tlen + 1) * sizeof(TCHAR)); 
        if (hglbCopy == NULL) 
        { 
            CloseClipboard(); 
            return TRUE; 
        } 
 
        // Lock the handle and copy the text to the buffer. 
        lptstrCopy = (LPSTR)GlobalLock(hglbCopy);
        lptstrCopy[0] = (TCHAR) 0;    // null character 
		for (i = 0; i < ic; i++)
		{
			LPSTR b = a[i].GetBuffer();
			size_t l = a[i].GetLength() *  sizeof(TCHAR);
			strncat(lptstrCopy, b, l);
		}
        lptstrCopy[tlen] = (TCHAR) 0;    // null character 
        GlobalUnlock(hglbCopy); 
 
        // Place the handle on the clipboard. 

		// For the appropriate data formats...
		if ( ::SetClipboardData( CF_TEXT, hglbCopy ) == NULL )
		{
			AfxMessageBox( "Unable to set Clipboard data" );
			CloseClipboard();
			return TRUE;
		}
		// ...
		CloseClipboard();
		return TRUE;
		
}

