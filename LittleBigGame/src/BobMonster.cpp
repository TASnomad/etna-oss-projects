#include "BobMonster.hh"

BobMonster::BobMonster(): Character(0.0, 0.0, 1, 1.0 / 1000.0)
{ }

BobMonster::BobMonster(double x, double y, int l, double s) :
	Character(x, y, l, s, "../assets/Monster/PGN/bob.png")
{ }

BobMonster::~BobMonster() { }

void BobMonster::Update()
{
	double pos;

	pos = (double) TimeManager::GetInstance().GetElapsedTime() * GetSpeed();
	SetX(GetX() + pos);
	SetY(GetY() + pos);
}

std::string &BobMonster::Serialize()
{
	std::string *tmp = new std::string();
	cJSON		*root = nullptr;

	tmp->assign(Character::Serialize());
	root = cJSON_Parse(tmp->c_str());
	if (root)
		cJSON_AddItemToObject(root, "class", cJSON_CreateString("BobMonster"));
	tmp->assign(cJSON_Print(root));
	return (*tmp);
}
