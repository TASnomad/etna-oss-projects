#include <gameEvent.h>

void handleMouseClick(t_window *window, int x, int y)
{
    SDL_Rect r;

    r.x = (x - 25) * window->factor;
    r.y = (y - 25) * window->factor;
    r.w = 100;
    r.h = 100;

    if (window == NULL) return;
    switch (window->screen_type)
    {
        case WINDOW_INIT_LOADING_SCREEN :
            display_loading_screen(window);
            display_home_screen(window);
            break;
        case WINDOW_HOME_SCREEN :
            display_home_screen(window);
            handleHomeScreenClick(window, x, y);
			break;
        case WINDOW_CHOOSE_SERVER_SCREEN :
            display_choose_server_screen(window);
            handleClickChooseServer(window, x, y);
        default :
            break;
    }
    //SDL_RenderPresent(window->renderer);
}