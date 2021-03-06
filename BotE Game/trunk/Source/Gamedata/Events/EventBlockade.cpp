#include "stdafx.h"
#include "EventBlockade.h"
#include "FontLoader.h"
#include "BotE.h"
#include "BotEDoc.h"
#include "Races\RaceController.h"

IMPLEMENT_SERIAL (CEventBlockade, CObject, 1)

CEventBlockade::CEventBlockade(void)
{
	m_Type = EVENT_SCREEN_TYPE_BLOCKADE;
}

CEventBlockade::CEventBlockade(const CString &sPlayersRaceID, const CString &headline, const CString &text)
	: CEventScreen(sPlayersRaceID, "Blockade", headline, text)
{
	m_Type = EVENT_SCREEN_TYPE_BLOCKADE;
}

CEventBlockade::~CEventBlockade(void)
{
}

///////////////////////////////////////////////////////////////////////
// Speichern / Laden
///////////////////////////////////////////////////////////////////////
void CEventBlockade::Serialize(CArchive &ar)
{
	__super::Serialize(ar);

	// wenn gespeichert wird
	if (ar.IsStoring())
	{
	}
	// wenn geladen wird
	else if (ar.IsLoading())
	{
	}
}

//////////////////////////////////////////////////////////////////////
// sonstige Funktionen
//////////////////////////////////////////////////////////////////////
void CEventBlockade::Create(void)
{
	CEventScreen::Create();
}

void CEventBlockade::Draw(Graphics* g, CGraphicPool* graphicPool) const
{
	CEventScreen::Draw(g, graphicPool);

	CString fontName = "";
	Gdiplus::REAL fontSize = 0.0;
	StringFormat fontFormat;
	SolidBrush fontBrush(static_cast<Gdiplus::ARGB>(Color::White));

	CBotEDoc* pDoc = resources::pDoc;
	AssertBotE(pDoc);

	CMajor* pMajor = dynamic_cast<CMajor*>(pDoc->GetRaceCtrl()->GetRace(m_sRace));
	AssertBotE(pMajor);

	CFontLoader::CreateGDIFont(pMajor, 5, fontName, fontSize);
	fontFormat.SetAlignment(StringAlignmentCenter);
	fontFormat.SetLineAlignment(StringAlignmentCenter);
	fontFormat.SetFormatFlags(StringFormatFlagsNoWrap);
	Gdiplus::Color color;
	CFontLoader::GetGDIFontColor(pMajor, 3, color);
	fontBrush.SetColor(color);

	g->DrawString(CComBSTR(m_strHeadline), -1, &Gdiplus::Font(CComBSTR(fontName), fontSize), RectF(0,0,1280,96), &fontFormat, &fontBrush);

	CFontLoader::CreateGDIFont(pMajor, 3, fontName, fontSize);
	fontFormat.SetAlignment(StringAlignmentCenter);
	fontFormat.SetLineAlignment(StringAlignmentCenter);
	fontFormat.SetFormatFlags(!StringFormatFlagsNoWrap);
	g->DrawString(CComBSTR(m_strText), -1, &Gdiplus::Font(CComBSTR(fontName), fontSize), RectF(100,450,1080,128), &fontFormat, &fontBrush);

	// Buttons zeichnen
	CFontLoader::GetGDIFontColor(pMajor, 2, color);
	fontBrush.SetColor(color);
	for (int i = 0; i < m_Buttons.GetSize(); i++)
		m_Buttons.GetAt(i)->DrawButton(*g, graphicPool, Gdiplus::Font(CComBSTR(fontName), fontSize), fontBrush);
}
