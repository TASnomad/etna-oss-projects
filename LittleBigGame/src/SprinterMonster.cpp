#include "SprinterMonster.hh"

SprinterMonster::SprinterMonster(): Character(0.0, 0.0, 25, 5.0 / 1000.0)
{ }

SprinterMonster::SprinterMonster(double x, double y, int l, double s) :
	Character(x, y, l, s)
{ }

SprinterMonster::~SprinterMonster() { }

void SprinterMonster::Update()
{
	double pos;

	pos = (double) TimeManager::GetInstance().GetElapsedTime() * GetSpeed();
	SetX(GetX() + pos);
	SetY(GetY() + pos);
}

std::string &SprinterMonster::Serialize()
{
	std::string *tmp = new std::string();
	cJSON		*root = nullptr;

	tmp->assign(Character::Serialize());
	root = cJSON_Parse(tmp->c_str());
	if (root)
		cJSON_AddItemToObject(root, "class", cJSON_CreateString("SprinterMonster"));
	tmp->assign(cJSON_Print(root));
	return (*tmp);
}
