#include <gameEvent.h>
void handleHomeScreenClick(t_window *window, int x, int y)
{
    int xButton;
    int yButton;
    int x2Button;
    int y2Button;

    xButton = BUTTON_HOME_SCREEN_X;
    yButton = BUTTON_HOME_SCREEN_Y;
    x2Button = BUTTON_HOME_SCREEN_X + BUTTON_HOME_SCREEN_W;
    y2Button = BUTTON_HOME_SCREEN_Y + BUTTON_HOME_SCREEN_H;

    if ((x >=  xButton && x <= x2Button))
    {
        if (y >= yButton && y <= y2Button)
        {
            if (window->serv == NULL)
            {
                window->serv = ctor_server("8081");

                if (window->serv != NULL)
                {
                    pthread_create(window->serv_thr, NULL, server_thread_entry, window->serv);
                    window->screen_type = WINDOW_SERVER_WAITING_SCREEN;
                    window->toDisplay = 1;
                }
                else
                {
                    window->still_running = 0;
                }
            }
        }

        if (y >= (yButton + BUTTON_HOME_SCREEN_H + BUTTON_SPACE) && y <= (y2Button + 2 * BUTTON_HOME_SCREEN_H + BUTTON_SPACE))
        {
            window->screen_type = WINDOW_CHOOSE_SERVER_SCREEN;
            window->toDisplay = 1;
        }
    }
    window->toDisplay = 1;
}