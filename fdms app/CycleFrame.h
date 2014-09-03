#pragma once

#include "childfrm.h"


class CCycleFrame : public CChildFrame
{
	DECLARE_DYNCREATE(CCycleFrame)
public:
	CCycleFrame();

// Implementation
public:
	virtual ~CCycleFrame();

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCycleFrame)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnUpdateFrameMenu(BOOL bActive, CWnd* pActiveWnd, HMENU hMenuAlt);
	static bool m_bMenuEnhanced;
};

class CNvBUFrame : public CChildFrame
{
	DECLARE_DYNCREATE(CNvBUFrame)
public:
	CNvBUFrame();

// Implementation
public:
	virtual ~CNvBUFrame();

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CNvBUFrame)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnUpdateFrameMenu(BOOL bActive, CWnd* pActiveWnd, HMENU hMenuAlt);
	static bool m_bMenuEnhanced;
};

class CNvGFrame : public CChildFrame
{
	DECLARE_DYNCREATE(CNvGFrame)
public:
	CNvGFrame();

// Implementation
public:
	virtual ~CNvGFrame();

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CNvGFrame)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnUpdateFrameMenu(BOOL bActive, CWnd* pActiveWnd, HMENU hMenuAlt);
	static bool m_bMenuEnhanced;
};


class CGBUvTFrame : public CChildFrame
{
	DECLARE_DYNCREATE(CGBUvTFrame)
public:
	CGBUvTFrame();

// Implementation
public:
	virtual ~CGBUvTFrame();

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGBUvTFrame)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnUpdateFrameMenu(BOOL bActive, CWnd* pActiveWnd, HMENU hMenuAlt);
	static bool m_bMenuEnhanced;
};