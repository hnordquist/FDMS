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
// 3/18/97

// SEBBoxes.H
// Various generic dialog boxes I have found to useful
// NOTE: Be sure to include the appropriate resources!!!



#ifndef SEBBOXES_H

	#define SEBBOXES_H
                       
                       
//kdm	#include <gui_lib.h>
//kdm	#include <gen.h>
    
    #include <stdlib.h>
    #include <time.h>
    #include "..\resource.h"
#include "ULSupport.h"
#include "ULProgressBarDlg.h"
#include "ULThreeQuestionDlg.h"
#include "ULPasswordChangeDlg.h"
#include "ULPasswordChangeRequestDlg.h"
    
    #include "SEBMacro.H"


	#define SEBBOXES_VERSION	__TIMESTAMP__
                       
    
 //kdm   #define uiMAX_SEB_BOXES		6  	// The maximum number of boxes to create in the
    								//    classes which can create more then one.
    								// I don't recommend increasing number because
    								//    there is an overall displayed limit of 12
    								//    in zApp.
	// Define the following below to avoid compilation errors for the boxes which you will
	// not use.
	// USE_SEB_MODELESS_MESSAGE
	// USE_SEB_QESTION_BOX
    // USE_SEB_THREE_QUESTION_BOX
    // USE_SEB_ABOUT_BOX
    // USE_SEB_PRODUCT_SUPPORT_UC_BOX
    // USE_SEB_IAEA_INSPECTION_BOX
    // USE_SEB_PASSWORD_REQUEST
    // USE_SEB_PASSWORD_CHANGE

//BAD SCOPE BUT NEEDED FOR NOW....KDM


	////////////////////////////////////////////////////////////////////////////////////
	// Creates and handles multiple modeless message boxes
	//    Simply create a SEB_Modeless_Message object and repeatively call New to
	//       create a new message box.
	//    A SEB_Modeless_Message must be created on the heap otherwise destructor
	//       called on function exit
	//    User can call Close_All to close all of the opened messages or delete the
	//       SEB_Modeless_Message object which automatically closes and cleans up
	//       after itself.
	//    Any icons or bitmaps displayed get set by manually setting your bitmap ID
	//       to the ID expected by the dialog box.
	////////////////////////////////////////////////////////////////////////////////////    								
class SEB_Modeless_Message  /* : public CGUI_CallBack*/
	{
		public:
			SEB_Modeless_Message(const char *pszBox_Title,		RESOURCE_ID Icon = NULL); 
									// Name of application to appear in title bar
									// The application icon to appear as the system menu (Windows 95)
			SEB_Modeless_Message(const char *pszBox_Title,		const char *pszMessage,
									RESOURCE_ID Icon = NULL); 
									// Name of application to appear in title bar
									// Message to appear in the box
									// The application icon to appear as the system menu (Windows 95)
			virtual ~SEB_Modeless_Message(void); 
			// Can call repeatedily and will create a new one each time - up to uiMAX_MODELESS_MESSAGES possible
			BOOL New(BOOL bUse_Small_Question_Box = TRUE);
			BOOL New(const char *pszMessage, BOOL bSet_Message = FALSE, BOOL bUse_Small_Question_Box = TRUE);     		
			// Will close and destroy all open modeless messages
			BOOL Close_All(void);     		
        protected:
			char *pszMsg, *pszTitle;
			const char *pszUse;
			RESOURCE_ID uiIcon;
/*			struct SEB_BOXES {
//kdm				CGUI_Dlg *pBox;
				CDialog *pBox;
				BOOL bOpen;
				} pBoxes[uiMAX_SEB_BOXES];
*/
			BOOL Set_Message(const char *pszMessage);
//kdm			BOOL Modeless_Exit(CGUI_Dlg *pDlg, GUI_ACTION ClosingAction);
			BOOL Modeless_Exit(CDialog *pDlg, int ClosingAction);
		};


/*KDM - functionality is obsolete with MessageBox() method in Win32/MFC.
	////////////////////////////////////////////////////////////////////////////////////
	// Creates and handles multiple YES/NO question boxes
	//    Has all of the properties of a SEB_Modeless_Message object, however it
	//       provides YES/NO buttons and notifications of the response.
	//    A SEB_Question_Box must be created on the heap otherwise destructor
	//       called on function exit
	//    Any icons or bitmaps displayed get set by manually setting your bitmap ID
	//       to the ID expected by the dialog box.
	////////////////////////////////////////////////////////////////////////////////////    								
	class SEB_Question_Box : public SEB_Modeless_Message {
		public:
			SEB_Question_Box(const char *pszBox_Title,
								CGUI_CallBack *pCB_Obj = NULL,
								GUI_CB_DlgNotifyFn pYES_NotifyFn = NULL,
								GUI_CB_DlgNotifyFn pNO_NotifyFn = NULL,
								RESOURCE_ID Icon = NULL,
								BOOL bUse_Small_Question_Box = TRUE); 
								// Name of application to appear in title bar
								// Your object - used for notification of button press
								// Your YES callback member function - used for notification of YES button press
								// Your NO callback member function - used for notification of NO button press
								// The application icon to appear as the system menu (Windows 95)
								// Whether to use a small box or large box
			SEB_Question_Box(const char *pszBox_Title,
								const char *pszQuestion,
								CGUI_CallBack *pCB_Obj = NULL,
								GUI_CB_DlgNotifyFn pYES_NotifyFn = NULL,
								GUI_CB_DlgNotifyFn pNO_NotifyFn = NULL,
								RESOURCE_ID Icon = NULL,
								BOOL bUse_Small_Question_Box = TRUE); 
								// Name of application to appear in title bar
								// Question to appear in the box
								// Your object - used for notification of button press
								// Your YES callback member function - used for notification of YES button press
								// Your NO callback member function - used for notification of NO button press
								// The application icon to appear as the system menu (Windows 95)
								// Whether to use a small box or large box
			virtual ~SEB_Question_Box(void); 
			// Sets the callbacks for the next go
			BOOL Set_Callbacks(CGUI_CallBack *pCB_Obj,
								GUI_CB_DlgNotifyFn pYES_NotifyFn,
								GUI_CB_DlgNotifyFn pNO_NotifyFn = NULL,
								RESOURCE_ID Icon = NULL);
			// Can call repeatedily and will create a new one each time - up to uiMAX_MODELESS_MESSAGES possible
			// Note: Modeless returns GUI_OK if worked, both return GUI_NONE if error, otherwise button press GUI_YES or GUI_NO
			GUI_ACTION New(BOOL bModeless = TRUE);
			GUI_ACTION New(const char *pszQuestion, BOOL bModeless = TRUE, BOOL bSet_Message = FALSE);
			// Return the number of buttons
			virtual UINT Get_Number_Buttons(void);
			// Disables all of the displayed buttons of all boxes
			void Disable_All_Buttons(void);
			// Will close and destroy all open modeless questions
			BOOL Close_All(void);     		
		protected:
			UINT uiNumber_Buttons;
        private:
			CGUI_CallBack *pObj;
			GUI_CB_DlgNotifyFn pYES, pNO;
			BOOL bSmall_Question_Box;
		};

*/

	////////////////////////////////////////////////////////////////////////////////////
	// Creates and handles multiple three question boxes
	//    Allows user to select amongst three choices which are set through the
	//       constructor
	//    Choices for modal and modeless
	//    A SEB_Three_Question_Box must be created on the heap if used as modeless 
	//       otherwise destructor called on function exit
	//    Buttons not defined result in GUI_CANCEL 
	//    Any icons or bitmaps displayed get set by manually setting your bitmap ID
	//       to the ID expected by the dialog box.
	////////////////////////////////////////////////////////////////////////////////////    								

  class SEB_Three_Question_Box /*: public SEB_Question_Box*/
  {
		public:
			SEB_Three_Question_Box(const char *pszBox_Title,
								const char *pszButton_1_Text,
								const char *pszButton_2_Text,
								const char *pszButton_3_Text,
//kdm								CGUI_CallBack *pCB_Obj = NULL,
//kdm								GUI_CB_DlgNotifyFn pButton1_NotifyFn = NULL,
//kdm								GUI_CB_DlgNotifyFn pButton2_NotifyFn = NULL,
//kdm								GUI_CB_DlgNotifyFn pButton3_NotifyFn = NULL,
								RESOURCE_ID Icon = NULL,
								BOOL bUse_Small_Question_Box = TRUE); 
								// Name of application to appear in title bar
								// Your Button #1 text
								// Your Button #2 text
								// Your Button #3 text
								// Your object - used for notification of button press
								// Your Button #1 callback member function - used for notification of Button #1 press
								// Your Button #2 callback member function - used for notification of Button #2 press
								// Your Button #3 callback member function - used for notification of Button #3 press
								// The application icon to appear as the system menu (Windows 95)
								// Whether to use a small box or large box
			SEB_Three_Question_Box(const char *pszBox_Title,
								const char *pszQuestion,
								const char *pszButton_1_Text,
								const char *pszButton_2_Text,
								const char *pszButton_3_Text,
//kdm								CGUI_CallBack *pCB_Obj = NULL,
//kdm								GUI_CB_DlgNotifyFn pButton1_NotifyFn = NULL,
//kdm								GUI_CB_DlgNotifyFn pButton2_NotifyFn = NULL,
//kdm								GUI_CB_DlgNotifyFn pButton3_NotifyFn = NULL,
								RESOURCE_ID Icon = NULL,
								BOOL bUse_Small_Question_Box = TRUE); 
								// Name of application to appear in title bar
								// Question to appear in the box
								// Your Button #1 text
								// Your Button #2 text
								// Your Button #3 text
								// Your object - used for notification of button press
								// Your Button #1 callback member function - used for notification of Button #1 press
								// Your Button #2 callback member function - used for notification of Button #2 press
								// Your Button #3 callback member function - used for notification of Button #3 press
								// The application icon to appear as the system menu (Windows 95)
								// Whether to use a small box or large box
			virtual ~SEB_Three_Question_Box(void); 
			// Sets the callbacks for the next go
/*kdm			BOOL Set_Callbacks(CGUI_CallBack *pCB_Obj,
								GUI_CB_DlgNotifyFn pButton1_NotifyFn = NULL,
								GUI_CB_DlgNotifyFn pButton2_NotifyFn = NULL,
								GUI_CB_DlgNotifyFn pButton3_NotifyFn = NULL,
								RESOURCE_ID Icon = NULL);
*/
			// Can call repeatedily and will create a new one each time - up to uiMAX_MODELESS_MESSAGES possible
			// Note: Modeless returns GUI_OK if worked, both return GUI_NONE if error, otherwise button press GUI_YES for #1, GUI_NO for #2, or GUI_CANCEL for #3
//kdm			GUI_ACTION New(BOOL bModeless = TRUE);
			int New(BOOL bModeless = TRUE);
//kdm			GUI_ACTION New(const char *pszQuestion, BOOL bModeless = TRUE, BOOL bSet_Message = FALSE);
			int New(const char *pszQuestion, BOOL bModeless = TRUE, BOOL bSet_Message = FALSE);
			// Disables all of the displayed buttons of all boxes
			void Disable_All_Buttons(void);
			// Will close and destroy all open modeless questions
			BOOL Close_All(void);     
		protected:
			char *pszMsg, *pszTitle;
			const char *pszUse;
			RESOURCE_ID uiIcon;
			ULThreeQuestionDlg* pThreeQuestionDlg;

        private:
//kdm			CGUI_CallBack *pObj;
//kdm			GUI_CB_DlgNotifyFn pBut1, pBut2, pBut3;
			char *pszBut1_Text, *pszBut2_Text, *pszBut3_Text;
			BOOL bSmall_Question_Box;
		};


	////////////////////////////////////////////////////////////////////////////////////
	// Creates and handles the standard IAEA inspection information and box
	//    Simply instantiate a SEB_IAEA_Inspection_Box object with the required
	//       structure COMPLETELY filled.
	//    The users button press is returned from display.
	//    The users inspection information is passed back through the same structure
	//       passed in.
	////////////////////////////////////////////////////////////////////////////////////    								
	class SEB_IAEA_Inspection_Box/* : public CGUI_CallBack*/
	{
		public:
			struct IAEA_INSPECTION_INFO {
				// Y2K - All years are four digits
				char *pszFacility_Location;			// The location of the facility - *Won't change
				UL_GEN_DATE_STRUCT *pReview_Date;		// Date of review being conducted - *Could change!
				UL_GEN_TIME_STRUCT *pReview_Time;		// Time of review being conducted - *Could change!
				UINT uiInspector_ID_Num;			// The number of inspector IDs - *Could change!
				char **ppszInspector_ID;			// The list of inspector names and organizations - *Could change! - *Last item must be NULL!
				UINT uiInspector_ID_Choices_Num;	// The number of inspector choices - *Could change!
				UINT *puiInspector_ID_Choices;		// The list of choices of participating inspector ID's - *Could change!
				char *pszInspection_Number;			// The inspection number - *Could change! Must be a string because could have leading 00 if greater then 1999
				UL_GEN_DATE_STRUCT *pStart_Date;		// The starting date of inspection data - *Could change! 
				UL_GEN_TIME_STRUCT *pStart_Time;		// The starting time of inspection data - *Could change! 
				UL_GEN_DATE_STRUCT *pEnd_Date;			// The ending date of inspection data - *Could change!
				UL_GEN_TIME_STRUCT *pEnd_Time;			// The ending time of inspection data - *Could change!
				char *pszComments;					// The inspection comments - *Could change!
				};
			SEB_IAEA_Inspection_Box(const char *pszApp_Name,
							char **ppszFacility_Name,
							struct IAEA_INSPECTION_INFO *pIAEA_Inspection_Info,
							RESOURCE_ID App_Icon = NULL);
							// Name of the application
							// Name of facility - *Could change!
							// Structure containing box inital and result information
							// The application icon to appear as the system menu (Windows 95)
			virtual ~SEB_IAEA_Inspection_Box(void);
			// Displays the SEB_IAEA_Inspection_Box - return GUI_OK, GUI_CANCEL, or GUI_NONE (Error) 
			int Display(BOOL bSet_To_New = FALSE);
			// Closes the SEB_IAEA_Inspection_Box
			BOOL Close(void);
			// Deletes an IAEA_INSPECTION_INFO structure completely
			BOOL Delete_IAEA_INSPECTION_INFO(struct IAEA_INSPECTION_INFO **ppIAEA_Inspection_Info);
        protected:
			char *pszApp, **ppszFacility;
//kdm			CGStr Temp_Add, Temp_Comments, Temp_Inspection_Number;
			CString Temp_Add, Temp_Comments, Temp_Inspection_Number;
			struct IAEA_INSPECTION_INFO *pInfo;
			RESOURCE_ID uiIcon;
//kdm			CGUI_Dlg *pInspect_Box;
			BOOL Setup_Box(void);
//kdm			void Box_Startup(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			void Set_To_New(void);
			void SEB_IAEA_Inspec_Get_Values(void);
/*			BOOL SEB_IAEA_Inspec_Number(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			BOOL SEB_IAEA_Inspec_Date(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			BOOL SEB_IAEA_Inspec_Date_Less(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			BOOL SEB_IAEA_Inspec_Time(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			BOOL SEB_IAEA_Inspec_Range_Date(CGUI_Dlg *pDlg);
			BOOL SEB_IAEA_Inspec_Range_Date_Less(CGUI_Dlg *pDlg);
			BOOL SEB_IAEA_Inspec_Range_Time(CGUI_Dlg *pDlg);
			BOOL SEB_IAEA_Inspec_Review_Date(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			BOOL SEB_IAEA_Inspec_Review_Time(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			BOOL SEB_IAEA_Inspec_Start_Date(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			BOOL SEB_IAEA_Inspec_End_Date(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			BOOL SEB_IAEA_Inspec_Start_Time(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			BOOL SEB_IAEA_Inspec_End_Time(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			void SEB_IAEA_Inspec_Add(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
			void SEB_IAEA_Inspec_Delete(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
*/
	};



		
	////////////////////////////////////////////////////////////////////////////////////
	// Creates and handles a password request dialog box
	//    Simply create a SEB_Password_Request object and call Display
	//    User can call Close to force close the opened box or delete the
	//       SEB_Password_Request object which automatically closes and cleans up
	//       after itself.
	//    Any icons or bitmaps displayed get set by manually setting your bitmap ID
	//       to the ID expected by the dialog box.
	////////////////////////////////////////////////////////////////////////////////////    								
	class SEB_Password_Request /*: public CGUI_CallBack*/
	{
		public:
			SEB_Password_Request(const char *pszValid_Password,
									const char *pszBox_Title,
									RESOURCE_ID Icon = NULL);
									// The password to be checked 
									// Name of application to appear in title bar
									// The application icon to appear as the system menu (Windows 95)
			virtual ~SEB_Password_Request(void); 
			// Displays the SEB_Password_Request - returns GUI_OK password correct, GUI_CANCEL, or GUI_NONE (Error) 
//kdm			GUI_ACTION Display(void);
			int Display(void);
			// Will close and destroy open box
//kdm			BOOL Close(void);     		
        protected:
			char *pszTitle;
			RESOURCE_ID uiIcon;
			char *pszPassword;
//KDM			CGUI_Dlg *pBox;
			ULPasswordChangeRequestDlg *pBox;
			BOOL bOpen, bPassword_Correct;
//kdm			CGStr Entered_Password;
			CString Entered_Password;
//kdm			BOOL Setup_Box(void);
//kdm			void SEB_Password_Request_Verify(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
		};



	////////////////////////////////////////////////////////////////////////////////////
	// Creates and handles a password changing dialog box
	//    Simply create a SEB_Password_Change object and call Display
	//    User can call Close to force close the opened box or delete the
	//       SEB_Password_Change object which automatically closes and cleans up
	//       after itself.
	//    Any icons or bitmaps displayed get set by manually setting your bitmap ID
	//       to the ID expected by the dialog box.
	////////////////////////////////////////////////////////////////////////////////////    								
	class SEB_Password_Change/* : public CGUI_CallBack*/
	{
		public:
			SEB_Password_Change(const char *pszBox_Title,
									RESOURCE_ID Icon = NULL); 
									// Name of application to appear in title bar
									// The application icon to appear as the system menu (Windows 95)
			virtual ~SEB_Password_Change(void); 
			// Displays the SEB_Password_Change - return GUI_OK, GUI_CANCEL, or GUI_NONE (Error) 
//kdm			GUI_ACTION Display(void);
			int Display(void);
			// Get the new password - returns NULL if a new password was not set
			const char *Get_Password(void);
			// Will close and destroy open box
//KDM			BOOL Close(void);     		
        protected:
			char *pszTitle;
			RESOURCE_ID uiIcon;
//kdm			CGUI_Dlg *pBox;
			ULPasswordChangeDlg *pBox;
			BOOL bOpen, bPassword_Same;
//kdm			CGStr Entered_Password1, Entered_Password2;
			CString Entered_Password1, Entered_Password2;
			BOOL Setup_Box(void);
//kdm			void SEB_Password_Change_Verify(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
		};



	////////////////////////////////////////////////////////////////////////////////////
	// Creates and handles a progress bar dialog box
	//    Simply create a SEB_Progress_Bar object and repeatedly call it the
	//       total number of times set
	//    User can call Close to force close the opened box or delete the
	//       SEB_Progress_Bar object which automatically closes and cleans up
	//       after itself.
	//    Any icons or bitmaps displayed get set by manually setting your bitmap ID
	//       to the ID expected by the dialog box.
	////////////////////////////////////////////////////////////////////////////////////    								
	class SEB_Progress_Bar/* : public CGUI_CallBack*/
	{
		public:
			SEB_Progress_Bar(unsigned long ulTotal_Progress_Calls,
									unsigned long ulCalls_Per_Update, 
									const char *pszMessage = NULL,
									const char *pszBox_Title = NULL,
									RESOURCE_ID Icon = NULL,
									BOOL bAllow_Cancel = TRUE);
									// The total number of times will call Progress_Bar_Update
									// Number of calls before bar updates itself - screen updates take much time
									// Status message which appears under "Please Wait" message
									// Name of application to appear in title bar
									// The application icon to appear as the system menu (Windows 95)
									// Allow the progress bar to be canceled
			virtual ~SEB_Progress_Bar(void); 
			// Repeatively call up to number of times setFirst time called it displays box 
			// Returns FALSE if cancel was pressed
			// NOTE: Only need to pass message once for change
            BOOL Progress_Bar_Update(const char *pszMessage = NULL);
            // NOTE: If no parameters passed then last values kept
            BOOL Progress_Bar_Reset(unsigned long ulTotal_Progress_Calls = NULL, 
									unsigned long ulCalls_Per_Update = NULL, 
									const char *pszMessage = NULL,
									const char *pszBox_Title = NULL,
									RESOURCE_ID Icon = NULL);
			// Will close and destroy open box
			BOOL Close(void);     		
        protected:
//kdm			CGUI_Dlg *pBox;
			ULProgressBarDlg *pProgressBarDlg;  //kdm
			RESOURCE_ID uiIcon;
			char *pszTitle, *pszMsg;
			unsigned long ulTotal_Calls, ulCall_Number, ulPer_Call;
			BOOL bCanceled, bOpen;
			virtual BOOL Setup_Box(void);
			BOOL bCan_Cancel;
//kdm			virtual void SEB_Progress_Bar_Cancel(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
		};



#endif


		
