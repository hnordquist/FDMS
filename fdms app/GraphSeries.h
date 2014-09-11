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
//GraphSeries.h - Version 3.0

#if !defined(AFX_GRAPHSERIES_H__A059628E_F6C8_4DA0_9E2F_820131D257E7__INCLUDED_)
#define AFX_GRAPHSERIES_H__A059628E_F6C8_4DA0_9E2F_820131D257E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GraphSeries.h : header file
//

#include "GraphDataSet.h"

/////////////////////////////////////////////////////////////////////////////
// CGraphSeries window

class CGraphSeries : public CStatic
{
// Construction
public:
	CGraphSeries();
	CGraphSeries(int iStyle, COLORREF cColor = RGB(0,0,0), bool bDoLines = true, int iGroup = 0);

// Attributes
public:


private:
	CObList *dataValues;	//list of CGraphDataSet objects
	bool bIsInteger;
	bool m_bClip;
	int m_iStyle;
	int m_iLineStyle;
	int	m_iLineWidth;
	COLORREF m_cColor;
	bool m_bDoLines;
	POSITION m_pIterateStart;
	int m_iGroup;
	bool m_bDoDisplay;
	bool m_bSelectable;
	CString seriesLabel;
	CString m_cSeriesLabelX;
	CString m_cSeriesLabelY;

// Operations
public:
	bool GetDoLines(){return m_bDoLines;};
	void SetDoLines(bool bDoLines){m_bDoLines = bDoLines;};
	COLORREF GetColor(){return m_cColor;};
	bool CGraphSeries::IterateGet(double *xValue, double *yValue);
	bool IsInteger(){return bIsInteger;};
	void SetDouble(){bIsInteger = false;};
	void SetLabel(CString label);
	void SetData(int xValue, int yValue);
	void SetData(double xValue, double yValue);
	void SetStyle(int iStyle){m_iStyle = iStyle;};
	void SetLineStyle(int iStyle, int iWidth = 1){m_iLineStyle = iStyle;m_iLineWidth = iWidth;};
	int  GetLineWidth(){return m_iLineWidth;};
	void SetColor(COLORREF cColor){m_cColor = cColor;};
	int  GetStyle(){return m_iStyle;};
	int  GetLineStyle(){return m_iLineStyle;};
	void SetGroup(int iGroup) { m_iGroup = iGroup;};
	int  GetGroup() {return m_iGroup;};
	bool GetDoDisplay(){return m_bDoDisplay;};
	bool GetSelectable(){return m_bSelectable;};
	void SetSelectable(bool bSelectable){m_bSelectable = bSelectable;};
	void SetClip(bool bClip) { m_bClip = bClip;};
	bool GetClip() {return m_bClip;};
	void ToggleDisplayOnEqGroup(int iGroup) { 
		if (iGroup == m_iGroup)
			m_bDoDisplay = !m_bDoDisplay;
	}
	CString GetLabel();
	CString GetLabelX(){return m_cSeriesLabelX;};
	CString GetLabelY(){return m_cSeriesLabelY;};
	void SetLabelX(CString cLabel){m_cSeriesLabelX = cLabel;};
	void SetLabelY(CString cLabel){m_cSeriesLabelY = cLabel;};
	int GetData(int xValue);
	double GetData(double xValue);
	int GetDataCount();
	int GetXDataValue();	//for scatter graphs only
	double GetdXDataValue();
	int GetYDataValue();	//for scatter graphs only
	double GetdYDataValue();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphSeries)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGraphSeries();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGraphSeries)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHSERIES_H__A059628E_F6C8_4DA0_9E2F_820131D257E7__INCLUDED_)
