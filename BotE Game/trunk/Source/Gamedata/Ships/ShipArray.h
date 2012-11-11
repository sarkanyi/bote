#pragma once

#include <vector>

class CShip;

class CShipArray
{
public:
//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
	CShipArray(void);
	~CShipArray(void);

//////////////////////////////////////////////////////////////////////
// iterators
//////////////////////////////////////////////////////////////////////
	typedef std::vector<CShip>::const_iterator const_iterator;
	const_iterator begin() const;
	const_iterator end() const;

	typedef std::vector<CShip>::iterator iterator;
	iterator begin();
	iterator end();

//////////////////////////////////////////////////////////////////////
// adding elements
//////////////////////////////////////////////////////////////////////
	//adds the passed ship at the end of this shiparray
	//@param it: will be updated and point to the same position as before
	//@param ship: the ship to add
	void Add(CShipArray::iterator& it, const CShip& ship);
	void Append(CShipArray::iterator& it, const CShipArray& other);
//////////////////////////////////////////////////////////////////////
// removing elements
//////////////////////////////////////////////////////////////////////
	void RemoveAll();
	//removes the element pointed to by the passed iterator
	//@param index: will be updated and point to the new position of the element which followed the erased one
	void RemoveAt(CShipArray::iterator& index);
//////////////////////////////////////////////////////////////////////
// getting elements
//////////////////////////////////////////////////////////////////////
	CShip& GetAt(int index);
	const CShip& GetAt(int index) const;
	CShip& ElementAt(int index);
	const CShip& ElementAt(int index) const;
	CShip& operator[](int index);
	const CShip& operator[](int index) const;

//////////////////////////////////////////////////////////////////////
// getting info
//////////////////////////////////////////////////////////////////////
	int GetSize() const;
	int GetUpperBound() const;
	bool empty() const;

//////////////////////////////////////////////////////////////////////
// Serialisierungsfunktionen
//////////////////////////////////////////////////////////////////////
	void Serialize(CArchive& ar);
	//special purpose serialization function
	void SerializeEndOfRoundData(CArchive& ar, const CString& sMajorID);
	//special purpose serialization function
	void SerializeNextRoundData(CArchive& ar, const CPoint& ptCurrentCombatSector);


private:
	void SetSize(int size);

	std::vector<CShip> m_vShips;
};
