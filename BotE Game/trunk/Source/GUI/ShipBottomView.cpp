// ShipBottomView.cpp : implementation file
//

#include "stdafx.h"
#include "IOData.h"
#include "GalaxyMenuView.h"
#include "FleetMenuView.h"
#include "TransportMenuView.h"
#include "ShipBottomView.h"
#include "PlanetBottomView.h"
#include "SmallInfoView.h"
#include "Races\RaceController.h"
#include "HTMLStringBuilder.h"
#include "Graphic\memdc.h"
#include "MainFrm.h"
#include "Ships/Ships.h"
#include "General/ResourceManager.h"

#include <cassert>

#ifdef _DEBUG
// #define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CShipBottomView
BOOLEAN CShipBottomView::m_bShowStation = FALSE;

IMPLEMENT_DYNCREATE(CShipBottomView, CBottomBaseView)

CShipBottomView::CShipBottomView() :
	m_iPage(1),
	m_bShowNextButton(FALSE),
	m_iTimeCounter(0),
	m_iWhichMainShipOrderButton(MAIN_BUTTON_NONE),
	m_LastKO(-1, -1),
	m_dc(),
	bw(120),
	bh(30),
	bd(5),
	bt(35),
	bb(bt + (MAIN_BUTTON_CANCEL -1) * (bh + bd) + bh)
{
	m_pShipOrderButton = NULL;
	m_vMainShipOrders.reserve(MAIN_BUTTON_CANCEL);
	m_vSecondaryShipOrders.resize(SHIP_ORDER::CANCEL + 1);
}

CShipBottomView::~CShipBottomView()
{
	if (m_pShipOrderButton)
	{
		delete m_pShipOrderButton;
		m_pShipOrderButton = NULL;
	}
}

BEGIN_MESSAGE_MAP(CShipBottomView, CBottomBaseView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

void CShipBottomView::OnNewRound()
{
	m_iPage = 1;
	m_vShipRects.clear();
	m_rLastMarkedRect = CRect(0,0,0,0);
	m_RectForTheShip = CRect(0,0,0,0);
}

// CShipBottomView drawing

static bool ShipCanHaveOrder(const CShips& ships, SHIP_ORDER::Typ order,
		const CSector* sector = NULL, const CSystem* system = NULL) {
	switch(order) {
		case SHIP_ORDER::TRAIN_SHIP:
			return sector->GetSunSystem() && system->GetOwnerOfSystem() == ships.GetOwnerOfShip()
				&& system->GetProduction()->GetShipTraining() > 0;
		case SHIP_ORDER::REPAIR:
			return ships.CanHaveOrder(SHIP_ORDER::REPAIR, true)
				&& sector->GetShipPort(ships.GetOwnerOfShip());
		case SHIP_ORDER::TRANSPORT:
			return ships.CanHaveOrder(order, true, false);
		case SHIP_ORDER::TERRAFORM:
			return ships.CanHaveOrder(order, false);
	}
	return ships.CanHaveOrder(order, true);
}

void CShipBottomView::SetupDrawing() {
	CRect client;
	GetClientRect(&client);

	m_dc.fontName = "";
	m_dc.fontSize = 0.0;

	/*g.SetSmoothingMode(SmoothingModeHighSpeed);
	g.SetInterpolationMode(InterpolationModeLowQuality);
	g.SetPixelOffsetMode(PixelOffsetModeHighSpeed);
	g.SetCompositingQuality(CompositingQualityHighSpeed);*/

	m_dc.g->SetSmoothingMode(SmoothingModeHighQuality);
	m_dc.g->SetInterpolationMode(InterpolationModeLowQuality);
	m_dc.g->SetPixelOffsetMode(PixelOffsetModeHighSpeed);
	m_dc.g->SetCompositingQuality(CompositingQualityHighSpeed);
	m_dc.g->ScaleTransform((REAL)client.Width() / (REAL)m_TotalSize.cx, (REAL)client.Height() / (REAL)m_TotalSize.cy);
	m_dc.g->Clear(Color::Black);

	// Rassenspezifische Schriftart ausw�hlen
	CFontLoader::CreateGDIFont(m_pPlayersRace, 2, m_dc.fontName, m_dc.fontSize);
	CFontLoader::GetGDIFontColor(m_pPlayersRace, 3, m_dc.normalColor);
	
	m_dc.fontBrush = new SolidBrush(m_dc.normalColor);

	m_dc.r.SetRect(0, 0, m_TotalSize.cx, m_TotalSize.cy);

	// Angezeigten Sektor, aktuelle Seite und dergleichen initialisieren
	if (m_LastKO != m_dc.pDoc->GetKO())
	{
		m_LastKO = m_dc.pDoc->GetKO();
		m_rLastMarkedRect = CRect(0,0,0,0);
		m_RectForTheShip = CRect(0,0,0,0);
		m_iPage = 1;
	}
	else if (m_bShowStation)
	{
		// Ebenfalls bei der Anzeige einer Station immer auf die erste Seite springen
		m_iPage = 1;
	}
	SetUpMainButtons();
}

// Achtung: r.right = Breite, r.bottom = H�he!
void CShipBottomView::DrawImage( CString resName, CRect r ) {
	Bitmap* bmp = m_dc.gp->GetGDIGraphic(resName);
	if (bmp)
		m_dc.g->DrawImage(bmp, r.left, r.top, r.Width(), r.Height());
}

void CShipBottomView::DrawSmallButton( const CString& resString, const CPoint& coords, SHIP_ORDER::Typ shiporder ) {
	m_dc.fontBrush->SetColor(this->GetFontColorForSmallButton());
	m_dc.fontFormat.SetAlignment(StringAlignmentCenter);
	m_dc.fontFormat.SetLineAlignment(StringAlignmentCenter);
	m_dc.fontFormat.SetFormatFlags(StringFormatFlagsNoWrap);

	m_dc.g->DrawImage(m_pShipOrderButton, coords.x, coords.y, bw, bh);
	m_dc.g->DrawString(CComBSTR(CResourceManager::GetString(resString)), -1, &Gdiplus::Font(CComBSTR(m_dc.fontName), m_dc.fontSize), RectF(coords.x,coords.y,bw,bh), &m_dc.fontFormat, m_dc.fontBrush);

	if( shiporder != SHIP_ORDER::NONE ) {
		m_vSecondaryShipOrders.at(shiporder).rect.SetRect(coords.x,coords.y,coords.x + bw,coords.y + bh);
	}
}

bool CShipBottomView::CheckDisplayShip(CShips *pShip, CSector *csec ) {
	if (m_LastKO != pShip->GetKO())
		return false;

	const BOOL is_base = pShip->IsStation();
	// Wenn eine Station angezeigt werden soll, dann muss der Typ von einer Station sein
	// Wenn keine Station angezeigt werden soll, dann darf der Typ nicht von einer Station sein
	if (m_bShowStation != is_base)
		return false;

	// Schiffe mit zu guter Stealthpower werden hier nicht angezeigt.
	const USHORT stealthPower = pShip->GetStealthPower();

	CString rid = m_pPlayersRace->GetRaceID();
	if (   pShip->GetOwnerOfShip() != rid				// Schiff geh�rt anderer Rasse als der aktuellen
		&& csec->GetScanPower(rid) < stealthPower		// Scanpower im Sektor ist kleiner stealthpower des Schiffs
		&& m_pPlayersRace->GetAgreement(pShip->GetOwnerOfShip()) < DIPLOMATIC_AGREEMENT::AFFILIATION) // Diplomatie ist kleiner "Affiliation"
			return false;
	return true;
}

void CShipBottomView::DrawShipContent() {
	CRect r(0, 0, m_TotalSize.cx, m_TotalSize.cy);
	USHORT column = 0;
	USHORT row = 0;
	CMajor* pMajor = m_pPlayersRace;
	CShipMap::iterator oneShip = m_dc.pDoc->m_ShipMap.begin();
	Gdiplus::Color markColor;
	Gdiplus::Font font(CComBSTR(m_dc.fontName), m_dc.fontSize);
	USHORT counter = 0;
	CShips* pShip;

	if (!CGalaxyMenuView::IsMoveShip())
		m_vShipRects.clear();

	markColor.SetFromCOLORREF(pMajor->GetDesign()->m_clrListMarkTextColor);
	CSector csec = m_dc.pDoc->CurrentSector();

	for(CShipMap::iterator i = m_dc.pDoc->m_ShipMap.begin(); i != m_dc.pDoc->m_ShipMap.end(); ++i)
	{
		pShip = &i->second;
		if( !CheckDisplayShip( pShip, &csec ) )
			continue;

		// mehrere Spalten anlegen, falls mehr als 3 Schiffe in dem System sind
		if (counter != 0 && counter%3 == 0)
		{
			column++;
			row = 0;
		}
		// Wenn wir eine Seite vollhaben
		if (counter%9 == 0)
			column = 0;

		if (counter < m_iPage*9 && counter >= (m_iPage-1)*9)
		{
			CPoint pt(250 * column, 65 * row);
			m_vShipRects.push_back(pair<CRect, CShips*>(CRect(pt.x, pt.y + 20, pt.x + 250, pt.y + 85), pShip));
		}

		// gro�es Bild der Station zeichnen
		if (m_bShowStation)
		{
			map<CString, CMajor*>* pmMajors = m_dc.pDoc->GetRaceCtrl()->GetMajors();
			for (map<CString, CMajor*>::const_iterator it = pmMajors->begin(); it != pmMajors->end(); ++it)
			{
				if (m_dc.pDoc->CurrentSector().GetOutpost(it->first) || m_dc.pDoc->CurrentSector().GetStarbase(it->first))
				{
					// geh�rt uns die Station oder kennen wir die andere Rasse
					if (pMajor->GetRaceID() == pShip->GetOwnerOfShip() || pMajor->IsRaceContacted(pShip->GetOwnerOfShip()))
					{
						DrawImage("Other\\" + it->second->GetPrefix() + "Starbase.bop", CRect(CPoint(550,20),CSize(235,200)));
						break;
					}
				}
			}
			break;
		}

		row++;
		counter++;
		oneShip = i;

		if (counter > m_iPage*9)
			break;
	}

	// Wenn nur ein Schiff in dem System ist, so wird es automatisch ausgew�hlt
	if (counter == 1 && !m_bShowStation && oneShip->second.GetCurrentOrder() <= SHIP_ORDER::ATTACK
		&& oneShip->second.GetOwnerOfShip() == pMajor->GetRaceID())
	{
		// Wenn wenn wir auf der Galaxiekarte sind
		if (resources::pMainFrame->GetActiveView(0, 1) == GALAXY_VIEW)
		{
			this->SetTimer(1,100,NULL);
			m_dc.pDoc->SetCurrentShip(oneShip);
			CGalaxyMenuView::SetMoveShip(TRUE);
			CSmallInfoView::SetDisplayMode(CSmallInfoView::DISPLAY_MODE_SHIP_BOTTEM_VIEW);
			resources::pMainFrame->InvalidateView(RUNTIME_CLASS(CSmallInfoView));
			m_iWhichMainShipOrderButton = MAIN_BUTTON_NONE;
			resources::pMainFrame->InvalidateView(RUNTIME_CLASS(CGalaxyMenuView));
		}
	}

	// Schiffe jetzt auch zeichnen
	for(std::vector<std::pair<CRect, CShips*>>::const_iterator itdraw = m_vShipRects.begin(); itdraw != m_vShipRects.end(); ++itdraw) {
		pShip = itdraw->second;
		
		// Kennen wir den Besizter des Schiffes?
		bool bUnknown = (pMajor->GetRaceID() != pShip->GetOwnerOfShip() && pMajor->IsRaceContacted(pShip->GetOwnerOfShip()) == false);
		if (bUnknown)
		{
			// Wenn kein diplomatischer Kontakt m�glich ist, wird das Schiff immer angezeigt
			CRace* pShipOwner = m_dc.pDoc->GetRaceCtrl()->GetRace(pShip->GetOwnerOfShip());
			if (pShipOwner)
				bUnknown = !pShipOwner->HasSpecialAbility(SPECIAL_NO_DIPLOMACY);
		}
		CRect loc = itdraw->first;
		// ist das Schiff gerade markiert?
		bool bMarked = (pShip == &m_dc.pDoc->CurrentShip()->second);
		pShip->DrawShip(m_dc.g, m_dc.gp, CPoint(loc.left,loc.top-20), bMarked, bUnknown, TRUE, m_dc.normalColor, markColor, font);
	}

	// Die Buttons f�r vor und zur�ck darstellen, wenn wir mehr als 9 Schiffe in dem Sektor sehen
	m_bShowNextButton = FALSE;
	if (counter > 9 && counter > m_iPage*9)
	{
		m_bShowNextButton = TRUE;
		DrawSmallButton(CResourceManager::GetString("BTN_NEXT"), CPoint(r.right-bw, r.top+210));
	}
	// back-Button
	if (m_iPage > 1)
	{
		DrawSmallButton(CResourceManager::GetString("BTN_BACK"), CPoint(r.right-bw, r.top));
	}
}

CPoint CShipBottomView::CalcSecondaryButtonTopLeft(short counter, bool top_down) const {
	const unsigned y = top_down ? bt + counter * (bh + bd)
		: bb - counter * (bh + bd) - bh;
	return CPoint(m_dc.r.right - (bw *2 + bd), y);
}

void CShipBottomView::DrawColonyshipOrders(short &counter) {
	CSector csec = m_dc.pDoc->CurrentSector();
	CShips pShip = m_dc.pDoc->CurrentShip()->second;
	CRect r(m_dc.r);

	// Kolonisierung (hier beachten wenn es eine Flotte ist, dort schauen ob auch jedes Schiff in
	// der Flotte auch kolonisieren kann)
	if (m_iTimeCounter > MAIN_BUTTON_CANCEL && ShipCanHaveOrder(pShip, SHIP_ORDER::COLONIZE))
	{
		// Wenn das System uns bzw. niemanden geh�rt k�nnen wir nur kolonisieren
		if (csec.GetOwnerOfSector() == "" || csec.GetOwnerOfSector() == pShip.GetOwnerOfShip())
			for (int l = 0; l < csec.GetNumberOfPlanets(); l++)
				if (csec.GetPlanet(l)->GetTerraformed() == TRUE
					&& csec.GetPlanet(l)->GetCurrentHabitant() == 0.0f)
				{
					DrawSmallButton("BTN_COLONIZE", CalcSecondaryButtonTopLeft(counter, false), SHIP_ORDER::COLONIZE);
					counter++;
					break;
				}
	}
	// Terraforming (hier beachten wenn es eine Flotte ist, dort schauen ob auch jedes Schiff in
	// der Flotte auch terraformen kann)
	if (m_iTimeCounter > (MAIN_BUTTON_CANCEL + counter) && ShipCanHaveOrder(pShip, SHIP_ORDER::TERRAFORM))

	{
		for (int l = 0; l < csec.GetNumberOfPlanets(); l++)
			if (csec.GetPlanet(l)->GetHabitable() == TRUE &&
				csec.GetPlanet(l)->GetTerraformed() == FALSE)
			{
				DrawSmallButton("BTN_TERRAFORM",CalcSecondaryButtonTopLeft(counter, false), SHIP_ORDER::TERRAFORM);
				counter++;
				break;
			}
	}
}

void CShipBottomView::DrawTransportshipOrders(short &counter) {
	const CSector& csec = m_dc.pDoc->CurrentSector();
	CShips pShip = m_dc.pDoc->CurrentShip()->second;
	CMajor* pMajor = m_pPlayersRace;
	CRect r(m_dc.r);

	// Au�enposten/Sternbasis bauen (hier beachten wenn es eine Flotte ist, dort schauen ob auch jedes
	// Schiff in der Flotte Stationen bauen kann)
	if (m_iTimeCounter > (MAIN_BUTTON_CANCEL + counter) && 
		// Ab hier check wegen Flotten, darum wirds lang (m�ssen nur einen der Befehle (egal ob Outpost oder
		// Starbase gebaut werden soll) �bergeben, weil wenn das eine geht, geht auch das andere
		ShipCanHaveOrder(pShip, SHIP_ORDER::BUILD_OUTPOST))
	{
		CPoint ShipKO = m_dc.pDoc->GetKO();
		// hier schauen, ob ich in der Schiffsinfoliste schon einen Au�enposten habe den ich bauen kann, wenn in dem
		// Sector noch kein Au�enposten steht und ob ich diesen in dem Sector �berhaupt bauen kann. Das geht nur
		// wenn der Sektor mir oder niemanden geh�rt
		if(csec.IsStationBuildable(SHIP_TYPE::OUTPOST, pShip.GetOwnerOfShip()))
		{
			// Hier �berpr�fen, ob ich einen Au�enposten technologisch �berhaupt bauen kann
			for (int l = 0; l < m_dc.pDoc->m_ShipInfoArray.GetSize(); l++)
				if(pMajor->CanBuildShip(SHIP_TYPE::OUTPOST, m_dc.researchLevels, m_dc.pDoc->m_ShipInfoArray.GetAt(l)))
				{
					// Wenn ja dann Schaltfl�che zum Au�enpostenbau einblenden
					DrawSmallButton("BTN_BUILD_OUTPOST",CalcSecondaryButtonTopLeft(counter, false),SHIP_ORDER::BUILD_OUTPOST);
					counter++;
					break;
				}
		}
		// Wenn hier schon ein Au�enposten steht, k�nnen wir vielleicht auch eine Sternbasis bauen
		else if (csec.IsStationBuildable(SHIP_TYPE::STARBASE, pShip.GetOwnerOfShip()))
		{
			// Hier �berpr�fen, ob ich eine Sternbasis technologisch �berhaupt bauen kann
			for (int l = 0; l < m_dc.pDoc->m_ShipInfoArray.GetSize(); l++)
				if(pMajor->CanBuildShip(SHIP_TYPE::STARBASE, m_dc.researchLevels, m_dc.pDoc->m_ShipInfoArray.GetAt(l)))
				{
					// Wenn ja dann Schaltfl�che zum Sternenbasisbau einblenden
					DrawSmallButton("BTN_BUILD_STARBASE",CalcSecondaryButtonTopLeft(counter, false),SHIP_ORDER::BUILD_STARBASE);
					counter++;
					break;
				}
		}
	}
	// Transport
	if (m_iTimeCounter > (MAIN_BUTTON_CANCEL + counter) && 
		ShipCanHaveOrder(pShip, SHIP_ORDER::TRANSPORT))
	{
		DrawSmallButton("BTN_TRANSPORT", CalcSecondaryButtonTopLeft(counter, false),SHIP_ORDER::TRANSPORT);
		counter++;
	}
}

void CShipBottomView::SetUpMainButtons() {
	m_vMainShipOrders.clear();
	const CRect& r = m_dc.r;
	unsigned top = r.top + bt;
	unsigned bottom = bt + bh;
	const unsigned left = r.right-bw;
	const unsigned shift = bh + bd;
	for(int i = 1; i <= MAIN_BUTTON_CANCEL; ++i) {
		m_vMainShipOrders.push_back(MainButtonInfo(static_cast<MAIN_BUTTON>(i), CRect(left, top, r.right, bottom)));
		top += shift;
		bottom += shift;
	}
	assert(m_vMainShipOrders.size() == MAIN_BUTTON_CANCEL);
}

void CShipBottomView::DrawMaincommandMenu() {
	// Taktik
	// Befehl
	// Aktion
	for(std::vector<const MainButtonInfo>::const_iterator i = m_vMainShipOrders.begin();
			i != m_vMainShipOrders.end(); ++i)
	{
		if(m_iTimeCounter < i->which)
			continue;
		DrawSmallButton(i->String(), CPoint(i->rect.left, i->rect.top),
			i->which == MAIN_BUTTON_CANCEL ? SHIP_ORDER::CANCEL : SHIP_ORDER::NONE);
	}
	if (m_iTimeCounter >= MAIN_BUTTON_CANCEL && m_iWhichMainShipOrderButton == MAIN_BUTTON_NONE)
	{
		this->KillTimer(1);
		m_iTimeCounter = 0;
	}
}

short CShipBottomView::DrawCombatMenu() {
	const CShips& pShip = m_dc.pDoc->CurrentShip()->second;
	short counter = 0;
	// angreifen
	if (m_iTimeCounter > MAIN_BUTTON_CANCEL && ShipCanHaveOrder(pShip, SHIP_ORDER::ATTACK))
	{
		DrawSmallButton("BTN_ATTACK", CalcSecondaryButtonTopLeft(counter), SHIP_ORDER::ATTACK);
		counter++;
	}
	// meiden
	else if (m_iTimeCounter > MAIN_BUTTON_CANCEL && ShipCanHaveOrder(pShip, SHIP_ORDER::AVOID))
	{
		DrawSmallButton("BTN_AVOID",CalcSecondaryButtonTopLeft(counter), SHIP_ORDER::AVOID);
		counter++;
	}
	return counter;
}

short CShipBottomView::DrawTacticsMenu() {
	CShips pShip = m_dc.pDoc->CurrentShip()->second;
	CRect r(m_dc.r);
	short counter = 0;
	CSector csec = m_dc.pDoc->CurrentSector();

	// folgende Befehle gehen alle nur, wenn es keine Station ist
	if (!pShip.IsStation())
	{
		// gruppieren
		if (m_iTimeCounter > (MAIN_BUTTON_CANCEL + counter))
		{
			DrawSmallButton("BTN_CREATE_FLEET",CalcSecondaryButtonTopLeft(counter),SHIP_ORDER::CREATE_FLEET);
			counter++;
		}
		// trainieren
		if (m_iTimeCounter > (MAIN_BUTTON_CANCEL + counter) && ShipCanHaveOrder(pShip, SHIP_ORDER::TRAIN_SHIP, &csec, &m_dc.pDoc->CurrentSystem()))
		{
			DrawSmallButton("BTN_TRAIN_SHIP",CalcSecondaryButtonTopLeft(counter),SHIP_ORDER::TRAIN_SHIP);
			counter++;
		}
		// tarnen (hier beachten wenn es eine Flotte ist, dort schauen ob sich jedes Schiff in der Flotte auch
		// tarnen kann)
		if (m_iTimeCounter > (MAIN_BUTTON_CANCEL + counter) && ShipCanHaveOrder(pShip, SHIP_ORDER::ENCLOAK))
		{
			DrawSmallButton("BTN_CLOAK",CalcSecondaryButtonTopLeft(counter),SHIP_ORDER::ENCLOAK);
			counter++;
		}
		if (m_iTimeCounter > (MAIN_BUTTON_CANCEL + counter) && ShipCanHaveOrder(pShip, SHIP_ORDER::DECLOAK))
		{
			DrawSmallButton("BTN_DECLOAK",CalcSecondaryButtonTopLeft(counter),SHIP_ORDER::DECLOAK);
			counter++;
		}
	}
	return counter;
}

short CShipBottomView::DrawOrdersMenu() {
	CShips pShip = m_dc.pDoc->CurrentShip()->second;
	CRect r(m_dc.r);
	CMajor* pMajor = m_pPlayersRace;
	CSector csec = m_dc.pDoc->CurrentSector();
	short counter = 0;

	// Systemangriff
	if (m_iTimeCounter > MAIN_BUTTON_CANCEL)
	{
		// Wenn im Sektor ein Sonnensystem ist
		if (csec.GetSunSystem() == TRUE &&
			// Wenn im System noch Bev�lkerung vorhanden ist
			csec.GetCurrentHabitants() > 0.0f &&
			// Wenn das System nicht der Rasse geh�rt, der auch das Schiff geh�rt
			m_dc.pDoc->CurrentSystem().GetOwnerOfSystem() != pShip.GetOwnerOfShip())
		{
			CRace* pOwnerOfSector = m_dc.pDoc->GetRaceCtrl()->GetRace(csec.GetOwnerOfSector());

			// Wenn im System eine Rasse lebt und wir mit ihr im Krieg sind
			if (pOwnerOfSector != NULL && pMajor->GetAgreement(pOwnerOfSector->GetRaceID()) == DIPLOMATIC_AGREEMENT::WAR
			// Wenn das System niemanden mehr geh�rt, aber noch Bev�lkerung drauf lebt (z.B. durch Rebellion)
				|| m_dc.pDoc->CurrentSystem().GetOwnerOfSystem() == "" && csec.GetMinorRace() == FALSE)
			{
				// nur wenn die Schiffe ungetarnt sind k�nnen sie Bombardieren
				// Ab hier check wegen Flotten
				if (ShipCanHaveOrder(pShip, SHIP_ORDER::ATTACK_SYSTEM))
				{
					DrawSmallButton("BTN_ATTACK_SYSTEM",CalcSecondaryButtonTopLeft(counter),SHIP_ORDER::ATTACK_SYSTEM);
					counter++;
				}
			}
		}
	}

	// Systemblockade
	if (m_iTimeCounter > (MAIN_BUTTON_CANCEL + counter) && ShipCanHaveOrder(pShip, SHIP_ORDER::BLOCKADE_SYSTEM))
	{
		// �berpr�fen ob man eine Blockade im System �berhaupt errichten kann
		// Wenn das System nicht der Rasse geh�rt, der auch das Schiff geh�rt
		CRace* pOwnerOfSystem = m_dc.pDoc->GetRaceCtrl()->GetRace(m_dc.pDoc->CurrentSystem().GetOwnerOfSystem());
		if (pOwnerOfSystem != NULL && pOwnerOfSystem->GetRaceID() != pShip.GetOwnerOfShip()
			&& pMajor->GetAgreement(pOwnerOfSystem->GetRaceID()) < DIPLOMATIC_AGREEMENT::FRIENDSHIP)
		{
			DrawSmallButton("BTN_BLOCKADE_SYSTEM",CalcSecondaryButtonTopLeft(counter),SHIP_ORDER::BLOCKADE_SYSTEM);
			counter++;
		}
	}
	// Flagschiffernennung, geht nur wenn es keine Flotte ist
	if (m_iTimeCounter > (MAIN_BUTTON_CANCEL + counter) && m_iWhichMainShipOrderButton == MAIN_BUTTON_ORDERS
		&& ShipCanHaveOrder(m_dc.pDoc->CurrentShip()->second, SHIP_ORDER::ASSIGN_FLAGSHIP))
	{
		DrawSmallButton("BTN_ASSIGN_FLAGSHIP",CalcSecondaryButtonTopLeft(counter),SHIP_ORDER::ASSIGN_FLAGSHIP);
		counter++;
	}
	// Warten
	if (m_iTimeCounter > (MAIN_BUTTON_CANCEL + counter) && m_iWhichMainShipOrderButton == MAIN_BUTTON_ORDERS
		&& ShipCanHaveOrder(m_dc.pDoc->CurrentShip()->second, SHIP_ORDER::WAIT_SHIP_ORDER))
	{
		DrawSmallButton("BTN_WAIT_SHIP_ORDER",CalcSecondaryButtonTopLeft(counter),SHIP_ORDER::WAIT_SHIP_ORDER);
		counter++;
	}
	// Wache
	if (m_iTimeCounter > (MAIN_BUTTON_CANCEL + counter) && m_iWhichMainShipOrderButton == MAIN_BUTTON_ORDERS
		&& ShipCanHaveOrder(m_dc.pDoc->CurrentShip()->second, SHIP_ORDER::SENTRY_SHIP_ORDER))
	{
		DrawSmallButton("BTN_SENTRY_SHIP_ORDER",CalcSecondaryButtonTopLeft(counter),SHIP_ORDER::SENTRY_SHIP_ORDER);
		counter++;
	}
	// Repairing
	// Only possible if
	// 1) the ship or any of the ships in its fleet are actually damaged.
	// 2) we (or an allied race) have a ship port in this sector.
	if (m_iTimeCounter > (MAIN_BUTTON_CANCEL + counter) && 
		ShipCanHaveOrder(pShip, SHIP_ORDER::REPAIR,
			&m_dc.pDoc->GetSector(
				pShip.GetKO().x,
				pShip.GetKO().y
			)
		)
	)
	{
		DrawSmallButton("BTN_REPAIR_SHIP",CalcSecondaryButtonTopLeft(counter),SHIP_ORDER::REPAIR);
		counter++;
	}
	return counter;
}

short CShipBottomView::DrawActionsMenu(bool isStation) {

	short counter = 0;
	if( !isStation ) {
		DrawColonyshipOrders(counter);
		DrawTransportshipOrders(counter);
	}
	// Schiff abwracken/zerst�ren
	if (m_iTimeCounter > (MAIN_BUTTON_CANCEL + counter) && ShipCanHaveOrder(m_dc.pDoc->CurrentShip()->second, SHIP_ORDER::DESTROY_SHIP))
	{
		DrawSmallButton("BTN_DESTROY_SHIP",CalcSecondaryButtonTopLeft(counter, false),SHIP_ORDER::DESTROY_SHIP);
		counter++;
	}

	return counter;
}

void CShipBottomView::DrawStationData() {
	CMajor* pMajor = m_pPlayersRace;
	CSector csec = m_dc.pDoc->CurrentSector();
	BYTE count = 0;

	// Wenn wir in dem Sektor gerade einen Au�enposten bauen, dann prozentualen Fortschritt anzeigen.
	// Es kann auch passieren, das mehrere Rassen gleichzeitig dort einen Au�enposten bauen, dann m�ssen wir
	// von jeder der Rasse den Fortschritt beim Stationsbau angeben
	map<CString, CMajor*>* pmMajors = m_dc.pDoc->GetRaceCtrl()->GetMajors();
	for (map<CString, CMajor*>::const_iterator it = pmMajors->begin(); it != pmMajors->end(); ++it)
		if (csec.GetIsStationBuilding(it->first) == TRUE)
		{
			CString station;
			if (csec.GetOutpost(it->first) == FALSE)
				station = CResourceManager::GetString("OUTPOST");
			else
				station = CResourceManager::GetString("STARBASE");
			m_dc.fontBrush->SetColor(Color(170,170,170));
			CString percent;
			percent.Format("%d",((csec.GetStartStationPoints(it->first)
				- csec.GetNeededStationPoints(it->first)) * 100
				/ csec.GetStartStationPoints(it->first)));

			CString sRaceName;
			if (pMajor == it->second || pMajor->IsRaceContacted(it->first))
				sRaceName = it->second->GetRaceName();
			else
				sRaceName = CResourceManager::GetString("UNKNOWN");

			CString s = station + CResourceManager::GetString("STATION_BUILDING", FALSE, sRaceName, percent);
			m_dc.fontFormat.SetAlignment(StringAlignmentCenter);
			m_dc.fontFormat.SetLineAlignment(StringAlignmentCenter);
			m_dc.g->DrawString(CComBSTR(s), -1, &Gdiplus::Font(CComBSTR(m_dc.fontName), m_dc.fontSize), PointF(250, 30+count*25), &m_dc.fontFormat, m_dc.fontBrush);
			count++;
		}
}

void CShipBottomView::DrawMenu() {
	CMajor* pMajor = m_pPlayersRace;

	CResearch *res = pMajor->GetEmpire()->GetResearch();
	m_dc.researchLevels[0] = res->GetBioTech();
	m_dc.researchLevels[1] = res->GetEnergyTech();
	m_dc.researchLevels[2] = res->GetCompTech();
	m_dc.researchLevels[3] = res->GetPropulsionTech();
	m_dc.researchLevels[4] = res->GetConstructionTech();
	m_dc.researchLevels[5] = res->GetWeaponTech();
	
	bool isStation = (m_dc.pDoc->CurrentShip()->second.IsStation());
	CRect rect;

	// Alle Rechtecke f�r die Buttons der Schiffsbefehle erstmal auf NULL setzen, damit wir nicht draufklicken
	// k�nnen. Wir d�rfen ja nur auf Buttons klicken k�nnen, die wir auch sehen
	for (unsigned j = 0; j < m_vSecondaryShipOrders.size(); j++)
		m_vSecondaryShipOrders.at(j).rect.SetRect(0,0,0,0);

	DrawMaincommandMenu();

	short counter = 0;
	if( m_iWhichMainShipOrderButton == MAIN_BUTTON_TACTICS )
		counter = DrawTacticsMenu();
	else if( m_iWhichMainShipOrderButton == MAIN_BUTTON_ORDERS && !isStation )
		counter = DrawOrdersMenu();
	else if( m_iWhichMainShipOrderButton == MAIN_BUTTON_ACTIONS )
		counter = DrawActionsMenu(isStation);
	else if( m_iWhichMainShipOrderButton == MAIN_BUTTON_COMBAT_BEHAVIOR )
		counter = DrawCombatMenu();

	if (m_iTimeCounter > (MAIN_BUTTON_CANCEL + counter))
	{
		this->KillTimer(1);
		m_iTimeCounter = 0;
	}
}

void CShipBottomView::OnDraw(CDC* dc)
{
	m_dc.pDoc = resources::pDoc;
	ASSERT(m_dc.pDoc);

	if (!m_dc.pDoc->m_bDataReceived)
		return;

	// Update Graphicpool Link

	m_dc.gp = m_dc.pDoc->GetGraphicPool();

	CMajor* pMajor = m_pPlayersRace;
	ASSERT(pMajor);
	if (!pMajor)
		return;
	// TODO: add draw code here

	// Doublebuffering wird initialisiert
	CMyMemDC pDC(dc);
	// Graphicsobjekt, in welches gezeichnet wird anlegen
	m_dc.g = new Graphics(pDC.GetSafeHdc());

	SetupDrawing();

	// Galaxie im Hintergrund zeichnen
	DrawImage( "Backgrounds\\" + pMajor->GetPrefix() + "galaxyV3.bop", CRect(CPoint(0,0),CSize(1075,249)) );
	CSector csec = m_dc.pDoc->CurrentSector();

	CString s;
	// Bis jetzt nur eine Anzeige bis max. 9 Schiffe
	if (m_iTimeCounter == 0)
	{
		DrawShipContent();
	}

	// Die ganzen Befehlsbuttons f�r die Schiffe anzeigen
	if (CGalaxyMenuView::IsMoveShip() == TRUE &&
		m_dc.pDoc->CurrentShip()->second.GetOwnerOfShip() == pMajor->GetRaceID())
	{
		DrawMenu();
	}

	if (m_bShowStation)	// Stationsansicht
	{
		DrawStationData();
	}

	// Wenn wir dem Schiff einen neuen Befehl geben, ohne das die Buttons �ber den Timer vollst�ndig gezeichnet wurden
	// verschwindet das Schiff in der Anzeige, da der TimeCounter nicht wieder auf NULL gesetzt wird. Dies machen
	// wir hier manuell, falls dieser Fall eintritt
	if (m_iTimeCounter > 10)
	{
		KillTimer(1);
		m_iTimeCounter =0;
	}

	m_dc.g->ReleaseHDC(pDC->GetSafeHdc());

	delete m_dc.fontBrush;
	delete m_dc.g;

	m_dc.pDoc = NULL;
}

// CShipBottomView diagnostics

#ifdef _DEBUG
void CShipBottomView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CShipBottomView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CShipBottomView message handlers

void CShipBottomView::OnInitialUpdate()
{
	CBottomBaseView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	CBotf2Doc* pDoc = resources::pDoc;
	ASSERT(pDoc);

	m_LastKO = pDoc->GetKO();
	m_iPage = 1;
	m_iTimeCounter = 0;
	m_bShowNextButton = FALSE;
	m_iWhichMainShipOrderButton = MAIN_BUTTON_NONE;
}

/// Funktion l�dt die rassenspezifischen Grafiken.
void CShipBottomView::LoadRaceGraphics()
{
	ASSERT((CBotf2Doc*)GetDocument());

	CMajor* pPlayer = m_pPlayersRace;
	ASSERT(pPlayer);

	CString sPrefix = pPlayer->GetPrefix();
	CString s = CIOData::GetInstance()->GetAppPath() + "Graphics\\Other\\" + sPrefix + "button_shiporder.bop";
	m_pShipOrderButton = Bitmap::FromFile(CComBSTR(s));
}

BOOL CShipBottomView::OnEraseBkgnd(CDC* /*pDC*/)
{
	// TODO: Add your message handler code here and/or call default
	return FALSE;
}


void CShipBottomView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CBotf2Doc* pDoc = resources::pDoc;
	assert(pDoc);

	if (!pDoc->m_bDataReceived)
		return;

	const CMajor* const pMajor = m_pPlayersRace;
	assert(pMajor);

	CalcLogicalPoint(point);

	if (m_RectForTheShip.PtInRect(point) && pDoc->CurrentShip()->second.GetOwnerOfShip() == pMajor->GetRaceID())
	{
		CSmallInfoView::SetDisplayMode(CSmallInfoView::DISPLAY_MODE_SHIP_BOTTEM_VIEW);
		resources::pMainFrame->InvalidateView(RUNTIME_CLASS(CSmallInfoView));
		// Wenn wir in der MainView nicht im Flottenmen� sind
		if (resources::pMainFrame->GetActiveView(0, 1) != FLEET_VIEW)
		{
			CGalaxyMenuView::SetMoveShip(TRUE);
			this->SetTimer(1,100,NULL);
			Invalidate(FALSE);
			resources::pMainFrame->InvalidateView(RUNTIME_CLASS(CGalaxyMenuView));
		}
		// Wenn wir in der MainView im Flottenmen� sind, dann stecken wir das angeklickte Schiff in die
		// gerade angezeigte Flotte
		// Fremde Flotten k�nnen nicht bearbeitet werden
		else
		{
			const CShipMap::iterator& fleetship = pDoc->FleetShip();
			assert(fleetship->second.GetOwnerOfShip() == pMajor->GetRaceID());
			const CShipMap::iterator& current_ship = pDoc->CurrentShip();
			// Jetzt f�gen wir der Flotte das angeklickte Schiff hinzu, wenn es nicht das Schiff selbst ist,
			// welches die Flotte anf�hrt
			if (fleetship != current_ship)
			{
				assert(fleetship->second.GetKO() == current_ship->second.GetKO());
				// sicherheitshalber wird hier nochmal �berpr�ft, dass keine Station hinzugef�gt werden kann und
				// das sich das Schiff auch im gleichen Sektor befindet
				if (fleetship->second.GetOwnerOfShip() == current_ship->second.GetOwnerOfShip()
					&& !current_ship->second.IsStation())
				{
					// Wenn das Schiff welches wir hinzuf�gen wollen selbst eine Flotte besizt, so m�ssen
					// wir diese Flotte nat�rlich auch noch hinzuf�gen
					//this is done in CShips::AddShipToFleet
					fleetship->second.AddShipToFleet(current_ship->second);
					// Wenn wir das Schiff da hinzugef�gt haben, dann m�ssen wir das aus der normalen Schiffsliste
					// rausnehmen, damit es nicht zweimal im Spiel vorkommt
					CShipMap::iterator to_erase = current_ship;
					//set the current ship to the next ship following the removed one of the same race in the
					//same sector which must not be a station, or to the fleetship in case there's none
					CShipMap::iterator next_current_ship = fleetship;
					CShipMap::iterator it = current_ship;
					++it;
					while(it != pDoc->m_ShipMap.end()) {
						if(fleetship->second.GetKO() != it->second.GetKO()
								|| fleetship->second.GetOwnerOfShip() != it->second.GetOwnerOfShip()
								|| it->second.IsStation()) {
							++it;
							continue;
						}
						next_current_ship = it;
						break;
					}
					assert(next_current_ship != to_erase);
					pDoc->m_ShipMap.EraseAt(to_erase);
					pDoc->SetCurrentShip(next_current_ship);

					Invalidate(FALSE);
					resources::pMainFrame->InvalidateView(RUNTIME_CLASS(CFleetMenuView));
					return;
				}
			}
		}
	}

	// �berpr�fen ob wir auf den "Next"-Button geklickt haben, falls wir mehr als 9 Schiffe in dem Sektor haben
	CRect r;
	r.SetRect(0,0,m_TotalSize.cx,m_TotalSize.cy);
	if (m_bShowNextButton == TRUE)
	{
		CRect next;
		next.SetRect(r.right-bw,r.top+210,r.right,r.top+240);
		if (next.PtInRect(point))
		{
			CGalaxyMenuView::SetMoveShip(FALSE);
			resources::pMainFrame->InvalidateView(RUNTIME_CLASS(CSmallInfoView));
			m_iPage++;
			Invalidate(FALSE);
		}
	}
	if (m_iPage > 1)	// haben wir auf den "back-button" geklickt
	{
		CRect back;
		back.SetRect(r.right-bw,r.top,r.right,r.top+bh);
		if (back.PtInRect(point))
		{
			CGalaxyMenuView::SetMoveShip(FALSE);
			resources::pMainFrame->InvalidateView(RUNTIME_CLASS(CSmallInfoView));
			m_iPage--;
			Invalidate(FALSE);
		}
	}
	// �berpr�fen, auf welchen Schiffsoberbefehle-Button ich geklickt habe
	if (CGalaxyMenuView::IsMoveShip() == TRUE)
	{
		for(std::vector<const MainButtonInfo>::const_iterator i = m_vMainShipOrders.begin();
			i != m_vMainShipOrders.end(); ++i)
		{
			if(!i->rect.PtInRect(point))
				continue;
			if(i->which != MAIN_BUTTON_CANCEL) {
				m_iWhichMainShipOrderButton = i->which;
				m_iTimeCounter = MAIN_BUTTON_CANCEL;
				SetTimer(1,100,NULL);
			}
		}
		// Ab jetzt die kleinen Buttons f�r die einzelnen genauen Schiffsbefehle
		network::RACE client = pDoc->GetRaceCtrl()->GetMappedClientID(pMajor->GetRaceID());
		for(std::vector<SecondaryButtonInfo>::const_iterator i = m_vSecondaryShipOrders.begin();
				i != m_vSecondaryShipOrders.end(); ++i)
			if (i->rect.PtInRect(point))
			{
				SHIP_ORDER::Typ nOrder = static_cast<SHIP_ORDER::Typ>(i - m_vSecondaryShipOrders.begin());
				// Bei manchen Befehlen m�ssen wir einen m�glichen Zielkurs wieder zur�cknehmen.
				if (nOrder != SHIP_ORDER::AVOID && nOrder != SHIP_ORDER::ATTACK && nOrder != SHIP_ORDER::ENCLOAK && nOrder != SHIP_ORDER::DECLOAK && nOrder != SHIP_ORDER::ASSIGN_FLAGSHIP && nOrder != SHIP_ORDER::CREATE_FLEET && nOrder != SHIP_ORDER::TRANSPORT)
					pDoc->CurrentShip()->second.SetTargetKO(CPoint(-1, -1), 0);
				// Wenn wir eine Flotte bilden wollen (Schiffe gruppieren), dann in der MainView die Flottenansicht zeigen
				if (nOrder == SHIP_ORDER::CREATE_FLEET)
				{
					pDoc->SetFleetShip(pDoc->CurrentShip()); // Dieses Schiff soll die Flotte beinhalten
					resources::pMainFrame->SelectMainView(FLEET_VIEW, pMajor->GetRaceID());		// Flottenansicht in der MainView anzeigen
				}
				// wenn wir ein Schiff zum Flagschiff ernennen wollen m�ssen wir schauen das diesen Befehl kein anderes
				// Schiff des Imperiums hat, wenn ja dann diesen Befehl l�schen
				else if (nOrder == SHIP_ORDER::ASSIGN_FLAGSHIP)
				{
					// Das ganze Schiffsarray und auch die Flotten durchgehen, wenn wir ein altes Flagschiff finden, diesem den
					// Titel wegnehmen
					for (CShipMap::iterator n = pDoc->m_ShipMap.begin(); n != pDoc->m_ShipMap.end(); ++n)
						if (n->second.GetOwnerOfShip() == pDoc->CurrentShip()->second.GetOwnerOfShip())
							if(n->second.UnassignFlagship(CShip::UNASSIGN_FLAGSHIP_MODE_ORDER))
								break;
					pDoc->CurrentShip()->second.SetCurrentOrder(SHIP_ORDER::ASSIGN_FLAGSHIP);
				}
				// Bei einem Transportbefehl muss in der MainView auch die Transportansicht angeblendet werden
				else if (nOrder == SHIP_ORDER::TRANSPORT)
				{
					resources::pMainFrame->SelectMainView(10, pMajor->GetRaceID());	// Transportansicht in der MainView anzeigen
					resources::pMainFrame->InvalidateView(RUNTIME_CLASS(CTransportMenuView));
				}
				else if (nOrder == SHIP_ORDER::TERRAFORM) {
					//command is given when clicked on planet
				}
				else if (nOrder == SHIP_ORDER::CANCEL)
					pDoc->CurrentShip()->second.UnsetCurrentOrder();
				else if (nOrder == SHIP_ORDER::ATTACK)
					pDoc->CurrentShip()->second.SetCombatTactic(COMBAT_TACTIC::CT_ATTACK);
				else if (nOrder == SHIP_ORDER::AVOID)
					pDoc->CurrentShip()->second.SetCombatTactic(COMBAT_TACTIC::CT_AVOID);
				// ansonsten ganz normal den Befehl geben
				else
					pDoc->CurrentShip()->second.SetCurrentOrder(nOrder);
				// bei Terraforming wird die Planetenansicht eingeblendet
				if (nOrder == SHIP_ORDER::TERRAFORM)
				{
					resources::pMainFrame->SelectBottomView(PLANET_BOTTOM_VIEW);
					resources::pMainFrame->InvalidateView(RUNTIME_CLASS(CPlanetBottomView));
					CSoundManager::GetInstance()->PlaySound(SNDMGR_MSG_TERRAFORM_SELECT, SNDMGR_PRIO_HIGH, 1.0f, client);
				}
				else
				{
					if (nOrder == SHIP_ORDER::COLONIZE)
						CSoundManager::GetInstance()->PlaySound(SNDMGR_MSG_COLONIZING, SNDMGR_PRIO_HIGH, 1.0f, client);
					else if (nOrder == SHIP_ORDER::BUILD_OUTPOST)
						CSoundManager::GetInstance()->PlaySound(SNDMGR_MSG_OUTPOST_CONSTRUCT, SNDMGR_PRIO_HIGH, 1.0f, client);
					else if (nOrder == SHIP_ORDER::BUILD_STARBASE)
						CSoundManager::GetInstance()->PlaySound(SNDMGR_MSG_STARBASE_CONSTRUCT, SNDMGR_PRIO_HIGH, 1.0f, client);
					CGalaxyMenuView::SetMoveShip(FALSE);
					CSmallInfoView::SetDisplayMode(CSmallInfoView::DISPLAY_MODE_SHIP_BOTTEM_VIEW);
					resources::pMainFrame->InvalidateView(RUNTIME_CLASS(CSmallInfoView));
				}
				Invalidate();
				break;
			}
	}

	CBottomBaseView::OnLButtonDown(nFlags, point);
}

void CShipBottomView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CBotf2Doc* pDoc = resources::pDoc;
	ASSERT(pDoc);

	if (!pDoc->m_bDataReceived)
		return;

	if (resources::pMainFrame->GetActiveView(0, 1) == FLEET_VIEW)
		return;

	CalcLogicalPoint(point);

	if (m_RectForTheShip.PtInRect(point))
	{
		CMajor* pMajor = m_pPlayersRace;
		ASSERT(pMajor);
		if (!pMajor)
			return;

		CGalaxyMenuView::SetMoveShip(FALSE);
		this->KillTimer(1);
		m_iTimeCounter = 0;
		pDoc->SetFleetShip(pDoc->CurrentShip()); // Dieses Schiff soll die Flotte beinhalten
		resources::pMainFrame->SelectMainView(FLEET_VIEW, pMajor->GetRaceID());	// Flottenansicht in der MainView anzeigen
	}

	CBottomBaseView::OnLButtonDblClk(nFlags, point);
}

void CShipBottomView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (CGalaxyMenuView::IsMoveShip())
		return;
	CBotf2Doc* pDoc = resources::pDoc;
	assert(pDoc);
	CalcLogicalPoint(point);
	// wurde die Maus �ber ein Schiff gehalten
	for(std::vector<std::pair<CRect, CShips*>>::const_iterator i = m_vShipRects.begin(); i != m_vShipRects.end(); ++i) {
		if (!i->first.PtInRect(point))
			continue;
		for(CShipMap::iterator j = pDoc->m_ShipMap.begin(); j != pDoc->m_ShipMap.end(); ++j) {
			if(&j->second != i->second)
				continue;
			pDoc->SetCurrentShip(j);
			CSmallInfoView::SetDisplayMode(CSmallInfoView::DISPLAY_MODE_SHIP_BOTTEM_VIEW);
			resources::pMainFrame->InvalidateView(RUNTIME_CLASS(CSmallInfoView));
			m_iWhichMainShipOrderButton = MAIN_BUTTON_NONE;
			break;
		}
		m_RectForTheShip = i->first;
		InvalidateRect(m_rLastMarkedRect, FALSE);
		CRect r = i->first;
		CalcDeviceRect(r);
		m_rLastMarkedRect = r;
		InvalidateRect(r, FALSE);
		return;
	}
	CBottomBaseView::OnMouseMove(nFlags, point);
}

void CShipBottomView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CBotf2Doc* pDoc = resources::pDoc;
	ASSERT(pDoc);

	if (!pDoc->m_bDataReceived)
		return;

	CalcLogicalPoint(point);

	if (CGalaxyMenuView::IsMoveShip() == TRUE)
	{
		CGalaxyMenuView::SetMoveShip(FALSE);
		if (resources::pMainFrame->GetActiveView(1, 1) == PLANET_BOTTOM_VIEW)	// Wenn wir kolon oder terraformen abbrechen wollen, zur�ck zum Schiffsmen�
		{
			pDoc->CurrentShip()->second.SetCurrentOrder(SHIP_ORDER::AVOID);
			m_bShowStation = false;
			Invalidate();
		}
		else // wenn wir schon im Schiffsmen� sind, dann brauchen wir nur die Befehlsbutton neu zeichnen
		{
			CRect r(809,30,1069,208);
			CalcDeviceRect(r);
			//InvalidateRect(r, FALSE);
			Invalidate();
			resources::pMainFrame->InvalidateView(RUNTIME_CLASS(CGalaxyMenuView));
		}
		CSmallInfoView::SetDisplayMode(CSmallInfoView::DISPLAY_MODE_SHIP_BOTTEM_VIEW);
		resources::pMainFrame->InvalidateView(RUNTIME_CLASS(CSmallInfoView));
	}

	CBottomBaseView::OnRButtonDown(nFlags, point);
}


void CShipBottomView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	m_iTimeCounter++;
	CRect r(830,20,m_TotalSize.cx, m_TotalSize.cy);
	CalcDeviceRect(r);
	InvalidateRect(r, FALSE);

	CBottomBaseView::OnTimer(nIDEvent);
}

///	Funktion erstellt zur aktuellen Mouse-Position einen HTML Tooltip
/// @return	der erstellte Tooltip-Text
CString CShipBottomView::CreateTooltip(void)
{
	CBotf2Doc* pDoc = resources::pDoc;
	ASSERT(pDoc);

	if (!pDoc->m_bDataReceived)
		return "";

	// Wo sind wir
	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	CalcLogicalPoint(pt);

	CMajor* pMajor = m_pPlayersRace;
	ASSERT(pMajor);
	if (!pMajor)
		return "";

	CShips* pShip = NULL;
	for (UINT i = 0; i < m_vShipRects.size(); i++)
		if (m_vShipRects[i].first.PtInRect(pt))
		{
			pShip = m_vShipRects[i].second;
			break;
		}

	if (!pShip)
		return "";

	// ist der Besitzer des Schiffes unbekannt?
	if (pMajor->GetRaceID() != pShip->GetOwnerOfShip())
	{
		bool bNoDiplomacy = false;
		CRace* pShipOwner = pDoc->GetRaceCtrl()->GetRace(pShip->GetOwnerOfShip());
		if (pShipOwner)
			bNoDiplomacy = pShipOwner->HasSpecialAbility(SPECIAL_NO_DIPLOMACY);

		if (!bNoDiplomacy && pMajor->IsRaceContacted(pShip->GetOwnerOfShip()) == false)
		{
			CString s = CResourceManager::GetString("UNKNOWN");
			s = CHTMLStringBuilder::GetHTMLColor(s);
			s = CHTMLStringBuilder::GetHTMLHeader(s, _T("h4"));
			s = CHTMLStringBuilder::GetHTMLCenter(s);
			return s;
		}
	}

	return pShip->GetTooltip();
}

/// Funktion ermittelt die Nummer des Schiffes im Array, �ber welches die Maus bewegt wurde.
/// @param pt Mauskoordinate
/// @return Nummer des Schiffes im Schiffsarray (<code>-1</code> wenn kein Schiff markiert wurde)
//int CShipBottomView::GetMouseOverShip(CPoint& pt)
//{
//	// TODO: Add your message handler code here and/or call default
//	const CBotf2Doc* const pDoc = resources::pDoc;
//	ASSERT(pDoc);
//
//	if (!pDoc->m_bDataReceived)
//		return -1;
//
//	const CMajor* const pMajor = m_pPlayersRace;
//	ASSERT(pMajor);
//	if (!pMajor)
//		return -1;
//
//	m_RectForTheShip.SetRect(0,0,0,0);
//
//	if (CGalaxyMenuView::IsMoveShip() == FALSE) {
//		USHORT counter = 0;
//		USHORT row = 0;
//		USHORT column = 0;
//		for (int i = 0; i < pDoc->m_ShipMap.GetSize(); i++) {
//			const CShip* const pShip = &pDoc->m_ShipMap.GetAt(i);
//			const CPoint& active_sector = pDoc->GetKO();
//			if(active_sector != pShip->GetKO())
//				continue;
//			const BOOL is_base = pShip->IsStation();
//			if (m_bShowStation != is_base)
//				continue;
//			// Schiffe mit zu guter Stealthpower werden hier nicht angezeigt.
//			USHORT stealthPower = MAXBYTE;
//			const CFleet* pFleet = pShip->GetFleet();
//			if (!pFleet)
//			{
//				stealthPower = pShip->GetStealthPower() * 20;
//				if (pShip->GetStealthPower() > 3 && pShip->GetCloak() == FALSE)
//					stealthPower = 3 * 20;
//			}
//			else
//				stealthPower = pFleet->GetFleetStealthPower(pShip);
//
//			const CString& sRaceID = pMajor->GetRaceID();
//			if (pShip->GetOwnerOfShip() != sRaceID
//				&& pDoc->GetSector(active_sector.x, active_sector.y).GetScanPower(sRaceID) <= stealthPower)
//				continue;
//			if (counter < m_iPage*9 && counter >= (m_iPage-1)*9)
//			{
//				// mehrere Spalten anlegen, falls mehr als 3 Schiffe in dem System sind
//				if (counter != 0 && counter%3 == 0)
//				{
//					column++;
//					row = 0;
//				}
//				if (counter%9 == 0)
//					column = 0;
//				m_RectForTheShip.SetRect(column*250+37,row*65+30,column*250+187,row*65+80);
//				if (m_RectForTheShip.PtInRect(pt))
//					// Maus wurde �ber ein Schiff bewegt -> Nummer zur�ckgeben
//					return i;
//			}
//			row++;
//			counter++;
//			if (counter > m_iPage*9)
//				break;
//		}
//	}
//
//	// kein Schiff markiert
//	return -1;
//}
