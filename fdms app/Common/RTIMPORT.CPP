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
�	Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
disclaimer. 
�	Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
disclaimer in the documentation and/or other materials provided with the distribution. 
�	Neither the name of Los Alamos National Security, LLC, Los Alamos National Laboratory, LANL, the U.S. Government, 
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

// RTImport.CPP
// Import class definition for the abstract class Review Tool
       
#include <stdafx.h>
#include "RTImport.H"
	

/////////////////////////////////////////////////////////////////////////////////////
//  RTImport constructor, destructor
/////////////////////////////////////////////////////////////////////////////////////

RTImport::RTImport(void) :
	bCommon_Params_Set(FALSE),
	pCommon_Params(NULL),
	bCommon_Pipes_Set(FALSE),
	pCommon_Pipes(NULL)
{}

 
RTImport::~RTImport(void) {}


//////////////////////////////////////////////////////////////////////////////////////////////////////
// Default app creation member function
//////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL RTImport::RT_Import_Create_App(const char * /*pszTool_Name*/, BOOL /*bBypass_Import_Prompt*/) {

	// This should ONLY be called if Review_Tool was told to create the app
	return FALSE;

}


//////////////////////////////////////////////////////////////////////////////////////////////////////
// Only needs to be overidden for tools which need to keep track of when another tool's setup is done
//////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL RTImport::RT_Import_Done(const char * /*pszTool*/) {

	return FALSE;
	
}


/////////////////////////////////////////////////////////////////////
// Access to the common data items across all review tool operations
/////////////////////////////////////////////////////////////////////

BOOL RTImport::Set_Common_Params(RTInit *const pRT_Common_Params) {
    
    pCommon_Params = pRT_Common_Params;
    bCommon_Params_Set = TRUE;
    
    // Also set the common error's
    RTError.Set_My_Name(pCommon_Params->Get_My_Name());

	return bCommon_Params_Set;
	
}


BOOL RTImport::Set_Common_Pipes(RTPipes *const pRT_Commmon_Pipes) {

    pCommon_Pipes = pRT_Commmon_Pipes;
    bCommon_Pipes_Set = TRUE;
    
    // Also set the common error's
    RTError.Set_My_Pipes(pCommon_Pipes);

	return bCommon_Pipes_Set;

}
 

/////////////////////////////////////////////////////////
// Call this procedure to close all open tools and provide proper busy messages
/////////////////////////////////////////////////////////
// Unlike Review, this one requires a bool value returned
BOOL RTImport::RT_Import_Exit_All_Tools(void) {

	UINT ctIndex;
	char *pszResponse = NULL;
	BOOL bSuccess = TRUE;
	
	
	// Tell all of the review tools connected to to exit
	for (ctIndex = 0; ctIndex < pCommon_Params->Get_Tools_Num(); ctIndex++) {
		if (pCommon_Pipes->Connected(pCommon_Params->Get_Pipe_Other_Name(ctIndex))) {
			// Check for a pipe failure
			if ((RTPipes::FAIL == pCommon_Pipes->Send(pCommon_Params->Get_Pipe_Other_Name(ctIndex), &pszResponse, szPIPE_EXIT_ALL, "")) ||
				(pszResponse == NULL)) {
				RT_Import_Error(uiRT_IMPORT_ERROR_PIPE, pCommon_Params->Get_Tool_Name(ctIndex));
				bSuccess = FALSE;
				}
			// Check for a busy message
			else if (strncmp(pszResponse, "BUSY", strlen("BUSY")) == 0) {
				// It is busy, but still needs to close it's pipe, just tell it to close it's pipe
				if (RTPipes::FAIL == pCommon_Pipes->Send(pCommon_Params->Get_Pipe_Other_Name(ctIndex), FALSE, szPIPE_EXITED, pCommon_Params->Get_Pipe_My_Name(), ""))
					RT_Import_Error(uiRT_IMPORT_ERROR_PIPE, pCommon_Params->Get_Tool_Name(ctIndex));
				RT_Import_Error(uiRT_IMPORT_ERROR_BUSY_EXIT, pCommon_Params->Get_Tool_Name(ctIndex));
				bSuccess = FALSE;
				}
			// Failed
			else if (strncmp(pszResponse, szPIPE_EXIT_ALL, strlen(szPIPE_EXIT_ALL)) != 0) {
				RT_Import_Error(uiRT_IMPORT_ERROR_PIPE, pCommon_Params->Get_Tool_Name(ctIndex));
				bSuccess = FALSE;
				}
			// Delete each response
			if (pszResponse) {
				delete [] pszResponse;
				pszResponse = NULL;
				}
			// Close the pipe
			pCommon_Pipes->Close(pCommon_Params->Get_Pipe_Other_Name(ctIndex));
			}
		}


	return bSuccess;

}


// Call this member function to put incoming displaying messages on hold when you are busy 
//    during long operations (Example - loading and saving)
BOOL RTImport::Set_Messages_On_Hold(BOOL bStatus) {
    
    return (pCommon_Pipes->Set_Messages_On_Hold(bStatus));
	
}


//////////////////////////////////////////////////////////////////////////////////////////
// Use the default error messages below except when an error occurs specific to your tool
//////////////////////////////////////////////////////////////////////////////////////////

void RTImport::RT_Import_Error(uiRT_IMPORT_ERROR uiError, const char *pszError_Parameter_1) {

//kdm	CGStr Message;
	CString Message;


	switch(uiError) {
		case uiRT_IMPORT_ERROR_NO_MEMORY :
			RTError.Error(uiRT_ERROR_NO_MEMORY);
			break;
		case uiRT_IMPORT_ERROR_PIPE :
			RTError.Error(uiRT_ERROR_PIPE, pszError_Parameter_1);
			break;
		case uiRT_IMPORT_ERROR_DB_CANT_SAVE :
			RTError.Error(uiRT_ERROR_DB_CANT_SAVE);
			break;
		case uiRT_IMPORT_ERROR_BUSY_EXIT :
			Set_Messages_On_Hold(TRUE);
			Message = pszError_Parameter_1;
			Message += " is busy right now.  You need to exit it manually";
//kdm			GUI_MsgBox(Message.Get_sz(), GUI_ICON_ERROR);
//kdm			MessageBox(NULL, Message.Get_sz(), "Error Message", MB_ICONERROR);
			MessageBox(NULL, Message, "Error Message", MB_ICONERROR);
			Set_Messages_On_Hold(FALSE);
			break;
		default:
			RTError.Error(uiError, pszError_Parameter_1);
			break;			
    	}


}


