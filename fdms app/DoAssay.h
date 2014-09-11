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
//////////////////////////////////////////////////////////////////////////
// Copyright 2005, The Regents Of the University of California.  This
// software was produced under U.S. Government contract (W-7405-ENG-36) at
// the Los Alamos National Laboratory by the Safeguards Science and
// Technology Group (N-1).  All Rights Reserved.
//////////////////////////////////////////////////////////////////////////
// $History: DoAssay.h $
// 
// *****************  Version 8  *****************
// User: Longo        Date: 2/10/05    Time: 3:46p
// Updated in $/FDMS/FDMS Application/FDMSNET
// checkpoint checkin
// 
// *****************  Version 7  *****************
// User: Longo        Date: 2/07/05    Time: 10:27a
// Updated in $/FDMS/FDMS Application/FDMSNET
// checkpoint checkin
// 
// *****************  Version 6  *****************
// User: Longo        Date: 1/05/05    Time: 5:28p
// Updated in $/FDMS/FDMS Application/FDMSNET
// 128 upped to 256 for max cycles
// 
// *****************  Version 5  *****************
// User: Longo        Date: 12/10/04   Time: 6:25p
// Updated in $/FDMS/FDMS Application/FDMSNET
// 
// *****************  Version 4  *****************
// User: Longo        Date: 12/10/04   Time: 4:44p
// Updated in $/FDMS/FDMS Application/FDMSNET
// Combining the GrandCom demo class wirth the SerialCom live detector
// control class
// 
// *****************  Version 3  *****************
// User: Longo        Date: 10/20/04   Time: 1:10p
// Updated in $/FDMS/FDMS Application/FDMSNET
// registry failed today, saving all work
//////////////////////////////////////////////////////////////////////////


#pragma once

#include <afxdisp.h>

#include ".\detectorthread.h"

#include "CCButton.h"

struct SMeasurementData
{
	UINT iResult;
	int  iType;
	int  iTime;
	int  iIonHV;
	int  iHeHV;
	int  iPoints;
};



struct DataRec
{
	short	ChPos[MAXIMUM_CYCLES_AUTHORIZED];
};

#define ACCUMULATORSIZE		1024

extern int		g_iDemoCount;

extern AnalysisRec g_sScanRecs[MAXIMUM_CYCLES_AUTHORIZED];  // change this to a class that guards overwriting, or use a dynamic array, something to use as a guard
extern int		g_dScanRecMax;


class CDoAssay : public CDialog
{
// Construction
public:
	CDoAssay(CWnd* pParent = NULL);   // standard constructor
	~CDoAssay();

	DECLARE_DYNAMIC(CDoAssay )


	bool bDemoVersion;
// Dialog Data
	//{{AFX_DATA(CDoAssay)
	enum { IDD = IDD_DOASSAY };
	CProgressCtrl	m_pProgress2;
	CProgressCtrl	m_pProgress1;
	CStatic	m_StaticAssm;
	CEdit	m_EditGrand;
	CEdit	m_EditActivity;
	CStatic	m_pStaticInstruction;
	CStatic	m_pStaticComment;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDoAssay)
	public:
	virtual int DoModal(int AssayIndex);
	virtual int DoModal(CString cString);
	virtual BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	bool	m_bPass;
	bool	m_bOffsetFail;
	bool	m_bReconciled;
	tAssayTypes		m_iMode;
	int		m_iItem;

	int		m_iGainOffsetInternal[2][12];
	int		m_iGainOffsetRemote[2][12];
	bool	m_bOffset;
	SMeasurementData* m_psMeasurementData;
	int		m_dAssayIndex;

// Implementation
protected:

	CString m_cID;
	bool	m_bMiniGrand;
	int		m_dGrandCmdTimeOut;
	char	m_szSound[MAX_PATH];

	CLogFile* m_pCommLog;
	CLogFile* m_pActivityLog;

public:
	bool bHasAbortableStatus() const;

protected:

	virtual void	UpdateParams(char*);
	virtual void	SetModeForUpdateParams();
	virtual void	CreateTheThread();

	CColorButton *CBWatch;
	CColorButton *CBOk;
	CColorButton *CBCancelAssay;

	DetectorThread* m_pGrandCom;

	COleDateTime m_Start;
	int		m_iOffsetMaxTime;
	int		m_iOffsetMin;
	int		m_iOffsetMax;

	RECT	m_Rect;
	int		m_dMessCount;
	bool	m_bMinimal;
	bool	m_bDoingBackground;
	bool	m_bDoingNormalize;
	bool	m_bGRANDFreeRun;
	bool	m_bHaveGrand;
	bool	m_bShutDown;
volatile	int		m_dCurrentState;
volatile	int     m_dGrandState;
//	int		m_dPassiveMinSamp;
//	int		m_dPassiveMaxSamp;
	int		m_iCycleTime;
	int		m_iCycles;
	int		m_iTotalSeconds;
	float	m_fICBackground1;
	float	m_fICBackground2;
	int		m_Grand_TicCount;
	int		m_dGrandTime;
	int		m_dGrossGammaGate1;
	int		m_dGrossGammaGate2;
	int     m_dStepIncrement;
	int		m_dGrandCycleTime;
	int		m_dGrandTickle;
	float   m_fCurrentCountTime;
	char    m_pText[1024];
	char	m_szScale[64];
	CString m_Actions;
	CString m_Grand;
	char	m_szTemp[1024];
	//int		m_dTPrev;
	//int		m_dCPrev;
	int		m_dCFirst;
	int		m_dGrossGammaCnt;
	int		m_dNeutronCnt;
	int		m_iReceivingOffsetChan;
	double	m_fGrossGamma1;
	double  m_fGrossGammaAverage1;
	double	m_fGrossGamma2;
	double  m_fGrossGammaAverage2;
	double  m_fNeutronA;
	double  m_fNeutronB;
	double  m_fNeutronC;
	double  m_fNeutronAAverage;
	double  m_fNeutronBAverage;
	double  m_fNeutronCAverage;
	bool	Accumulate(char*);
	void	ClearAccumulator(void);
	void	PostToActions(char*);
	void	PostToGrand(char*);
	void	PostInstruction(char*);
	void	PostComment(char*);

	bool	mbWantRemote;

	CFont	InstructionFont;
	CFont	ItemFont;
    void CrackGains(char *Data, int* iValue1, int* iValue2, int* iValue3, int* iValue4);

	void SetDataInfoLabel();

 	afx_msg LRESULT GrandCmdAccumulator(WPARAM WParam, LPARAM LParam);
	afx_msg LRESULT StateMachine(WPARAM WParam, LPARAM LParam);
	afx_msg LRESULT GrandStateMachine(WPARAM WParam, LPARAM LParam);
	afx_msg LRESULT CmdDistributor(WPARAM WParam, LPARAM LParam);

	void TerminateDetectorMeasurement();

	char m_szGrandCmdBuffer[ACCUMULATORSIZE];

	// Generated message map functions
	//{{AFX_MSG(CDoAssay)
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnWatchButton();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnAck();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};