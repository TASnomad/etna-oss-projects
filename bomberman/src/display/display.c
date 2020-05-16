#include <window.h>
#include <utils.h>

void handle_display(t_window *window)
{
    if (window == NULL) return;
    switch (window->screen_type)
    {
        case WINDOW_INIT_LOADING_SCREEN :
            display_loading_screen(window);
            break;
        case WINDOW_HOME_SCREEN :
            display_home_screen(window);
            break;
        case WINDOW_SERVER_WAITING_SCREEN :
            display_server_waiting_screen(window);
			if (connect_to_server(window, "127.0.0.1", "8081") == -1)
			{
				window->still_running = 0;
			}
            break;
		case WINDOW_GAME_SCREEN :
			display_game_screen(window);
			break;
        case WINDOW_CHOOSE_SERVER_SCREEN :
            display_choose_server_screen(window);
            break;
        case WINDOW_PLAYER_WAITING_SCREEN:
            display_loading_party_screen(window);
            break;
        default :
            break;
    }
}