//////////////////////////////////////////////////////////////////////////
// Copyright 2004, The Regents Of the University of California.  This
// software was produced under U.S. Government contract (W-7405-ENG-36) at
// the Los Alamos National Laboratory by the Safeguards Science and
// Technology Group (N-1).  All Rights Reserved.
//////////////////////////////////////////////////////////////////////////
// $History: RevwTool.cpp $
// 
// *****************  Version 7  *****************
// User: 186846       Date: 12/18/07   Time: 4:34p
// Updated in $/FDMS/FDMS Application/FDMSNET/Common
// VS 2005 changes completed
// 
// *****************  Version 6  *****************
// User: Longo        Date: 3/16/05    Time: 2:21p
// Updated in $/FDMS/FDMS Application/FDMSNET/Common
// 
// *****************  Version 5  *****************
// User: Longo        Date: 12/10/04   Time: 4:45p
// Updated in $/FDMS/FDMS Application/FDMSNET/Common
// try/catch and warning removal
// 
// *****************  Version 4  *****************
// User: Longo        Date: 11/30/04   Time: 4:13p
// Updated in $/FDMS/FDMS Application/FDMSNET/Common
// catch a few errors to prevent failure
//////////////////////////////////////////////////////////////////////////

// Steven Edward Buck is the perpetrator of this design and implementation
// Los Alamos National Laboratory
// NIS-5
// 3/10/97

// Rev_Tool.CPP
// Main source file for the abstract class Review Tool

#include <stdafx.h>
#include <math.h>
#include "RevwTool.H"   
#include "ULStartToolMsgDlg.h"


/////////////////////////////////////////////////////////////////////////////////////
//  Global procedures for starting a Review Tool
/////////////////////////////////////////////////////////////////////////////////////

// Gets the command line arguments without having an application
//kdm    BOOL Review_Tool_GetCmdLineArgs(HINSTANCE hInstance, CGStr &Args, UINT *puiNumArgs, char **paszCmdLineParms[]) {
BOOL Review_Tool_GetCmdLineArgs(HINSTANCE hInstance, int NumArgs, CString &Args, UINT *puiNumArgs, char **paszCmdLineParms[]) 
{
	int uiEndIndex = 0, uiStartIndex = 0;
	BOOL bFinished = FALSE;
	CString LineStr;
	char *pArgStr;

	(*puiNumArgs) = NumArgs;
	(*paszCmdLineParms) = new char *[(NumArgs) + 1];
	if ((*paszCmdLineParms) != NULL) 
	{

		//Command arguments are counted and loaded into the paszCmdLineParms array.
		//The first item in the paszCmdLineParms is reserved for, and loaded with, the
		//command line path to the review tool.
		for(int Count = 1; Count < NumArgs; Count++)
		{
			uiStartIndex = Args.Find("/", uiStartIndex);
			uiEndIndex = Args.Find("/", uiStartIndex + 1);
			if (uiStartIndex != -1 && (LineStr = Args.Mid(uiStartIndex + 1, uiEndIndex == -1 ? Args.GetLength() : uiEndIndex ))) 
			{
				if (!LineStr.IsEmpty()) 
				{
					pArgStr = new char[LineStr.GetLength() + 1];  //kdm
					strcpy(pArgStr, (LPCSTR)LineStr);  //kdm
					((*paszCmdLineParms))[Count] = pArgStr;

					uiStartIndex++;  //Increment the starting location in the command line args string.
				}
				else
					bFinished = TRUE;
			}
			else 
				bFinished = TRUE;
		}

		const UINT uiBUFFER_LEN = 260;
		char szBuffer[uiBUFFER_LEN + 1];
		GetModuleFileName(hInstance, szBuffer, uiBUFFER_LEN);
		LineStr = szBuffer;
		LineStr.Remove(' ');
		pArgStr = new char[LineStr.GetLength() + 1];  //kdm
		strcpy(pArgStr, (LPCTSTR)LineStr); //kdm
		((*paszCmdLineParms))[0] = pArgStr;

		(*paszCmdLineParms)[(NumArgs)] = NULL;  //Load last place in the array with a NULL.

	}//end IF paszCmdLineParms != NULL 


	return TRUE;

}


BOOL Review_Tool_Startup_Check(const char *pszTOOL_INI, const char *pszTOOL_NAME, UINT uiNum_Args, const char **ppszCommand_Line_Args) {

	UINT ctArg_Num;
	fstream *pMyself = NULL;
	//kdm	CGStr Message;
	CString Message;
	BOOL bSuccess = TRUE, bSkip_Running_Msg = FALSE;


	// If /SKIP_RUNNING_MSG on command line paramter then just shut down without any message
	for (ctArg_Num = 1; ctArg_Num < uiNum_Args; ctArg_Num++) {
		if (_strnicmp(ppszCommand_Line_Args[ctArg_Num], szSKIP_RUNNING_MSG, strlen(szSKIP_RUNNING_MSG)) == 0) {
			bSkip_Running_Msg = TRUE;
		}
	}

	// Check that tool is properly installed
	if ((pMyself = new fstream(pszTOOL_INI, ios::in | ios::_Nocreate)) != NULL) {
		if (pMyself->is_open() == 0) 
		{
			delete pMyself;

			// Determine if another tool running or bad directory structure, if missing will continue and later
			//    display message of bad ini
			if ((pMyself = new fstream(pszTOOL_INI, ios::in | ios::_Nocreate)) != NULL) 
			{
				if (pMyself->is_open() == 0) {
					Message = "The parameter file \"";
					Message += pszTOOL_INI;
					Message += "\" could not be found.  Some of the ";
					Message += pszTOOL_NAME;
					Message += " files have been deleted or moved.  Please reinstall ";
					Message += pszTOOL_NAME;
					Message += ".";
					MessageBox(NULL, Message, "Error Message", MB_ICONERROR);
				}
				else 
				{
					if (!bSkip_Running_Msg) 
					{
						Message = pszTOOL_NAME;
						Message += " is already running.  Only one version can run at a time.\n\nIf you just exited the application, you may have to wait a few seconds before restarting it.";
						MessageBox(NULL, Message, "Error Message", MB_ICONERROR);
					}
				}

				delete pMyself;
				bSuccess = FALSE;
			}

			//			MessageBox(NULL, "StartupCheck is returning FALSE", "DEBUG", MB_OK);
			return FALSE;
		}

		else delete pMyself;
	}


	return bSuccess;

}




/////////////////////////////////////////////////////////////////////////////////////
//  Review_Tool constructor:  Setup all intial parameters, and the pipe
/////////////////////////////////////////////////////////////////////////////////////

Review_Tool::Review_Tool(RESOURCE_ID uiRTIcon, RTSetup& rRT_Setup, RTImport& rRT_Import, RTReanalyze& rRT_Reanalyze, RTReview& rRT_Review, RTInit &rRT_Init, RTTitle *pRT_Title, RTPipes *pRT_Pipe) :
uiIcon(uiRTIcon),
rSetup(rRT_Setup),
rImport(rRT_Import),
rReanalyze(rRT_Reanalyze),
rReview(rRT_Review),
rInit(rRT_Init),
pTitle(NULL),
bTitle_Provided(FALSE),	
pPipe(NULL),
bPipe_Provided(FALSE),	
eCurrent_Task(TASK_TITLE),
bRT_Exit(FALSE),
bWas_Connected(FALSE),
pName(NULL),
bApp_Created(FALSE)
{

	// Set or create title
	// NOTE: Keep outside of parameter loading in case splash screen open and have to close due to error later
	if (pRT_Title) {
		pTitle = pRT_Title;
		bTitle_Provided = TRUE;
	}
	else if ((pTitle = new RTTitle(uiRTIcon)) == NULL) {
		Rev_Tool_Error(uiREV_TOOL_ERROR_NO_MEMORY);
		exit(-1);
	}

	// Set the pipe if provided, must create later if not
	if (pRT_Pipe) {
		pPipe = pRT_Pipe;
		bPipe_Provided = TRUE;
	}

}


Review_Tool::~Review_Tool(void) {

	if (pName) {
		delete pName;
		pName = NULL;
	}
	if ((!bTitle_Provided) && (pTitle)) {
		delete pTitle; 
		pTitle = NULL;
	}
	if ((!bPipe_Provided) && (pPipe)) {
		delete pPipe; 
		pPipe = NULL;
	}

}


////////////////////////////////////////////////////////////////////////////////////////
//  Review_Tool launcher:  After declaring a Review Tool, just call this member function
////////////////////////////////////////////////////////////////////////////////////////

void Review_Tool::Review_Tool_Begin(BOOL bApp_Already_Created, const char *pszTool_Name, UINT uiNum_Args, char **ppszCommand_Line_Args) {

	UINT ctArg_Num, ctIndex, uiSplash_Screen_Timeout_Seconds;
	char *pszStart_Mode = NULL;
	BOOL bTool_Found;
	BOOL bStart_Mode_Found = FALSE, bCreate_Success = TRUE;

	bApp_Created = bApp_Already_Created;
	// Set the tool name
	if ((pName = new char [strlen(pszTool_Name) + 1]) == NULL) {
		Rev_Tool_Error(uiREV_TOOL_ERROR_NO_MEMORY);
		exit(-1);
	}
	strcpy(pName, pszTool_Name);


	// Just get the splash screen timeout.  Load the rest of the parameters later.
	if (!rInit.Load_Splash_Screen_Timeout_Seconds(&uiSplash_Screen_Timeout_Seconds)) {
		Rev_Tool_Error(uiRT_ERROR_INIT_LOAD, rInit.Get_Errored_Init_Parameter(), rInit.Get_Warning_Init_Parameter());
		return;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////
	// Determine the appropriate dialog app of app to create before doing anything else
	//////////////////////////////////////////////////////////////////////////////////////////////////
	if (!bApp_Created) {
		// Just get the start mode.  Load the rest of the parameters later.
		if (!rInit.Load_Start_Mode(&pszStart_Mode)) {
			Rev_Tool_Error(uiRT_ERROR_INIT_LOAD, rInit.Get_Errored_Init_Parameter(), rInit.Get_Warning_Init_Parameter());
			return;
		}
		// JUST CALL THE APPROPRIATE CREATE APP!!!
		// Do NOT try to determine the start mode yet because all of the command line parameters can not be evaluated at this time.
		// If no command line parameters or the first parameter is TITLE, then set the current task to the start mode listed in the ini
		// ***The title mode is a special mode, if it is on the command line then just run in normal mode.
		// This allows a user to display the title inspection info screen without getting into the INI file
		if ((uiNum_Args == 1) || // Command line arguments do not exist
			((uiNum_Args == 2) && ((_strnicmp(szBEGIN_TITLE, ppszCommand_Line_Args[1], strlen(szBEGIN_TITLE)) == 0) ||
			(_strnicmp(szBEGIN_INSPECTION, ppszCommand_Line_Args[1], strlen(szBEGIN_INSPECTION)) == 0)))) {
				if ((uiNum_Args == 2) && ((_strnicmp(szBEGIN_TITLE, ppszCommand_Line_Args[1], strlen(szBEGIN_TITLE)) == 0) ||
					(_strnicmp(szBEGIN_INSPECTION, ppszCommand_Line_Args[1], strlen(szBEGIN_INSPECTION)) == 0)))
					bCreate_Success = pTitle->RT_Title_Create_App(pName);
				else if (_strnicmp(szBEGIN_TITLE, pszStart_Mode, strlen(szBEGIN_TITLE)) == 0)
					bCreate_Success = pTitle->RT_Title_Create_App(pName);
				else if (_strnicmp(szBEGIN_INSPECTION, pszStart_Mode, strlen(szBEGIN_INSPECTION)) == 0)
					bCreate_Success = pTitle->RT_Title_Create_App(pName);
				else if (_strnicmp(szBEGIN_SETUP, pszStart_Mode, strlen(szBEGIN_SETUP)) == 0)
					bCreate_Success = rSetup.RT_Setup_Create_App(pName);
				else if (_strnicmp(szBEGIN_IMPORT, pszStart_Mode, strlen(szBEGIN_IMPORT)) == 0)
					bCreate_Success = rImport.RT_Import_Create_App(pName, FALSE);
				else if (_strnicmp(szBEGIN_REANALYZE, pszStart_Mode, strlen(szBEGIN_REANALYZE)) == 0)
					bCreate_Success = rReanalyze.RT_Reanalyze_Create_App(pName);
				else if (_strnicmp(szBEGIN_REVIEW, pszStart_Mode, strlen(szBEGIN_REVIEW)) == 0)
					bCreate_Success = rReview.RT_Review_Create_App(pName);
				// Default to title
				else bCreate_Success = pTitle->RT_Title_Create_App(pName);
			}

			// Command line parameters exist, determine how to start and what app create to call
		else {
			// Do not want to call more then one, first one found is always used
			ctArg_Num = 1;
			bStart_Mode_Found = FALSE;
			while ((!bStart_Mode_Found) && (ctArg_Num < uiNum_Args)) {
				if (_strnicmp(ppszCommand_Line_Args[ctArg_Num], szBEGIN_TITLE, strlen(szBEGIN_TITLE)) == 0) {
					bCreate_Success = pTitle->RT_Title_Create_App(pName);
					bStart_Mode_Found = TRUE;
				}
				else if (_strnicmp(ppszCommand_Line_Args[ctArg_Num], szBEGIN_INSPECTION, strlen(szBEGIN_INSPECTION)) == 0) {
					bCreate_Success = pTitle->RT_Title_Create_App(pName);
					bStart_Mode_Found = TRUE;
				}
				else if (_strnicmp(ppszCommand_Line_Args[ctArg_Num], szBEGIN_SETUP, strlen(szBEGIN_SETUP)) == 0) {
					bCreate_Success = rSetup.RT_Setup_Create_App(pName);
					bStart_Mode_Found = TRUE;
				}
				else if (_strnicmp(ppszCommand_Line_Args[ctArg_Num], szBEGIN_IMPORT, strlen(szBEGIN_IMPORT)) == 0) {
					bCreate_Success = rImport.RT_Import_Create_App(pName, TRUE);
					bStart_Mode_Found = TRUE;
				}
				else if (_strnicmp(ppszCommand_Line_Args[ctArg_Num], szBEGIN_REANALYZE, strlen(szBEGIN_REANALYZE)) == 0) {
					bCreate_Success = rReanalyze.RT_Reanalyze_Create_App(pName);
					bStart_Mode_Found = TRUE;
				}
				else if (_strnicmp(ppszCommand_Line_Args[ctArg_Num], szBEGIN_REVIEW, strlen(szBEGIN_REVIEW)) == 0) {
					bCreate_Success = rReview.RT_Review_Create_App(pName);
					bStart_Mode_Found = TRUE;
				}
				ctArg_Num++;
			}
			// If a start mode was not found then use mode in parameter file
			if (!bStart_Mode_Found) {
				if (_strnicmp(szBEGIN_TITLE, pszStart_Mode, strlen(szBEGIN_TITLE)) == 0) {
					bCreate_Success = pTitle->RT_Title_Create_App(pName);
					eCurrent_Task = TASK_TITLE;
				}
				else if (_strnicmp(szBEGIN_INSPECTION, pszStart_Mode, strlen(szBEGIN_INSPECTION)) == 0) {
					bCreate_Success = pTitle->RT_Title_Create_App(pName);
					eCurrent_Task = TASK_TITLE;
				}
				else if (_strnicmp(szBEGIN_SETUP, pszStart_Mode, strlen(szBEGIN_SETUP)) == 0) {
					bCreate_Success = rSetup.RT_Setup_Create_App(pName);
					eCurrent_Task = TASK_SETUP;
				}
				else if (_strnicmp(szBEGIN_IMPORT, pszStart_Mode, strlen(szBEGIN_IMPORT)) == 0) {
					bCreate_Success = rImport.RT_Import_Create_App(pName, FALSE);
					eCurrent_Task = TASK_IMPORT;
				}
				else if (_strnicmp(szBEGIN_REANALYZE, pszStart_Mode, strlen(szBEGIN_REANALYZE)) == 0) {
					bCreate_Success = rReanalyze.RT_Reanalyze_Create_App(pName);
					eCurrent_Task = TASK_REANALYZE;
				}
				else if (_strnicmp(szBEGIN_REVIEW, pszStart_Mode, strlen(szBEGIN_REVIEW)) == 0) {
					bCreate_Success = rReview.RT_Review_Create_App(pName);
					eCurrent_Task = TASK_REVIEW;
				}
				// Default to title
				else {
					bCreate_Success = pTitle->RT_Title_Create_App(pName);
					eCurrent_Task = TASK_TITLE;
				}
			}
		}

		if (pszStart_Mode) {
			delete [] pszStart_Mode;
			pszStart_Mode = NULL;
		}
		if (!bCreate_Success) {
			Rev_Tool_Error(uiREV_TOOL_ERROR_NO_MEMORY);
			return;
		}
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////
	// Display the splash screen
	//////////////////////////////////////////////////////////////////////////////////////////////////
	pTitle->RT_Title_Splash_Display(uiSplash_Screen_Timeout_Seconds);


	//////////////////////////////////////////////////////////////////////////////////////////////////
	// Load the parameters	
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// Only continue if ini properly loaded
	if ((!rInit.Load_Parameters()) ||
		(!rInit.Parameters_Loaded())) {
			if (pTitle) 
				//kdm			pTitle->RT_Title_Splash_Close();
				Rev_Tool_Error(uiRT_ERROR_INIT_LOAD, rInit.Get_Errored_Init_Parameter(), rInit.Get_Warning_Init_Parameter());
			return;
		}

		// Set the common error messages name
		if (!RTError.Set_My_Name(rInit.Get_My_Name()))
			Rev_Tool_Error(uiREV_TOOL_ERROR_NO_MEMORY);



		//////////////////////////////////////////////////////////////////////////////////////////////////
		// Set or create all of the communication pipes
		//////////////////////////////////////////////////////////////////////////////////////////////////
		if (!pPipe) {
			if ((pPipe = new RTPipes(*this, rInit.Get_Pipe_My_Name())) == NULL) {
				Rev_Tool_Error(uiREV_TOOL_ERROR_NO_MEMORY);
				return;
			}
		}

		for (ctIndex = 0; ctIndex < rInit.Get_Tools_Num(); ctIndex++) {
			if (!pPipe->Create(rInit.Get_Pipe_Other_Name(ctIndex), rInit.Get_Pipe_Timeout(ctIndex)))
				Rev_Tool_Error(uiREV_TOOL_ERROR_PIPE_OPEN_FAIL, rInit.Get_Tool_Name(ctIndex));
		}


		try
		{
			//////////////////////////////////////////////////////////////////////////////////////////////////
			// Setup the three main operations - giving pointers to the RTInit and Pipe to 
			//the different upper layer objects : Title, Setup, Import, Reanalyze, Review. - kdm
			//////////////////////////////////////////////////////////////////////////////////////////////////
			pTitle->Set_Common_Params(&rInit);
			pTitle->Set_Common_Pipes(pPipe);
			rSetup.Set_Common_Params(&rInit);
			rSetup.Set_Common_Pipes(pPipe);
			rImport.Set_Common_Params(&rInit);
			rImport.Set_Common_Pipes(pPipe);
			rReanalyze.Set_Common_Params(&rInit);
			rReanalyze.Set_Common_Pipes(pPipe);
			rReview.Set_Common_Params(&rInit);
			rReview.Set_Common_Pipes(pPipe);



			//////////////////////////////////////////////////////////////////////////////////////////////////
			// Begin : Determine whether normal or abnormal operations and begin accordingly
			//////////////////////////////////////////////////////////////////////////////////////////////////
			// If no command line parameters or the first parameter is TITLE, then set the current task to the start mode listed in the ini
			// The title mode is a special mode, if it is on the command line then just run in normal mode.
			// This allows a user to display the title inspection info screen without getting into the INI file
			if ((uiNum_Args == 1) || // Command line arguments do not exist, begin normally
				((uiNum_Args == 2) && ((_strnicmp(szBEGIN_TITLE, ppszCommand_Line_Args[1], strlen(szBEGIN_TITLE)) == 0) ||
				(_strnicmp(szBEGIN_INSPECTION, ppszCommand_Line_Args[1], strlen(szBEGIN_INSPECTION)) == 0))))
			{
				if ((uiNum_Args == 2) && ((_strnicmp(szBEGIN_TITLE, ppszCommand_Line_Args[1], strlen(szBEGIN_TITLE)) == 0) ||
					(_strnicmp(szBEGIN_INSPECTION, ppszCommand_Line_Args[1], strlen(szBEGIN_INSPECTION)) == 0)))
					eCurrent_Task = TASK_TITLE;
				else if (_strnicmp(szBEGIN_INSPECTION, rInit.Get_My_Start_Mode(), strlen(szBEGIN_INSPECTION)) == 0)
					eCurrent_Task = TASK_TITLE;
				else if (_strnicmp(szBEGIN_TITLE, rInit.Get_My_Start_Mode(), strlen(szBEGIN_TITLE)) == 0)
					eCurrent_Task = TASK_TITLE;
				else if (_strnicmp(szBEGIN_SETUP, rInit.Get_My_Start_Mode(), strlen(szBEGIN_SETUP)) == 0)
					eCurrent_Task = TASK_SETUP;
				else if (_strnicmp(szBEGIN_IMPORT, rInit.Get_My_Start_Mode(), strlen(szBEGIN_IMPORT)) == 0)
					eCurrent_Task = TASK_IMPORT;
				else if (_strnicmp(szBEGIN_REANALYZE, rInit.Get_My_Start_Mode(), strlen(szBEGIN_REANALYZE)) == 0)
					eCurrent_Task = TASK_REANALYZE;
				else if (_strnicmp(szBEGIN_REVIEW, rInit.Get_My_Start_Mode(), strlen(szBEGIN_REVIEW)) == 0)
					eCurrent_Task = TASK_REVIEW;
				else // Default to title
					eCurrent_Task = TASK_TITLE;

				// Begin under normal operations
				Review_Tool_Begin_Normal();
			}

			else // Command line arguments exist, likely to begin abnormally
			{ 
				bStart_Mode_Found = FALSE;

				//NOTE: in the code below, the string comparison process starts with the second character
				//in the command argument since the first character in a command line argument is a "/".  

				// Initialize parameters according to the command line
				for (ctArg_Num = 1; ctArg_Num < uiNum_Args; ctArg_Num++)
				{
					if (_strnicmp(ppszCommand_Line_Args[ctArg_Num], szBEGIN_SETUP, strlen(szBEGIN_SETUP)) == 0)
					{
						if (!bStart_Mode_Found) 
						{
							eCurrent_Task = TASK_SETUP_ONLY;
							bStart_Mode_Found = TRUE;
						}
					}
					else if (_strnicmp(ppszCommand_Line_Args[ctArg_Num], szBEGIN_IMPORT, strlen(szBEGIN_IMPORT)) == 0)
					{
						if (!bStart_Mode_Found) 
						{
							eCurrent_Task = TASK_IMPORT_ONLY;
							bStart_Mode_Found = TRUE;
						}
					}
					else if (_strnicmp(ppszCommand_Line_Args[ctArg_Num], szBEGIN_REANALYZE, strlen(szBEGIN_REANALYZE)) == 0)
					{
						if (!bStart_Mode_Found) 
						{
							eCurrent_Task = TASK_REANALYZE_ONLY;
							bStart_Mode_Found = TRUE;
						}
					}
					else if (_strnicmp(ppszCommand_Line_Args[ctArg_Num], szBEGIN_REVIEW, strlen(szBEGIN_REVIEW)) == 0)
					{
						if (!bStart_Mode_Found)
						{
							eCurrent_Task = TASK_REVIEW_ONLY;
							bStart_Mode_Found = TRUE;
						}
					}
					else if (_strnicmp(ppszCommand_Line_Args[ctArg_Num], szBEGIN_SR, strlen(szBEGIN_SR)) == 0)
					{
						if (!bStart_Mode_Found)
						{
							eCurrent_Task = TASK_REVIEW_ONLY;
							bStart_Mode_Found = TRUE;
						}
					}
					// The skip running message command line parameter
					else if (_strnicmp(ppszCommand_Line_Args[ctArg_Num], szSKIP_RUNNING_MSG, strlen(szSKIP_RUNNING_MSG)) == 0) 
					{
						// Do nothing, not used here
					}
					else // User wants to immediately connect to a specific tool
					{
						bTool_Found = FALSE;
						for (ctIndex = 0; ctIndex < rInit.Get_Tools_Num(); ctIndex++) 
						{
							CString Temp = rInit.Get_Pipe_Other_Name(ctIndex);
							int StrLen = strlen(rInit.Get_Pipe_Other_Name(ctIndex));
							//Compare strings to determine argument.  Skip the "/" in the argument.  i.e. /RAD  would be the argument.  Look only at RAD.
							if (_strnicmp(ppszCommand_Line_Args[ctArg_Num], rInit.Get_Pipe_Other_Name(ctIndex), StrLen) == 0)
							{
								if (rInit.Get_Tool_Installed(ctIndex))
								{
									// Connect to the tool
									if (!pPipe->Connect(rInit.Get_Pipe_Other_Name(ctIndex))) 
									{
										//kdm								if (pTitle) pTitle->RT_Title_Splash_Close();
										Rev_Tool_Error(uiREV_TOOL_ERROR_PIPE_OPEN_FAIL, rInit.Get_Tool_Name(ctIndex));
									}
									// Let tool know that you have begun
									else if (RTPipes::FAIL == pPipe->Send(rInit.Get_Pipe_Other_Name(ctIndex), FALSE, szPIPE_STARTED, rInit.Get_Pipe_My_Name(), "")) 
									{
										//kdm								if (pTitle) pTitle->RT_Title_Splash_Close();
										Rev_Tool_Error(uiREV_TOOL_ERROR_PIPE, rInit.Get_Tool_Name(ctIndex));
									}
									bTool_Found = TRUE;
								}
								else 
								{
									//kdm							if (pTitle) pTitle->RT_Title_Splash_Close();
									Rev_Tool_Error(uiREV_TOOL_ERROR_NOT_INSTALLED, rInit.Get_Tool_Name(ctIndex));
								}
							}
						}
						// Unknown command line parameter
						if (!bTool_Found) 
						{
							//kdm					if (pTitle) pTitle->RT_Title_Splash_Close();
							Rev_Tool_Error(uiREV_TOOL_ERROR_INVALID_COMMAND_LINE, ppszCommand_Line_Args[ctArg_Num]);
						}
					}
				}

				// Begin under abnormal (single mode) operations if one is set
				if (bStart_Mode_Found)
					Review_Tool_Begin_Abnormal();
				else
					Review_Tool_Begin_Normal();
			}

		}
		catch (...)
		{
		}

		try
		{
			//////////////////////////////////////////////////////////////////////////////////////////////////
			// Cleanup : Close all pipes and perform other cleanup
			//////////////////////////////////////////////////////////////////////////////////////////////////
			// Disconnect from any tools connected to	
			// NOTE: Only used for SETUP or IMPORT.  Review uses exit all or exit only then manually notifies exits and closes pipe
			for (ctIndex = 0; ctIndex < rInit.Get_Tools_Num(); ctIndex++) {
				if (pPipe->Connected(rInit.Get_Pipe_Other_Name(ctIndex))) {
					// If it fails, then not don't worry; nothing can be gained by notifying user
					pPipe->Send(rInit.Get_Pipe_Other_Name(ctIndex), FALSE, szPIPE_EXITED, rInit.Get_Pipe_My_Name(), "");
					pPipe->Close(rInit.Get_Pipe_Other_Name(ctIndex));
					//kdm			FlushEvents();
					bWas_Connected = TRUE;
				}
			}


			// Wait before shutting down to be sure that all messages get serviced
			// otherwise system could hang.
			/*  THIS CODE WAS PUT IN PLACE TO MIMIC THE FUNCTION OF GUI LIB 
			UPPER LAYER VERSION CODE.  HOWEVER, IT SEEMS TO NO LONGER BE
			NEEDED SINCE THIS WAS CONVERTED TO PURE WIN32 AND  MFC.  HOWEVER,
			IT IS LEFT HERE UNTIL THE CODE IS COMPLETELY TESTED AND IS VERIFIED
			TO BE CORRECT.	
			*/
			if (bWas_Connected)
			{
				MSG msg;
				//LONG lIdle = 0;
				DWORD StartTime = GetTickCount();
				while(abs((LONG)(GetTickCount() - StartTime))  < (int)((rInit.Get_Pipe_Timeout_Max() + 1)*1000))
				{
					if(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
						if(msg.message == WM_QUIT || WM_CLOSE)
							break;
						else 
						{
							TRACE0("CALLING PUMPMESSAGE from Review_Tool_Begin\n");
							AfxGetApp()->PumpMessage();
						}
				}
			}

			//kdm		CGUI_App::Wait(rInit.Get_Pipe_Timeout_Max() + 1);
		}
		catch (...)
		{
		}
} 


void Review_Tool::Review_Tool_Begin_Abnormal(void) {

	//char *pszPipe_Request = NULL;
	RTSetup::SETUP_OPERATION eSetup_Operation;
	RTImport::IMPORT_OPERATION eImport_Operation;
	RTReanalyze::REANALYZE_OPERATION eReanalyze_Operation;


	// Do the requested task then exit
	switch (eCurrent_Task) {

		// The title mode is a special mode, if it is on the command line then just run in normal mode.
		// This allows a user to display the title inspection info screen without getting into the INI file
		//case TASK_TITLE_ONLY :

		case TASK_SETUP_ONLY :

			eSetup_Operation = rSetup.RT_Setup_Begin();
			if (eSetup_Operation == RTSetup::SETUP_FAIL)
				Rev_Tool_Error(uiREV_TOOL_ERROR_SETUP_FAIL);

			break;


		case TASK_IMPORT_ONLY :

			eImport_Operation = rImport.RT_Import_Begin(TRUE);
			if ((eImport_Operation != RTImport::IMPORT_EXIT) && (eImport_Operation != RTImport::IMPORT_FAIL)) {
				if ((rInit.Get_IR_Used()) && (pPipe->Connected(rInit.Get_Pipe_Other_Name(rInit.Get_IR_Number())))) {
					if (RTPipes::FAIL == pPipe->Send(rInit.Get_Pipe_Other_Name(rInit.Get_IR_Number()), FALSE, szPIPE_IMPORT_DONE, rInit.Get_Pipe_My_Name(), ""))
						Rev_Tool_Error(uiREV_TOOL_ERROR_PIPE, rInit.Get_Tool_Name(rInit.Get_IR_Number()));
				}
			}
			else if (eImport_Operation == RTImport::IMPORT_FAIL)
				Rev_Tool_Error(uiREV_TOOL_ERROR_IMPORT_FAIL);

			break;


		case TASK_REANALYZE_ONLY :

			eReanalyze_Operation = rReanalyze.RT_Reanalyze_Begin();
			if ((eReanalyze_Operation != RTReanalyze::REANALYZE_EXIT) && (eReanalyze_Operation != RTReanalyze::REANALYZE_FAIL)) {
				if ((rInit.Get_IR_Used()) && (pPipe->Connected(rInit.Get_Pipe_Other_Name(rInit.Get_IR_Number())))) {
					if (RTPipes::FAIL == pPipe->Send(rInit.Get_Pipe_Other_Name(rInit.Get_IR_Number()), FALSE, szPIPE_REANALYZE_DONE, rInit.Get_Pipe_My_Name(), ""))
						Rev_Tool_Error(uiREV_TOOL_ERROR_PIPE, rInit.Get_Tool_Name(rInit.Get_IR_Number()));
				}
			}
			else if (eReanalyze_Operation == RTReanalyze::REANALYZE_FAIL)
				Rev_Tool_Error(uiREV_TOOL_ERROR_REANALYZE_FAIL);

			break;


		case TASK_REVIEW_ONLY :

			// There is no such thing as only performing a review, other modes could be initiated by the tool
			//   such as reanalyze.
			eCurrent_Task = TASK_REVIEW;
			Review_Tool_Begin_Normal();

			break;

	}

	eCurrent_Task = TASK_EXIT;

}


void Review_Tool::Review_Tool_Begin_Normal(void) {

	RTTitle::START_OPERATION eStart_Operation;
	RTSetup::SETUP_OPERATION eSetup_Operation;
	RTImport::IMPORT_OPERATION eImport_Operation;
	RTReanalyze::REANALYZE_OPERATION eReanalyze_Operation;
	RTReview::REVIEW_OPERATION eReview_Operation;


	// Do the requested task
	while (eCurrent_Task != TASK_EXIT) {

		switch (eCurrent_Task)
		{

			case TASK_TITLE :

				// Allow the app to create itself first - Must always do here since normal ops can happen in any order
				if ((!bApp_Created) && (!pTitle->RT_Title_Create_App(rInit.Get_My_Name()))) {
					Rev_Tool_Error(uiREV_TOOL_ERROR_TITLE_FAIL);
					eCurrent_Task = TASK_EXIT;
				}

				eStart_Operation = pTitle->RT_Title_Begin();
				// Determine how user wants to start
				switch (eStart_Operation)
				{
					case RTTitle::START_TITLE :
						eCurrent_Task = TASK_TITLE;
						break;

					case RTTitle::START_SETUP :
						eCurrent_Task = TASK_IMPORT;
						break;

					case RTTitle::START_IMPORT :					
						eCurrent_Task = TASK_IMPORT;
						break;

					case RTTitle::START_REANALYZE :					
						eCurrent_Task = TASK_REANALYZE;
						break;

					case RTTitle::START_REVIEW :
						eCurrent_Task = TASK_REVIEW;
						break;

					case RTTitle::START_EXIT :
						eCurrent_Task = TASK_EXIT;
						break;

					case RTTitle::START_FAIL :
						Rev_Tool_Error(uiREV_TOOL_ERROR_TITLE_FAIL);
						eCurrent_Task = TASK_EXIT;
						break;
				}
				break;

			case TASK_SETUP :

				// Allow the app to create itself first - Must always do here since normal ops can happen in any order
				if ((!bApp_Created) && (!rSetup.RT_Setup_Create_App(rInit.Get_My_Name()))) {
					Rev_Tool_Error(uiREV_TOOL_ERROR_TITLE_FAIL);
					eCurrent_Task = TASK_EXIT;
				}

				// Determine how user wants to continue
				eSetup_Operation = rSetup.RT_Setup_Begin();
				switch (eSetup_Operation)
				{

					case RTSetup::SETUP_TITLE :
						eCurrent_Task = TASK_TITLE;
						break;

					case RTSetup::SETUP_IMPORT :					
						eCurrent_Task = TASK_IMPORT;
						break;

					case RTSetup::SETUP_REANALYZE :					
						eCurrent_Task = TASK_REANALYZE;
						break;

					case RTSetup::SETUP_REVIEW :
						eCurrent_Task = TASK_REVIEW;
						break;

					case RTSetup::SETUP_EXIT :
						eCurrent_Task = TASK_EXIT;
						break;

					case RTSetup::SETUP_FAIL :
						Rev_Tool_Error(uiREV_TOOL_ERROR_TITLE_FAIL);
						eCurrent_Task = TASK_EXIT;
						break;

				}

				break;

			case TASK_IMPORT :

				// Allow the app to create itself first - Must always do here since normal ops can happen in any order
				if ((!bApp_Created) && (!rImport.RT_Import_Create_App(rInit.Get_My_Name(), FALSE))) {
					Rev_Tool_Error(uiREV_TOOL_ERROR_TITLE_FAIL);
					eCurrent_Task = TASK_EXIT;
				}

				eImport_Operation = rImport.RT_Import_Begin();
				// Determine how user wants to continue
				switch (eImport_Operation) 
				{

				case RTImport::IMPORT_TITLE :
					eCurrent_Task = TASK_TITLE;
					// if canceleed (not moving to review) then reset the inspection info
					pPipe->Set_Messages_On_Hold(TRUE);
					if (!rInit.Reset_Inspection_Info())
						Rev_Tool_Error(uiREV_TOOL_ERROR_OPTIONS_RESET_FAIL);
					pPipe->Set_Messages_On_Hold(FALSE);
					break;

				case RTImport::IMPORT_SETUP :
					eCurrent_Task = TASK_TITLE;
					// if canceled (not moving to review) then reset the inspection info
					pPipe->Set_Messages_On_Hold(TRUE);
					if (!rInit.Reset_Inspection_Info())
						Rev_Tool_Error(uiREV_TOOL_ERROR_OPTIONS_RESET_FAIL);
					pPipe->Set_Messages_On_Hold(FALSE);
					break;

				case RTImport::IMPORT_REANALYZE :
					eCurrent_Task = TASK_REANALYZE;
					// if canceleed (not moving to review) then reset the inspection info
					pPipe->Set_Messages_On_Hold(TRUE);
					if (!rInit.Reset_Inspection_Info())
						Rev_Tool_Error(uiREV_TOOL_ERROR_OPTIONS_RESET_FAIL);
					pPipe->Set_Messages_On_Hold(FALSE);
					break;

				case RTImport::IMPORT_REVIEW :
					eCurrent_Task = TASK_REVIEW;
					// if successful (moving to review) then save the new inspection info
					pPipe->Set_Messages_On_Hold(TRUE);
					if (!rInit.Save_Inspection_Info())
						Rev_Tool_Error(uiREV_TOOL_ERROR_OPTIONS_SAVE_FAIL);
					pPipe->Set_Messages_On_Hold(FALSE);
					break;

				case RTImport::IMPORT_EXIT :
					eCurrent_Task = TASK_EXIT;
					break;

				case RTImport::IMPORT_FAIL :
					Rev_Tool_Error(uiREV_TOOL_ERROR_IMPORT_FAIL);
					eCurrent_Task = TASK_EXIT;
					break;

				}
				break;

			case TASK_REANALYZE :

				// Allow the app to create itself first - Must always do here since normal ops can happen in any order
				if ((!bApp_Created) && (!rReanalyze.RT_Reanalyze_Create_App(rInit.Get_My_Name()))) {
					Rev_Tool_Error(uiREV_TOOL_ERROR_TITLE_FAIL);
					eCurrent_Task = TASK_EXIT;
				}

				eReanalyze_Operation = rReanalyze.RT_Reanalyze_Begin();
				// Determine how user wants to continue
				switch (eReanalyze_Operation) 
				{

				case RTReanalyze::REANALYZE_TITLE :
					eCurrent_Task = TASK_TITLE;
					// if canceled (not moving to review) then reset the inspection info
					if (!rInit.Reset_Inspection_Info())
						Rev_Tool_Error(uiREV_TOOL_ERROR_OPTIONS_RESET_FAIL);
					break;

				case RTReanalyze::REANALYZE_SETUP :					
					eCurrent_Task = TASK_TITLE;
					// if canceled (not moving to review) then reset the inspection info
					if (!rInit.Reset_Inspection_Info())
						Rev_Tool_Error(uiREV_TOOL_ERROR_OPTIONS_RESET_FAIL);
					break;

				case RTReanalyze::REANALYZE_IMPORT :
					eCurrent_Task = TASK_IMPORT;
					// if canceled (not moving to review) then reset the inspection info
					pPipe->Set_Messages_On_Hold(TRUE);
					if (!rInit.Reset_Inspection_Info())
						Rev_Tool_Error(uiREV_TOOL_ERROR_OPTIONS_RESET_FAIL);
					pPipe->Set_Messages_On_Hold(FALSE);
					break;

				case RTReanalyze::REANALYZE_REVIEW :
					eCurrent_Task = TASK_REVIEW;
					// if successful (moving to review) then save the new inspection info
					pPipe->Set_Messages_On_Hold(TRUE);
					if (!rInit.Save_Inspection_Info())
						Rev_Tool_Error(uiREV_TOOL_ERROR_OPTIONS_SAVE_FAIL);
					pPipe->Set_Messages_On_Hold(FALSE);
					break;

				case RTReanalyze::REANALYZE_EXIT :
					eCurrent_Task = TASK_EXIT;
					break;

				case RTReanalyze::REANALYZE_FAIL :
					Rev_Tool_Error(uiREV_TOOL_ERROR_REANALYZE_FAIL);
					eCurrent_Task = TASK_EXIT;
					break;

				}
				break;

			case TASK_REVIEW :

				// Allow the app to create itself first - Must always do here since normal ops can happen in any order
				if ((!bApp_Created) && (!rReview.RT_Review_Create_App(rInit.Get_My_Name()))) {
					Rev_Tool_Error(uiREV_TOOL_ERROR_TITLE_FAIL);
					eCurrent_Task = TASK_EXIT;
				}

				eReview_Operation = rReview.RT_Review_Begin();
				// Determine if or how user wants to continue
				switch (eReview_Operation)
				{
					case RTReview::REVIEW_TITLE :
						eCurrent_Task = TASK_TITLE;
						break;

					case RTReview::REVIEW_SETUP :
						eCurrent_Task = TASK_SETUP;
						break;

					case RTReview::REVIEW_IMPORT :
						eCurrent_Task = TASK_IMPORT;
						break;

					case RTReview::REVIEW_REANALYZE :
						eCurrent_Task = TASK_REANALYZE;
						break;

					case RTReview::REVIEW_EXIT :
						eCurrent_Task = TASK_EXIT;
						break;

					case RTReview::REVIEW_FAIL :
						Rev_Tool_Error(uiREV_TOOL_ERROR_REVIEW_FAIL);
						eCurrent_Task = TASK_EXIT;
						break;
				}
				break;

		} // switch current task

	} // while not exit

} 


/////////////////////////////////////////////////////////////////////////////////////
//  Virtual member functions which can be overidden if desired
/////////////////////////////////////////////////////////////////////////////////////

// Used to be notified when a setup done message is received, calls appropriate operation with data
BOOL Review_Tool::RT_Setup_Done(const char *pszPipe_Other_Name) {


	// Don't continue if no tools installed!!!
	if (rInit.Get_Tools_Num() == 0) return FALSE;

	// Verify that the pipe name is a correct pipe name
	// Must compare the return index value against pipe name for that index value because Get_Pipe_Other_Index_Number
	//    will return 0 for safety if it is not found
	if (0 == rInit.Get_Pipe_Other_Index_Number(pszPipe_Other_Name))
		if (0 != strcmp(pszPipe_Other_Name, rInit.Get_Pipe_Other_Name(0)))
			return FALSE;


	return (rSetup.RT_Setup_Done(rInit.Get_Tool_Name(rInit.Get_Pipe_Other_Index_Number(pszPipe_Other_Name))));

}


// Used to be notified when a import done message is received, calls appropriate operation with data
BOOL Review_Tool::RT_Import_Done(const char *pszPipe_Other_Name) {

	// Don't continue if no tools installed!!!
	if (rInit.Get_Tools_Num() == 0) return FALSE;

	// Verify that the pipe name is a correct pipe name
	// Must compare the return index value against pipe name for that index value because Get_Pipe_Other_Index_Number
	//    will return 0 for safety if it is not found
	if (0 == rInit.Get_Pipe_Other_Index_Number(pszPipe_Other_Name))
		if (0 != strcmp(pszPipe_Other_Name, rInit.Get_Pipe_Other_Name(0)))
			return FALSE;


	return (rImport.RT_Import_Done(rInit.Get_Tool_Name(rInit.Get_Pipe_Other_Index_Number(pszPipe_Other_Name))));

}


// Used to be notified when a import done message is received, calls appropriate operation with data
BOOL Review_Tool::RT_Reanalyze_Done(const char *pszPipe_Other_Name) {

	// Don't continue if no tools installed!!!
	if (rInit.Get_Tools_Num() == 0) return FALSE;

	// Verify that the pipe name is a correct pipe name
	// Must compare the return index value against pipe name for that index value because Get_Pipe_Other_Index_Number
	//    will return 0 for safety if it is not found
	if (0 == rInit.Get_Pipe_Other_Index_Number(pszPipe_Other_Name))
		if (0 != strcmp(pszPipe_Other_Name, rInit.Get_Pipe_Other_Name(0)))
			return FALSE;


	return (rReanalyze.RT_Reanalyze_Done(rInit.Get_Tool_Name(rInit.Get_Pipe_Other_Index_Number(pszPipe_Other_Name))));

}


// Used to be notified when a location change message is received, calls appropriate operation with data
BOOL Review_Tool::RT_Location_Change(const char *pszPipe_Other_Name, const char *pszLocation) {

	// Don't continue if no tools installed!!!
	if (rInit.Get_Tools_Num() == 0) return FALSE;

	// Verify that the pipe name is a correct pipe name
	// Must compare the return index value against pipe name for that index value because Get_Pipe_Other_Index_Number
	//    will return 0 for safety if it is not found
	if (0 == rInit.Get_Pipe_Other_Index_Number(pszPipe_Other_Name))
		if (0 != strcmp(pszPipe_Other_Name, rInit.Get_Pipe_Other_Name(0)))
			return FALSE;


	return (rReview.RT_Set_Location(rInit.Get_Tool_Name(rInit.Get_Pipe_Other_Index_Number(pszPipe_Other_Name)), pszLocation));

}


// Used to be notified when a data done message is received, calls appropriate operation with data
BOOL Review_Tool::RT_Display_Data(const char *pszPipe_Other_Name, const char *pszTimestamp, BOOL *pbLocation_Failed) {

	// Don't continue if no tools installed!!!
	if (rInit.Get_Tools_Num() == 0) return FALSE;

	// Verify that the pipe name is a correct pipe name
	// Must compare the return index value against pipe name for that index value because Get_Pipe_Other_Index_Number
	//    will return 0 for safety if it is not found
	if (0 == rInit.Get_Pipe_Other_Index_Number(pszPipe_Other_Name))
		if (0 != strcmp(pszPipe_Other_Name, rInit.Get_Pipe_Other_Name(0)))
			return FALSE;


	return (rReview.RT_Display_Data(rInit.Get_Tool_Name(rInit.Get_Pipe_Other_Index_Number(pszPipe_Other_Name)), pszTimestamp, pbLocation_Failed));

}


// Used to be notified when a data area done message is received, calls appropriate operation with data
BOOL Review_Tool::RT_Display_Data_Area(const char *pszPipe_Other_Name, const char *pszStart_Timestamp, const char *pszEnd_Timestamp, BOOL *pbLocation_Failed) {

	// Don't continue if no tools installed!!!
	if (rInit.Get_Tools_Num() == 0) return FALSE;

	// Verify that the pipe name is a correct pipe name
	// Must compare the return index value against pipe name for that index value because Get_Pipe_Other_Index_Number
	//    will return 0 for safety if it is not found
	if (0 == rInit.Get_Pipe_Other_Index_Number(pszPipe_Other_Name))
		if (0 != strcmp(pszPipe_Other_Name, rInit.Get_Pipe_Other_Name(0)))
			return FALSE;


	return (rReview.RT_Display_Data_Area(rInit.Get_Tool_Name(rInit.Get_Pipe_Other_Index_Number(pszPipe_Other_Name)), pszStart_Timestamp, pszEnd_Timestamp, pbLocation_Failed));

}


// Used to be notified when a reanalyze message is received, calls appropriate operation with data
BOOL Review_Tool::RT_Reanalyze_All_Data(void) {

	return (rReview.RT_Reanalyze_All_Data());

}


BOOL Review_Tool::RT_Pipe_Not_Open(const char *pszPipe_Other_Name) 
{

	//kdm	CGStr Message;
	CString Message;
	BOOL bSuccess = FALSE;


	pPipe->Set_Messages_On_Hold(TRUE);

	// Display start tool message if set to
	if (rInit.Get_Data_Match_Display_Connect_Message(rInit.Get_Pipe_Other_Index_Number(pszPipe_Other_Name))) {
		Message = rInit.Get_My_Name();
		Message += " has not yet been set up to work with ";
		Message += rInit.Get_Tool_Name(rInit.Get_Pipe_Other_Index_Number(pszPipe_Other_Name));
		Message += ".  Would you like to connect the two?";
		//kdm		if (GUI_YES == GUI_MsgBox(Message, GUI_ICON_WARN, GUI_YESNO))
		if (IDYES == MessageBox(NULL, Message, "Warning Message", MB_ICONEXCLAMATION| MB_YESNO))
			bSuccess = RT_Start_Tool(rInit.Get_Tool_Name(rInit.Get_Pipe_Other_Index_Number(pszPipe_Other_Name)));
	}
	// Don't display start tool message
	else {
		bSuccess = RT_Start_Tool(rInit.Get_Tool_Name(rInit.Get_Pipe_Other_Index_Number(pszPipe_Other_Name)));
	}

	pPipe->Set_Messages_On_Hold(FALSE);


	return bSuccess;

}


BOOL Review_Tool::RT_Start_Tool(const char *pszTool) {

	char szPath[201], *pszExec_Name, *pszPipe_Other_Name;
	BOOL bSuccess = FALSE;
	//kdm	CGStr Exe_Name, Command_Line, Message;
	CString Exe_Name, Command_Line, Message;
	STARTUPINFO Startup_Information;
	PROCESS_INFORMATION Process_Information;
	char *pszFinal_Command_Line_Parameters = NULL;
	UINT ctIndex, ctConnect_Try, ctTotal_Connect_Tries;
	//kdm	CGUI_Dlg Start_Tool_Box(IDD_RT_START_TOOL, NULL, DLG_POSN_NO_SAVE);
	ULStartToolMsgDlg* StartToolDlg = NULL;

	// GUI.LIB fix:  GUI_Exec will never set the working directory of an application
	//               Fixed by calling WIN32 CreateProcess


	// Be sure that the tool is installed
	if (!rInit.Get_Tool_Installed(rInit.Get_Tool_Index_Number(pszTool))) {
		Rev_Tool_Error(uiREV_TOOL_ERROR_NOT_INSTALLED, pszTool);
		return FALSE;
	}


	// Determine the full path of the tool to start
	// NEED TO HAVE - NO DRIVE LETTER MEANS THAT THE WORKING DIRECTORY WILL NOT BE SET!!!
	if (!GetFullPathName(rInit.Get_Tool_Exec_Name(rInit.Get_Tool_Index_Number(pszTool)), 200, szPath, &pszExec_Name)) {
		Rev_Tool_Error(uiREV_TOOL_ERROR_CANT_START_TOOL, pszTool);
		return FALSE;
	}
	// Save the exec name and full path for the exec name
	// This is needed so that the convert can be found anywhere and the executable name gets removed below
	Exe_Name = szPath;

	//	CString DebugMsg;
	//	DebugMsg.Format("DEBUG MSG: Attempting to communicate with %s", Exe_Name);
	//	MessageBox(NULL, DebugMsg, "DEBUG MSG", MB_OK);

	// If no extension exists, add .EXE
	if (!strchr(pszExec_Name, '.'))
		Exe_Name += ".EXE";
	// Remove the executable from the path - should ALWAYS be added from above
	// NOTE: Extra + 1 is to keep the last \ because GUI.LIB expects it there
	szPath[strlen(szPath) - strlen(pszExec_Name) - 1 + 1] = NULL;


	// Use this windows startup info as the default
	GetStartupInfo(&Startup_Information);


	// Create the command line paramters
	// NOTE: Command line must contain full app as first parameter otherwise it replaces the first with it
	// NOTE: Don't use /, for some reason it uses spaces in path to separate into two parameters
	Command_Line = "\"";
	//KDM	Command_Line += Exe_Name.Get_sz();
	Command_Line += Exe_Name;
	Command_Line += "\" ";
	Command_Line += " /";
	Command_Line += rInit.Get_Pipe_My_Name();
	Command_Line += " /";
	for (ctIndex = 0; ctIndex < rInit.Get_Tools_Num(); ctIndex++)
		if (pPipe->Connected(rInit.Get_Pipe_Other_Name(ctIndex))) {
			Command_Line += rInit.Get_Pipe_Other_Name(ctIndex);
			Command_Line += " /";
		}
		Command_Line += szSKIP_RUNNING_MSG;
		Command_Line += " /";
		Command_Line += szBEGIN_REVIEW;


		bSuccess = FALSE;
		// Must create a char * like this because it can be changed by CreateProcess
		//KDM	if ((!Command_Line.Create_sz(&pszFinal_Command_Line_Parameters)) ||
		//KDM		(!CreateProcess(Exe_Name.Get_sz(), pszFinal_Command_Line_Parameters, NULL, NULL, FALSE, NULL, NULL, szPath, &Startup_Information, &Process_Information))) {

		pszFinal_Command_Line_Parameters = new char[Command_Line.GetLength() + 1]; //kdm
		strcpy(pszFinal_Command_Line_Parameters, (LPCTSTR) Command_Line);  //kdm

		//	DebugMsg.Format("CREATEPROCESS PARAMETERS: EXE NAME: %s, COMMAND PARAMS: %s", Exe_Name, pszFinal_Command_Line_Parameters);
		//	MessageBox(NULL, DebugMsg, "DEBUG MSG", MB_OK);

		if (pszFinal_Command_Line_Parameters == NULL ||
			(!CreateProcess(Exe_Name, pszFinal_Command_Line_Parameters, NULL, NULL, FALSE, NULL, NULL, szPath, &Startup_Information, &Process_Information))) 
		{
			Rev_Tool_Error(uiREV_TOOL_ERROR_CANT_START_TOOL, pszTool);
			bSuccess = FALSE;
		}
		else  // Success starting the review tool 
		{
			TRACE0("Starting into RT_START_TOOL\n");
			// Close the process AND thread handles - must do both!!!
			CloseHandle(Process_Information.hProcess);
			CloseHandle(Process_Information.hThread);

			// Check to see if the tool has started properly
			if (rInit.Get_Pipe_Other_Name(pszTool, (const char **)&pszPipe_Other_Name)) {

				// Wait the tools pipe timeout to see if it connects
				ctConnect_Try = 0;
				ctTotal_Connect_Tries = 1 + rInit.Get_Pipe_Timeout(rInit.Get_Tool_Index_Number(pszTool));
				while ((!bSuccess) && (ctConnect_Try < ctTotal_Connect_Tries)) {

					//Go into a loop that processes incoming messages, but holds the program at 
					//this execution point for 1 second.  This is done to allow DDE messages to continue
					//coming in to this program while the other review tool process/program starts up
					//and it's client connects to this program's server.
					MSG msg;
					//LONG lIdle = 0;
					DWORD StartTime = GetTickCount();
					while(abs((long)(GetTickCount() - StartTime))  < 1000)
					{
						if(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
							AfxGetApp()->PumpMessage();
					}


					if (pPipe->Connected(pszPipe_Other_Name))
						bSuccess = TRUE;
					ctConnect_Try++;

					// If after two seconds it still has not connected, put up a box to let the user know what is going on
					if (ctConnect_Try == 2)
					{
						//kdm	Start_Tool_Box.SetTitle(rInit.Get_My_Name());
						Message = "Please wait...\n\nStarting ";
						Message += pszTool;
						Message += '.';
						/*					StartToolDlg = new ULStartToolMsgDlg(rInit.Get_My_Name(), Message, uiIcon);
						StartToolDlg->Create(IDD_RT_START_TOOL);
						StartToolDlg->ShowWindow(SW_SHOW | SW_SHOWNORMAL);
						*/
					}
				}

				// Close the wait box if it has been opened
				if (ctConnect_Try >= 2)
					//kdm				Start_Tool_Box.Close();

					// Could have already been started, connect if already started
					if (!bSuccess) 
					{
						// Tell it to connect for every tool I am connected to
						if (!pPipe->Connect(pszPipe_Other_Name))
							Rev_Tool_Error(uiREV_TOOL_ERROR_PIPE_OPEN_FAIL, pszTool);
						else bSuccess = TRUE;
					}
			}
		}

		TRACE0("Exiting RT_START_TOOL\n");

		// Delete the final command line parameters
		if (pszFinal_Command_Line_Parameters) {
			delete [] pszFinal_Command_Line_Parameters;
			pszFinal_Command_Line_Parameters = NULL;
		}


		if(StartToolDlg)
		{
			StartToolDlg->DestroyWindow();
			delete StartToolDlg;
		}
		return bSuccess;

}


// Used to display exit message box when a exit message is received through a pipe, calls current operation exit if yes
BOOL Review_Tool::RT_Receive_Exit(const char * /*pszPipe_Other_Name*/) {

	// Shut down everything
	Review_Tool_Exit();

	return TRUE;

}


/////////////////////////////////////////////////////////////////////
// Access to the common data items across all review tool operations
/////////////////////////////////////////////////////////////////////

const RTInit& Review_Tool::Get_Common_Params(void) {

	return (rInit);

}


const RTPipes& Review_Tool::Get_Common_Pipe(void) {

	return (*pPipe);

}


/////////////////////////////////////////////////////////////////////////////////////
//  Exit procedures
/////////////////////////////////////////////////////////////////////////////////////

void Review_Tool::Review_Tool_Exit(void)
{
	try
	{
		UINT ctIndex;


		// Close all of the pipes now to prevent a send to a closed app
		for (ctIndex = 0; ctIndex < rInit.Get_Tools_Num(); ctIndex++) {
			if (pPipe->Connected(rInit.Get_Pipe_Other_Name(ctIndex))) {
				pPipe->Close(rInit.Get_Pipe_Other_Name(ctIndex));
				bWas_Connected = TRUE;
			}
		}

		// Make sure all of the processes are shut down
		switch (eCurrent_Task) {
			case TASK_TITLE :
				pTitle->RT_Title_Exit();
				break;
			case TASK_SETUP :
			case TASK_SETUP_ONLY :
				rSetup.RT_Setup_Exit();
				break;
			case TASK_IMPORT :
			case TASK_IMPORT_ONLY :
				rImport.RT_Import_Exit();
				break;
			case TASK_REVIEW :
			case TASK_REVIEW_ONLY :
				rReview.RT_Review_Exit();
				break;
		}
	}
	catch (...)
	{
	}
}

/////////////////////////////////////////////////////////////////////////////////////
//  Error handling functions
/////////////////////////////////////////////////////////////////////////////////////

void Review_Tool::Rev_Tool_Error(uiREV_TOOL_ERROR uiError, const char *pszError_Parameter_1, const char *pszError_Parameter_2) 
{

	//kdm	CGStr Message;
	CString Message;


	switch(uiError) { 
		case uiREV_TOOL_ERROR_NO_MEMORY:
			RTError.Error(uiRT_ERROR_NO_MEMORY);
			break;
		case uiREV_TOOL_ERROR_INVALID_COMMAND_LINE:
			if (pPipe) pPipe->Set_Messages_On_Hold(TRUE);
			if (pszError_Parameter_1) {
				Message = "The command line contains an invalid parameter:";
				Message += "\n\nParameter: \"";
				Message += pszError_Parameter_1;
				Message += "\"";
			}
			else {
				Message = "The command line contains an invalid parameter.";
			}
			//KDM			GUI_MsgBox(Message.Get_sz(), GUI_ICON_ERROR);
			MessageBox(NULL, Message, "Error Message", MB_ICONERROR);
			if (pPipe) pPipe->Set_Messages_On_Hold(FALSE);
			break;
		case uiREV_TOOL_ERROR_PIPE_OPEN_FAIL:
			RTError.Error(uiRT_ERROR_PIPE_OPEN_FAIL, pszError_Parameter_1, rInit.Get_My_Name());
			break;
		case uiREV_TOOL_ERROR_PIPE:
			RTError.Error(uiRT_ERROR_PIPE, pszError_Parameter_1);
			break;
		case uiREV_TOOL_ERROR_TITLE_FAIL:
			if (pPipe) pPipe->Set_Messages_On_Hold(TRUE);
			//kdm			GUI_MsgBox("An unrecoverable error occurred during the inspection information title screen.", GUI_ICON_ERROR);
			MessageBox(NULL, "An unrecoverable error occurred during the inspection information title screen.", "Error Message", MB_ICONERROR);
			if (pPipe) pPipe->Set_Messages_On_Hold(FALSE);
			break;
		case uiREV_TOOL_ERROR_SETUP_FAIL:
			if (pPipe) pPipe->Set_Messages_On_Hold(TRUE);
			//kdm			GUI_MsgBox("An unrecoverable error occurred during the setup process.", GUI_ICON_ERROR);
			MessageBox(NULL, "An unrecoverable error occurred during the setup process.","Error Message",  MB_ICONERROR);
			if (pPipe) pPipe->Set_Messages_On_Hold(FALSE);
			break;
		case uiREV_TOOL_ERROR_IMPORT_FAIL:
			if (pPipe) pPipe->Set_Messages_On_Hold(TRUE);
			//kdm			GUI_MsgBox("An unrecoverable error occurred during the import process.", GUI_ICON_ERROR);
			MessageBox(NULL, "An unrecoverable error occurred during the import process.", "Error Message", MB_ICONERROR);
			if (pPipe) pPipe->Set_Messages_On_Hold(FALSE);
			break;
		case uiREV_TOOL_ERROR_REANALYZE_FAIL:
			if (pPipe) pPipe->Set_Messages_On_Hold(TRUE);
			//kdm			GUI_MsgBox("An unrecoverable error occurred during the reanalyze process.", GUI_ICON_ERROR);
			MessageBox(NULL, "An unrecoverable error occurred during the reanalyze process.", "Error Message", MB_ICONERROR);
			if (pPipe) pPipe->Set_Messages_On_Hold(FALSE);
			break;
		case uiREV_TOOL_ERROR_REVIEW_FAIL:
			if (pPipe) pPipe->Set_Messages_On_Hold(TRUE);
			//kdm			GUI_MsgBox("An unrecoverable error occurred during the review process.", GUI_ICON_ERROR);
			MessageBox(NULL, "An unrecoverable error occurred during the review process.", "Error Message", MB_ICONERROR);
			if (pPipe) pPipe->Set_Messages_On_Hold(FALSE);
			break;                          
		case uiREV_TOOL_ERROR_NOT_INSTALLED:
			RTError.Error(uiRT_ERROR_NOT_INSTALLED, pszError_Parameter_1);
			break;
		case uiREV_TOOL_ERROR_CANT_START_TOOL:
			RTError.Error(uiRT_ERROR_CANT_START_TOOL, pszError_Parameter_1);
			break;
		case uiREV_TOOL_ERROR_OPTIONS_SAVE_FAIL:
			RTError.Error(uiRT_ERROR_OPTIONS_SAVE_FAIL);
			break;
		case uiREV_TOOL_ERROR_OPTIONS_RESET_FAIL:
			RTError.Error(uiRT_ERROR_OPTIONS_RESET_FAIL);
			break;
		default:
			RTError.Error(uiError, pszError_Parameter_1, pszError_Parameter_2); 
			break;			
	}

}
