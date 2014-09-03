// MeasureDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FDMS.h"
#include "DoAssay.h"
#include "DemoDoAssay.h"

#include "AcceptReject.h"
#include "dconfig.h"

#ifdef _XJDEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMeasureDlg dialog
HWND g_hMeasureDlg;

CMeasureDlg::CMeasureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMeasureDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMeasureDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMeasureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMeasureDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMeasureDlg, CDialog)
	//{{AFX_MSG_MAP(CMeasureDlg)
	ON_BN_CLICKED(ID_BUTTON_BG,OnBackGround)
	ON_BN_CLICKED(ID_BUTTON_ALLOTHER,OnAllOther)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMeasureDlg message handlers

void CMeasureDlg::OnBackGround( )
{
	ShowWindow(SW_HIDE);

	CDoAssay* pDoAssay = NULL;

	try
	{
		CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();

		if (pApp->m_bDemoVersion)
			pDoAssay = new DemoDoAssay();
		else
			pDoAssay = new CDoAssay();

		SMeasurementData sMeasurementData;
		sMeasurementData.iResult = 0;
		//sMeasurementData.iType = ???;
		sMeasurementData.iTime = GETPROFINT(eBGCYCLETIME);
		sMeasurementData.iIonHV = GETPROFINT(eICHVBIAS);
		sMeasurementData.iHeHV = GETPROFINT(eHVBIAS);
		sMeasurementData.iPoints = GETPROFINT(eBGCYCLES);


		//give the data to doassay
		pDoAssay->m_psMeasurementData = &sMeasurementData;


		if (pDoAssay->DoModal(-1)==IDOK)
		{
			if (pDoAssay->m_bPass)
			{
				double dAveA, dAveB, dAveC;
				double dAve1, dAve2;
				dAveA = dAveB = dAveC = 0.0;
				dAve1 = dAve2 = 0.0;
				if (g_dScanRecMax > 0)
				{
					for (int i = 0; i < g_dScanRecMax; i++)
					{
						dAveA += g_sScanRecs[i].dNeutronA;
						dAveB += g_sScanRecs[i].dNeutronB;
						dAveC += g_sScanRecs[i].dNeutronC;
						dAve1 += g_sScanRecs[i].dGrossGammaAve1;
						dAve2 += g_sScanRecs[i].dGrossGammaAve2;
						//g_sScanRecs[20];
					}

					dAveA /= g_dScanRecMax;
					dAveB /= g_dScanRecMax;
					dAveC /= g_dScanRecMax;
					dAve1 /= g_dScanRecMax;
					dAve2 /= g_dScanRecMax;

					// do not remove the global background measurements from these background measurements
				}
				CString cTemp;
				CAcceptReject cAcceptReject;
				cAcceptReject.m_cDialogLabel = "Background Measurement Results";
				cAcceptReject.m_cLabelNew	 = "New";
				cAcceptReject.m_cLabelPrev	 = "Previous";
				cAcceptReject.m_cLabel[NeutA]	 = "Chan A:";
				cAcceptReject.m_cLabel[NeutB]	 = "Chan B:";
				cAcceptReject.m_cLabel[NeutC]	 = "Chan C:";
				cAcceptReject.m_cLabel[Gamma1]	 = "Chan 1:";
				cAcceptReject.m_cLabel[Gamma2]	 = "Chan 2:";

				int cht;
				for (cht = NeutA; cht < MaxChannels; cht++)
				{
					cAcceptReject.m_cPrev[cht].Format("%.3lf",g_fBkgMeasurements[cht]);
				}

				cTemp.Format("%.3lf",dAveA);
				cAcceptReject.m_cNew[NeutA] = cTemp;
				cTemp.Format("%.3lf",dAveB);
				cAcceptReject.m_cNew[NeutB] = cTemp;
				cTemp.Format("%.3lf",dAveC);
				cAcceptReject.m_cNew[NeutC] = cTemp;
				cTemp.Format("%.3lf",dAve1);
				cAcceptReject.m_cNew[Gamma1] = cTemp;
				cTemp.Format("%.3lf",dAve2);
				cAcceptReject.m_cNew[Gamma2] = cTemp;

				if (cAcceptReject.DoModal()==IDOK)
				{
					//save the background
					for (cht = NeutA; cht < MaxChannels; cht++)
					{
						CIniFileProcessor::SaveSingleValue((tIniNames)(eBACKGROUND_A + cht), cAcceptReject.m_cNew[cht]);
					}

					//save the background date
					COleDateTime cNow = COleDateTime::GetCurrentTime();

					// globalize the newly accepted background measurements
					CFDMSApp* pApp = (CFDMSApp*)AfxGetApp();
					pApp->GetSavedBkgMeasurements();

					pApp->UpdateMeasurementsBG(cNow);
				}
			}
		}
		else
		{
					// background measurement cancelled or failed
		}

	}
	catch (...)
	{
	}

	if (pDoAssay)
		delete pDoAssay;
	ShowWindow(SW_SHOW);
}

void CMeasureDlg::OnAllOther( )
{
	ShowWindow(SW_HIDE);
	::ShowWindow(g_hVerifyDlg,SW_SHOW);
}


BOOL CMeasureDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// xxxx: Add extra initialization here
	CRect cRect;

	GetDlgItem(IDC_STATIC_1)->GetWindowRect(&cRect);
	ScreenToClient(&cRect);
	m_cButtonBG.Create("Background",
		WS_CHILD | WS_TABSTOP | WS_VISIBLE |
		BS_PUSHBUTTON | BS_MULTILINE | BS_CENTER,
		cRect,this,ID_BUTTON_BG);
	m_cButtonBG.SetFaceColor(RGB(0,255,255),false);
	GetDlgItem(IDC_STATIC_1)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC_2)->GetWindowRect(&cRect);
	ScreenToClient(&cRect);
	m_cButtonAllOther.Create("Verification",
		WS_CHILD | WS_TABSTOP | WS_VISIBLE |
		BS_PUSHBUTTON | BS_MULTILINE | BS_CENTER,
		cRect,this,ID_BUTTON_ALLOTHER);
	m_cButtonAllOther.SetFaceColor(RGB(0,255,255),false);
	GetDlgItem(IDC_STATIC_2)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CMeasureDlg::PreTranslateMessage(MSG* pMsg) 
{
	// xxxx: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_ESCAPE || pMsg->wParam ==VK_RETURN))
	{
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}





