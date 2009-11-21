/*
 *   Copyright (C)2004-2009 Sir Pustekuchen
 *
 *   Author   :  Sir Pustekuchen
 *   Home     :  http://birth-of-the-empires.de.vu
 *
 */
// GenShipName.h: Schnittstelle f�r die Klasse CGenShipName.
//
//////////////////////////////////////////////////////////////////////
#pragma once
#include <map>
#include <vector>

using namespace std;

// forwar declaration
class CBotf2Doc;

class CGenShipName : public CObject  
{
public:
	DECLARE_SERIAL (CGenShipName)
	
	/// ctor
	CGenShipName();
	
	/// dtor
	virtual ~CGenShipName();
	
	/// Die Serialisierungsfunktion
	virtual void Serialize(CArchive &ar);

	/// Funktion zum einmaligen Einlesen der ganzen Schiffsnamen.
	void Init(CBotf2Doc* pDoc);
	
	/// Diese Funktion generiert einen einmaligen Schiffsnamen. Als Parameter werden daf�r die Rasse <code>sRaceID</code>
	/// und ein Parameter, welcher angibt ob es sich um eine Station handelt <code>station</code> �bergeben.
	CString GenerateShipName(const CString& sRaceID, BOOLEAN station);

private:
	/// Feld aller Schiffsnamen
	map<CString, vector<CString> > m_mShipNames;
	
	/// Feld aller benutzten Schiffsnamen
	map<CString, vector<CString> > m_mUsedNames;
	
	/// Z�hlvariable, welche die Buchstabenbennung am Ende vom Schiffsnamen vornimmt. Zu Beginn auf 'A' gesetzt.
	map<CString, USHORT> m_mCounter;
};