#ifndef	__MONSTERFACTORY_H__
#define	__MONSTERFACTORY_H__

#include "Character.hh"
#include "ZombieMonster.hh"
#include "SprinterMonster.hh"
#include "BobMonster.hh"
#include "RonflexMonster.hh"
#include "PlanktonMonster.hh"

#include <string>

class MonsterFactory
{
	public:
		MonsterFactory();
		~MonsterFactory();
		Character *Create(const std::string &);
		Character *Unserialize(const std::string &);
};

#endif	/* !__MONSTERFACTORY_H__ */
