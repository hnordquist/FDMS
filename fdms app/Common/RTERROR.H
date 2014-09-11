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
// 3/24/97

// RTError.H
// Class definition for the abstract class Review Tool Error
// Common error messages across all Review Tools
       
       


#ifndef RTERROR_H

	#define RTERROR_H

//kdm	#include <gui_lib.h>
	#include <stdlib.h> 

//kdm	#include "SEBBoxes.H"
	#include "RTPipe.H"
	
//kdm	#include "gen.h"


	#define szRTERROR_VERSION			__TIMESTAMP__


	#define uiRT_ERROR								UINT
	#define uiRT_ERROR_NO_MEMORY					1
	#define uiRT_ERROR_NOT_INSTALLED				2		// Requires tool name to be passed
	#define uiRT_ERROR_INIT_LOAD					3		// Requires param errored for first and param warning for second, NULL accepted
	#define uiRT_ERROR_CANT_START_TOOL				4		// Requires tool name to be passed
	#define uiRT_ERROR_PIPE_OPEN_FAIL				5		// Requires tool name and your name to be passed
	#define uiRT_ERROR_PIPE							6		// Requires tool name to be passed
	#define uiRT_ERROR_PIPE_DEAD					15		// Requires tool name to be passed
	#define uiRT_ERROR_OPTIONS_SAVE_FAIL			20
	#define uiRT_ERROR_OPTIONS_RESET_FAIL			21
	#define uiRT_ERROR_DB_CANT_SAVE					35		// Requires DB name to be passed
	#define uiRT_ERROR_INSPEC_CHANGE				40
	#define uiRT_ERROR_OVERWRITE_RAWDATA		    50
	#define uiRT_ERROR_COMMON_INSPEC_LOAD			60		// Requires label warning name to be passed as first parameter
	#define uiRT_ERROR_COMMON_INSPEC_SAVE			61

	
	

//kdm	class Review_Tool_Error : protected CGUI_CallBack {
	class Review_Tool_Error : protected ULCallBack {

		public :
		
		    Review_Tool_Error(const char *pszMy_Name, RTPipes *pRTPipes = NULL);
		    Review_Tool_Error(void);
		    ~Review_Tool_Error(void);

			virtual BOOL Set_My_Name(const char *pszMy_Name);
			virtual BOOL Set_My_Pipes(RTPipes *pRT_Pipes);
		    
			virtual void Error(uiRT_ERROR uiError, const char *pszError_Parameter_1 = NULL, const char *pszError_Parameter_2 = NULL);
            
            
        private :
        
        	char *pszName;
			RTPipes *pPipes;
        	
		};
		
#endif


		
