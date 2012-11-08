#include "stdafx.h"

#include "ShipArray.h"
#include "Ships/ship.h"

#include <cassert>


CShipArray::CShipArray(void) :
	m_vShips()
{
}

CShipArray::~CShipArray(void)
{
}

CShipArray::const_iterator CShipArray::begin() const {
	return m_vShips.begin();
}
CShipArray::const_iterator CShipArray::end() const {
	return m_vShips.end();
}

CShipArray::iterator CShipArray::begin() {
	return m_vShips.begin();
}
CShipArray::iterator CShipArray::end() {
	return m_vShips.end();
}

void CShipArray::Add(const CShip& ship, CShipArray::const_iterator& it) {
	const int memory = it - begin();
	m_vShips.push_back(ship);
	it = begin() + memory;
}
void CShipArray::Add(const CShip& ship) {
	m_vShips.push_back(ship);
}

void CShipArray::RemoveAll() {
	m_vShips.clear();
}
void CShipArray::RemoveAt(int index) {
	const std::vector<CShip>::const_iterator it = m_vShips.begin() + index;
	m_vShips.erase(it);
}

CShip& CShipArray::GetAt(int index) {
	return m_vShips.at(index);
}
const CShip& CShipArray::GetAt(int index) const {
	return m_vShips.at(index);
}
CShip& CShipArray::ElementAt(int index) {
	return m_vShips.at(index);
}
const CShip& CShipArray::ElementAt(int index) const {
	return m_vShips.at(index);
}
CShip& CShipArray::operator[](int index) {
	return m_vShips.at(index);
}
const CShip& CShipArray::operator[](int index) const {
	return m_vShips.at(index);
}

int CShipArray::GetUpperBound() const {
	return m_vShips.size() - 1;
}
int CShipArray::GetSize() const {
	return m_vShips.size();
}
bool CShipArray::empty() const {
	return m_vShips.empty();
}
void CShipArray::SetSize(int size) {
	m_vShips.resize(size);
}

void CShipArray::Serialize(CArchive& ar) {
	if(ar.IsStoring()) {
		ar << m_vShips.size();
		for(std::vector<CShip>::iterator it = m_vShips.begin(); it != m_vShips.end(); ++it)
			it->Serialize(ar);
	}
	if(ar.IsLoading()) {
		int size;
		ar >> size;
		//We need to call clear(), since resize() calls destructors only for elements from after the new
		//lenght (if new size is smaller), and apparently the >> operator allocates stuff with new
		//with the CString members of the ship class in their Serialize functions above.
		//Thus, call the destructors of all the current elements explicitely and manually.
		m_vShips.clear();
		m_vShips.resize(size);
		for(std::vector<CShip>::iterator it = m_vShips.begin(); it != m_vShips.end(); ++it)
			it->Serialize(ar);
	}
}

void CShipArray::SerializeEndOfRoundData(CArchive& ar, const CString& sMajorID) {
	if(ar.IsStoring()) {
		std::vector<int> vShips;
		for (int i = 0; i < GetSize(); i++)
			if (GetAt(i).GetOwnerOfShip() == sMajorID)
				vShips.push_back(i);
		ar << vShips.size();
		for (size_t i = 0; i < vShips.size(); i++)
			GetAt(vShips[i]).Serialize(ar);
	}

	if(ar.IsLoading()) {
		int number = 0;
		ar >> number;
		for (int i = 0; i < GetSize(); i++)
			if (GetAt(i).GetOwnerOfShip() == sMajorID)
				RemoveAt(i--);
		int oldSize = GetSize();
		SetSize(oldSize + number);
		for (int i = oldSize; i < GetSize(); i++)
		{
			GetAt(i).Serialize(ar);
			assert(GetAt(i).GetOwnerOfShip() == sMajorID);
		}
	}
}

void CShipArray::SerializeNextRoundData(CArchive& ar, const CPoint& ptCurrentCombatSector) {
	if(ar.IsStoring()) {
		int nCount = 0;
		for (int i = 0; i < GetSize(); i++)
			if (GetAt(i).GetKO() == ptCurrentCombatSector)
				nCount++;
		ar << nCount;
		// nur Schiffe aus diesem Sektor senden
		for (int i = 0; i < GetSize(); i++)
			if (GetAt(i).GetKO() == ptCurrentCombatSector)
				GetAt(i).Serialize(ar);
	}

	if(ar.IsLoading()) {
		// Es werden nur Schiffe aus dem aktuellen Kampfsektor empfangen
		int nCount = 0;
		ar >> nCount;
		// alle Schiffe aus dem Kampfsektor entfernen
		for (int i = 0; i < GetSize(); i++)
			if (GetAt(i).GetKO() == ptCurrentCombatSector)
				RemoveAt(i--);
		int nSize = GetSize();
		// empfangene Schiffe wieder hinzufügen
		SetSize(nSize + nCount);
		for (int i = nSize; i < GetSize(); i++)
			GetAt(i).Serialize(ar);
	}
}
