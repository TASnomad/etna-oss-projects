#ifndef __GAME_EVENT_H__
#define __GAME_EVENT_H__

#include <SDL2/SDL.h>
#include <window.h>
void handleEvent(t_window *window, SDL_Event e);
void handleChooseServerEvents(t_window *window, SDL_Event e);
void handleClickChooseServer(t_window *window, int x, int y);
void handleHomeScreenClick(t_window *window, int x, int y);
void handleMouseClick(t_window *window, int x, int y);
#endif /* !__GAME_EVENT_H__ */