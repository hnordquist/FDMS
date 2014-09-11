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

// ULAboutBoxDlg.cpp : implementation file
// $History: ULAboutBoxDlg.cpp $ 
// 
// *****************  Version 3  *****************
// User: Longo        Date: 9/17/04    Time: 3:35p
// Updated in $/FDMS/FDMS Application/FDMSNET/ULApplicationClasses
// First integration of the MFC Upper Layer code
//

#include "stdafx.h"
#include "..\FDMS.h"  //REQT:  Change this to use the new application header.
#include "ULAboutBoxDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ULAboutBoxDlg dialog


ULAboutBoxDlg::ULAboutBoxDlg(CString ApplicationName /* = "" */,
		CString	ApplicationVersion/* = "" */,
		CString	ReviewToolVersion/* = "" */,
		CString	FacilityName/* = "" */,
		CString LocationName/* = "" */,
		int IconResID /* = 0*/,CWnd* pParent /*=NULL*/)
	: CDialog(ULAboutBoxDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(ULAboutBoxDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	//Set the contents of the about box.
	m_ApplicationName = ApplicationName;
	m_ApplicationVersion = ApplicationVersion;
	m_ReviewToolVersion = ReviewToolVersion;
	m_FacilityName = FacilityName;
	m_LocationName = LocationName;
//	m_MemoryAvailable = MemoryAvailable;
//	m_MemoryResources = MemoryResources;
	m_IconID = IconResID;		

}


void ULAboutBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ULAboutBoxDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(ULAboutBoxDlg, CDialog)
	//{{AFX_MSG_MAP(ULAboutBoxDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ULAboutBoxDlg message handlers

BOOL ULAboutBoxDlg::OnInitDialog() 
{
	CString MemoryStr, ResourcesStr;

	CDialog::OnInitDialog();
	
	//Set the title and contents of the about dialog box.
	CString Title;
	Title.Format("About %s", m_ApplicationName);
	SetWindowText(Title);

	//Set the contents of the dialog box.
	((CStatic*)GetDlgItem(IDC_SEB_ABOUT_APP_NAME))->SetWindowText(m_ApplicationName	);
	((CStatic*)GetDlgItem(IDC_SEB_ABOUT_APP_VERSION))->SetWindowText(m_ApplicationVersion);
	((CStatic*)GetDlgItem(IDC_SEB_ABOUT_FACILITY))->SetWindowText(m_FacilityName);
	((CStatic*)GetDlgItem(IDC_SEB_ABOUT_LOCATION))->SetWindowText(m_LocationName);

	// Set the version message if the version information exists.
	if (!m_ReviewToolVersion.IsEmpty() )
	{
		CString VersionStr;
		VersionStr.Format("operating in review tool mode %s", m_ReviewToolVersion);
		((CStatic*)GetDlgItem(IDC_SEB_ABOUT_RT_VERSION))->SetWindowText(VersionStr);
	}

	//Load system resources and memory information based on whether _WIN32 mode.
	//This code exists to maintain the flow of the original GUI lib version of the upper layer.
#ifdef _WIN32

		MEMORYSTATUS Memory;

		Memory.dwLength = sizeof(MEMORYSTATUS);

		GlobalMemoryStatus(&Memory);

		MemoryStr.Format( "Memory Available: %lu KB Free", Memory.dwAvailPhys / 1000);
		ResourcesStr.Format( "Resources Available: %5.1f%% Free", (((float)Memory.dwAvailVirtual / (float)Memory.dwTotalVirtual) * 100.0));

#else
		DWORD dMemory_Available;

		dMemory_Available = GetFreeSpace(NULL);
		MemoryStr.Format( "Memory Available: %lu KB Free", dMemory_Available);

		UINT uiResources_Available;
		uiResources_Available = GetFreeSystemResources(GFSR_SYSTEMRESOURCES);
		ResourcesStr.Format( "Resources Available: %u%% Free", uiResources_Available);
#endif
		((CStatic*)GetDlgItem(IDC_SEB_ABOUT_MEMORY))->SetWindowText(MemoryStr);
		((CStatic*)GetDlgItem(IDC_SEB_ABOUT_RESOURCES))->SetWindowText(ResourcesStr);

	//Set the icon for the dialog box.
	if(m_IconID != 0)
		SetIcon(AfxGetApp()->LoadIcon(m_IconID), false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
