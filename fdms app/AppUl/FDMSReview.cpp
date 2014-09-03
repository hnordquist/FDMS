// $History: FDMSReview.cpp $ 
// 
// *****************  Version 11  *****************
// User: 186846       Date: 2/24/10    Time: 5:02p
// Updated in $/FDMS/FDMSNET/AppUl
// 
// *****************  Version 10  *****************
// User: 186846       Date: 2/26/08    Time: 8:54a
// Updated in $/FDMS/FDMS Application/FDMSNET/AppUl
// checkpoint
// 
// *****************  Version 9  *****************
// User: 186846       Date: 12/19/07   Time: 2:42p
// Updated in $/FDMS/FDMS Application/FDMSNET/AppUl
// 
// *****************  Version 8  *****************
// User: Longo        Date: 11/14/05   Time: 3:43p
// Updated in $/FDMS/FDMS Application/FDMSNET/AppUl
// 
// *****************  Version 7  *****************
// User: Longo        Date: 3/16/05    Time: 2:24p
// Updated in $/FDMS/FDMS Application/FDMSNET/AppUl
// 
// *****************  Version 6  *****************
// User: Longo        Date: 2/10/05    Time: 3:46p
// Updated in $/FDMS/FDMS Application/FDMSNET/AppUl
// checkpoint checkin
// 
// *****************  Version 5  *****************
// User: Longo        Date: 1/11/05    Time: 4:03p
// Updated in $/FDMS/FDMS Application/FDMSNET/AppUl
// 
// *****************  Version 4  *****************
// User: Longo        Date: 11/30/04   Time: 4:22p
// Updated in $/FDMS/FDMS Application/FDMSNET/AppUl
// added code to auto-select the matching row in the measeur dialog list,
// when involkd by IR tool review message
// 
// *****************  Version 3  *****************
// User: Longo        Date: 11/24/04   Time: 3:43p
// Updated in $/FDMS/FDMS Application/FDMSNET/AppUl
// Some good things are working now
// 
// *****************  Version 2  *****************
// User: Longo        Date: 9/17/04    Time: 3:32p
// Updated in $/FDMS/FDMS Application/FDMSNET/AppUl
// First integration of the MFC Upper Layer code
// FDMSReview.cpp: implementation of the FDMSReview class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\FDMS.h"
#include "..\CycVer.h"
#include "..\mainfrm.h"
#include "..\matchmeasurement.h"
#include "..\selectmeasurement.h"

#include <comutil.h>

#include "FDMSReview.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FDMSReview::FDMSReview()
{

}

FDMSReview::~FDMSReview()
{

}

BOOL FDMSReview::RT_Review_My_Save(void) 
{
	return TRUE;
}

//******************************************************************************************************
//RT_Review_My_Exit() : this method receives a SHUTDOWN message from the 
//upper layer.  This message is received when another tool is instructed to shut down
//all other tools.
//******************************************************************************************************
BOOL FDMSReview::RT_Review_My_Exit(void) 
{
	// This will cause the application to exit.
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Required through RTReview to provide the Review Begin procedure
//which is specific to the review tool in which this upper layer is inserted.
//ENTRY POINT FOR THE APPLICATION - REQUIRED TO BE FILLED IN 
//BY APPLICATIONS INSTALLING THE UPPER LAYER     <RQMT> //kdm
////////////////////////////////////////////////////////////////////////////////////////////////////////

RTReview::REVIEW_OPERATION FDMSReview::RT_Review_Begin(void) 
{
	//Complete the tasks that normally would have been completed in the 
	//application class InitInstance() method.  This is driven by the upper layer
	//design.

	CFDMSApp* pMe = ((CFDMSApp*)AfxGetApp());
	pMe->DoFinalInitialization(true);

	Set_Messages_On_Hold(FALSE);	// Allow messages received via pipe commands to be processed.

	AfxGetApp()->Run();
	return (RTReview::REVIEW_EXIT);
}

void FDMSReview::SetMessagesOnHold(bool ProcessMsgs)
{
	Set_Messages_On_Hold(ProcessMsgs);	// Allow messages received via pipe commands to be processed.

}

bool FDMSReview::ActivateMeasurementDialog(int mindex)
{
	//mindex++; // ?? off by one in the table???

	bool ret = true;
	//g_hMeasureDlg =   // basic opening dialog (background, verification)
	//g_hVerifyDlg =  // the four measurement choices (cycle, verif, unused, return)
	//g_hAllOtherDlg = // verification measurement and the 3 graphs + return
	//g_hCycVer = // cycle measurement with singular cycle graph + return

#define CYCLEDLG wndhndl[0]
#define CHOOSINGDLG wndhndl[1]
#define VGRAPHDLG wndhndl[2]
#define OPENINGCHOICEDLG wndhndl[3]
#define SELECTMEASUREMENTDLG pSD
#define CURSELMSRDLG ((CSelectMeasurement*)pApp->GetMeasurementDialogPtr())
#define BPSTS   75

	BOOL wndhndl[4];

	CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
	CMainFrame* pMF = (CMainFrame*)(pApp->m_pMainWnd);
	CSelectMeasurement* pSD = (CSelectMeasurement*)pApp->GetMeasurementDialogPtr();

	CMeasureDlg& MD = pMF->m_MeasureDlg;
	OPENINGCHOICEDLG = IsWindowVisible(g_hMeasureDlg);

	CVerifyDlg& VD = pMF->m_VerifyDlg;
	CHOOSINGDLG = IsWindowVisible(g_hVerifyDlg);

	CAllOtherDlg& AOD = pMF->m_AllOtherDlg;
	VGRAPHDLG = IsWindowVisible(g_hAllOtherDlg);

	CCycVer& CD = pMF->m_CycVer;
	CYCLEDLG = IsWindowVisible(g_hCycVer);

	//!! stop here
	// trying to figure out how to force the selection dialog to select the current row on start up,
	// thought I could use a message but that only works if the dialog is already up, 
	// have to come up with a way to start the dialog and then slect the proper row too: in initdialog

	try
	{

	switch (g_iMPMeasurementType[mindex])
	{
	case CycleVerification:
		{
			if (CYCLEDLG)  // is cycle dialog open?
			{
				//open CSelectMeasurement
				CD.OpenSelectMeasurement(mindex);
			}
			else
			{
				if (SELECTMEASUREMENTDLG)
				{
					if (SELECTMEASUREMENTDLG->m_iSubsetSelector != CycleVerification)
					{
						if (SELECTMEASUREMENTDLG->m_bBlock) // can't close it, just return without doing anything
						{
							ret = false;
						}
						else
						{
							//close SELECTMEASUREMENTDLG
							SELECTMEASUREMENTDLG->OnCancel();

							//close VGRAPHDLG
							AOD.OnReturn();

							//open CYCLEDLG
							VD.OnCycleVer();					::Sleep(BPSTS);

							//open CSelectMeasurement
							CD.OpenSelectMeasurement(mindex);
						}
					}
				} else
					if (OPENINGCHOICEDLG)
					{
						//open CHOOSINGDLG
						MD.OnAllOther();					::Sleep(BPSTS);

						//open CYCLEDLG
						VD.OnCycleVer();					::Sleep(BPSTS);

						//open CSelectMeasurement
						CD.OpenSelectMeasurement(mindex);
					} else
						if (VGRAPHDLG)
						{
							//close VGRAPHDLG
							AOD.OnReturn();

							//open CHOOSINGDLG (should be open)

							//open CYCLEDLG
							VD.OnCycleVer();					::Sleep(BPSTS);

							//open CSelectMeasurement
							CD.OpenSelectMeasurement(mindex);
						} else
							if (CHOOSINGDLG)
							{
								//open CYCLEDLG
								VD.OnCycleVer();					::Sleep(BPSTS);

								//open CSelectMeasurement
								CD.OpenSelectMeasurement(mindex);
							}
			}
		}
	case AssemblyVerification:
		{
			if (VGRAPHDLG)  // is verification  dialog open?
			{
				//open CSelectMeasurement
				AOD.OpenSelectMeasurement(mindex);
			}
			else
			{
				if (SELECTMEASUREMENTDLG) 
				{
					if (SELECTMEASUREMENTDLG->m_iSubsetSelector != AssemblyVerification)
					{
						if (SELECTMEASUREMENTDLG->m_bBlock) // can't close it, just return without doing anything
						{
							ret = false;
						}
						else
						{
							//close SELECTMEASUREMENTDLG
							SELECTMEASUREMENTDLG->OnCancel();

							//close CYCLEDLG
							CD.OnReturn();

							//open VGRAPHDLG
							VD.OnAssemVer();					::Sleep(BPSTS);

							//open CSelectMeasurement
							AOD.OpenSelectMeasurement(mindex);
						}
					}
				} else
					if (OPENINGCHOICEDLG)
					{
						//open CHOOSINGDLG
						MD.OnAllOther();

						//open VGRAPHDLG
						VD.OnAssemVer();					::Sleep(BPSTS);

						//open CSelectMeasurement
						AOD.OpenSelectMeasurement(mindex);
					} else
						if (CYCLEDLG)
						{
							//close CYCLEDLG
							CD.OnReturn();

							//open CHOOSINGDLG (should be open)

							//open VGRAPHDLG
							VD.OnAssemVer();					::Sleep(BPSTS);

							//open CSelectMeasurement
							AOD.OpenSelectMeasurement(mindex);
						} else
							if (CHOOSINGDLG)
							{
								//open VGRAPHDLG
								VD.OnAssemVer();					::Sleep(BPSTS);

								//open CSelectMeasurement
								AOD.OpenSelectMeasurement(mindex);
							}
			}
		}
	} // switch measurement type
	}
	catch (...)
	{
	}

	return ret;
}

//*******************************************************************************************************************************************
//These methods override virtual functions that are defined for the upper layer.  
//These methods are called in response to messages received via pipes established for communication in the upper layer.
//These methods are called by the upper layer classes in response to received commands/requests from other client applications.

//APPLICATION responds to requests for time matching.  Requests may or may not include location.
//*********************************************************************************************************************************************

BOOL FDMSReview::RT_Display_My_Data(UINT /*uiEvent_Number*/) {

	return FALSE;

}

extern void handlecomerr(const _com_error& comerr);

#import "..\Coms\CoFacilityMgr.exe"  no_namespace named_guids
#include "..\StartupImpDlg.h"

BOOL FDMSReview::RT_Display_My_Data(const char* /*pszTool_Name*/, const char *pszTimestamp, const char *pszLocation, BOOL *pbLocation_Failed)
{
	bool RetVal = false;
	ULSupport SuptLib;
	UL_MSG_DATA_STRUCT Msg = SuptLib.ConvertTimeMatchMsgToStruct(pszTimestamp);

	StartupImpDlg StartUpDlg;

	// this code matches on the long detector name, so . . . use the fac mgr here to look up the short name
	short mFacilityID;
	long mStationID;
	CString mStationShortName, mInstrumentType;
	CString mFacilityLongName, mFacilityShortName;

	StartUpDlg.DoIt("Matching an Event . . .");

	CIniFileProcessor::GetIRSFacilityID(mFacilityID);
	try
	{
	
		CString FacilityDirectory;

		// Using the Facility Manager, obtain info on the current facility
		IFacilityMgrDataPtr pIFacMgrData(CLSID_FacilityMgrData);

		_bstr_t bstrStationLongName = pszLocation;
		if (bstrStationLongName.length() > 0) // skip if empty station name
		{

			mStationID = 0;
			_bstr_t bstrInstrumentType;
			HRESULT hr = pIFacMgrData->GetStationID(mFacilityID, bstrStationLongName, &mStationID, bstrInstrumentType.GetAddress());
			if (FAILED(hr))
			{
				mStationShortName = pszLocation;
			}
			else
				mStationShortName = pIFacMgrData->GetStationShortName(mFacilityID, mStationID, bstrInstrumentType).GetBSTR();
		}
		else
				mStationShortName = pszLocation;
	} 
	catch (const _com_error& comerr) 
	{
		handlecomerr(comerr);				
	}
	catch (...)
	{
	}

	try
	{
		//Load the date and time information into a COleDateTime object.
		MatchMeasurement m(mStationShortName.GetBuffer(), etgDaysOnly, eigDetector);  // match to detector id and the closest day tolerance (180 days really big!)
		COleDateTime a, b;
		a.SetDateTime(Msg.year, Msg.month, Msg.day, Msg.hour, Msg.minute, Msg.second);
		b.SetDateTime(Msg.year, Msg.month, Msg.day, Msg.hour, Msg.minute, Msg.second);
		m.SetMatchDate(a, b);

		m.Match();  
		if (m.Matched(eisMP))
		{
			int gi = m.GetMeasurementDateMatch(eisMP);
			//if (gi < 0)
			//	gi = m.GetDischargeDateMatch();
			RetVal = ActivateMeasurementDialog(gi);
		}
	}
	catch (...)
	{
	}
	StartUpDlg.DestroyWindow();

*pbLocation_Failed = false; //This is temporary  - if this is not returned FALSE, the upper layer sees that this is an error.
return RetVal;  //TRUE return value indicates success.
}

BOOL FDMSReview::RT_Display_My_Data(const char* /*pszTool_Name*/, const char * /*pszTimestamp*/)
{
	bool RetVal = false;


	/****

	RetVal = true;

	// see TestRADFDMS and INCC Review.cpp for actual lookup matching and subsequent processing?

	// OK so this is to match existing data in FDSM basedon dector and date
	But what about the data in the RAD to FDMS XML file, just what is that supposed to be used for huh?
	I've forgotten the requiremtns, time to write them down.

	//1)   Find detector/location in DB, notify if not found, or ignore if not found?
	//2)   Search parameters needed? (get unattended measurement parameters for this detector )
	//3)   Look for measurement on this date
	//3.1a)   select the measurement, make it the current measurement
	//3.1b)   open up the grid, or pop up the graphs, have to decide what to do
	//3.1c)   exit this sequence
	//4)   No exact match, look for the closest one with the correct detector
	//4.1)    if found go to 3.1a
	//5)   Nothing found within the search parameters
	//5.1)    Pick the one that is closest in time (forward or backward)
	****/

	return RetVal;  //TRUE return value indicates success.
}

