#include "stdafx.h"
#include "Test.h"

#include "BotEDoc.h"
#include "Ships/Ships.h"
#include "Races/RaceController.h"



CTest::CTest(const CBotEDoc& doc) :
	m_Doc(doc)
{
}

CTest::~CTest(void)
{
}

const CTest* const CTest::GetInstance(const CBotEDoc& doc) {
	static CTest instance(doc);
	return &instance;
}

void CTest::Run() const {
	if(MT::CMyTrace::IsLoggingEnabledFor("shipmap"))
		TestShipMap();
	if(MT::CMyTrace::IsLoggingEnabledFor("genshipname"))
		TestGenShipname();
}

void CTest::TestGenShipname() const {
	CGenShipName gsn(m_Doc.m_GenShipName);

	//check uniqueness of generated shipnames
	const CRaceController& race_ctrl = *m_Doc.m_pRaceCtrl;
	CString sRaceID = "";
	CString sRaceName = "";
	for(int j = 0; j < 10; ++j)
	{
		const unsigned at = rand()%race_ctrl.size();
		unsigned index = 0;
		for(CRaceController::const_iterator i = race_ctrl.begin(); i != race_ctrl.end(); ++i)
		{
			if(at != index) {
				++index;
				continue;
			}
			sRaceID = i->second->GetRaceID();
			sRaceName = i->second->GetName();
			break;
		}

		std::set<CString> seen;
		std::vector<CString> seen_v;
		for(unsigned i = 0; i < 10000; ++i)
		{
			const CString& name = gsn.GenerateShipName(sRaceID, sRaceName, false);
			if(seen.find(name) != seen.end()) {
				CString s;
				s.Format("repeated name: %s", name);
				MYTRACE("genshipname")(MT::LEVEL_ERROR, s);
				break;
			}
			seen.insert(name);
			seen_v.push_back(name);
		}
	}
}

void CTest::TestShipMap() const {

	{
		const CShipMap shipmap(m_Doc.m_ShipMap);

		//////////////////////////////////////////////////////////////////////
		// special ships
		//////////////////////////////////////////////////////////////////////
		//special ships should be at the beginning
		if(shipmap.CurrentShip() != shipmap.begin())
			MYTRACE("shipmap")(MT::LEVEL_ERROR, "CurrentShip not at begin()");
		if(shipmap.FleetShip() != shipmap.begin())
			MYTRACE("shipmap")(MT::LEVEL_ERROR, "FleetShip not at begin()");

		//////////////////////////////////////////////////////////////////////
		// iterator_at
		//////////////////////////////////////////////////////////////////////
		//directly after creation, keys are the same as indices
		//check iterator_at(index) against at(key)
		const CShipMap::const_iterator& ci = shipmap.iterator_at(0);
		if(shipmap.at(0)->GetName() != ci->second->GetName())
			MYTRACE("shipmap")(MT::LEVEL_ERROR, "iterator at index 0 should point to ship with key 0");

		const CShipMap::const_iterator& ci2 = shipmap.iterator_at(m_Doc.m_ShipMap.GetUpperBound());
		if(shipmap.at(m_Doc.m_ShipMap.GetUpperBound())->GetName() != ci2->second->GetName())
			MYTRACE("shipmap")(MT::LEVEL_ERROR, "iterator at max index should point to ship with key max index");

		//check iterator_at(index) against index_of()
		//should allow getting an end() iterator
		for(CShipMap::const_iterator i = shipmap.begin();;) {
			if(i == shipmap.iterator_at(shipmap.index_of(i))) {
				if(i == shipmap.end())
					break;
				++i;
				continue;
			}
			MYTRACE("shipmap")(MT::LEVEL_ERROR, "i != shipmap.iterator_at(shipmap.index_of(i)");
			if(i == shipmap.end())
				break;
			++i;
		}
	}

	//////////////////////////////////////////////////////////////////////
	// empty shipmap
	//////////////////////////////////////////////////////////////////////

	{

		const CShipMap shipmap2;
		if(shipmap2.end() != shipmap2.begin())
			MYTRACE("shipmap")(MT::LEVEL_ERROR, "shipmap.end() != shipmap2.begin()");

		const CShipMap::const_iterator& i4 = shipmap2.iterator_at(0);
		if(i4 != shipmap2.begin())
			MYTRACE("shipmap")(MT::LEVEL_ERROR, "i4 != shipmap2.begin()");
		const CShipMap::const_iterator& i5 = shipmap2.iterator_at(0);
		if(i5 != shipmap2.end())
			MYTRACE("shipmap")(MT::LEVEL_ERROR, "i5 != shipmap2.end()");

	}

	//////////////////////////////////////////////////////////////////////
	// one element
	//////////////////////////////////////////////////////////////////////

	CShipMap shipmap3;
	const boost::shared_ptr<CShips> s(new CShips(static_cast<CShip>(m_Doc.m_ShipInfoArray.GetAt(0))));
	shipmap3.Add(s);

	const CShips* ship = &shipmap3.GetAt(0);
	if(ship != shipmap3.begin()->second.get())
		MYTRACE("shipmap")(MT::LEVEL_ERROR, "&ship != &shipmap3.begin()->second");
	const CShipMap::const_iterator& i6 = shipmap3.iterator_at(1);
	if(i6 != shipmap3.end())
		MYTRACE("shipmap")(MT::LEVEL_ERROR, "i6 != shipmap3.end()");
	if(&shipmap3.CurrentShip()->second != &shipmap3.begin()->second)
		MYTRACE("shipmap")(MT::LEVEL_ERROR, "&shipmap3.CurrentShip()->second != &shipmap3.begin()->second");
}
