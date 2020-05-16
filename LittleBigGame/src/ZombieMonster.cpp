#include "ZombieMonster.hh"

ZombieMonster::ZombieMonster(): Character(0.0, 0.0, 100, 1.0 / 1000.0)
{ }

ZombieMonster::ZombieMonster(double x, double y, int l, double s) :
	Character(x, y, l, s)
{ }

ZombieMonster::~ZombieMonster() { }

void ZombieMonster::Update()
{
	double pos;

	pos = (double) TimeManager::GetInstance().GetElapsedTime() * GetSpeed();
	SetX(GetX() + pos);
	SetY(GetY() + pos);
}

std::string &ZombieMonster::Serialize()
{
	std::string *tmp = new std::string();
	cJSON		*root = nullptr;

	tmp->assign(Character::Serialize());
	root = cJSON_Parse(tmp->c_str());
	if (root)
		cJSON_AddItemToObject(root, "class", cJSON_CreateString("ZombieMonster"));
	tmp->assign(cJSON_Print(root));
	return (*tmp);
}
