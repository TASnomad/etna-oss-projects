#include <window.h>

void display_choose_server_screen(t_window *window)
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
    SDL_Color   buttonBg;
    SDL_Color   buttonFg;
    t_button    *button;

    r.x = getPXSizeForWindow(window->factor, BUTTON_HOME_SCREEN_X);
    r.y = getPXSizeForWindow(window->factor, BUTTON_HOME_SCREEN_Y);
    r.w = getPXSizeForWindow(window->factor, BUTTON_HOME_SCREEN_W);
    r.h = getPXSizeForWindow(window->factor, BUTTON_HOME_SCREEN_H);

    buttonBg = getHomeScreenButtonBgColor();
    buttonFg = getHomeScreenButtonFgColor();


    //first button which contain text input
    button = createButton();
    initButton(button, NULL, window->ipInput, r, buttonBg, buttonFg);
    drawButton(button, window);

    //second button which contain submit input
    r.y += r.h + BUTTON_SPACE;
    if (strlen(window->ipInput) < 7)
    {
        buttonBg = getChooseScreenButtonBgColor();
    }
    initButton(button, NULL, "Submit", r, buttonBg, buttonFg);
    drawButton(button, window);
    free(button);
    SDL_RenderPresent(window->renderer);
}

SDL_Color getChooseScreenButtonBgColor()
{
    SDL_Color c;
    c.r = 149;
    c.g = 165;
    c.b = 166;

    return c;
}

