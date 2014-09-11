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
// Steven Edward Buck
// Los Alamos National Laboratory
// NIS-5
// 3/12/97

// RTTitle.CPP
// Setup class definition for the abstract class Review Tool
       
#include <stdafx.h>
#include <afxwin.h>
     
#include "RTTitle.H"
#include "ULSupport.h"
#include "ULMICInspectionInfoDlg.h"
#include "ULSplashDlg.h"
#include "ULTitleDlg.h"
	

/////////////////////////////////////////////////////////////////////////////////////
//  RTTitle constructor, destructor
/////////////////////////////////////////////////////////////////////////////////////

RTTitle::RTTitle(RESOURCE_ID Icon, const char *pszName_Version, const char *pszProgrammer_Email, BOOL bCreate_Title_As_App) :
	bCommon_Params_Set(FALSE),
	pCommon_Params(NULL),
	bCommon_Pipes_Set(FALSE),
	pCommon_Pipes(NULL),
//KDM	pSplash(NULL),
//kdm	pTitle_DlgApp(NULL),
//kdm	pTitle_Dlg(NULL),
//kdm	pCommon_Title(NULL),
	uiIcon(Icon),
	pszSplash_Title1(NULL),
	pszSplash_Title2(NULL),
	eStart_Operation(START_SETUP),
	bInspection_Info_Open(FALSE),
	bCreate_As_App(bCreate_Title_As_App),
	m_pSplashDlg(NULL)
{
		
	if (pszName_Version) {
		if ((pszSplash_Title1 = new char [strlen(pszName_Version) + 1]) != NULL)
			strcpy(pszSplash_Title1, pszName_Version);
		}
	if (pszProgrammer_Email) {
		if ((pszSplash_Title2 = new char [strlen(pszProgrammer_Email) + 1]) != NULL)
			strcpy(pszSplash_Title2, pszProgrammer_Email);
		}
		
}

 
RTTitle::~RTTitle(void) 
{

	if(m_pSplashDlg) delete m_pSplashDlg;
	if(pszSplash_Title1) delete [] pszSplash_Title1;
	if(pszSplash_Title2) delete [] pszSplash_Title2;

//kdm	RT_Title_Delete();
//KDM	RT_Splash_Delete();
		
}


/////////////////////////////////////////////////////////////////////
// Access to the common data items across all review tool operations
/////////////////////////////////////////////////////////////////////

BOOL RTTitle::Set_Common_Params(RTInit *const pRT_Common_Params) {
    
    pCommon_Params = pRT_Common_Params;
    bCommon_Params_Set = TRUE;
    
    // Also set the common error's
    RTError.Set_My_Name(pCommon_Params->Get_My_Name());
    
	return bCommon_Params_Set;
	
}


BOOL RTTitle::Set_Common_Pipes(RTPipes *const pRT_Commmon_Pipes) {

    pCommon_Pipes = pRT_Commmon_Pipes;
    bCommon_Pipes_Set = TRUE;
    
    // Also set the common error's
    RTError.Set_My_Pipes(pCommon_Pipes);

	return bCommon_Pipes_Set;

}


/////////////////////////////////////////////////////////////////////////////////////////////
// Displays the splash screen immediately
/////////////////////////////////////////////////////////////////////////////////////////////
BOOL RTTitle::RT_Title_Splash_Display(UINT uiSplash_Screen_Timeout_Seconds) {

/*	if (pSplash) {
		return TRUE;
		}
*/

	if (uiSplash_Screen_Timeout_Seconds != 0) 
	{
	/*kdm
	if ((pSplash = new CGUI_Dlg(IDD_RT_SPLASH, NULL, DLG_POSN_NO_SAVE)) == NULL) 
		{
			RT_Title_Error(uiRT_TITLE_ERROR_NO_MEMORY);
			return FALSE;
			}
		else {
			if (uiIcon) 
				pSplash->SetIcon(uiIcon);

			if (pszSplash_Title1)
				pSplash->DefineFldStaticTxt(IDC_RT_SPLASH_NAME_VERSION, pszSplash_Title1);
			if (pszSplash_Title2)
				pSplash->DefineFldStaticTxt(IDC_RT_SPLASH_PROGRAMMER_EMAIL, pszSplash_Title2);

			pSplash->SetTimeOut(uiSplash_Screen_Timeout_Seconds);

			pSplash->CGUI_Dlg::GoModeless(TRUE);
			*/
			m_pSplashDlg = new ULSplashDlg(pszSplash_Title1,pszSplash_Title2, uiSplash_Screen_Timeout_Seconds, uiIcon);
			m_pSplashDlg->Create(IDD_RT_SPLASH);
			m_pSplashDlg->ShowWindow(SW_SHOW | SW_SHOWNORMAL);
//kdm			}
		}


	return TRUE;

}


/*BOOL RTTitle::RT_Title_Splash_Close(void) {

	FlushEvents();
	CGUI_App::ProcessEvents();


	if (pSplash == NULL) return FALSE;

	// GUI.LIB fix: Use these two WIN32 to check to see if splash got closed because the app got closed
	//              Must do this way because exit validates and control validates do not get called
	if ((IsWindow(pSplash->GetWndHandle())) && (IsWindowVisible(pSplash->GetWndHandle()))) {
		pSplash->Close();
		FlushEvents();
		CGUI_App::ProcessEvents();
		return TRUE;
		}
	else return FALSE;

}
*/

/////////////////////////////////////////////////////////////////////////////////////////////
// Creates and displays default\common review tool title screen
// NOTE: This class is used (this member functions called) only when this tool is
//       run individually or is the first tool to be run within a team of tools
// Return value is the next operation to be taken
// If you would like to display a different title screen or 
/////////////////////////////////////////////////////////////////////////////////////////////
RTTitle::START_OPERATION RTTitle::RT_Title_Begin(void) {

	eStart_Operation = START_EXIT;


	if (RT_Title_Create()) 
	{

	 	RT_Title_Start_Screen_ON();

		// All pCommon_Title calls from now on MUST only be for CGUI_Dlg, if unsure, cast them.
//kdm		if (bCreate_As_App)
//kdm	 		pTitle_DlgApp->Go();
//kdm		else pTitle_Dlg->Go();

		// Don't let splash screen stay open beyond title screen - NOTE: Must be before deleting the app or dialog app
//KDM		RT_Title_Splash_Close();

//kdm		RT_Title_Delete();

//kdm		CGUI_App::ProcessEvents();
	 	}
    
    
	return (eStart_Operation);
	
}


/////////////////////////////////////////////////////////////////////////////////////////////
// The title dialog box creation and delation member functions
/////////////////////////////////////////////////////////////////////////////////////////////

BOOL RTTitle::RT_Title_Create_App(const char * /*pszTool_Name*/)
 {

/*	if (bCreate_As_App) {
		// Return if already created
		if (pTitle_DlgApp) return TRUE;

		// Create the title dialog box
		if (pszTool_Name) {
			if ((pTitle_DlgApp = new CGUI_DlgApp(pszTool_Name, IDD_RT_TITLE, NULL, DLG_POSN_NO_SAVE)) == NULL) {
				RT_Title_Error(uiRT_TITLE_ERROR_NO_MEMORY);
				return FALSE;
				}
			pTitle_DlgApp->SetTitle(pszTool_Name);
			}
		else {
			if ((pTitle_DlgApp = new CGUI_DlgApp("", IDD_RT_TITLE, NULL, DLG_POSN_NO_SAVE)) == NULL) {
				RT_Title_Error(uiRT_TITLE_ERROR_NO_MEMORY);
				return FALSE;
				}
			pTitle_DlgApp->SetTitle("");
			}

		if (uiIcon) pTitle_DlgApp->SetIcon(uiIcon);
		
		// Keep a common title pointer for simplicity later
		pCommon_Title = pTitle_DlgApp;
		}

	else {
		// Return if already created
		if (pTitle_Dlg) return TRUE;

		// Create the title dialog box
		if (pszTool_Name) {
			if ((pTitle_Dlg = new CGUI_Dlg(IDD_RT_TITLE, NULL, DLG_POSN_NO_SAVE)) == NULL) {
				RT_Title_Error(uiRT_TITLE_ERROR_NO_MEMORY);
				return FALSE;
				}
			pTitle_Dlg->SetTitle(pszTool_Name);
			}
		else {
			if ((pTitle_Dlg = new CGUI_Dlg(IDD_RT_TITLE, NULL, DLG_POSN_NO_SAVE)) == NULL) {
				RT_Title_Error(uiRT_TITLE_ERROR_NO_MEMORY);
				return FALSE;
				}
			}

		if (uiIcon) pTitle_Dlg->SetIcon(uiIcon);

		// Keep a common title pointer for simplicity later
		pCommon_Title = (CGUI_DlgApp *)pTitle_Dlg;
		}

	CGUI_App::ProcessEvents();
	FlushEvents();
*/


	return TRUE;
}



BOOL RTTitle::RT_Title_Create(void) {

	// Be sure it is already created
	if (!RT_Title_Create_App(pCommon_Params->Get_My_Name()))
		return FALSE;


	// GUI.LIB fix, will GPF if not exactly set for app or dlgapp
/*	if (bCreate_As_App) {
		// Don't set the icon here again, GUI.LIB GPF
//kdm		pTitle_DlgApp->SetTitle(pCommon_Params->Get_My_Name());
		// Setup the elements common to all title box screens
/*		pTitle_DlgApp->DefinePushBtn(IDC_RT_TITLE_CHANGE, GUI_NONE);
		pTitle_DlgApp->SetCtrlNotifyFn(IDC_RT_TITLE_CHANGE, (CGUI_CallBack*)((void *)this), (GUI_CB_DlgNotifyFn)&RTTitle::RT_Title_Change);
		pTitle_DlgApp->DefinePushBtn(IDC_RT_TITLE_CONTINUE, GUI_OK);
		pTitle_DlgApp->SetCtrlNotifyFn(IDC_RT_TITLE_CONTINUE, (CGUI_CallBack*)((void *)this), (GUI_CB_DlgNotifyFn)&RTTitle::RT_Title_Continue);
		pTitle_DlgApp->DefinePushBtn(IDC_RT_TITLE_NEW, GUI_OK);
		pTitle_DlgApp->SetCtrlNotifyFn(IDC_RT_TITLE_NEW, (CGUI_CallBack*)((void *)this), (GUI_CB_DlgNotifyFn)&RTTitle::RT_Title_New);
		pTitle_DlgApp->DefinePushBtn(IDC_RT_TITLE_EXIT, GUI_CANCEL);
		pTitle_DlgApp->SetCtrlNotifyFn(IDC_RT_TITLE_EXIT, (CGUI_CallBack*)((void *)this), (GUI_CB_DlgNotifyFn)&RTTitle::RT_Title_Exit);
		// GUI.LIB FIX - Exit validate functions for an entire dialog do not accept a FALSE return value.  This is the only way to do it.
		pTitle_DlgApp->DefineFldTxt(IDC_RT_TITLE_EXIT_NOTIFY_FIX, "", 1);    
		// Do not disable or will not try to validate pTitle_DlgApp->SetEnable(IDC_RT_EXIT_NOTIFY_FIX, FALSE);
		pTitle_DlgApp->SetVisible(IDC_RT_TITLE_EXIT_NOTIFY_FIX, FALSE);
		pTitle_DlgApp->SetCtrlValidateFn(IDC_RT_TITLE_EXIT_NOTIFY_FIX, (CGUI_CallBack*)((void *)this), (GUI_CB_DlgValidateFn)&RTTitle::RT_Title_Exit_Validate);
			
		pTitle_DlgApp->DefineFldStaticTxt(IDC_RT_TITLE_FACILITY_NAME);
		pTitle_DlgApp->DefineFldStaticTxt(IDC_RT_TITLE_PROMPT_FACILITY_NAME);
		pTitle_DlgApp->DefineFldStaticTxt(IDC_RT_TITLE_REVIEW_DATE_TIME);
		pTitle_DlgApp->DefineFldStaticTxt(IDC_RT_TITLE_PROMPT_REVIEW_DATE);
		pTitle_DlgApp->DefineFldStaticTxt(IDC_RT_TITLE_LOCATION);
		pTitle_DlgApp->DefineFldStaticTxt(IDC_RT_TITLE_PROMPT_LOCATION);
		pTitle_DlgApp->DefineFldStaticTxt(IDC_RT_TITLE_REVIEW_NUMBER);
		pTitle_DlgApp->DefineFldStaticTxt(IDC_RT_TITLE_PROMPT_REVIEW_NUMBER);
		pTitle_DlgApp->DefineFldStaticTxt(IDC_RT_TITLE_INSPECTORS);
		pTitle_DlgApp->DefineFldStaticTxt(IDC_RT_TITLE_PROMPT_INSPECTORS); 
		pTitle_DlgApp->DefineFldStaticTxt(IDC_RT_TITLE_PROMPT_REVIEW_PERIOD);
		pTitle_DlgApp->DefineFldStaticTxt(IDC_RT_TITLE_START_DATE_TIME);
		pTitle_DlgApp->DefineFldStaticTxt(IDC_RT_TITLE_PROMPT_START_DATE);
		pTitle_DlgApp->DefineFldStaticTxt(IDC_RT_TITLE_END_DATE_TIME);
		pTitle_DlgApp->DefineFldStaticTxt(IDC_RT_TITLE_PROMPT_END_DATE);
		pTitle_DlgApp->DefineFldStaticTxt(IDC_RT_TITLE_COMMENTS);
		pTitle_DlgApp->DefineFldStaticTxt(IDC_RT_TITLE_PROMPT_COMMENTS);
		}
	else {
		// Don't set the icon here again, GUI.LIB GPF
		pTitle_Dlg->SetTitle(pCommon_Params->Get_My_Name());
		// Setup the elements common to all title box screens
		pTitle_Dlg->DefinePushBtn(IDC_RT_TITLE_CHANGE, GUI_NONE);
		pTitle_Dlg->SetCtrlNotifyFn(IDC_RT_TITLE_CHANGE, (CGUI_CallBack*)((void *)this), (GUI_CB_DlgNotifyFn)&RTTitle::RT_Title_Change);
		pTitle_Dlg->DefinePushBtn(IDC_RT_TITLE_CONTINUE, GUI_OK);
		pTitle_Dlg->SetCtrlNotifyFn(IDC_RT_TITLE_CONTINUE, (CGUI_CallBack*)((void *)this), (GUI_CB_DlgNotifyFn)&RTTitle::RT_Title_Continue);
		pTitle_Dlg->DefinePushBtn(IDC_RT_TITLE_NEW, GUI_OK);
		pTitle_Dlg->SetCtrlNotifyFn(IDC_RT_TITLE_NEW, (CGUI_CallBack*)((void *)this), (GUI_CB_DlgNotifyFn)&RTTitle::RT_Title_New);
		pTitle_Dlg->DefinePushBtn(IDC_RT_TITLE_EXIT, GUI_CANCEL);
		pTitle_Dlg->SetCtrlNotifyFn(IDC_RT_TITLE_EXIT, (CGUI_CallBack*)((void *)this), (GUI_CB_DlgNotifyFn)&RTTitle::RT_Title_Exit);
		// GUI.LIB FIX - Exit validate functions for an entire dialog do not accept a FALSE return value.  This is the only way to do it.
		pTitle_Dlg->DefineFldTxt(IDC_RT_TITLE_EXIT_NOTIFY_FIX, "", 1);    
		// Do not disable or will not try to validate pTitle_Dlg->SetEnable(IDC_RT_EXIT_NOTIFY_FIX, FALSE);
		pTitle_Dlg->SetVisible(IDC_RT_TITLE_EXIT_NOTIFY_FIX, FALSE);
		pTitle_Dlg->SetCtrlValidateFn(IDC_RT_TITLE_EXIT_NOTIFY_FIX, (CGUI_CallBack*)((void *)this), (GUI_CB_DlgValidateFn)&RTTitle::RT_Title_Exit_Validate);
			
		pTitle_Dlg->DefineFldStaticTxt(IDC_RT_TITLE_FACILITY_NAME);
		pTitle_Dlg->DefineFldStaticTxt(IDC_RT_TITLE_PROMPT_FACILITY_NAME);
		pTitle_Dlg->DefineFldStaticTxt(IDC_RT_TITLE_REVIEW_DATE_TIME);
		pTitle_Dlg->DefineFldStaticTxt(IDC_RT_TITLE_PROMPT_REVIEW_DATE);
		pTitle_Dlg->DefineFldStaticTxt(IDC_RT_TITLE_LOCATION);
		pTitle_Dlg->DefineFldStaticTxt(IDC_RT_TITLE_PROMPT_LOCATION);
		pTitle_Dlg->DefineFldStaticTxt(IDC_RT_TITLE_REVIEW_NUMBER);
		pTitle_Dlg->DefineFldStaticTxt(IDC_RT_TITLE_PROMPT_REVIEW_NUMBER);
		pTitle_Dlg->DefineFldStaticTxt(IDC_RT_TITLE_INSPECTORS);
		pTitle_Dlg->DefineFldStaticTxt(IDC_RT_TITLE_PROMPT_INSPECTORS); 
		pTitle_Dlg->DefineFldStaticTxt(IDC_RT_TITLE_PROMPT_REVIEW_PERIOD);
		pTitle_Dlg->DefineFldStaticTxt(IDC_RT_TITLE_START_DATE_TIME);
		pTitle_Dlg->DefineFldStaticTxt(IDC_RT_TITLE_PROMPT_START_DATE);
		pTitle_Dlg->DefineFldStaticTxt(IDC_RT_TITLE_END_DATE_TIME);
		pTitle_Dlg->DefineFldStaticTxt(IDC_RT_TITLE_PROMPT_END_DATE);
		pTitle_Dlg->DefineFldStaticTxt(IDC_RT_TITLE_COMMENTS);
		pTitle_Dlg->DefineFldStaticTxt(IDC_RT_TITLE_PROMPT_COMMENTS);
		}
*/
		ULTitleDlg Dlg;
		Dlg.DoModal();

    return TRUE;

}


/*void RTTitle::RT_Title_Delete(void) {

/*	if (bCreate_As_App) {
		if (pTitle_DlgApp) {
			delete pTitle_DlgApp;
			pTitle_DlgApp = NULL;
			}
		}
	else {
		if (pTitle_Dlg) {
			delete pTitle_Dlg;
			pTitle_Dlg = NULL;
			}
		}

	FlushEvents();  // Always perform after deleting a dialog app
*/
//}


/*kdm - splash dialog handles it's own destroy.
void RTTitle::RT_Splash_Delete(void) {

	// Will delete itself
	RT_Title_Splash_Close();
	if (pszSplash_Title1) {
		delete [] pszSplash_Title1;
		pszSplash_Title1 = NULL;
		}
	if (pszSplash_Title2) {
		delete [] pszSplash_Title2;
		pszSplash_Title2 = NULL;
		}
	FlushEvents();  // Always perform after deleting a dialog app

}
*/

/////////////////////////////////////////////////////////////////////////////////////////////
// The title dialog box screen displaying member functions
/////////////////////////////////////////////////////////////////////////////////////////////

void RTTitle::RT_Title_Start_Screen_ON(void) {

//kdm	CGStr Date_String, Time_String, String;
	CString Date_String, Time_String, String;
//kdm	UINT ctIndex;
//kdm	CFmtdDate Date;
	COleDateTime Date; 
//kdm	CFmtdTime Time;
	COleDateTime Time;
	ULSupport Library;
	

    // Set the inspector information boxes
//kdm    pCommon_Title->FldStaticTxtSetValue(IDC_RT_TITLE_FACILITY_NAME, pCommon_Params->Get_Facility_Name());
 //kdm   Date.Set(*pCommon_Params->Get_Inspection_Date());
	UL_GEN_DATE_STRUCT InspectionDate = *pCommon_Params->Get_Inspection_Date();  //kdm
    Date.SetDate(InspectionDate.yr, InspectionDate.mon, InspectionDate.day);   //kdm

//kdm	Date.Get(&Date_String);
	Date_String = Library.GetCurrentDate();
	UL_GEN_TIME_STRUCT  InspectionTime = *pCommon_Params->Get_Inspection_Time();  //kdm
    Time.SetTime(InspectionTime.hr, InspectionTime.min, InspectionTime.sec); //kdm
//kdm	Time.Get(&Time_String);
	Time_String = Library.GetCurrentDate();

	String = Date_String;
	String += "  -  ";
	String += Time_String;
 //kdm   pCommon_Title->FldStaticTxtSetValue(IDC_RT_TITLE_REVIEW_DATE_TIME, String.Get_sz());
//kdm    pCommon_Title->FldStaticTxtSetValue(IDC_RT_TITLE_REVIEW_DATE_TIME, (LPCTSTR)String);
/*	pCommon_Title->FldStaticTxtSetValue(IDC_RT_TITLE_LOCATION, pCommon_Params->Get_Facility_Location());
    pCommon_Title->FldStaticTxtSetValue(IDC_RT_TITLE_REVIEW_NUMBER, pCommon_Params->Get_Inspection_Number());
	if (pCommon_Params->Get_Inspector_ID_Num() > 0) {
		String = "";
		for (ctIndex = 0 ; ctIndex < pCommon_Params->Get_Inspector_ID_Choices_Num(); ctIndex++) {
    		String += pCommon_Params->Get_Inspector_ID(pCommon_Params->Get_Inspector_ID_Choice(ctIndex));
    		if (ctIndex < (pCommon_Params->Get_Inspector_ID_Choices_Num() - 1))
    			String += ", ";
    		}
//KDM		pCommon_Title->FldStaticTxtSetValue(IDC_RT_TITLE_INSPECTORS, String.Get_sz());
		pCommon_Title->FldStaticTxtSetValue(IDC_RT_TITLE_INSPECTORS, (LPCTSTR)String);
		}
*/
	UL_GEN_DATE_STRUCT StartDate = *pCommon_Params->Get_Start_Date(); //kdm
 //kdm   Date.Set(*pCommon_Params->Get_Start_Date());
    Date.SetDate(StartDate.yr, StartDate.mon, StartDate.day);  //kdm

//kdm	Date.Get(&Date_String);
	Date_String = Library.GetCurrentDate();
	UL_GEN_TIME_STRUCT StartTime = *pCommon_Params->Get_Start_Time();
//kdm    Time.Set(*pCommon_Params->Get_Start_Time());
    Time.SetTime(StartTime.hr, StartTime.min, StartTime.sec);
//kdm	Time.Get(&Time_String);
	Time_String = Library.GetCurrentDate(); 

	String = Date_String;
	String += "  -  ";
	String += Time_String;
 //KDM   pCommon_Title->FldStaticTxtSetValue(IDC_RT_TITLE_START_DATE_TIME, String.Get_sz());
//kdm    pCommon_Title->FldStaticTxtSetValue(IDC_RT_TITLE_START_DATE_TIME, (LPCTSTR)String);

	UL_GEN_DATE_STRUCT  EndDate = *pCommon_Params->Get_End_Date();
 //kdm   Date.Set(*pCommon_Params->Get_End_Date());
    Date.SetDate(EndDate.yr, EndDate.mon, EndDate.day);
//kdm	Date.Get(&Date_String);
	Date_String = Library.GetCurrentDate(); 
	UL_GEN_TIME_STRUCT EndTime = *pCommon_Params->Get_End_Time();
 //kdm   Time.Set(*pCommon_Params->Get_End_Time());
    Time.SetTime(EndTime.hr, EndTime.min, EndTime.sec);
//kdm	Time.Get(&Time_String);
	Time_String = Library.GetCurrentDate(); 

	String = Date_String;
	String += "  -  ";
	String += Time_String;
 //KDM   pCommon_Title->FldStaticTxtSetValue(IDC_RT_TITLE_END_DATE_TIME, String.Get_sz());
//kdm    pCommon_Title->FldStaticTxtSetValue(IDC_RT_TITLE_END_DATE_TIME, (LPCTSTR)String);
 /*   pCommon_Title->FldStaticTxtSetValue(IDC_RT_TITLE_COMMENTS, pCommon_Params->Get_Inspection_Comments());


	// Turn all of the appropriate controls on    
	pCommon_Title->SetVisible(IDC_RT_TITLE_PROMPT_FACILITY_NAME, TRUE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_PROMPT_FACILITY_NAME, TRUE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_FACILITY_NAME, TRUE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_FACILITY_NAME, TRUE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_PROMPT_REVIEW_DATE, TRUE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_PROMPT_REVIEW_DATE, TRUE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_REVIEW_DATE_TIME, TRUE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_REVIEW_DATE_TIME, TRUE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_PROMPT_LOCATION, TRUE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_PROMPT_LOCATION, TRUE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_LOCATION, TRUE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_LOCATION, TRUE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_PROMPT_REVIEW_NUMBER, TRUE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_PROMPT_REVIEW_NUMBER, TRUE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_REVIEW_NUMBER, TRUE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_REVIEW_NUMBER, TRUE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_PROMPT_INSPECTORS, TRUE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_PROMPT_INSPECTORS, TRUE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_INSPECTORS, TRUE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_INSPECTORS, TRUE); 
	pCommon_Title->SetVisible(IDC_RT_TITLE_PROMPT_REVIEW_PERIOD, TRUE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_PROMPT_REVIEW_PERIOD, TRUE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_PROMPT_START_DATE, TRUE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_PROMPT_START_DATE, TRUE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_START_DATE_TIME, TRUE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_START_DATE_TIME, TRUE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_PROMPT_END_DATE, TRUE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_PROMPT_END_DATE, TRUE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_END_DATE_TIME, TRUE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_END_DATE_TIME, TRUE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_PROMPT_COMMENTS, TRUE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_PROMPT_COMMENTS, TRUE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_COMMENTS, TRUE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_COMMENTS, TRUE);   
    
	// Keep last, prevents hanging
	pCommon_Title->SetVisible(IDC_RT_TITLE_CHANGE, TRUE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_CHANGE, TRUE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_CONTINUE, TRUE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_CONTINUE, TRUE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_NEW, TRUE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_NEW, TRUE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_EXIT, TRUE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_EXIT, TRUE);
*/
}


void RTTitle::RT_Title_Start_Screen_OFF(void) {

/*	pCommon_Title->SetVisible(IDC_RT_TITLE_CHANGE, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_CHANGE, FALSE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_CONTINUE, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_CONTINUE, FALSE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_NEW, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_NEW, FALSE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_EXIT, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_EXIT, FALSE);
    
	pCommon_Title->SetVisible(IDC_RT_TITLE_PROMPT_FACILITY_NAME, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_PROMPT_FACILITY_NAME, FALSE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_FACILITY_NAME, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_FACILITY_NAME, FALSE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_PROMPT_REVIEW_DATE, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_PROMPT_REVIEW_DATE, FALSE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_REVIEW_DATE_TIME, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_REVIEW_DATE_TIME, FALSE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_PROMPT_LOCATION, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_PROMPT_LOCATION, FALSE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_LOCATION, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_LOCATION, FALSE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_PROMPT_REVIEW_NUMBER, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_PROMPT_REVIEW_NUMBER, FALSE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_REVIEW_NUMBER, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_REVIEW_NUMBER, FALSE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_PROMPT_INSPECTORS, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_PROMPT_INSPECTORS, FALSE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_INSPECTORS, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_INSPECTORS, FALSE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_PROMPT_REVIEW_PERIOD, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_PROMPT_REVIEW_PERIOD, FALSE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_PROMPT_START_DATE, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_PROMPT_START_DATE, FALSE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_START_DATE_TIME, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_START_DATE_TIME, FALSE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_PROMPT_END_DATE, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_PROMPT_END_DATE, FALSE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_END_DATE_TIME, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_END_DATE_TIME, FALSE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_PROMPT_COMMENTS, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_PROMPT_COMMENTS, FALSE);
	pCommon_Title->SetVisible(IDC_RT_TITLE_COMMENTS, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_COMMENTS, FALSE);   
  */  
}

/////////////////////////////////////////////////////////////////////////////////////////////
// The title dialog box button member functions
/////////////////////////////////////////////////////////////////////////////////////////////

/*void RTTitle::RT_Title_Change(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) {
	
	pCommon_Pipes->Set_Messages_On_Hold(TRUE);

	pCommon_Title->SetEnable(IDC_RT_TITLE_CHANGE, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_CONTINUE, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_NEW, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_EXIT, FALSE);

	bInspection_Info_Open = TRUE;
	if (pCommon_Params->RT_Init_Change_Inspection_Info(uiIcon)) {
		RT_Title_Start_Screen_ON();
	    CGUI_App::ProcessEvents();
		}
	else {
		pCommon_Title->SetEnable(IDC_RT_TITLE_CHANGE, TRUE);
		pCommon_Title->SetEnable(IDC_RT_TITLE_CONTINUE, TRUE);
		pCommon_Title->SetEnable(IDC_RT_TITLE_NEW, TRUE);
		pCommon_Title->SetEnable(IDC_RT_TITLE_EXIT, TRUE); 
		}
	bInspection_Info_Open = FALSE;

	pCommon_Pipes->Set_Messages_On_Hold(FALSE);
		
}
*/
/*
void RTTitle::RT_Title_Continue(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) {

	eStart_Operation = START_REVIEW;

}
*/

/*
void RTTitle::RT_Title_New(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) {

	BOOL bInspection_Info_Open, bMIC_Inspec_Info_Loaded = FALSE;

	pCommon_Pipes->Set_Messages_On_Hold(TRUE);


	pCommon_Title->SetEnable(IDC_RT_TITLE_CHANGE, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_CONTINUE, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_NEW, FALSE);
	pCommon_Title->SetEnable(IDC_RT_TITLE_EXIT, FALSE);

	// Check to see if should default inspection parameters to a mic inspection information file
	if (pCommon_Params->Get_MIC_Inspec_Info_File_Use()) {
		bMIC_Inspec_Info_Loaded = RT_Title_Load_MIC_Inspection_Information();
		}

	bInspection_Info_Open = TRUE;
	if (pCommon_Params->RT_Init_Change_Inspection_Info(uiIcon, ((!pCommon_Params->Get_MIC_Inspec_Info_File_Use()) || (!bMIC_Inspec_Info_Loaded)), FALSE, FALSE)) {
		eStart_Operation = START_IMPORT;
		}
	else {
		// If used MIC inspection information parameters above, then change them back by resetting each one
		pCommon_Params->Reset_Inspection_Info();
		eStart_Operation = START_TITLE;
		// GUI.LIB BUG - Seems to lose the change button and thinks it is an exit button
		}
	bInspection_Info_Open = FALSE;


	pCommon_Pipes->Set_Messages_On_Hold(FALSE);

}
*/

BOOL RTTitle::RT_Title_Load_MIC_Inspection_Information(void) 
{

//kdm	CGUI_Dlg *pMIC_Load_Inspec_Info = NULL;
//kdm	CGStr MIC_Inspec_Info_Title, MIC_Full_File_Name;
	CString MIC_Inspec_Info_Title, MIC_Full_File_Name;
	BOOL bMIC_Parameters_Loaded = FALSE;
/*	char *pszMIC_Facility = NULL, 
		*pszMIC_Previous = NULL, 
		*pszMIC_Date_Time = NULL, 
		*pszMIC_Inspector_Name = NULL, 
		*pszMIC_Inspector_ID = NULL, 
		*pszMIC_Inspection_Number = NULL, 
		*pszMIC_Full_Filename = NULL;
*/
	char	*pszMIC_Full_Filename = NULL;
//kdm	char *ppszAsci_Month[12] = {"January","February","March", "April","May","June", "July","August","September", "October","November","December"};
//kdm	char *pszCurrent_Asci_Month;
//kdm	GEN_DATE_STRUCT Temp_Date_Struct;
//kdm	GEN_TIME_STRUCT Temp_Time_Struct;
//kdm	time_t Time;
//KDM	struct tm *Time_Struct;
//kdm	UINT ctInspector_ID, ctInspector_Choice;
//kdm	BOOL bInspector_ID_Already_Added;
//kdm	char **ppszNew_Inspector_IDs = NULL;
//kdm	UINT puiTemp_Inspector_ID_Choices[1] = {0};
//kdm	CGStr New_Inspector_Name_ID;
//kdm	CString New_Inspector_Name_ID;


	bMIC_Parameters_Loaded = FALSE;

	
	// Check that the file exists.  Since it could be *.ip, check this way in order to expand the * into
	//    the first filename in the specified directory with the extension.	
	HANDLE MIC_File_Handle;
	WIN32_FIND_DATA MIC_File_Info;
	if (INVALID_HANDLE_VALUE == (MIC_File_Handle = FindFirstFile(pCommon_Params->Get_MIC_Inspec_Info_Filename(), &MIC_File_Info))) 
		RT_Title_Error(uiRT_TITLE_ERROR_MIC_INSPECTION_INFORMATION_FILE, pCommon_Params->Get_MIC_Inspec_Info_Filename());
	else  //File exists.  Pass the file name to the dialog box class.
	{
		// Close the file search handle
		FindClose(MIC_File_Handle);
		
		MIC_Full_File_Name.Format("%s", pCommon_Params->Get_MIC_Inspec_Info_Filename());
		if(MIC_Full_File_Name.ReverseFind('\\') != -1)
		{
			MIC_Full_File_Name.SetAt(MIC_Full_File_Name.ReverseFind('\\') + 1, '\0');
		}
		
		MIC_Full_File_Name += MIC_File_Info.cFileName;
		
		
		//Assemble the title for the dialog box.
		MIC_Inspec_Info_Title = pCommon_Params->Get_My_Name();
		MIC_Inspec_Info_Title += " - Loading MIC Inspection Information";
			
		
		//Create the MIC inspection dialog.  Pass in information that is needed.
		ULMICInspectionInfoDlg MICDlg(MIC_Full_File_Name, MIC_Inspec_Info_Title, pCommon_Params, uiIcon ); //kdm
		int RetVal = MICDlg.DoModal();  //kdm

		
		// All of the parameters were found, set them for the new inspection
		bMIC_Parameters_Loaded = (RetVal != IDCANCEL) ? false: true;
			
	}//end ELSE INI file exists.


	if (pszMIC_Full_Filename) 
		delete [] pszMIC_Full_Filename;


	return bMIC_Parameters_Loaded;

}


/*void RTTitle::RT_Title_Exit(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) {

	eStart_Operation = START_EXIT;
	
}
*/
/*
BOOL RTTitle::RT_Title_Exit_Validate(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID) {

	if (eStart_Operation != START_TITLE)
		// Exit to the new mode
		return TRUE;
	else {
		// Don't exit, just reset the title screen
		RT_Title_Start_Screen_ON();
	    CGUI_App::ProcessEvents();
		return FALSE;
		}

}
*/

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Provide the Title Exit procedure if necessary
///////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL RTTitle::RT_Title_Exit(void) {

	// Close all of the dialog boxes which are open
/*	if (bInspection_Info_Open) {
		// FIX: pointer to dialog box not within scope
		}

	if (bCreate_As_App)
	 	pTitle_DlgApp->CGUI_Dlg::Close();
	else pTitle_Dlg->Close();

	eStart_Operation = START_EXIT;
	
*/	
	return TRUE;
	
}


//////////////////////////////////////////////////////////////////////////////////////////
// Use the default error messages below except when an error occurs specific to your tool
//////////////////////////////////////////////////////////////////////////////////////////

void RTTitle::RT_Title_Error(uiRT_TITLE_ERROR uiError, const char *pszError_Parameter_1) {

//kdm	CGStr Message;
	CString Message;


	switch(uiError) {
		case uiRT_TITLE_ERROR_NO_MEMORY :
			RTError.Error(uiRT_ERROR_NO_MEMORY);
			break;
		case uiRT_TITLE_ERROR_INSPEC_CHANGE :
			RTError.Error(uiRT_ERROR_INSPEC_CHANGE);
			break;
		case uiRT_TITLE_ERROR_MIC_INSPECTION_INFORMATION_FILE :
			Message = "The inspection information file \"";
			Message += pszError_Parameter_1;
			Message += "\" which contains the inspection information entered within Multi-Instrument Collect could not be properly read.  ";
			Message += "The inspection information for the new inspection will be defaulted according to the last inspection.";
//KDM			GUI_MsgBox(Message.Get_sz());
			MessageBox(NULL, Message, "Message", MB_OK);
			break;
		default:
			RTError.Error(uiError, pszError_Parameter_1);
			break;			
    	}

}
		
