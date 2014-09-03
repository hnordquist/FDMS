// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__8565E54D_9E7C_46DC_8A34_424D72DB3894__INCLUDED_)
#define AFX_STDAFX_H__8565E54D_9E7C_46DC_8A34_424D72DB3894__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define  _WIN32_WINNT   0x0500
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#define _CRT_SECURE_NO_DEPRECATE
#define _ATL_SECURE_NO_DEPRECATE
#define _AFX_SECURE_NO_DEPRECATE

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxcview.h>
#include <afxdisp.h>        // MFC Automation classes

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxdisp.h>

#define TIMER_ALL_TIC		8000

#define DEFAULTBADVALUE -999.9f
#define MAXIMUM_CYCLES_AUTHORIZED 1024
struct AnalysisRec 
{
	double	dGrossGammaAve1;
	double	dGrossGammaAve2;
	double  dNeutronA;
	double  dNeutronB;
	double  dNeutronC;
};

extern void dvaprintf( char * format, ...);

struct ComData
{
	int Size;
	char*Data;
};


//color definitions
#define RED				RGB(255,0,0)
#define GREEN			RGB(0,255,0) 
#define BLUE			RGB(0,0,255)
#define YELLOW			RGB(255,255,0)
#define ORANGE			RGB(255,153,51)
#define HOT_PINK		RGB(255,51,153)
#define PINK			RGB(255,128,128)
#define PURPLE			RGB(153,0,204)
#define CYAN			RGB(0,255,255)
#define BLACK			RGB(0,0,0)
#define WHITE			RGB(255,255,255)
#define LAVENDER		RGB(199,177,255)
#define PEACH			RGB(255,226,177)
#define SKY_BLUE		RGB(142,255,255)
#define FOREST_GREEN	RGB(0,192,0)
#define BROWN			RGB(80,50,0)
#define TURQUOISE		RGB(0,192,192)
#define ROYAL_BLUE		RGB(0,0,192)
#define GRAY			RGB(192,192,192)
#define DARK_GRAY		RGB(128,128,128)
#define TAN				RGB(255,198,107)

#define DARK_BLUE		RGB(0,0,128)
#define MAROON			RGB(128,0,0)
#define DUSK			RGB(255,143,107)
#define LIGHT_GRAY		RGB(225,225,225)	//only for 3D graph lines

#define WARNINGPINK		RGB(255,206,255)

typedef enum
{
	eisMP = 0,
	eisDB = 1,
	//eisM = 2,
	eisNum
} tMatchIdentitySource;


// MP Facility	ID	Measurement type	Status	Enrichment (%)	Burnup (GWd/MT)	Discharge day	month	year	cycle #	Thres A	Thres B	 Measurement day	month	year	Cooling Time (years)	 NA	 NB	 NC	 G1	 G2 	Detector      
	typedef enum {
		efhFacility, efhID,
		efhMeasType, efhStatus,
		efhEnrichment, efhBurnUp,
		efhDischDay,efhDischMonth,efhDischYear,
		efhCycle,
		efhThresholdA,
		efhThresholdB, 
		efhMeasDay,efhMeasMonth,efhMeasYear,
		efhCoolingTime,
		efhNeutronA, efhNeutronB, efhNeutronC,
		efhGamma1, efhGamma2, efhDetector, efhHeaderCount
	} CSVFileHeaderIDs;


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__8565E54D_9E7C_46DC_8A34_424D72DB3894__INCLUDED_)
