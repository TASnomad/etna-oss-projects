#include <window.h>

void display_server_waiting_screen(t_window *window)
{
    SDL_RenderClear(window->renderer);
    SDL_Rect    r;
    SDL_Color   bgScreen;

    if (window->toDisplay == 0) return;
    r.x = 0;
    r.y = 0;
    r.w = getPXSizeForWindow(window->factor, 800);
    r.h = getPXSizeForWindow(window->factor, 600);

    bgScreen = getServerWaitingScreenbackgroundColor();
    SDL_SetRenderDrawColor(window->renderer, bgScreen.r, bgScreen.g, bgScreen.b, bgScreen.a);
    SDL_RenderFillRect(window->renderer, &r);
    display_server_waiting_buttons(window);
    SDL_RenderPresent(window->renderer);
    window->toDisplay = 0;
}

SDL_Color getServerWaitingScreenbackgroundColor()
{
    SDL_Color c;
    c.r = 189;
    c.g = 189;
    c.b = 189;

    return c;
}

SDL_Color getServerWaitingScreenButtonBgColor()
{
    SDL_Color c;

    c.r = 13;
    c.g = 71;
    c.b = 161;

    return c;
}

SDL_Color getServerWaitingScreenButtonFgColor()
{
    SDL_Color c;

    c.r = 227;
    c.g = 242;
    c.b = 253;

    return c;
}

void display_server_waiting_buttons(t_window *window)
{
    SDL_Rect    r;
    SDL_Color   buttonBg;
    SDL_Color   buttonFg;
    t_button    *button;
    int         i;
    //int         len;
    char        **res;
    char        **saveRes;

    r.x = getPXSizeForWindow(window->factor, BUTTON_HOME_SCREEN_X);
    r.y = getPXSizeForWindow(window->factor, BUTTON_HOME_SCREEN_Y - BUTTON_HOME_SCREEN_H);
    r.w = getPXSizeForWindow(window->factor, BUTTON_HOME_SCREEN_W);
    r.h = getPXSizeForWindow(window->factor, BUTTON_HOME_SCREEN_H);
    i = 0;

    buttonBg = getServerWaitingScreenbackgroundColor();
    buttonFg = getServerWaitingScreenButtonFgColor();

    button = createButton();
    initButton(button, NULL, "En attente de joueurs... Ip du serveur :", r, buttonBg, buttonFg);
    drawButton(button, window);

    res = get_server_ip(window->serv);
    saveRes = res;
    while (*res)
    {
        char *str = *res;
        r.y = getPXSizeForWindow(window->factor, BUTTON_HOME_SCREEN_Y + (i * BUTTON_HOME_SCREEN_H) + BUTTON_SPACE);
        initButton(button, NULL, str, r, buttonBg, buttonFg);
        drawButton(button, window);
        free(str);
        i++;
        res++;
    }

    free(saveRes);
    free(button);
}