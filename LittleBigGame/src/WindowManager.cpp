#include "WindowManager.hh"

WindowManager WindowManager::_instance = WindowManager();

WindowManager::WindowManager(): _mainScene(new Scene()) { }

WindowManager::~WindowManager() { }

void WindowManager::InitWindow(std::string title, unsigned int width, unsigned int height, unsigned int fps)
{
	_mainWindow = new sf::RenderWindow(sf::VideoMode(width, height), title);
	_mainWindow->setFramerateLimit(fps);
}

WindowManager &WindowManager::GetInstance()
{
	return _instance;
}

sf::RenderWindow *WindowManager::GetWindow()
{
	return _mainWindow;
}

Scene *WindowManager::GetScene()
{
	return _mainScene;
}