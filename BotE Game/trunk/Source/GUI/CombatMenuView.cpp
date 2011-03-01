// EventMenuView.cpp : implementation file
//

#include "stdafx.h"
#include "botf2.h"
#include "Botf2Doc.h"
#include "CombatMenuView.h"
#include "Races\RaceController.h"
#include "Ships\Combat.h"
#include "Ships\Fleet.h"
// CCombatMenuView

IMPLEMENT_DYNCREATE(CCombatMenuView, CMainBaseView)

CCombatMenuView::CCombatMenuView()
{
}

CCombatMenuView::~CCombatMenuView()
{
	for (int i = 0; i < m_CombatDecisionButtons.GetSize(); i++)
	{
		delete m_CombatDecisionButtons[i];
		m_CombatDecisionButtons[i] = NULL;
	}
	m_CombatDecisionButtons.RemoveAll();

	for (int i = 0; i < m_CombatTacticButtons.GetSize(); i++)
	{
		delete m_CombatTacticButtons[i];
		m_CombatTacticButtons[i] = NULL;
	}
	m_CombatTacticButtons.RemoveAll();

	for (int i = 0; i < m_CombatOrderButtons.GetSize(); i++)
	{
		delete m_CombatOrderButtons[i];
		m_CombatOrderButtons[i] = NULL;
	}
	m_CombatOrderButtons.RemoveAll();

	for (int i = 0; i < m_FriendShipsCursor.GetSize(); i++)
	{
		delete m_FriendShipsCursor[i];
		m_FriendShipsCursor[i] = NULL;
	}
	m_FriendShipsCursor.RemoveAll();

	for (int i = 0; i < m_EnemyShipsCursor.GetSize(); i++)
	{
		delete m_EnemyShipsCursor[i];
		m_EnemyShipsCursor[i] = NULL;
	}
	m_EnemyShipsCursor.RemoveAll();

	for (int i = 0; i < m_ShipTypeButton.GetSize(); i++)
	{
		delete m_ShipTypeButton[i];
		m_ShipTypeButton[i] = NULL;
	}
	m_ShipTypeButton.RemoveAll();
}

BEGIN_MESSAGE_MAP(CCombatMenuView, CMainBaseView)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

void CCombatMenuView::OnNewRound()
{
	// angeklickten Button zur�cksetzen
	for (int i = 0; i < m_CombatDecisionButtons.GetSize(); i++)
		m_CombatDecisionButtons[i]->SetState(0);

	for (int i = 0; i < m_CombatTacticButtons.GetSize(); i++)
		m_CombatTacticButtons[i]->SetState(0);
	// ersten Button aktivieren
	m_CombatTacticButtons[0]->SetState(2);

	for (int i = 0; i < m_CombatOrderButtons.GetSize(); i++)
		m_CombatOrderButtons[i]->SetState(0);

	// kleine Buttons deaktivieren
	for (int i = 0; i < m_FriendShipsCursor.GetSize(); i++)
		m_FriendShipsCursor[i]->SetState(2);
	for (int i = 0; i < m_EnemyShipsCursor.GetSize(); i++)
		m_EnemyShipsCursor[i]->SetState(2);
	
	m_bInOrderMenu = false;
	m_vInvolvedShips.RemoveAll();
	m_sFriends.clear();
	m_sEnemies.clear();
	m_nPageFriends = 0;
	m_nPageEnemies = 0;
	m_vShipRects.clear();
	m_pMarkedShip = NULL;
	m_rLastMarkedRect = CRect(0,0,0,0);
	m_nShipType = -1;

	m_ShipTypeButton[0]->SetText(CResourceManager::GetString("ALL_SHIPS"));
}

// CEventMenuView drawing

void CCombatMenuView::OnDraw(CDC* dc)
{
	SetFocus();
	CBotf2Doc* pDoc = (CBotf2Doc*)GetDocument();
	ASSERT(pDoc);

	if (!pDoc->m_bDataReceived)
		return;

	CMajor* pMajor = m_pPlayersRace;
	ASSERT(pMajor);
	if (!pMajor)
		return;

	// TODO: add draw code here
	CMemDC pDC(dc);
	CRect client;
	GetClientRect(&client);
		
	// Graphicsobjekt, in welches gezeichnet wird anlegen
	Graphics g(pDC->GetSafeHdc());
	
	g.Clear(Color::Black);
	g.SetSmoothingMode(SmoothingModeHighSpeed);
	g.SetInterpolationMode(InterpolationModeLowQuality);
	g.SetPixelOffsetMode(PixelOffsetModeHighSpeed);
	g.SetCompositingQuality(CompositingQualityHighSpeed);
	g.ScaleTransform((REAL)client.Width() / (REAL)m_TotalSize.cx, (REAL)client.Height() / (REAL)m_TotalSize.cy);
	
	if (pDoc->m_bRoundEndPressed)
	{
		DrawCombatInfoMenue(&g);
		return;
	}

	// beteiligte Schiff sammeln
	m_vInvolvedShips.RemoveAll();
	for (int i = 0; i < pDoc->m_ShipArray.GetSize(); i++)
	{
		CShip* pShip = &pDoc->m_ShipArray[i];
		if (pShip->GetKO() != pDoc->m_ptCurrentCombatSector)
			continue;

		m_vInvolvedShips.Add(pShip);
		// Wenn das Schiff eine Flotte anf�hrt, dann auch die Zeiger auf die Schiffe in der Flotte reingeben
		if (pShip->GetFleet())
			for (int j = 0; j < pShip->GetFleet()->GetFleetSize(); j++)
				m_vInvolvedShips.Add(pShip->GetFleet()->GetShipFromFleet(j));
	}
	
	// grobe prozentuale Kampfchance und beteiligte Rassen berechnen
	const CAnomaly* pAnomaly = pDoc->GetSector(pDoc->m_ptCurrentCombatSector).GetAnomaly();
	m_dWinningChance = CCombat::GetWinningChance(pMajor, m_vInvolvedShips, pDoc->GetRaceCtrl()->GetRaces(), m_sFriends, m_sEnemies, pAnomaly);

	m_dWinningChance = min(0.99, m_dWinningChance);
	m_dWinningChance = max(0.01, m_dWinningChance);

	if (m_sFriends.find(m_pPlayersRace) != m_sFriends.end())
	{
		if (!m_bInOrderMenu)
			DrawCombatDecisionMenue(&g);
		else
			DrawCombatOrderMenue(&g);
	}
	else
		DrawCombatInfoMenue(&g);

	g.ReleaseHDC(pDC->GetSafeHdc());
}

// CCombatMenuView diagnostics

#ifdef _DEBUG
void CCombatMenuView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CCombatMenuView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCombatMenuView message handlers

void CCombatMenuView::OnInitialUpdate()
{
	CMainBaseView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	CBotf2Doc* pDoc = (CBotf2Doc*)GetDocument();

	m_TotalSize = CSize(1280, 1024);

	CMajor* pMajor = m_pPlayersRace;
	ASSERT(pMajor);
	CString sPrefix = pMajor->GetPrefix();
	
	bg_combatdecmenu	= pDoc->GetGraphicPool()->GetGDIGraphic("Events\\CombatDec.boj");
	bg_combatinfomenu	= pDoc->GetGraphicPool()->GetGDIGraphic("Events\\CombatInfo.boj");
	bg_combatordermenu	= pDoc->GetGraphicPool()->GetGDIGraphic("Events\\CombatOrder.boj");
	
	m_bInOrderMenu = false;

	CreateButtons();
	
	// View bei den Tooltipps anmelden
	pDoc->GetMainFrame()->AddToTooltip(this);
}

void CCombatMenuView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: Add your specialized code here and/or call the base class
	CMainBaseView::OnPrepareDC(pDC, pInfo);
}

BOOL CCombatMenuView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;	
}

/////////////////////////////////////////////////////////////////////////////////////////
// Hier die Funktion zum Zeichnen des Nachrichtenmen�s
/////////////////////////////////////////////////////////////////////////////////////////
void CCombatMenuView::DrawCombatDecisionMenue(Graphics* g)
{	
	CBotf2Doc* pDoc = (CBotf2Doc*)GetDocument();
	ASSERT(pDoc);

	CMajor* pMajor = m_pPlayersRace;
	ASSERT(pMajor);
	if (!pMajor)
		return;
	
	CString fontName = "";
	Gdiplus::REAL fontSize = 0.0;
	
	// Rassenspezifische Schriftart ausw�hlen
	CFontLoader::CreateGDIFont(pMajor, 2, fontName, fontSize);
	// Schriftfarbe w�hlen
	Gdiplus::Color normalColor;
	CFontLoader::GetGDIFontColor(pMajor, 3, normalColor);
	SolidBrush fontBrush(normalColor);

	StringFormat fontFormat;
	fontFormat.SetAlignment(StringAlignmentCenter);
	fontFormat.SetLineAlignment(StringAlignmentCenter);
	fontFormat.SetFormatFlags(StringFormatFlagsNoWrap);

	if (bg_combatdecmenu)
		g->DrawImage(bg_combatdecmenu, 0, 0, 1280, 1024);

	Gdiplus::Color markColor;
	markColor.SetFromCOLORREF(pMajor->GetDesign()->m_clrListMarkTextColor);
	fontBrush.SetColor(markColor);

	CPoint p = pDoc->m_ptCurrentCombatSector;

	// Beteiligte Rassen darstellen...
	// Zuerst uns selbst und all unsere Verb�ndeten zeichnen
	int nPosX = m_TotalSize.cx / 2 - 100 - ((m_sFriends.size() - 1) * 220) / 2;
	for (std::set<const CRace*>::const_iterator it = m_sFriends.begin(); it != m_sFriends.end(); ++it)
	{
		const CRace* pRace = *it;
		Bitmap* graphic	= pDoc->GetGraphicPool()->GetGDIGraphic("Races\\" + pRace->GetGraphicFileName());
		if (graphic == NULL)
			graphic = pDoc->GetGraphicPool()->GetGDIGraphic("Races\\ImageMissing.boj");
		if (graphic)
			g->DrawImage(graphic, nPosX, 125, 200, 200);
		
		g->DrawRectangle(&Pen(Color(200,200,200), 2), nPosX, 125, 200, 200);
		// Rassenname �ber das Bild zeichnen
		g->DrawString(pRace->GetRaceName().AllocSysString(), -1, &Gdiplus::Font(fontName.AllocSysString(), fontSize), RectF(nPosX,100,200,25), &fontFormat, &fontBrush);

		// Rassensymbol noch in die Ecke zeichnen
		graphic	= pDoc->GetGraphicPool()->GetGDIGraphic("Symbols\\" + pRace->GetRaceID() + ".bop");
		if (graphic)
			g->DrawImage(graphic, nPosX + 125, 250, 75, 75);
		
		nPosX += 220;
	}
	Bitmap* graphic = NULL;

	// dann alle gegnerischen Rassen zeichnen
	nPosX = m_TotalSize.cx / 2 - 100 - ((m_sEnemies.size() - 1) * 220) / 2;
	for (std::set<const CRace*>::const_iterator it = m_sEnemies.begin(); it != m_sEnemies.end(); ++it)
	{
		const CRace* pRace = *it;
		graphic	= pDoc->GetGraphicPool()->GetGDIGraphic("Races\\" + pRace->GetGraphicFileName());
		if (graphic == NULL)
			graphic = pDoc->GetGraphicPool()->GetGDIGraphic("Races\\ImageMissing.boj");
		if (graphic)
			g->DrawImage(graphic, nPosX, 450, 200, 200);			
		
		g->DrawRectangle(&Pen(Color(200,200,200), 2), nPosX, 450, 200, 200);
		// Rassenname �ber das Bild zeichnen
		g->DrawString(pRace->GetRaceName().AllocSysString(), -1, &Gdiplus::Font(fontName.AllocSysString(), fontSize), RectF(nPosX,425,200,25), &fontFormat, &fontBrush);

		// Rassensymbol noch in die Ecke zeichnen
		graphic	= pDoc->GetGraphicPool()->GetGDIGraphic("Symbols\\" + pRace->GetRaceID() + ".bop");
		if (graphic)
			g->DrawImage(graphic, nPosX + 125, 575, 75, 75);
		
		nPosX += 220;
	}
	
	// gr��te Schriftart laden
	CFontLoader::CreateGDIFont(pMajor, 5, fontName, fontSize);
	fontSize *= 1.5;
	fontBrush.SetColor(normalColor);
	g->DrawString(CResourceManager::GetString("AGAINST").AllocSysString(), -1, &Gdiplus::Font(fontName.AllocSysString(), fontSize), RectF(0,325,m_TotalSize.cx,100), &fontFormat, &fontBrush);

	// Gewinnchance zeichnen
	CString s;
	s.Format("%s: %.0lf%%", CResourceManager::GetString("WINNING_CHANCE"), m_dWinningChance * 100);
	bool bFlipHand = false;
	if (m_dWinningChance < 0.4)
	{
		graphic	= pDoc->GetGraphicPool()->GetGDIGraphic("Other\\win0.bop");
		fontBrush.SetColor(Color(200,0,0));
	}
	else if (m_dWinningChance < 0.6)
	{
		fontBrush.SetColor(Color(255,100,0));
		graphic	= pDoc->GetGraphicPool()->GetGDIGraphic("Other\\win40.bop");
		bFlipHand = true;
	}
	else if (m_dWinningChance < 0.75)
	{
		fontBrush.SetColor(Color(255,225,0));
		graphic	= pDoc->GetGraphicPool()->GetGDIGraphic("Other\\win60.bop");
		bFlipHand = true;
	}
	else if (m_dWinningChance < 0.9)
	{
		fontBrush.SetColor(Color(200,225,0));
		graphic	= pDoc->GetGraphicPool()->GetGDIGraphic("Other\\win75.bop");
	}
	else
	{
		fontBrush.SetColor(Color(0,200,0));
		graphic	= pDoc->GetGraphicPool()->GetGDIGraphic("Other\\win90.bop");
	}
	if (graphic)
	{
		g->DrawImage(graphic, 280, 680, 150, 200);

		// bei fast gleichstand einen Daumen hoch, den anderen runter
		if (bFlipHand)
		{
			graphic = graphic->Clone(0, 0, graphic->GetWidth(), graphic->GetHeight(), PixelFormat32bppARGB);
			graphic->RotateFlip(RotateNoneFlipY);
		}
		g->DrawImage(graphic, 850, 680, 150, 200);
	}

	g->DrawString(s.AllocSysString(), -1, &Gdiplus::Font(fontName.AllocSysString(), fontSize), RectF(0,680,m_TotalSize.cx,200), &fontFormat, &fontBrush);
	
	// Schriftart f�r gro�e Buttons laden
	CFontLoader::CreateGDIFont(pMajor, 3, fontName, fontSize);
	// Schriftfarbe w�hlen
	Gdiplus::Color btnColor;
	CFontLoader::GetGDIFontColor(pMajor, 2, btnColor);
	SolidBrush btnBrush(btnColor);
	// Buttons f�r Kampfentscheidung zeichnen
	DrawGDIButtons(g, &m_CombatDecisionButtons, -1, Gdiplus::Font(fontName.AllocSysString(), fontSize), btnBrush);
		
	// �berschrift in der Mitte zeichnen
	// Rassenspezifische Schriftart ausw�hlen
	CFontLoader::CreateGDIFont(pMajor, 5, fontName, fontSize);
	// Schriftfarbe w�hlen
	CFontLoader::GetGDIFontColor(pMajor, 3, normalColor);
	fontBrush.SetColor(normalColor);
	
	if (pDoc->GetSector(p).GetKnown(pMajor->GetRaceID()))
		s.Format("%s %s", CResourceManager::GetString("COMBAT_IN_SECTOR"), pDoc->GetSector(p).GetName(TRUE));
	else
		s.Format("%s %c%i", CResourceManager::GetString("COMBAT_IN_SECTOR"), (char)(p.y+97), p.x + 1);
	g->DrawString(s.AllocSysString(), -1, &Gdiplus::Font(fontName.AllocSysString(), fontSize), RectF(0,10,m_TotalSize.cx, 50), &fontFormat, &fontBrush);
}

void CCombatMenuView::DrawCombatInfoMenue(Graphics* g)
{	
	CBotf2Doc* pDoc = (CBotf2Doc*)GetDocument();
	ASSERT(pDoc);

	CMajor* pMajor = m_pPlayersRace;
	ASSERT(pMajor);
	if (!pMajor)
		return;
	
	CString fontName = "";
	Gdiplus::REAL fontSize = 0.0;
	
	// Rassenspezifische Schriftart ausw�hlen
	CFontLoader::CreateGDIFont(pMajor, 2, fontName, fontSize);
	// Schriftfarbe w�hlen
	Gdiplus::Color normalColor;
	CFontLoader::GetGDIFontColor(pMajor, 3, normalColor);
	SolidBrush fontBrush(normalColor);

	StringFormat fontFormat;
	fontFormat.SetAlignment(StringAlignmentCenter);
	fontFormat.SetLineAlignment(StringAlignmentCenter);
	fontFormat.SetFormatFlags(!StringFormatFlagsNoWrap);

	if (bg_combatinfomenu)
		g->DrawImage(bg_combatinfomenu, 0, 0, 1280, 1024);

	// gr��te Schriftart laden
	CFontLoader::CreateGDIFont(pMajor, 5, fontName, fontSize);
	CString s;
	s.Format("%s ...", CResourceManager::GetString("PLEASE_WAIT"));
	g->DrawString(s.AllocSysString(), -1, &Gdiplus::Font(fontName.AllocSysString(), fontSize), RectF(0,m_TotalSize.cy / 2,m_TotalSize.cx,m_TotalSize.cy / 2), &fontFormat, &fontBrush);
	fontSize *= 1.5;
	if (pDoc->m_bRoundEndPressed)
		g->DrawString(CResourceManager::GetString("COMBATCALCULATION_IS_RUNNING").AllocSysString(), -1, &Gdiplus::Font(fontName.AllocSysString(), fontSize), RectF(0,0,m_TotalSize.cx,m_TotalSize.cy), &fontFormat, &fontBrush);
	else
		g->DrawString(CResourceManager::GetString("OTHER_PLAYERS_IN_COMBAT").AllocSysString(), -1, &Gdiplus::Font(fontName.AllocSysString(), fontSize), RectF(0,0,m_TotalSize.cx,m_TotalSize.cy), &fontFormat, &fontBrush);

	if (!pDoc->m_bRoundEndPressed)
	{
		// keine Kampfbeteiligung
		pDoc->m_nCombatOrder = COMBAT_NON;
		Sleep(250);
		pDoc->m_bRoundEndPressed = true;
		client.EndOfRound(pDoc);		
	}	
}

void CCombatMenuView::DrawCombatOrderMenue(Graphics* g)
{	
	m_vShipRects.clear();

	CBotf2Doc* pDoc = (CBotf2Doc*)GetDocument();
	ASSERT(pDoc);

	CMajor* pMajor = m_pPlayersRace;
	ASSERT(pMajor);
	if (!pMajor)
		return;
	
	CString fontName = "";
	Gdiplus::REAL fontSize = 0.0;
	
	// Rassenspezifische Schriftart ausw�hlen
	CFontLoader::CreateGDIFont(pMajor, 2, fontName, fontSize);
	// Schriftfarbe w�hlen
	Gdiplus::Color normalColor;
	CFontLoader::GetGDIFontColor(pMajor, 3, normalColor);
	SolidBrush fontBrush(normalColor);

	StringFormat fontFormat;
	fontFormat.SetAlignment(StringAlignmentCenter);
	fontFormat.SetLineAlignment(StringAlignmentCenter);
	fontFormat.SetFormatFlags(!StringFormatFlagsNoWrap);

	if (bg_combatinfomenu)
		g->DrawImage(bg_combatordermenu, 0, 0, 1280, 1024);

	CPoint p = pDoc->m_ptCurrentCombatSector;

	g->SetSmoothingMode(SmoothingModeHighQuality);
	g->SetInterpolationMode(InterpolationModeLowQuality);
	g->SetPixelOffsetMode(PixelOffsetModeHighSpeed);
	g->SetCompositingQuality(CompositingQualityHighSpeed);

	int nRow = 0;
	int nCol = 0;
	int nCounter = 0;
	for (int i = 0; i < m_vInvolvedShips.GetSize(); i++)
	{
		CShip* pShip = m_vInvolvedShips[i];
		if (m_nShipType != -1 && pShip->GetShipType() != m_nShipType)
			continue;

		CRace* pRace = pDoc->GetRaceCtrl()->GetRace(pShip->GetOwnerOfShip());

		if (!pRace || m_sFriends.find(pRace) == m_sFriends.end())
			continue;

		nCounter++;		
		if (nCounter <= m_nPageFriends * 18)
			continue;

		if (nCol == 2)
		{
			// Button f�r Weiter aktivieren
			if (m_FriendShipsCursor[1]->GetState() == 2)
				m_FriendShipsCursor[1]->SetState(0);
			break;
		}
		
		CPoint pt(50 + 225 * nCol, 255 + 65 * nRow);
		bool bMarked = pShip == m_pMarkedShip;
		pShip->DrawShip(g, pDoc->GetGraphicPool(), pt, bMarked, false, false, normalColor, normalColor, Gdiplus::Font(fontName.AllocSysString(), fontSize));
		// aktueller Schiffsbefehl anzeigen
		if (pShip->GetOwnerOfShip() == pMajor->GetRaceID())
		{
			Bitmap* graphic = NULL;
			switch (pShip->GetCombatTactic())
			{
			case COMBAT_TACTIC_ATTACK:	graphic	= pDoc->GetGraphicPool()->GetGDIGraphic("Other\\tactic_attack.bop"); break;
			case COMBAT_TACTIC_AVOID:	graphic	= pDoc->GetGraphicPool()->GetGDIGraphic("Other\\tactic_avoid.bop"); break;
			case COMBAT_TACTIC_RETREAT: graphic	= pDoc->GetGraphicPool()->GetGDIGraphic("Other\\tactic_retreat.bop"); break;
			}
			
			if (graphic)
				g->DrawImage(graphic, pt.x, pt.y + 40, 30, 30);
		}
		
		m_vShipRects.push_back(pair<CRect, CShip*>(CRect(pt.x, pt.y + 20, pt.x + 250, pt.y + 85), pShip));
		nRow++;		
		
		if (nRow%9 == 0)
		{
			nRow = 0;
			nCol++;
		}
	}

	if (m_nPageFriends > 0)
	{
		// Button f�r zur�ck einblenden
		if (m_FriendShipsCursor[0]->GetState() == 2)
			m_FriendShipsCursor[0]->SetState(0);
	}

	nRow = 0;
	nCol = 0;
	nCounter = 0;
	for (int i = 0; i < m_vInvolvedShips.GetSize(); i++)
	{
		CShip* pShip = m_vInvolvedShips[i];
		if (m_nShipType != -1 && pShip->GetShipType() != m_nShipType)
			continue;

		CRace* pRace = pDoc->GetRaceCtrl()->GetRace(pShip->GetOwnerOfShip());
				
		if (!pRace || m_sEnemies.find(pRace) == m_sEnemies.end())
			continue;

		nCounter++;		
		if (nCounter <= m_nPageEnemies * 18)
			continue;

		if (nCol == 2)
		{
			// Button f�r Weiter aktivieren
			if (m_EnemyShipsCursor[1]->GetState() == 2)
				m_EnemyShipsCursor[1]->SetState(0);
			break;
		}
		
		CPoint pt(750 + 225 * nCol, 255 + 65 * nRow);
		bool bMarked = pShip == m_pMarkedShip;
		pShip->DrawShip(g, pDoc->GetGraphicPool(), pt, bMarked, false, false, normalColor, normalColor, Gdiplus::Font(fontName.AllocSysString(), fontSize));
		m_vShipRects.push_back(pair<CRect, CShip*>(CRect(pt.x, pt.y + 20, pt.x + 250, pt.y + 85), pShip));
		nRow++;		

		if (nRow%9 == 0)
		{
			nRow = 0;
			nCol++;
		}
	}

	if (m_nPageEnemies > 0)
	{
		// Button f�r zur�ck einblenden
		if (m_EnemyShipsCursor[0]->GetState() == 2)
			m_EnemyShipsCursor[0]->SetState(0);
	}

	// Beschreibungstext hinzuf�gen
	CString s;
	s = CResourceManager::GetString("CHOOSE_TACTIC_DESC1") + "\n" + CResourceManager::GetString("CHOOSE_TACTIC_DESC2") + "\n" + CResourceManager::GetString("CHOOSE_TACTIC_DESC3");
	g->DrawString(s.AllocSysString(), -1, &Gdiplus::Font(fontName.AllocSysString(), fontSize), RectF(0, 170, m_TotalSize.cx, 75), &fontFormat, &fontBrush);
	s = CResourceManager::GetString("SHOWN_SHIPTYPES");
	g->DrawString(s.AllocSysString(), -1, &Gdiplus::Font(fontName.AllocSysString(), fontSize), RectF(0, 255, m_TotalSize.cx, 25), &fontFormat, &fontBrush);

	// gr��te Schriftart laden und VS in der Mitte zeichnen
	CFontLoader::CreateGDIFont(pMajor, 5, fontName, fontSize);
	fontSize *= 2.0;
	g->DrawString(CResourceManager::GetString("VS").AllocSysString(), -1, &Gdiplus::Font(fontName.AllocSysString(), fontSize), RectF(0,65,m_TotalSize.cx,m_TotalSize.cy), &fontFormat, &fontBrush);
	
	// Schriftart f�r kleine Buttons laden
	CFontLoader::CreateGDIFont(pMajor, 2, fontName, fontSize);

	// �berschriften �ber den Schiffsdarstellungen zeichnen
	Gdiplus::Color markColor;
	markColor.SetFromCOLORREF(pMajor->GetDesign()->m_clrListMarkTextColor);
	fontBrush.SetColor(markColor);
	
	g->DrawString(CResourceManager::GetString("FRIENDLY_SHIPS").AllocSysString(), -1, &Gdiplus::Font(fontName.AllocSysString(), fontSize), RectF(50,255,480,25), &fontFormat, &fontBrush);
	g->DrawString(CResourceManager::GetString("ENEMY_SHIPS").AllocSysString(), -1, &Gdiplus::Font(fontName.AllocSysString(), fontSize), RectF(750,255,480,25), &fontFormat, &fontBrush);

	// Schriftfarbe w�hlen
	Gdiplus::Color btnColor;
	CFontLoader::GetGDIFontColor(pMajor, 2, btnColor);
	SolidBrush btnBrush(btnColor);
	// Buttons f�r vor und zur�ck zeichnen
	DrawGDIButtons(g, &m_FriendShipsCursor, -1, Gdiplus::Font(fontName.AllocSysString(), fontSize), btnBrush);
	DrawGDIButtons(g, &m_EnemyShipsCursor, -1, Gdiplus::Font(fontName.AllocSysString(), fontSize), btnBrush);
	// Button f�r Schiffstypwechsel zeichnen
	DrawGDIButtons(g, &m_ShipTypeButton, -1, Gdiplus::Font(fontName.AllocSysString(), fontSize), btnBrush);

	// Schriftart f�r gro�e Buttons laden
	CFontLoader::CreateGDIFont(pMajor, 3, fontName, fontSize);
	// Schriftfarbe w�hlen
	CFontLoader::GetGDIFontColor(pMajor, 2, btnColor);
	btnBrush.SetColor(btnColor);
	// Buttons f�r Kampfentscheidung zeichnen
	DrawGDIButtons(g, &m_CombatTacticButtons, -1, Gdiplus::Font(fontName.AllocSysString(), fontSize), btnBrush);
	DrawGDIButtons(g, &m_CombatOrderButtons, -1, Gdiplus::Font(fontName.AllocSysString(), fontSize), btnBrush);
	// Grafik des Taktiksymbols zentriert �ber den Taktikbuttons zeichnen
	Bitmap* graphic = NULL;
	graphic	= pDoc->GetGraphicPool()->GetGDIGraphic("Other\\tactic_attack.bop");
	if (graphic)
		g->DrawImage(graphic, 460, 80, 40, 40);
	graphic	= pDoc->GetGraphicPool()->GetGDIGraphic("Other\\tactic_avoid.bop");
	if (graphic)
		g->DrawImage(graphic, 620, 80, 40, 40);
	graphic	= pDoc->GetGraphicPool()->GetGDIGraphic("Other\\tactic_retreat.bop");
	if (graphic)
		g->DrawImage(graphic, 780, 80, 40, 40);
			
	// �berschrift in der Mitte zeichnen
	// Rassenspezifische Schriftart ausw�hlen
	CFontLoader::CreateGDIFont(pMajor, 5, fontName, fontSize);
	// Schriftfarbe w�hlen
	CFontLoader::GetGDIFontColor(pMajor, 3, normalColor);
	fontBrush.SetColor(normalColor);

	if (pDoc->GetSector(p).GetKnown(pMajor->GetRaceID()))
		s.Format("%s %s", CResourceManager::GetString("COMBAT_IN_SECTOR"), pDoc->GetSector(p).GetName(TRUE));
	else
		s.Format("%s %c%i", CResourceManager::GetString("COMBAT_IN_SECTOR"), (char)(p.y+97), p.x + 1);
	g->DrawString(s.AllocSysString(), -1, &Gdiplus::Font(fontName.AllocSysString(), fontSize), RectF(0,10,m_TotalSize.cx, 50), &fontFormat, &fontBrush);	
}

void CCombatMenuView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CBotf2Doc* pDoc = (CBotf2Doc*)GetDocument();
	ASSERT(pDoc);

	if (!pDoc->m_bDataReceived)
		return;

	if (pDoc->m_bRoundEndPressed)
		return;

	CMajor* pMajor = m_pPlayersRace;
	ASSERT(pMajor);
	if (!pMajor)
		return;

	CalcLogicalPoint(point);
	
	if (!m_bInOrderMenu)
	{
		int iClick = COMBAT_NON;
		if (ButtonReactOnLeftClick(point, &m_CombatDecisionButtons, iClick))
		{
			if (iClick == COMBAT_USER)
			{
				// ins detailierte Schiffsbefehlsmen� springen
				m_bInOrderMenu = true;
				Invalidate();
				for (int i = 0; i < m_CombatDecisionButtons.GetSize(); i++)
					m_CombatDecisionButtons[i]->SetState(0);	
			}
			else
			{
				// Befehl festlegen
				pDoc->m_nCombatOrder = iClick;
				
				pDoc->m_bRoundEndPressed = true;
				client.EndOfRound(pDoc);
			}		
			return;
		}
	}
	else
	{
		int iClick = -1;
		if (ButtonReactOnLeftClick(point, &m_CombatTacticButtons, iClick))
		{		
			return;
		}

		if (ButtonReactOnLeftClick(point, &m_CombatOrderButtons, iClick))
		{	
			if (iClick == 0)
			{
				m_bInOrderMenu = false;
				Invalidate();
				for (int i = 0; i < m_CombatOrderButtons.GetSize(); i++)
					m_CombatOrderButtons[i]->SetState(0);
			}
			else
			{
				// Befehl festlegen
				pDoc->m_nCombatOrder = COMBAT_USER;
				pDoc->m_bRoundEndPressed = true;
				client.EndOfRound(pDoc);
			}
			
			return;
		}

		if (ButtonReactOnLeftClick(point, &m_FriendShipsCursor, iClick))
		{		
			if (iClick == 0)
				m_nPageFriends--;
			else
				m_nPageFriends++;
			Invalidate();
			return;
		}

		if (ButtonReactOnLeftClick(point, &m_EnemyShipsCursor, iClick))
		{		
			if (iClick == 0)
				m_nPageEnemies--;
			else
				m_nPageEnemies++;
			Invalidate();
			return;
		}

		if (ButtonReactOnLeftClick(point, &m_ShipTypeButton, iClick, true, true))
		{		
			m_nShipType++;
			if (m_nShipType > ALIEN - 1)
				m_nShipType = -1;
			CString s = CResourceManager::GetString("ALL_SHIPS");
			if (m_nShipType != -1)
			{
				CShip temp;
				temp.SetShipType(m_nShipType);
				s = temp.GetShipTypeAsString();
			}
			m_ShipTypeButton[0]->SetText(s);
			// Seiten zur�cksetzen
			m_nPageFriends = 0;
			m_nPageEnemies = 0;
			// kleine Buttons deaktivieren
			for (int i = 0; i < m_FriendShipsCursor.GetSize(); i++)
				m_FriendShipsCursor[i]->SetState(2);
			for (int i = 0; i < m_EnemyShipsCursor.GetSize(); i++)
				m_EnemyShipsCursor[i]->SetState(2);
			
			Invalidate();
			return;
		}

		// wurde auf ein Schiffe geklickt?
		for (UINT i = 0; i < m_vShipRects.size(); i++)
		{
			if (m_vShipRects[i].first.PtInRect(point))
			{
				CShip* pShip = m_vShipRects[i].second;
				ASSERT(pShip);
				// es k�nnen nur eigene Schiffe eingestellt werden
				if (pShip->GetOwnerOfShip() != pMajor->GetRaceID())
					return;
				// aktuell eingestellt Befehl an das Schiff �bergeben
				int nOrder = -1;
				for (int j = 0; j < m_CombatTacticButtons.GetSize(); j++)
					if (m_CombatTacticButtons[j]->GetState() == 2)
					{
						nOrder = j;
						break;
					}
				switch (nOrder)
				{
				case 0: pShip->SetCombatTactic(COMBAT_TACTIC_ATTACK);	break;
				case 1: pShip->SetCombatTactic(COMBAT_TACTIC_AVOID);	break;
				case 2: if (pShip->GetManeuverability() != 0)
							pShip->SetCombatTactic(COMBAT_TACTIC_RETREAT);
						break;
				default: ASSERT(FALSE);
				}
				
				CRect r = m_vShipRects[i].first;
				CalcDeviceRect(r);				
				InvalidateRect(r, FALSE);
				return;
			}
		}
	}

	CMainBaseView::OnLButtonDown(nFlags, point);
}

void CCombatMenuView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CBotf2Doc* pDoc = (CBotf2Doc*)GetDocument();
	ASSERT(pDoc);

	if (!pDoc->m_bDataReceived)
		return;

	if (!m_bInOrderMenu)
		return;

	CMajor* pMajor = m_pPlayersRace;
	ASSERT(pMajor);
	if (!pMajor)
		return;
	
	CalcLogicalPoint(point);
	
	for (int i = 0; i < m_CombatTacticButtons.GetSize(); i++)
	{
		if (m_CombatTacticButtons[i]->GetRect().PtInRect(point))
		{	
			// allen eigenen Schiffen den geklickten Befehl geben
			for (int j = 0; j < m_vInvolvedShips.GetSize(); j++)
			{
				CShip* pShip = m_vInvolvedShips[j];
				if (m_nShipType != -1 && pShip->GetShipType() != m_nShipType)
					continue;

				if (pShip->GetOwnerOfShip() != pMajor->GetRaceID() || pShip->GetKO() != pDoc->m_ptCurrentCombatSector)
					continue;
				
				switch (i)
				{
				case 0: pShip->SetCombatTactic(COMBAT_TACTIC_ATTACK);	break;
				case 1: pShip->SetCombatTactic(COMBAT_TACTIC_AVOID);	break;
				case 2: if (pShip->GetManeuverability() != 0)
							pShip->SetCombatTactic(COMBAT_TACTIC_RETREAT);
						break;
				default: ASSERT(FALSE);
				}
			}

			Invalidate();
			return;
		}
	}

	// wurde auf ein Schiffe geklickt?
	for (UINT i = 0; i < m_vShipRects.size(); i++)
	{
		if (m_vShipRects[i].first.PtInRect(point))
		{
			CShip* pShip = m_vShipRects[i].second;
			ASSERT(pShip);
			// es k�nnen nur eigene Schiffe eingestellt werden
			if (pShip->GetOwnerOfShip() != pMajor->GetRaceID())
				return;
			// aktuell eingestellten Befehl an alle Schiffe dieser Klasse �bergeben
			for (int j = 0; j < m_vInvolvedShips.GetSize(); j++)
			{
				CShip* pShip2 = m_vInvolvedShips[j];
				
				if (pShip2->GetOwnerOfShip() != pMajor->GetRaceID() || pShip2->GetKO() != pDoc->m_ptCurrentCombatSector || pShip2->GetShipClass() != pShip->GetShipClass())
					continue;		
				
				// aktuell eingestellt Befehl an das Schiff �bergeben
				int nOrder = -1;
				for (int j = 0; j < m_CombatTacticButtons.GetSize(); j++)
					if (m_CombatTacticButtons[j]->GetState() == 2)
					{
						nOrder = j;
						break;
					}
				switch (nOrder)
				{
				case 0: pShip2->SetCombatTactic(COMBAT_TACTIC_ATTACK);	break;
				case 1: pShip2->SetCombatTactic(COMBAT_TACTIC_AVOID);	break;
				case 2: if (pShip2->GetManeuverability() != 0)
							pShip2->SetCombatTactic(COMBAT_TACTIC_RETREAT);
						break;
				default: ASSERT(FALSE);
				}
			}

			Invalidate();
			return;
		}
	}

	CMainBaseView::OnLButtonDblClk(nFlags, point);
}

void CCombatMenuView::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (!m_bInOrderMenu)
		return;

	// TODO: Add your message handler code here and/or call default
	CBotf2Doc* pDoc = (CBotf2Doc*)GetDocument();
	ASSERT(pDoc);

	if (!pDoc->m_bDataReceived)
		return;

	if (pDoc->m_bRoundEndPressed)
		return;

	CMajor* pMajor = m_pPlayersRace;
	ASSERT(pMajor);
	if (!pMajor)
		return;

	CalcLogicalPoint(point);
	
	int iClick = -1;
	if (ButtonReactOnLeftClick(point, &m_ShipTypeButton, iClick, true, true))
	{		
		m_nShipType--;
		if (m_nShipType < -1)
			m_nShipType = ALIEN - 1;
		CString s = CResourceManager::GetString("ALL_SHIPS");
		if (m_nShipType != -1)
		{
			CShip temp;
			temp.SetShipType(m_nShipType);
			s = temp.GetShipTypeAsString();
		}
		m_ShipTypeButton[0]->SetText(s);
		// Seiten zur�cksetzen
		m_nPageFriends = 0;
		m_nPageEnemies = 0;
		// kleine Buttons deaktivieren
		for (int i = 0; i < m_FriendShipsCursor.GetSize(); i++)
			m_FriendShipsCursor[i]->SetState(2);
		for (int i = 0; i < m_EnemyShipsCursor.GetSize(); i++)
			m_EnemyShipsCursor[i]->SetState(2);
		
		Invalidate();
		return;
	}

	CMainBaseView::OnRButtonDown(nFlags, point);
}

void CCombatMenuView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default	
	CBotf2Doc* pDoc = (CBotf2Doc*)GetDocument();
	ASSERT(pDoc);

	if (!pDoc->m_bDataReceived)
		return;

	if (pDoc->m_bRoundEndPressed)
		return;

	CalcLogicalPoint(point);
	// Entscheidungsbuttons �berpr�fen
	if (!m_bInOrderMenu)
		ButtonReactOnMouseOver(point, &m_CombatDecisionButtons);
	else
	{
		// wurde die Maus �ber ein Schiff gehalten
		for (UINT i = 0; i < m_vShipRects.size(); i++)
		{
			if (m_vShipRects[i].first.PtInRect(point))
			{
				bool bNewMarkedShip = m_vShipRects[i].second != m_pMarkedShip;
				if (bNewMarkedShip)
				{
					InvalidateRect(m_rLastMarkedRect, FALSE);
					CRect r = m_vShipRects[i].first;
					CalcDeviceRect(r);
					m_rLastMarkedRect = r;
					m_pMarkedShip = m_vShipRects[i].second;
					InvalidateRect(r, FALSE);
				}				
				return;
			}
		}

		ButtonReactOnMouseOver(point, &m_CombatTacticButtons);
		ButtonReactOnMouseOver(point, &m_CombatOrderButtons);
		ButtonReactOnMouseOver(point, &m_FriendShipsCursor);
		ButtonReactOnMouseOver(point, &m_EnemyShipsCursor);
		ButtonReactOnMouseOver(point, &m_ShipTypeButton);
	}

	CMainBaseView::OnMouseMove(nFlags, point);
}

void CCombatMenuView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	CMainBaseView::OnKeyDown(nChar, nRepCnt, nFlags);
}

///	Funktion erstellt zur aktuellen Mouse-Position einen HTML Tooltip
/// @return	der erstellte Tooltip-Text
CString CCombatMenuView::CreateTooltip(void)
{
	if (!m_bInOrderMenu)
		return "";

	CBotf2Doc* pDoc = (CBotf2Doc*)GetDocument();
	ASSERT(pDoc);

	if (!pDoc->m_bDataReceived)
		return "";

	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	CalcLogicalPoint(pt);
	
	for (UINT i = 0; i < m_vShipRects.size(); i++)
		if (m_vShipRects[i].first.PtInRect(pt))
			return m_vShipRects[i].second->GetTooltip(false);			
	
	return "";
}

void CCombatMenuView::CreateButtons()
{
	CBotf2Doc* pDoc = (CBotf2Doc*)GetDocument();
	ASSERT(pDoc);

	CMajor* pMajor = m_pPlayersRace;
	ASSERT(pMajor);

	CString sPrefix = pMajor->GetPrefix();
	// alle Buttons in der View anlegen und Grafiken laden
	
	CString fileN = "Other\\" + sPrefix + "button.bop";
	CString fileI = "Other\\" + sPrefix + "buttoni.bop";
	CString fileA = "Other\\" + sPrefix + "buttona.bop";
	// Buttons in der Kampfentscheidungsansicht
	m_CombatDecisionButtons.Add(new CMyButton(CPoint(320,950), CSize(160,40), CResourceManager::GetString("BTN_DETAILS"),  fileN, fileI, fileA));
	m_CombatDecisionButtons.Add(new CMyButton(CPoint(480,950), CSize(160,40), CResourceManager::GetString("BTN_HAILING"),  fileN, fileI, fileA));
	m_CombatDecisionButtons.Add(new CMyButton(CPoint(640,950), CSize(160,40), CResourceManager::GetString("BTN_RETREAT"),  fileN, fileI, fileA));
	m_CombatDecisionButtons.Add(new CMyButton(CPoint(800,950), CSize(160,40), CResourceManager::GetString("BTN_AUTOCOMBAT"),  fileN, fileI, fileA));
	// Buttons in der Befehlsmen�ansicht
	m_CombatTacticButtons.Add(new CMyButton(CPoint(400,125), CSize(160,40), CResourceManager::GetString("BTN_ATTACK"),  fileN, fileI, fileA));
	m_CombatTacticButtons.Add(new CMyButton(CPoint(560,125), CSize(160,40), CResourceManager::GetString("BTN_AVOID"),  fileN, fileI, fileA));
	m_CombatTacticButtons.Add(new CMyButton(CPoint(720,125), CSize(160,40), CResourceManager::GetString("BTN_RETREAT"),  fileN, fileI, fileA));

	m_CombatOrderButtons.Add(new CMyButton(CPoint(480,950), CSize(160,40), CResourceManager::GetString("BTN_BACK"),  fileN, fileI, fileA));
	m_CombatOrderButtons.Add(new CMyButton(CPoint(640,950), CSize(160,40), CResourceManager::GetString("BTN_READY"),  fileN, fileI, fileA));

	// kleine Buttons in der Befehlsansicht
	m_FriendShipsCursor.Add(new CMyButton(CPoint(200,885), CSize(90,30), CResourceManager::GetString("BTN_BACK"),  fileN, fileI, fileA));
	m_FriendShipsCursor.Add(new CMyButton(CPoint(290,885), CSize(90,30), CResourceManager::GetString("BTN_FURTHER"),  fileN, fileI, fileA));
	m_EnemyShipsCursor.Add(new CMyButton(CPoint(900,885), CSize(90,30), CResourceManager::GetString("BTN_BACK"),  fileN, fileI, fileA));
	m_EnemyShipsCursor.Add(new CMyButton(CPoint(990,885), CSize(90,30), CResourceManager::GetString("BTN_FURTHER"),  fileN, fileI, fileA));

	// Button zum Umschalten der Schiffstypen
	m_ShipTypeButton.Add(new CMyButton(CPoint(580,285), CSize(120,30), CResourceManager::GetString("ALL_SHIPS"),  fileN, fileI, fileA));
}
	