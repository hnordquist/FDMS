// Steven Edward Buck
// Los Alamos National Laboratory
// NIS-5
// 3/10/97

// Rev_Tool.CPP
// Main source file for the abstract class Review Tool

#include <stdafx.h>
#include "RTError.H"   



/////////////////////////////////////////////////////////////////////////////////////
//  Review_Tool_Error constructor:  Setup all intial parameters, and the pipe
/////////////////////////////////////////////////////////////////////////////////////

Review_Tool_Error::Review_Tool_Error(const char *pszMy_Name, RTPipes *pRTPipes) :
	pszName(NULL),
	pPipes(pRTPipes)
	{
	
	if ((pszName = new char [strlen(pszMy_Name) + 1]) != NULL)
		strcpy(pszName, pszMy_Name);
	else
	    Error(uiRT_ERROR_NO_MEMORY);
	    
}

	
Review_Tool_Error::Review_Tool_Error(void) :
	pszName(NULL),
	pPipes(NULL)
	{}

	
Review_Tool_Error::~Review_Tool_Error(void) {

	if (pszName)
		delete [] pszName;
		
}
    
    
/////////////////////////////////////////////////////////////////////////////////////
//  Setting member functions
/////////////////////////////////////////////////////////////////////////////////////

BOOL Review_Tool_Error::Set_My_Name(const char *pszMy_Name) {

	if (pszName) delete [] pszName;
	if ((pszName = new char [strlen(pszMy_Name) + 1]) != NULL) {
		strcpy(pszName, pszMy_Name);
		return TRUE;
		}
	else
	    Error(uiRT_ERROR_NO_MEMORY);
	    
	    
	return FALSE;
	
}


BOOL Review_Tool_Error::Set_My_Pipes(RTPipes *pRT_Pipes) {

	pPipes = pRT_Pipes;

	    
	return TRUE;
	
}


/////////////////////////////////////////////////////////////////////////////////////
//  Error handling functions
/////////////////////////////////////////////////////////////////////////////////////

void Review_Tool_Error::Error(uiRT_ERROR uiError, const char *pszError_Parameter_1, const char *pszError_Parameter_2) {

//kdm	CGStr Message;
	CString ErrorMessage;
	
	
	switch(uiError) { 
		case uiRT_ERROR_NO_MEMORY :
			if (pPipes) pPipes->Set_Messages_On_Hold(TRUE);
//kdm			GUI_MsgBox("Your system has run out of free memory.  Please try to increase the available memory and then begin again.", GUI_ICON_ERROR);
			MessageBox(NULL, "Your system has run out of free memory.  Please try to increase the available memory and then begin again.", "Error Message", MB_ICONERROR);
			if (pPipes) pPipes->Set_Messages_On_Hold(FALSE);
			break;
		case uiRT_ERROR_NOT_INSTALLED:
			if (pPipes) pPipes->Set_Messages_On_Hold(TRUE);
			if (pszError_Parameter_1 == NULL)
				ErrorMessage += "The review tool";
			else
				ErrorMessage += pszError_Parameter_1;
			ErrorMessage += " is not installed on this system.";
//kdm			GUI_MsgBox(Message, GUI_ICON_ERROR);
			MessageBox(NULL, ErrorMessage, "Error Message", MB_ICONERROR);
			if (pPipes) pPipes->Set_Messages_On_Hold(FALSE);
			break;
		case uiRT_ERROR_INIT_LOAD :
			if (pPipes) pPipes->Set_Messages_On_Hold(TRUE);
			ErrorMessage = "";
			if (pszError_Parameter_2) {
				ErrorMessage += "An error occurred while trying to load the initialization file.  However, application execution was able to continue.";
				ErrorMessage += "\n\nParameter: \"";
				ErrorMessage += pszError_Parameter_2;
				ErrorMessage += "\"";
				if (pszError_Parameter_2)
					ErrorMessage += "\n\n";
				}
			if (pszError_Parameter_1) {
				ErrorMessage += "An error occurred while trying to load the initialization file.  Application execution was not able to continue.";
				ErrorMessage += "\n\nParameter: \"";
				ErrorMessage += pszError_Parameter_1;
				ErrorMessage += "\"";
				}
			if (!pszError_Parameter_1 && !pszError_Parameter_2) {
				ErrorMessage += "An unknown error occurred while trying to load the initialization file.  Application execution was not able to continue.";
				}
//kdm			GUI_MsgBox(ErrorMessage.Get_sz(), GUI_ICON_ERROR);
//kdm			MessageBox(NULL, ErrorMessage.Get_sz(), "Error ErrorMessage", MB_ICONERROR);
			MessageBox(NULL, ErrorMessage, "Error Message", MB_ICONERROR);
			if (pPipes) pPipes->Set_Messages_On_Hold(FALSE);
			break;
		case uiRT_ERROR_CANT_START_TOOL:
			if (pPipes) pPipes->Set_Messages_On_Hold(TRUE);
			if (pszError_Parameter_1 == NULL)
				ErrorMessage += "The review tool";
			else
				ErrorMessage += pszError_Parameter_1;
			ErrorMessage += " could not be found.  The software may have to be reinstalled.";
//kdm			GUI_MsgBox(ErrorMessage, GUI_ICON_ERROR);
			MessageBox(NULL, ErrorMessage, "Error Message", MB_ICONERROR);
			if (pPipes) pPipes->Set_Messages_On_Hold(FALSE);
			break;
		case uiRT_ERROR_PIPE_OPEN_FAIL:
			if (pPipes) pPipes->Set_Messages_On_Hold(TRUE);
			if (pszError_Parameter_2 == NULL)
				ErrorMessage += "Could not create a communication path to ";
			else {
				ErrorMessage = pszError_Parameter_2;
				ErrorMessage += " could not create a communication path to ";
				}
			if (pszError_Parameter_1 == NULL)
				ErrorMessage += "one of the tools";
			else
				ErrorMessage += pszError_Parameter_1;
			ErrorMessage += " and will not be able to send information to it.";
//kdm			GUI_MsgBox(ErrorMessage, GUI_ICON_ERROR);
			MessageBox(NULL, (LPCTSTR)ErrorMessage, "Error Message", MB_ICONERROR);
			if (pPipes) pPipes->Set_Messages_On_Hold(FALSE);
			break;
		case uiRT_ERROR_PIPE :
			if (pPipes) pPipes->Set_Messages_On_Hold(TRUE);
			ErrorMessage = "An error occurred while trying to communicate with ";
			if (pszError_Parameter_1 == NULL)
				ErrorMessage += "one of the tools";
			else {
				ErrorMessage += pszError_Parameter_1;
				ErrorMessage += ".";
				}
//kdm			GUI_MsgBox(ErrorMessage, GUI_ICON_ERROR);
			MessageBox(NULL, ErrorMessage, "Error Message", MB_ICONERROR);
			if (pPipes) pPipes->Set_Messages_On_Hold(FALSE);
			break;
		case uiRT_ERROR_PIPE_DEAD :
			if (pPipes) pPipes->Set_Messages_On_Hold(TRUE);
			ErrorMessage = "An unrecoverable error occurred while sending a message to ";
			if (pszError_Parameter_1 == NULL)
				ErrorMessage += "one of the tools";
			else {
				ErrorMessage += pszError_Parameter_1;
				ErrorMessage += ".";
				}
//kdm			GUI_MsgBox(ErrorMessage, GUI_ICON_ERROR);
			MessageBox(NULL, ErrorMessage, "Error Message", MB_ICONERROR);
			if (pPipes) pPipes->Set_Messages_On_Hold(FALSE);
			break;
		case uiRT_ERROR_OPTIONS_SAVE_FAIL :
			if (pPipes) pPipes->Set_Messages_On_Hold(TRUE);
//kdm			GUI_MsgBox("An error occurred while trying to save the default parameters.", GUI_ICON_ERROR);
			MessageBox(NULL, "An error occurred while trying to save the default parameters.","Error ErrorMessage",  MB_ICONERROR);
			if (pPipes) pPipes->Set_Messages_On_Hold(FALSE);
			break;
		case uiRT_ERROR_OPTIONS_RESET_FAIL :
			if (pPipes) pPipes->Set_Messages_On_Hold(TRUE);
//kdm			GUI_MsgBox("An error occurred while trying to reset the parameters.", GUI_ICON_ERROR);
			MessageBox(NULL, "An error occurred while trying to reset the parameters.", "Error ErrorMessage", MB_ICONERROR);
			if (pPipes) pPipes->Set_Messages_On_Hold(FALSE);
			break;
		case uiRT_ERROR_DB_CANT_SAVE :
			if (pPipes) pPipes->Set_Messages_On_Hold(TRUE);
			if (pszError_Parameter_1) {
				ErrorMessage = "An error occurred while trying to save the database: \"";
				ErrorMessage += pszError_Parameter_1;
				ErrorMessage += "\".  ";
				}
			else ErrorMessage = "An error occurred while trying to save the database.  ";
			ErrorMessage += "The database could not be saved.";
//kdm			GUI_MsgBox(ErrorMessage.Get_sz(), GUI_ICON_WARN);
//kdm			MessageBox(NULL, ErrorMessage.Get_sz(),"Warning ErrorMessage",  MB_ICONEXCLAMATION);
			MessageBox(NULL, ErrorMessage,"Warning ErrorMessage",  MB_ICONEXCLAMATION);

			if (pPipes) pPipes->Set_Messages_On_Hold(FALSE);
			break;
		case uiRT_ERROR_INSPEC_CHANGE :
			if (pPipes) pPipes->Set_Messages_On_Hold(TRUE);
//kdm			GUI_MsgBox("An error occurred while trying to change the inspection information.", GUI_ICON_WARN);
			MessageBox(NULL, "An error occurred while trying to change the inspection information.", "Warning ErrorMessage", MB_ICONEXCLAMATION);
			if (pPipes) pPipes->Set_Messages_On_Hold(FALSE);
			break;
		case uiRT_ERROR_OVERWRITE_RAWDATA :
			if (pPipes) pPipes->Set_Messages_On_Hold(TRUE);
//KDM			GUI_MsgBox("You can not overwrite the raw data.", GUI_ICON_ERROR);
			MessageBox(NULL, "You can not overwrite the raw data.", "Error ErrorMessage", MB_ICONERROR);
			if (pPipes) pPipes->Set_Messages_On_Hold(FALSE);
			break;
		case uiRT_ERROR_COMMON_INSPEC_LOAD :
			if (pPipes) pPipes->Set_Messages_On_Hold(TRUE);
			ErrorMessage = "An error occurred while loading the common inspection information.";
			if (pszError_Parameter_1) {
				ErrorMessage += "\n\nParameter: \"";
				ErrorMessage += pszError_Parameter_1;
				ErrorMessage += "\"";
				}
//KDM			GUI_MsgBox(ErrorMessage.Get_sz(), GUI_ICON_ERROR);
//kdm			MessageBox(NULL, ErrorMessage.Get_sz(),"Error ErrorMessage",  MB_ICONERROR);
			MessageBox(NULL, ErrorMessage,"Error Message",  MB_ICONERROR);
			if (pPipes) pPipes->Set_Messages_On_Hold(FALSE);
			break;
		case uiRT_ERROR_COMMON_INSPEC_SAVE :
			if (pPipes) pPipes->Set_Messages_On_Hold(TRUE);
//kdm			GUI_MsgBox("An error occurred while saving the common inspection information.", GUI_ICON_ERROR);
			MessageBox(NULL, "An error occurred while saving the common inspection information.", "Error ErrorMessage", MB_ICONERROR);
			if (pPipes) pPipes->Set_Messages_On_Hold(FALSE);
			break;
		default: 
			if (pPipes) pPipes->Set_Messages_On_Hold(TRUE);
//KDM			GUI_MsgBox("An unrecognizable error has occurred.", GUI_ICON_WARN);
			MessageBox(NULL, "An unrecognizable error has occurred.", "Warning ErrorMessage", MB_ICONEXCLAMATION);
			if (pPipes) pPipes->Set_Messages_On_Hold(FALSE);
			break;			
    	}

}
