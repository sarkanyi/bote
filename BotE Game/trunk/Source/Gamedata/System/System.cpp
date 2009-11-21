// System.cpp: Implementierung der Klasse CSystem.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "System.h"
#include "Botf2Doc.h"
#include "Races\RaceController.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


IMPLEMENT_SERIAL (CSystem, CObject, 1)

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CSystem::CSystem()
{
	ResetSystem();
}

CSystem::~CSystem()
{
	m_BuildingDestroy.RemoveAll();
	m_Buildings.RemoveAll();
	m_BuildableBuildings.RemoveAll();
	m_AllwaysBuildableBuildings.RemoveAll();
	m_BuildableUpdates.RemoveAll();
	m_BuildableWithoutAssemblylistCheck.RemoveAll();
	m_BuildableShips.RemoveAll();
	m_BuildableTroops.RemoveAll();
	m_TradeRoutes.RemoveAll();
	m_ResourceRoutes.RemoveAll();
	m_Troops.RemoveAll();
}

//////////////////////////////////////////////////////////////////////
// Kopierkonstruktor
//////////////////////////////////////////////////////////////////////
CSystem::CSystem(const CSystem &rhs)
{
	m_sOwnerOfSystem = rhs.m_sOwnerOfSystem;
	m_dHabitants = rhs.m_dHabitants;
	m_iMoral = rhs.m_iMoral;
	m_byBlockade = rhs.m_byBlockade;
	m_iFoodBuildings = rhs.m_iFoodBuildings;
	m_iIndustryBuildings = rhs.m_iIndustryBuildings;
	m_iEnergyBuildings = rhs.m_iEnergyBuildings;
	m_iSecurityBuildings = rhs.m_iSecurityBuildings;
	m_iResearchBuildings = rhs.m_iResearchBuildings;
	m_iTitanMines = rhs.m_iTitanMines;
	m_iDeuteriumMines = rhs.m_iDeuteriumMines;
	m_iDuraniumMines = rhs.m_iDuraniumMines;
	m_iIridiumMines = rhs.m_iIridiumMines;
	m_iCrystalMines = rhs.m_iCrystalMines;
	m_iFoodStore = rhs.m_iFoodStore;
	m_iTitanStore = rhs.m_iTitanStore;
	m_iDeuteriumStore = rhs.m_iDeuteriumStore;
	m_iDuraniumStore = rhs.m_iDuraniumStore;
	m_iCrystalStore = rhs.m_iCrystalStore;
	m_iIridiumStore = rhs.m_iIridiumStore;
	m_iDilithiumStore = rhs.m_iDilithiumStore;
	for (int i = 0; i < m_BuildingDestroy.GetSize(); i++)
		m_BuildingDestroy.ElementAt(i) = rhs.m_BuildingDestroy.GetAt(i);
	m_Workers = rhs.m_Workers;
	m_AssemblyList = rhs.m_AssemblyList;
	m_Production = rhs.m_Production;
	for (int i = 0; i < rhs.m_Buildings.GetSize(); i++)
		m_Buildings.Add(rhs.m_Buildings.GetAt(i));
	for (int i = 0; i < m_BuildableBuildings.GetSize(); i++)
		m_BuildableBuildings.ElementAt(i) = rhs.m_BuildableBuildings.GetAt(i);
	for (int i = 0; i < m_AllwaysBuildableBuildings.GetSize(); i++)
		m_AllwaysBuildableBuildings.ElementAt(i) = rhs.m_AllwaysBuildableBuildings.GetAt(i);
	for (int i = 0; i < m_BuildableUpdates.GetSize(); i++)
		m_BuildableUpdates.ElementAt(i) = rhs.m_BuildableUpdates.GetAt(i);
	for (int i = 0; i < m_BuildableWithoutAssemblylistCheck.GetSize(); i++)
		m_BuildableWithoutAssemblylistCheck.ElementAt(i) = rhs.m_BuildableWithoutAssemblylistCheck.GetAt(i);
	for (int i = 0; i < m_BuildableShips.GetSize(); i++)
		m_BuildableShips.ElementAt(i) = rhs.m_BuildableShips.GetAt(i);
	for (int i = 0; i < m_BuildableTroops.GetSize(); i++)
		m_BuildableTroops.ElementAt(i) = rhs.m_BuildableTroops.GetAt(i);
	for (int i = 0; i < rhs.m_TradeRoutes.GetSize(); i++)
		m_TradeRoutes.Add(rhs.m_TradeRoutes.GetAt(i));
	m_byMaxTradeRoutesFromHab = rhs.m_byMaxTradeRoutesFromHab;
	for (int i = 0; i < rhs.m_ResourceRoutes.GetSize(); i++)
		m_ResourceRoutes.Add(rhs.m_ResourceRoutes.GetAt(i));
	for (int i = 0; i < rhs.m_Troops.GetSize(); i++)
		m_Troops.Add(rhs.m_Troops.GetAt(i));
	m_bAutoBuild = rhs.m_bAutoBuild;
}

//////////////////////////////////////////////////////////////////////
// Zuweisungsoperator
//////////////////////////////////////////////////////////////////////
CSystem & CSystem::operator=(const CSystem & rhs)
{
	if (this == &rhs)
		return *this;
	m_sOwnerOfSystem = rhs.m_sOwnerOfSystem;
	m_dHabitants = rhs.m_dHabitants;
	m_iMoral = rhs.m_iMoral;
	m_byBlockade = rhs.m_byBlockade;
	m_iFoodBuildings = rhs.m_iFoodBuildings;
	m_iIndustryBuildings = rhs.m_iIndustryBuildings;
	m_iEnergyBuildings = rhs.m_iEnergyBuildings;
	m_iSecurityBuildings = rhs.m_iSecurityBuildings;
	m_iResearchBuildings = rhs.m_iResearchBuildings;
	m_iTitanMines = rhs.m_iTitanMines;
	m_iDeuteriumMines = rhs.m_iDeuteriumMines;
	m_iDuraniumMines = rhs.m_iDuraniumMines;
	m_iIridiumMines = rhs.m_iIridiumMines;
	m_iCrystalMines = rhs.m_iCrystalMines;
	m_iFoodStore = rhs.m_iFoodStore;
	m_iTitanStore = rhs.m_iTitanStore;
	m_iDeuteriumStore = rhs.m_iDeuteriumStore;
	m_iDuraniumStore = rhs.m_iDuraniumStore;
	m_iCrystalStore = rhs.m_iCrystalStore;
	m_iIridiumStore = rhs.m_iIridiumStore;
	m_iDilithiumStore = rhs.m_iDilithiumStore;
	for (int i = 0; i < m_BuildingDestroy.GetSize(); i++)
		m_BuildingDestroy.ElementAt(i) = rhs.m_BuildingDestroy.GetAt(i);
	m_Workers = rhs.m_Workers;
	m_AssemblyList = rhs.m_AssemblyList;
	m_Production = rhs.m_Production;
	for (int i = 0; i < rhs.m_Buildings.GetSize(); i++)
		m_Buildings.Add(rhs.m_Buildings.GetAt(i));
	for (int i = 0; i < m_BuildableBuildings.GetSize(); i++)
		m_BuildableBuildings.ElementAt(i) = rhs.m_BuildableBuildings.GetAt(i);
	for (int i = 0; i < m_AllwaysBuildableBuildings.GetSize(); i++)
		m_AllwaysBuildableBuildings.ElementAt(i) = rhs.m_AllwaysBuildableBuildings.GetAt(i);
	for (int i = 0; i < m_BuildableUpdates.GetSize(); i++)
		m_BuildableUpdates.ElementAt(i) = rhs.m_BuildableUpdates.GetAt(i);
	for (int i = 0; i < m_BuildableWithoutAssemblylistCheck.GetSize(); i++)
		m_BuildableWithoutAssemblylistCheck.ElementAt(i) = rhs.m_BuildableWithoutAssemblylistCheck.GetAt(i);
	for (int i = 0; i < m_BuildableShips.GetSize(); i++)
		m_BuildableShips.ElementAt(i) = rhs.m_BuildableShips.GetAt(i);
	for (int i = 0; i < m_BuildableTroops.GetSize(); i++)
		m_BuildableTroops.ElementAt(i) = rhs.m_BuildableTroops.GetAt(i);
	for (int i = 0; i < rhs.m_TradeRoutes.GetSize(); i++)
		m_TradeRoutes.Add(rhs.m_TradeRoutes.GetAt(i));
	m_byMaxTradeRoutesFromHab = rhs.m_byMaxTradeRoutesFromHab;
	for (int i = 0; i < rhs.m_ResourceRoutes.GetSize(); i++)
		m_ResourceRoutes.Add(rhs.m_ResourceRoutes.GetAt(i));
	for (int i = 0; i < rhs.m_Troops.GetSize(); i++)
		m_Troops.Add(rhs.m_Troops.GetAt(i));
	m_bAutoBuild = rhs.m_bAutoBuild;
	return *this;
}

///////////////////////////////////////////////////////////////////////
// Speichern / Laden
///////////////////////////////////////////////////////////////////////
void CSystem::Serialize(CArchive &ar)		
{
	CObject::Serialize(ar);
	m_AssemblyList.Serialize(ar);
	m_Production.Serialize(ar);
	m_Workers.Serialize(ar);
	// wenn gespeichert wird
	if (ar.IsStoring())
	{
		ar << m_sOwnerOfSystem;
		ar << m_dHabitants;
		ar << m_iMoral;
		ar << m_byBlockade;
		ar << m_iFoodStore;
		ar << m_iTitanStore;
		ar << m_iDeuteriumStore;
		ar << m_iDuraniumStore;
		ar << m_iCrystalStore;
		ar << m_iIridiumStore;
		ar << m_iDilithiumStore;
		ar << m_iFoodBuildings;
		ar << m_iIndustryBuildings;
		ar << m_iEnergyBuildings;	
		ar << m_iSecurityBuildings;
		ar << m_iResearchBuildings;
		ar << m_iTitanMines;
		ar << m_iDeuteriumMines;
		ar << m_iDuraniumMines;
		ar << m_iIridiumMines;
		ar << m_iCrystalMines;
		ar << m_Buildings.GetSize();
		for (int i = 0; i < m_Buildings.GetSize(); i++)
			m_Buildings.GetAt(i).Serialize(ar);
		m_BuildableBuildings.Serialize(ar);
		m_BuildableUpdates.Serialize(ar);
		m_BuildableShips.Serialize(ar);
		m_BuildableTroops.Serialize(ar);
		m_AllwaysBuildableBuildings.Serialize(ar);
		m_BuildableWithoutAssemblylistCheck.Serialize(ar);
		m_BuildingDestroy.Serialize(ar);
		ar << m_TradeRoutes.GetSize();
		for (int i = 0; i < m_TradeRoutes.GetSize(); i++)
			m_TradeRoutes.GetAt(i).Serialize(ar);
		ar << m_byMaxTradeRoutesFromHab;
		ar << m_ResourceRoutes.GetSize();
		for (int i = 0; i < m_ResourceRoutes.GetSize(); i++)
			m_ResourceRoutes.GetAt(i).Serialize(ar);
		ar << m_Troops.GetSize();
		for (int i = 0; i < m_Troops.GetSize(); i++)
			m_Troops.GetAt(i).Serialize(ar);
		ar << m_bAutoBuild;
	}
	// wenn geladen wird
	if (ar.IsLoading())
	{
		int number = 0;
		ar >> m_sOwnerOfSystem;
		ar >> m_dHabitants;
		ar >> m_iMoral;
		ar >> m_byBlockade;
		ar >> m_iFoodStore;
		ar >> m_iTitanStore;
		ar >> m_iDeuteriumStore;
		ar >> m_iDuraniumStore;
		ar >> m_iCrystalStore;
		ar >> m_iIridiumStore;
		ar >> m_iDilithiumStore;
		ar >> m_iFoodBuildings;
		ar >> m_iIndustryBuildings;
		ar >> m_iEnergyBuildings;	
		ar >> m_iSecurityBuildings;
		ar >> m_iResearchBuildings;
		ar >> m_iTitanMines;
		ar >> m_iDeuteriumMines;
		ar >> m_iDuraniumMines;
		ar >> m_iIridiumMines;
		ar >> m_iCrystalMines;
		ar >> number;
		m_Buildings.RemoveAll();
		m_Buildings.SetSize(number);
		for (int i = 0; i < number; i++)
			m_Buildings.GetAt(i).Serialize(ar);
		m_BuildableBuildings.RemoveAll();
		m_BuildableUpdates.RemoveAll();
		m_BuildableShips.RemoveAll();
		m_BuildableTroops.RemoveAll();
		m_AllwaysBuildableBuildings.RemoveAll();
		m_BuildableWithoutAssemblylistCheck.RemoveAll();
		m_BuildingDestroy.RemoveAll();

		m_BuildableBuildings.Serialize(ar);
		m_BuildableUpdates.Serialize(ar);
		m_BuildableShips.Serialize(ar);
		m_BuildableTroops.Serialize(ar);
		m_AllwaysBuildableBuildings.Serialize(ar);
		m_BuildableWithoutAssemblylistCheck.Serialize(ar);
		m_BuildingDestroy.Serialize(ar);
		ar >> number;
		m_TradeRoutes.RemoveAll();
		m_TradeRoutes.SetSize(number);
		for (int i = 0; i < number; i++)
			m_TradeRoutes.GetAt(i).Serialize(ar);
		ar >> m_byMaxTradeRoutesFromHab;
		ar >> number;
		m_ResourceRoutes.RemoveAll();
		m_ResourceRoutes.SetSize(number);
		for (int i = 0; i < number; i++)
			m_ResourceRoutes.GetAt(i).Serialize(ar);
		ar >> number;
		m_Troops.RemoveAll();
		m_Troops.SetSize(number);
		for (int i = 0; i < number; i++)
			m_Troops.GetAt(i).Serialize(ar);
		ar >> m_bAutoBuild;
	}
}

//////////////////////////////////////////////////////////////////////
// Zugriffsfunktionen
//////////////////////////////////////////////////////////////////////

// Funktion gibt die Anzahl oder die RunningNumber (ID) der Geb�ude zur�ck, welche Arbeiter ben�tigen.
// Wir �bergeben daf�r als Parameter den Typ des Geb�udes (FARM, BAUHOF usw.) und einen Modus.
// Ist der Modus NULL, dann bekommen wir die Anzahl zur�ck, ist der Modus EINS, dann die RunningNumber.
USHORT CSystem::GetNumberOfWorkbuildings(int WhatWorkbuilding, int Modus, BuildingInfoArray* buildingInfos) const
{
	// "Modus" gibt an, ob wir die Anzahl der Geb�ude oder die aktuelle RunningNumber
	// des speziellen Geb�udes zur�ckgegeben wird
	// Modus == 0 -> Anzahl
	// Modus == 1 -> RunningNumber
	USHORT returnValue = 0;
	USHORT runningNumber = 0;
	if (Modus == 0)
	{
		if (WhatWorkbuilding == 0) returnValue = m_iFoodBuildings;
		else if (WhatWorkbuilding == 1) returnValue = m_iIndustryBuildings;
		else if (WhatWorkbuilding == 2) returnValue = m_iEnergyBuildings;
		else if (WhatWorkbuilding == 3) returnValue = m_iSecurityBuildings;
		else if (WhatWorkbuilding == 4) returnValue = m_iResearchBuildings;
		else if (WhatWorkbuilding == 5) returnValue = m_iTitanMines;
		else if (WhatWorkbuilding == 6) returnValue = m_iDeuteriumMines;
		else if (WhatWorkbuilding == 7) returnValue = m_iDuraniumMines;
		else if (WhatWorkbuilding == 8) returnValue = m_iCrystalMines;
		else if (WhatWorkbuilding == 9) returnValue = m_iIridiumMines;
		else if (WhatWorkbuilding == 10) returnValue = m_Workers.GetWorker(10);
		return returnValue;
	}
	else if (Modus == 1)
	{
		for (int i = 0; i < m_Buildings.GetSize(); i++)
		{
			CBuildingInfo* buildingInfo = &buildingInfos->GetAt(m_Buildings.GetAt(i).GetRunningNumber() - 1);
			
			if (buildingInfo->GetWorker() == TRUE)
			{
				if (WhatWorkbuilding == 0)
					if (buildingInfo->GetFoodProd())
					{
						runningNumber = m_Buildings.GetAt(i).GetRunningNumber();
						return runningNumber;
					}
				if (WhatWorkbuilding == 1)
					if (buildingInfo->GetIPProd())
					{
						runningNumber = m_Buildings.GetAt(i).GetRunningNumber();
						return runningNumber;
					}
				if (WhatWorkbuilding == 2)
					if (buildingInfo->GetEnergyProd())
					{
						runningNumber = m_Buildings.GetAt(i).GetRunningNumber();
						return runningNumber;
					}
				if (WhatWorkbuilding == 3)
					if (buildingInfo->GetSPProd())
					{
						runningNumber = m_Buildings.GetAt(i).GetRunningNumber();
						return runningNumber;
					}
				if (WhatWorkbuilding == 4)
					if (buildingInfo->GetFPProd())
					{
						runningNumber = m_Buildings.GetAt(i).GetRunningNumber();
						return runningNumber;
					}
				if (WhatWorkbuilding == 5)
					if (buildingInfo->GetTitanProd())
					{
						runningNumber = m_Buildings.GetAt(i).GetRunningNumber();
						return runningNumber;
					}
				if (WhatWorkbuilding == 6)
					if (buildingInfo->GetDeuteriumProd())
					{
						runningNumber = m_Buildings.GetAt(i).GetRunningNumber();
						return runningNumber;
					}
				if (WhatWorkbuilding == 7)
					if (buildingInfo->GetDuraniumProd())
					{
						runningNumber = m_Buildings.GetAt(i).GetRunningNumber();
						return runningNumber;
					}
				if (WhatWorkbuilding == 8)
					if (buildingInfo->GetCrystalProd())
					{
						runningNumber = m_Buildings.GetAt(i).GetRunningNumber();
						return runningNumber;
					}
				if (WhatWorkbuilding == 9)
					if (buildingInfo->GetIridiumProd())
					{
						runningNumber = m_Buildings.GetAt(i).GetRunningNumber();
						return runningNumber;
					}
			}
		}
	}
	return 0;
}

// Funktion gibt die Anzahl des Geb�udes mit der �bergebenen RunningNumber zur�ck.
USHORT CSystem::GetNumberOfBuilding(USHORT runningNumber) const
{
	USHORT number = 0;
	for (int i = 0; i < m_Buildings.GetSize(); i++)
		if (runningNumber == m_Buildings.GetAt(i).GetRunningNumber())
			number++;
	return number;
}

// Funktion gibt den Lagerinhalt der Ressource zur�ck, die an die Funktion �bergeben wurde.
UINT CSystem::GetRessourceStore(USHORT res) const
{
	switch (res)
	{
	case TITAN: {return this->GetTitanStore();}
	case DEUTERIUM: {return this->GetDeuteriumStore();}
	case DURANIUM: {return this->GetDuraniumStore();}
	case CRYSTAL: {return this->GetCrystalStore();}
	case IRIDIUM: {return this->GetIridiumStore();}
	case DILITHIUM: {return this->GetDilithiumStore();}
	}
	return 0;
}

// Funktion gibt einen Zeiger auf den Lagerinhalt der Ressource zur�ck, die an die Funktion �bergeben wurde.
UINT* CSystem::GetRessourceStorages(USHORT res)
{
	switch (res)
	{
	case TITAN: {return &m_iTitanStore;}
	case DEUTERIUM: {return &m_iDeuteriumStore;}
	case DURANIUM: {return &m_iDuraniumStore;}
	case CRYSTAL: {return &m_iCrystalStore;}
	case IRIDIUM: {return &m_iIridiumStore;}
	case DILITHIUM: {return &m_iDilithiumStore;}
	}
	return 0;
}

// Funktion gibt die Anzahl der aktuell in dem Feld gespeicherten Geb�ude mit der RunningNumber == number zur�ck.
// Diese Geb�ude sollen am Ende der Runde abgerissen werden.
USHORT CSystem::GetBuildingDestroy(int RunningNumber)
{
	USHORT number = 0; 
	for (int i = 0; i < m_BuildingDestroy.GetSize(); i++)
		if (m_BuildingDestroy.GetAt(i) == RunningNumber)
			number++;
	return number;
}

// Funktion sagt ob ein bestimmtes Geb�ude in dem System baubar ist oder nicht. Als Parameter werden daf�r
// die RunningNumber des gew�nschten Geb�udes und der Wert �bergeben.
void CSystem::SetBuildableBuildings(int RunningNumber, BOOLEAN TrueOrFalse)
{
	if (TrueOrFalse == FALSE)
	{
		for (int t = 0; t < m_BuildableBuildings.GetSize(); t++)
			if (m_BuildableBuildings.GetAt(t) == RunningNumber)
			{
				m_BuildableBuildings.RemoveAt(t);
				break;
			}
	}
	else
		m_BuildableBuildings.Add(RunningNumber);
}

// Funktion sagt ob ein bestimmtes Geb�udeupdate in dem System baubar ist oder nicht. Als Parameter werden daf�r
// die RunningNumber des gew�nschten Geb�udes und der Wert �bergeben.
void CSystem::SetBuildableUpdates(int RunningNumber, BOOLEAN TrueOrFalse)
{
	if (TrueOrFalse == FALSE)
	{
		for (int t = 0; t < m_BuildableUpdates.GetSize(); t++)
			if (m_BuildableUpdates.GetAt(t) == RunningNumber)
			{
				m_BuildableUpdates.RemoveAt(t);
				break;
			}
	}
	else
		m_BuildableUpdates.Add(RunningNumber);
}

// Setzt ein Geb�ude mit der richtigen RunningNumber online bzw. offline
void CSystem::SetIsBuildingOnline(int index, BOOLEAN newStatus)
{
	ASSERT(index < m_Buildings.GetSize());
	m_Buildings.ElementAt(index).SetIsBuildingOnline(newStatus);
}

// Komplette Zugriffsfunktion f�r das Arbeiterobjekt. Bei Modus 0 wird der "WhatWorker" inkrementiert, bei Modus 2 wird
// er dekrementiert und bei Modus 2 wird der "WhatWorker" auf den Wert von Value gesetzt.
void CSystem::SetWorker(int WhatWorker, int Value, int Modus)
{
	// Modus == 0 --> Inkrement
	// Modus == 1 --> Dekrement
	// Modus == 2 --> SetWorkers inkl. Value
	if (Modus == 0)
		m_Workers.InkrementWorker(WhatWorker);
	else if (Modus == 1)
		m_Workers.DekrementWorker(WhatWorker);
	else if (Modus == 2)
		m_Workers.SetWorker(WhatWorker,Value);
}

// Funktion setzt alle vorhandenen Arbeiter soweit wie m�glich in Geb�ude, die Arbeiter ben�tigen.
void CSystem::SetWorkersIntoBuildings()
{
	m_Workers.SetWorker(10,(int)(m_dHabitants));
	m_Workers.CalculateFreeWorkers();
	int numberOfWorkBuildings = 0;
	int workers = 0;
	for (int i = FOOD_WORKER; i <= IRIDIUM_WORKER; i++)
	{
		numberOfWorkBuildings += GetNumberOfWorkbuildings(i,0,NULL);
		workers += m_Workers.GetWorker(i);
	}
	while (m_Workers.GetWorker(11) > 0 && workers < numberOfWorkBuildings)
	{
		for (int i = FOOD_WORKER; i <= IRIDIUM_WORKER; i++)
			if (m_Workers.GetWorker(11) > 0 && GetNumberOfWorkbuildings(i,0,NULL) > m_Workers.GetWorker(i))
			{
				workers++;
				m_Workers.InkrementWorker(i);
				m_Workers.DekrementWorker(11);
			}
	}
}

// Funktion addiert resAdd zum Lagerinhalt der jeweiligen Ressource.
void CSystem::SetRessourceStore(USHORT res, int resAdd)
{
	// �berpr�fung das wir bei Lagerverkleinerung keine negativen Lager bekommen k�nnen
	if (resAdd < 0)
		if ((resAdd + (int)this->GetRessourceStore(res)) < 0)
			resAdd = this->GetRessourceStore(res) * (-1);
	// zum Lager hinzuf�gen/entfernen
	switch (res)
	{
	case TITAN:		{m_iTitanStore += resAdd; break;}
	case DEUTERIUM: {m_iDeuteriumStore += resAdd; break;}
	case DURANIUM:	{m_iDuraniumStore += resAdd; break;}
	case CRYSTAL:	{m_iCrystalStore += resAdd; break;}
	case IRIDIUM:	{m_iIridiumStore += resAdd; break;}
	case DILITHIUM:	{m_iDilithiumStore += resAdd; break;}
	}
}

// Diese Funktion setzt die abzurei�enden Geb�ude fest. Die zu �bergebenden Parameter sind die RunningNumber
// des Geb�udes und einen Wert 1 f�r hinzuf�gen und 0 f�r wieder aus der Liste nehmen
void CSystem::SetBuildingDestroy(int RunningNumber, BOOLEAN add)
{
	// Geb�ude zu Liste hinzuf�gen
	if (add == TRUE)
	{
		USHORT maxDestroy = this->GetNumberOfBuilding(RunningNumber);
		if (m_byBlockade > NULL)
			maxDestroy -= maxDestroy * m_byBlockade / 100;
		if (maxDestroy > this->GetBuildingDestroy(RunningNumber))
			m_BuildingDestroy.Add(RunningNumber);
	}
	else
	{
		for (int i = 0; i < m_BuildingDestroy.GetSize(); i++)
			if (m_BuildingDestroy.GetAt(i) == RunningNumber)
			{
				m_BuildingDestroy.RemoveAt(i);
				break;
			}
	}
}

// Funktion setzt die Bev�lkerungsanzahl des Systems. �bergeben wird die Bev�lkerung aller Planeten des Sektors.
// Gleichzeitig �berpr�ft die Funktion auch, ob man eine weitere Handelsroute aufgrund der Bev�lkerung bekommt, dann
// gibt die Funktion ein <code>TRUE</code> zur�ck, ansonsten <code>FALSE</code>.
BOOLEAN CSystem::SetHabitants(double habitants)
{
	m_dHabitants = habitants;

	if (((USHORT)m_dHabitants / TRADEROUTEHAB) > m_byMaxTradeRoutesFromHab)
	{
		m_byMaxTradeRoutesFromHab++;
		if (m_sOwnerOfSystem != "")
			return TRUE;
	}
	// Wenn die Bev�lkerung wieder zu niedrig ist, dann die Varibale wieder runterz�hlen
	else if (((USHORT)m_dHabitants / TRADEROUTEHAB) < m_byMaxTradeRoutesFromHab)
		m_byMaxTradeRoutesFromHab--;
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// sonstige Funktionen
//////////////////////////////////////////////////////////////////////
// Funktion berechnet aus den Eigenschaften der stehenden Geb�ude alle Attribute der System-Klasse.
void CSystem::CalculateVariables(BuildingInfoArray* buildingInfos, CResearchInfo* ResearchInfo, CArray<CPlanet>* planets, CMajor* pOwner, const CString *sMonopolOwner)
{
	int NumberOfBuildings;
	NumberOfBuildings = m_Buildings.GetSize();
	// Alle werde wieder auf NULL setzen	
	m_Production.Reset();
	// Die Anzahl der Arbeiter aus der aktuellen Bev�lkerung berechnen und auch an die Klasse CWorker �bergeben
	m_Workers.SetWorker(10,(int)(m_dHabitants));
	m_Workers.CheckWorkers();
	// Die Latinumprod. aus der Bev�lkerung berechnen und modifizieren durch jeweilige Rasseneigenschaft
	m_Production.m_iLatinumProd = (int)(m_dHabitants);

	float fCreditsMulti = 1.0f;
	if (pOwner->IsRaceProperty(FINANCIAL))
		fCreditsMulti += 0.5f;
	if (pOwner->IsRaceProperty(PRODUCER))
		fCreditsMulti += 0.25f;
	if (pOwner->IsRaceProperty(WARLIKE) || pOwner->IsRaceProperty(HOSTILE))
		fCreditsMulti -= 0.1f;
	if (pOwner->IsRaceProperty(SNEAKY))
		fCreditsMulti -= 0.2f;
	if (pOwner->IsRaceProperty(SECRET))
		fCreditsMulti -= 0.25f;
	if (pOwner->IsRaceProperty(SOLOING))
		fCreditsMulti -= 0.5f;

	fCreditsMulti = max(fCreditsMulti, 0.0f);
	m_Production.m_iLatinumProd = (int)(m_Production.m_iLatinumProd * fCreditsMulti);
	// Die Geb�ude online setzen, wenn das Objekt der Klasse CWorker das sagt
	// zuerst die Anzahl der Arbeiter auslesen und schauen ob die Arbeiter vielleicht gr��er sind als die 
	// Anzahl der jeweiligen Geb�ude (z.B. durch Abri� aus letzter Runde) -> dann Arbeiter auf Geb�udeanzahl verringern
	if (m_Workers.GetWorker(FOOD_WORKER) > m_iFoodBuildings) m_Workers.SetWorker(FOOD_WORKER,m_iFoodBuildings);
	unsigned short foodWorker = m_Workers.GetWorker(0);
	if (m_Workers.GetWorker(1) > m_iIndustryBuildings) m_Workers.SetWorker(1,m_iIndustryBuildings);
	unsigned short industryWorker = m_Workers.GetWorker(1);
	if (m_Workers.GetWorker(2) > m_iEnergyBuildings) m_Workers.SetWorker(2,m_iEnergyBuildings);
	unsigned short energyWorker = m_Workers.GetWorker(2);
	if (m_Workers.GetWorker(3) > m_iSecurityBuildings) m_Workers.SetWorker(3,m_iSecurityBuildings);
	unsigned short securityWorker = m_Workers.GetWorker(3);
	if (m_Workers.GetWorker(4) > m_iResearchBuildings) m_Workers.SetWorker(4,m_iResearchBuildings);
	unsigned short researchWorker = m_Workers.GetWorker(4);
	if (m_Workers.GetWorker(5) > m_iTitanMines) m_Workers.SetWorker(5,m_iTitanMines);
	unsigned short titanWorker = m_Workers.GetWorker(5);
	if (m_Workers.GetWorker(6) > m_iDeuteriumMines) m_Workers.SetWorker(6,m_iDeuteriumMines);
	unsigned short deuteriumWorker = m_Workers.GetWorker(6);
	if (m_Workers.GetWorker(7) > m_iDuraniumMines) m_Workers.SetWorker(7,m_iDuraniumMines);
	unsigned short duraniumWorker = m_Workers.GetWorker(7);
	if (m_Workers.GetWorker(8) > m_iCrystalMines) m_Workers.SetWorker(8,m_iCrystalMines);
	unsigned short crystalWorker = m_Workers.GetWorker(8);
	if (m_Workers.GetWorker(9) > m_iIridiumMines) m_Workers.SetWorker(9,m_iIridiumMines);
	unsigned short iridiumWorker = m_Workers.GetWorker(9);
	
	// Wenn wir Handelsg�ter in der Bauliste stehen haben, dann Anzahl der Online-Fabs in Latinum umrechnen
	if (m_AssemblyList.GetAssemblyListEntry(0) == 0)
		m_Production.m_iLatinumProd += industryWorker;

	// Die einzelnen Produktionen berechnen
	for (int i = 0; i < NumberOfBuildings; i++)
	{
		const CBuildingInfo* buildingInfo = &buildingInfos->GetAt(m_Buildings.GetAt(i).GetRunningNumber() - 1);

		// Bei einer Blockade werden auch die Werften offline gesetzt
		if (m_byBlockade >= 100 && buildingInfo->GetShipYard() && buildingInfo->GetNeededEnergy() > 0)
			m_Buildings.GetAt(i).SetIsBuildingOnline(FALSE);

		// Geb�ude offline setzen
		if (buildingInfo->GetWorker() == TRUE)
		{
			m_Buildings.ElementAt(i).SetIsBuildingOnline(FALSE);
			// Jetzt wieder wenn m�glich online setzen		
			if (buildingInfo->GetFoodProd() > 0 && foodWorker > 0)
			{
				m_Buildings.ElementAt(i).SetIsBuildingOnline(TRUE);
				foodWorker--;
			}
			else if (buildingInfo->GetIPProd() > 0 && industryWorker > 0)
			{
				m_Buildings.ElementAt(i).SetIsBuildingOnline(TRUE);
				industryWorker--;
			}
			else if (buildingInfo->GetEnergyProd() > 0 && energyWorker > 0)
			{
				m_Buildings.ElementAt(i).SetIsBuildingOnline(TRUE);
				energyWorker--;
			}
			else if (buildingInfo->GetSPProd() > 0 && securityWorker > 0)
			{
				m_Buildings.ElementAt(i).SetIsBuildingOnline(TRUE);
				securityWorker--;
			}
			else if (buildingInfo->GetFPProd() > 0 && researchWorker > 0)
			{
				m_Buildings.ElementAt(i).SetIsBuildingOnline(TRUE);
				researchWorker--;
			}
			else if (buildingInfo->GetTitanProd() > 0 && titanWorker > 0)
			{
				m_Buildings.ElementAt(i).SetIsBuildingOnline(TRUE);
				titanWorker--;
			}
			else if (buildingInfo->GetDeuteriumProd() > 0 && deuteriumWorker > 0)
			{
				m_Buildings.ElementAt(i).SetIsBuildingOnline(TRUE);
				deuteriumWorker--;
			}
			else if (buildingInfo->GetDuraniumProd() > 0 && duraniumWorker > 0)
			{
				m_Buildings.ElementAt(i).SetIsBuildingOnline(TRUE);
				duraniumWorker--;
			}
			else if (buildingInfo->GetCrystalProd() > 0 && crystalWorker > 0)
			{
				m_Buildings.ElementAt(i).SetIsBuildingOnline(TRUE);
				crystalWorker--;
			}
			else if (buildingInfo->GetIridiumProd() > 0 && iridiumWorker > 0)
			{
				m_Buildings.ElementAt(i).SetIsBuildingOnline(TRUE);
				iridiumWorker--;
			}
		}
		
		// Die einzelnen Produktionen berechnen (ohne Boni)
		// vorher noch schauen, ob diese Geb�ude auch online sind
		if (m_Buildings.GetAt(i).GetIsBuildingOnline() == TRUE)
			m_Production.CalculateProduction(buildingInfo);
	}
	// Latinum durch Handelsrouten berechnen
	m_Production.m_iLatinumProd += LatinumFromTradeRoutes();

	// Besitzt jemand ein Monopol auf eine Ressource, so verdoppelt sich seine Produktion
	if (sMonopolOwner[TITAN] == m_sOwnerOfSystem)
		m_Production.m_iTitanProd *= 2;
	if (sMonopolOwner[DEUTERIUM] == m_sOwnerOfSystem)
		m_Production.m_iDeuteriumProd *= 2;
	if (sMonopolOwner[DURANIUM] == m_sOwnerOfSystem)
		m_Production.m_iDuraniumProd *= 2;
	if (sMonopolOwner[CRYSTAL] == m_sOwnerOfSystem)
		m_Production.m_iCrystalProd *= 2;
	if (sMonopolOwner[IRIDIUM] == m_sOwnerOfSystem)
		m_Production.m_iIridiumProd *= 2;
	
	// Die Boni auf die einzelnen Produktionen berechnen
	short tmpFoodBoni = 0;
	short tmpIndustryBoni = 0;
	short tmpEnergyBoni = 0;
	short tmpSecurityBoni = 0;
	short tmpResearchBoni = 0;
	short tmpTitanBoni = 0;
	short tmpDeuteriumBoni = 0;
	short tmpDuraniumBoni = 0;
	short tmpCrystalBoni = 0;
	short tmpIridiumBoni = 0;
	short tmpDilithiumBoni = 0;
	short tmpAllRessourcesBoni = 0;
	short tmpLatinumBoni = 0;

	short neededEnergy = 0;
	for (int i = 0; i < NumberOfBuildings; i++)
	{
		const CBuildingInfo* buildingInfo = &buildingInfos->GetAt(m_Buildings.GetAt(i).GetRunningNumber() - 1);

		// Hier die n�tige Energie von der produzierten abziehen, geht aber nur hier, wenn wir keine Boni zur Energie reinmachen
		if (m_Buildings.GetAt(i).GetIsBuildingOnline() == TRUE && buildingInfo->GetNeededEnergy() > 0)
			//m_Production.m_iEnergyProd -= m_Buildings.GetAt(i).GetNeededEnergy();
			neededEnergy += buildingInfo->GetNeededEnergy();

		if (m_Buildings.GetAt(i).GetIsBuildingOnline() == TRUE)
		{
			// Es wird IMMER abgerundet, gemacht durch "floor"
			tmpFoodBoni			+= buildingInfo->GetFoodBoni();
			tmpIndustryBoni		+= buildingInfo->GetIndustryBoni();
			tmpEnergyBoni		+= buildingInfo->GetEnergyBoni();
			tmpSecurityBoni		+= buildingInfo->GetSecurityBoni();
			tmpResearchBoni		+= buildingInfo->GetResearchBoni();
			tmpAllRessourcesBoni = buildingInfo->GetAllRessourcesBoni();
			tmpTitanBoni		+= buildingInfo->GetTitanBoni() + tmpAllRessourcesBoni;
			tmpDeuteriumBoni	+= buildingInfo->GetDeuteriumBoni() + tmpAllRessourcesBoni;
			tmpDuraniumBoni		+= buildingInfo->GetDuraniumBoni() + tmpAllRessourcesBoni;
			tmpCrystalBoni		+= buildingInfo->GetCrystalBoni() + tmpAllRessourcesBoni;
			tmpIridiumBoni		+= buildingInfo->GetIridiumBoni() + tmpAllRessourcesBoni;
			tmpDilithiumBoni	+= buildingInfo->GetDilithiumBoni(); 
			tmpLatinumBoni		+= buildingInfo->GetLatinumBoni();
		}
	}
	
	// Jetzt werden noch eventuelle Boni durch die Planetenklassen dazugerechnet
	BYTE dilithiumProdMulti = 0;
	for (int i = 0; i < planets->GetSize(); i++)
		if (planets->GetAt(i).GetColonized() == TRUE && planets->GetAt(i).GetCurrentHabitant() > 0.0f)
		{
			// pro Planetengr��e gibt es 25% Bonus
			if (planets->GetAt(i).GetBoni()[TITAN] == TRUE)
				tmpTitanBoni	+= (planets->GetAt(i).GetSize()+1) * 25;
			if (planets->GetAt(i).GetBoni()[DEUTERIUM] == TRUE)
				tmpDeuteriumBoni+= (planets->GetAt(i).GetSize()+1) * 25;
			if (planets->GetAt(i).GetBoni()[DURANIUM] == TRUE)
				tmpDuraniumBoni	+= (planets->GetAt(i).GetSize()+1) * 25;
			if (planets->GetAt(i).GetBoni()[CRYSTAL] == TRUE)
				tmpCrystalBoni	+= (planets->GetAt(i).GetSize()+1) * 25;
			if (planets->GetAt(i).GetBoni()[IRIDIUM] == TRUE)
				tmpIridiumBoni	+= (planets->GetAt(i).GetSize()+1) * 25;
			if (planets->GetAt(i).GetBoni()[6] == TRUE)	// food
				tmpFoodBoni		+= (planets->GetAt(i).GetSize()+1) * 25;
			if (planets->GetAt(i).GetBoni()[7] == TRUE)	// energy
				tmpEnergyBoni	+= (planets->GetAt(i).GetSize()+1) * 25;
			// Menge des abgebauten Dilithiums mit der Anzahl der kolonisierten Planeten mit Dilithiumvorkommen
			// multiplizieren
			if (planets->GetAt(i).GetBoni()[DILITHIUM] == TRUE)
				dilithiumProdMulti += 1;
		}
	m_Production.m_iDilithiumProd	*= dilithiumProdMulti;

	m_Production.m_iFoodProd		+= (int)(tmpFoodBoni*m_Production.m_iFoodProd/100);
	m_Production.m_iIndustryProd	+= (int)(tmpIndustryBoni*m_Production.m_iIndustryProd/100);
	m_Production.m_iEnergyProd		+= (int)(tmpEnergyBoni*m_Production.m_iEnergyProd/100);
	m_Production.m_iSecurityProd	+= (int)(tmpSecurityBoni*m_Production.m_iSecurityProd/100);
	m_Production.m_iResearchProd	+= (int)(tmpResearchBoni*m_Production.m_iResearchProd/100);
	m_Production.m_iTitanProd		+= (int)(tmpTitanBoni*m_Production.m_iTitanProd/100);
	m_Production.m_iDeuteriumProd	+= (int)(tmpDeuteriumBoni*m_Production.m_iDeuteriumProd/100);
	m_Production.m_iDuraniumProd	+= (int)(tmpDuraniumBoni*m_Production.m_iDuraniumProd/100);
	m_Production.m_iCrystalProd		+= (int)(tmpCrystalBoni*m_Production.m_iCrystalProd/100);
	m_Production.m_iIridiumProd		+= (int)(tmpIridiumBoni*m_Production.m_iIridiumProd/100);
	m_Production.m_iDilithiumProd	+= (int)(tmpDilithiumBoni*m_Production.m_iDilithiumProd/100);
	m_Production.m_iLatinumProd		+= (int)(tmpLatinumBoni*m_Production.m_iLatinumProd/100);

	// Wenn das System blockiert wird, dann verringern sich bestimmte Produktionswerte
	if (m_byBlockade > NULL)
	{
		//m_Production.m_iFoodProd		-= (int)(m_byBlockade * m_Production.m_iFoodProd/100);
		m_Production.m_iIndustryProd	-= (int)(m_byBlockade * m_Production.m_iIndustryProd/100);
		//m_Production.m_iEnergyProd		-= (int)(m_byBlockade * m_Production.m_iEnergyProd/100);
		m_Production.m_iSecurityProd	-= (int)(m_byBlockade * m_Production.m_iSecurityProd/100);
		m_Production.m_iResearchProd	-= (int)(m_byBlockade * m_Production.m_iResearchProd/100);
		m_Production.m_iTitanProd		-= (int)(m_byBlockade * m_Production.m_iTitanProd/100);
		m_Production.m_iDeuteriumProd	-= (int)(m_byBlockade * m_Production.m_iDeuteriumProd/100);
		m_Production.m_iDuraniumProd	-= (int)(m_byBlockade * m_Production.m_iDuraniumProd/100);
		m_Production.m_iCrystalProd		-= (int)(m_byBlockade * m_Production.m_iCrystalProd/100);
		m_Production.m_iIridiumProd		-= (int)(m_byBlockade * m_Production.m_iIridiumProd/100);
		m_Production.m_iDilithiumProd	-= (int)(m_byBlockade * m_Production.m_iDilithiumProd/100);
		m_Production.m_iLatinumProd		-= (int)(m_byBlockade * m_Production.m_iLatinumProd/100);
		if (m_byBlockade >= 100)
			m_Production.m_bShipYard	= false;
	}

	// Hier noch Boni auf das Spezialzeug geben (z.B. Scanpower usw.)
	m_Production.m_iScanPower		+= (int)(m_Production.m_iScanPowerBoni*m_Production.m_iScanPower/100);
	m_Production.m_iScanRange		+= (int)(m_Production.m_iScanRangeBoni*m_Production.m_iScanRange/100);
	m_Production.m_iGroundDefend	+= (int)(m_Production.m_iGroundDefendBoni*m_Production.m_iGroundDefend/100);
	m_Production.m_iShipDefend		+= (int)(m_Production.m_iShipDefendBoni*m_Production.m_iShipDefend/100);
	m_Production.m_iShieldPower		+= (int)(m_Production.m_iShieldPowerBoni*m_Production.m_iShieldPower/100);

	///// HIER DIE BONI DURCH SPEZIALFORSCHUNG //////
	// Hier die Boni durch die Uniqueforschung "Wirtschaft" -> 10% mehr Industrie
	if (ResearchInfo->GetResearchComplex(5)->GetFieldStatus(1) == RESEARCHED)
		m_Production.m_iIndustryProd += (int)(ResearchInfo->GetResearchComplex(5)->GetBonus(1)*m_Production.m_iIndustryProd/100);
	else if (ResearchInfo->GetResearchComplex(5)->GetFieldStatus(3) == RESEARCHED)
		m_Production.m_iLatinumProd += (int)(ResearchInfo->GetResearchComplex(5)->GetBonus(3) * m_Production.m_iLatinumProd / 100);
	// Hier die Boni durch die Uniqueforschung "Produktion"
	if (ResearchInfo->GetResearchComplex(6)->GetFieldStatus(1) == RESEARCHED)
		m_Production.m_iFoodProd += (int)(ResearchInfo->GetResearchComplex(6)->GetBonus(1)*m_Production.m_iFoodProd/100); 
	else if (ResearchInfo->GetResearchComplex(6)->GetFieldStatus(2) == RESEARCHED)
	{
		m_Production.m_iTitanProd += (int)(ResearchInfo->GetResearchComplex(6)->GetBonus(2)*m_Production.m_iTitanProd/100);
		m_Production.m_iDeuteriumProd += (int)(ResearchInfo->GetResearchComplex(6)->GetBonus(2)*m_Production.m_iDeuteriumProd/100);
		m_Production.m_iDuraniumProd += (int)(ResearchInfo->GetResearchComplex(6)->GetBonus(2)*m_Production.m_iDuraniumProd/100);
		m_Production.m_iCrystalProd += (int)(ResearchInfo->GetResearchComplex(6)->GetBonus(2)*m_Production.m_iCrystalProd/100);
		m_Production.m_iIridiumProd += (int)(ResearchInfo->GetResearchComplex(6)->GetBonus(2)*m_Production.m_iIridiumProd/100);
	}
	// Wenn wir die Uniqueforschung "Produktion" gew�hlt haben, und dort mehr Energie haben wollen -> 20% mehr!
	else if (ResearchInfo->GetResearchComplex(6)->GetFieldStatus(3) == RESEARCHED)
		m_Production.m_iEnergyProd += (int)(ResearchInfo->GetResearchComplex(6)->GetBonus(3)*m_Production.m_iEnergyProd/100); 
	
	// Hier die Boni durch die Uniqueforschung "Forschung und Sicherheit"
	if (ResearchInfo->GetResearchComplex(7)->GetFieldStatus(1) == RESEARCHED)
		m_Production.m_iResearchProd += (int)(ResearchInfo->GetResearchComplex(7)->GetBonus(1)*m_Production.m_iResearchProd/100);
	else if (ResearchInfo->GetResearchComplex(7)->GetFieldStatus(2) == RESEARCHED)
		m_Production.m_iSecurityProd += (int)(ResearchInfo->GetResearchComplex(7)->GetBonus(1)*m_Production.m_iSecurityProd/100);
	else if (ResearchInfo->GetResearchComplex(7)->GetFieldStatus(3) == RESEARCHED)
	{
		m_Production.m_iResearchProd += (int)(ResearchInfo->GetResearchComplex(7)->GetBonus(3)*m_Production.m_iResearchProd/100);
		m_Production.m_iSecurityProd += (int)(ResearchInfo->GetResearchComplex(7)->GetBonus(3)*m_Production.m_iSecurityProd/100);
	}
	
	// Maximalenergie, also hier noch ohne Abz�ge durch energiebed�rftige Geb�ude
	m_Production.m_iMaxEnergyProd = m_Production.m_iEnergyProd;
	// hier die gesamte Energie durch energiebed�rftige Geb�ude abziehen
	m_Production.m_iEnergyProd -= neededEnergy;
	
	// imperiumweite Moralprod mit aufrechnen
	m_Production.m_iMoralProd += m_Production.m_iMoralProdEmpireWide[m_sOwnerOfSystem];
	// Den Moralboni im System noch auf die einzelnen Produktionen anrechnen
	m_Production.IncludeSystemMoral(m_iMoral);
	// ben�tigte Nahrung durch Bev�lkerung von der Produktion abiehen
	m_Production.m_iFoodProd -= (int)ceil(m_dHabitants*10);	// ceil, wird auf Kommezahl berechnet, z.B brauchen wir f�r 
												// 14.5 Mrd. Leute 145 Nahrung und nicht 140 bzw. 150
	// Jetzt noch die freien Arbeiter berechnen
	m_Workers.CalculateFreeWorkers();
}

// Funktion berechnet die Lagerinhalte des Systems. Aufrufen bei Ende bzw. Beginn einer neuen Runde.
// Gibt die Funktion TRUE zur�ck hat sich das System Aufgrund zu schlechter Moral vom Besitzer losgesagt.
BOOLEAN CSystem::CalculateStorages(CResearchInfo* researchInfo, int diliAdd)
{
	// wie oben gesagt, noch �berarbeiten, wenn negatives Lager sein w�rdem dann darf das System nicht mehr wachsen
	// bzw. wird kleiner!!!
	m_iFoodStore += m_Production.m_iFoodProd;
	m_iTitanStore += m_Production.m_iTitanProd;
	m_iDeuteriumStore += m_Production.m_iDeuteriumProd;
	m_iDuraniumStore += m_Production.m_iDuraniumProd;
	m_iCrystalStore += m_Production.m_iCrystalProd;
	m_iIridiumStore += m_Production.m_iIridiumProd;
	m_iDilithiumStore += m_Production.m_iDilithiumProd + diliAdd;
	
	// Lagerobergrenzen
	if (m_iFoodStore > 25000)
		m_iFoodStore = 25000;
	if (m_iTitanStore > 125000)
		m_iTitanStore = 125000;
	if (m_iDeuteriumStore > 125000)
		m_iDeuteriumStore = 125000;
	if (m_iDuraniumStore > 125000)
		m_iDuraniumStore = 125000;
	if (m_iCrystalStore > 125000)
		m_iCrystalStore = 125000;
	if (m_iIridiumStore > 125000)
		m_iIridiumStore = 125000;
	
	short multi = 1;
	///// HIER DIE BONI DURCH SPEZIALFORSCHUNG //////
	// Hier die Boni durch die Uniqueforschung "Lager und Transport" -> doppeltes Dilithiumlager
	if (researchInfo->GetResearchComplex(10)->GetFieldStatus(1) == RESEARCHED)
		multi = researchInfo->GetResearchComplex(10)->GetBonus(1);
	if ((int)m_iDilithiumStore > 100 * multi)
		m_iDilithiumStore = 100 * multi;

	m_iMoral += (short)m_Production.m_iMoralProd;
	
	// Wenn wir eine Moral von -1 haben, senkt sich der Wert auf maximal noch 85
	if (m_iMoral < 85 && m_Production.m_iMoralProd == -1)
		m_iMoral = 85;
	// Wenn wir eine Moral von -2 haben, senkt sich der Wert auf maximal noch 70
	else if (m_iMoral < 70 && m_Production.m_iMoralProd == -2)
		m_iMoral = 70;

	// Moralanhebung bzw. -absenkung mal als "Lager" gesehen -> Moral, die ins Lager/System kommt
	// hier noch berechnen, das Moral von Stufen abh�ngt
	if (m_iMoral <= 30) m_iMoral -= 4;
	else if (m_iMoral <= 40) m_iMoral -= 3;
	else if (m_iMoral <= 50) m_iMoral -= 2;
	else if (m_iMoral <= 60) m_iMoral -= 1;
	else if (m_iMoral >= 190) m_iMoral -= 5;
	else if (m_iMoral >= 173) m_iMoral -= 4;
	else if (m_iMoral >= 152) m_iMoral -= 3;
	else if (m_iMoral >= 121) m_iMoral -= 2;
	else if (m_iMoral >= 110) m_iMoral -= 1;

	// ohne weiter Moralprod gehts wieder auf 100 zur�ck, wenn die Moral nicht gr��er 110 ist
	if (m_iMoral > 100 && m_iMoral <= 109 && m_Production.m_iMoralProd == 0) m_iMoral--; 
	if (m_iMoral > 200) m_iMoral -= 6;
	if (m_iMoral < 0) m_iMoral = 0;
	if (m_iMoral > 200) m_iMoral = 200;
	
	// Jetzt �berpr�fen, ob das System sich bei einer Moral von unter 30 vom Imperium lossagt
	if (m_iMoral < 30)
	{
		// zuf�llig, nicht zwangsl�ufig sagt es sich los, erst bei einer Moral von 8
		USHORT random = rand()%(m_iMoral+1);
		if (random < 8)
		{
			m_sOwnerOfSystem = "";
			m_Production.m_iFoodProd = 10;
			m_Production.m_iIndustryProd = 5;	
			m_iMoral = 65;
			if (m_iFoodStore < 0)
				m_iFoodStore = 0;
			return TRUE;		// Das System hat sich losgesagt
		}
	}
	return FALSE;
}

// Funktion l�scht alle Geb�ude, die die �bergebene RunningNumber haben und gibt deren Anzahl zur�ck.
// -> Danach mu� AddBuilding() mit dem Nachfolger gleich der Anzahl aufgerufen werden.
int CSystem::UpdateBuildings(int RunningNumber)
{
	int counter = 0;
	for (int i = 0; i < m_Buildings.GetSize(); i++)
		if (m_Buildings.GetAt(i).GetRunningNumber() == RunningNumber)
		{
			m_Buildings.RemoveAt(i--);
			counter++;
		}
	return counter;
}

// Funktion rei�t alle Geb�ude ab, die in der Variable m_BuildingDestroy stehen. Funktion wird in der Doc 
// bei NextRound() aufgerufen.
bool CSystem::DestroyBuildings(void)
{
	bool destroy = !m_BuildingDestroy.IsEmpty();
	for (int i = 0; i < m_BuildingDestroy.GetSize(); i++)
		// Schleife von hinten durchlaufen, weil Arbeiter die Geb�ude von "vorn" besetzten
		// und ich nicht gerade die online-Geb�ude abrei�en will, sondern die wahrscheinlich offline
		for (int j = m_Buildings.GetUpperBound(); j >= 0; j--)
			if (m_Buildings.GetAt(j).GetRunningNumber() == m_BuildingDestroy.GetAt(i))
			{
				m_Buildings.RemoveAt(j);
				m_BuildingDestroy.RemoveAt(i--);
				break;
			}
	if (destroy)
		m_BuildingDestroy.RemoveAll();		
	return destroy;
}

// Funktion berechnet die baubaren Geb�ude und Geb�udeupdates in dem System.
void CSystem::CalculateBuildableBuildings(CSector* sector, BuildingInfoArray* buildingInfo, CMajor* pMajor, CGlobalBuildings* globals)
{
	CEmpire* empire = pMajor->GetEmpire();
	ASSERT(empire);

	for (int k = 0; k < m_BuildableWithoutAssemblylistCheck.GetSize(); )
		m_BuildableWithoutAssemblylistCheck.RemoveAt(k);
	m_BuildableWithoutAssemblylistCheck.RemoveAll();
	m_BuildableWithoutAssemblylistCheck.FreeExtra();

	for (int k = 0; k < m_BuildableBuildings.GetSize(); )
		m_BuildableBuildings.RemoveAt(k);
	m_BuildableBuildings.RemoveAll();

/*
	Algorithmus:

	1. schon stehende Geb�ude und Geb�ude in der Liste der immer baubaren Geb�ude 
	   durchgehen und stehende Geb�ude gleichzeitig nach Voraussetzungen pr�fen
		1.1 - JA)   k�nnen dieses bauen und Punkt (0.) durchf�hren
		1.2 - NEIN) k�nnen es nicht bauen

	2. Geb�udeinfos durchgehen, nach allg. Voraussetzungen checken und pr�fen, dass
	   diese ID oder ein �quivalent NICHT schon in der Liste der baubaren Geb�ude sind
		2.1 - JA)   Wenn Geb�ude eine ID des Vorg�ngers besitzt
			2.1.1 - JA)   Schauen ob dieses Vorg�ngergeb�ude im System steht
				2.1.1.1 - JA)	Geb�udeupgrade in die Liste nehmen
				2.1.1.2 - NEIN) k�nnen es nicht bauen
			2.1.2 - NEIN) �berpr�fen ob ein Nachfolger des Geb�udes in der Liste der baubaren Geb�ude steht (*)
				2.1.2.1 - JA)	k�nnen es nicht bauen
				2.1.2.2 - NEIN) �berpr�fen ob ein Nachfolger des Geb�udes in dem System steht (*)
					2.1.2.2.1 - JA)	  k�nnen es nicht bauen
					2.1.2.2.1 - NEIN) k�nnen es bauen
		2.2 - NEIN) k�nnen es nicht bauen

	(*) -> nur wenn dieses Geb�ude auch updatebar ist!!! Ansonsten k�nnen wir es bauen

	3. zus�tzlichen Baulistencheck durchf�hren, d.h wenn es z.B. nur einmal pro Imperium
	   baubar ist, dann darf es sich auch in keiner Bauliste befinden. Auch Updates im System
	   d�rfen nicht schon in der Bauliste stehen.
		3.1 - JA)   Auftrag bleibt baubar
		3.2 - NEIN) Auftrag ist nicht mehr baubar

	0. mindst. baubare Geb�ude durchgehen und checken ob unsere ID in der Liste vorkommt
	0.1 - JA)   nichts weiter machen
	0.2 - NEIN) m�glichen Vorg�nger unseres Geb�udes in der Liste suchen. Gefunden?
		0.2.1 - JA)   Vorg�nger ID mit unserer ID ersetzen
		0.2.2 - NEIN) Unserer ID in die Liste einschreiben
		
	// gut w�re eine Variable, die alle ID�s speichert, die wir vor dem Baulistencheck bauen k�nnen.
	// Wenn wir dann einen Baulistencheck machen, bleibt diese Variable davon unber�hrt. Nur die Variablen,
	// in denen die nun baubaren Auftr�ge stehen werden ver�ndert. Wenn ich einen Auftrag in die Assemblylist
	// nehme, wird der Baulistencheck ausgef�hrt. Auch wenn ich diesen wieder aus der Bauliste nehme wird der
	// Baulistencheck ausgef�hrt. Nun k�nnte man noch einbauen, das dieser nicht gleich f�r jedes Geb�ude 
	// gemacht wird, sondern nur bei denen, bei denen es notwendig ist.
*/
// ------------------------------------ 1 -------------------------------------------- //
	// schon stehende Geb�ude checken
	USHORT minID = 0;
	BOOLEAN equivalents = FALSE;	// steht in dem System ein Geb�ude einer anderen Rasse (eins was wir nicht h�tten bauen k�nnen)
	for (int i = 0; i < m_Buildings.GetSize(); i++)
	{
		// Wenn wir auf das Geb�ude in der Infoliste zugreifen wollen, dann steht dieses dort immer
		// an der Stelle ID-1, weil das Feld bei 0 beginnt und unsere ID's aber bei 1 anfangen!!!
		USHORT id = m_Buildings.GetAt(i).GetRunningNumber();
		BOOLEAN found = FALSE;
		// Da das Feld der Geb�ude im System aufsteigend nach der RunningNumber sortiert ist, brauchen wir immer erst
		// checken, wenn wir eine neue RunningNumber haben
		if (id > minID)
		{
			// steht hier ein Geb�ude einer anderen Rasse?
			if (buildingInfo->GetAt(id-1).GetOwnerOfBuilding() != pMajor->GetRaceBuildingNumber())
				equivalents = TRUE;
			minID = id;
			// Checken das das Geb�ude nicht schon in der Liste der baubaren Geb�ude vorkommt
			for (int k = 0; k < m_BuildableWithoutAssemblylistCheck.GetSize(); k++)
				if (m_BuildableWithoutAssemblylistCheck.GetAt(k) == id)
				{
					found = TRUE;
					break;
				}
			// Checken ob wir es durch die vorhanden Planeten bauen k�nnen
			if (found == FALSE && CheckPlanet(&buildingInfo->GetAt(id-1),sector))
				// allg. Voraussetzungen des Geb�udes checken
				if (CheckGeneralConditions(&buildingInfo->GetAt(id-1),sector,globals,pMajor))
				{
					// Wenn wir hier angekommen sind, dann k�nnten wir das Geb�ude bauen.
					// Ist es ein Geb�ude, welches wir manchmal mindst immer bauen k�nnen (alle Geb�ude, die Arbeiter
					// ben�tigen!!!), dann m�ssen wir noch Punkt (0.) durchf�hren.
					BOOLEAN id_in_list = FALSE;
					const CBuildingInfo* info = &buildingInfo->GetAt(m_Buildings.GetAt(i).GetRunningNumber() - 1);

					if (info->GetWorker())
					{
						for (int j = 0; j < m_AllwaysBuildableBuildings.GetSize(); j++)
							if (id == m_AllwaysBuildableBuildings.GetAt(j))
							{
								id_in_list = TRUE;	// gefunden -> k�nnen abbrechen
								break;
							}
						// Wir haben unseres Geb�ude nicht schon in der Liste der immer baubaren Geb�ude gefunden,
						// also mit Punkt (0.2) fortfahren!
						if (id_in_list == FALSE && buildingInfo->GetAt(id-1).GetPredecessorID() != 0)
						{
							BOOLEAN found_predecessor = FALSE;
							// Vorg�nger unseres Geb�udes finden (es werden noch nicht alle m�glichen Vorg�nger gesucht!!!)
							for (int j = 0; j < m_AllwaysBuildableBuildings.GetSize(); j++)
								if (m_AllwaysBuildableBuildings.GetAt(j) == buildingInfo->GetAt(id-1).GetPredecessorID())
								{
									m_AllwaysBuildableBuildings.SetAt(j, id);
									found_predecessor = TRUE;
									break;
								}
							// Haben wir keinen Vorg�nger gefunden, so f�gen wir dieses Geb�ude ein
							if (found_predecessor == FALSE)
								m_AllwaysBuildableBuildings.Add(id);
						}
					}
					// Nachdem wir Punkt (0.) durchgef�hrt haben f�gen wir unser jetziges Geb�ude noch in die Liste 
					// der baubaren Geb�ude und Updates ein (ohne Baulistencheck)
					m_BuildableWithoutAssemblylistCheck.Add(id);
				}
		}
	}
	// Hier noch die Geb�ude aus der immer baubaren Geb�udeliste in die Liste der baubaren Geb�ude hinzuf�gen,
	// wenn sie nicht schon drin stehen
	USHORT size = m_BuildableWithoutAssemblylistCheck.GetSize();
	for (int i = 0; i < m_AllwaysBuildableBuildings.GetSize(); i++)
	{
		// Gibt es in der Liste der immer baubaren Geb�ude ein Geb�ude einer anderen Rasse?
		if (buildingInfo->GetAt(m_AllwaysBuildableBuildings.GetAt(i)-1).GetOwnerOfBuilding() != pMajor->GetRaceBuildingNumber())
			equivalents = TRUE;
		BOOLEAN found = FALSE;
		for (int j = 0; j < size; j++)
			if (m_AllwaysBuildableBuildings.GetAt(i) == m_BuildableWithoutAssemblylistCheck.GetAt(j))
			{
				found = TRUE;
				break;
			}
		if (found == FALSE)
			m_BuildableWithoutAssemblylistCheck.Add(m_AllwaysBuildableBuildings.GetAt(i));
	}	

// ------------------------------------ 2 -------------------------------------------- //
	// alle anderen Geb�udeinfos durchgehen
	for (int i = 0; i < buildingInfo->GetSize(); i++)
	{	
		// �berpr�fen, dass dieses Geb�ude nicht schon in der Liste der 
		// baubaren Geb�ude und Updates vorhanden ist
		BOOLEAN found = FALSE;
		for (int j = 0; j < m_BuildableWithoutAssemblylistCheck.GetSize(); j++)
		{
			// schauen ob es in der Liste der baubaren Geb�ude ist
			if (m_BuildableWithoutAssemblylistCheck.GetAt(j) == buildingInfo->GetAt(i).GetRunningNumber())
			{
				found = TRUE;	// ist schon in der Liste der baubaren Geb�ude
				break;
			}
		}
		
		// Haben wir noch nicht dieses Geb�ude in der Liste gefunden, so k�nnten wir es vielleicht bauen, deswegen
		// jetzt die ganzen Voraussetzungen pr�fen
		if (found == FALSE && CheckTech(&buildingInfo->GetAt(i), empire->GetResearch()) == TRUE)
			// Checken ob wir es durch die vorhanden Planeten bauen k�nnen
			if (CheckPlanet(&buildingInfo->GetAt(i),sector))
				// allg. Voraussetzungen des Geb�udes checken
				if (CheckGeneralConditions(&buildingInfo->GetAt(i),sector,globals,pMajor))
				{
					// Wenn wir hier sind sind wir mittlerweile schon bei Punkt (2.1) angekommen
					// �berpr�fen ob das Geb�ude einen Vorg�nger besitzt
					if (buildingInfo->GetAt(i).GetPredecessorID() != 0)	// Punkt 2.1.1
					{
						BOOLEAN found_predecessor = FALSE;
						// Schauen ob ein Geb�ude mit dieser ID im System schon steht
						for (int k = 0; k < m_Buildings.GetSize(); k++)
							if (m_Buildings.GetAt(k).GetRunningNumber() == buildingInfo->GetAt(i).GetPredecessorID())
							{
								found_predecessor = TRUE;
								break;
							}						
						// Haben wir einen Vorg�nger des Geb�udes im System stehen
						if (found_predecessor == TRUE)	// Punkt 2.1.1.1
						{
							// Geb�udeupgrade in die Liste nehmen
							m_BuildableWithoutAssemblylistCheck.Add(buildingInfo->GetAt(i).GetRunningNumber()*(-1));
						}
						// ansonsten haben wir keinen Vorg�nger des Geb�udes im System stehen // Punkt 2.1.1.2						
					}
					// Geb�ude hat keinen Vorg�nger
					// es geh�rt zu einer Rasse oder kann von jeder Rasse gebaut werden -> Sprich 0
					else if (buildingInfo->GetAt(i).GetOwnerOfBuilding() == 0 || buildingInfo->GetAt(i).GetOwnerOfBuilding() == pMajor->GetRaceBuildingNumber())	// Punkt 2.1.2
					{
						// schauen das sich nicht schon ein �quivalent in der Liste befindet, aber nur wenn das Geb�ude eine andere Rasse bauen
						// kann oder wenn es jeder bauen kann
						if (equivalents == TRUE)
						{
							for (int j = 0; j < m_BuildableWithoutAssemblylistCheck.GetSize(); j++)
							{
								short tid = m_BuildableWithoutAssemblylistCheck.GetAt(j);
								// Nur wenn es sich hierbei nicht um ein Upgrade handelt!
								if (tid > 0)
									// Nur die �quivalenz abfragen, zu der das Geb�ude "tid" geh�rt
									if (buildingInfo->GetAt(i).GetEquivalent(buildingInfo->GetAt(tid-1).GetOwnerOfBuilding()) == tid)
									{
										found = TRUE;
										break;
									}
							}							
							// Schauen, das sich nicht schon ein �quivalent bei den stehenden Geb�uden befindet
							if (found == FALSE)
							{
								minID = 0;
								for (int j = 0; j < m_Buildings.GetSize(); j++)
								{
									USHORT tid = m_Buildings.GetAt(j).GetRunningNumber();
									if (tid > minID)	// Weil geordnetes Feld
										if (buildingInfo->GetAt(i).GetEquivalent(buildingInfo->GetAt(tid-1).GetOwnerOfBuilding()) == tid)
										{
											found = TRUE;
											break;
										}
								}								
							}
						}
						// Gibt es schon ein �quivalenzgeb�ude in dem System, so k�nnen wir die nicht bauen
						if (found == TRUE)
							continue;
						
						// Nachfolgerchecks brauchen wir nur machen, wenn das Geb�ude auch updatebar ist!
						if (buildingInfo->GetAt(i).GetUpgradeable())
						{							
							// �berpr�fen ob ein Nachfolger des Geb�udes in der Liste der baubaren Geb�ude steht
							if (CheckFollower(buildingInfo,buildingInfo->GetAt(i).GetRunningNumber(),0,equivalents) == FALSE)
								// �berpr�fen ob ein Nachfolger des Geb�udes schon im System steht
								if (CheckFollower(buildingInfo,buildingInfo->GetAt(i).GetRunningNumber(),1,equivalents) == FALSE)
									m_BuildableWithoutAssemblylistCheck.Add(buildingInfo->GetAt(i).GetRunningNumber());							
						}
						// Ansonsten k�nnen wir es bauen.
						else
							m_BuildableWithoutAssemblylistCheck.Add(buildingInfo->GetAt(i).GetRunningNumber());
					}
				}
	}
	
	// Baulistencheck noch durchf�hren und fertig sind wir
	AssemblyListCheck(buildingInfo,globals);
	// Sortierung in der Reihenfolge nach ihrer ID
	c_arraysort<CArray<short,short>,short>(m_BuildableBuildings,sort_asc);
}

BOOLEAN CSystem::AssemblyListCheck(BuildingInfoArray* buildingInfo, CGlobalBuildings* globals, int ID)
{
	// zu allererst die Liste der baubaren Geb�ude und Updates mit der Liste der in dieser Runde baubaren Geb�ude
	// vor dem AssemblyListCheck f�llen
	for (int k = 0; k < m_BuildableBuildings.GetSize(); )
		m_BuildableBuildings.RemoveAt(k);
	for (int k = 0; k < m_BuildableUpdates.GetSize(); )
		m_BuildableUpdates.RemoveAt(k);
	m_BuildableBuildings.RemoveAll();
	m_BuildableUpdates.RemoveAll();
	m_BuildableBuildings.FreeExtra();
	m_BuildableUpdates.FreeExtra();
	for (int i = 0; i < m_BuildableWithoutAssemblylistCheck.GetSize(); i++)
	{
		// Unterscheidung zwischen Upgrades und normalen Geb�uden. Auch bei den Upgrades muss geschaut werden, dass dieses Geb�ude
		// noch nicht zu oft vorkommt
		int entry = m_BuildableWithoutAssemblylistCheck.GetAt(i);
		if (entry < 0)
			entry *= (-1);

		// wenn dieses Geb�ude nur X mal pro Imperium vorkommen darf, dann darf dieses Geb�ude nicht mehr als X mal
		// als globales Geb�ude vorkommen
		BOOLEAN found = FALSE;
		if (buildingInfo->GetAt(entry - 1).GetMaxInEmpire().Number > 0)
		{
			short ID = buildingInfo->GetAt(entry - 1).GetMaxInEmpire().RunningNumber;
			short number = 0;
			for (int j = 0; j < globals->GetGlobalBuildings()->GetSize(); j++)
				if (ID == globals->GetGlobalBuildings()->GetAt(j))
				{
					number++;
					if (number >= buildingInfo->GetAt(entry - 1).GetMaxInEmpire().Number)
					{
						found = TRUE;
						break;
					}
				}
		}
		// wir haben das Geb�ude nicht in der Liste der globalen Geb�ude gefunden
		if (found == FALSE)
		{
			if (m_BuildableWithoutAssemblylistCheck.GetAt(i) > 0)
				m_BuildableBuildings.Add(entry);
			else
				m_BuildableUpdates.Add(entry);
		}
	}
	USHORT inSystem = 0;
	// nach Updates in der Bauliste suchen. Wenn wir ein Update in der Bauliste gefunden haben, so
	// m�ssen wir dieses und all deren Vorg�nger aus der Liste der baubaren Geb�ude entfernen
	for (int i = 0; i < ALE; i++)
	{
		if (m_AssemblyList.GetAssemblyListEntry(i) < 0)
		{
			// Vorg�nger des Updates aus der Liste entfernen
			for (int t = 0; t < m_BuildableBuildings.GetSize(); t++)
			{
				USHORT pre = buildingInfo->GetAt(m_AssemblyList.GetAssemblyListEntry(i)*(-1)-1).GetPredecessorID();
				if (pre == m_BuildableBuildings.GetAt(t))
				{
					m_BuildableBuildings.RemoveAt(t--);
					// Haben wir einen Vorg�nger entfernt, dann auch alle Updates des Vorg�ngers, die in der Liste 
					// der Updates sind entfernen

					// HIER FIXEN: Auch wenn ein Vorg�nger nicht mehr baubar war m�ssen wir das Update aus der Bauliste
					// entfernen!!!
					/*	
					for (int s = 0; s < m_BuildableUpdates.GetSize(); s++)
					{
						USHORT pre2 = buildingInfo->GetAt(m_BuildableUpdates.GetAt(s)-1).GetPredecessorID();
						if (pre2 == pre)
							m_BuildableUpdates.RemoveAt(s--);
					}*/
				}
				for (int s = 0; s < m_BuildableUpdates.GetSize(); s++)
				{
					USHORT pre2 = buildingInfo->GetAt(m_BuildableUpdates.GetAt(s)-1).GetPredecessorID();
					if (pre2 == pre)
						m_BuildableUpdates.RemoveAt(s--);
				}
			}
		}
		else if (m_AssemblyList.GetAssemblyListEntry(i) < 10000 && m_AssemblyList.GetAssemblyListEntry(i) != 0)
		{
			// hier die maximal baubaren Geb�ude pro Imperium checken
			// Die Geb�ude befinden sich auch schon in der GlobalBuildings Variable, wenn sie schon in der
			// Bauliste sind
			if (buildingInfo->GetAt(m_AssemblyList.GetAssemblyListEntry(i)-1).GetMaxInEmpire().Number > 0)
			{
				USHORT n = 0;
				for (int j = 0; j < globals->GetGlobalBuildings()->GetSize(); j++)
				{
					if (globals->GetGlobalBuildings()->GetAt(j) == buildingInfo->GetAt(m_AssemblyList.GetAssemblyListEntry(i)-1).GetMaxInEmpire().RunningNumber)
						n++;
					if (n >= buildingInfo->GetAt(m_AssemblyList.GetAssemblyListEntry(i)-1).GetMaxInEmpire().Number)
						break;
				}
				// Haben wir dieses Geb�ude schon "oft genug" stehen und in der Bauliste der Systeme, dann aus der
				// Bauliste nehmen
				if (n >= buildingInfo->GetAt(m_AssemblyList.GetAssemblyListEntry(i)-1).GetMaxInEmpire().Number)
					for (int t = 0; t < m_BuildableBuildings.GetSize(); t++)
						if (m_AssemblyList.GetAssemblyListEntry(i) == m_BuildableBuildings.GetAt(t))
						{
							m_BuildableBuildings.RemoveAt(t--);
							break;
						}
			}
			// hier die maximal baubaren Geb�ude pro System checken
			if (buildingInfo->GetAt(m_AssemblyList.GetAssemblyListEntry(i)-1).GetMaxInSystem().Number > 0)
			{
				USHORT n = 0;
				for (int j = 0; j < m_Buildings.GetSize(); j++)
				{
					if (m_Buildings.GetAt(j).GetRunningNumber() == buildingInfo->GetAt(m_AssemblyList.GetAssemblyListEntry(i)-1).GetMaxInSystem().RunningNumber)
						n++;
					if (n >= buildingInfo->GetAt(m_AssemblyList.GetAssemblyListEntry(i)-1).GetMaxInSystem().Number)
						break;
				}
				// Bauliste durchgehen und wenn dieses Gebe�ude in dieser vorkommt, dann auch mitz�hlen
				for (int j = 0; j < ALE; j++)
					if (m_AssemblyList.GetAssemblyListEntry(i) == m_AssemblyList.GetAssemblyListEntry(j))
						if (m_AssemblyList.GetAssemblyListEntry(j) > 0 && m_AssemblyList.GetAssemblyListEntry(j) < 10000)
							if (m_AssemblyList.GetAssemblyListEntry(j) == buildingInfo->GetAt(m_AssemblyList.GetAssemblyListEntry(j)-1).GetMaxInSystem().RunningNumber)
								n++;
				// Haben wir dieses Geb�ude schon "oft genug" stehen und in der Bauliste des Systems, dann aus der
				// Bauliste nehmen
				if (n >= buildingInfo->GetAt(m_AssemblyList.GetAssemblyListEntry(i)-1).GetMaxInSystem().Number)
					for (int t = 0; t < m_BuildableBuildings.GetSize(); t++)
						if (m_AssemblyList.GetAssemblyListEntry(i) == m_BuildableBuildings.GetAt(t))
						{
							m_BuildableBuildings.RemoveAt(t--);
							break;
						}
			}
		}
	}
	return FALSE;
}

// Funktion berechnet die baubaren Schiffe in dem System.
void CSystem::CalculateBuildableShips(CBotf2Doc* pDoc, const CPoint& p)
{
	ASSERT(pDoc);

	for (int k = 0; k < m_BuildableShips.GetSize(); )
		m_BuildableShips.RemoveAt(k);
	m_BuildableShips.RemoveAll();
	m_BuildableShips.FreeExtra();
	// Hier jetzt schauen ob wir eine Werft haben und anhand der gr��e der Werft k�nnen wir bestimmte
	// Schiffstypen bauen
	CMajor* pMajor = dynamic_cast<CMajor*>(pDoc->GetRaceCtrl()->GetRace(m_sOwnerOfSystem));
	if (!pMajor)
		return;
	// Array mit baubaren Minorraceschiffen f�llen
	int nMinorShipNumber = -1;
	if (pDoc->GetSector(p).GetMinorRace())
	{
		CMinor* pMinor = pDoc->GetRaceCtrl()->GetMinorRace(pDoc->GetSector(p).GetName());
		if (pMinor)
			nMinorShipNumber = pMinor->GetRaceShipNumber();
	}

	if (m_Production.GetShipYard())
	{
		CArray<USHORT> obsoleteClasses;
		for (int i = 0; i < pDoc->GetShipInfos()->GetSize(); i++)
		{
			CShipInfo* pShipInfo = &(pDoc->GetShipInfos()->GetAt(i));
			// Au�enposten und Sternenbasen k�nnen wir nat�rlich nicht hier bauen
			if ((pShipInfo->GetRace() == pMajor->GetRaceShipNumber() || pShipInfo->GetRace() == nMinorShipNumber)
				&& pShipInfo->GetShipType() != OUTPOST && pShipInfo->GetShipType() != STARBASE)
			{
				CResearch* pResearch = pMajor->GetEmpire()->GetResearch();

				BOOLEAN buildable = TRUE;
				// zuerstmal die Forschungsstufen checken
				if (pResearch->GetBioTech() <  pShipInfo->GetBioTech())
					buildable = FALSE;
				else if (pResearch->GetEnergyTech() < pShipInfo->GetEnergyTech())
					buildable = FALSE;
				else if (pResearch->GetCompTech() < pShipInfo->GetComputerTech())
					buildable = FALSE;
				else if (pResearch->GetPropulsionTech() < pShipInfo->GetPropulsionTech())
					buildable = FALSE;
				else if (pResearch->GetConstructionTech() < pShipInfo->GetConstructionTech())
					buildable = FALSE;
				else if (pResearch->GetWeaponTech() < pShipInfo->GetWeaponTech())
					buildable = FALSE;
				// Wenn durch dieses Schiff ein anderes Schiff veraltet ist (nur wenn es technologisch baubar ist)
				if (pShipInfo->GetObsoleteShipClass() != "" && buildable == TRUE)
				{
					for (int j = 0; j < pDoc->GetShipInfos()->GetSize(); j++)
						if (pDoc->GetShipInfos()->GetAt(j).GetShipClass() == pShipInfo->GetObsoleteShipClass())
						{
							obsoleteClasses.Add(pDoc->GetShipInfos()->GetAt(j).GetID());
							break;
						}
				}				
				// Wenn das Schiff nur in einem bestimmten System gebaut werden kann, dann hier checken
				if (!pShipInfo->GetOnlyInSystem().IsEmpty())
				{
					if (pShipInfo->GetOnlyInSystem() != pDoc->GetSector(p).GetName())
						buildable = FALSE;					
					// der Besitzer der Schiffsklasse wird auf den Besitzer des Schiffes gesetzt. Somit kann
					// eine Majorrace dann auch die Schiffe der Minorrace bauen
					else
						pShipInfo->SetRace(pMajor->GetRaceShipNumber());
				}
				else if (pShipInfo->GetRace() == nMinorShipNumber)
					buildable = FALSE;

				if (pShipInfo->GetShipSize() > m_Production.GetMaxBuildableShipSize())
						buildable = FALSE;
				if (buildable == TRUE)
					m_BuildableShips.Add(pShipInfo->GetID());
			}
		}
		// Nochmal die jetzt baubare Schiffsliste durchgehen und schauen, ob manche Schiffe veraltet sind und somit
		// nicht mehr baubar sind
		for (int i = 0; i < obsoleteClasses.GetSize(); i++)
		{
			for (int j = 0; j < m_BuildableShips.GetSize(); j++)
				if (m_BuildableShips.GetAt(j) == obsoleteClasses.GetAt(i))
				{
					m_BuildableShips.RemoveAt(j);
					break;
				}
		}
	}
}

// Diese Funktion berechnet die baubaren Truppen in diesem System
void CSystem::CalculateBuildableTroops(const CArray<CTroopInfo>* troopInfos, const CResearch *research)
{
	for (int k = 0; k < m_BuildableTroops.GetSize(); )
		m_BuildableTroops.RemoveAt(k);
	m_BuildableTroops.RemoveAll();
	m_BuildableTroops.FreeExtra();
	// Hier jetzt schauen ob wir eine Kaserne haben
	if (m_Production.GetBarrack())
		for (int i = 0; i < troopInfos->GetSize(); i++)
			if (troopInfos->GetAt(i).GetOwner() == m_sOwnerOfSystem)
			{
				BOOLEAN buildable = TRUE;
				// zuerstmal die Forschungsstufen checken
				if (research->GetBioTech() < troopInfos->GetAt(i).GetNeededTechlevel(0))
					buildable = FALSE;
				else if (research->GetEnergyTech() < troopInfos->GetAt(i).GetNeededTechlevel(1))
					buildable = FALSE;
				else if (research->GetCompTech() < troopInfos->GetAt(i).GetNeededTechlevel(2))
					buildable = FALSE;
				else if (research->GetPropulsionTech() < troopInfos->GetAt(i).GetNeededTechlevel(3))
					buildable = FALSE;
				else if (research->GetConstructionTech() < troopInfos->GetAt(i).GetNeededTechlevel(4))
					buildable = FALSE;
				else if (research->GetWeaponTech() < troopInfos->GetAt(i).GetNeededTechlevel(5))
					buildable = FALSE;
				if (buildable == TRUE)
					m_BuildableTroops.Add(troopInfos->GetAt(i).GetID());
			}
}

// Funktion berechnet die Anzahl aller Farmen, Bauh�fe usw., also alle Geb�ude die Arbeiter ben�tigen.
// Sie mu� am Rundenanfang vor CalculateVariables() aufgerufen werden und sortiert gleichzeitig das
// CArray m_Buildings nach der RunningNumber.
// In der Doc-Klasse nach der Funktion DestroyBuildings() und zu Beginn aufrufen!
void CSystem::CalculateNumberOfWorkbuildings(BuildingInfoArray *buildingInfos)
{
	// Sortierung
	c_arraysort<CArray<CBuilding>,CBuilding>(m_Buildings,sort_asc);

	// Berechnung der Geb�ude
	m_iFoodBuildings = 0;
	m_iIndustryBuildings = 0;
	m_iEnergyBuildings = 0;
	m_iSecurityBuildings = 0;
	m_iResearchBuildings = 0;
	m_iTitanMines = 0;
	m_iDeuteriumMines = 0;
	m_iDuraniumMines = 0;
	m_iIridiumMines = 0;
	m_iCrystalMines = 0;
	for (int i = 0; i < m_Buildings.GetSize(); i++)
	{
		const CBuildingInfo* buildingInfo = &buildingInfos->GetAt(m_Buildings.GetAt(i).GetRunningNumber() - 1);
		
		if (buildingInfo->GetWorker() == TRUE)
		{
			if (buildingInfo->GetFoodProd() > 0)
				m_iFoodBuildings++;
			else if (buildingInfo->GetIPProd() > 0)
				m_iIndustryBuildings++;
			else if (buildingInfo->GetEnergyProd() > 0)
				m_iEnergyBuildings++;
			else if (buildingInfo->GetSPProd() > 0)
				m_iSecurityBuildings++;
			else if (buildingInfo->GetFPProd() > 0)
				m_iResearchBuildings++;
			else if (buildingInfo->GetTitanProd() > 0)
				m_iTitanMines++;
			else if (buildingInfo->GetDeuteriumProd() > 0)
				m_iDeuteriumMines++;
			else if (buildingInfo->GetDuraniumProd() > 0)
				m_iDuraniumMines++;
			else if (buildingInfo->GetCrystalProd() > 0)
				m_iCrystalMines++;
			else if (buildingInfo->GetIridiumProd() > 0)
				m_iIridiumMines++;
		}
	}
}

// Funktion berechnet die imperiumweite Moralproduktion, welche aus diesem System generiert wird.
void CSystem::CalculateEmpireWideMoralProd(BuildingInfoArray *buildingInfos)
{
	for (int i = 0; i < m_Buildings.GetSize(); i++)
		if (m_Buildings.GetAt(i).GetIsBuildingOnline())
		{
			const CBuildingInfo *buildingInfo = &buildingInfos->GetAt(m_Buildings.GetAt(i).GetRunningNumber() - 1);
			m_Production.m_iMoralProdEmpireWide[m_sOwnerOfSystem] += buildingInfo->GetMoralProdEmpire();
		}
}

// Funktion setzt das letzte Geb�ude, welches gebaut wurde online, sofern dies m�glich ist.
int CSystem::SetNewBuildingOnline(BuildingInfoArray *buildingInfos)
{
	unsigned short CheckValue = 0;
	unsigned short lastBuilding = m_Buildings.GetUpperBound();

	const CBuildingInfo *buildingInfo = &buildingInfos->GetAt(m_Buildings.GetAt(lastBuilding).GetRunningNumber() - 1);

	if (m_Workers.GetWorker(11) > 0 && buildingInfo->GetWorker() == TRUE)
	{
		if (buildingInfo->GetFoodProd() > 0)
			m_Workers.InkrementWorker(0);
		if (buildingInfo->GetIPProd() > 0)
			m_Workers.InkrementWorker(1);
		if (buildingInfo->GetEnergyProd() > 0)
			m_Workers.InkrementWorker(2);
		if (buildingInfo->GetSPProd() > 0)
			m_Workers.InkrementWorker(3);
		if (buildingInfo->GetFPProd() > 0)
			m_Workers.InkrementWorker(4);
		if (buildingInfo->GetTitanProd() > 0)
			m_Workers.InkrementWorker(5);
		if (buildingInfo->GetDeuteriumProd() > 0)
			m_Workers.InkrementWorker(6);
		if (buildingInfo->GetDuraniumProd() > 0)
			m_Workers.InkrementWorker(7);
		if (buildingInfo->GetCrystalProd() > 0)
			m_Workers.InkrementWorker(8);
		if (buildingInfo->GetIridiumProd() > 0)
			m_Workers.InkrementWorker(9);
	}
	else if (m_Buildings.GetAt(lastBuilding).GetIsBuildingOnline())
		CheckValue = 0;
	else
		CheckValue = 1;
	if (buildingInfo->GetNeededEnergy() > 0)
	{
		if (buildingInfo->GetNeededEnergy() <= m_Production.m_iEnergyProd)
		{
			m_Buildings.ElementAt(lastBuilding).SetIsBuildingOnline(TRUE);
			CheckValue = 0;
		}
		else
			CheckValue = 2;
	}
	return CheckValue;	// Wenn CheckValue == 0, dann konnte das Geb�ude in Betrieb genommen werden
						// Wenn CheckValue == 1, dann fehlen Arbeiter
						// Wenn CheckValue == 2, dann fehlt Energie im System
}

// Funktion �berpr�ft Geb�ude die Energie ben�tigen und schaltet diese gegebenfalls ab,
// falls zuwenig Energie im System vorhanden ist.
// Diese Funktion aufrufen, bevor wir CalculateVariables() usw. aufrufen, weil wir ja die b�sen Onlinegeb�ude vorher
// ausschalten wollen.
int CSystem::CheckEnergyBuildings(BuildingInfoArray *buildingInfos)
{
	BOOLEAN CheckValue = 0;
	unsigned short NumberOfBuildings = m_Buildings.GetSize();
	for (int i = 0; i < NumberOfBuildings; i++)
	{
		const CBuildingInfo *buildingInfo = &buildingInfos->GetAt(m_Buildings.GetAt(i).GetRunningNumber() - 1);

		if (m_Production.m_iEnergyProd < 0 && buildingInfo->GetNeededEnergy() > 0)
		{
			m_Buildings.ElementAt(i).SetIsBuildingOnline(FALSE);
			m_Production.m_iEnergyProd += buildingInfo->GetNeededEnergy();
			CheckValue = 1;
		}
	}
	// Wenn CheckValue == 0, dann genug Energie vorhanden, ansonsten wurde mindst. ein Geb�ude abgeschaltet
	return CheckValue;
}

// Funktion baut die Geb�ude der Minorrace, wenn wir eine Mitgliedschaft mit dieser erreicht haben.
void CSystem::BuildBuildingsForMinorRace(CSector* sector, BuildingInfoArray* buildingInfo, USHORT averageTechlevel, const CMinor* pMinor)
{
	if (m_Buildings.GetSize() < 5)
	{
		// in exist[.] steht dann, ob wir einen Rohstoff abbauen k�nnen, wenn ja, dann k�nnen wir auch das Geb�ude bauen
		BOOLEAN exist[DILITHIUM + 1] = {0};
		sector->GetAvailableResources(exist, true);

		// Schauen, welche Geb�udestufe ungef�hr in dem System steht
		// Jetzt nach der Fortschrittlichkeit der kleinen Rasse gehen
		short level = 0;
		switch (pMinor->GetTechnologicalProgress())
		{
		case 0:	// sehr r�ckst�ndig
			{
				USHORT temp = rand()%3+1;
				level = averageTechlevel-temp;				
				break;
			}
		case 1:	// r�ckst�ndig
			{
				USHORT temp = rand()%2+1;
				level = averageTechlevel-temp;				
				break;
			}
		case 2:	// normal fortschrittlich
			{
				USHORT temp = rand()%3;
				level = averageTechlevel+1-temp;
				break;
			}
		case 3:	// fortschrittlich
			{
				USHORT temp = rand()%2+1;
				level = averageTechlevel+temp;
				break;
			}
		case 4:	// sehr fortschrittlich
			{
				USHORT temp = rand()%3+1;
				level = averageTechlevel+temp;
				break;
			}
		}

		if (level < 0)
			level = 0;
		else if (level > NoTL)
			level = NoTL;
		// Researchlevels f�r Minor zusammenbauen
		BYTE researchLevels[6] = {level, level, level, level, level, level};

		// f�r den Algorithmus unten (muss gr��er 0 sein um einmal in die Schleife zu gehen)
		level++;

		// die g�ltige Eigenschaft der Minor festlegen
		vector<int> vProperty;
		if (pMinor->IsRaceProperty(FINANCIAL))
			vProperty.push_back(FINANCIAL);
		if (pMinor->IsRaceProperty(WARLIKE))
			vProperty.push_back(WARLIKE);
		if (pMinor->IsRaceProperty(AGRARIAN))
			vProperty.push_back(AGRARIAN);
		if (pMinor->IsRaceProperty(INDUSTRIAL))
			vProperty.push_back(INDUSTRIAL);
		if (pMinor->IsRaceProperty(SECRET))
			vProperty.push_back(SECRET);
		if (pMinor->IsRaceProperty(SCIENTIFIC))
			vProperty.push_back(SCIENTIFIC);
		if (pMinor->IsRaceProperty(PRODUCER))
			vProperty.push_back(PRODUCER);
		if (pMinor->IsRaceProperty(PACIFIST))
			vProperty.push_back(PACIFIST);
		if (pMinor->IsRaceProperty(SNEAKY))
			vProperty.push_back(SNEAKY);
		if (pMinor->IsRaceProperty(SOLOING))
			vProperty.push_back(SOLOING);
		if (pMinor->IsRaceProperty(HOSTILE))
			vProperty.push_back(HOSTILE);

		if (vProperty.size() == 0)
			vProperty.push_back(NOTHING_SPECIAL);
		
		int nProperty = vProperty[rand()%vProperty.size()];
		// Jetzt die Geb�ude bauen und auch das System nach der Art der MinorRace angleichen
		// z.B. bei einer landwirtschaftlichen Rasse viele Farmen
		// Die Rassenartzuweisung m�sste man vielleicht noch dynamisch machen. Bis jetzt wird immer
		// davon ausgegangen, dass z.B. die Major auf dem Cardassianerplatz bestimmte Eigenschaften vertritt.
		// Dies kann sich durch Modifikationen aber ver�ndern.
		// ALPHA5 -> fromRace m�sste noch fest in der Minorrace stehen, daher ist folgender Block vorerst entfernt
		BYTE fromRace = rand()%6 + 1; // liegt erstmal zwischen human und dominion
		/*
		switch (pMinor->GetKind())
		{
		case FINANCIAL: fromRace = FERENGI; break;
		case WARLIKE: fromRace = KLINGON; break;
		case INDUSTRIAL: fromRace = CARDASSIAN; break;
		case SECRET: fromRace = ROMULAN; break;
		case PRODUCER: fromRace = KLINGON; break;
		case PACIFIST: fromRace = FERENGI; break;
		case SNEAKY: fromRace = CARDASSIAN; break;
		default: fromRace = HUMAN;
		}
		*/
		
		USHORT runningNumber[10] = {0,0,0,0,0,0,0,0,0,0};
		for (int i = 0; i < buildingInfo->GetSize(); i++)
			if (fromRace == buildingInfo->GetAt(i).GetOwnerOfBuilding())
			{
				if (!buildingInfo->GetAt(i).IsBuildingBuildableNow(researchLevels))
					continue;
				
				if (buildingInfo->GetAt(i).GetFoodProd() > 0 && buildingInfo->GetAt(i).GetWorker() == TRUE)
				{				
					runningNumber[0] = buildingInfo->GetAt(i).GetRunningNumber();
				}
				else if (buildingInfo->GetAt(i).GetIPProd() > 0 && buildingInfo->GetAt(i).GetWorker() == TRUE)
				{
					runningNumber[1] = buildingInfo->GetAt(i).GetRunningNumber();
				}
				else if (buildingInfo->GetAt(i).GetEnergyProd() > 0 && buildingInfo->GetAt(i).GetWorker() == TRUE)
				{
					runningNumber[2] = buildingInfo->GetAt(i).GetRunningNumber();
				}
				else if (buildingInfo->GetAt(i).GetSPProd() > 0 && buildingInfo->GetAt(i).GetWorker() == TRUE)
				{
					runningNumber[3] = buildingInfo->GetAt(i).GetRunningNumber();
				}
				else if (buildingInfo->GetAt(i).GetFPProd() > 0 && buildingInfo->GetAt(i).GetWorker() == TRUE)
				{
					runningNumber[4] = buildingInfo->GetAt(i).GetRunningNumber();
				}
				else if (buildingInfo->GetAt(i).GetTitanProd() > 0 && buildingInfo->GetAt(i).GetWorker() == TRUE && exist[TITAN] == TRUE)
				{
					runningNumber[5] = buildingInfo->GetAt(i).GetRunningNumber();
				}
				else if (buildingInfo->GetAt(i).GetDeuteriumProd() > 0 && buildingInfo->GetAt(i).GetWorker() == TRUE && exist[DEUTERIUM] == TRUE)
				{
					runningNumber[6] = buildingInfo->GetAt(i).GetRunningNumber();
				}
				else if (buildingInfo->GetAt(i).GetDuraniumProd() > 0 && buildingInfo->GetAt(i).GetWorker() == TRUE && exist[DURANIUM] == TRUE)
				{
					runningNumber[7] = buildingInfo->GetAt(i).GetRunningNumber();
				}
				else if (buildingInfo->GetAt(i).GetCrystalProd() > 0 && buildingInfo->GetAt(i).GetWorker() == TRUE && exist[CRYSTAL] == TRUE)
				{
					runningNumber[8] = buildingInfo->GetAt(i).GetRunningNumber();
				}
				else if (buildingInfo->GetAt(i).GetIridiumProd() > 0 && buildingInfo->GetAt(i).GetWorker() == TRUE && exist[IRIDIUM] == TRUE)
				{
					runningNumber[9] = buildingInfo->GetAt(i).GetRunningNumber();
				}
			}			
		
		// Jetzt haben wir die ganzen RunningNumbers der baubaren Geb�ude, nun gucken welche Art von Rasse wir haben
		// und danach zuf�llig die Geb�ude bauen
		USHORT numberOfBuildings = 0;
		numberOfBuildings += (int)m_dHabitants+1 + rand()%5;
		int foodBuildings = 0;
		while (numberOfBuildings > 0)
		{
			USHORT build = rand()%10;
			
			USHORT temp;
			switch (nProperty)
			{
			// Wenn die Rasse eine landwirtschaftliche Rasse ist
			case AGRARIAN:
				{
					if (build >= 5)
					{
						temp = rand()%3;
						if (temp == 0)
							build = 0;
					}
					else if (build == 4 || build == 2)
					{
						temp = rand()%2;
						if (temp == 0)
							build = 0;
					}
					else if (build == 3)
						build = 0;
					break;
				}
			case INDUSTRIAL:
				{
					if (build >= 5)
					{
						temp = rand()%3;
						if (temp == 0)
							build = 1;
					}
					else if (build == 4 || build == 2)
					{
						temp = rand()%2;
						if (temp == 0)
							build = 1;
					}
					else if (build == 3)
					{
						temp = rand()%2;
						if (temp == 0)
							build = 0;
						else
							build = 1;
					}
					break;
				}
			case SECRET:
				{
					if (build >= 5)
					{
						temp = rand()%3;
						if (temp == 0)
							build = 3;
					}
					else if (build == 4)
					{
						temp = rand()%4;
						if (temp == 0)
							build = 3;
					}
					else if (build == 2)
					{
						temp = rand()%4;
						if (temp == 0)
							build = 0;
						else if (temp == 1)
							build = 3;
					}
					break;
				}
			case SCIENTIFIC:
				{
					if (build >= 5)
					{
						temp = rand()%3;
						if (temp == 0)
							build = 4;
					}
					else if (build == 1)
					{
						temp = rand()%6;
						if (temp == 0)
							build = 4;
					}
					else if (build == 2)
					{
						temp = rand()%4;
						if (temp == 0)
							build = 4;
					}
					else if (build == 3)
					{
						temp = rand()%2;
						if (temp == 0)
							build = 0;
						else
							build = 4;
					}
					break;
				}
			case PRODUCER:
				{
					if (build < 5 && build > 1)
					{
						temp = rand()%5;
						if (temp == 0)
							build = 5;
						else if (temp == 1)
							build = 6;
						else if (temp == 2)
							build = 7;
						else if (temp == 3)
							build = 8;
						else if (temp == 4)
							build = 9;
					}
					break;
				}
			case SNEAKY:
				break;
			case SOLOING:
				break;
			case NOTHING_SPECIAL:
				break;
				// Pazifisten, kriegerische Rassen sowie Finanzrassen bekommen nur weniger Geheimdienst drauf
			default:
				{
					if (build == 2 || build == 3)
					{
						if (rand()%2 == NULL)
							build = 1;
					}					
				}
			}
			
			if (runningNumber[build] != 0)
			{
				if (build == 0)
					foodBuildings++;
				CBuilding building(runningNumber[build]);
				building.SetIsBuildingOnline(buildingInfo->GetAt(runningNumber[build] - 1).GetAllwaysOnline());
				m_Buildings.Add(building);
				numberOfBuildings--;
			}
		}
		// wenn zuwenig Nahrungsgeb�ude vorhanden sind, so werden hier noch ein paar mehr hinzugef�gt
		while (runningNumber[0] != 0 && foodBuildings < m_dHabitants/4)
		{
			CBuilding building(runningNumber[0]);
			building.SetIsBuildingOnline(buildingInfo->GetAt(runningNumber[0] - 1).GetAllwaysOnline());
			m_Buildings.Add(building);
			foodBuildings++;
		}

		// Nun Lager f�llen, sp�ter vlt. noch �ndern nach der Rassenart (bei landwirtschaftl. mehr Nahrung im Lager)
		// Ressourcen sind abh�ngig von der Anzahl der jeweiligen Geb�ude und dem technologischen Fortschritt der
		// Minorrace
		this->CalculateNumberOfWorkbuildings(buildingInfo);
		this->SetFoodStore(this->GetFoodStore() + rand()%(this->GetNumberOfWorkbuildings(0,0,NULL) * (pMinor->GetTechnologicalProgress() + 1) * 100 + 1));
		for (int res = TITAN; res <= IRIDIUM; res++)
		{
			int resAdd = rand()%(this->GetNumberOfWorkbuildings(res + 5, 0, NULL) * (pMinor->GetTechnologicalProgress() + 1) * 100 + 1);
			this->SetRessourceStore(res, resAdd);			
		}
	}
}

// Funktion berechnet und baut die Startgeb�ude in einem System, nachdem wir einen Planeten
// in diesem kolonisiert haben.
void CSystem::BuildBuildingsAfterColonization(CSector *sector, BuildingInfoArray *buildingInfo, USHORT colonizationPoints)
{
	CBotf2Doc* pDoc = ((CBotf2App*)AfxGetApp())->GetDocument();
	ASSERT(pDoc);

	CMajor* pMajor = dynamic_cast<CMajor*>(pDoc->GetRaceCtrl()->GetRace(sector->GetOwnerOfSector()));
	ASSERT(pMajor);

	BYTE byRaceBuildingID = pMajor->GetRaceBuildingNumber();

	BYTE researchLevels[6] =
		{
			pMajor->GetEmpire()->GetResearch()->GetBioTech(),
			pMajor->GetEmpire()->GetResearch()->GetEnergyTech(),
			pMajor->GetEmpire()->GetResearch()->GetCompTech(),
			pMajor->GetEmpire()->GetResearch()->GetPropulsionTech(),
			pMajor->GetEmpire()->GetResearch()->GetConstructionTech(),
			pMajor->GetEmpire()->GetResearch()->GetWeaponTech()
		};

	// in exist[.] steht dann, ob wir einen Rohstoff abbauen k�nnen, wenn ja, dann k�nnen wir auch das Geb�ude bauen
	BOOLEAN exist[DILITHIUM + 1] = {0};
	sector->GetAvailableResources(exist, true);

	USHORT start = 0;
	for (int i = 0; i < buildingInfo->GetSize(); i++)
		if (buildingInfo->GetAt(i).GetOwnerOfBuilding() == byRaceBuildingID)
		{
			start = i;
			break;
		}
	
	short counter[10] = {0};
	USHORT runningNumber[10] = {0,0,0,0,0,0,0,0,0,0};
	for (int i = start; i < buildingInfo->GetSize(); i++)
	{
		// Abbruchbedingung, wenn das Geb�ude eh nicht mehr zu den Geb�uden des Sektorbesitzers geh�ren.
		// Daf�r muss aber die Geb�udeliste geordnet nach den Besitzern vorliegen (das sie aktuell auch ist)
		if (buildingInfo->GetAt(i).GetOwnerOfBuilding() != byRaceBuildingID)
			break;
		// wenn das Geb�ude nicht gebaut werden kann, dann mit dem n�chsten weitermachen
		if (!buildingInfo->GetAt(i).IsBuildingBuildableNow(researchLevels))
			continue;

		if (buildingInfo->GetAt(i).GetFoodProd() > 0 && buildingInfo->GetAt(i).GetWorker() == TRUE && counter[0] < colonizationPoints)
		{
			counter[0]++;
			runningNumber[0] = buildingInfo->GetAt(i).GetRunningNumber();

		}
		else if (buildingInfo->GetAt(i).GetIPProd() > 0 && buildingInfo->GetAt(i).GetWorker() == TRUE && counter[1] < colonizationPoints)
		{
			counter[1]++;
			runningNumber[1] = buildingInfo->GetAt(i).GetRunningNumber();
		}
		else if (buildingInfo->GetAt(i).GetEnergyProd() > 0 && buildingInfo->GetAt(i).GetWorker() == TRUE && counter[2] < colonizationPoints)
		{
			counter[2]++;
			runningNumber[2] = buildingInfo->GetAt(i).GetRunningNumber();
		}
		else if (buildingInfo->GetAt(i).GetSPProd() > 0 && buildingInfo->GetAt(i).GetWorker() == TRUE && counter[3] < colonizationPoints)
		{
			counter[3]++;
			runningNumber[3] = buildingInfo->GetAt(i).GetRunningNumber();
		}
		else if (buildingInfo->GetAt(i).GetFPProd() > 0 && buildingInfo->GetAt(i).GetWorker() == TRUE && counter[4] < colonizationPoints)
		{
			counter[4]++;
			runningNumber[4] = buildingInfo->GetAt(i).GetRunningNumber();
		}
		else if (buildingInfo->GetAt(i).GetTitanProd() > 0 && buildingInfo->GetAt(i).GetWorker() == TRUE && counter[5] < colonizationPoints && exist[0] == TRUE)
		{
			counter[5]++;
			runningNumber[5] = buildingInfo->GetAt(i).GetRunningNumber();
		}
		else if (buildingInfo->GetAt(i).GetDeuteriumProd() > 0 && buildingInfo->GetAt(i).GetWorker() == TRUE && counter[6] < colonizationPoints && exist[1] == TRUE)
		{
			counter[6]++;
			runningNumber[6] = buildingInfo->GetAt(i).GetRunningNumber();
		}
		else if (buildingInfo->GetAt(i).GetDuraniumProd() > 0 && buildingInfo->GetAt(i).GetWorker() == TRUE && counter[7] < colonizationPoints && exist[2] == TRUE)
		{
			counter[7]++;
			runningNumber[7] = buildingInfo->GetAt(i).GetRunningNumber();
		}
		else if (buildingInfo->GetAt(i).GetCrystalProd() > 0 && buildingInfo->GetAt(i).GetWorker() == TRUE && counter[8] < colonizationPoints && exist[3] == TRUE)
		{
			counter[8]++;
			runningNumber[8] = buildingInfo->GetAt(i).GetRunningNumber();
		}
		else if (buildingInfo->GetAt(i).GetIridiumProd() > 0 && buildingInfo->GetAt(i).GetWorker() == TRUE && counter[9] < colonizationPoints && exist[4] == TRUE)
		{
			counter[9]++;
			runningNumber[9] = buildingInfo->GetAt(i).GetRunningNumber();
		}		
	}
	// wenn schon Geb�ude eines Typs stehen, dann d�rfen keine des gleichen Typ zus�tzlich gebaut werden. Z.B. wenn
	// schon Typ 4 Automatikfarmen stehen darf das Kolonieschiff nicht auch noch Primitive Farmen bauen.
	this->CalculateNumberOfWorkbuildings(buildingInfo);
	for (int build = 0; build < 10; build++)
	{
		if (this->GetNumberOfWorkbuildings(build, 0, NULL) > NULL)
			runningNumber[build] = 0;
		if (runningNumber[build] != 0)
		{
			// Geb�ude anlegen, umso besser das Kolonieschiff ist, desto mehr Geb�ude stehen dann auf dem System
			for (int i = 0; i < colonizationPoints*2; i++)
			{
				CBuilding building(runningNumber[build]);
				building.SetIsBuildingOnline(buildingInfo->GetAt(runningNumber[build]-1).GetAllwaysOnline());				
				m_Buildings.Add(building);
			}
			// Geb�ude mit Arbeitern besetzen
			this->SetWorker(build,colonizationPoints*2,2);
		}
	}
	this->CalculateNumberOfWorkbuildings(buildingInfo);

	// Wenn das System nach einer Bombardierung komplett ausgel�scht wurde und von uns wieder neu kolonisiert wurde,
	// so m�ssen die mindst. immer baubaren Geb�ude gel�scht werden. Dies wird hier jedesmal gemacht, wenn man
	// eine neue Kolonie gr�ndet
	m_AllwaysBuildableBuildings.RemoveAll();
	
}
// Funktion �berpr�ft, ob wie aufgrund der Bev�lkerung hier im System �berhaupt (noch) eine Handelsroute
// anlegen k�nnen
BOOLEAN CSystem::CanAddTradeRoute(CResearchInfo* researchInfo)
{
	// aller 20Mrd. Bev�lkerung bekommt man eine neue Handelsroute
	USHORT currentTradeRoutes = m_TradeRoutes.GetSize();
	short addResRoute = 1;
	USHORT maxTradeRoutes = (USHORT)(m_dHabitants / TRADEROUTEHAB) + m_Production.GetAddedTradeRoutes();
	///// HIER DIE BONI DURCH SPEZIALFORSCHUNG //////
	// Hier die Boni durch die Uniqueforschung "Lager und Transport" -> eine Ressourcenroute mehr
	if (researchInfo->GetResearchComplex(10)->GetFieldStatus(3) == RESEARCHED)
		addResRoute += researchInfo->GetResearchComplex(10)->GetBonus(3);
	///// HIER DIE BONI DURCH SPEZIALFORSCHUNG //////
	// Bei dieser Spezialforschung gibt es wom�glich auch eine Ressourcenroute mehr
	// Hier die Boni durch die Uniqueforschung "Handel" -> mindestens eine Handelsroute
	if (researchInfo->GetResearchComplex(11)->GetFieldStatus(3) == RESEARCHED)
	{
		if (maxTradeRoutes == NULL)
		{
			addResRoute += researchInfo->GetResearchComplex(11)->GetBonus(3);
			maxTradeRoutes += researchInfo->GetResearchComplex(11)->GetBonus(3);
		}
	}
	if (m_ResourceRoutes.GetSize() > addResRoute)
		currentTradeRoutes += m_ResourceRoutes.GetSize() - addResRoute;
	return (currentTradeRoutes < maxTradeRoutes);
}

// Funktion generiert eine neue Handelsroute. Wenn die Funktion <code>TRUE</code> zur�ckgibt, dann konnte die
// Handelsroute erfolgreich angelegt werden. Als Parameter wird dabei die Koordinate <code>dest</code>des
// Zielsektors �bergeben sowie ein Zeiger auf alle Systeme <code>systems</code> auf der Map..
BOOLEAN CSystem::AddTradeRoute(CPoint dest, CSystem systems[][STARMAP_SECTORS_VCOUNT], CResearchInfo* researchInfo)
{
	BOOLEAN canAddTradeRoute = CanAddTradeRoute(researchInfo);
	// als erstes �berpr�fen, das noch keine Handelsroute aus einem System, welches den gleichen Besitzer wie
	// dieses System hat, zu diesem Ziel f�hrt. Man kann immer nur eine Handelsroute zu einem bestimmten
	// System haben.
	if (canAddTradeRoute)
		for (int y = 0; y < STARMAP_SECTORS_VCOUNT; y++)
			for (int x = 0; x < STARMAP_SECTORS_HCOUNT; x++)
				if (systems[x][y].GetOwnerOfSystem() == this->GetOwnerOfSystem())
					if (&systems[x][y] != this)
						for (int i = 0; i < systems[x][y].GetTradeRoutes()->GetSize(); i++)
							if (systems[x][y].GetTradeRoutes()->GetAt(i).GetDestKO() == dest)
								return FALSE;

	// nachschauen, das nicht schon eine Handelsroute mit dem Ziel vorhanden ist, wenn dies der Fall ist, dann wird
	// diese Handelsroute gel�scht
	for (int i = 0; i < m_TradeRoutes.GetSize(); i++)
		if (m_TradeRoutes.GetAt(i).GetDestKO() == dest)
		{
			// Wenn wir eine Route aufheben wollen, so die Dauer der Handelsroute negativ setzen. Wenn die Handelsroute
			// eh nur noch weniger als 5 Runden dauert, dann wird die K�ndigungsdauer auf diese k�rzere Zeit eingestellt.
			if (m_TradeRoutes.GetAt(i).GetDuration() > 0)
			{
				// weniger als 5 Runden
				if (m_TradeRoutes.GetAt(i).GetDuration() < 5)
					m_TradeRoutes.ElementAt(i).SetDuration(m_TradeRoutes.GetAt(i).GetDuration()-6);
				// ansonsten normal einstellen
				else
					m_TradeRoutes.ElementAt(i).SetDuration(-1);
				return TRUE;
			}
			else
				return FALSE;
		}

	if (canAddTradeRoute)
	{
		CTradeRoute route;
		route.GenerateTradeRoute(dest);
		m_TradeRoutes.Add(route);
		return TRUE;
	}
	return FALSE;
}

// Funktion gibt das gesamte Latinum zur�ck, was alle Handelsrouten aus diesem System generiert haben.
USHORT CSystem::LatinumFromTradeRoutes()
{
	USHORT latinum = 0;
	for (int i = 0; i < m_TradeRoutes.GetSize(); i++)
		latinum += m_TradeRoutes.GetAt(i).GetLatinum(this->m_Production.GetIncomeOnTradeRoutes());
	return latinum;
}

// Funkt �berpr�ft alle Handelsrouten in dem System, ob sie Aufgrund der Bev�lkerung oder der Geb�ude noch
// da sein k�nnen. Wurden Handelsrouten gel�scht, so gibt die Funktion die Anzahl der L�schungen zur�ck
BYTE CSystem::CheckTradeRoutes(CResearchInfo* researchInfo)
{
	BYTE number = 0;
	// Dauer der Handelsrouten �berpr�fen. Wenn eine negative Dauer von -5 erreicht wurde, so wird die Handelsroute
	// gel�scht, wenn eine positive Dauer von 1 erreicht wurde, so verl�ngert sich die Dauer wieder auf 20 Runden.
	// Bei allen anderen F�llen wird um eins dekrementiert.
	for (int i = 0; i < m_TradeRoutes.GetSize(); i++)
	{
		// Wenn das System blockiert wird, dann produziert die Handelsroute kein Latinum
		if (GetBlockade() > NULL)
			m_TradeRoutes.GetAt(i).SetLatinum(0);
		if (m_TradeRoutes.GetAt(i).GetDuration() <= -5)
		{
			m_TradeRoutes.RemoveAt(i--);
			number++;
		}
		else if (m_TradeRoutes.GetAt(i).GetDuration() == 1)
			m_TradeRoutes.ElementAt(i).SetDuration(20);
		else
			m_TradeRoutes.ElementAt(i).SetDuration(m_TradeRoutes.GetAt(i).GetDuration()-1);
	}

	// Hier Bev�lkerungs und Geb�udecheck durchf�hren
	USHORT currentTradeRoutes = m_TradeRoutes.GetSize();
	short addResRoute = 1;
	USHORT maxTradeRoutes = (USHORT)(m_dHabitants / TRADEROUTEHAB) + m_Production.GetAddedTradeRoutes();
	///// HIER DIE BONI DURCH SPEZIALFORSCHUNG //////
	// Hier die Boni durch die Uniqueforschung "Lager und Transport" -> eine Ressourcenroute mehr
	if (researchInfo->GetResearchComplex(10)->GetFieldStatus(3) == RESEARCHED)
		addResRoute += researchInfo->GetResearchComplex(10)->GetBonus(3);
	///// HIER DIE BONI DURCH SPEZIALFORSCHUNG //////
	// Bei dieser Spezialforschung gibt es wom�glich auch eine Ressourcenroute mehr
	// Hier die Boni durch die Uniqueforschung "Handel" -> mindestens eine Handelsroute
	if (researchInfo->GetResearchComplex(11)->GetFieldStatus(3) == RESEARCHED)
	{		
		if (maxTradeRoutes == NULL)
		{
			addResRoute += researchInfo->GetResearchComplex(11)->GetBonus(3);
			maxTradeRoutes += researchInfo->GetResearchComplex(11)->GetBonus(3);
		}
	}
	if (m_ResourceRoutes.GetSize() > addResRoute)
		currentTradeRoutes += m_ResourceRoutes.GetSize() - addResRoute;
	while (currentTradeRoutes > maxTradeRoutes)
	{
		if (m_TradeRoutes.GetSize() > 0)
		{
			m_TradeRoutes.RemoveAt(m_TradeRoutes.GetUpperBound());
			number++;
			currentTradeRoutes--;
		}
		else
			break;
	}
	return number;
}

// Funktion generiert eine neue Ressourcenroute. Wenn die Funktion <code>TRUE</code> zur�ckgibt, dann konnte die
// Ressourcenroute erfolgreich angelegt werden. Als Parameter wird dabei die Koordinate <code>dest</code> des
// Zielsektors �bergeben sowie die Art der betroffenen Ressource <code>res</code> und einen Zeiger auf alle
// Systeme <code>systems</code> auf der Map.
BOOLEAN CSystem::AddResourceRoute(CPoint dest, BYTE res, CSystem systems[][STARMAP_SECTORS_VCOUNT], CResearchInfo* researchInfo)
{
	// Es gibt immer mindst. eine Ressourcenroute pro System. Insgesamt stehen "Anzahl Handelsrouten + 1" Ressourcen-
	// routen zur Vref�gung.
	short addResRoute = 1;
	///// HIER DIE BONI DURCH SPEZIALFORSCHUNG //////
	// Hier die Boni durch die Uniqueforschung "Lager und Transport" -> eine Ressourcenroute mehr
	if (researchInfo->GetResearchComplex(10)->GetFieldStatus(3) == RESEARCHED)
		addResRoute += researchInfo->GetResearchComplex(10)->GetBonus(3);
	///// HIER DIE BONI DURCH SPEZIALFORSCHUNG //////
	// Bei dieser Spezialforschung gibt es wom�glich auch eine Ressourcenroute mehr
	// Hier die Boni durch die Uniqueforschung "Handel" -> mindestens eine Handelsroute
	if (researchInfo->GetResearchComplex(11)->GetFieldStatus(3) == RESEARCHED)
	{
		USHORT maxTradeRoutes = (USHORT)(m_dHabitants / TRADEROUTEHAB) + m_Production.GetAddedTradeRoutes();
		if (maxTradeRoutes == NULL)
			addResRoute += researchInfo->GetResearchComplex(11)->GetBonus(3);
	}
		
	USHORT maxResourceRoutes = (USHORT)(m_dHabitants / TRADEROUTEHAB) + m_Production.GetAddedTradeRoutes() + addResRoute;
	
	if (systems[dest.x][dest.y].GetOwnerOfSystem() != this->GetOwnerOfSystem())
		return FALSE;
	if (systems[dest.x][dest.y].GetHabitants() == 0.0f || this->GetHabitants() == 0.0f)
		return FALSE;
	if (maxResourceRoutes <= m_ResourceRoutes.GetSize() + m_TradeRoutes.GetSize())
		return FALSE;
	// gibt es schon eine Route mit dem Rohstoff ins Zielsystem?
	for (int i = 0; i < m_ResourceRoutes.GetSize(); i++)
		if (m_ResourceRoutes.GetAt(i).GetResource() == res && m_ResourceRoutes.GetAt(i).GetKO() == dest)
			return FALSE;
	
	CResourceRoute route;
	route.GenerateResourceRoute(dest, res);
	m_ResourceRoutes.Add(route);
	return TRUE;
}

// Funktion �berpr�ft alle Ressourcenrouten in dem System, ob sie Aufgrund der Bev�lkerung oder der Geb�ude noch
// da sein d�rfen. Wurden Ressourcenrouten gel�scht, so gibt die Funktion die Anzahl der L�schungen zur�ck.
BYTE CSystem::CheckResourceRoutes(CResearchInfo* researchInfo)
{
	// Es gibt immer mindst. eine Ressourcenroute pro System. Insgesamt stehen "Anzahl Handelsrouten + 1" Ressourcen-
	// routen zur Vref�gung.
	short addResRoute = 1;
	///// HIER DIE BONI DURCH SPEZIALFORSCHUNG //////
	// Hier die Boni durch die Uniqueforschung "Lager und Transport" -> eine Ressourcenroute mehr
	if (researchInfo->GetResearchComplex(10)->GetFieldStatus(3) == RESEARCHED)
		addResRoute += researchInfo->GetResearchComplex(10)->GetBonus(3);
	///// HIER DIE BONI DURCH SPEZIALFORSCHUNG //////
	// Bei dieser Spezialforschung gibt es wom�glich auch eine Ressourcenroute mehr
	// Hier die Boni durch die Uniqueforschung "Handel" -> mindestens eine Handelsroute
	if (researchInfo->GetResearchComplex(11)->GetFieldStatus(3) == RESEARCHED)
	{
		USHORT maxTradeRoutes = (USHORT)(m_dHabitants / TRADEROUTEHAB) + m_Production.GetAddedTradeRoutes();
		if (maxTradeRoutes == NULL)
			addResRoute += researchInfo->GetResearchComplex(11)->GetBonus(3);
	}

	USHORT maxResourceRoutes = (USHORT)(m_dHabitants / TRADEROUTEHAB) + m_Production.GetAddedTradeRoutes() + addResRoute;
	USHORT currentResourceRoutes = m_ResourceRoutes.GetSize() + m_TradeRoutes.GetSize();
	BYTE number = 0;
	while (currentResourceRoutes > maxResourceRoutes)
	{
		if (m_ResourceRoutes.GetSize() > 0)
		{
			m_ResourceRoutes.RemoveAt(m_ResourceRoutes.GetUpperBound());
			number++;
			currentResourceRoutes--;
		}
		else
			break;
	}
	return number;
}

// Wenn in diesem System Truppen stationiert sind, dann wird deren Moralwert mit einbezogen.
// Ist die Moral im System unter 100, so wird der Moralwert der Einheit dazuaddiert, wenn er �ber 100 ist, dann wird
// der Moralwert abgezogen.
void CSystem::IncludeTroopMoralValue(CArray<CTroopInfo>* troopInfo)
{
	if (m_Troops.GetSize() > 0 && m_iMoral != 100)
		for (int i = 0; i < m_Troops.GetSize(); i++)
		{
			BYTE id = m_Troops.GetAt(i).GetID();
			// Wenn die Systemmoral kleiner als 100 ist, dann wird der Moralwert der Truppe dazuaddiert
			if (m_iMoral < 100)
			{
				m_iMoral += troopInfo->GetAt(id).GetMoralValue();
				if (m_iMoral > 100)
					m_iMoral = 100;
			}
			// Wenn die Systemmoral gr��er als 100 ist, dann wird der Moralwert der Truppe abgezogen
			else if (m_iMoral > 100)
			{
				m_iMoral -= troopInfo->GetAt(id).GetMoralValue();
				if (m_iMoral < 100)
					m_iMoral = 100;
			}
		}
}

// Funktion setzt alle Werte wieder zur�ck auf standard.
void CSystem::ResetSystem()
{
	m_sOwnerOfSystem = "";			// Der Besitzer des Systems
	m_dHabitants = 0.0f;			// Einwohner in dem System
	m_iMoral = 100;					// Moral im System
	m_byBlockade = 0;				// Prozentwert der Blockade im System
	// Anzahl der Geb�ude des Types
	m_iFoodBuildings = 0;			// Anzahl der Nahrungsgeb�ude in dem System
	m_iIndustryBuildings = 0;		// Anzahl der Industriegeb�ude in dem System
	m_iEnergyBuildings = 0;			// Anzahl der Energiegeb�ude in dem System
	m_iSecurityBuildings = 0;		// Anzahl der Geheimdienstgeb�ude in dem System
	m_iResearchBuildings = 0;		// Anzahl der Forschungsgeb�ude in dem System
	m_iTitanMines = 0;				// Anzahl der Titanminen in dem System
	m_iDeuteriumMines = 0;			// Anzahl der Deuteriumminen in dem System
	m_iDuraniumMines = 0;			// Anzahl der Duraniumminen in dem System
	m_iIridiumMines = 0;			// Anzahl der Iridiumminen in dem System
	m_iCrystalMines = 0;			// Anzahl der Crystalminen in dem System
	// Lagerkapazit�ten
	m_iFoodStore = 1000;
	m_iTitanStore = 0;
	m_iDeuteriumStore = 0;
	m_iDuraniumStore = 0;
	m_iCrystalStore = 0;
	m_iIridiumStore = 0;
	m_iDilithiumStore = 0;
	m_BuildingDestroy.RemoveAll();
	m_Production.Reset();
	m_AssemblyList.Reset();
	m_Buildings.RemoveAll();
	m_BuildableBuildings.RemoveAll();
	m_AllwaysBuildableBuildings.RemoveAll();
	m_BuildableUpdates.RemoveAll();
	m_BuildableWithoutAssemblylistCheck.RemoveAll();
	m_BuildableShips.RemoveAll();
	m_BuildableTroops.RemoveAll();
	m_TradeRoutes.RemoveAll();
	m_ResourceRoutes.RemoveAll();
	m_byMaxTradeRoutesFromHab = 0;
	m_Troops.RemoveAll();
	m_bAutoBuild = FALSE;
}

//////////////////////////////////////////////////////////////////////
// Hilfsfunktionen
//////////////////////////////////////////////////////////////////////
BOOLEAN CSystem::CheckTech(CBuildingInfo* building, CResearch* research)
{
	// n�tige Forschungsstufen checken
	if (research->GetBioTech() <  building->GetBioTech())
		return 0;
	if (research->GetEnergyTech() < building->GetEnergyTech())
		return 0;
	if (research->GetCompTech() < building->GetCompTech())
		return 0;		
	if (research->GetPropulsionTech() < building->GetPropulsionTech())
		return 0;
	if (research->GetConstructionTech() < building->GetConstructionTech())
		return 0;
	if (research->GetWeaponTech() < building->GetWeaponTech())
		return 0;
	return 1;
}

BOOLEAN CSystem::CheckPlanet(CBuildingInfo* building, CSector* sector)
{
	BOOLEAN Ok = FALSE;
	BOOLEAN dilithium = FALSE;
	int number = sector->GetNumberOfPlanets();
	CPlanet planet;
	for (int i = 0; i < number && Ok == FALSE; i++)
	{
		planet = *sector->GetPlanet(i);
		if (planet.GetColonized())
		{
			//{M,O,L,P,H,Q,K,G,R,F,C,N,A,B,E,Y,I,J,S,T}
			if (building->GetPlanetTypes(PLANETCLASS_A) == 1 && planet.GetClass() == 'A' && Ok == FALSE)	// nach A suchen
				Ok = TRUE;
			if (building->GetPlanetTypes(PLANETCLASS_B) == 1 && planet.GetClass() == 'B' && Ok == FALSE)	// nach B suchen
				Ok = TRUE;
			if (building->GetPlanetTypes(PLANETCLASS_C) == 1 && planet.GetClass() == 'C' && Ok == FALSE)	// nach C suchen
				Ok = TRUE;
			if (building->GetPlanetTypes(PLANETCLASS_E) == 1 && planet.GetClass() == 'E' && Ok == FALSE)	// nach E suchen
				Ok = TRUE;	// D fehlt wieder, weil ja Asteroid
			if (building->GetPlanetTypes(PLANETCLASS_F) == 1 && planet.GetClass() == 'F' && Ok == FALSE)	// nach F suchen
				Ok = TRUE;
			if (building->GetPlanetTypes(PLANETCLASS_G) == 1 && planet.GetClass() == 'G' && Ok == FALSE)	// nach G suchen
				Ok = TRUE;
			if (building->GetPlanetTypes(PLANETCLASS_H) == 1 && planet.GetClass() == 'H' && Ok == FALSE)	// nach H suchen
				Ok = TRUE;
			if (building->GetPlanetTypes(PLANETCLASS_I) == 1 && planet.GetClass() == 'I' && Ok == FALSE)	// nach I suchen
				Ok = TRUE;
			if (building->GetPlanetTypes(PLANETCLASS_J) == 1 && planet.GetClass() == 'J' && Ok == FALSE)	// nach J suchen
				Ok = TRUE;
			if (building->GetPlanetTypes(PLANETCLASS_K) == 1 && planet.GetClass() == 'K' && Ok == FALSE)	// nach K suchen
				Ok = TRUE;
			if (building->GetPlanetTypes(PLANETCLASS_L) == 1 && planet.GetClass() == 'L' && Ok == FALSE)	// nach L suchen
				Ok = TRUE;
			if (building->GetPlanetTypes(PLANETCLASS_M) == 1 && planet.GetClass() == 'M' && Ok == FALSE)	// nach M suchen
				Ok = TRUE;
			if (building->GetPlanetTypes(PLANETCLASS_N) == 1 && planet.GetClass() == 'N' && Ok == FALSE)	// nach N suchen
				Ok = TRUE;
			if (building->GetPlanetTypes(PLANETCLASS_O) == 1 && planet.GetClass() == 'O' && Ok == FALSE)	// nach O suchen
				Ok = TRUE;
			if (building->GetPlanetTypes(PLANETCLASS_P) == 1 && planet.GetClass() == 'P' && Ok == FALSE)	// nach P suchen
				Ok = TRUE;
			if (building->GetPlanetTypes(PLANETCLASS_Q) == 1 && planet.GetClass() == 'Q' && Ok == FALSE)	// nach Q suchen
				Ok = TRUE;
			if (building->GetPlanetTypes(PLANETCLASS_R) == 1 && planet.GetClass() == 'R' && Ok == FALSE)	// nach R suchen
				Ok = TRUE;
			if (building->GetPlanetTypes(PLANETCLASS_S) == 1 && planet.GetClass() == 'S' && Ok == FALSE)	// nach S suchen
				Ok = TRUE;
			if (building->GetPlanetTypes(PLANETCLASS_T) == 1 && planet.GetClass() == 'T' && Ok == FALSE)	// nach T suchen
				Ok = TRUE;
			if (building->GetPlanetTypes(PLANETCLASS_Y) == 1 && planet.GetClass() == 'Y' && Ok == FALSE)	// nach Y suchen
				Ok = TRUE;
		}
	}
	
	// Wenn das Geb�ude Dilithium produziert, so muss auf einem kolonisierten Planeten auch Dilithium vorhanden sein
	if (building->GetDilithiumProd() > 0)
	{
		for (int i = 0; i < sector->GetNumberOfPlanets(); i++)
			if (sector->GetPlanet(i)->GetColonized())		
				dilithium |= sector->GetPlanet(i)->GetBoni()[DILITHIUM];
		if (!dilithium || !Ok)
			return 0;
	}
	if (Ok == FALSE)	// Haben also kein Planet im System gefunden, auf dem wir das Geb�ude bauen k�nnten
		return 0;
	else
		return 1;
}

BOOLEAN CSystem::CheckGeneralConditions(CBuildingInfo* building, CSector* sector, CGlobalBuildings* globals, CMajor* pMajor)
{
/*	Allgemeine Voraussetzungen
	--------------------------
	ben�tigte Anzahl eigener Systeme
	max X mal pro System
	max X mal pro Imperium
	Nur Heimatplanet
	Nur eigene Kolonie
	Nur Minorraceplanet
	Nur erobertes System
	Nur baubar in System mit Name
	Nur wirklicher Besitzer des Geb�udes
	minimale Bev�lkerung im System
	mindst. X Geb�ude von ID im System
	mindts. X Geb�ude von ID im Imperium	
*/	
	// ben�tigte Anzahl eigener Systeme
	if (building->GetNeededSystems() > pMajor->GetEmpire()->GetNumberOfSystems())
		return FALSE;
	// Nur Heimatplanet checken
	if (building->GetOnlyHomePlanet())
	{
		if (m_sOwnerOfSystem == pMajor->GetRaceID() && sector->GetName() != pMajor->GetHomesystemName())
			return FALSE;
	}
	// Nur eigene Kolonie checken (Heimatsystem ist auch eine eigene Kolonie)
	if (building->GetOnlyOwnColony())
		if (sector->GetColonyOwner() != m_sOwnerOfSystem)
			return FALSE;
	// Nur Minorraceplanet checken
	if (building->GetOnlyMinorRace())
		if (sector->GetMinorRace() == FALSE)
			return FALSE;
	// Nur erobertes System checken
	if (building->GetOnlyTakenSystem())
		if (sector->GetTakenSector() == FALSE)
			return FALSE;
	// Nur baubar in System mit Name checken
	if (building->GetOnlyInSystemWithName() != "0" && building->GetOnlyInSystemWithName() != "")
		if (building->GetOnlyInSystemWithName() != sector->GetName())
			return FALSE;
	// Nur wirklicher Besitzer des Geb�udes
	if (building->GetOnlyRace())
		if (pMajor->GetRaceBuildingNumber() != building->GetOwnerOfBuilding())
			return FALSE;
	// Minimale Bev�lkerungsanzahl checken
	if (building->GetMinHabitants() > 0)
		if (m_dHabitants < (double)building->GetMinHabitants())
			return FALSE;
	// mindst. X Geb�ude von ID im System
	if (building->GetMinInSystem().Number > 0)
	{
		USHORT number = 0;
		for (int i = 0; i < m_Buildings.GetSize(); i++)
		{
			if (m_Buildings.GetAt(i).GetRunningNumber() == building->GetMinInSystem().RunningNumber)
				number++;
			if (number >= building->GetMinInSystem().Number)
				break;
		}
		if (number < building->GetMinInSystem().Number)
			return FALSE;
	}
	// max. X Geb�ude von ID im System
	if (building->GetMaxInSystem().Number > 0)
	{
		USHORT number = 0;
		for (int i = 0; i < m_Buildings.GetSize(); i++)
		{
			if (m_Buildings.GetAt(i).GetRunningNumber() == building->GetMaxInSystem().RunningNumber)
				number++;
			if (number >= building->GetMaxInSystem().Number)
				return FALSE;
		}
	}
	// mindst. X Geb�ude von ID im Imperium
	if (building->GetMinInEmpire().Number > 0)
	{
		USHORT number = 0;
		for (int i = 0; i < globals->GetGlobalBuildings()->GetSize(); i++)
		{
			if (globals->GetGlobalBuildings()->GetAt(i) == building->GetMinInEmpire().RunningNumber)
				number++;
			if (number >= building->GetMinInEmpire().Number)
				break;
		}
		if (number < building->GetMinInEmpire().Number)
			return FALSE;
	}
	// max. X Geb�ude von ID im Imperium
	if (building->GetMaxInEmpire().Number > 0)
	{
		USHORT number = 0;
		for (int i = 0; i < globals->GetGlobalBuildings()->GetSize(); i++)
		{
			if (globals->GetGlobalBuildings()->GetAt(i) == building->GetMaxInEmpire().RunningNumber)
				number++;
			if (number >= building->GetMaxInEmpire().Number)
				return FALSE;
		}
	}	

	return TRUE;
}

// Diese private Hilfsfunktion �berpr�ft, ob es einen Nachfolger zu unserem Geb�ude in der Liste der baubaren
// Geb�ude (-> flag == 0) oder als stehendes Geb�ude (-> flag == 1) im System schon steht. Die Funktion 
// gibt TRUE zur�ck, wenn wir einen Nachfolger gefunden haben, ansonsten FALSE.
BOOLEAN CSystem::CheckFollower(BuildingInfoArray* buildings, USHORT id, BOOLEAN flag, BOOLEAN equivalence)
{
	// Checken ob ein potentieller Nachfolger in der Liste der baubaren Geb�ude vorkommt
	if (flag == 0)
	{	
		/*	zum Algorithmus:
			Ich durchsuche jede ID im Feld der baubaren Geb�ude nach potentiellen Vorg�ngern. Ist dann einer
			der Vorg�nger meine ID, dann k�nnen wir abbrechen.
		*/
		for (int i = 0; i < m_BuildableWithoutAssemblylistCheck.GetSize(); i++)
		{
			// Weil zu diesem Zeitpunkt auch Geb�udeupgrades (negative ID) in diesem Feld vorkommen k�nnen, darf ich
			// nur die echten Geb�ude betrachten
			if (m_BuildableWithoutAssemblylistCheck.GetAt(i) > 0)
			{
				// Vorg�ngerID eines Geb�udes aus der Liste der baubaren Geb�ude holen
				USHORT pre = buildings->GetAt(m_BuildableWithoutAssemblylistCheck.GetAt(i)-1).GetPredecessorID();
				// Haben wir jetzt schon den Vorg�nger gefunden (eher unwahrscheinlich)
				if (pre == id)
					return TRUE;
				// jetzt in die Schleife um sich immer neue ID�s zu holen
				int z = 0;
				do {
					// Vorg�ngerID des Vorg�ngers holen
					if (pre > 0)
					{
						USHORT oldPre = pre;
						pre = buildings->GetAt(pre-1).GetPredecessorID();
						if (oldPre == pre)
						{							
							CString s;
							s.Format("ERROR with Buildings Predecessor ID:\nBuilding: %s\n\nID: %d\nit is the same like the ID of the building", buildings->GetAt(oldPre-1).GetBuildingName(), oldPre);
							AfxMessageBox(s);
							break;
						}			
					}					
				} while (pre != id && pre != 0);// mache das solange wir keinen Vorg�nger gefunden haben oder es keinen Vorg�nger gibt
				// Gibt es einen Vorg�nger
				if (pre == id)
					return TRUE;				
											
				// So, sind wir jetzt noch dabei und haben in dem System auch Geb�ude von anderen Rassen stehen (also der
				// Parameter equivalence == TRUE), dannn m�ssen wir auch noch bei dem �quivalenzgeb�ude nach Vorg�ngern suchen
				if (equivalence == TRUE)
				{
					// Vorg�ngerID des entsprechenden �quivalenzgeb�udes holen
					USHORT race = buildings->GetAt(id-1).GetOwnerOfBuilding();
					//USHORT race = buildings->GetAt(m_BuildableWithoutAssemblylistCheck.GetAt(i)-1).GetOwnerOfBuilding();
					USHORT equi = buildings->GetAt(m_BuildableWithoutAssemblylistCheck.GetAt(i)-1).GetEquivalent(race);
/*					if (equi > 0)
					{					
						pre = buildings->GetAt(equi-1).GetPredecessorID();	// pre ist Vorg�nger des �quivalenzgeb�udes
						// Jetzt wie oben die do - while Schleife durchlaufen
						do {
							// Vorg�ngerID des "Vorg�ngers holen
							if (pre > 0)
								pre = buildings->GetAt(pre-1).GetPredecessorID();
						} while (pre != equi && pre != 0);// mache das solange wir keinen Vorg�nger gefunden haben oder es keinen Vorg�nger gibt
						// Gibt es einen Vorg�nger
						if (pre == equi)
							return TRUE;
					}*/
					if (equi == id)
						return TRUE;
					pre = buildings->GetAt(m_BuildableWithoutAssemblylistCheck.GetAt(i)-1).GetPredecessorID();
					// Jetzt wir oben die do - while Schleife durchlaufen
					do {
						// Vorg�ngerID des "Vorg�ngers holen
						if (pre > 0)
						{
							USHORT oldPre = pre;
							equi = buildings->GetAt(pre-1).GetEquivalent(race);
							pre = buildings->GetAt(pre-1).GetPredecessorID();
							if (oldPre == pre)
							{							
								CString s;
								s.Format("ERROR with Buildings Predecessor ID:\nBuilding: %s\n\nID: %d\nit is the same like the ID of the building", buildings->GetAt(oldPre-1).GetBuildingName(), oldPre);
								AfxMessageBox(s);
								break;
							}	
						}						
					} while (id != equi && pre != 0); // mache das solange wir keinen Vorg�nger gefunden haben oder es keinen Vorg�nger gibt
					// Gibt es einen Vorg�nger
					if (equi == id)
						return TRUE;					
				}
			}
		}
	}
	// Checken ob ein potentieller Nachfolger schon im System steht. Normalerweise ist dieser schon in der Liste
	// der baubaren Geb�ude, aber z.B. Geb�ude die nur einmal pro System gebaut werden k�nnen sind nicht in dieser Liste.
	// z.B. wenn wir eine geupdatede Werft einer anderen Rasse in dem System stehen haben, darf ich nicht einen
	// potentiellen �quivalenten Vorg�nger dieser Werft unsererseits bauen.
	else
	{	
		USHORT minID = 0;
		// Der Algorithmus ist wie der obere. Nur wird hier das Feld der stehenden Geb�ude im System durchlaufen
		for (int i = 0; i < m_Buildings.GetSize(); i++)
		{
			// Da das Geb�udefeld nach der RunningNumber geordnet ist, brauchen wir erst wieder checken
			// wenn wir ein Geb�ude mit einer neune RunningNumber haben
			if (m_Buildings.GetAt(i).GetRunningNumber() > minID)
				minID = m_Buildings.GetAt(i).GetRunningNumber();
			{
				// Vorg�ngerID eines Geb�udes aus der Liste der stehenden Geb�ude holen
				USHORT pre = buildings->GetAt(m_Buildings.GetAt(i).GetRunningNumber()-1).GetPredecessorID();
				// Haben wir jetzt schon den Vorg�nger gefunden (eher unwahrscheinlich)
				if (pre == id)
					return TRUE;				
				// jetzt in die Schleife um sich immer neue ID�s zu holen
				do {
					// Vorg�ngerID des "Vorg�ngers holen
					if (pre > 0)
					{
						USHORT oldPre = pre;
						pre = buildings->GetAt(pre-1).GetPredecessorID();
						if (oldPre == pre)
						{							
							CString s;
							s.Format("ERROR with Buildings Predecessor ID:\nBuilding: %s\n\nID: %d\nit is the same like the ID of the building", buildings->GetAt(oldPre-1).GetBuildingName(), oldPre);
							AfxMessageBox(s);
							break;
						}
					}					
				} while (pre != id && pre != 0);// mache das solange wir keinen Vorg�nger gefunden haben oder es keinen Vorg�nger gibt
				// Gibt es einen Vorg�nger
				if (pre == id)
					return TRUE;				

				// So, sind wir jetzt noch dabei und haben in dem System auch Geb�ude von anderen Rassen stehen (also der
				// Parameter equivalence == TRUE), dannn m�ssen wir auch noch bei dem �quivalenzgeb�ude nach Vorg�ngern suchen
				if (equivalence == TRUE)
				{
					// Vorg�ngerID des entsprechenden �quivalenzgeb�udes holen
					USHORT race = buildings->GetAt(id-1).GetOwnerOfBuilding();
					USHORT equi = buildings->GetAt(m_Buildings.GetAt(i).GetRunningNumber()-1).GetEquivalent(race);
					if (equi == id)
						return TRUE;
					pre = buildings->GetAt(m_Buildings.GetAt(i).GetRunningNumber()-1).GetPredecessorID();
					// Jetzt wir oben die do - while Schleife durchlaufen
					do {
						// Vorg�ngerID des "Vorg�ngers holen
						if (pre > 0)
						{
							USHORT oldPre = pre;
							equi = buildings->GetAt(pre-1).GetEquivalent(race);
							pre = buildings->GetAt(pre-1).GetPredecessorID();
							if (oldPre == pre)
							{							
								CString s;
								s.Format("ERROR with Buildings Predecessor ID:\nBuilding: %s\n\nID: %d\nit is the same like the ID of the building", buildings->GetAt(oldPre-1).GetBuildingName(), oldPre);
								AfxMessageBox(s);
								break;
							}	
						}						
					} while (id != equi && pre != 0);// mache das solange wir keinen Vorg�nger gefunden haben oder es keinen Vorg�nger gibt
					// Gibt es einen Vorg�nger
					if (id == equi)
						return TRUE;					
				}
			}
		}
	}
	return FALSE;
}

/*
BOOLEAN CSystem::CheckOncePerSystem(CBuildingInfo* building)
{
	// 3. checken ob nur einmal pro System
	int number = m_Buildings.GetSize();
	for (int i = 0; i < number; i++)
		if (building->GetOncePerSystem() == TRUE && m_Buildings.GetAt(i).GetRunningNumber() == building->GetRunningNumber())
			return 0;
	for (int i = 0; i < ALE; i++)
		if (m_AssemblyList.GetAssemblyListEntry(i) == building->GetRunningNumber() && building->GetOncePerSystem() == TRUE)
			return 0;
	return 1;
}

BOOLEAN CSystem::CheckOnlyHomeSystem(CBuildingInfo* building, CSector* sector, CEmpire* empire)
{
	// 6. checken ob nur Heimatplanet
	if (building->GetOnlyHomePlanet() == TRUE && empire->GetEmpireName() == HUMAN && sector->GetName() != "Sol")
		return 0;
	if (building->GetOnlyHomePlanet() == TRUE && empire->GetEmpireName() == FERENGI && sector->GetName() != "Ferenginar")
		return 0;
	if (building->GetOnlyHomePlanet() == TRUE && empire->GetEmpireName() == KLINGON && sector->GetName() != "Qo'nos")
		return 0;
	if (building->GetOnlyHomePlanet() == TRUE && empire->GetEmpireName() == ROMULAN && sector->GetName() != "Romulus")
		return 0;
	if (building->GetOnlyHomePlanet() == TRUE && empire->GetEmpireName() == CARDASSIAN && sector->GetName() != "Cardassia")
		return 0;
	if (building->GetOnlyHomePlanet() == TRUE && empire->GetEmpireName() == DOMINION && sector->GetName() != "Omarion")
		return 0;
	return 1;
}
*/
/*void CSystem::CalculateBuildableBuildings(CSector* sector, CBuildingInfo* buildingInfo, CEmpire* empire)
{
	for (int k = 0; k < m_BuildableBuildings.GetSize(); )
		m_BuildableBuildings.RemoveAt(k);
	for (int k = 0; k < m_BuildableUpdates.GetSize(); )
		m_BuildableUpdates.RemoveAt(k);
	m_BuildableBuildings.RemoveAll();
	m_BuildableUpdates.RemoveAll();
	m_BuildableBuildings.FreeExtra();
	m_BuildableUpdates.FreeExtra();
	// Man m��te hier einf�gen, dass nur die Geb�ude gecheckt werden, die durch die Rasse auch baubar sind
	// so, z.B. bei der F�d von Geb�ude 1 bis 200, Fer von 201 bis 400)
	// sp�ter machen, wenn wir alle Geb�ude in der Datenbank haben
	for (int i = 1; i < NOB; i++)
	{
		// Checken ob wir es durch Techstufen bauen k�nnen
		if (this->CheckTech(&buildingInfo[i],empire->GetResearch()))
			// Checken ob wir es durch die vorhanden Planeten bauen k�nnen
			if (this->CheckPlanet(&buildingInfo[i],sector))
				// Checken ob wir es nur einmal im System bauen k�nnen
				if (this->CheckOncePerSystem(&buildingInfo[i]))
					// Checken ob wir es nur im Heimatsystem bauen k�nnen
					if (this->CheckOnlyHomeSystem(&buildingInfo[i],sector,empire))
					{
						// Checken ob andere Geb�ude dadurch nicht mehr baubar sind, wenn ja Geb�ude nicht mehr baubar
						if (i > 1 && /*buildingInfo[i].GetWorker() == TRUE &&*//* buildingInfo[i-1].GetFollower() == i && buildingInfo[i-1].GetIsUpdateable() == TRUE)
							for (int j = 0; j < m_BuildableBuildings.GetSize(); j++)
								if (m_BuildableBuildings.GetAt(j) == i-1)
								{
									m_BuildableBuildings.RemoveAt(j);
									j--;
									break;
								}
						m_BuildableBuildings.Add(buildingInfo[i].GetRunningNumber());
					}
	}
	
	// Hier jetzt schauen, ob ein Geb�ude steht, welches wir h�tten gar nicht bauen
	// d�rfen, dieses k�nnen wir dann trotzdem bauen
	for (int i = 0; i < m_Buildings.GetSize(); i++)			// Anzahl der aktuellen Geb�ude
	{
		BOOLEAN okay = FALSE;
		BOOLEAN stop = FALSE;
		//if (m_bBuildableBuildings[m_Buildings.GetAt(i).GetRunningNumber()] == FALSE && m_Buildings.GetAt(i).GetFollower() != 0)
		// Wenn ein Geb�ude nicht in der baubare Geb�udeliste vorkommt
		for (int j = 0; j < m_BuildableBuildings.GetSize(); j++)
			if (m_BuildableBuildings.GetAt(j) == m_Buildings.GetAt(i).GetRunningNumber())
			{
				stop = TRUE;
				break;
			}
		// es steht ein Geb�ude, welches ich h�tte nicht bauen k�nnen
		if (stop == FALSE)
		{
			// Nun d�rfen wir dieses Geb�ude bauen, d�rfen aber nicht das bis jetzt baubare bauen z.B. es steht erst fab1
			// wir k�nnten aber schon fab3 bauen, dann d�rfen wir fab1 bauen, fab3 aber nicht
			// d.h. wir m�ssen dieses Geb�ude ersetzen
			if (m_Buildings.GetAt(i).GetFoodProd() > 0 && m_Buildings.GetAt(i).GetWorker() == TRUE)
			{
				// Suche nach in der Liste stehender Farm, diese ersetzen!
				okay = FALSE;
				for (int t = 0; t < m_BuildableBuildings.GetSize(); t++)
					if (buildingInfo[m_BuildableBuildings.GetAt(t)].GetFoodProd() > 0 && buildingInfo[m_BuildableBuildings.GetAt(t)].GetWorker() == TRUE)
					{
						m_BuildableBuildings.SetAt(t,m_Buildings.GetAt(i).GetRunningNumber());
						okay = TRUE;
						break;
					}
				if (okay == FALSE)	// kein �hnliches Geb�ude in der Liste gefunden -> nicht ersetzen sondern einf�gen
					m_BuildableBuildings.Add(m_Buildings.GetAt(i).GetRunningNumber());
			}
			else if (m_Buildings.GetAt(i).GetIPProd() > 0 && m_Buildings.GetAt(i).GetWorker() == TRUE)
			{
				// Suche nach in der Liste stehender Fabriken, diese ersetzen!
				okay = FALSE;
				for (int t = 0; t < m_BuildableBuildings.GetSize(); t++)
					if (buildingInfo[m_BuildableBuildings.GetAt(t)].GetIPProd() > 0 && buildingInfo[m_BuildableBuildings.GetAt(t)].GetWorker() == TRUE)
					{
						m_BuildableBuildings.SetAt(t,m_Buildings.GetAt(i).GetRunningNumber());
						okay = TRUE;
						break;
					}
				if (okay == FALSE)	// kein �hnliches Geb�ude in der Liste gefunden -> nicht ersetzen sondern einf�gen
					m_BuildableBuildings.Add(m_Buildings.GetAt(i).GetRunningNumber());
			}
			else if (m_Buildings.GetAt(i).GetEnergyProd() > 0 && m_Buildings.GetAt(i).GetWorker() == TRUE)
			{
				// Suche nach in der Liste stehender Energiedingers, diese ersetzen!
				okay = FALSE;
				for (int t = 0; t < m_BuildableBuildings.GetSize(); t++)
					if (buildingInfo[m_BuildableBuildings.GetAt(t)].GetEnergyProd() > 0 && buildingInfo[m_BuildableBuildings.GetAt(t)].GetWorker() == TRUE)
					{
						m_BuildableBuildings.SetAt(t,m_Buildings.GetAt(i).GetRunningNumber());
						okay = TRUE;
						break;
					}
				if (okay == FALSE)	// kein �hnliches Geb�ude in der Liste gefunden -> nicht ersetzen sondern einf�gen
					m_BuildableBuildings.Add(m_Buildings.GetAt(i).GetRunningNumber());
			}
			else if (m_Buildings.GetAt(i).GetSPProd() > 0 && m_Buildings.GetAt(i).GetWorker() == TRUE)
			{
				// Suche nach in der Liste stehender Intelzentren, diese ersetzen!
				okay = FALSE;
				for (int t = 0; t < m_BuildableBuildings.GetSize(); t++)
					if (buildingInfo[m_BuildableBuildings.GetAt(t)].GetSPProd() > 0 && buildingInfo[m_BuildableBuildings.GetAt(t)].GetWorker() == TRUE)
					{
						m_BuildableBuildings.SetAt(t,m_Buildings.GetAt(i).GetRunningNumber());
						okay = TRUE;
						break;
					}
				if (okay == FALSE)	// kein �hnliches Geb�ude in der Liste gefunden -> nicht ersetzen sondern einf�gen
					m_BuildableBuildings.Add(m_Buildings.GetAt(i).GetRunningNumber());
			}
			else if (m_Buildings.GetAt(i).GetFPProd() > 0 && m_Buildings.GetAt(i).GetWorker() == TRUE)
			{
				// Suche nach in der Liste stehender Laboratorien, diese ersetzen!
				okay = FALSE;
				for (int t = 0; t < m_BuildableBuildings.GetSize(); t++)
					if (buildingInfo[m_BuildableBuildings.GetAt(t)].GetFPProd() > 0 && buildingInfo[m_BuildableBuildings.GetAt(t)].GetWorker() == TRUE)
					{
						m_BuildableBuildings.SetAt(t,m_Buildings.GetAt(i).GetRunningNumber());
						okay = TRUE;
						break;
					}
				if (okay == FALSE)	// kein �hnliches Geb�ude in der Liste gefunden -> nicht ersetzen sondern einf�gen
					m_BuildableBuildings.Add(m_Buildings.GetAt(i).GetRunningNumber());
			}
			else if (m_Buildings.GetAt(i).GetTitanProd() > 0 && m_Buildings.GetAt(i).GetWorker() == TRUE)
			{
				// Suche nach in der Liste stehender Titanminen, diese ersetzen!
				okay = FALSE;
				for (int t = 0; t < m_BuildableBuildings.GetSize(); t++)
					if (buildingInfo[m_BuildableBuildings.GetAt(t)].GetTitanProd() > 0 && buildingInfo[m_BuildableBuildings.GetAt(t)].GetWorker() == TRUE)
					{
						m_BuildableBuildings.SetAt(t,m_Buildings.GetAt(i).GetRunningNumber());
						okay = TRUE;
						break;
					}
				if (okay == FALSE)	// kein �hnliches Geb�ude in der Liste gefunden -> nicht ersetzen sondern einf�gen
					m_BuildableBuildings.Add(m_Buildings.GetAt(i).GetRunningNumber());
			}
			else if (m_Buildings.GetAt(i).GetDeuteriumProd() > 0 && m_Buildings.GetAt(i).GetWorker() == TRUE)
			{
				// Suche nach in der Liste stehender Deuteriumminen, diese ersetzen!
				okay = FALSE;
				for (int t = 0; t < m_BuildableBuildings.GetSize(); t++)
					if (buildingInfo[m_BuildableBuildings.GetAt(t)].GetDeuteriumProd() > 0 && buildingInfo[m_BuildableBuildings.GetAt(t)].GetWorker() == TRUE)
					{
						m_BuildableBuildings.SetAt(t,m_Buildings.GetAt(i).GetRunningNumber());
						okay = TRUE;
						break;
					}
				if (okay == FALSE)	// kein �hnliches Geb�ude in der Liste gefunden -> nicht ersetzen sondern einf�gen
					m_BuildableBuildings.Add(m_Buildings.GetAt(i).GetRunningNumber());
			}
			else if (m_Buildings.GetAt(i).GetDuraniumProd() > 0 && m_Buildings.GetAt(i).GetWorker() == TRUE)
			{
				// Suche nach in der Liste stehender Duraniumminen, diese ersetzen!
				okay = FALSE;
				for (int t = 0; t < m_BuildableBuildings.GetSize(); t++)
					if (buildingInfo[m_BuildableBuildings.GetAt(t)].GetDuraniumProd() > 0 && buildingInfo[m_BuildableBuildings.GetAt(t)].GetWorker() == TRUE)
					{
						m_BuildableBuildings.SetAt(t,m_Buildings.GetAt(i).GetRunningNumber());
						okay = TRUE;
						break;
					}
				if (okay == FALSE)	// kein �hnliches Geb�ude in der Liste gefunden -> nicht ersetzen sondern einf�gen
					m_BuildableBuildings.Add(m_Buildings.GetAt(i).GetRunningNumber());
			}
			else if (m_Buildings.GetAt(i).GetCrystalProd() > 0 && m_Buildings.GetAt(i).GetWorker() == TRUE)
			{
				// Suche nach in der Liste stehender Crystalminen, diese ersetzen!
				okay = FALSE;
				for (int t = 0; t < m_BuildableBuildings.GetSize(); t++)
					if (buildingInfo[m_BuildableBuildings.GetAt(t)].GetCrystalProd() > 0 && buildingInfo[m_BuildableBuildings.GetAt(t)].GetWorker() == TRUE)
					{
						m_BuildableBuildings.SetAt(t,m_Buildings.GetAt(i).GetRunningNumber());
						okay = TRUE;
						break;
					}
				if (okay == FALSE)	// kein �hnliches Geb�ude in der Liste gefunden -> nicht ersetzen sondern einf�gen
					m_BuildableBuildings.Add(m_Buildings.GetAt(i).GetRunningNumber());
			}
			else if (m_Buildings.GetAt(i).GetIridiumProd() > 0 && m_Buildings.GetAt(i).GetWorker() == TRUE)
			{
				// Suche nach in der Liste stehender Iridiumminen, diese ersetzen!
				okay = FALSE;
				for (int t = 0; t < m_BuildableBuildings.GetSize(); t++)
					if (buildingInfo[m_BuildableBuildings.GetAt(t)].GetIridiumProd() > 0 && buildingInfo[m_BuildableBuildings.GetAt(t)].GetWorker() == TRUE)
					{
						m_BuildableBuildings.SetAt(t,m_Buildings.GetAt(i).GetRunningNumber());
						okay = TRUE;
						break;
					}
				if (okay == FALSE)	// kein �hnliches Geb�ude in der Liste gefunden -> nicht ersetzen sondern einf�gen
					m_BuildableBuildings.Add(m_Buildings.GetAt(i).GetRunningNumber());
			}
		}
	}
	// Jetzt noch die baubaren Updates berechnen
	for (int i = 0; i < m_BuildableBuildings.GetSize(); i++)
		// Wenn das Geb�ude Updatebar ist
		if (buildingInfo[m_BuildableBuildings.GetAt(i)].GetIsUpdateable())
		{
			// Dann schauen ob ich dessen nachfolger bauen kann
			if (buildingInfo[m_BuildableBuildings.GetAt(i)+1].GetBioTech() <= empire->GetResearch()->GetBioTech())
				if (buildingInfo[m_BuildableBuildings.GetAt(i)+1].GetEnergyTech() <= empire->GetResearch()->GetEnergyTech())
					if (buildingInfo[m_BuildableBuildings.GetAt(i)+1].GetCompTech() <= empire->GetResearch()->GetCompTech())
						if (buildingInfo[m_BuildableBuildings.GetAt(i)+1].GetPropulsionTech() <= empire->GetResearch()->GetPropulsionTech())
							if (buildingInfo[m_BuildableBuildings.GetAt(i)+1].GetConstructionTech() <= empire->GetResearch()->GetConstructionTech())
								if (buildingInfo[m_BuildableBuildings.GetAt(i)+1].GetWeaponTech() <= empire->GetResearch()->GetWeaponTech())
									m_BuildableUpdates.Add(m_BuildableBuildings.GetAt(i));
		}

/*	for (i = 0; i < m_Buildings.GetSize(); i++)
			// Wenn das Geb�ude Updatebar ist
		if (buildingInfo[(m_Buildings.GetAt(i).GetRunningNumber())].GetIsUpdateable())
		{
			// Dann schauen ob ich dessen nachfolger bauen kann
			if (buildingInfo[(m_Buildings.GetAt(i).GetRunningNumber())+1].GetBioTech() <= empire->GetResearch()->GetBioTech())
				if (buildingInfo[(m_Buildings.GetAt(i).GetRunningNumber())+1].GetEnergyTech() <= empire->GetResearch()->GetEnergyTech())
					if (buildingInfo[(m_Buildings.GetAt(i).GetRunningNumber())+1].GetCompTech() <= empire->GetResearch()->GetCompTech())
						if (buildingInfo[(m_Buildings.GetAt(i).GetRunningNumber())+1].GetPropulsionTech() <= empire->GetResearch()->GetPropulsionTech())
							if (buildingInfo[(m_Buildings.GetAt(i).GetRunningNumber())+1].GetConstructionTech() <= empire->GetResearch()->GetConstructionTech())
								if (buildingInfo[(m_Buildings.GetAt(i).GetRunningNumber())+1].GetWeaponTech() <= empire->GetResearch()->GetWeaponTech())
									m_BuildableUpdates.Add((m_Buildings.GetAt(i).GetRunningNumber()));
		}*/




	// Wenn wir ein Update in der Bauliste haben, dann mu� dieses aus der Bauliste
	// verschwinden. Auch das zugeh�rige Geb�ude mu� verschwinden
/*	for (int j = 0; j < ALE; j++)
		if (m_iAssemblyList[j] < 0)
		{
			for (int t = 0; t < m_BuildableBuildings.GetSize(); t++)
				if (m_iAssemblyList[j]*(-1) == m_BuildableBuildings.GetAt(t))
				{
					m_BuildableBuildings.RemoveAt(t);
					break;
				}
			for (int t = 0; t < m_BuildableUpdates.GetSize(); t++)
				if (m_iAssemblyList[j]*(-1) == m_BuildableUpdates.GetAt(t))
				{
					m_BuildableUpdates.RemoveAt(t);
					break;
				}
		}
}
*/
/*
// statische Testfunktion
static BOOLEAN AssemblyListCheck(BuildingInfoArray* infos, CSystem systems[][20], int ID, CPoint systemKO)
{
	for (int y = 0; y < 20; y++)
		for (int x = 0; x < 30; x++)
			if (systems[x][y].GetAssemblyList()->GetAssemblyListEntry(0) == ID && ID != 0)
				return TRUE;
	
	return FALSE;
}*/