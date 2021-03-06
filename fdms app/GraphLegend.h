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
//GraphLegend.h - Version 3.0

#if !defined(AFX_GRAPHLEGEND_H__99A8A552_4029_40CC_839A_D8210C46CD8B__INCLUDED_)
#define AFX_GRAPHLEGEND_H__99A8A552_4029_40CC_839A_D8210C46CD8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GraphLegend.h : header file
//

#include "GraphLegendSet.h"

/////////////////////////////////////////////////////////////////////////////
// CGraphLegend window

class CGraphLegend : public CStatic
{
// Construction
public:
	CGraphLegend();

// Attributes
public:


private:
	int legendLength;
	int m_iCount;
	CObList *legendLabels;
	CObList *legendDoDraw;

// Operations
public:
	void SetLegendText(int datagroup, CString label, COLORREF cColor, int iStyle, bool bDoShow = true);
//	void SetColor(COLORREF cColor){m_cColor = cColor;};
//	void SetStyle(int iStyle){m_iStyle = iStyle;};
//	COLORREF GetColor(){return m_cColor;};
//	int GetStyle(){return m_iStyle;};
	CString GetLegendText(int datagroup);
	COLORREF GetLegendColor(int datagroup);
	int GetLegendStyle(int datagroup);
	int GetLegendLength();
	int GetLegendCount(){return m_iCount;};
	bool GetLegendDoDraw(int datagroup);
	void SetLegendDoDraw(int datagroup, bool bDoDraw);
	void ToggleLegendDoDraw(int datagroup);
	void Remove(int dataGroup);
	void RemoveAll();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphLegend)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGraphLegend();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGraphLegend)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHLEGEND_H__99A8A552_4029_40CC_839A_D8210C46CD8B__INCLUDED_)
