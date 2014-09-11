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
#include "stdafx.h"
#include "FDMS.h"
#include ".\meacol.h"
#include ".\dconfig.h"

#ifdef _XJDEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMeaCol::CMeaCol(void)
{
	for (int i = 0; i < eColumnCount; i++)
	{
		m_bColumnRWMap[i] = m_szFieldData[i].bMutable;
		m_bAscending[i] = TRUE;
		columns[i] = NULL;
	}
}

CMeaCol::~CMeaCol(void)
{
}

// MS Facility	ID	Measurement type	Status	Enrichment (%)	Burnup (GWd/MT)	Discharge day	month	year	cycle #	Thres A	Thres B	 Measurement day	month	year	Cooling Time (years)	 NA	 NB	 NC	 G1	 G2 	Detector      
/*
		efhFacility, efhID,
		efhMeasType, efhStatus,
		efhEnrichment, efhBurnUp,
		efhDischDay,efhDischMonth,efhDischYear,
		efhCycle,
		efhThresholdA,		efhThresholdB, 
		efhMeasDay,efhMeasMonth,efhMeasYear,
		efhCoolingTime,
		efhNeutronA, efhNeutronB, efhNeutronC,
		efhGamma1, efhGamma2, efhDetector, efhHeaderCount
		*/
const UINT CMeaCol::m_fileheadermap[] =
{
	CMeaCol::eFacility, CMeaCol::eID, 
	CMeaCol::eMeasType, -1,
	CMeaCol::eEnrichment, CMeaCol::eBurnUp,
	CMeaCol::eDischDate, CMeaCol::eDischDate, CMeaCol::eDischDate,
	CMeaCol::eCycle,
	CMeaCol::eThresholdA,	CMeaCol::eThresholdB,
	CMeaCol::eMeasDate, CMeaCol::eMeasDate, CMeaCol::eMeasDate,
	CMeaCol::eCoolingTime,
	CMeaCol::eNeutronA, CMeaCol::eNeutronB, CMeaCol::eNeutronC,
	CMeaCol::eGamma1, CMeaCol::eGamma2, CMeaCol::eDetector
};


static const CString curdatenowtime = (COleDateTime::GetCurrentTime()).Format(VAR_DATEVALUEONLY, LANG_USER_DEFAULT );

CField::FieldDesc CMeaCol::m_szFieldData[eColumnCount] =
{
	{CField::eString, "Item",		"0",				"128", true, "%05d", "", 40, LVCFMT_LEFT, "", true},
	{CField::eString, "Facility",	"0",				"128", true, "", "", 40, LVCFMT_LEFT, "", true},
	{CField::eString, "ID",			"0",				"128", false, "", "", 32, LVCFMT_LEFT, "", true},
	{CField::eLong,		"Type",		"1",				"2", true, "%d", "%d", 10, LVCFMT_CENTER, "", true},
	{CField::eDouble, "Enrichment",	"0.0",				"100.0", true, "%.2lf", "%g", 0, LVCFMT_RIGHT, "", true},
	{CField::eDouble, "Burnup",	"0.0",				"100.0", true, "%.2lf", "%g", 10, LVCFMT_RIGHT, "", true},
	{CField::eDate, "Disch. Date",	"1 January 1950",	curdatenowtime, true, "", "", 20, LVCFMT_LEFT, "", true},
	{CField::eLong, "Cycle #",		"0",				"9", true, "%d", "%d", 10, LVCFMT_CENTER, "", true},
	{CField::eDateTime, "Meas. Date","1 January 1950",	curdatenowtime, true, "", "", 25, LVCFMT_LEFT, "", true},
	{CField::eDouble, "Cooling Time","0.0",				"1e6", true, "%.2lf", "%g", -5, LVCFMT_RIGHT, "", true},
	{CField::eDouble, "Neutron A",	"0.0",				"1e9", true, "%.2lf", "%g", 12, LVCFMT_RIGHT, "", true}, //%#6.5lf
	{CField::eDouble, "Neutron B",	"0.0",				"1e9", true, "%.2lf", "%g", 12, LVCFMT_RIGHT, "", true}, //%#6.5lf
	{CField::eDouble, "Neutron C",	"0.0",				"1e9", true, "%.2lf", "%g", 12, LVCFMT_RIGHT, "", true}, //%#7.lf
	{CField::eDouble, "Gamma 1",	"0.0",				"1e9", true, "%.2lf", "%g", 19, LVCFMT_RIGHT, "", true},
	{CField::eDouble, "Gamma 2",	"0.0",				"1e9", true, "%.2lf", "%g", 19, LVCFMT_RIGHT, "", true},
	{CField::eString, "Detector",	"0",				"128", true, "", "", 5, LVCFMT_LEFT, "", true},
	{CField::eDouble, "A (Threshold)",	"0.00",			"1e6", true, "%.2lf", "%g", -10, LVCFMT_RIGHT, "", true},
	{CField::eDouble, "B (Threshold)",	"0.00",			"1e6", true, "%.2lf", "%g", -10, LVCFMT_RIGHT, "", true},
	{CField::eLong, "internal",		"0",				"9999", false, "%d", "%d", -15, LVCFMT_CENTER, "", false}
};

void CMeaCol::ClearOut()
{
	for (int i = 0; i < eColumnCount; i++)
	{
		CField* p = NULL;
		p = columns[i];
		if (p)
			delete p;
	}
}

void CMeaCol::SetAt(int i, CField* p)
{
	columns[i] = p;
}

CField* CMeaCol::GetAt(int i)
{
	return columns[i];
}

void CMeaCol::LoadRangesFromIniData()
{
	CString Low, High;
	for (int i = eItem_RANGE;
		i < (eItem_RANGE + eColumnCount);
		i++)
	{
		GETRANGEVALUES((tIniNames)i, Low, High);
		int index = i - eItem_RANGE;

		Low.Trim(); High.Trim();
		m_szFieldData[ index ].low = Low;
		m_szFieldData[ index ].high = High;

		switch(m_szFieldData[ index ]._type)
		{
			case CField::eString:
				break;
			case CField::eDouble:
				{
					float dLow, dHigh;
					sscanf(Low, "%g", &dLow);
					sscanf(High, "%g", &dHigh);
					m_szFieldData[ index ].low.Format(m_szFieldData[ index ].format, dLow);
					m_szFieldData[ index ].high.Format(m_szFieldData[index ].format, dHigh);
				}
				break;
			case CField::eLong:
				break;
			case CField::eDate:
				break;
			case CField::eTime:
				break;
			case CField::eDateTime:
				break;
			case CField::eBoolean:
				break;
			default:
				break;
		}
	}


}
void CMeaCol::Reconstruct()
{
	LoadRangesFromIniData();
	for (int i = 0; i < eColumnCount; i++)
	{
		CField* p = NULL;
		switch(m_szFieldData[i]._type)
		{
			case CField::eString:
			case CField::eImageField:
				p = new CFieldString(m_szFieldData[i].name,m_szFieldData[i].low, m_szFieldData[i].high, m_szFieldData[i].format, m_szFieldData[i].inputFormat,128);
				SetAt(i, p);
				break;
			case CField::eDouble:
				p = new CFieldDouble(m_szFieldData[i].name,m_szFieldData[i].low, m_szFieldData[i].high, m_szFieldData[i].format, m_szFieldData[i].inputFormat);
				SetAt(i, p);
				break;
			case CField::eLong:
				p = new CFieldLong(m_szFieldData[i].name,m_szFieldData[i].low, m_szFieldData[i].high, m_szFieldData[i].format, m_szFieldData[i].inputFormat);
				SetAt(i, p);
				break;
			case CField::eDate:
				p = new CFieldDate(m_szFieldData[i].name,m_szFieldData[i].low, m_szFieldData[i].high, m_szFieldData[i].format, m_szFieldData[i].inputFormat, VAR_DATEVALUEONLY);
				SetAt(i,p);
				break;
			case CField::eTime:
				p = new CFieldDate(m_szFieldData[i].name,m_szFieldData[i].low, m_szFieldData[i].high, m_szFieldData[i].format, m_szFieldData[i].inputFormat, VAR_TIMEVALUEONLY);
				SetAt(i, p);
				break;
			case CField::eDateTime:
				p = new CFieldDate(m_szFieldData[i].name,m_szFieldData[i].low, m_szFieldData[i].high, m_szFieldData[i].format, m_szFieldData[i].inputFormat);
				SetAt(i,p);
				break;
			case CField::eBoolean:
				p = new CFieldBoolean(m_szFieldData[i].name,m_szFieldData[i].low, m_szFieldData[i].high, m_szFieldData[i].format, m_szFieldData[i].inputFormat);
				SetAt(i,p);
				break;
			default:
				p = NULL;
				break;
		}
		if (p)
			p->tooltipTemplate = m_szFieldData[i].tooltipTemplate;
	}
}


int CMeaCol::count() const
{
		return eColumnCount;
}

extern tMeasurementType tImageToMeasurementType(LPCSTR s);

bool CMyListCtrl::CopyToClipboard(bool bAll, bool bHeaders)
{
	const PTCHAR  _cediteol = "\r\n";
	if ( !OpenClipboard() )
	{
		AfxMessageBox( "Cannot open the Clipboard" );
		return true;
	}
	// Remove the current Clipboard contents
	if( !EmptyClipboard() )
	{
		AfxMessageBox( "Cannot empty the Clipboard" );
		return true;
	}
	// Get the currently selected data

	int nItem, j, tlen, headeroffset;
	const int ic = GetItemCount();
	CStringArray a;
	a.SetSize(ic);
	tlen = 0;
	headeroffset = 0;
	if (bHeaders)
	{
		CString s;
		for (j = efhFacility;  j < efhHeaderCount; j++)
		{
			UINT icol = j;  // direct mapping from header id to header string
			s.Append(CFDMSApp::GetFileHeader(CSVFileHeaderIDs(icol)));
			s.AppendChar('\t');
		}
		s.Append(_cediteol);
		a.SetAtGrow(0, s);
		tlen += s.GetLength();
		headeroffset = 1;
	}

	// now for the rows
	for (nItem = 0; nItem < ic; nItem++)
	{
		if (!bAll && !GetItemState(nItem, LVIS_SELECTED))
			continue;

		CString s = GetItemText(nItem,0);
		for (j = efhFacility; j < efhHeaderCount; j++)
		{
			if (j == efhDischMonth || j == efhDischYear)  // blend of three columns into one, skip 2
				continue;
			if (j == efhMeasMonth || j == efhMeasYear)  // blend of three columns into one, skip 2
				continue;
			UINT icol = CMeaCol::m_fileheadermap[j];
			if (j == efhDischDay || j == efhMeasDay)  //  build combined disch date
			{
				int y = 0,m = 0,d = 0;
				CString dt = GetItemText(nItem,icol);
				if (dt.IsEmpty())
				{
					dt = ("\t\t");
				}
				else
				{
					CFieldDate::XConvert3(dt, y,	m, d);
					dt.Format("%d\t%d\t%d",d,m,y);
				}
				s.Append(dt);
			}
			else
			if (j == efhStatus)  // get status from the related globals
			{
				int iData;
				iData = GetItemData(nItem);
				if (iData >= 0 && iData < MP_ARRAYSIZE)
				{
					CString cs;
					cs.Format("%d", g_iMPStatus[iData] );
					s.Append(cs); 
				}
				else
					s.Append("0"); 
			}
			else
			if (j == efhMeasType)  // convert string to number from the item text
			{
				//CString cs;
				//cs.Format("%d", tImageToMeasurementType(GetItemText(nItem,icol)));
				s.Append(GetItemText(nItem,icol));
			}
			else
				s.Append(GetItemText(nItem,icol));
			s.AppendChar('\t');
		}
		s.Append(_cediteol);
		a.SetAtGrow(nItem + headeroffset, s);
		tlen += s.GetLength();
	}

    // Allocate a global memory object for the text. 
	LPTSTR  lptstrCopy; 
	HGLOBAL hglbCopy; 
    hglbCopy = GlobalAlloc(GMEM_MOVEABLE, 
        (tlen + 1) * sizeof(TCHAR)); 
    if (hglbCopy == NULL) 
    { 
        CloseClipboard(); 
        return true; 
    } 

    // Lock the handle and copy the text to the buffer. 
    lptstrCopy = (LPSTR)GlobalLock(hglbCopy);
    lptstrCopy[0] = (TCHAR) 0;    // null character 
	for (nItem = 0; nItem < a.GetCount(); nItem++)
	{
		LPSTR b = a[nItem].GetBuffer();
		size_t l = a[nItem].GetLength() *  sizeof(TCHAR);
		strncat(lptstrCopy, b, l);
	}
    lptstrCopy[tlen] = (TCHAR) 0;    // null character 
    GlobalUnlock(hglbCopy); 

    // Place the handle on the clipboard. 

	// For the appropriate data formats...
	if ( SetClipboardData( CF_TEXT, hglbCopy ) == NULL )
	{
		AfxMessageBox( "Unable to set Clipboard data" );
		CloseClipboard();
		return true;
	}
	CloseClipboard();
		return TRUE;
		
}