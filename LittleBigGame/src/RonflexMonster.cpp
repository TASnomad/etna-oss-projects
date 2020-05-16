#include "RonflexMonster.hh"

RonflexMonster::RonflexMonster(): Character(0.0, 0.0, 1, 1.0 / 1000.0)
{ }

RonflexMonster::RonflexMonster(double x, double y, int l, double s) :
	Character(x, y, l, s, "../assets/Monster/PGN/ronflex.png")
{ }

RonflexMonster::~RonflexMonster() { }

void RonflexMonster::Update()
{
	double pos;

	pos = (double) TimeManager::GetInstance().GetElapsedTime() * GetSpeed();
	SetX(GetX() + pos);
	SetY(GetY() + pos);
}

std::string &RonflexMonster::Serialize()
{
	std::string *tmp = new std::string();
	cJSON		*root = nullptr;

	tmp->assign(Character::Serialize());
	root = cJSON_Parse(tmp->c_str());
	if (root)
		cJSON_AddItemToObject(root, "class", cJSON_CreateString("RonflexMonster"));
	tmp->assign(cJSON_Print(root));
	return (*tmp);
}
