#pragma once

class CShip;
class CSector;
class CSystem;
class CBotf2Doc;


class CSanity
{
	private:
		CSanity(void);
	public:
		~CSanity(void);


	static void SanityCheckFleet(const CShip& ship);
	static void SanityCheckSectorAndSystem(
		const CSector& sector,
		const CSystem& system,
		const CBotf2Doc& doc);
	static void ShipInfo(const CArray<CShip, CShip>& shiparray, int index, const CString& indexname);
	static void ShipArray(const CBotf2Doc& doc);


};
