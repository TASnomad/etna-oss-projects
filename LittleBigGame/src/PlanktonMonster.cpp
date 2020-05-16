#include "PlanktonMonster.hh"

PlanktonMonster::PlanktonMonster(): Character(0.0, 0.0, 1, 1.0 / 1000.0)
{ }

PlanktonMonster::PlanktonMonster(double x, double y, int l, double s) :
	Character(x, y, l, s, "../assets/Monster/PGN/plankton.png")
{ }

PlanktonMonster::~PlanktonMonster() { }

void PlanktonMonster::Update()
{
	double pos;

	pos = (double) TimeManager::GetInstance().GetElapsedTime() * GetSpeed();
	SetX(GetX() + pos);
	SetY(GetY() + pos);
}

std::string &PlanktonMonster::Serialize()
{
	std::string *tmp = new std::string();
	cJSON		*root = nullptr;

	tmp->assign(Character::Serialize());
	root = cJSON_Parse(tmp->c_str());
	if (root)
		cJSON_AddItemToObject(root, "class", cJSON_CreateString("PlanktonMonster"));
	tmp->assign(cJSON_Print(root));
	return (*tmp);
}
