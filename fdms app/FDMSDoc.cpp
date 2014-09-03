// FDMSDoc.cpp : implementation of the CFDMSDoc class
//

#include "stdafx.h"
#include "FDMS.h"

#include "FDMSDoc.h"

#ifdef _XJDEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFDMSDoc

IMPLEMENT_DYNCREATE(CFDMSDoc, CDocument)

BEGIN_MESSAGE_MAP(CFDMSDoc, CDocument)
	//{{AFX_MSG_MAP(CFDMSDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFDMSDoc construction/destruction

CFDMSDoc::CFDMSDoc()
{

}

CFDMSDoc::~CFDMSDoc()
{
}

BOOL CFDMSDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CFDMSDoc serialization

void CFDMSDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}

/////////////////////////////////////////////////////////////////////////////
// CFDMSDoc diagnostics

#ifdef _DEBUG
void CFDMSDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFDMSDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFDMSDoc commands


void CFDMSDoc::SetTitle(LPCTSTR lpszTitle) 
{
	CDocument::SetTitle(lpszTitle);
}
