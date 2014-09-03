//////////////////////////////////////////////////////////////////////////
// Copyright 2005, The Regents Of the University of California.  This
// software was produced under U.S. Government contract (W-7405-ENG-36) at
// the Los Alamos National Laboratory by the Safeguards Science and
// Technology Group (N-1).  All Rights Reserved.
//////////////////////////////////////////////////////////////////////////
// $History: FDMS.h $ 
// 
// *****************  Version 16  *****************
// User: 186846       Date: 2/24/10    Time: 5:02p
// Updated in $/FDMS/FDMSNET
// 
// *****************  Version 15  *****************
// User: 186846       Date: 4/01/08    Time: 3:40p
// Updated in $/FDMS/FDMSNET
// Mar. 29, fix for 398, partial for 447
// 
// *****************  Version 14  *****************
// User: Longo        Date: 3/16/05    Time: 2:21p
// Updated in $/FDMS/FDMS Application/FDMSNET
// nearing the deadlines ...
// 
// *****************  Version 13  *****************
// User: Longo        Date: 3/09/05    Time: 3:01p
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 12  *****************
// User: Longo        Date: 2/28/05    Time: 10:00a
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 11  *****************
// User: Longo        Date: 2/10/05    Time: 3:46p
// Updated in $/FDMS/FDMS Application/FDMSNET
// checkpoint checkin
// 
// *****************  Version 10  *****************
// User: Longo        Date: 2/07/05    Time: 10:27a
// Updated in $/FDMS/FDMS Application/FDMSNET
// checkpoint checkin
// 
// *****************  Version 9  *****************
// User: Longo        Date: 1/07/05    Time: 2:25p
// Updated in $/FDMS/FDMS Application/FDMSNET
// adding the secondary import editor dilaog
// 
// *****************  Version 8  *****************
// User: Longo        Date: 12/21/04   Time: 4:13p
// Updated in $/FDMS/FDMS Application/FDMSNET
// added a few things
// 
// *****************  Version 7  *****************
// User: Longo        Date: 12/10/04   Time: 6:25p
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 6  *****************
// User: Longo        Date: 12/10/04   Time: 4:44p
// Updated in $/FDMS/FDMS Application/FDMSNET
// Combining the GrandCom demo class wirth the SerialCom live detector
// control class
// 
// *****************  Version 5  *****************
// User: Longo        Date: 11/30/04   Time: 4:22p
// Updated in $/FDMS/FDMS Application/FDMSNET
// added code to auto-select the matching row in the measeur dialog list,
// when involkd by IR tool review message
// 
// *****************  Version 4  *****************
// User: Longo        Date: 10/20/04   Time: 1:10p
// Updated in $/FDMS/FDMS Application/FDMSNET
// registry failed today, saving all work
// 
// *****************  Version 3  *****************
// User: Longo        Date: 9/17/04    Time: 3:32p
// Updated in $/FDMS/FDMS Application/FDMSNET
// First integration of the MFC Upper Layer code
// FDMS.h : main header file for the FDMS application
//
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FDMS_H__04E4FC10_684F_48AC_AB68_7EEBDA538D80__INCLUDED_)
#define AFX_FDMS_H__04E4FC10_684F_48AC_AB68_7EEBDA538D80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "MeasureDlg.h"
#include "VerifyDlg.h"
#include "AllOtherDlg.h"
#include "IniFileProcessor.h"
#include "LogFile.h"


#include ".\AppUL\FDMSReview.h"
#include ".\AppUL\FDMSSetup.h"
#include ".\AppUL\FDMSImpReimp.h"

#include "RTTitle.h" 
#include "RevwTool.h"
#include "RTInit.h"
#include "LoggingLog.h"

#define szTOOL_VERSION	"Version 1.0"  //must change with each new release
#define szCONTACT_NAME		"DEVELOPER NAME HERE"
#define szCONTACT_EMAIL		"SOMEONE@lanl.gov"
/////////////////////////////////////////////////////////////////////////////
// CFDMSApp:
// See FDMS.cpp for the implementation of this class
//

typedef enum {NeutA,NeutB,NeutC,Gamma1,Gamma2,MaxChannels} DeviceChannelType;

typedef struct
{
	double val[MaxChannels];
} ChannelCounts;

typedef enum {UnspecifiedVerification = 0, CycleVerification = 1, AssemblyVerification = 2, /*RatesOnly = 3,*/ MeasurementTypeCount = 4} tMeasurementType;

typedef enum  {eBackgroundAssay, eNormalAssay, eBaselineAssay, eReVerificationAssay, eUnknownAssay} tAssayTypes;


#define NAMESHORT		"FDMS"

// todo: jfl these hard-coded ranges will cause trouble later, please account for buffer overrun everywhere 
#define DB_ARRAYSIZE	10000
#define MP_ARRAYSIZE	 1000
#define CV_ARRAYSIZE		9

#define MY_SELECT		(WM_USER+6)
#define MY_SHOW			(WM_USER+5)
#define MY_SUBEDIT_FINISHED	(WM_USER+3)
#define MY_ENTEM_EDIT		(WM_USER+4)

#define MENU_SPAN 50

#define MAX_NAME_LENGTH	32
#define MAX_ID_LENGTH	32

//extern char		g_cIniFile[MAX_PATH];
extern int		g_iEntriesInDB;
extern char		g_szDBFilePathName[280];
extern char		g_szDBFileHeader[200];
extern char	g_szFacilityNames[DB_ARRAYSIZE][MAX_NAME_LENGTH];
extern char	g_szItemIDs[DB_ARRAYSIZE][MAX_ID_LENGTH];

extern int		g_iMeasurementType[DB_ARRAYSIZE];
extern float	g_fEnrichmentLevel[DB_ARRAYSIZE];
extern float	g_fBurnUp[DB_ARRAYSIZE];
extern int		g_iDischargeDateDay[DB_ARRAYSIZE];
extern int		g_iDischargeDateMonth[DB_ARRAYSIZE];
extern int		g_iDischargeDateYear[DB_ARRAYSIZE];
extern int		g_iCycle[DB_ARRAYSIZE];
extern float	g_fNChanAThresh[DB_ARRAYSIZE];
extern float	g_fNChanBThresh[DB_ARRAYSIZE];
extern int		g_iMeasurementDateDay[DB_ARRAYSIZE];
extern int		g_iMeasurementDateMonth[DB_ARRAYSIZE];
extern int		g_iMeasurementDateYear[DB_ARRAYSIZE];
extern float	g_fCoolingTime[DB_ARRAYSIZE];
extern float	g_fNChanA[DB_ARRAYSIZE];
extern float	g_fNChanB[DB_ARRAYSIZE];
extern float	g_fNChanC[DB_ARRAYSIZE];
extern float	g_fGDose1[DB_ARRAYSIZE];
extern float	g_fGDose2[DB_ARRAYSIZE];
extern double	g_dTemp[DB_ARRAYSIZE];
extern float	g_fLogBurnup[DB_ARRAYSIZE];
extern float	g_fLogCoolingTime[DB_ARRAYSIZE];
extern float	g_fLogDecayCorTotNeutrons[DB_ARRAYSIZE];
extern float	g_fLogGammaDose[DB_ARRAYSIZE];
extern float    g_fLogNoverG[DB_ARRAYSIZE];

extern char		g_szDetectorIDs[DB_ARRAYSIZE][MAX_NAME_LENGTH];

extern int		g_iError;
extern BOOL		g_bPlot1;
extern BOOL		g_bPlot2;
extern BOOL		g_bPlot3;
extern BOOL		g_bPlot4;
extern BOOL		g_bIncludeGood;
extern BOOL		g_bIncludeSuspect;
extern BOOL		g_bAcceptGood;
extern BOOL		g_bPlotMostRecent;

extern char	g_szMLFilePathName[280];
extern char	g_szMPFilePathName[280];

extern int 	g_iMPItemIndices[MP_ARRAYSIZE];
extern int	g_iMPEntriesIn;
extern char	g_szMPFacilityNames[MP_ARRAYSIZE][MAX_NAME_LENGTH];
extern char	g_szMPItemIDs[MP_ARRAYSIZE][MAX_ID_LENGTH];
extern int	g_iMPMeasurementType[MP_ARRAYSIZE];
extern float	g_fMPEnrichmentPercent[MP_ARRAYSIZE];
extern float	g_fMPBurnUp[MP_ARRAYSIZE];
extern int	g_iMPDischargeDateDay[MP_ARRAYSIZE];
extern int	g_iMPDischargeDateMonth[MP_ARRAYSIZE];
extern int	g_iMPDischargeDateYear[MP_ARRAYSIZE];
extern int	g_iMPCycle[MP_ARRAYSIZE];
extern float	g_fMPNChanAThresh[MP_ARRAYSIZE];
extern float	g_fMPNChanBThresh[MP_ARRAYSIZE];
extern int		g_iMPMeasurementDateDay[MP_ARRAYSIZE];
extern int		g_iMPMeasurementDateMonth[MP_ARRAYSIZE];
extern int		g_iMPMeasurementDateYear[MP_ARRAYSIZE];
//extern int		g_iMPMeasurementStat[MP_ARRAYSIZE];
extern float	g_fMPLogBurnup[MP_ARRAYSIZE];
extern float	g_fMPLogCoolingTime[MP_ARRAYSIZE];
extern float	g_fMPCoolingTime[MP_ARRAYSIZE];
extern float	g_fMPLogGammaDose[MP_ARRAYSIZE];
extern int		g_iMPStatus[MP_ARRAYSIZE];
extern float	g_fMPNChanA[MP_ARRAYSIZE];
extern float	g_fMPNChanB[MP_ARRAYSIZE];
extern float	g_fMPNChanC[MP_ARRAYSIZE];
extern float	g_fMPGDose1[MP_ARRAYSIZE];
extern float	g_fMPGDose2[MP_ARRAYSIZE];
extern int		g_iMPIndexMostRecentMeasurement;
extern float    g_fMPLogNoverG[MP_ARRAYSIZE];
extern float	g_fMPLogDecayCorTotNeutrons[MP_ARRAYSIZE];
#define	g_dMPMeasurementTime g_fMPNChanBThresh

extern char		g_szMPDetectorIDs[MP_ARRAYSIZE][MAX_NAME_LENGTH];

extern float	g_fDeltaEnrichment;
extern float	g_fDeltaCoolingTime;

extern float	g_fGrayDataX1[DB_ARRAYSIZE];
extern float	g_fGrayDataY1[DB_ARRAYSIZE];
extern float	g_fGrayDataX2[DB_ARRAYSIZE];
extern float	g_fGrayDataY2[DB_ARRAYSIZE];
extern float	g_fGrayDataX3[DB_ARRAYSIZE];
extern float	g_fGrayDataY3[DB_ARRAYSIZE];
extern float	g_fGrayDataX4[DB_ARRAYSIZE];
extern float	g_fGrayDataY4[DB_ARRAYSIZE];

extern float	g_fBlueDataX1[DB_ARRAYSIZE];
extern float	g_fBlueDataY1[DB_ARRAYSIZE];
extern float	g_fBlueDataX2[DB_ARRAYSIZE];
extern float	g_fBlueDataY2[DB_ARRAYSIZE];
extern float	g_fBlueDataX3[DB_ARRAYSIZE];
extern float	g_fBlueDataY3[DB_ARRAYSIZE];
extern float	g_fBlueDataX4[DB_ARRAYSIZE];
extern float	g_fBlueDataY4[DB_ARRAYSIZE];

extern float	g_fMPGoodDataX1[MP_ARRAYSIZE];
extern float	g_fMPGoodDataY1[MP_ARRAYSIZE];
extern float	g_fMPGoodDataX2[MP_ARRAYSIZE];
extern float	g_fMPGoodDataY2[MP_ARRAYSIZE];
extern float	g_fMPGoodDataX3[MP_ARRAYSIZE];
extern float	g_fMPGoodDataY3[MP_ARRAYSIZE];
extern float	g_fMPGoodDataX4[MP_ARRAYSIZE];
extern float	g_fMPGoodDataY4[MP_ARRAYSIZE];

extern float	g_fMPNeutralDataX1[MP_ARRAYSIZE];
extern float	g_fMPNeutralDataY1[MP_ARRAYSIZE];
extern float	g_fMPNeutralDataX2[MP_ARRAYSIZE];
extern float	g_fMPNeutralDataY2[MP_ARRAYSIZE];
extern float	g_fMPNeutralDataX3[MP_ARRAYSIZE];
extern float	g_fMPNeutralDataY3[MP_ARRAYSIZE];
extern float	g_fMPNeutralDataX4[MP_ARRAYSIZE];
extern float	g_fMPNeutralDataY4[MP_ARRAYSIZE];

extern float	g_fMPSuspectDataX1[MP_ARRAYSIZE];
extern float	g_fMPSuspectDataY1[MP_ARRAYSIZE];
extern float	g_fMPSuspectDataX2[MP_ARRAYSIZE];
extern float	g_fMPSuspectDataY2[MP_ARRAYSIZE];
extern float	g_fMPSuspectDataX3[MP_ARRAYSIZE];
extern float	g_fMPSuspectDataY3[MP_ARRAYSIZE];
extern float	g_fMPSuspectDataX4[MP_ARRAYSIZE];
extern float	g_fMPSuspectDataY4[MP_ARRAYSIZE];

extern float	g_fMostRecentDataX1;
extern float	g_fMostRecentDataY1;
extern float	g_fMostRecentDataX2;
extern float	g_fMostRecentDataY2;
extern float	g_fMostRecentDataX3;
extern float	g_fMostRecentDataY3;
extern float	g_fMostRecentDataX4;
extern float	g_fMostRecentDataY4;

extern int		g_iGrayCount1;
extern int		g_iBlueCount1;
extern int		g_iGrayCount2;
extern int		g_iBlueCount2;
extern int		g_iGrayCount3;
extern int		g_iBlueCount3;
extern int		g_iGrayCount4;
extern int		g_iBlueCount4;
extern int		g_iGoodCount;
extern int		g_iNeutralCount;
extern int		g_iSuspectCount;

extern BOOL		g_bFit1Possible;
extern BOOL		g_bFit2Possible;
extern BOOL		g_bFit3Possible;

extern float	g_fSlope1;
extern float	g_fIntercept1;
extern float	g_fStatY1;
extern float	g_fStatX1;
extern float	g_fMeanX1;

extern float	g_fSlope2;
extern float	g_fIntercept2;
extern float	g_fStatY2;
extern float	g_fStatX2;
extern float	g_fMeanX2;

extern float	g_fSlope3;
extern float	g_fIntercept3;
extern float	g_fStatY3;
extern float	g_fStatX3;
extern float	g_fMeanX3;

extern int		g_iFitPoints1;
extern int		g_iFitPoints2;
extern int		g_iFitPoints3;

extern float	g_fCVAverages[CV_ARRAYSIZE];
extern float	g_fCVOneSigma[CV_ARRAYSIZE];

extern float	g_fBkgMeasurements[MaxChannels];

typedef enum
{
	eUnmeasured	           , // 0 is non measurement measurement
	eMostRecent	           , // 1 is value for most recent measurement on input
	eMostRecentNoFits	           , // 2 is value for most recent measurement on exit if no fits
	eMostRecentGood	           , // 3 is value for most recent measurement on exit if good
	eMostRecentSuspect	           , // 4 is value for most recent measurement on exit if suspect
	eNoFits	           , // 5 is value for measurement on exit if no fits
	eGood	           , // 6 is value for measurement on exit if good
	eSuspect	           , // 7 is value for measurement on exit if suspect
	eAdded	          , // 8 is measurement is added to the DB
	eNine,
	eTen
} tMStatus;
typedef enum
{
	eMain,
	eNvBUCycle,
	eCycleFrame,
	eGBUvTCycle,
	eNvGFCycle,
	eULMenu,
	eMenuTypeCountMax
} tMenuType;

// bracket a numerical value with low and high values
#define CLAMP(v,l,h) ( (v < l) ? (l) : ((v > h) ? (h) : (v)))


class CFDMSApp : public CWinApp
{
public:
	CFDMSApp();


	void ReadDatabase();
	void WriteMeasurementPlan();
	void WriteDatabase();
	void UpdateGraphData();
	void UpdateAllDocs();
	void GetSavedBkgMeasurements();
	void UpdateMeasurements(int i);
	void UpdateMeasurementsBG(COleDateTime& cNow);
	void UpdateMeasurementsVBG(COleDateTime& cbtime, float bkg[5], PTCHAR pszDetcte);

	bool MakeNewConfigFile();

	bool RemoveGlobalMPEntry(int iIndex);
	void ClearMPEntry(int iIndex);

	// all of this menu management and type-casting stuff should go into it's own class set
	void AddUserInfoToMenus(CFrameWnd* pFrame, tMenuType index);
	void LoadCustomMenuValues(CMenu* pEstateMenu, int zid, int index);
	void FindMenuItem(CMenu* pTargMenu, LPCTSTR MenuString, int& pos, CMenu*& pMenu);

	void UnitizeCustomMenuEntry(CString& entry, UINT nID);
	void CheckCustomMenuItems(CFrameWnd* pFWin, bool bChild);

	void CreateToolMenu(CFrameWnd* pFrame);
	CMenu* GetPopUpForPref(CWnd* pFWin, UINT nID);
	void SetTemporaryIniValue(CMenu* pMenu, UINT nID, UINT nScopeID);
	tIniNames GetRelatedIniVal(UINT nID);
	void CheckMenuItem(CMenu * pMenu, UINT nID);
	void GetMenuItemValue(CString& menustr, UINT nScopeID);
	void InitializeConfirmationValues(CMenu* pMenu);
	void RefreshRelatedPlotz(UINT nScopeID);

	BOOL DoFinalInitialization(const bool  bSplash);

	void SetMeasurementDialogPtr(CDialog* pDlg) { m_pMeasurementDialog = pDlg; }
	CDialog* GetMeasurementDialogPtr() { return m_pMeasurementDialog; }


	CMultiDocTemplate* m_pDocTemplate1;
	CMultiDocTemplate* m_pDocTemplate2;
	CMultiDocTemplate* m_pDocTemplate3;
	CMultiDocTemplate* m_pDocTemplate4;

	int MyMessageBox(LPCTSTR lpszPrompt, UINT nType, UINT nIDPrompt, LPCTSTR lpszTitle);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFDMSApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFDMSApp)
	afx_msg void OnAppAbout();
	afx_msg void OnEditIni();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
	afx_msg void OnEditResetacknowledgements();
	afx_msg void OnEditConfigDUMP_O_RAMA_Current();
	afx_msg void OnEditConfigDUMP_O_RAMA_Default();
	afx_msg void OnEditPrefBGC(UINT nID);
	afx_msg void OnEditPrefBGCT(UINT nID);
	afx_msg void OnEditPrefDE(UINT nID);
	afx_msg void OnEditPrefDCT(UINT nID);
	afx_msg void OnEditPrefSMC(UINT nID);
	afx_msg void OnEditPrefSMCT(UINT nID);
	afx_msg void OnEditPrefDID(UINT nID);
	afx_msg void OnViewLog();
//	virtual BOOL OnIdle(LONG lCount);


public:
	afx_msg void OnToolGoose(UINT nID);
	afx_msg void OnImportButton();

protected:
	void GetDeltaEnrichmentValue(float& fv);
	void GetDeltaCoolingTimeValue(float& fv);
	void SetRelatedGlobalValue(tIniNames nID);

	void InitF77Globals();
	BOOL InitIniFileandRelatedValues();
	BOOL ObtainRuntimeAssets();

	void ULStartup(bool& bNoUpperLayer, bool& bUpperLayerError, CDialog& rStartUpDlg);

public:
	CStringArray m_MLog;
	LoggingLog* m_pLogLog;
	bool bSL;

	void AppendToLog(LPCTSTR s);
	void AppendToLogF( char * format, ...);
	void SaveFullConfig(bool bUseDefault);
	void 	NotifyChildChangedPrefs();
	void 	NotifyMainChangedPrefs();
	bool    bNeedsMenuUpdate() const;
protected:
	bool 	m_bPrefChangedChild;
	bool 	m_bPrefChangedMain;
	bool	m_bMenusConstructed[eMenuTypeCountMax];

	CDialog* m_pMeasurementDialog;

	
	//DWORD m_dwSplashTime;
	//CSplashWnd m_splash;

public:

	void SetAllMessagesOnHold(bool ProcessMsgs);

	UINT m_ProcessUpperLayerMsgQueueMsg;

	char m_EXEPath[_MAX_PATH];
	RTTitle* m_pRT_Title;
	Review_Tool* m_pReview_Tool;
	FDMSReview* m_pRT_Review;
	RTInit* m_pRT_Init;
	FDMSSetup* m_pRT_Setup;
	FDMSImpReimp* m_pRT_ImpReimp;

	bool 	m_bDemoVersion;

	afx_msg void OnAppExit();

	static PTCHAR GetFileHeader(CSVFileHeaderIDs id, USHORT length = 0);
private:
	static PTCHAR m_fileheaderstrings[efhHeaderCount];

};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FDMS_H__04E4FC10_684F_48AC_AB68_7EEBDA538D80__INCLUDED_)
