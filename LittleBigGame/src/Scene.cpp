#include "Scene.hh"
#include "MonsterFactory.hh"

Scene::Scene() { }

Scene::Scene(const std::string &serializedScene)
{
	MonsterFactory mf = MonsterFactory();
	std::string	buffer;
	cJSON		*elements = cJSON_Parse(serializedScene.c_str());
	cJSON		*tmp = nullptr;
	int 		size = cJSON_GetArraySize(elements);

	for (int i = 0; i < size; i++)
	{
		tmp = cJSON_GetArrayItem(elements, i);
		buffer = std::string(cJSON_Print(tmp));
		AddEntity(mf.Unserialize(buffer));
	}
}

Scene::~Scene() { }

std::list<AbstractEntity *> Scene::GetEntities() const
{
	return this->_list;
}

void Scene::AddEntity(AbstractEntity *entity)
{
	this->_list.push_back(entity);
}

void Scene::RemoveEntity(AbstractEntity *entity)
{
	this->_list.remove(entity);
}

void Scene::Update()
{
	TimeManager::GetInstance().Update();
	for (AbstractEntity *one : _list)
		one->Update();
	for (AbstractEntity *one: _list)
		one->Draw();
}

std::string &Scene::Serialize()
{
	std::string *buffer = new std::string();
	cJSON		*elements = cJSON_CreateArray();
	cJSON		*tmp = nullptr;

	for (AbstractEntity *e : _list)
	{
		tmp = cJSON_Parse(e->Serialize().c_str());
		cJSON_AddItemToArray(elements, tmp);
	}
	buffer->assign(cJSON_Print(elements));
	return (*buffer);
}
