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
// $History: IniFileProcessor.cpp $ 
// 
// *****************  Version 16  *****************
// User: 186846       Date: 12/18/07   Time: 4:34p
// Updated in $/FDMS/FDMS Application/FDMSNET
// VS 2005 changes completed
// 
// *****************  Version 15  *****************
// User: Longo        Date: 7/27/05    Time: 3:54p
// Updated in $/FDMS/FDMS Application/FDMSNET
// language improvement in a closing message
// 
// *****************  Version 14  *****************
// User: Longo        Date: 3/11/05    Time: 10:35a
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 13  *****************
// User: Longo        Date: 2/07/05    Time: 10:19a
// Updated in $/FDMS/FDMS Application/FDMSNET
// checkpoint checkin
// 
// *****************  Version 12  *****************
// User: Longo        Date: 1/10/05    Time: 12:47p
// Updated in $/FDMS/FDMS Application/FDMSNET
// matching on date time AND facility, detector and item id
// 
// *****************  Version 11  *****************
// User: Longo        Date: 12/21/04   Time: 4:14p
// Updated in $/FDMS/FDMS Application/FDMSNET
// added a few things
// 
// *****************  Version 10  *****************
// User: Longo        Date: 12/10/04   Time: 6:25p
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 9  *****************
// User: Longo        Date: 12/10/04   Time: 4:44p
// Updated in $/FDMS/FDMS Application/FDMSNET
// Combining the GrandCom demo class wirth the SerialCom live detector
// control class
// 
// *****************  Version 8  *****************
// User: Longo        Date: 11/24/04   Time: 3:48p
// Updated in $/FDMS/FDMS Application/FDMSNET
// * bring up select measurement dialog upon requests from UL
// * added matching class with logic: uses 182 day window to seek for
// match
// * compiler warning removal
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "fdms.h"
#include "IniFileProcessor.h"

#include "..\AppUL\radeventlist.h"

#ifdef _XJDEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CString CIniFileProcessor::DefaultSourcePath;
CString CIniFileProcessor::DefaultSourceFullPath;  // mem leak here?
bool CIniFileProcessor::m_bULPresent = false;

void CIniFileProcessor::SetDefaultSource()
{
	char lg_cIniFile[MAX_PATH];
	GetModuleFileName(NULL,lg_cIniFile,sizeof(lg_cIniFile));
	char *c = strrchr(lg_cIniFile,'\\');//find the last "\"
	if (c) 
		*c = '\0';						//make it null to punt file name
	strcat(lg_cIniFile,"\\");

	DefaultSourcePath = lg_cIniFile;

	DefaultSourceFullPath = CIniFileProcessor::DefaultSourcePath;
	DefaultSourceFullPath.Append(NAMESHORT);
	DefaultSourceFullPath.Append(".INI");
}

#define MPFNAME  ".\\measurementplan.csv"
#define MFNAME  ".\\measurements.csv"
#define DBFNAME  ".\\database.csv"
#define ULININAME  ".\\ULAYR.ini"
#define ULAPPSETTINGSNAME  ".\\ApplicationSettings.ini"
#define IRDATAFILENAME  RADEventsList::GetDefaultFilename()
#define IRININAME  "irs.ini"

#define CONFIGSECTION "CONFIGURATION"
#define GRANDSECTION "GRAND"
#define BKGSECTION "BACKGROUND"
#define FACSECTION "DEFAULT_FACILITY_INFO"

IniFileEntry CIniFileProcessor::m_Entry[ININAMECOUNT] =
{

	{eBGCYCLETIME,			"BGCYCLETIME",		false,	false,NULL, "30", GRANDSECTION},
	{eBGCYCLES	,			"BGCYCLES",			false,	false,NULL, "3", GRANDSECTION},
	{eOFFSET_CNTRL,			"OFFSET_CNTRL",		true,	false,NULL, "Yes", GRANDSECTION},
	{eOFFSET_CNTRL_REMOTE,	"OFFSET_CNTRL_REMOTE",true,	false,NULL, "No", GRANDSECTION},
	{eOFFSET_LIMIT_LOW,		"OFFSET_LIMIT_LOW",false,	false,NULL, "700", GRANDSECTION},
	{eOFFSET_LIMIT_HIGH,	"OFFSET_LIMIT_HIGH",false,	false,NULL, "1200", GRANDSECTION},
	{eOFFSET_TIME	,		"OFFSET_TIME",false,		false,NULL, "60", GRANDSECTION},
	{eGAMMAGATE1	,		"GAMMAGATE1",false,			false,NULL, "1", GRANDSECTION},
	{eGAMMAGATE2	,		"GAMMAGATE2",false,			false,NULL, "250000", GRANDSECTION},

	{eBAUD			,	"BAUD",	false,					false,NULL, "9600", GRANDSECTION},
	{eDATABITS		,	"DATABITS"       ,false,		false,NULL, "8", GRANDSECTION},
	{eSTOPBITS		,	"STOPBITS",	false,				false,NULL, "1", GRANDSECTION},
	{eTICKLE		,		"TICKLE",false,				false,NULL, "1000", GRANDSECTION},
	{ePORT			,	"PORT",false,					false,NULL, "COM1", GRANDSECTION},
	{ePARITY		,		"PARITY",false,				false,NULL, "None", GRANDSECTION},

	{eCYCLETIME		,	"CYCLETIME",false,				false,NULL, "20", GRANDSECTION},

	{eMINSAMPLES		,	"MINSAMPLES",false,			false,NULL, "3", GRANDSECTION},
	{eMAXSAMPLES	,		"MAXSAMPLES",false,			false,NULL, "7", GRANDSECTION},

	{eCOMMANDTIMEOUT,		"COMMANDTIMEOUT",false,		false,NULL, "10", GRANDSECTION},
	{eICHVBIAS		,	"ICHVBIAS",false,				false,NULL, "300", GRANDSECTION},
	{eHVBIAS		,		"HVBIAS",false,				false,NULL, "1000", GRANDSECTION},
	{eCYCLES		,		"CYCLES",false,				false,NULL, "1", GRANDSECTION},
	{eMINIGRAND		,	"MINIGRAND", true,				false,NULL, "Yes", GRANDSECTION},

	{eICBACKGROUND1,	"IC_BACKGROUND1", false,		false,NULL, "0.0000", GRANDSECTION},
	{eICBACKGROUND2,	"IC_BACKGROUND2", false,		false,NULL, "0.0000", GRANDSECTION},

	{eSCALE			,	"SCALE", true,					false,NULL, "cm", CONFIGSECTION},
	{eMEASUREMENTS	,	"MEASUREMENTS",true,			false,NULL, MFNAME, CONFIGSECTION},
	{eDATABASE		,	"DATABASE",true,				false,NULL, DBFNAME, CONFIGSECTION},
	{eMEASUREMENTPLAN,		"MEASUREMENTPLAN",true,		false,NULL, MPFNAME, CONFIGSECTION},
	{eDELTA_ENRICHMENT,	"DELTA_ENRICHMENT", true,		false,NULL, "0.15", CONFIGSECTION},
	{eDELTA_COOLINGTIME,	"DELTA_COOLINGTIME", true,	false,NULL, "5.0", CONFIGSECTION},

	{ePASSIVE_SYM		,	"PASSIVE",false,			false,NULL, "", "USE_THIS_SECTION"},

	{eACTIVITYLOG	,		"ACTIVITYLOG",true,			false,NULL, "", CONFIGSECTION},
	{eCOMMLOG		,		"COMMLOG",true,				false,NULL, "", CONFIGSECTION},
	{eLOGLOC		,		"LOGLOC",true,				false,NULL, "", CONFIGSECTION},

	{eIRCONFIGLOC	,		"IRCONFIGLOC",true,			false,NULL, "", CONFIGSECTION},
	{eULINILOC		,		"ULINILOC",true,			false,NULL, "", CONFIGSECTION},
	{eULAPPSETTINGSLOC,		"ULAPPSETTINGSLOC",true,	false,NULL, "", CONFIGSECTION},
	{eIRINILOC,				"IRINILOC",true,			false,NULL, "", CONFIGSECTION},
	{eIRDATALOC,			"IRDATALOC",true,			false,NULL, "", CONFIGSECTION},

	{eIRCONFIGFILENAME,		"IRCONFIGFILENAME",true,	false,NULL, "", CONFIGSECTION},
	{eULININAME		,		"ULININAME",true,			false,NULL, ULININAME, CONFIGSECTION},
	{eULAPPSETTINGSNAME,	"ULAPPSETTINGSNAME",true,	false,NULL, ULAPPSETTINGSNAME, CONFIGSECTION},
	{eIRININAME,			"IRININAME",true,			false,NULL, IRININAME, CONFIGSECTION},
	{eIRDATAFILENAME,		"IRDATAFILENAME",true,		false,NULL, IRDATAFILENAME, CONFIGSECTION},

	{eMATCHDAYS,			"MATCHDAYS", true,			false,NULL, "182", CONFIGSECTION},
	{eMATCHMINUTES,			"MATCHMINUTES", true,		false,NULL, "60", CONFIGSECTION},
	{eMATCHSECONDS,			"MATCHSECONDS", true,		false,NULL, "180", CONFIGSECTION},

	{eFACILITYID,			"FacilityID",false,			false,NULL, "", FACSECTION},

	{eDEMOVERSION,			"DEMONSTRATION",true,				false,NULL, "No", CONFIGSECTION},

	{eBACKGROUND_A,"BACKGROUND_A",true,					false,NULL, "0.000", BKGSECTION},
	{eBACKGROUND_B,"BACKGROUND_B",true,					false,NULL, "0.000", BKGSECTION},
	{eBACKGROUND_C,"BACKGROUND_C",true,					false,NULL, "0.000", BKGSECTION},
	{eBACKGROUND_1,"BACKGROUND_1",true,					false,NULL, "0.000", BKGSECTION},
	{eBACKGROUND_2,"BACKGROUND_2",true,					false,NULL, "0.000", BKGSECTION},
	{eOFFSET_INTERNAL_CH0,"OFFSET_INTERNAL_CH0",true,	false,NULL, "1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000", BKGSECTION},
	{eOFFSET_INTERNAL_CH1,"OFFSET_INTERNAL_CH1",true,	false,NULL, "1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000", BKGSECTION},
	{eOFFSET_REMOTE_CH0,"OFFSET_REMOTE_CH0",true,		false,NULL, "1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000", BKGSECTION},
	{eOFFSET_REMOTE_CH1,"OFFSET_REMOTE_CH1",true,		false,NULL, "1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000", BKGSECTION},

	{eDETECTOR_ID,"DETECTOR_ID",			true,		false,NULL, "FD01", CONFIGSECTION},

	{eBGCYCLES_CHOICE,"BGCYCLES_CHOICE",false,		false,NULL, "   1  ,   2  ,  3,5,10", CONFIGSECTION},
	{eBGCYCLETIME_CHOICE,"BGCYCLESTIME_CHOICE",false,	false,NULL, "5,10,20,40,60", CONFIGSECTION},
	{eCYCLES_CHOICE,"CYCLES_CHOICE",false,		false,NULL, "1,2,3,5,10", CONFIGSECTION},
	{eCYCLETIME_CHOICE,"CYCLESTIME_CHOICE",false,	false,NULL, "5,10,20,40,60", CONFIGSECTION},
	{eDELTA_ENRICHMENT_CHOICE,"DELTA_ENRICHMENT_CHOICE",false,	false,NULL, "0.1,0.15,0.2,0.3,0.5,.75,1.0", CONFIGSECTION},
	{eDELTA_COOLINGTIME_CHOICE,"DELTA_COOLINGTIME_CHOICE",false,	false,NULL, "0.1,0.2,0.5,1.0,2.0,5.0,10.0", CONFIGSECTION},
	{eDETECTOR_ID_CHOICE,"DETECTOR_ID_CHOICE",false,	false,NULL, "FD01, FD02,FD03,FD04,FD05,FD06,FD07,FD08,FD09,FD10", CONFIGSECTION},

	{eBGCYCLES_CHOICE_CONF,"BGCYCLES_CHOICE_CONF",true,	false,NULL, "No", CONFIGSECTION},
	{eBGCYCLETIME_CHOICE_CONF,"BGCYCLESTIME_CHOICE_CONF",true,	false,NULL, "No", CONFIGSECTION},
	{eCYCLES_CHOICE_CONF,"CYCLES_CHOICE_CONF",true,	false,NULL, "No", CONFIGSECTION},
	{eCYCLETIME_CHOICE_CONF,"CYCLESTIME_CHOICE_CONF",true,	false,NULL, "No", CONFIGSECTION},
	{eDELTA_ENRICHMENT_CHOICE_CONF,"DELTA_ENRICHMENT_CHOICE_CONF",true,	false,NULL, "No", CONFIGSECTION},
	{eDELTA_COOLINGTIME_CHOICE_CONF,"DELTA_COOLINGTIME_CHOICE_CONF",true,	false,NULL, "No", CONFIGSECTION},
	{eDETECTOR_ID_CHOICE_CONF,"DETECTOR_ID_CHOICE_CONF",true,	false,NULL, "No", CONFIGSECTION},


	// ranges
	{eItem_RANGE,		"eItem_RANGE",		false,	false,NULL, "1,128", CONFIGSECTION},
	{eFacility_RANGE,	"eFacility_RANGE",	false,	false,NULL, "1,128", CONFIGSECTION},
	{eID_RANGE,			"eID_RANGE",		false,	false,NULL, "1,218", CONFIGSECTION},
	{eMeasType_RANGE,	"eMeasType_RANGE",	false,	false,NULL, "1,2", CONFIGSECTION},
	{eEnrichment_RANGE,	"eEnrichment_RANGE",false,	false,NULL, "0.00,100.0", CONFIGSECTION},
	{eBurnUp_RANGE,		"eBurnUp_RANGE",	false,	false,NULL, "0.00,100.0", CONFIGSECTION},
	{eDischDate_RANGE,	"eDischDate_RANGE",	false,	false,NULL, "1 January 1950, 1 February 2038", CONFIGSECTION},
	{eCycle_RANGE,		"eCycle_RANGE",		false,	false,NULL, "1,9", CONFIGSECTION},
	{eThresholdA_RANGE,	"eThresholdA_RANGE",false,	false,NULL, "0, 1e6", CONFIGSECTION},
	{eThresholdB_RANGE,	"eThresholdB_RANGE",false,	false,NULL, "0.00, 1e6", CONFIGSECTION},
	{eMeasDate_RANGE,	"eMeasDate_RANGE",	false,	false,NULL,  "1 January 1950, 1 February 2038", CONFIGSECTION},
	{eCoolingTime_RANGE,"eCoolingTime_RANGE",false,	false,NULL, "0.0, 1e6", CONFIGSECTION},
	{eNeutronA_RANGE,	"eNeutronA_RANGE",	false,	false,NULL, "0.0,	1e9", CONFIGSECTION},
	{eNeutronB_RANGE,	"eNeutronB_RANGE",	false,	false,NULL, "0.0,	1e9", CONFIGSECTION},
	{eNeutronC_RANGE,	"eNeutronC_RANGE",	false,	false,NULL, "0.0,	1e9", CONFIGSECTION},
	{eGamma1_RANGE,		"eGamma1_RANGE",	false,	false,NULL, "0.0,	1e9", CONFIGSECTION},
	{eGamma2_RANGE,		"eGamma2_RANGE",	false,	false,NULL, "0.0,	1e9", CONFIGSECTION},

	{eDetector_RANGE,"eDetector_RANGE",false,	false,NULL, "1,218", CONFIGSECTION}
};


/////////////////////////////////////////////////////////////////////////////
// CIniFileProcessor dialog

CIniFileProcessor::CIniFileProcessor(CWnd* pParent /*=NULL*/)
: CDialog(CIniFileProcessor::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIniFileProcessor)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_iSelected = -1;
	m_bInitialized = FALSE;
	m_bETStat = false;

}

CIniFileProcessor::~CIniFileProcessor()
{

}


void CIniFileProcessor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIniFileProcessor)
	DDX_Control(pDX, IDOK, m_cOK);
	DDX_Control(pDX, IDCANCEL, m_cCancel);
	DDX_Control(pDX, ID_EDIT_TOGGLE, m_cEdit);
	DDX_Control(pDX, IDC_LIST1, m_cListCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIniFileProcessor, CDialog)
	//{{AFX_MSG_MAP(CIniFileProcessor)
	ON_BN_CLICKED(ID_EDIT_TOGGLE, OnEditToggle)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, OnColumnclickList1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnItemchangedList1)
	ON_WM_SIZE()
	ON_WM_TIMER()


	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIniFileProcessor message handlers
//Facility	ID	Measurement type	Enrichment (%)	Burnup (GWd/MT)	Discharge date (dd-mm-year)	cycle #	Thres A	Thres B	 Measurement date (dd-mm-year)	Cooling Time (years)	 NA	 NB	 NC	 G1	 G2   Detector

const char* CIniFileProcessor::m_szFieldNames[4] = {
	"Option Name",
		"Current Value",
		"Default Value",
		"Section"
};


void CIniFileProcessor::CacheIniFileValues()
{
	for (int iData = 0; iData < ININAMECOUNT; iData++)
	{
		LoadSavedValues(iData);
	}
}

void CIniFileProcessor::Terminate()
{
	if (HasUnsavedChanges())
	{
		CString cTemp;
		cTemp = _T("FDMS can retain the settings that have been changed in this session.\r\nDo you want to retain the modified settings and option values?");
		if (AfxMessageBox(cTemp, MB_YESNO) == IDYES)
		{
			MakeModificationsPermanent();
		}
	}
	FreeValues();
}

void CIniFileProcessor::FreeValues()
{

	for (int iData = 0; iData < ININAMECOUNT; iData++)
	{
		if (m_Entry[iData].iniValue != NULL)
		{
			delete [] m_Entry[iData].iniValue;
			m_Entry[iData].iniValue = NULL;
		}
	}
}

BOOL CIniFileProcessor::OnInitDialog() 
{
	CDialog::OnInitDialog();

	for (int iData = 0; iData < 4; iData++)
		m_cListCtrl.InsertColumn(iData,m_szFieldNames[iData],
		LVCFMT_LEFT,
		m_cListCtrl.GetStringWidth(m_szFieldNames[iData])+100);

	{
		bool ColumnRWMap[4] = {false, true, false, false};
		m_cListCtrl.SetROColumns((bool*)ColumnRWMap);

	}

	m_cListCtrl.ModifyStyle(0, LVS_NOLABELWRAP | LVS_SINGLESEL | LVS_SHOWSELALWAYS);

	CString cTemp;
	int iNewItem = 0;
	for (int iData = 0; iData < ININAMECOUNT; iData++)
	{
		UpdateRow(true, iNewItem, iData);
		iNewItem++;
	}

	m_cListCtrl.SetExtendedStyle(m_cListCtrl.GetExtendedStyle()|LVS_EX_HEADERDRAGDROP | LVS_EX_FULLROWSELECT);
	// Win2k/XP only?
	//m_cListCtrl.SetExtendedStyle(m_cListCtrl.GetExtendedStyle()|LVS_EX_TRACKSELECT |LVS_EX_ONECLICKACTIVATE );
	GetDlgItem(IDOK)->EnableWindow(false);

	GetClientRect(&m_OldRect);
/*	CRect initRect;
	initRect.left = m_OldRect.right - GetSystemMetrics(SM_CXHSCROLL);
	initRect.top = m_OldRect.bottom - GetSystemMetrics(SM_CYVSCROLL);
	m_Grip.Create(WS_CHILD | SBS_SIZEBOX | SBS_SIZEBOXBOTTOMRIGHTALIGN | 
		SBS_SIZEGRIP | WS_VISIBLE, initRect, this, AFX_IDW_SIZE_BOX);
*/
	m_bInitialized = TRUE;
	return TRUE; 
}

bool CIniFileProcessor::UpdateValue(int iEntry, int iColumnID, LPCSTR p)
{
	bool res = false;
	switch (iColumnID)
	{
	case 1: // changing value allowed
		{
			int l = CLAMP(strlen(p),0,510);
			if  (0 != strcmp ( p, m_Entry[iEntry].iniValue) )  // they are not the same
			{
				TCHAR foo[512];
				CString::CopyChars(foo, p, l);
				foo[l] = 0;
				if (m_Entry[iEntry].iniValue != NULL)
				{
					delete [] m_Entry[iEntry].iniValue;
				}

				m_Entry[iEntry].iniValue = new TCHAR[strlen(foo)+1];
				strcpy(m_Entry[iEntry].iniValue, foo);
				m_Entry[iEntry].bChanged = true;
				res = true;
			}
			else
			{
				// no change, do nothing
			}
		}
		break;
	case 0: // cannot change the name
	case 2: // cannot change default value
	case 3: // changing section not allowed either
		break;
	}

	return res;
}

LRESULT CIniFileProcessor::OnSubEdit(WPARAM , LPARAM p) 
{
	LV_DISPINFO *dispinfo = (LV_DISPINFO*)p;

	if (dispinfo->item.pszText != NULL)
	{
		TCHAR bar[512];
		dispinfo->item.cchTextMax = CLAMP(dispinfo->item.cchTextMax, 0, 510); // leave trailing blank space
		CString::CopyChars(bar,dispinfo->item.pszText, dispinfo->item.cchTextMax);
		bar[dispinfo->item.cchTextMax] = 0;
		if (UpdateValue(dispinfo->item.iItem , dispinfo->item.iSubItem, (PTCHAR)&bar))
			m_cListCtrl.SetItemText(dispinfo->item.iItem, dispinfo->item.iSubItem, (PTCHAR)&bar);
		else
			m_cListCtrl.SetItemText(dispinfo->item.iItem, dispinfo->item.iSubItem, (PTCHAR)&bar);
	}

	return 0;
}

void CIniFileProcessor::OnEditToggle() 
{
	m_bETStat = !m_bETStat;
	if (m_bETStat)
	{
		SetDlgItemText(ID_EDIT_TOGGLE, "Lock Data");
		m_cListCtrl.ModifyStyle(0, LVS_EDITLABELS);
		m_cListCtrl.ModifyStyleEx(0, WS_EX_CLIENTEDGE | WS_EX_DLGMODALFRAME);	   
	}
	else
	{
		SetDlgItemText(ID_EDIT_TOGGLE, "Edit Data");
		m_cListCtrl.ModifyStyle(LVS_EDITLABELS, 0);
		m_cListCtrl.ModifyStyleEx(WS_EX_CLIENTEDGE | WS_EX_DLGMODALFRAME, 0);
	}
}

int CIniFileProcessor::GetIntValue(tIniNames i)
{
	int d = 0;
	if ((i >= eBGCYCLETIME && i < ININAMECOUNT) && m_Entry[i].iniValue)
		sscanf( m_Entry[i].iniValue, "%d", &d);
	return d;
}
int CIniFileProcessor::GetStringValue(tIniNames i, PTCHAR p, int len)
{
	int d = -1;
	if ((i >= eBGCYCLETIME && i < ININAMECOUNT) && m_Entry[i].iniValue)
	{
		len = CLAMP(len, 0, (int)strlen(m_Entry[i].iniValue));
		strncpy(p, m_Entry[i].iniValue, len);
		p[len] = 0;
		d = len;
	}
	return d;
}

bool  CIniFileProcessor::GetString(tIniNames i, CString& szV)
{
	char loc[512];
	int n;
	n = CIniFileProcessor::GetStringValue(i,loc,sizeof(loc));
	if (n > 0)
		szV = loc;
	return true;
}

bool CIniFileProcessor::GetULFullPath(tIniNames i, CString& szFP)
{
	char loc[380];
	char nam[256];
	int l, n;

	switch(i)
	{
	case eIRININAME:
		strcpy(loc, DefaultSourcePath.GetBuffer());
		l = 1;
		break;
	default:
		l = CIniFileProcessor::GetStringValue((tIniNames)(i - 5),loc,sizeof(loc));
	}

	n = CIniFileProcessor::GetStringValue(i,nam,sizeof(nam));
	if (l > 0)
	{
		char *c = strrchr(loc,'\\');//find the last "\", 
		if (c) 
			*c = '\0';
		if (i == eIRININAME || i == eIRDATAFILENAME)  // jfl hack attack warning
		{
			c = strrchr(loc,'\\');//find the last "\", removing the probable "fdms" location
			if (c) 
				*c = '\0';
		}
		szFP.Format("%s\\%s",loc, nam);
	}
	else
		szFP = nam;

	return true;

}

// assuming all args are appended to any args that the UL expects, so decrement __argc by 1 if the specific arg we are looking for is found
bool CIniFileProcessor::ExtractFlags(LPTSTR m_lpCmdLine)
{
	// looking for debug flag only
	TCHAR foo[512];
	strcpy(foo, m_lpCmdLine);
	_strlwr(foo);
	char* c = strstr(foo, "/demo");
	if (c != NULL)
	{
		int index = foo - c;
		CIniFileProcessor::SaveSingleValue(eDEMOVERSION,"yes");
		for (int i = 0; i < 5; i++)
			m_lpCmdLine[index+i] = ' ';
		__argc--;
	}
	return true;

}


int CIniFileProcessor::GetBooleanValue(tIniNames i, bool& bValue)
{
	char szValue[64];
	int dkfj = GetStringValue(i,szValue,sizeof(szValue));
	szValue[0] = (char)toupper(szValue[0]);
	if ((szValue[0] != 'N') && (szValue[0] != 'Y'))
		szValue[0] = 'Y';
	if (szValue[0] == 'N')
		bValue = false;
	else
		bValue = true;

	return dkfj;
}

int CIniFileProcessor::GetStringArrayValues(tIniNames i, CStringArray& Array, int& iArrayLen)
{
	char temp[_MAX_PATH];
	int d = CIniFileProcessor::GetStringValue(i,temp,sizeof(temp));
	int n = 0;
	char seps[]   = ","; // commas ONLY
	char *token;

	if (d < 0)
		return d;

	token = strtok( temp, seps );
	while( token != NULL && n < iArrayLen )
	{
		CString s(token);
		s.Trim();
		Array.Add(s);
		n++;
		token = strtok( NULL, seps );
	}
	iArrayLen = n;
	return d;
}

int CIniFileProcessor::GetRangeValue(tIniNames i, CString& szLow, CString& szHigh)
{
	char temp[MAX_PATH];
	int d = CIniFileProcessor::GetStringValue(i,temp,sizeof(temp));
	char seps[]   = ","; // commas ONLY
	char *token;

	if (d < 0)
		return d;

	token = strtok( temp, seps );
	if( token != NULL )
	{
		szLow = token;
	}
	token = strtok( NULL, seps );
	if( token != NULL )
	{
		szHigh = token;
	}

	return d;
}

void CIniFileProcessor::UpdateRow(bool bNew, int iItem, int iData)
{
	CString cTemp;
	int iColumn = 1;

	cTemp.Format(_T("%s"),m_Entry[iData].name);

	if (bNew)
	{
		m_cListCtrl.InsertItem(iItem,cTemp);
	}
	else
	{
		m_cListCtrl.SetItemText(iItem,iColumn,cTemp);
	}

	cTemp.Format(_T("%s"),m_Entry[iData].iniValue);
	m_cListCtrl.SetItemText(iItem,iColumn++,cTemp);

	cTemp.Format(_T("%s"),m_Entry[iData].defIniValue);
	m_cListCtrl.SetItemText(iItem,iColumn++,cTemp);

	m_cListCtrl.SetItemText(iItem,iColumn++,m_Entry[iData].section);
}

void CIniFileProcessor::LoadSavedValues(int iData)
{
	TCHAR foo[512];

	// get value from the real ini file source
	//DWORD dw = 
		GetPrivateProfileString(
		m_Entry[iData].section,
		m_Entry[iData].name,
		m_Entry[iData].defIniValue,
		foo,sizeof(foo),DefaultSourceFullPath);

	if (m_Entry[iData].iniValue != NULL)
	{
		delete [] m_Entry[iData].iniValue;
	}

	m_Entry[iData].iniValue = new TCHAR[strlen(foo)+1];
	foo[strlen(foo)] = 0;  // redundant
	strcpy(m_Entry[iData].iniValue, foo);

}

bool CIniFileProcessor::HasUnsavedChanges()
{
	for (int iData = 0; iData < ININAMECOUNT; iData++)
	{
		if(m_Entry[iData].bChanged)
		{
			if (iData != eULAPPSETTINGSLOC && iData != eULINILOC && // don't save any changes made by the facility manager
				iData != eDEMOVERSION) // don't save any changes made on the command line
				return true;
		}
	}

	return false;
}

bool CIniFileProcessor::MakeModificationsPermanent()
{
	for (int iData = 0; iData < ININAMECOUNT; iData++)
	{
		if(m_Entry[iData].bChanged)
		{
			if ( WritePrivateProfileString(
				m_Entry[iData].section,
				m_Entry[iData].name,
				m_Entry[iData].iniValue,
				DefaultSourceFullPath) )
			{
				m_Entry[iData].bChanged = false;
			}
		}
	}
	return true;
}

bool CIniFileProcessor::SaveSingleValue(tIniNames i,LPCSTR lpString)
{
	return UpdateValue(i, 1, lpString);
}


bool CIniFileProcessor::SaveFullConfigFile(CString szFileName, bool bUseDefaults)
{
	for (int iData = 0; iData < ININAMECOUNT; iData++)
	{
		if (bUseDefaults)
		{
			WritePrivateProfileString(
				m_Entry[iData].section,
				m_Entry[iData].name,
				m_Entry[iData].defIniValue,
				szFileName);
		}
		else
		{
			WritePrivateProfileString(
				m_Entry[iData].section,
				m_Entry[iData].name,
				m_Entry[iData].iniValue,
				szFileName);
		}
	}
	return true;
}

void CIniFileProcessor::OnColumnclickList1(NMHDR* , LRESULT* pResult) 
{
//	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	//this is the column selected to sort on.
	//pNMListView->iSubItem;


	*pResult = 0;
}





void CIniFileProcessor::OnOK() 
{
	m_iSelected = -1;

	CDialog::OnOK();
}



void CIniFileProcessor::OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	//this is the column selected to sort on.

	if (pNMListView->uChanged & LVIF_STATE)
	{
		if (pNMListView->uNewState != 0)
			GetDlgItem(IDOK)->EnableWindow(true);
		else
			GetDlgItem(IDOK)->EnableWindow(false);
	}
	*pResult = 0;
}
#define INVALIDATETIMER 2500

void CIniFileProcessor::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	if(m_bInitialized && cx > 0 && cy > 130)
	{
		KillTimer(INVALIDATETIMER);
		int iY_Offset = cy - m_OldRect.bottom;
		int iX_Offset = cx - m_OldRect.right;

		CRect rect;
		CWnd *item;
		item = GetDlgItem(IDC_LIST1);
		item->GetWindowRect(&rect);
		ScreenToClient(&rect);
		item->MoveWindow(rect.left, rect.top,rect.Width() + iX_Offset, 
			rect.Height() + iY_Offset,TRUE);

		item = GetDlgItem(IDOK);
		item->GetWindowRect(&rect);
		ScreenToClient(&rect);
		item->MoveWindow(rect.left,rect.top + iY_Offset,rect.Width(),
			rect.Height(),TRUE);

		item = GetDlgItem(IDCANCEL);
		item->GetWindowRect(&rect);
		ScreenToClient(&rect);
		item->MoveWindow(rect.left,rect.top + iY_Offset,rect.Width(),
			rect.Height(),TRUE);

		item = GetDlgItem(ID_EDIT_TOGGLE);
		item->GetWindowRect(&rect);
		ScreenToClient(&rect);
		item->MoveWindow(rect.left,rect.top + iY_Offset,rect.Width(),
			rect.Height(),TRUE);

		CWnd *resizeWnd = GetDlgItem(AFX_IDW_SIZE_BOX);
		if(resizeWnd)
		{
			CRect rect,rect2;
			resizeWnd->GetWindowRect(&rect);
			GetClientRect(rect2);
			rect2.left = rect2.right - rect.Width();
			rect2.top = rect2.bottom - rect.Height();
			resizeWnd->MoveWindow(&rect2);
		}
		GetClientRect(&m_OldRect);
		//invalidate
		SetTimer(INVALIDATETIMER,100,NULL);
	}
}

void CIniFileProcessor::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == INVALIDATETIMER)
	{
		Invalidate(TRUE);
		KillTimer(INVALIDATETIMER);
	}
	CDialog::OnTimer(nIDEvent);
}


bool CIniFileProcessor::MakeNewConfigFile()
{
	FILE* temp;
	if ((temp = fopen(CIniFileProcessor::DefaultSourceFullPath,"wt"))!=NULL)
	{
		fprintf(temp,"[CONFIGURATION]\n");
		fprintf(temp,"DATABASE=%s\n",DBFNAME);
		fprintf(temp,"MEASUREMENTPLAN=%s\n",MPFNAME);
		fprintf(temp,"MEASUREMENTS=%s\n",MFNAME);
		fprintf(temp,"DELTA_ENRICHMENT=0.15\n");
		fprintf(temp,"DELTA_COOLINGTIME=5.0\n");
		fprintf(temp,"\n");

		fprintf(temp,"[GRAND]\n");
		fprintf(temp,"MINIGRAND=Yes\n");
		fprintf(temp,"PORT=COM1\n");
		fprintf(temp,"BAUD=9600\n");
		fprintf(temp,"PARITY=NONE\n");
		fprintf(temp,"DATABITS=8\n");
		fprintf(temp,"STOPBITS=1\n");
		fprintf(temp,"TICKLE=1000\n");
		fprintf(temp,"COMMANDTIMEOUT=10\n");
		fprintf(temp,"ICHVBIAS=300\n");
		fprintf(temp,"HVBIAS=1000\n");

		fprintf(temp,"OFFSET_CNTRL=Yes\n");
		fprintf(temp,"OFFSET_CNTRL_REMOTE=No\n");
		fprintf(temp,"OFFSET_LIMIT_LOW=700\n");
		fprintf(temp,"OFFSET_LIMIT_HIGH=1200\n");
		fprintf(temp,"OFFSET_TIME=60\n");
		fprintf(temp,"BGCYCLETIME=30\n");
		fprintf(temp,"BGCYCLES=3\n");
		fprintf(temp,"CYCLETIME=20\n");
		fprintf(temp,"CYCLES=3\n");
		fprintf(temp,"\n");

		fprintf(temp,"[BACKGROUND]\n");
		fprintf(temp,"BACKGROUND_A=0.000\n");
		fprintf(temp,"BACKGROUND_B=0.000\n");
		fprintf(temp,"BACKGROUND_C=0.000\n");
		fprintf(temp,"BACKGROUND_1=0.000\n");
		fprintf(temp,"BACKGROUND_2=0.000\n");
		fprintf(temp,"\n");

		fclose(temp);
		return true;
	}
	else
		return false;
}

bool CIniFileProcessor::Confirm(tIniNames /*i*/)
{
	// if this value has a confirmation value per change, prompt user
	return false;
}


bool  CIniFileProcessor::GetIRSFacilityID(short& iVal) // unique function to retrieve the id from a unique ini file only for use with IRS
{
	CString irsinipath;
	TCHAR foo[64];
	CIniFileProcessor::GetULFullPath(eIRININAME, irsinipath);

	// Get value from the real ini file source
	// We do it this way (always reloading the file content) as it may change externally, and it resides in a different file from the FDMS.ini anyway
	DWORD dw = 
		GetPrivateProfileString(
		m_Entry[eFACILITYID].section,
		m_Entry[eFACILITYID].name,
		m_Entry[eFACILITYID].defIniValue,
		foo,sizeof(foo),irsinipath);

	if (dw == 0)
	{
		return false;
	}

	if (m_Entry[eFACILITYID].iniValue != NULL)
	{
		delete [] m_Entry[eFACILITYID].iniValue;
	}

	m_Entry[eFACILITYID].iniValue = new TCHAR[strlen(foo)+1];
	foo[strlen(foo)] = 0;  // redundant
	strcpy(m_Entry[eFACILITYID].iniValue, foo);

	iVal = (short)GetIntValue(eFACILITYID);

	return true;
}
