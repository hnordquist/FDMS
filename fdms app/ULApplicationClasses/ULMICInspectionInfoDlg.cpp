// ULMICInspectionInfoDlg.cpp : implementation file
// $History: ULMICInspectionInfoDlg.cpp $ 
// 
// *****************  Version 5  *****************
// User: 186846       Date: 12/18/07   Time: 4:34p
// Updated in $/FDMS/FDMS Application/FDMSNET/ULApplicationClasses
// VS 2005 changes completed
// 
// *****************  Version 4  *****************
// User: Longo        Date: 11/24/04   Time: 3:45p
// Updated in $/FDMS/FDMS Application/FDMSNET/ULApplicationClasses
// compiler warning removal
// 
// *****************  Version 3  *****************
// User: Longo        Date: 9/17/04    Time: 3:35p
// Updated in $/FDMS/FDMS Application/FDMSNET/ULApplicationClasses
// First integration of the MFC Upper Layer code
//

#include "stdafx.h"
#include "..\FDMS.h" //REQT:  Change this to use the new application header.
#include "ULMICInspectionInfoDlg.h"
#include "ULSupport.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ULMICInspectionInfoDlg dialog


ULMICInspectionInfoDlg::ULMICInspectionInfoDlg(
	CString InspecINIFileName, CString DlgWndTitle, RTInit* pCommonParams, 
	int IconResID /* = 0*/,   CWnd* pParent /*=NULL*/)
	: CDialog(ULMICInspectionInfoDlg::IDD, pParent), 
	m_FailedInit(false)
{
	//{{AFX_DATA_INIT(ULMICInspectionInfoDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_InspecINIFile = InspecINIFileName;
	m_Title = DlgWndTitle;
	m_IconID = IconResID;
	m_pCommonParams = pCommonParams;

	// Create the memory for the MIC parameters first 
	//If of the memory allocations fail, then delete each successfully
	//allocated chunk of memory and return an indication of an error.
	if ((NULL == (pszMIC_Facility = new (char [100]))) ||
		(NULL == (pszMIC_Previous = new (char [100]))) ||
		(NULL == (pszMIC_Date_Time = new (char [100]))) ||
		(NULL == (pszMIC_Inspector_Name = new (char [100]))) ||
		(NULL == (pszMIC_Inspector_ID = new (char [100]))) ||
		(NULL == (pszMIC_Inspection_Number = new (char [100]))) //||
		//(NULL == (pszMIC_Full_Filename = new (char [strlen(pCommonParams->Get_MIC_Inspec_Info_Filename()) + 1])))) 
		)
	{
		if (pszMIC_Facility) delete [] pszMIC_Facility;
		if (pszMIC_Previous) delete [] pszMIC_Previous;
		if (pszMIC_Date_Time) delete [] pszMIC_Date_Time;
		if (pszMIC_Inspector_Name) delete [] pszMIC_Inspector_Name;
		if (pszMIC_Inspector_ID) delete [] pszMIC_Inspector_ID;
		if (pszMIC_Inspection_Number) delete [] pszMIC_Inspection_Number;

		//Report error to user with instructions as to how to recover.
		MessageBox("Out of memory.  Could not create the MIC inspection information dialog.\n\nTerminate the application and free resources.  Then try restart application and try again",
			"Error Message", MB_OK|MB_ICONERROR);
		m_FailedInit = true;
	}

	if(!m_FailedInit)
	{
		//Read all of the parameters from the INI file.  If any of them not found,
		//then indicate failure of initialization.

		// Get each one of the MIC parameters from an INI file.
		// If found, set as new inspection parameters
		// If not found (NULL returned because copied default empty parameter) then do nothing
		if ((NULL != GetPrivateProfileString("GENERAL", "FACILITY", "", pszMIC_Facility, 100, (LPCTSTR)InspecINIFileName)) &&
			(NULL != GetPrivateProfileString("GENERAL", "PREVIOUS", "", pszMIC_Previous, 100, (LPCTSTR)InspecINIFileName)) &&
			(NULL != GetPrivateProfileString("GENERAL", "DATE_TIME", "", pszMIC_Date_Time, 100, (LPCTSTR)InspecINIFileName)) &&
			(NULL != GetPrivateProfileString("GENERAL", "INSPECTOR_NAME", "", pszMIC_Inspector_Name, 100, (LPCTSTR)InspecINIFileName)) &&
			(NULL != GetPrivateProfileString("GENERAL", "INSPECTOR_ID", "", pszMIC_Inspector_ID, 100, (LPCTSTR)InspecINIFileName)) &&
			(NULL != GetPrivateProfileString("GENERAL", "INSPECTION_NUMBER", "", pszMIC_Inspection_Number, 100, (LPCTSTR)InspecINIFileName)))
			m_FailedInit = true;

	}

}


void ULMICInspectionInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ULMICInspectionInfoDlg)
	DDX_Control(pDX, IDC_RT_MIC_INSPEC_INFO_LIST, m_MICInspecListCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ULMICInspectionInfoDlg, CDialog)
	//{{AFX_MSG_MAP(ULMICInspectionInfoDlg)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ULMICInspectionInfoDlg message handlers

BOOL ULMICInspectionInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//Set the icon for the dialog if the caller submitted an ID to be used for it.
	if(m_IconID != 0)
		SetIcon(AfxGetApp()->LoadIcon(m_IconID), false);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

int ULMICInspectionInfoDlg::DoModal() 
{

	//If initialization was successful, then load the contents of the 
	//inspection dialog box.
	if(!m_FailedInit)
	{
		time_t Time;
		UL_GEN_DATE_STRUCT Temp_Date_Struct;
		UL_GEN_TIME_STRUCT Temp_Time_Struct;
		BOOL bInspector_ID_Already_Added;
		struct tm *Time_Struct;
		UINT ctInspector_ID, ctInspector_Choice;
		CString New_Inspector_Name_ID;
		//char **ppszNew_Inspector_IDs = NULL;
		UINT puiTemp_Inspector_ID_Choices[1] = {0};
		char *ppszAsci_Month[12] = {"January","February","March", "April","May","June", "July","August","September", "October","November","December"};
		char *pszCurrent_Asci_Month;








		//Load the dialog contents based on inputs to the constructor.

		///////////////////////////////////////////////////////////////////////////////////
		// Set the current inspection date and time
		///////////////////////////////////////////////////////////////////////////////////
		time(&Time);
		Time_Struct = localtime(&Time);
		Temp_Date_Struct.yr = Time_Struct->tm_year; // Y2K compliant, will take years above 99
		Temp_Date_Struct.mon = Time_Struct->tm_mon + 1;
		Temp_Date_Struct.day = Time_Struct->tm_mday;
		Temp_Time_Struct.hr = Time_Struct->tm_hour;
		Temp_Time_Struct.min = Time_Struct->tm_min;
		Temp_Time_Struct.sec = Time_Struct->tm_sec;
		Temp_Time_Struct.msec = 0;
		m_pCommonParams->Set_Inspection_Date(&Temp_Date_Struct);
		m_pCommonParams->Set_Inspection_Time(&Temp_Time_Struct);


		///////////////////////////////////////////////////////////////////////////////////
		// Set the inspection number
		///////////////////////////////////////////////////////////////////////////////////
		m_pCommonParams->Set_Inspection_Number(pszMIC_Inspection_Number);


		///////////////////////////////////////////////////////////////////////////////////
		// Set the inspector names and selected
		///////////////////////////////////////////////////////////////////////////////////
		// Determine if the inspectors name has already been added
		bInspector_ID_Already_Added = FALSE;
		ctInspector_ID = 0;
		while ((!bInspector_ID_Already_Added) && (ctInspector_ID < m_pCommonParams->Get_Inspector_ID_Num())) 
		{
			if (NULL == _strnicmp(pszMIC_Inspector_Name, m_pCommonParams->Get_Inspector_ID(ctInspector_ID), strlen(pszMIC_Inspector_Name)))
				bInspector_ID_Already_Added = TRUE;
			ctInspector_ID++;
		}

		// If not already there, add the MIC entered inspector ID and clear all choices but this one
		if (!bInspector_ID_Already_Added) 
		{
			// Use the temporary list box to perform the sort for the newly added inspector ID
			for (ctInspector_ID = 0; ctInspector_ID < m_pCommonParams->Get_Inspector_ID_Num(); ctInspector_ID++)
				//kdm					pMIC_Load_Inspec_Info->PL_MultiSelAdd(IDC_RT_MIC_INSPEC_INFO_LIST, pCommon_Params->Get_Inspector_ID(ctInspector_ID));
				m_MICInspecListCtrl.AddString(m_pCommonParams->Get_Inspector_ID(ctInspector_ID));

			New_Inspector_Name_ID = pszMIC_Inspector_Name;
			New_Inspector_Name_ID += " - ";
			New_Inspector_Name_ID += pszMIC_Inspector_ID;

			//kdm			pMIC_Load_Inspec_Info->PL_MultiSelAdd(IDC_RT_MIC_INSPEC_INFO_LIST, (LPCSTR)New_Inspector_Name_ID);
			m_MICInspecListCtrl.AddString(New_Inspector_Name_ID);

			// Get the new sorted list - do not handle if it does not create
			/*  add back in later : removed until function of the dialog box is determined.
			if (ppszNew_Inspector_IDs = new (char (* ([m_pCommonParams->Get_Inspector_ID_Num() + 1])))) 
			{ // + 1 for new ID
			for (ctInspector_ID = 0; ctInspector_ID < m_pCommonParams->Get_Inspector_ID_Num() + 1; ctInspector_ID++) 
			{
			pMIC_Load_Inspec_Info->PL_MultiSelGetValue(IDC_RT_MIC_INSPEC_INFO_LIST, ctInspector_ID, &bInspector_ID_Already_Added, &New_Inspector_Name_ID);
			if ((ppszNew_Inspector_IDs[ctInspector_ID] = new char [New_Inspector_Name_ID.GetLength() + 1]) != NULL)
			strcpy(ppszNew_Inspector_IDs[ctInspector_ID], (LPCTSTR)New_Inspector_Name_ID);
			}

			// Set the new inspection IDs
			m_pCommonParams->Set_Inspector_IDs((const char**)ppszNew_Inspector_IDs, m_pCommonParams->Get_Inspector_ID_Num() + 1); // + 1 for new ID

			// Delete the temporary inspection IDs
			SEB_DELETELISTSTRING(ppszNew_Inspector_IDs, pCommon_Params->Get_Inspector_ID_Num(), ctInspector_ID); // Get_Inspector_ID_Num now set to correct number
			}
			*/
		}

		// Clear all choices but the one which was entered through MIC
		bInspector_ID_Already_Added = FALSE;
		ctInspector_Choice = 0;
		while ((!bInspector_ID_Already_Added) && (ctInspector_Choice < m_pCommonParams->Get_Inspector_ID_Num())) 
		{
			if (NULL == _strnicmp(pszMIC_Inspector_Name, m_pCommonParams->Get_Inspector_ID(ctInspector_Choice), strlen(pszMIC_Inspector_Name)))
				bInspector_ID_Already_Added = TRUE;
			else
				ctInspector_Choice++;;
		}

		puiTemp_Inspector_ID_Choices[0] = ctInspector_Choice;
		m_pCommonParams->Set_Inspector_ID_Choices(puiTemp_Inspector_ID_Choices, 1);


		///////////////////////////////////////////////////////////////////////////////////
		// Set the review end date and time for the data range
		///////////////////////////////////////////////////////////////////////////////////
		strtok(pszMIC_Date_Time, ", :"); // Get past day of week
		Temp_Date_Struct.day = atoi(strtok(NULL, ", :")); // Get integer day of week

		// Get integer month
		pszCurrent_Asci_Month = strtok(NULL, ", :");
		for (Temp_Date_Struct.mon = 1; ((_stricmp(pszCurrent_Asci_Month, ppszAsci_Month[Temp_Date_Struct.mon - 1]) != 0) && (Temp_Date_Struct.mon <= 12)); Temp_Date_Struct.mon++);
		Temp_Date_Struct.yr = atoi(strtok(NULL, ", :")); // Get integer year
		Temp_Time_Struct.hr = atoi(strtok(NULL, ", :")); // Get integer hour
		Temp_Time_Struct.min = atoi(strtok(NULL, ", :")); // Get integer minute
		Temp_Time_Struct.sec = atoi(strtok(NULL, ", :")); // Get integer second
		m_pCommonParams->Set_End_Date(&Temp_Date_Struct);
		m_pCommonParams->Set_End_Time(&Temp_Time_Struct);

		///////////////////////////////////////////////////////////////////////////////////
		// Set the review start date and time for the data range
		///////////////////////////////////////////////////////////////////////////////////
		strtok(pszMIC_Previous, ", :"); // Get past day of week
		Temp_Date_Struct.day = atoi(strtok(NULL, ", :")); // Get integer day of week

		// Get integer month
		pszCurrent_Asci_Month = strtok(NULL, ", :");
		for (Temp_Date_Struct.mon = 1; ((_stricmp(pszCurrent_Asci_Month, ppszAsci_Month[Temp_Date_Struct.mon - 1]) != 0) && (Temp_Date_Struct.mon <= 12)); Temp_Date_Struct.mon++);
		Temp_Date_Struct.yr = atoi(strtok(NULL, ", :")); // Get integer year
		Temp_Time_Struct.hr = atoi(strtok(NULL, ", :")); // Get integer hour
		Temp_Time_Struct.min = atoi(strtok(NULL, ", :")); // Get integer minute
		Temp_Time_Struct.sec = atoi(strtok(NULL, ", :")); // Get integer second
		m_pCommonParams->Set_Start_Date(&Temp_Date_Struct);
		m_pCommonParams->Set_Start_Time(&Temp_Time_Struct);

		///////////////////////////////////////////////////////////////////////////////////
		// Set the comments to blank	
		///////////////////////////////////////////////////////////////////////////////////
		m_pCommonParams->Set_Inspection_Comments("");

	}//end IF initialization was successful.
	else  //initialization failed.  Terminate the dialog.
		return IDCANCEL;

	return CDialog::DoModal();
}

void ULMICInspectionInfoDlg::OnDestroy() 
{
	CDialog::OnDestroy();

	//Free the memory that was allocated for the inspection information 
	//read from the INI file.
	if (pszMIC_Facility) {
		delete [] pszMIC_Facility;
		pszMIC_Facility = NULL;
	}
	if (pszMIC_Previous) {
		delete [] pszMIC_Previous;
		pszMIC_Previous = NULL;
	}
	if (pszMIC_Date_Time) {
		delete [] pszMIC_Date_Time;
		pszMIC_Date_Time = NULL;
	}
	if (pszMIC_Inspector_Name) {
		delete [] pszMIC_Inspector_Name;
		pszMIC_Inspector_Name = NULL;
	}
	if (pszMIC_Inspector_ID) {
		delete [] pszMIC_Inspector_ID;
		pszMIC_Inspector_ID = NULL;
	}
	if (pszMIC_Inspection_Number) {
		delete [] pszMIC_Inspection_Number;
		pszMIC_Inspection_Number = NULL;
	}

}
