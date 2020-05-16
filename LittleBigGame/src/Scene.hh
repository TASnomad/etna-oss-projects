#ifndef	__SCENE_HH__
#define	__SCENE_HH__

#include "AbstractEntity.hh"
#include "ZombieMonster.hh"
#include "SprinterMonster.hh"
#include "TimeManager.hh"
#include <list>

class Scene : public ISerializable
{
	private:
		std::list<AbstractEntity *> _list;
	public:
		Scene();
		Scene(const std::string &);
		virtual ~Scene();
		std::list<AbstractEntity *> GetEntities() const;
		void AddEntity(AbstractEntity *);
		void RemoveEntity(AbstractEntity *);
		void Update();
		virtual std::string &Serialize() override;
};

#endif	/* !__SCENE_HH__ */
