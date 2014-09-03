//////////////////////////////////////////////////////////////////////////
// Copyright 2004, The Regents Of the University of California.  This
// software was produced under U.S. Government contract (W-7405-ENG-36) at
// the Los Alamos National Laboratory by the Safeguards Science and
// Technology Group (N-1).  All Rights Reserved.
//////////////////////////////////////////////////////////////////////////
// $History: IRSINI.cpp $
// 
// *****************  Version 6  *****************
// User: 186846       Date: 2/24/10    Time: 5:02p
// Updated in $/FDMS/FDMSNET
// 
// *****************  Version 5  *****************
// User: 186846       Date: 2/26/08    Time: 8:54a
// Updated in $/FDMS/FDMS Application/FDMSNET
// checkpoint
// 
// *****************  Version 4  *****************
// User: 186846       Date: 12/19/07   Time: 2:42p
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 3  *****************
// User: Longo        Date: 3/16/05    Time: 2:21p
// Updated in $/FDMS/FDMS Application/FDMSNET
// nearing the deadlines ...
// 
// *****************  Version 2  *****************
// User: Longo        Date: 12/10/04   Time: 4:44p
// Updated in $/FDMS/FDMS Application/FDMSNET
// Combining the GrandCom demo class wirth the SerialCom live detector
// control class
// 
// *****************  Version 1  *****************
// User: Longo        Date: 11/19/04   Time: 4:28p
// Created in $/FDMS/FDMS Application/FDMSNET
// uses FM to get UL config file paths

#include "stdafx.h"

#import "..\Coms\CoFacilityMgr.exe"  no_namespace named_guids

#include <msxml2.h>
#include <stdio.h>
#include <comutil.h>


#include ".\FDMS.h"
#include ".\IniFileProcessor.h"
#include ".\StartupImpDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define IRCONFIGFOLDER  "config"
#define IRFDMSCONFIGFOLDERNAME  "config\\fdms\\"
#define IRDATANAME  "data\\fdms\\"


void 	CIniFileProcessor::PrepareULInfo()
{

	// get the location of the irs.ini file, this has the facility #
	// get the facility number if it is found.

	// Also, buildout the location for the data dir, the ulayer ini and app ini (in config dir)
	StartupImpDlg StartUpDlg;
	StartUpDlg.DoIt("Checking for related applications . . .");

	short FacilityID;
	bool found = CIniFileProcessor::GetIRSFacilityID(FacilityID);
	if (found && FacilityID >= 0)  //  file content found that works, so no UL code is in play
	try
	{
	
		// Using the Facility Manager, obtain info on the current facility
		CString FacilityDirectory;
		IFacilityMgrDataPtr pIFacMgrData(CLSID_FacilityMgrData);
		FacilityDirectory = pIFacMgrData->GetFacilityDirectory(FacilityID).GetBSTR();

		CString r = FacilityDirectory.Right(1);
		if (r.Compare("\\") != 0)
			FacilityDirectory.AppendChar('\\');

		pIFacMgrData = NULL;
		m_bULPresent = true;

		// NOW: finally, we can set the config. data and file name(s) for eULAPPSETTINGSNAME and eULININAME
		if (FacilityDirectory.GetLength() > 1)
		{
			CString configdir = FacilityDirectory;
			CString datadir = FacilityDirectory;
			datadir.Append(IRDATANAME); // todo: jfl hack warning
			configdir.Append(IRFDMSCONFIGFOLDERNAME);  // todo: jfl hack warning
			CIniFileProcessor::SaveSingleValue(eULINILOC, configdir);
			CIniFileProcessor::SaveSingleValue(eULAPPSETTINGSLOC, configdir);
			CIniFileProcessor::SaveSingleValue(eIRDATALOC, datadir);

			bool bModFilePaths = CreateIRSSchemeAssets(NULL, FacilityDirectory.GetBuffer(), configdir.GetBuffer(), datadir.GetBuffer());
			if (bModFilePaths)
			{

			}
		}
	}
	catch(...)
	{
		// no facility manager found, thus, no UL in play
		m_bULPresent  = false;
	}

	StartUpDlg.DestroyWindow();

	CIniFileProcessor::GetULFullPath(eULININAME, ULSupport::ULIniPath);
	CIniFileProcessor::GetULFullPath(eULAPPSETTINGSNAME, ULSupport::ULAppSettingsIniPath);
	CIniFileProcessor::GetULFullPath(eIRININAME, ULSupport::ULIRSIniPath);
	CIniFileProcessor::GetULFullPath(eIRDATAFILENAME, ULSupport::ULIRSDataPath);

}

// create IRS assets
// given facility folder foo with foo\config and foo\data
//		create foo\config\fdms\*.ini and 
//		create foo\data\fdms\data\*csv

#include "atlfile.h"

typedef enum {eDB, eMPlan, eMeas, eULIni, eAppIni, eMax} FDMSFileType;

HRESULT LoadNewFDMSFileResource(UINT file, LPVOID & lpIFBinaryResLock)
{
	static const WORD idmap[eMax] = {IDR_DBCSV,IDR_MPCSV,IDR_MSCSV,IDR_ULINI,IDR_APPINI};

	HGLOBAL hResLoad;     // handle to loaded resource 
	HRSRC hRes;         // handle/ptr. to res. info. in hExe 
	HRESULT hr = S_OK;
	LPCTSTR ir = MAKEINTRESOURCE(idmap[file]);
	char szModule[_MAX_PATH];
	::GetModuleFileName(NULL,szModule, sizeof(szModule));

	HMODULE hm = GetModuleHandle( szModule );  

//	CLogWin& logref = CmdLineState::GetCmdLineState();

	hRes = FindResource(hm, ir, RT_RCDATA);
	if (hRes == NULL) 
	{ 
		DWORD foo = GetLastError();
//		logref.BatchLogL("Resource id %ul not located (%ul)", IDR_INCCIRSINI, foo);
		return S_FALSE;
	} 

	// Load the binary resource into global memory. 
	hResLoad = LoadResource(hm, hRes); 
	if (hResLoad == NULL) 
	{ 
		DWORD foo = GetLastError();
//		logref.BatchLogL("Resource data %ul not loaded (%ul)", foo);
		return S_FALSE;
	}

	// Lock the DB binary into global memory. 
	lpIFBinaryResLock = LockResource(hResLoad); 
	if (lpIFBinaryResLock == NULL) 
	{ 
		DWORD foo = GetLastError();
//		logref.BatchLogL("Resource not locked (%ul)", foo);
		return S_FALSE;
	} 

	return hr;
}
#include <atlpath.h>

void BuildFDMSFilePath(UINT file, CPath& Path)
{
	static const char* namemap[eMax] = {"database.csv","measurementplan.csv","measurements.csv",
		"ULAYR.ini","ApplicationSettings.ini"};

	Path.Append(namemap[file]);
}

bool CreateFDMSFile(FDMSFileType f, const CPath& ContextPath, CPath& FullPath)
{
	static const int sizemap[eMax] = {196,202,202,6598,803};

	bool bCreated = false;
	LPVOID lpIFBinaryResLock = NULL;    // pointer to resource data 
	HRESULT hr = LoadNewFDMSFileResource(f, lpIFBinaryResLock);  // locks resource handle

	// now write it
	if (hr == S_OK && lpIFBinaryResLock)
	{
		FullPath = ContextPath;
		BuildFDMSFilePath(f, FullPath);
		CAtlFile ini;

		HRESULT hr = ini.Create(FullPath,    // file to open
			GENERIC_WRITE,          // open for writing
			FILE_SHARE_WRITE,       // share for writing
			CREATE_NEW,         // existing file only
			FILE_ATTRIBUTE_NORMAL);
		if (hr != S_OK) 
		{ 
			//sprintf(msg1, "Cannot create file %.256s (0x%.8x)", FullPath, hr);
		}
		else
		{
			DWORD bytes_written = 0;
			const DWORD dwMemlen = DWORD(sizemap[f]);
			hr = ini.Write(lpIFBinaryResLock,dwMemlen,&bytes_written);
			if (FAILED(hr))
			{
				//sprintf(msg1, "Cannot create file %.256s (0x%.8x)", FullPath, hr);
			}
			else
			{
				//sprintf(msg1, "Created FDMS IRS configuration folder and file material %.256s", FullPath);
				bCreated = true;
			}
		}
	}

	if (lpIFBinaryResLock)
	{
		UnlockResource(lpIFBinaryResLock);
		BOOL freeres = FreeResource(lpIFBinaryResLock);
	}

	return bCreated;
}


#include <io.h>
bool CIniFileProcessor::CreateIRSSchemeAssets(PTCHAR pMsg, PTCHAR facility, PTCHAR config, PTCHAR data)
{
	bool bCreatedCfg = false, bCreatedDB = false;
	//See if config and data locs are there

	CPath fac(facility);
	if (!fac.IsDirectory())
		return false;

	//if logging is enabled and silent creation is enabled and what else is enabled and they are not there, create them
	CPath FDMScfg(config);
	CPath FDMSdata(data);

	fac.Append(IRCONFIGFOLDER);

	// we act only when fac folder exists, and FDMS subs do not
	BOOL stat,
		facexists = (_access(fac, 00) == 0);

	if  (facexists)
	{
//		char msg1[512];
//		char msg2[512];
//		msg1[0] = msg2[0]= NULL;
//		CLogWin& logref = CmdLineState::GetCmdLineState();
		CPath NewPath;
		bool bNoCfg = (_access( FDMScfg, 00 ) == -1);  // config folder not there
		bool bNoDB = (_access( FDMSdata, 00 ) == -1);  // data folder not there
		if (bNoCfg || bNoDB)
		{
//			strcpy(pMsg, "INCC configuration and database folders not found: using default content.\r\n");
		}
		if (bNoCfg)  // config folder not there
		{
			// create config folder with app and UL ini's within the folder
			stat = CreateDirectory(FDMScfg, NULL);	
			if (stat)
			{
				bCreatedCfg = CreateFDMSFile(eULIni, FDMScfg, NewPath);
				bCreatedCfg = CreateFDMSFile(eAppIni, FDMScfg, NewPath);
			}
			else
			{
				//sprintf(msg1, "Cannot create FDMS IRS configuration folder at %.256s", FDMScfg);
			}
		}
		if  (bNoDB)  // data folder not there
		{
			// create data folder then the database materials within the data folder
			stat = CreateDirectory(FDMSdata, NULL);	
			if (stat)
			{
				bCreatedDB = CreateFDMSFile(eDB, FDMSdata, NewPath);
				if (bCreatedDB)
				{
					CIniFileProcessor::SaveSingleValue(eDATABASE,NewPath);
					strcpy(g_szDBFilePathName,NewPath);
				}
				bCreatedDB = CreateFDMSFile(eMPlan, FDMSdata, NewPath);
				if (bCreatedDB)
				{
					CIniFileProcessor::SaveSingleValue(eMEASUREMENTPLAN,NewPath);
					strcpy(g_szMPFilePathName,NewPath);
				}
				bCreatedDB = CreateFDMSFile(eMeas, FDMSdata, NewPath);
				if (bCreatedDB)
				{
					CIniFileProcessor::SaveSingleValue(eMEASUREMENTS,NewPath);
					strcpy(g_szMLFilePathName,NewPath);
				}
			}

				//			if (bCreatedDB)
//				sprintf(msg2, "Created INCC IRS database %.256s", GetULINCCDataPath());
//			else
//				sprintf(msg2, "Cannot create NCC IRS database at %.256s",GetULINCCDataPath());
		}
//		if (strlen(msg1) > 0)
//		{
//			strcat(pMsg, msg1); strcat(pMsg, "\r\n");
//		}
//		if (strlen(msg2) > 0)
//		{
//			strcat(pMsg, msg2);
//		}
	}
	return bCreatedCfg || bCreatedDB;
}

