#ifndef	__PLANKTONMONSTER_HH__
#define	__PLANKTONMONSTER_HH__

#include "Character.hh"
#include "TimeManager.hh"

class PlanktonMonster: public Character
{
	public:
		PlanktonMonster();
		PlanktonMonster(double, double, int, double);
		~PlanktonMonster();
		virtual void Update() override;
		virtual std::string &Serialize() override;
};

#endif	/* !__PLANKTONMONSTER_HH__ */
