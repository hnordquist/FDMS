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
// sbuck@lanl.gov (505)667-5129
// 8/28/96

// PIPE.H
// Class declaration for pipe/messaging objects for the Integrated Review Software
       
       


#ifndef PIPE_H

	#define PIPE_H
   
//kdm 	#include <gui_lib.h>
//kdm 	#include "gui_dde.h" 
	#include <stdlib.h> 

//kdm	#include "gen.h"
#include "ULDDEClient.h"		//kdm
#include "ULDDEServer.h"	//kdm
#include "ULCallBack.h"				//kdm


	// Not meant to be instantiated
	// Must inherit from in order to register member functions to be called
	//	when message received
	class Pipe_CallBack
	{
		public:
			Pipe_CallBack();
			~Pipe_CallBack();
		
		private:
	};


	// Typedefs for message handling member functions
	// Member function which gets called when a string is received and responds with a string when exited
	typedef BOOL (Pipe_CallBack::*Pipe_CB_StringFn)(char *pszRequest, char **pszResponse);
	// Member function which just gets called when a string is received and no response is expected
	typedef BOOL (Pipe_CallBack::*Pipe_CB_CommandFn)(char *pszCommand);


	// Class declaration for Interated Review Station messaging system
	// Instantiate a Pipe objects in modules handling messaging with the appropriate
	//	names, topics, object (this) pointer, and message handling member functions
	// ***NOTE: If more then one pipe object in a single app, then it's Server Name must be the same!!!
//KDM	class Pipe : public CGUI_CallBack, public Pipe_CallBack {
	class Pipe : public ULCallBack, public Pipe_CallBack {

		public:
		
			// Two way pipes
			Pipe(const char *pszMy_Name, const char *pszMy_Topic, const char *pszTheir_Name, const char *pszTheir_Topic,
				Pipe_CallBack *pCB_Obj,
				// Receive string and respond string
				Pipe_CB_StringFn mpString_Function,
				Pipe_CB_CommandFn mpCommand_Function = NULL); 

			// One way pipe enter messages
			Pipe(const char *pszMy_Name, const char *pszMy_Topic, 
				Pipe_CallBack *pCB_Obj,
				// Receive string and respond string
				Pipe_CB_StringFn mpString_Function,
				Pipe_CB_CommandFn mpCommand_Function = NULL); 

			// One way pipe exit messages
			Pipe(const char *Their_Name, const char *Their_Topic);      

			// Pipe dstor
			virtual ~Pipe(void); 

            // Call before ever using the pipe to check that it was created correctly
			BOOL Pipe_Created(void);
						
            // Must call before beginning communications
            // NOTE: Be sure that other end has instantiated their end of the pipe first!
            BOOL Open_Communications(void);
            // NOTE: Disconnects other end.  Use delete if want to disconnect your end.
            BOOL Close_Communications(void);
            
            // Amount of time to wait before giving up on a response
			BOOL Set_Timeout(double dTime_Out);


			// Send a string message and receive response as a string - creates response						
            BOOL Send(const char *pszString_Send, char **pszString_Receive);
			// Send a string message - Will return TRUE if good send AND optional receive same command back check
			//						   Will return FALSE if bad send OR optional receive same command back check
            BOOL Send(const char *pszCommand_Send, BOOL bVerify_Response = FALSE);
            
		
		private:
			
			enum DIRECTIONS {
				ENTER,
				EXIT,
				BOTH
				} Direction;
			
//kdm			CGUI_DDE_Server *pPipe_In;
			ULDDEServer *pPipe_In;
 //kdm           CGUI_DDE_Client *pPipe_Out;
            ULDDEClient *pPipe_Out;
			char *pszPipe_My_Name, *pszPipe_Their_Name;
			char *pszPipe_My_Topic, *pszPipe_Their_Topic;
			
			BOOL bCommand_Function;
			BOOL bPipe_Created, bPipe_Open;
			double dPipe_Time_Out;
			
			Pipe_CallBack *mpCB_Obj;
			Pipe_CB_StringFn mpCBO_StringFn;
			Pipe_CB_CommandFn mpCBO_CommandFn;
			
//kdm			BOOL Receive_String(CGStr ItemName, CGStr *pReturnData);
			bool Receive_String(CString ItemName, CString *pReturnData);
//kdm			BOOL Receive_Command(CGStr Cmd);
 			bool Receive_Command(CString Cmd);

		};
		
#endif

		
