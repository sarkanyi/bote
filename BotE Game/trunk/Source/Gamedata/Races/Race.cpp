#include "stdafx.h"
#include "Race.h"
#include "BotE.h"
#include "BotEDoc.h"
#include "RaceController.h"
#include "AI\DiplomacyAI.h"
#include "AI\MajorAI.h"
#include "HTMLStringBuilder.h"
#include "General/Loc.h"



//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CRace::CRace(RACE_TYPE type) :
	CInGameEntity(),
	m_sID(),
	m_sHomeSystem(),
	m_sNameArticle(),
	m_RaceType(type),
	m_nProperty(0),
	m_byShipNumber(0),
	m_byBuildingNumber(0),
	m_byMoralNumber(0),
	m_nSpecialAbility(0),
	m_bDeleted(false),
	m_sGraphicFile(),
	m_pDiplomacyAI()
{
}

CRace::~CRace(void)
{
}

///////////////////////////////////////////////////////////////////////
// Speichern / Laden
///////////////////////////////////////////////////////////////////////
void CRace::Serialize(CArchive &ar)
{
	CInGameEntity::Serialize(ar);

	// wenn gespeichert wird
	if (ar.IsStoring())
	{
		ar << m_sID;			// Rassen-ID
		ar << m_sHomeSystem;	// Name des Heimatsystems
		ar << m_sNameArticle;	// Artikel f�r Rassenname
		ar << m_nProperty;		// Rasseneigenschaften
		ar << m_byShipNumber;	// zugewiesene Nummer welche Schiffe verwendet werden sollen
		ar << m_byBuildingNumber;	// zugewiesene Nummer welche Geb�ude verwendet werden sollen
		ar << m_byMoralNumber;	// zugewiesene Nummer welche Moralwerte verwendet werden sollen
		ar << m_nSpecialAbility;// Spezialf�higkeiten der Rasse

		// Ingame-Attribute (Rassenwechselwirkung)
		// Beziehungsmap (Rassen-ID, Beziehungswert)
		ar << m_mRelations.size();
		for (map<CString, BYTE>::const_iterator it = m_mRelations.begin(); it != m_mRelations.end(); ++it)
			ar << it->first << it->second;
		// Diplomatischer Status gegen�ber anderen Rassen (Rassen-ID, Status)
		ar << m_mAgreement.size();
		for (map<CString, DIPLOMATIC_AGREEMENT::Typ>::const_iterator it = m_mAgreement.begin(); it != m_mAgreement.end(); ++it)
			ar << it->first << it->second;
		// kennt die Rasse eine andere Rasse (Rassen-ID, Wahrheitswert)
		ar << m_vInContact.size();
		for (set<CString>::const_iterator it = m_vInContact.begin(); it != m_vInContact.end(); ++it)
			ar << *it;
		// diplomatische Nachrichten
		ar << m_vDiplomacyNewsIn.size();
		for (vector<CDiplomacyInfo>::iterator it = m_vDiplomacyNewsIn.begin(); it != m_vDiplomacyNewsIn.end(); ++it)
			it->Serialize(ar);
		ar << m_vDiplomacyNewsOut.size();
		for (vector<CDiplomacyInfo>::iterator it = m_vDiplomacyNewsOut.begin(); it != m_vDiplomacyNewsOut.end(); ++it)
			it->Serialize(ar);

		// gemachte Angebote der letzten beiden Runden
		ar << m_mLastOffers.size();
		for (map<CString, CDiplomacyInfo>::iterator it = m_mLastOffers.begin(); it != m_mLastOffers.end(); ++it)
		{
			ar << it->first;
			it->second.Serialize(ar);
		}

		// grafische Attribute
		ar << m_sGraphicFile;	// Name der zugeh�rigen Grafikdatei
	}
	// wenn geladen wird
	else if (ar.IsLoading())
	{
		ar >> m_sID;			// Rassen-ID
		ar >> m_sHomeSystem;	// Name des Heimatsystems
		ar >> m_sNameArticle;	// Artikel f�r Rassenname
		ar >> m_nProperty;		// Rasseneigenschaften
		ar >> m_byShipNumber;	// zugewiesene Nummer welche Schiffe verwendet werden sollen
		ar >> m_byBuildingNumber;	// zugewiesene Nummer welche Geb�ude verwendet werden sollen
		ar >> m_byMoralNumber;	// zugewiesene Nummer welche Moralwerte verwendet werden sollen
		ar >> m_nSpecialAbility;// Spezialf�higkeiten der Rasse

		// Ingame-Attribute (Rassenwechselwirkung)
		// Beziehungsmap (Rassen-ID, Beziehungswert)
		m_mRelations.clear();
		size_t mapSize = 0;
		ar >> mapSize;
		for (size_t i = 0; i < mapSize; i++)
		{
			CString key;
			BYTE value;
			ar >> key;
			ar >> value;
			m_mRelations[key] = value;
		}
		// Diplomatischer Status gegen�ber anderen Rassen (Rassen-ID, Status)
		m_mAgreement.clear();
		mapSize = 0;
		ar >> mapSize;
		for (size_t i = 0; i < mapSize; i++)
		{
			CString key;
			int value;
			ar >> key;
			ar >> value;
			m_mAgreement[key] = (DIPLOMATIC_AGREEMENT::Typ)value;
		}
		// kennt die Rasse eine andere Rasse (Rassen-ID, Wahrheitswert)
		m_vInContact.clear();
		size_t vectorSize = 0;
		ar >> vectorSize;
		for (size_t i = 0; i < vectorSize; i++)
		{
			CString sID;
			ar >> sID;
			m_vInContact.insert(sID);
		}
		// diplomatische Nachrichten
		m_vDiplomacyNewsIn.clear();
		vectorSize = 0;
		ar >> vectorSize;
		m_vDiplomacyNewsIn.resize(vectorSize);
		for (size_t i = 0; i < vectorSize; i++)
			m_vDiplomacyNewsIn[i].Serialize(ar);
		// diplomatische Nachrichten
		m_vDiplomacyNewsOut.clear();
		vectorSize = 0;
		ar >> vectorSize;
		m_vDiplomacyNewsOut.resize(vectorSize);
		for (size_t i = 0; i < vectorSize; i++)
			m_vDiplomacyNewsOut[i].Serialize(ar);

		// gemachte Angebote der letzten beiden Runden
		m_mLastOffers.clear();
		mapSize = 0;
		ar >> mapSize;
		for (size_t i = 0; i < mapSize; i++)
		{
			CString key;
			ar >> key;
			CDiplomacyInfo info;
			info.Serialize(ar);
			m_mLastOffers[key] = info;
		}

		// grafische Attribute
		ar >> m_sGraphicFile;	// Name der zugeh�rigen Grafikdatei
	}
}

//////////////////////////////////////////////////////////////////////
// sonstige Funktionen
//////////////////////////////////////////////////////////////////////

bool CRace::IsEnemyOf(const CString& other) const
{
	if(other == m_sID)
		return false;
	const DIPLOMATIC_AGREEMENT::Typ agreement = GetAgreement(other);
	if (agreement >= DIPLOMATIC_AGREEMENT::FRIENDSHIP)
		return false;
	return agreement != DIPLOMATIC_AGREEMENT::NAP;
}

/// Funktion zum setzen der Rasseneigenschaften
/// @param nProp Rasseneigenschaft
/// @return <code>true</code>, wenn die Rasse die Eigenschaft besitzt, sonst <code>false</code>
bool CRace::IsRaceProperty(RACE_PROPERTY::Typ nProp) const
{
	switch (nProp)
	{
		case RACE_PROPERTY::AGRARIAN:	return (m_nProperty & RACE_AGRARIAN)	== RACE_AGRARIAN;
		case RACE_PROPERTY::FINANCIAL:	return (m_nProperty & RACE_FINANCIAL)	== RACE_FINANCIAL;
		case RACE_PROPERTY::HOSTILE:	return (m_nProperty & RACE_HOSTILE)		== RACE_HOSTILE;
		case RACE_PROPERTY::INDUSTRIAL:	return (m_nProperty & RACE_INDUSTRIAL)	== RACE_INDUSTRIAL;
		case RACE_PROPERTY::PACIFIST:	return (m_nProperty & RACE_PACIFIST)	== RACE_PACIFIST;
		case RACE_PROPERTY::PRODUCER:	return (m_nProperty & RACE_PRODUCER)	== RACE_PRODUCER;
		case RACE_PROPERTY::SCIENTIFIC:	return (m_nProperty & RACE_SCIENTIFIC)	== RACE_SCIENTIFIC;
		case RACE_PROPERTY::SECRET:		return (m_nProperty & RACE_SECRET)		== RACE_SECRET;
		case RACE_PROPERTY::SNEAKY:		return (m_nProperty & RACE_SNEAKY)		== RACE_SNEAKY;
		case RACE_PROPERTY::SOLOING:	return (m_nProperty & RACE_SOLOING)		== RACE_SOLOING;
		case RACE_PROPERTY::WARLIKE:	return (m_nProperty & RACE_WARLIKE)		== RACE_WARLIKE;
	}

	AssertBotE(FALSE);
	return RACE_PROPERTY::NOTHING_SPECIAL;
}

/// Funktion zum Setzen von Rasseneigenschaften.
/// @param nProp Rasseneigenschaft
/// @param is <code>true</code> oder <code>false</code>
void CRace::SetRaceProperty(RACE_PROPERTY::Typ nProp, bool is)
{
	switch (nProp)
	{
		case RACE_PROPERTY::NOTHING_SPECIAL:	if (is) m_nProperty = 0;							break;
		case RACE_PROPERTY::AGRARIAN:			SetAttributes(is, RACE_AGRARIAN, m_nProperty);		break;
		case RACE_PROPERTY::FINANCIAL:			SetAttributes(is, RACE_FINANCIAL, m_nProperty);		break;
		case RACE_PROPERTY::HOSTILE:			SetAttributes(is, RACE_HOSTILE, m_nProperty);		break;
		case RACE_PROPERTY::INDUSTRIAL:			SetAttributes(is, RACE_INDUSTRIAL, m_nProperty);	break;
		case RACE_PROPERTY::PACIFIST:			SetAttributes(is, RACE_PACIFIST, m_nProperty);		break;
		case RACE_PROPERTY::PRODUCER:			SetAttributes(is, RACE_PRODUCER, m_nProperty);		break;
		case RACE_PROPERTY::SCIENTIFIC:			SetAttributes(is, RACE_SCIENTIFIC, m_nProperty);	break;
		case RACE_PROPERTY::SECRET:				SetAttributes(is, RACE_SECRET, m_nProperty);		break;
		case RACE_PROPERTY::SNEAKY:				SetAttributes(is, RACE_SNEAKY, m_nProperty);		break;
		case RACE_PROPERTY::SOLOING:			SetAttributes(is, RACE_SOLOING, m_nProperty);		break;
		case RACE_PROPERTY::WARLIKE:			SetAttributes(is, RACE_WARLIKE, m_nProperty);		break;
	}
}

/// Funktion zum Erfragen der Rassenspezialeigenschaften
/// @param ability Rassenspezialeigenschaft
/// @return <code>true</code>, wenn die Rasse die Spezialeigenschaft besitzt, sonst <code>false</code>
bool CRace::HasSpecialAbility(int ability) const
{
	return (m_nSpecialAbility & ability) > 0;
}

/// Funktion zum Setzen von Spezialeigenschaften der Rasse.
/// @param ability Spezialeigenschaft
/// @param is <code>true</code> oder <code>false</code>
void CRace::SetSpecialAbility(int ability, bool is)
{
	if (ability == 0 && is)
		m_nSpecialAbility = 0;
	else
		SetAttributes(is, ability, m_nSpecialAbility);
}

/// Funktion setzt die neue Beziehung zur Rasse.
/// @param sRaceID Rassen-ID zu der die Beziehung ge�ndert werden soll
/// @param nAdd Wert der zur alten Beziehung addiert werden soll
void CRace::SetRelation(const CString& sRaceID, short nAdd)
{
	short nRelation = m_mRelations[sRaceID] + nAdd;

	if (nRelation > 100)
		nRelation = 100;
	else if (nRelation < 0)
		nRelation = 0;

	m_mRelations[sRaceID] = (BYTE)nRelation;

	if (nRelation == 0)
		m_mRelations.erase(sRaceID);
}

/// Funktion legt den diplomatischen Status zu einer anderes Rasse fest.
/// @param sOtherRace andere Rasse
/// @param nNewAgreement neuer Vertrag
void CRace::SetAgreement(const CString& sOtherRace, DIPLOMATIC_AGREEMENT::Typ nNewAgreement)
{
	// wenn kein Vertrag besteht, so kann der Eintrag auch aus der Map entfernt werden
	if (nNewAgreement == DIPLOMATIC_AGREEMENT::NONE)
		m_mAgreement.erase(sOtherRace);
	else
		m_mAgreement[sOtherRace] = nNewAgreement;
}

/// Funktion l�sst die Diplomatie-KI der Rasse Angebote an andere Rassen erstellen.
void CRace::MakeOffersAI(void)
{
	CBotEDoc* pDoc = resources::pDoc;
	AssertBotE(pDoc);

	// Alle zuletzt gemachten Angebote die �lter als 2 Runden sind l�schen
	set<CString> sRemovingOffers;
	for (map<CString, CDiplomacyInfo>::const_iterator it = m_mLastOffers.begin(); it != m_mLastOffers.end(); ++it)
		if (it->second.m_nSendRound + 2 < pDoc->GetCurrentRound())
			sRemovingOffers.insert(it->first);
	for (set<CString>::const_iterator it = sRemovingOffers.begin(); it != sRemovingOffers.end(); ++it)
		m_mLastOffers.erase(*it);

	// wenn es sich um eine Majorrace handelt, dann die Lieblingsminorrace berechnen
	if (this->IsMajor())
	{
		// bei einem menschlichen Spieler wird die KI nicht ausgef�hrt
		if (dynamic_cast<CMajor*>(this)->AHumanPlays())
			return;
		boost::dynamic_pointer_cast<CMajorAI>(m_pDiplomacyAI)->CalcFavoriteMinors();
	}

	// Angebote machen
	const CRaceController& race_ctrl = *pDoc->GetRaceCtrl();

	for (CRaceController::const_iterator it = race_ctrl.begin(); it != race_ctrl.end(); ++it)
		if (m_sID != it->first)
		{
			// Minorangebot zu anderer Minor geht nicht!
			if (IsMinor() && it->second->IsMinor())
				continue;

			// Wenn an die Rasse in den letzen zwei Runden schon ein Angebot
			// gemacht wurde, dann nicht gleich wieder eins schicken
			if (GetLastOffer(it->first))
				continue;

			// TODO: Wird folgender Code noch ben�tigt, wenn obige Funktion nun verwendet wird?
			// Nur wenn wir noch keine Angebote an diese Rasse gemacht haben
			/*
			bool bOffer = false;
			for (UINT i = 0; i < m_vDiplomacyNewsOut.size(); i++)
			{
				if (m_vDiplomacyNewsOut[i].m_nFlag == DIPLOMACY_OFFER && m_vDiplomacyNewsOut[i].m_sToRace == it->first && m_vDiplomacyNewsOut[i].m_sFromRace == m_sID)
				{
					bOffer = true;
					break;
				}
			}

			if (bOffer)
				continue;
			*/

			// Angebot erstellen
			CDiplomacyInfo info;
			CString sID = it->first;
			if (m_pDiplomacyAI->MakeOffer(sID, info))
			{
				this->GetOutgoingDiplomacyNews()->push_back(info);
				// Angebot die n�chsten 2 Runden lang merken. Wenn wir von anderen Rassen ein Angebot bekommen
				// so nehmen wir es an, wenn es kleiner gleich unserem Angebot aus den letzten beiden Runden ist.
				if (info.m_nFlag == DIPLOMACY_OFFER && info.m_nType > DIPLOMATIC_AGREEMENT::NONE)
					m_mLastOffers[sID] = info;
			}
		}
}

/// Funktion l�sst die KI auf diplomatische Angebote reagieren.
/// @param pOffer diplomatisches Angebot
void CRace::ReactOnOfferAI(CDiplomacyInfo* pOffer)
{
	// auf Angebot reagieren
	if (pOffer->m_nFlag == DIPLOMACY_OFFER)
	{
		ANSWER_STATUS::Typ nResult = m_pDiplomacyAI->ReactOnOffer(*pOffer);
		pOffer->m_nAnswerStatus = nResult;
	}
	else
		AfxMessageBox("CRace::ReactOnOfferAI(): Could not react on non diplomacy offers!");
}

void CRace::ResetDiplomacyAI()
{
	// Diplomatie-KI nullen
	if(m_pDiplomacyAI)
		m_pDiplomacyAI->Reset();
	m_pDiplomacyAI.reset();
}

/// Funktion gibt zur�ck, ob eine andere Rasse bekannt ist.
/// @param sRaceID ID der anderen Rasse
/// @return <code>true</code> wenn Kontakt zu der anderen Rasse besteht, ansonsten <code>false</code>
bool CRace::IsRaceContacted(const CString& sRaceID) const
{
	if (m_vInContact.find(sRaceID) != m_vInContact.end())
		return true;

	return false;
}

/// Funktion legt fest, ob die Rasse eine andere Rasse kennt.
/// @param sRace andere Rasse
/// @param bKnown <code>TRUE</code> wenn sie sie kennenlernt, ansonsten <code>FALSE</code>
void CRace::SetIsRaceContacted(const CString& sRace, bool bKnown)
{
	// nach der neuen Rasse im Vektor suchen
	if (m_vInContact.find(sRace) != m_vInContact.end())
	{
		// soll sie kennengelernt werden, ist aber schon im Feld, dann aus der Funktion springen
		if (bKnown)
			return;
		// ansonsten aus dem Vektor l�schen
		else
		{
			m_vInContact.erase(sRace);
			return;
		}
	}

	// soll die Rasse hinzugef�gt werden, ist aber noch nicht im Vektor. Dann einfach anh�ngen
	if (bKnown)
		m_vInContact.insert(sRace);
}

std::vector<CString> CRace::GetPropertiesAsStrings() const
{
	std::vector<CString> sProperties;
	if (IsRaceProperty(RACE_PROPERTY::FINANCIAL))
		sProperties.push_back(CLoc::GetString("FINANCIAL"));
	if (IsRaceProperty(RACE_PROPERTY::WARLIKE))
		sProperties.push_back(CLoc::GetString("WARLIKE"));
	if (IsRaceProperty(RACE_PROPERTY::AGRARIAN))
		sProperties.push_back(CLoc::GetString("AGRARIAN"));
	if (IsRaceProperty(RACE_PROPERTY::INDUSTRIAL))
		sProperties.push_back(CLoc::GetString("INDUSTRIAL"));
	if (IsRaceProperty(RACE_PROPERTY::SECRET))
		sProperties.push_back(CLoc::GetString("SECRET"));
	if (IsRaceProperty(RACE_PROPERTY::SCIENTIFIC))
		sProperties.push_back(CLoc::GetString("SCIENTIFIC"));
	if (IsRaceProperty(RACE_PROPERTY::PRODUCER))
		sProperties.push_back(CLoc::GetString("PRODUCER"));
	if (IsRaceProperty(RACE_PROPERTY::PACIFIST))
		sProperties.push_back(CLoc::GetString("PACIFIST"));
	if (IsRaceProperty(RACE_PROPERTY::SNEAKY))
		sProperties.push_back(CLoc::GetString("SNEAKY"));
	if (IsRaceProperty(RACE_PROPERTY::SOLOING))
		sProperties.push_back(CLoc::GetString("SOLOING"));
	if (IsRaceProperty(RACE_PROPERTY::HOSTILE))
		sProperties.push_back(CLoc::GetString("HOSTILE"));
	if (sProperties.empty())
		sProperties.push_back(CLoc::GetString("NONE"));

	return sProperties;
}

CString CRace::GetPropertiesAsString() const
{
	const std::vector<CString>& properties = GetPropertiesAsStrings();
	CString result;
	for(std::vector<CString>::const_iterator i = properties.begin();;)
	{
		result += *i;
		++i;
		if(i == properties.end())
			break;
		result += ", ";
	}
	return result;
}

/// Funktion erstellt eine Tooltipinfo der Rasse.
/// @return	der erstellte Tooltip-Text
CString CRace::GetTooltip(void) const
{
	CString sName = m_sName;
	sName = CHTMLStringBuilder::GetHTMLColor(sName);
	sName = CHTMLStringBuilder::GetHTMLHeader(sName, _T("h3"));
	sName += CHTMLStringBuilder::GetHTMLStringNewLine();
	sName += CHTMLStringBuilder::GetHTMLStringNewLine();

	// Eigenschaften anzeigen
	const std::vector<CString>& sProperties = GetPropertiesAsStrings();

	CString sProb = CLoc::GetString("PROPERTIES");
	sProb = CHTMLStringBuilder::GetHTMLColor(sProb, _T("silver"));
	sProb = CHTMLStringBuilder::GetHTMLHeader(sProb, _T("h4"));
	sProb += CHTMLStringBuilder::GetHTMLStringNewLine();
	sProb += CHTMLStringBuilder::GetHTMLStringHorzLine();
	sProb += CHTMLStringBuilder::GetHTMLStringNewLine();

	CString s = "";
	for (UINT i = 0; i < sProperties.size(); i++)
	{
		s += sProperties[i];
		s += CHTMLStringBuilder::GetHTMLStringNewLine();
	}
	s = CHTMLStringBuilder::GetHTMLColor(s);
	s = CHTMLStringBuilder::GetHTMLHeader(s, _T("h5"));
	sProb += s;

	return CHTMLStringBuilder::GetHTMLCenter(sName + sProb);
}

bool CRace::CanBeContactedBy(const CString& sRaceID) const {
	return !IsRaceContacted(sRaceID) && !HasSpecialAbility(SPECIAL_NO_DIPLOMACY);
}

void CRace::Contact(const CRace& Race, const CSector& /*p*/) {
	const CString& sContactedRaceID = Race.GetRaceID();
	AssertBotE(!IsRaceContacted(sContactedRaceID));
	SetIsRaceContacted(sContactedRaceID, true);
}

void CRace::LostShipToAnomaly(const CShips& /*ship*/, const CString& /*anomaly*/)
{
	//TODO: If a minor is forced to retreat, and happens to retreat to an anomaly,
	//its ships are slowly destructed since it doesn't move.
	//Forbidding to retreat to anomalies is no good idea, as it would cause probs in case that
	//all sectors around the current one are anomalies.
	//So this should probably best be fixed by making minors able to move their ships
	//(which is on the TODO list anyway).
}
