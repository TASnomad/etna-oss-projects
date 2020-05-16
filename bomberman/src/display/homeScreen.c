#include <window.h>

void display_home_screen(t_window *window)
{
    SDL_RenderClear(window->renderer);
    SDL_Rect    r;
    SDL_Color   bgScreen;

    if (window->toDisplay == 0) return;

    window->toDisplay = 0;
    r.x = 0;
    r.y = 0;
    r.w = getPXSizeForWindow(window->factor, 800);
    r.h = getPXSizeForWindow(window->factor, 600);

    bgScreen = getHomeScreenbackgroundColor();
    SDL_SetRenderDrawColor(window->renderer, bgScreen.r, bgScreen.g, bgScreen.b, bgScreen.a);
    SDL_RenderFillRect(window->renderer, &r);
    displayHomeScreenButtons(window);
    SDL_RenderPresent(window->renderer);
}

SDL_Color getHomeScreenbackgroundColor()
{
    SDL_Color c;
    c.r = 25;
    c.g = 118;
    c.b = 210;

    return c;
}

SDL_Color getHomeScreenButtonBgColor()
{
    SDL_Color c;

    c.r = 13;
    c.g = 71;
    c.b = 161;

    return c;
}

SDL_Color getHomeScreenButtonFgColor()
{
    SDL_Color c;

    c.r = 227;
    c.g = 242;
    c.b = 253;

    return c;
}

void displayHomeScreenButtons(t_window *window)
{
    SDL_Rect    r;
    SDL_Color   buttonBg;
    SDL_Color   buttonFg;
    t_button    *button;
    char        *text1;
    char        *text2;

    r.x = getPXSizeForWindow(window->factor, BUTTON_HOME_SCREEN_X);
    r.y = getPXSizeForWindow(window->factor, BUTTON_HOME_SCREEN_Y);
    r.w = getPXSizeForWindow(window->factor, BUTTON_HOME_SCREEN_W);
    r.h = getPXSizeForWindow(window->factor, BUTTON_HOME_SCREEN_H);

    buttonBg = getHomeScreenButtonBgColor();
    buttonFg = getHomeScreenButtonFgColor();

    text1 = "Demarrer une partie";
    text2 = "Rejoindre une partie";

    button = createButton();
    initButton(button, NULL, text1, r, buttonBg, buttonFg);
    drawButton(button, window);

    r.y += r.h + BUTTON_SPACE;
    initButton(button, NULL, text2, r, buttonBg, buttonFg);
    drawButton(button, window);
    free(button);
}