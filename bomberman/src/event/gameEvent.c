#include <gameEvent.h>
#include <utils.h>

void handleEvent(t_window *window, SDL_Event e)
{
	if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE)
	{
		window->still_running = 0;
		printf("Bye guys \n");
	}
	
	if (e.type == SDL_MOUSEBUTTONUP) {
		handleMouseClick(window, e.button.x, e.button.y);
	}
	if (window->screen_type == WINDOW_CHOOSE_SERVER_SCREEN)
	{
		handleChooseServerEvents(window, e);
	}

	/* if (window->screen_type == WINDOW_GAME_SCREEN)
	{
		handleGameScreenEvents(window, e);
	} */
	if (window->toDisplay == 1) handle_display(window);

	if (window->screen_type == WINDOW_PLAYER_WAITING_SCREEN || window->screen_type == WINDOW_SERVER_WAITING_SCREEN)
	{
		if (window->refreshTime % 5 == 0 && wait_until_game_start(window))
		{
			puts("Can play\n");
			window->screen_type = WINDOW_GAME_SCREEN;
			window->toDisplay = 1;
			SDL_RenderClear(window->renderer);
			window->refreshTime = 1;
		}
		window->refreshTime++;
	}
}