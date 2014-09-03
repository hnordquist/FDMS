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
