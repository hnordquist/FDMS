//////////////////////////////////////////////////////////////////////////
// Copyright 2004, The Regents Of the University of California.  This
// software was produced under U.S. Government contract (W-7405-ENG-36) at
// the Los Alamos National Laboratory by the Safeguards Science and
// Technology Group (N-1).  All Rights Reserved.
//////////////////////////////////////////////////////////////////////////
// $History: IniFileProcessor.h $
// 
// *****************  Version 14  *****************
// User: 186846       Date: 2/24/10    Time: 5:02p
// Updated in $/FDMS/FDMSNET
// 
// *****************  Version 13  *****************
// User: 186846       Date: 1/28/08    Time: 4:07p
// Updated in $/FDMS/FDMS Application/FDMSNET
// wtf
// 
// *****************  Version 12  *****************
// User: Longo        Date: 3/11/05    Time: 10:35a
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 11  *****************
// User: Longo        Date: 1/10/05    Time: 12:47p
// Updated in $/FDMS/FDMS Application/FDMSNET
// matching on date time AND facility, detector and item id
// 
// *****************  Version 10  *****************
// User: Longo        Date: 12/21/04   Time: 4:14p
// Updated in $/FDMS/FDMS Application/FDMSNET
// added a few things
// 
// *****************  Version 9  *****************
// User: Longo        Date: 12/10/04   Time: 6:25p
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 8  *****************
// User: Longo        Date: 12/10/04   Time: 4:44p
// Updated in $/FDMS/FDMS Application/FDMSNET
// Combining the GrandCom demo class wirth the SerialCom live detector
// control class
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "mlistctl.h"

/////////////////////////////////////////////////////////////////////////////
// CIniFileProcessor dialog

typedef struct 
{
	short id;
	PTCHAR name;
	bool bIsString, bChanged;
	PTCHAR iniValue;
	PTCHAR defIniValue;
	PTCHAR section;
	CString szDesc;

	void IniFileEntry()
	{
		id = 0;
		name = NULL;
		bIsString = false;
		bChanged = false;
		iniValue = NULL;
		defIniValue = NULL;
		section = NULL;
	}

} IniFileEntry;

	typedef enum
	{
		eBGCYCLETIME,			
		eBGCYCLES	,		
		eOFFSET_CNTRL,		
		eOFFSET_CNTRL_REMOTE	,
		eOFFSET_LIMIT_LOW	,
		eOFFSET_LIMIT_HIGH	,
		eOFFSET_TIME			,
		eGAMMAGATE1			,
		eGAMMAGATE2			,
		eBAUD				,
		eDATABITS			,
		eSTOPBITS			,
		eTICKLE				,
		ePORT				,
		ePARITY				,
		eCYCLETIME			,

		eMINSAMPLES			,
		eMAXSAMPLES			,

		eCOMMANDTIMEOUT		,
		eICHVBIAS			,
		eHVBIAS				,
		eCYCLES				,
		eMINIGRAND		,	

		eICBACKGROUND1,
		eICBACKGROUND2,

		eSCALE				,

		eMEASUREMENTS,	
		eDATABASE	,	
		eMEASUREMENTPLAN,		
		eDELTA_ENRICHMENT,	
		eDELTA_COOLINGTIME,	
		ePASSIVE_SYM		,
		eACTIVITYLOG		,	
		eCOMMLOG			,	
		eLOGLOC			,

		eIRCONFIGLOC,		// location of config folder
		eULINILOC,			// location of ULAYR.ini
		eULAPPSETTINGSLOC,	// location of ApplicationSettings.ini
		eIRINILOC,			// location of irs.ini
		eIRDATALOC,			// location of data folder

		eIRCONFIGFILENAME,	// notused
		eULININAME,			// defaults to ULAYR.ini
		eULAPPSETTINGSNAME,	// defaults to ApplicationSettings.ini
		eIRININAME,			// defaults to irs.ini
		eIRDATAFILENAME,	// defaults to radfdms.xml

		eMATCHDAYS,			// matching measurement error # of days, subsumes need for years, too
		eMATCHMINUTES,		// matching measurement error # of days, subsumes need for hours
		eMATCHSECONDS,		// matching measurement error # of seconds

		eFACILITYID,

		eDEMOVERSION,

		eBACKGROUND_A,
		eBACKGROUND_B,
		eBACKGROUND_C,
		eBACKGROUND_1,
		eBACKGROUND_2,
		eOFFSET_INTERNAL_CH0,
		eOFFSET_INTERNAL_CH1,
		eOFFSET_REMOTE_CH0,
		eOFFSET_REMOTE_CH1,

		eDETECTOR_ID,

		eBGCYCLES_CHOICE,
		eBGCYCLETIME_CHOICE,
		eCYCLES_CHOICE,
		eCYCLETIME_CHOICE,
		eDELTA_ENRICHMENT_CHOICE,
		eDELTA_COOLINGTIME_CHOICE,
		eDETECTOR_ID_CHOICE,

		eBGCYCLES_CHOICE_CONF,
		eBGCYCLETIME_CHOICE_CONF,
		eCYCLES_CHOICE_CONF,
		eCYCLETIME_CHOICE_CONF,
		eDELTA_ENRICHMENT_CHOICE_CONF,
		eDELTA_COOLINGTIME_CHOICE_CONF,
		eDETECTOR_ID_CHOICE_CONF,

		eItem_RANGE,
		eFacility_RANGE,
		eID_RANGE,

		eMeasType_RANGE,
		eEnrichment_RANGE,
		eBurnUp_RANGE,
		eDischDate_RANGE,
		eCycle_RANGE,
		eThresholdA_RANGE,
		eThresholdB_RANGE,
		eMeasDate_RANGE,
		eCoolingTime_RANGE,
		eNeutronA_RANGE,
		eNeutronB_RANGE,
		eNeutronC_RANGE,
		eGamma1_RANGE,
		eGamma2_RANGE,
		eDetector_RANGE,

		ININAMECOUNT
	} tIniNames;



class CIniFileProcessor : public CDialog
{
// Construction
public:
	CIniFileProcessor(CWnd* pParent = NULL);   // standard constructor

	~CIniFileProcessor();

	// only used if dialog based editor is invoked.
	int m_iSelected;
	bool m_bETStat;
	void UpdateRow(bool bNew, int iItem, int iData);
	//--------------------

	static void FreeValues();
	static void SetDefaultSource();
	
	static CString DefaultSourcePath;
	static CString DefaultSourceFullPath;

	static int GetIntValue(tIniNames i);
	static int GetIntArrayValues(tIniNames i, int* pArray, int& iArrayLen);
	static int GetStringValue(tIniNames i, PTCHAR p, int len);
	static int GetStringArrayValues(tIniNames i, CStringArray& Array, int& iArrayLen);
	static int GetBooleanValue(tIniNames i, bool& bValue);
	static int GetRangeValue(tIniNames i, CString& szLow, CString& szHigh);

	static bool SaveFullConfigFile(CString szFileName, bool bUseDefaults);
	static bool SaveSingleValue(tIniNames i, LPCSTR lpString);

	//static CString defValueProcessor(int iData);
	static void LoadSavedValues(int iData);

	static bool Confirm(tIniNames i);

	static void CacheIniFileValues();
	static bool HasUnsavedChanges();
	static bool MakeModificationsPermanent();
	static void Terminate();

	static bool MakeNewConfigFile();

	static bool UpdateValue(int iEntry, int iColumnID, LPCSTR p);

	static void PrepareULInfo();
	static bool GetULPresent() {return m_bULPresent;}
	static bool CreateIRSSchemeAssets(PTCHAR pMsg, PTCHAR facility, PTCHAR config, PTCHAR data);
	static bool GetULFullPath(tIniNames i, CString& szFP);
	static bool GetIRSFacilityID(short& iVal); // unique function to retrieve the id from a unique ini file only for use with IRS
	static bool GetString(tIniNames i, CString& szV);

	static bool ExtractFlags(LPTSTR m_lpCmdLine);


// Dialog Data
	//{{AFX_DATA(CIniFileProcessor)
	enum { IDD = IDD_DIALOG_INIEDIT };
	CButton	m_cOK;
	CButton	m_cCancel;
	CButton	m_cEdit;
	CMyListCtrl	m_cListCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIniFileProcessor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	static const char* m_szFieldNames[];
	static IniFileEntry m_Entry[ININAMECOUNT];
	static bool m_bULPresent;


	// Generated message map functions
	//{{AFX_MSG(CIniFileProcessor)
	virtual BOOL OnInitDialog();
	afx_msg void OnColumnclickList1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnEditToggle();
	afx_msg LRESULT OnSubEdit(WPARAM w, LPARAM pResult);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CRect m_OldRect;
	BOOL m_bInitialized;
	//CScrollBar m_Grip;
public:

};
