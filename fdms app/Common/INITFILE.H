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

// $History: INITFILE.H $ 
// 
// *****************  Version 2  *****************
// User: Longo        Date: 9/17/04    Time: 3:34p
// Updated in $/FDMS/FDMS Application/FDMSNET/Common
// bogus changes, having no effect
// Class declaration for working with initialization files


#ifndef INITFILE_H

	#define INITFILE_H

	//#include <stdlib.h> 
	#include <fstream>
	#include <iostream>
	#include <strstream>
	//#include <vector>

	//#include <string.h>
	//#include <stdio.h>
	//#include <limits.h>
	//#include <windows.h>

using namespace std;

#include "ULSupport.h"
	
//kdm	#include "gen.h"

	#include "SEBMacro.H"



                     
class Initialization_File : public fstream { // , protected strstreambuf {

		public:
		
			Initialization_File(const char *pszInitialization_Name, BOOL bWrite_Access = TRUE); 
			virtual ~Initialization_File(void);
			                        

			// Call to check if the file was open properly and is ready
			BOOL Initialization_File_Created(void); 
			const char *Get_Initialization_Name(void); // Returns NULL if file not open


			// Open the init file and get and save all future data using it
			BOOL Initialization_File_Open(const char *pszInitialization_Name, BOOL bWrite_Access = TRUE);
			// Close the current Initialization file
			BOOL Initialization_File_Close(void);
			// Save the init file and the future changes made using the above member 
			// procedures under new name (and open)
			BOOL Initialization_Save_As(const char *pszInitialization_Name, BOOL bWrite_Access = TRUE);


			// NOTE FOR ALL LABEL READS BELOW:
			//    ALL POINTER VALUES ARE PASS BY REFERENCE
			//    WILL AUTOMATICALLY ALLOCATE ALL VALUES EXCEPT SINGLE
			//        POINTERS WHICH ARE ASSUMED TO BE ALREADY ALLOCATED
			//    RESPONSIBILITY IS LEFT WITH THE USER TO DELETE THE VALUES
			// Creates and fills a character array which is a comma separated string of values
			BOOL Label_Read(const char *pszLabel, char **ppszValue);
			// Creates and fills a 2D array of characters which is a list of string values - Option to add a Null string at end of list
			// NOTE: uiNumber_Values does not count the NULL if added at the end
			// NOTE: Will check the last string for commas in order to determine if too many strings listed
			BOOL Label_Read(const char *pszLabel, char ***pppszValue, const UINT uiNumber_Values = UINT_MAX, BOOL bAdd_NULL_At_List_End = FALSE);
			// Sets an unsigned integer
			BOOL Label_Read(const char *pszLabel, UINT *puiValue);
			// Creates and fills an integer array
			BOOL Label_Read(const char *pszLabel, UINT **ppuiValue, const UINT uiNumber_Values = UINT_MAX);
			// Creates and fills an array of integer array
			BOOL Label_Read(const char *pszLabel, UINT ***pppuiValue, const UINT *puiNumber_Values, const UINT uiNumber_List);
			// Sets a boolean by default, int if specified
			BOOL Label_Read(const char *pszLabel, int *piValue, BOOL bBoolean_Value = TRUE);
			// Creates and fills an array of boolean by default, int if specified
			BOOL Label_Read(const char *pszLabel, int **ppiValue, const UINT uiNumber_Values = UINT_MAX, BOOL bBoolean_Value = TRUE);
			// Sets DWORD
			BOOL Label_Read(const char *pszLabel, DWORD *pdValue);
			// Sets float
			BOOL Label_Read(const char *pszLabel, float *pfValue);
			// Creates and sets GEN_DATE_STRUCT - in C++ structs pass by value
			// Y2K - No problem since GEN_DATE_STRUCT takes four digits
			BOOL Label_Read(const char *pszLabel, UL_GEN_DATE_STRUCT **pValue);
			// Creates and sets GEN_TIME_STRUCT - in C++ structs pass by value
			BOOL Label_Read(const char *pszLabel, UL_GEN_TIME_STRUCT **pValue);
			

			// Associated label changes
			// NOTE: Change made immediately if Buffer_Changes is set to FALSE
			// NOTE: Changes are buffered in memory if Buffer_Changes is set to TRUE
			// NOTE: Changes being kept in memory are dumped to file when
			//       Buffer_Changes set to FALSE, any other action results
			//       in losing buffer
			BOOL Buffer_Changes(BOOL bStatus);
			BOOL Label_Change(const char *pszLabel, const char *pszValue);
			BOOL Label_Change(const char *pszLabel, const char **ppszValue, const UINT uiNumber_Values);
			BOOL Label_Change(const char *pszLabel, const UINT uiValue);
			BOOL Label_Change(const char *pszLabel, const UINT *puiValue, const UINT uiNumber_Values);
			BOOL Label_Change(const char *pszLabel, const UINT **ppuiValue, const UINT *puiNumber_Values, const UINT uiNumber_List);
			BOOL Label_Change(const char *pszLabel, const DWORD dValue);
			BOOL Label_Change(const char *pszLabel, const float fValue);
			BOOL Label_Change(const char *pszLabel, const int iValue, BOOL bBoolean_Value = TRUE);
			BOOL Label_Change(const char *pszLabel, const int *piValue, const UINT uiNumber_Values, BOOL bBoolean_Value = TRUE);
			// Y2K - No problem since GEN_DATE_STRUCT takes four digits
			BOOL Label_Change(const char *pszLabel, const UL_GEN_DATE_STRUCT *pValue);
			BOOL Label_Change(const char *pszLabel, const UL_GEN_TIME_STRUCT *pValue);

			
			
			// COPYING AND DELETING HELPERS
			// Note: Expects the address of the destination pointer to be passed - Creates the copy for you
			BOOL Copy_2D_List(char ***pDestination, const UINT uiDestination_Length, const char ** pSource, const UINT uiSoure_Length);
			BOOL Copy_2D_List(UINT **pDestination, const UINT uiDestination_Length, const UINT *pSource, const UINT uiSource_Length);
			// Note: Expects the address of the source pointer to be passed
			//       Does automatic checking to see if not null
			void Delete_2D_List(char ***pSource, const UINT uiLength);
			void Delete_2D_List(UINT ***pSource, const UINT uiLength);


		
		protected:

		    //fstream *pInit_File;
			//vector lines;

		    char *pszInit_Name;

			BOOL bInit_File_Created, bWritable;


			// Misc. member functions used to process the file
			BOOL Find_Label(const char *pszLabel, char **ppszLine);
			UINT Line_Find_Num_Strings(const char *pszLine);
			void Line_Replace_Commas(char *pszLine);
			BOOL Line_Find_Next(char **ppszSubstring, char **ppszLine);
			BOOL Set_Boolean(char *pszBoolean_String, BOOL *pbValue);
			BOOL Save_Label(const char *pszLabel, const char *pszValue);
			BOOL Remove_Spaces_At_Begin(char **ppszString);
			BOOL Remove_Spaces_At_End(char *pszString);
			BOOL No_Commas_In_String(const char *pszString);



		private:

		    BOOL bBuffer_Status;
			strstreambuf *pFile_Buffer, *pTemp_File_Buffer;

			void Initialization_File_Delete(void);

			//CMapStringToPtr _map;

		    
		};
		
#endif

