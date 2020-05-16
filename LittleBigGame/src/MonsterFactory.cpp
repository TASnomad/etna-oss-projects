#include "MonsterFactory.hh"

MonsterFactory::MonsterFactory() { }

MonsterFactory::~MonsterFactory() { }


Character *MonsterFactory::Create(const std::string &monsterName)
{
	Character *res = nullptr;

	if (monsterName == "ZombieMonster")
		res = new ZombieMonster();
	if (monsterName == "SprinterMonster")
		res = new SprinterMonster();
	if (monsterName == "BobMonster")
		res = new BobMonster();
	if (monsterName == "RonflexMonster")
		res = new RonflexMonster();
	return res;
}

Character *MonsterFactory::Unserialize(const std::string &monster)
{
	Character *res = nullptr;
	std::string *buf = new std::string();
	cJSON		*root = nullptr;
	cJSON		*className = nullptr;

	buf->assign(monster);
	root = cJSON_Parse(buf->c_str());
	className = cJSON_GetObjectItemCaseSensitive(root, "class");
	if (cJSON_IsString(className))
	{
		cJSON *x = cJSON_GetObjectItemCaseSensitive(root, "x");
		cJSON *y = cJSON_GetObjectItemCaseSensitive(root, "y");
		cJSON *life = cJSON_GetObjectItemCaseSensitive(root, "life");
		cJSON *speed = cJSON_GetObjectItemCaseSensitive(root, "speed");
		if (std::string(className->valuestring) == "SprinterMonster")
			res = new SprinterMonster(x->valuedouble, y->valuedouble, life->valueint, speed->valuedouble);

		if (std::string(className->valuestring) == "ZombieMonster")
			res = new ZombieMonster(x->valuedouble, y->valuedouble, life->valueint, speed->valuedouble);
		if (std::string(className->valuestring) == "BobMonster")
			res = new BobMonster(x->valuedouble, y->valuedouble, life->valueint, speed->valuedouble);
		if (std::string(className->valuestring) == "RonflexMonster")
			res = new RonflexMonster(x->valuedouble, y->valuedouble, life->valueint, speed->valuedouble);
		if (std::string(className->valuestring) == "PlanktonMonster")
			res = new PlanktonMonster(x->valuedouble, y->valuedouble, life->valueint, speed->valuedouble);
	}
	return res;
}
