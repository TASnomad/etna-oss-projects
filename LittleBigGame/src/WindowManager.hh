#ifndef	__WINDOW_MANAGER_HH__
#define	__WINDOW_MANAGER_HH__

#include	<SFML/Graphics.hpp>
#include	"Scene.hh"

class WindowManager
{
	private:
		static WindowManager _instance;
		sf::RenderWindow *_mainWindow;
		Scene	*_mainScene;
		WindowManager();
		WindowManager(const WindowManager &);
		WindowManager& operator=(const WindowManager &) = delete;

	public:
		~WindowManager();
		static WindowManager &GetInstance();
		void InitWindow(std::string, unsigned int, unsigned int, unsigned int = 30);
		sf::RenderWindow *GetWindow();
		Scene *GetScene();
};

#endif	/* !__WINDOW_MANAGER_HH__ */