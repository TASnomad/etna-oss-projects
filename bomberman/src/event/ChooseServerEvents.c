#include <gameEvent.h>
#include <utils.h>

void handleChooseServerEvents(t_window *window, SDL_Event e)
{
    if (e.type == SDL_TEXTINPUT)
    {
        strcat(window->ipInput, e.text.text);
        window->toDisplay = 1;
    }
    else if (e.key.keysym.sym == SDLK_BACKSPACE)
    {
        window->keyBoardLock++;
        if (window->keyBoardLock >= 2)
        {
            window->keyBoardLock = 0;
            return;
        }
        window->ipInput = deleteLastChar(window->ipInput);
        window->toDisplay = 1;
    }
}


void handleClickChooseServer(t_window *window, int x, int y)
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
        if (y >= (yButton + BUTTON_HOME_SCREEN_H + BUTTON_SPACE) && y <= (y2Button + 2 * BUTTON_HOME_SCREEN_H + BUTTON_SPACE))
        {
            window->screen_type = WINDOW_CHOOSE_SERVER_SCREEN;
            if (connect_to_server(window, window->ipInput, "8081") != -1)
            {
                window->screen_type = WINDOW_PLAYER_WAITING_SCREEN;
            }
        }
    }
    window->toDisplay = 1;
}