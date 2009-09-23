/*
 *   Copyright (C)2004-2009 Sir Pustekuchen
 *
 *   Author   :  Sir Pustekuchen
 *   Home     :  http://birth-of-the-empires.de.vu
 *
 */
#pragma once
#include "BottomBaseView.h"

// CPlanetBottomView view

class CPlanetBottomView : public CBottomBaseView
{
	DECLARE_DYNCREATE(CPlanetBottomView)

protected:
	CPlanetBottomView();           // protected constructor used by dynamic creation
	virtual ~CPlanetBottomView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

private:
	// Funktionen

	// Attribute
	CRect* m_arroundThePlanets;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


