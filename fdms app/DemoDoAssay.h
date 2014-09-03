//////////////////////////////////////////////////////////////////////////
// Copyright 2004, The Regents Of the University of California.  This
// software was produced under U.S. Government contract (W-7405-ENG-36) at
// the Los Alamos National Laboratory by the Safeguards Science and
// Technology Group (N-1).  All Rights Reserved.
//////////////////////////////////////////////////////////////////////////
// $History: DemoDoAssay.h $
// 
// *****************  Version 1  *****************
// User: Longo        Date: 12/10/04   Time: 4:48p
// Created in $/FDMS/FDMS Application/FDMSNET
// Combining the GrandCom demo class wirth the SerialCom live detector
// control class
//////////////////////////////////////////////////////////////////////////
#pragma once

#include ".\DoAssay.h"

class DemoDoAssay : public CDoAssay
{
	DECLARE_DYNAMIC(DemoDoAssay )
// Construction
public:
	DemoDoAssay(CWnd* pParent = NULL);   // standard constructor
	~DemoDoAssay();

	protected:
	char	m_cMode[32];
	virtual void	UpdateParams(char*);
	virtual void	SetModeForUpdateParams();
	virtual void	CreateTheThread();

	DECLARE_MESSAGE_MAP()

};