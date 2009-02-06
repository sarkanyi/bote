// Ship.cpp: Implementierung der Klasse CShip.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "stdafx.h"
#include "Ship.h"
#include "Sector.h"
#include "Fleet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL (CShip, CObject, 1)
//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CShip::CShip()
{
	m_TargetKO[0] = CPoint(-1,-1);
	m_iCrewExperiance = 0;
	m_nTerraformingPlanet = -1;
	m_bIsFlagShip = FALSE;
	m_bySpecial[0] = m_bySpecial[1] = NONE;
	m_Fleet = NULL;
	for (int i = TITAN; i <= DILITHIUM; i++)
		m_iLoadedResources[i] = 0;
	m_bCloakOn = FALSE;
}

CShip::~CShip()
{
	for (int i = 0; i < m_TorpedoWeapons.GetSize(); )
		m_TorpedoWeapons.RemoveAt(i);
	for (int i = 0; i < m_BeamWeapons.GetSize(); )
		m_BeamWeapons.RemoveAt(i);
	for (int i = 0; i < m_Path.GetSize(); )
		m_Path.RemoveAt(i);
	for (int i = 0; i < m_Troops.GetSize(); )
		m_Troops.RemoveAt(i);	
	m_TorpedoWeapons.RemoveAll();
	m_BeamWeapons.RemoveAll();
	m_Path.RemoveAll();
	m_Troops.RemoveAll();
	if (m_Fleet)
	{
		m_Fleet->DeleteFleet();
		delete m_Fleet;
	}
	m_Fleet = NULL;
}

//////////////////////////////////////////////////////////////////////
// Kopierkonstruktor
//////////////////////////////////////////////////////////////////////
CShip::CShip(const CShip & rhs)
{
	m_Hull = rhs.m_Hull;
	m_Shield = rhs.m_Shield;
	for (int i = 0; i < rhs.m_TorpedoWeapons.GetSize(); i++)
		m_TorpedoWeapons.Add(rhs.m_TorpedoWeapons.GetAt(i));
	for (int i = 0; i < rhs.m_BeamWeapons.GetSize(); i++)
		m_BeamWeapons.Add(rhs.m_BeamWeapons.GetAt(i));
	for (int i = 0; i < rhs.m_Path.GetSize(); i++)
		m_Path.Add(rhs.m_Path.GetAt(i));
	for (int i = 0; i < rhs.m_Troops.GetSize(); i++)
		m_Troops.Add(rhs.m_Troops.GetAt(i));
	// Zeiger auf Fleet speziell behandeln	
	if (rhs.m_Fleet)
	{
		m_Fleet = new CFleet();
		*m_Fleet = *(rhs.m_Fleet);
	}
	else
		m_Fleet = NULL;
	
	//m_Fleet = rhs.&m_Fleet;
	m_iID = rhs.m_iID;
	m_KO = rhs.m_KO;
	for (int i=0;i<4;i++)
		m_TargetKO[i] = rhs.m_TargetKO[i];
	m_iOwnerOfShip = rhs.m_iOwnerOfShip;
	m_iMaintenanceCosts = rhs.m_iMaintenanceCosts;
	m_iShipType = rhs.m_iShipType;
	m_byShipSize = rhs.m_byShipSize;
	m_byManeuverability = rhs.m_byManeuverability;
	m_iSpeed = rhs.m_iSpeed;
	m_iRange = rhs.m_iRange;
	m_iScanPower = rhs.m_iScanPower;
	m_iScanRange = rhs.m_iScanRange;
	m_iCrewExperiance = rhs.m_iCrewExperiance;
	m_iStealthPower = rhs.m_iStealthPower;
	m_bCloakOn = rhs.m_bCloakOn;
	m_iStorageRoom = rhs.m_iStorageRoom;
	for (int i = TITAN; i <= DILITHIUM; i++)
		m_iLoadedResources[i] = rhs.m_iLoadedResources[i];
	m_iColonizePoints = rhs.m_iColonizePoints;
	m_iStationBuildPoints = rhs.m_iStationBuildPoints;
	m_iCurrentOrder = rhs.m_iCurrentOrder;
	m_nTerraformingPlanet = rhs.m_nTerraformingPlanet;
	m_strShipName = rhs.m_strShipName;
	m_strShipDescription = rhs.m_strShipDescription;
	m_strShipClass = rhs.m_strShipClass;
	m_bIsFlagShip = rhs.m_bIsFlagShip;
	m_bySpecial[0] = rhs.m_bySpecial[0];
	m_bySpecial[1] = rhs.m_bySpecial[1];
}

//////////////////////////////////////////////////////////////////////
// Zuweisungsoperator
//////////////////////////////////////////////////////////////////////
CShip & CShip::operator=(const CShip & rhs)
{
	if (this == &rhs)
		return *this;
	m_Hull = rhs.m_Hull;
	m_Shield = rhs.m_Shield;
	for (int i = 0; i < rhs.m_TorpedoWeapons.GetSize(); i++)
		m_TorpedoWeapons.Add(rhs.m_TorpedoWeapons.GetAt(i));
	for (int i = 0; i < rhs.m_BeamWeapons.GetSize(); i++)
		m_BeamWeapons.Add(rhs.m_BeamWeapons.GetAt(i));
	for (int i = 0; i < rhs.m_Path.GetSize(); i++)
		m_Path.Add(rhs.m_Path.GetAt(i));
	for (int i = 0; i < rhs.m_Troops.GetSize(); i++)
		m_Troops.Add(rhs.m_Troops.GetAt(i));
	// Zeiger auf Fleet speziell behandeln
	if (rhs.m_Fleet)
	{
		TRACE("Achtung, wenn im Zuweisungsoperator der CShip-Klasse ein Shiffsobjekt eine Flotte beitzt, dann kommt es manchmal beim Beenden des Programmes aus mir unverst�ndlichen Gr�nden zu Memory-Leaks!\n");
		m_Fleet = new CFleet();
		*m_Fleet = *(rhs.m_Fleet);
	}
	else
		m_Fleet = NULL;
	
	m_iID = rhs.m_iID;
	m_KO = rhs.m_KO;
	for (int i=0;i<4;i++)
		m_TargetKO[i] = rhs.m_TargetKO[i];
	m_iOwnerOfShip = rhs.m_iOwnerOfShip;
	m_iMaintenanceCosts = rhs.m_iMaintenanceCosts;
	m_iShipType = rhs.m_iShipType;
	m_byShipSize = rhs.m_byShipSize;
	m_byManeuverability = rhs.m_byManeuverability;
	m_iSpeed = rhs.m_iSpeed;
	m_iRange = rhs.m_iRange;
	m_iScanPower = rhs.m_iScanPower;
	m_iScanRange = rhs.m_iScanRange;
	m_iCrewExperiance = rhs.m_iCrewExperiance;
	m_iStealthPower = rhs.m_iStealthPower;
	m_bCloakOn = rhs.m_bCloakOn;
	m_iStorageRoom = rhs.m_iStorageRoom;
	for (int i = TITAN; i <= DILITHIUM; i++)
		m_iLoadedResources[i] = rhs.m_iLoadedResources[i];
	m_iColonizePoints = rhs.m_iColonizePoints;
	m_iStationBuildPoints = rhs.m_iStationBuildPoints;
	m_iCurrentOrder = rhs.m_iCurrentOrder;
	m_nTerraformingPlanet = rhs.m_nTerraformingPlanet;
	m_strShipName = rhs.m_strShipName;
	m_strShipDescription = rhs.m_strShipDescription;
	m_strShipClass = rhs.m_strShipClass;
	m_bIsFlagShip = rhs.m_bIsFlagShip;
	m_bySpecial[0] = rhs.m_bySpecial[0];
	m_bySpecial[1] = rhs.m_bySpecial[1];
	return *this;
}

//////////////////////////////////////////////////////////////////////
// Serialisierungsfunktion
//////////////////////////////////////////////////////////////////////
void CShip::Serialize(CArchive &ar)
{
	CObject::Serialize(ar);
	m_Hull.Serialize(ar);
	m_Shield.Serialize(ar);
	
	// wenn gespeichert wird
	if (ar.IsStoring())
	{
		ar << m_Fleet;
		ar << m_iID;
		ar << m_KO;
		for (int i = 0; i < 4; i++)
			ar << m_TargetKO[i];
		ar << m_iOwnerOfShip;
		ar << m_iMaintenanceCosts;
		ar << m_iShipType;
		ar << m_byShipSize;
		ar << m_byManeuverability;
		ar << m_iSpeed;
		ar << m_iRange;
		ar << m_iScanPower;
		ar << m_iScanRange;
		ar << m_iCrewExperiance;
		ar << m_iStealthPower;
		ar << m_bCloakOn;
		ar << m_iStorageRoom;
		for (int i = TITAN; i <= DILITHIUM; i++)
			ar << m_iLoadedResources[i];
		ar << m_iColonizePoints;
		ar << m_iStationBuildPoints;
		ar << m_iCurrentOrder;
		ar << m_bySpecial[0];
		ar << m_bySpecial[1];
		ar << m_nTerraformingPlanet;
		ar << m_strShipName;
		ar << m_strShipDescription;
		ar << m_strShipClass;
		ar << m_bIsFlagShip;
		ar << m_TorpedoWeapons.GetSize();
		for (int i = 0; i < m_TorpedoWeapons.GetSize(); i++)
			m_TorpedoWeapons.GetAt(i).Serialize(ar);
		ar << m_BeamWeapons.GetSize();
		for (int i = 0; i < m_BeamWeapons.GetSize(); i++)
			m_BeamWeapons.GetAt(i).Serialize(ar);
		m_Path.Serialize(ar);
		ar << m_Troops.GetSize();
		for (int i = 0; i < m_Troops.GetSize(); i++)
			m_Troops.GetAt(i).Serialize(ar);
	}
	// wenn geladen wird
	if (ar.IsLoading())
	{
		int number = 0;
		ar >> m_Fleet;
		ar >> m_iID;
		ar >> m_KO;
		for (int i = 0; i < 4; i++)
			ar >> m_TargetKO[i];
		ar >> m_iOwnerOfShip;
		ar >> m_iMaintenanceCosts;
		ar >> m_iShipType;
		ar >> m_byShipSize;
		ar >> m_byManeuverability;
		ar >> m_iSpeed;
		ar >> m_iRange;
		ar >> m_iScanPower;
		ar >> m_iScanRange;
		ar >> m_iCrewExperiance;
		ar >> m_iStealthPower;
		ar >> m_bCloakOn;
		ar >> m_iStorageRoom;
		for (int i = TITAN; i <= DILITHIUM; i++)
			ar >> m_iLoadedResources[i];
		ar >> m_iColonizePoints;
		ar >> m_iStationBuildPoints;
		ar >> m_iCurrentOrder;
		ar >> m_bySpecial[0];
		ar >> m_bySpecial[1];
		ar >> m_nTerraformingPlanet;
		ar >> m_strShipName;
		ar >> m_strShipDescription;
		ar >> m_strShipClass;
		ar >> m_bIsFlagShip;
		ar >> number;
		m_TorpedoWeapons.RemoveAll();
		m_TorpedoWeapons.SetSize(number);
		for (int i = 0; i < number; i++)
			m_TorpedoWeapons.GetAt(i).Serialize(ar);
		ar >> number;
		m_BeamWeapons.RemoveAll();
		m_BeamWeapons.SetSize(number);
		for (int i = 0; i < number; i++)
			m_BeamWeapons.GetAt(i).Serialize(ar);		
		m_Path.Serialize(ar);
		ar >> number;
		m_Troops.RemoveAll();
		m_Troops.SetSize(number);
		for (int i = 0; i < number; i++)
			m_Troops.GetAt(i).Serialize(ar);
	}
}

//////////////////////////////////////////////////////////////////////
// Zugriffsfunktion
//////////////////////////////////////////////////////////////////////
const CPoint CShip::GetTargetKO()
{
	return m_TargetKO[0];
}

// Funktion gibt den Schiffstyp als char* zur�ck
CString CShip::GetShipTypeAsString(BOOL plural)
{
	/*	#define TRANSPORTER			0
		#define COLONYSHIP            1
		#define PROBE				2
		#define SCOUT				3
		#define FIGHTER				4	// J�ger
		#define FRIGATE				5
		#define DESTROYER			6
		#define CRUISER				7
		#define HEAVY_DESTROYER     8
		#define HEAVY_CRUISER       9
		#define BATTLESHIP			10
		#define FLAGSHIP			11
		#define OUTPOST				12
		#define STARBASE            13
		#define ALIEN				14 */
	// Oben im Beschreibungsrechteck den Namen des Projektes hinschreiben
	CString shipType;
	if (plural == FALSE)
		switch (m_iShipType)
		{
		case TRANSPORTER: shipType = CResourceManager::GetString("TRANSPORTER"); break;
		case COLONYSHIP: shipType = CResourceManager::GetString("COLONIZESHIP"); break;
		case PROBE: shipType = CResourceManager::GetString("PROBE"); break;
		case SCOUT: shipType = CResourceManager::GetString("SCOUT"); break;
		case FIGHTER: shipType = CResourceManager::GetString("FIGHTER"); break;
		case FRIGATE: shipType = CResourceManager::GetString("FRIGATE"); break;
		case DESTROYER: shipType = CResourceManager::GetString("DESTROYER"); break;
		case CRUISER: shipType = CResourceManager::GetString("CRUISER"); break;
		case HEAVY_DESTROYER: shipType = CResourceManager::GetString("HEAVY_DESTROYER"); break;
		case HEAVY_CRUISER: shipType = CResourceManager::GetString("HEAVY_CRUISER"); break;
		case BATTLESHIP: shipType = CResourceManager::GetString("BATTLESHIP"); break;
		case FLAGSHIP: shipType = CResourceManager::GetString("FLAGSHIP"); break;
		case OUTPOST: shipType = CResourceManager::GetString("OUTPOST"); break;
		case STARBASE: shipType = CResourceManager::GetString("STARBASE"); break;
		case ALIEN: shipType = CResourceManager::GetString("ALIEN"); break;
		}
	else
		switch (m_iShipType)
		{
		case TRANSPORTER: shipType = CResourceManager::GetString("TRANSPORTERS"); break;
		case COLONYSHIP: shipType = CResourceManager::GetString("COLONIZESHIPS"); break;
		case PROBE: shipType = CResourceManager::GetString("PROBES"); break;
		case SCOUT: shipType = CResourceManager::GetString("SCOUTS"); break;
		case FIGHTER: shipType = CResourceManager::GetString("FIGHTERS"); break;
		case FRIGATE: shipType = CResourceManager::GetString("FRIGATES"); break;
		case DESTROYER: shipType = CResourceManager::GetString("DESTROYERS"); break;
		case CRUISER: shipType = CResourceManager::GetString("CRUISERS"); break;
		case HEAVY_DESTROYER: shipType = CResourceManager::GetString("HEAVY_DESTROYERS"); break;
		case HEAVY_CRUISER: shipType = CResourceManager::GetString("HEAVY_CRUISERS"); break;
		case BATTLESHIP: shipType = CResourceManager::GetString("BATTLESHIPS"); break;
		case FLAGSHIP: shipType = CResourceManager::GetString("FLAGSHIPS"); break;
		case OUTPOST: shipType = CResourceManager::GetString("OUTPOSTS"); break;
		case STARBASE: shipType = CResourceManager::GetString("STARBASES"); break;
		case ALIEN: shipType = CResourceManager::GetString("ALIENS"); break;
		}
	return shipType;
}

// Funktion gibt den aktuellen Schiffsauftrag als char* zur�ck
CString CShip::GetCurrentOrderAsString()
{
	/*
	#define AVOID               0
	#define ATTACK              1
	#define CLOAK               2
	#define ATTACK_SYSTEM       3
	#define RAID_SYSTEM         4
	#define BLOCKADE_SYSTEM		5
	#define DESTROY_SHIP        6
	#define COLONIZE            7
	#define TERRAFORM           8
	#define BUILD_OUTPOST       9
	#define BUILD_STARBASE		10
	#define ASSIGN_FLAGSHIP     11
	#define CREATE_FLEET        12
	#define TRANSPORT			13
	#define FOLLOW_SHIP			14
	#define TRAIN_SHIP			15
	*/
	CString order;
	switch (m_iCurrentOrder)
	{
	case AVOID: order = CResourceManager::GetString("AVOID_ORDER"); break;
	case ATTACK: order = CResourceManager::GetString("ATTACK_ORDER"); break;
	case CLOAK: 
		if (m_bCloakOn)
			order = CResourceManager::GetString("DECLOAK_ORDER");
		else
			order = CResourceManager::GetString("CLOAK_ORDER"); break;
	case ATTACK_SYSTEM: order = CResourceManager::GetString("ATTACK_SYSTEM_ORDER"); break;
	case RAID_SYSTEM: order = CResourceManager::GetString("RAID_SYSTEM_ORDER"); break;
	case BLOCKADE_SYSTEM: order = CResourceManager::GetString("BLOCKADE_SYSTEM_ORDER"); break;
	case DESTROY_SHIP: order = CResourceManager::GetString("DESTROY_SHIP_ORDER"); break;
	case COLONIZE: order = CResourceManager::GetString("COLONIZE_ORDER"); break;
	case TERRAFORM: order = CResourceManager::GetString("TERRAFORM_ORDER"); break;
	case BUILD_OUTPOST: order = CResourceManager::GetString("BUILD_OUTPOST_ORDER"); break;
	case BUILD_STARBASE: order = CResourceManager::GetString("BUILD_STARBASE_ORDER"); break;
	case ASSIGN_FLAGSHIP: order = CResourceManager::GetString("ASSIGN_FLAGSHIP_ORDER"); break;
	case CREATE_FLEET: order = CResourceManager::GetString("CREATE_FLEET_ORDER"); break;
	case TRANSPORT: order = CResourceManager::GetString("TRANSPORT_ORDER"); break;
	case FOLLOW_SHIP: order = CResourceManager::GetString("FOLLOW_SHIP_ORDER"); break;
	case TRAIN_SHIP: order = CResourceManager::GetString("TRAIN_SHIP_ORDER"); break;
	default: order = "nothing"; break;
	}
	return order;
}

void CShip::CreateFleet()
{
	if (!m_Fleet)
		m_Fleet = new CFleet();
}

void CShip::CheckFleet()
{
	if (m_Fleet)
		if (m_Fleet->GetFleetSize() == 0)
		{
			m_Fleet->DeleteFleet();
			delete m_Fleet;
			m_Fleet = NULL;
		}
}

void CShip::DeleteFleet()
{
	if (m_Fleet)
	{
		m_Fleet->DeleteFleet();
		delete m_Fleet;
		m_Fleet = NULL;
	}
}

BOOLEAN CShip::HasSpecial(BYTE ability) const
{
	if (m_bySpecial[0] == ability || m_bySpecial[1] == ability)
		return TRUE;
	else
		return FALSE;
}

/// Funktion gibt die gesamte Offensivpower des Schiffes zur�ck, welches es in 100s anrichten w�rde.
/// Dieser Wert hat keinen direkten Kampfeinfluss, er ist nur zum Vergleich heranzuziehen.
UINT CShip::GetCompleteOffensivePower()
{
	UINT beamDmg	 = 0;
	UINT torpedoDmg  = 0;
	for (int i = 0; i < GetBeamWeapons()->GetSize(); i++)
	{
		short counter = 0;
		for (int j = 0; j < 100; j++)
		{
			if (counter == 0)
				counter = GetBeamWeapons()->GetAt(i).GetBeamLenght() 
							+ GetBeamWeapons()->GetAt(i).GetRechargeTime();
			if (counter > GetBeamWeapons()->GetAt(i).GetRechargeTime())
			{
				UINT tempBeamDmg = (UINT)GetBeamWeapons()->GetAt(i).GetBeamPower()
							* GetBeamWeapons()->GetAt(i).GetBeamNumber()
							* GetBeamWeapons()->GetAt(i).GetShootNumber();
				// besondere Beamf�higkeiten erh�hen den BeamDmg um einen selbst gew�hlten Mulitplikator
				// der dadurch erhaltende Schaden entspricht nicht dem wirklichen Schaden!
				if (GetBeamWeapons()->GetAt(i).GetPiercing())
					tempBeamDmg = (UINT)(tempBeamDmg * 1.5);
				if (GetBeamWeapons()->GetAt(i).GetModulating())
					tempBeamDmg *= 3;
				beamDmg += tempBeamDmg;
			}			
			counter--;
		}
	}
	beamDmg /= 3;
	for (int i = 0; i < GetTorpedoWeapons()->GetSize(); i++)
	{
		UINT tempTorpedoDmg = (UINT)(GetTorpedoWeapons()->GetAt(i).GetTorpedoPower() *
									GetTorpedoWeapons()->GetAt(i).GetNumber() * 100 *
									GetTorpedoWeapons()->GetAt(i).GetNumberOfTupes() /
									GetTorpedoWeapons()->GetAt(i).GetTupeFirerate());
		// besondere Torpedof�higkeiten erh�hen den Torpedoschaden um einen selbst gew�hlten Mulitplikator
		// der dadurch erhaltende Schaden entspricht nicht dem wirklichen Schaden!
		BYTE type = GetTorpedoWeapons()->GetAt(i).GetTorpedoType();
		if (CTorpedoInfo::GetPenetrating(type))
			tempTorpedoDmg = (UINT)(tempTorpedoDmg * 1.5);
		if (CTorpedoInfo::GetIgnoreAllShields(type))
			tempTorpedoDmg *= 3;
		if (CTorpedoInfo::GetCollapseShields(type))
			tempTorpedoDmg = (UINT)(tempTorpedoDmg * 1.25);
		if (CTorpedoInfo::GetDoubleShieldDmg(type))
			tempTorpedoDmg = (UINT)(tempTorpedoDmg * 1.5);
		if (CTorpedoInfo::GetDoubleHullDmg(type))
			tempTorpedoDmg = (UINT)(tempTorpedoDmg * 2);
		if (CTorpedoInfo::GetReduceManeuver(type))
			tempTorpedoDmg = (UINT)(tempTorpedoDmg * 1.1);
		torpedoDmg += tempTorpedoDmg;
	}
	return (beamDmg + torpedoDmg);
}

/// Funktion gibt die gesamte Defensivst�rke des Schiffes zur�ck. Dabei wird die maximale H�lle, die maximalen
/// Schilde und die Schildaufladezeit beachtet. Dieser Wert hat keinen direkten Kampfeinfluss, er ist nur zum
/// Vergleich heranzuziehen.
UINT CShip::GetCompleteDefensivePower()
{
	UINT def = GetHull()->GetMaxHull() + GetShield()->GetMaxShield()
		+ (GetShield()->GetMaxShield() / 300 + 2 * GetShield()->GetShieldType()) * 100;
	if (GetHull()->GetAblative())
		def = (UINT)(def * 1.1);
	if (GetHull()->GetPolarisation())
		def = (UINT)(def * 1.1);

	return def;
}

/// Funktion gibt den schon benutzten Lagerraum im Schiff zur�ck.
USHORT CShip::GetUsedStorageRoom(const CArray<CTroopInfo>* troopInfo)
{
	USHORT usedStorage = 0;
	// ben�tigten Platz durch Truppen, welche schon im Schiff sind ermitteln
	for (int i = 0; i < this->GetTransportedTroops()->GetSize(); i++)
	{
		BYTE id = this->GetTransportedTroops()->GetAt(i).GetID();
		usedStorage += troopInfo->GetAt(id).GetSize();
	}		
	for (int i = TITAN; i <= DILITHIUM; i++)
	{
		// Dilithium wird im Verh�ltnis 1:250 gelagert, die anderen Ressourcen im Verh�ltnis 1:1
		if (i == DILITHIUM)
			usedStorage += this->GetLoadedResources(i) * 250;
		else
			usedStorage += this->GetLoadedResources(i);
	}
	return usedStorage;
}

/// Funktion gibt das Erfahrungslevel des Schiffes zur�ck. Damit sind nicht die genauen Erfahrungspunkte gemeint, sondern das erreichte
/// Level aufgrund der Erfahrungspunkte.
/// @return Erfahrungstufe
BYTE CShip::GetExpLevel() const
{
	USHORT exp = m_iCrewExperiance;
	// Greenhorn
	if (exp < 250)
		return 0;
	// Ang�nger
	else if (exp < 500)
		return 1;
	// Normal
	else if (exp < 1000)
		return 2;
	// Profi
	else if (exp < 2500)
		return 3;
	// Veteran
	else if (exp < 5000)
		return 4;
	// Elite
	else if (exp < 10000)
		return 5;
	// Legende
	else
		return 6;
}
