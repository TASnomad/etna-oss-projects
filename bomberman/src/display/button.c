#include <window.h>

t_button    *createButton()
{
    t_button *button = (t_button *) malloc(sizeof(t_button));
    initButtonRect(button);
    button->value = NULL;
    button->displayText = NULL;

    return button;
}

void initButton(t_button *button, char *value, char *displayText, SDL_Rect rect, SDL_Color bgCol, SDL_Color fgCol)
{
    button->value = value;
    button->displayText = displayText;
    button->rect = rect;
    button->bgCol = bgCol;
    button->fgCol = fgCol;
}

void initButtonRect(t_button *button)
{
    button->rect.x = 0;
    button->rect.y = 0;
    button->rect.w = 0;
    button->rect.h = 0;
}

void drawButton(t_button *button, t_window *window)
{
    SDL_Rect txt = button->rect;
    txt.x = txt.x + 10;
    txt.y = txt.y + 10;
    txt.w = txt.w - 20;
    txt.h = txt.h - 20;
    SDL_SetRenderDrawColor(window->renderer, button->bgCol.r, button->bgCol.g, button->bgCol.b, button->bgCol.a);
    SDL_RenderFillRect(window->renderer, &button->rect);
    drawText(window, button->displayText, txt, button->fgCol); 
}