/*
 *   Copyright (C)2004-2011 Sir Pustekuchen
 *
 *   Author   :  Sir Pustekuchen
 *   Home     :  http://birth-of-the-empires.de
 *
 */
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // Hauptsymbole
#include "ChatDlg.h"
#include "CommandLineParameters.h"

#define WM_INITVIEWS	(WM_APP+10)
#define WM_UPDATEVIEWS	(WM_APP+11)
#define WM_COMBATVIEW	(WM_APP+12)
#define WM_SHOWCHATDLG	(WM_APP+13)

/////////////////////////////////////////////////////////////////////////////
// CBotf2App:
// Siehe botf2.cpp f�r die Implementierung dieser Klasse
class CBotf2App : public CWinApp
{
private:
	CChatDlg *m_pChatDlg;
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	CCommandLineParameters m_clp;

public:
	/// Konstruktor
	CBotf2App();

	/// Destruktor
	virtual ~CBotf2App();

	/// Funktion gibt einen Zeiger auf das zugeh�rige Dokument der SDI Anwendung zur�ck.
	CBotf2Doc* GetDocument() const;

	/// Funktion gibt einen Zeiger auf den ChatDialog zur�ck.
	CChatDlg* GetChatDlg() {return m_pChatDlg;}
	CCommandLineParameters const* const GetCommandLineParameters() const {
		return &m_clp;
	}

// �berschreibungen
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

private:
// Implementierung
	DECLARE_MESSAGE_MAP()
	afx_msg void OnAppAbout();
	afx_msg void InitViews(WPARAM, LPARAM);
	afx_msg void UpdateViews(WPARAM, LPARAM);
	afx_msg void ShowChatDlg(WPARAM, LPARAM);
	afx_msg void OnSettings();
	afx_msg void OnChat();
	afx_msg void OnOpenManual();
	afx_msg void OnHelpWiki();
	afx_msg void OnOpenReadme();
	afx_msg void OnOpenCredits();
};
