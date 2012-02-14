// ShipInfo.cpp: Implementierung der Klasse CShipInfo.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ShipInfo.h"
#include "Races\Empire.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL (CShipInfo, CObject, 1)
//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CShipInfo::CShipInfo()
{
}

CShipInfo::~CShipInfo()
{
}

//////////////////////////////////////////////////////////////////////
// Kopierkonstruktor
//////////////////////////////////////////////////////////////////////
CShipInfo::CShipInfo(const CShipInfo & rhs) : CShip(rhs)
{	
	m_iRace = rhs.m_iRace;					
	// n�tige Forschung
	m_iBioTech = rhs.m_iBioTech;
	m_iEnergyTech = rhs.m_iEnergyTech;
	m_iCompTech = rhs.m_iCompTech;
	m_iPropulsionTech = rhs.m_iPropulsionTech;
	m_iConstructionTech = rhs.m_iConstructionTech;
	m_iWeaponTech = rhs.m_iWeaponTech;	
	// aktuell n�tige Rohstoffe zum Bau des Schiffes
	m_iNeededIndustry = rhs.m_iNeededIndustry;
	m_iNeededTitan = rhs.m_iNeededTitan;
	m_iNeededDeuterium = rhs.m_iNeededDeuterium;
	m_iNeededDuranium = rhs.m_iNeededDuranium;
	m_iNeededCrystal = rhs.m_iNeededCrystal;
	m_iNeededIridium = rhs.m_iNeededIridium;
	m_iNeededDeritium = rhs.m_iNeededDeritium;
	// n�tige Rohstoffe zum Bau, so wie die Grundkosten sind, denn durch CalculateFinalCosts werden die n�tigen Rohstoffe
	// und Industrieleistung erh�ht. Beim n�chsten Aufruf brauchen wir aber wieder die urspr�nglichen Kosten
	m_iBaseIndustry = rhs.m_iBaseIndustry;
	m_iBaseTitan = rhs.m_iBaseTitan;
	m_iBaseDeuterium = rhs.m_iBaseDeuterium;
	m_iBaseDuranium = rhs.m_iBaseDuranium;
	m_iBaseCrystal = rhs.m_iBaseCrystal;
	m_iBaseIridium = rhs.m_iBaseIridium;
	m_iBaseDeritium = rhs.m_iBaseDeritium;
	// n�tiger Systemname
	m_strOnlyInSystem = rhs.m_strOnlyInSystem;
	// Schiffsklasse, welche durch diese Schiffsklasse ersetzt wird
	m_strObsoletesClass = rhs.m_strObsoletesClass;
}

//////////////////////////////////////////////////////////////////////
// Zuweisungsoperator
//////////////////////////////////////////////////////////////////////
CShipInfo & CShipInfo::operator=(const CShipInfo & rhs)
{
	if (this == &rhs)
		return *this;
	
	// Basisklasse
	__super::operator=(rhs);

	m_iRace = rhs.m_iRace;					
	// n�tige Forschung
	m_iBioTech = rhs.m_iBioTech;
	m_iEnergyTech = rhs.m_iEnergyTech;
	m_iCompTech = rhs.m_iCompTech;
	m_iPropulsionTech = rhs.m_iPropulsionTech;
	m_iConstructionTech = rhs.m_iConstructionTech;
	m_iWeaponTech = rhs.m_iWeaponTech;	
	// aktuell n�tige Rohstoffe zum Bau des Schiffes
	m_iNeededIndustry = rhs.m_iNeededIndustry;
	m_iNeededTitan = rhs.m_iNeededTitan;
	m_iNeededDeuterium = rhs.m_iNeededDeuterium;
	m_iNeededDuranium = rhs.m_iNeededDuranium;
	m_iNeededCrystal = rhs.m_iNeededCrystal;
	m_iNeededIridium = rhs.m_iNeededIridium;
	m_iNeededDeritium = rhs.m_iNeededDeritium;
	// n�tige Rohstoffe zum Bau, so wie die Grundkosten sind, denn durch CalculateFinalCosts werden die n�tigen Rohstoffe
	// und Industrieleistung erh�ht. Beim n�chsten Aufruf brauchen wir aber wieder die urspr�nglichen Kosten
	m_iBaseIndustry = rhs.m_iBaseIndustry;
	m_iBaseTitan = rhs.m_iBaseTitan;
	m_iBaseDeuterium = rhs.m_iBaseDeuterium;
	m_iBaseDuranium = rhs.m_iBaseDuranium;
	m_iBaseCrystal = rhs.m_iBaseCrystal;
	m_iBaseIridium = rhs.m_iBaseIridium;
	m_iBaseDeritium = rhs.m_iBaseDeritium;
	// n�tiger Systemname
	m_strOnlyInSystem = rhs.m_strOnlyInSystem;
	// Schiffsklasse, welche durch diese Schiffsklasse ersetzt wird
	m_strObsoletesClass = rhs.m_strObsoletesClass;

	return *this;
}


///////////////////////////////////////////////////////////////////////
// Speichern / Laden
///////////////////////////////////////////////////////////////////////
void CShipInfo::Serialize(CArchive &ar)		
{
	__super::Serialize(ar);

	// wenn gespeichert wird
	if (ar.IsStoring())
	{
		ar << m_iRace;
		ar << m_iBioTech;
		ar << m_iEnergyTech;
		ar << m_iCompTech;
		ar << m_iPropulsionTech;
		ar << m_iConstructionTech;
		ar << m_iWeaponTech;	
		ar << m_iNeededIndustry;
		ar << m_iNeededTitan;
		ar << m_iNeededDeuterium;
		ar << m_iNeededDuranium;
		ar << m_iNeededCrystal;
		ar << m_iNeededIridium;
		ar << m_iNeededDeritium;
		ar << m_iBaseIndustry;
		ar << m_iBaseTitan;
		ar << m_iBaseDeuterium;
		ar << m_iBaseDuranium;
		ar << m_iBaseCrystal;
		ar << m_iBaseIridium;
		ar << m_iBaseDeritium;
		ar << m_strOnlyInSystem;
		ar << m_strObsoletesClass;
	}
	// wenn geladen wird
	if (ar.IsLoading())
	{
		ar >> m_iRace;
		ar >> m_iBioTech;
		ar >> m_iEnergyTech;
		ar >> m_iCompTech;
		ar >> m_iPropulsionTech;
		ar >> m_iConstructionTech;
		ar >> m_iWeaponTech;	
		ar >> m_iNeededIndustry;
		ar >> m_iNeededTitan;
		ar >> m_iNeededDeuterium;
		ar >> m_iNeededDuranium;
		ar >> m_iNeededCrystal;
		ar >> m_iNeededIridium;
		ar >> m_iNeededDeritium;
		ar >> m_iBaseIndustry;
		ar >> m_iBaseTitan;
		ar >> m_iBaseDeuterium;
		ar >> m_iBaseDuranium;
		ar >> m_iBaseCrystal;
		ar >> m_iBaseIridium;
		ar >> m_iBaseDeritium;
		ar >> m_strOnlyInSystem;
		ar >> m_strObsoletesClass;
	}
}

// Funktion l�scht die Bewaffnung
void CShipInfo::DeleteWeapons()
{
	for (int i = 0; i < m_TorpedoWeapons.GetSize(); )
		m_TorpedoWeapons.RemoveAt(i);
	for (int i = 0; i < m_BeamWeapons.GetSize(); )
		m_BeamWeapons.RemoveAt(i);
	m_TorpedoWeapons.RemoveAll();
	m_BeamWeapons.RemoveAll();
}

// Funktion berechnet die finalen Kosten zum Bau des Schiffes. Diese mu� immer aufgerufen werden, wenn wir was an
// dem Schiffsobjekt ge�ndert haben.
void CShipInfo::CalculateFinalCosts()
{
	m_iNeededIndustry	= 0;
	m_iNeededTitan		= 0;
	m_iNeededDeuterium	= 0;
	m_iNeededDuranium	= 0;
	m_iNeededCrystal	= 0;
	m_iNeededIridium	= 0;
	m_iNeededDeritium	= 0;
	// Industrieleistung, die wir zus�tzlich durch bessere Schilde erbringen m�ssen
/*	m_iNeededIndustry += m_Shield.GetMaxShield() / 10 * m_Shield.GetShieldType();
	// Industrieleistung, die wir zus�tzlich durch bessere H�lle  erbringen m�ssen
	// besseres Material erh�ht die Industrieleistung nicht, sondern nur ob wir eine Doppelh�lle haben
	m_iNeededIndustry += (USHORT)(m_Hull.GetBaseHull() * (m_Hull.GetDoubleHull()));
	// Industrieleistung die wir zus�tzlich erbringen m�ssen, wenn wir besserer Beams dranbauen
	for (int i = 0; i < m_BeamWeapons.GetSize(); i++)
		m_iNeededIndustry += (USHORT)(10 * (m_BeamWeapons.GetAt(i).GetBeamPower() * m_BeamWeapons.GetAt(i).GetBeamType()));
	// Industrieleistung die wir zus�tzlich erbringen m�ssen, wenn wir besserer Torpedos dranbauen
//	for (int i = 0; i < m_TorpedoWeapons.GetSize(); i++)
//		m_iNeededIndustry += (USHORT)((m_TorpedoWeapons.GetAt(i).GetTorpedoPower() * m_TorpedoWeapons.GetAt(i).GetNumber()) 
//							 / m_TorpedoWeapons.GetAt(i).GetTupeFirerate() * m_TorpedoWeapons.GetAt(i).GetNumberOfTupes() * 50);
*/	
	// Ab hier neue Kostenberechnung. Es geht mehr der Wert aller 100s ein. Also nicht mehr der absolute St�rkewert.
	// Es wird nur ein gewisser Anteil durch den Beamtype mit aufgerechnet, da dieser sonst keinen Einfluss auf den
	// Beamschaden hat, nur auf die Reichweite der Beams.
	USHORT beamTypeAdd = 0;
	for (int i = 0; i < m_BeamWeapons.GetSize(); i++)
		beamTypeAdd += (USHORT)(GetBeamWeapons()->GetAt(i).GetBeamPower()
								* GetBeamWeapons()->GetAt(i).GetBeamNumber()
								* m_BeamWeapons.GetAt(i).GetBeamType()
								* 3);
	if (m_BeamWeapons.GetSize() > 1)
		beamTypeAdd		/= m_BeamWeapons.GetSize();
	m_iNeededIndustry	+= beamTypeAdd;
	m_iNeededIndustry	+= this->GetCompleteOffensivePower();
	// Eine Doppelh�lle verteuert die Industriekosten nochmals um 25%
	if (m_Hull.GetDoubleHull())
		m_iNeededIndustry	+= (UINT)(this->GetCompleteDefensivePower() / 1.5);
	else
		m_iNeededIndustry	+= this->GetCompleteDefensivePower() / 2;
	
	m_iNeededIndustry	+= GetShield()->GetMaxShield() / 200 * ((USHORT)pow((float)GetShield()->GetShieldType(), 2.5f));
	m_iNeededIndustry	/= 2;

	// Kosten erstmal wieder auf die Ausgangswerte setzen
	m_iNeededIndustry	+= m_iBaseIndustry;
	m_iNeededTitan		+= m_iBaseTitan;
	m_iNeededDeuterium	+= m_iBaseDeuterium;
	m_iNeededDuranium	+= m_iBaseDuranium;
	m_iNeededCrystal	+= m_iBaseCrystal;
	m_iNeededIridium	+= m_iBaseIridium;
	m_iNeededDeritium	+= m_iBaseDeritium;
	
	// zus�tzliche Rohstoffe die wir f�r bessere Anbauten/Umbauten ben�tigen
	// wir brauchen f�r bessere H�llen auch besseres Material
	switch (m_Hull.GetHullMaterial())
	{
	case TITAN: m_iNeededTitan += (USHORT)(m_Hull.GetBaseHull() * (m_Hull.GetDoubleHull()+1)); break;
	case DURANIUM: m_iNeededDuranium += (USHORT)(m_Hull.GetBaseHull() * (m_Hull.GetDoubleHull()+1)); break;
	case IRIDIUM: m_iNeededIridium += (USHORT)(m_Hull.GetBaseHull() * (m_Hull.GetDoubleHull()+1)); break;
	}
	// f�r bessere Torpedos (ab 500 Dmg und mehr) brauchen wir zus�tzliches Deuterium
	for (int i = 0; i < m_TorpedoWeapons.GetSize(); i++)
	{
		if (m_TorpedoWeapons.GetAt(i).GetTorpedoPower() >= 500)
			m_iNeededDeuterium += m_TorpedoWeapons.GetAt(i).GetTorpedoPower() * m_TorpedoWeapons.GetAt(i).GetNumber()
			* m_TorpedoWeapons.GetAt(i).GetNumberOfTupes();	
	}
}

// Funktion bestimmt die 1. Order des Schiffs nach dem Bau anhand dessen Typs
void CShipInfo::SetStartOrder()
{
	if (m_iShipType >= SCOUT)
		m_iCurrentOrder = ATTACK;
	else
		m_iCurrentOrder = AVOID;
}


// Funktion zeichnet wichtige Informationen zu dem Schiff
void CShipInfo::DrawShipInformation(Graphics* g, CRect rect, Gdiplus::Font* font, Gdiplus::Color clrNormal, Gdiplus::Color clrMark, CResearch* research)
{
	CRect r;
	CString s;	
	r.SetRect(rect.left,rect.top+8,rect.right,rect.top+33);
	s.Format("%s",this->GetShipTypeAsString());
	
	SolidBrush fontBrush(clrMark);
	StringFormat fontFormat;
	fontFormat.SetAlignment(StringAlignmentCenter);
	fontFormat.SetLineAlignment(StringAlignmentNear);
	fontFormat.SetFormatFlags(StringFormatFlagsNoWrap);
	g->DrawString(s.AllocSysString(), -1, font, RectF((REAL)r.left, (REAL)r.top, (REAL)r.Width(), (REAL)r.Height()), &fontFormat, &fontBrush);
	
	// Gr��e, Unterhalt und Reichweite anzeigen
	r.SetRect(rect.left,rect.top+36,rect.right,rect.top+56);
	CString speed = CResourceManager::GetString("SPEED");
	CString range = CResourceManager::GetString("RANGE");
	BYTE tmpRange = m_iRange;
	BYTE tmpSpeed = m_iSpeed;
	if (research->GetResearchInfo()->GetResearchComplex(2)->GetFieldStatus(1) == RESEARCHED)
	{
		if (m_iRange == RANGE_SHORT)
			tmpRange = (BYTE)(research->GetResearchInfo()->GetResearchComplex(2)->GetBonus(1));
	}
	else if (research->GetResearchInfo()->GetResearchComplex(2)->GetFieldStatus(2) == RESEARCHED)
	{
		if (m_iSpeed == 1)
			tmpSpeed = (BYTE)(research->GetResearchInfo()->GetResearchComplex(2)->GetBonus(2));
	}
	
	if (rect.Width() >= 501)
	{
		CString Size = CResourceManager::GetString("SHIPSIZE");		
		CString sMaintenance = CResourceManager::GetString("SHIPCOSTS");
		switch (tmpRange)
		{
		case RANGE_SHORT: s.Format("%s: %d  -  %s: %d  -  %s: %d  -  %s: %s",Size, m_byShipSize, sMaintenance, m_iMaintenanceCosts, speed, tmpSpeed, range, CResourceManager::GetString("SHORT"));break;
		case RANGE_MIDDLE: s.Format("%s: %d  -  %s: %d  -  %s: %d  -  %s: %s",Size, m_byShipSize, sMaintenance, m_iMaintenanceCosts, speed, tmpSpeed, range, CResourceManager::GetString("MIDDLE")); break;
		case RANGE_LONG: s.Format("%s: %d  -  %s: %d  -  %s: %d  -  %s: %s",Size, m_byShipSize, sMaintenance, m_iMaintenanceCosts, speed, tmpSpeed, range, CResourceManager::GetString("LONG")); break;
		}
	}
	else
	{
		switch (tmpRange)
		{
		case RANGE_SHORT: s.Format("%s: %d  -  %s: %s",speed, tmpSpeed, range, CResourceManager::GetString("SHORT"));break;
		case RANGE_MIDDLE: s.Format("%s: %d  -  %s: %s",speed, tmpSpeed, range, CResourceManager::GetString("MIDDLE")); break;
		case RANGE_LONG: s.Format("%s: %d  -  %s: %s",speed, tmpSpeed, range, CResourceManager::GetString("LONG")); break;
		}
	}
	
	fontBrush.SetColor(clrNormal);
	g->DrawString(s.AllocSysString(), -1, font, RectF((REAL)r.left, (REAL)r.top, (REAL)r.Width(), (REAL)r.Height()), &fontFormat, &fontBrush);
	fontBrush.SetColor(clrMark);
	
	r.SetRect(rect.left,rect.top+62,rect.right,rect.top+84);
	s = CResourceManager::GetString("ARMAMENT");
	g->DrawString(s.AllocSysString(), -1, font, RectF((REAL)r.left, (REAL)r.top, (REAL)r.Width(), (REAL)r.Height()), &fontFormat, &fontBrush);
	// Waffen typenrein sammeln
	std::map<CString, int> mBeamWeapons;
	for (int i = 0; i < m_BeamWeapons.GetSize(); i++)
	{
		s.Format("%s %d %s", CResourceManager::GetString("TYPE"), m_BeamWeapons.GetAt(i).GetBeamType(),m_BeamWeapons.GetAt(i).GetBeamName());
		mBeamWeapons[s] += m_BeamWeapons.GetAt(i).GetBeamNumber();
	}

	std::map<CString, int> mTorpedoWeapons;
	for (int i = 0; i < m_TorpedoWeapons.GetSize(); i++)
	{
		s.Format("%s (%s)", m_TorpedoWeapons.GetAt(i).GetTupeName(), m_TorpedoWeapons.GetAt(i).GetTorpedoName());
		mTorpedoWeapons[s] += m_TorpedoWeapons.GetAt(i).GetNumberOfTupes();				
	}

	// Waffenbeschreibungstext zusammenbauen
	CString sWeapons = "";
	for (map<CString, int>::const_iterator it = mBeamWeapons.begin(); it != mBeamWeapons.end(); ++it)
	{
		if (sWeapons != "")
			sWeapons += ", ";

		s.Format("%d x %s", it->second, it->first);
		sWeapons += s;
	}
	for (map<CString, int>::const_iterator it = mTorpedoWeapons.begin(); it != mTorpedoWeapons.end(); ++it)
	{
		if (sWeapons != "")
			sWeapons += ", ";

		s.Format("%d x %s", it->second, it->first);
		sWeapons += s;
	}

	if (sWeapons == "")
		sWeapons = CResourceManager::GetString("NONE");
	r.SetRect(rect.left,rect.top+84,rect.right,rect.top+143);
	fontBrush.SetColor(clrNormal);
	fontFormat.SetFormatFlags(!StringFormatFlagsNoWrap);
	g->DrawString(sWeapons.AllocSysString(), -1, font, RectF((REAL)r.left, (REAL)r.top, (REAL)r.Width(), (REAL)r.Height()), &fontFormat, &fontBrush);
			
	// Wenn das Rechteck, welches wir an diese Funktion �bergeben haben breiter als 500 ist, dann wird der Abstand
	// zwischen Bewaffnung und "Schilde und H�lle" um eine Zeile verk�rzt
	USHORT sub = 0;
	if (rect.Width() > 500)
		sub = 20;
	r.SetRect(rect.left,rect.top+143-sub,rect.right,rect.top+165-sub);
	s = CResourceManager::GetString("SHIELDS")+" "+CResourceManager::GetString("AND")+" "+CResourceManager::GetString("HULL");
	fontFormat.SetFormatFlags(StringFormatFlagsNoWrap);
	fontBrush.SetColor(clrMark);
	g->DrawString(s.AllocSysString(), -1, font, RectF((REAL)r.left, (REAL)r.top, (REAL)r.Width(), (REAL)r.Height()), &fontFormat, &fontBrush);
	
	r.SetRect(rect.left,rect.top+165-sub,rect.right,rect.top+185-sub);
	float shieldBoni = 1.0f;
	if (research->GetResearchInfo()->GetResearchComplex(1)->GetFieldStatus(1) == RESEARCHED)
		shieldBoni += (float)(research->GetResearchInfo()->GetResearchComplex(1)->GetBonus(1)) / 100;

	s.Format("%s %d %s: %s %d",CResourceManager::GetString("TYPE"),m_Shield.GetShieldType(),
		CResourceManager::GetString("SHIELDS"), CResourceManager::GetString("CAPACITY"), (UINT)(m_Shield.GetMaxShield() * shieldBoni));
	fontBrush.SetColor(clrNormal);
	g->DrawString(s.AllocSysString(), -1, font, RectF((REAL)r.left, (REAL)r.top, (REAL)r.Width(), (REAL)r.Height()), &fontFormat, &fontBrush);
	r.SetRect(rect.left,rect.top+185-sub,rect.right,rect.top+225-sub);
	CString material;
	switch (m_Hull.GetHullMaterial())
	{
	case TITAN: material = CResourceManager::GetString("TITAN");; break;
	case DURANIUM: material = CResourceManager::GetString("DURANIUM");; break;
	case IRIDIUM: material = CResourceManager::GetString("IRIDIUM");; break;
	default: material = "";
	}
	float hullBoni = 1.0f;
	if (research->GetResearchInfo()->GetResearchComplex(1)->GetFieldStatus(2) == RESEARCHED)
		hullBoni += (float)(research->GetResearchInfo()->GetResearchComplex(1)->GetBonus(2)) / 100;
	
	if (m_Hull.GetDoubleHull() == TRUE)
		s.Format("%s%s: %s %d",material, CResourceManager::GetString("DOUBLE_HULL_ARMOUR"),
			CResourceManager::GetString("INTEGRITY"), (int)(m_Hull.GetMaxHull()*hullBoni));
	else
		s.Format("%s%s: %s %d",material, CResourceManager::GetString("HULL_ARMOR"),
			CResourceManager::GetString("INTEGRITY"), (int)(m_Hull.GetMaxHull()*hullBoni));
	fontFormat.SetFormatFlags(StringFormatFlagsNoWrap);
	g->DrawString(s.AllocSysString(), -1, font, RectF((REAL)r.left, (REAL)r.top, (REAL)r.Width(), (REAL)r.Height()), &fontFormat, &fontBrush);
		
	if (rect.Width() >= 501)
	{
		// Man�vrierbarkeit anzeigen
		fontBrush.SetColor(clrMark);
		r.SetRect(rect.left,rect.top+266,rect.right,rect.top+288);
		g->DrawString(CResourceManager::GetString("MANEUVERABILITY").AllocSysString(), -1, font, RectF((REAL)r.left, (REAL)r.top, (REAL)r.Width(), (REAL)r.Height()), &fontFormat, &fontBrush);
		switch (m_byManeuverability)
		{
		case 9: s = CResourceManager::GetString("PHENOMENAL");	break;
		case 8:	s = CResourceManager::GetString("EXCELLENT");	break;
		case 7:	s = CResourceManager::GetString("VERYGOOD");	break;
		case 6:	s = CResourceManager::GetString("GOOD");		break;
		case 5:	s = CResourceManager::GetString("NORMAL");		break;
		case 4:	s = CResourceManager::GetString("ADEQUATE");	break;
		case 3:	s = CResourceManager::GetString("BAD");			break;
		case 2:	s = CResourceManager::GetString("VERYBAD");		break;
		case 1:	s = CResourceManager::GetString("MISERABLE");	break;
		default:s = CResourceManager::GetString("NONE");
		}
		fontBrush.SetColor(clrNormal);
		r.SetRect(rect.left,rect.top+288,rect.right,rect.top+308);
		g->DrawString(s.AllocSysString(), -1, font, RectF((REAL)r.left, (REAL)r.top, (REAL)r.Width(), (REAL)r.Height()), &fontFormat, &fontBrush);

		// Scan anzeigen
		fontBrush.SetColor(clrMark);
		r.SetRect(rect.left,rect.top+311,rect.right,rect.top+333);
		g->DrawString(CResourceManager::GetString("SENSORS").AllocSysString(), -1, font, RectF((REAL)r.left, (REAL)r.top, (REAL)r.Width(), (REAL)r.Height()), &fontFormat, &fontBrush);
		r.SetRect(rect.left,rect.top+333,rect.right,rect.top+353);
		CString Scanrange = CResourceManager::GetString("SCANRANGE");
		CString Scanpower = CResourceManager::GetString("SCANPOWER");
		BYTE ScanRange = m_iScanRange;
		USHORT ScanPower = m_iScanPower;
		s.Format("%s: %d  -  %s: %d",Scanrange, ScanRange, Scanpower, ScanPower);
		fontBrush.SetColor(clrNormal);
		g->DrawString(s.AllocSysString(), -1, font, RectF((REAL)r.left, (REAL)r.top, (REAL)r.Width(), (REAL)r.Height()), &fontFormat, &fontBrush);
		fontBrush.SetColor(clrMark);

		// Spezialf�higkeiten anzeigen
		fontBrush.SetColor(clrMark);
		r.SetRect(rect.left,rect.top+356,rect.right,rect.top+378);
		g->DrawString(CResourceManager::GetString("SPECIAL_ABILITIES").AllocSysString(), -1, font, RectF((REAL)r.left, (REAL)r.top, (REAL)r.Width(), (REAL)r.Height()), &fontFormat, &fontBrush);
		s = "";
		fontBrush.SetColor(clrNormal);
		if (m_bySpecial[0] == ASSULTSHIP || m_bySpecial[1] == ASSULTSHIP)
			s += CResourceManager::GetString("ASSAULTSHIP")+" \n";
		if (m_bySpecial[0] == BLOCKADESHIP || m_bySpecial[1] == BLOCKADESHIP)
			s += CResourceManager::GetString("BLOCKADESHIP")+" \n";
		if (m_bySpecial[0] == COMMANDSHIP || m_bySpecial[1] == COMMANDSHIP)
			s += CResourceManager::GetString("COMMANDSHIP")+" \n";
		if (m_bySpecial[0] == DOGFIGHTER || m_bySpecial[1] == DOGFIGHTER)
			s += CResourceManager::GetString("DOGFIGHTER")+" \n";
		if (m_bySpecial[0] == DOGKILLER || m_bySpecial[1] == DOGKILLER)
			s += CResourceManager::GetString("DOGKILLER")+" \n";
		if (m_bySpecial[0] == PATROLSHIP || m_bySpecial[1] == PATROLSHIP)
			s += CResourceManager::GetString("PATROLSHIP")+" \n";
		if (m_bySpecial[0] == RAIDER || m_bySpecial[1] == RAIDER)
			s += CResourceManager::GetString("RAIDER")+" \n";
		if (m_bySpecial[0] == SCIENCEVESSEL || m_bySpecial[1] == SCIENCEVESSEL)
			s += CResourceManager::GetString("SCIENCESHIP")+" \n";
		if (m_Shield.GetRegenerative())
			s += CResourceManager::GetString("REGENERATIVE_SHIELDS")+" \n";
		if (m_Hull.GetAblative())
			s += CResourceManager::GetString("ABLATIVE_ARMOR")+" \n";
		if (m_Hull.GetPolarisation())
			s += CResourceManager::GetString("HULLPOLARISATION")+" \n";
		if (m_iStealthPower > 3)
			s += CResourceManager::GetString("CAN_CLOAK")+" \n";
		if (s.IsEmpty())
			s = CResourceManager::GetString("NONE")+" \n";;
		r.SetRect(rect.left,rect.top+378,rect.right,rect.top+478);
		fontBrush.SetColor(clrNormal);
		fontFormat.SetFormatFlags(!StringFormatFlagsNoWrap);
		g->DrawString(s.AllocSysString(), -1, font, RectF((REAL)r.left, (REAL)r.top, (REAL)r.Width(), (REAL)r.Height()), &fontFormat, &fontBrush);		
	}
}

/// Funktion gibt zur�ck, ob das Schiff mit der aktuellen Forschung einer Rasse baubar ist.
/// @param researchLevels Forschungsstufen der Rasse
/// @return Wahrheitswert
bool CShipInfo::IsThisShipBuildableNow(const BYTE reserachLevels[6]) const
{
	// zuerstmal die Forschungsstufen checken
	if (reserachLevels[5] < this->GetWeaponTech())
		return false;
	if (reserachLevels[4] < this->GetConstructionTech())
		return false;
	if (reserachLevels[3] < this->GetPropulsionTech())
		return false;	
	if (reserachLevels[2] < this->GetComputerTech())
		return false;
	if (reserachLevels[1] < this->GetEnergyTech())
		return false;
	if (reserachLevels[0] < this->GetBioTech())
		return false;
	
	return true;
}

USHORT CShipInfo::GetNeededResource(BYTE res) const
{
	switch (res)
	{
	case TITAN:		return this->GetNeededTitan();
	case DEUTERIUM: return this->GetNeededDeuterium();
	case DURANIUM:	return this->GetNeededDuranium();
	case CRYSTAL:	return this->GetNeededCrystal();
	case IRIDIUM:	return this->GetNeededIridium();
	case DERITIUM:	return this->GetNeededDeritium();
	default:		return NULL;
	}
}