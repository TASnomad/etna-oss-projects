#include <window.h>
#include <utils.h>

void display_loading_party_screen(t_window *window)
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

    bgScreen = getServerWaitingScreenbackgroundColor();
    SDL_SetRenderDrawColor(window->renderer, bgScreen.r, bgScreen.g, bgScreen.b, bgScreen.a);
    SDL_RenderFillRect(window->renderer, &r);
    SDL_Color   buttonBg;
    SDL_Color   buttonFg;
    t_button    *button;

    r.x = getPXSizeForWindow(window->factor, BUTTON_HOME_SCREEN_X);
    r.y = getPXSizeForWindow(window->factor, BUTTON_HOME_SCREEN_Y);
    r.w = getPXSizeForWindow(window->factor, BUTTON_HOME_SCREEN_W);
    r.h = getPXSizeForWindow(window->factor, BUTTON_HOME_SCREEN_H);

    buttonBg = getServerWaitingScreenbackgroundColor();
    buttonFg = getHomeScreenButtonFgColor();


    //first button which contain text input
    button = createButton();
    initButton(button, NULL, "En attente des autres joueurs", r, buttonBg, buttonFg);
    drawButton(button, window);

    r.y += r.h + BUTTON_SPACE;

    initButton(button, NULL, window->ipInput, r, buttonBg, buttonFg);
    drawButton(button, window);
    free(button);
    SDL_RenderPresent(window->renderer);
}