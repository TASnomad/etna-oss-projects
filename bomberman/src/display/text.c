#include <window.h>

void drawText(t_window *window, char *string, SDL_Rect r, SDL_Color color)
{
    int letter_w;
    int width;
    int fullWidth;
    int len;

    letter_w = getPXSizeForWindow(window->factor, BUTTON_LETTER_W);
    len = strlen(string);
    width = getPXSizeForWindow(window->factor, BUTTON_HOME_SCREEN_W - BUTTON_SPACE * 2);
    fullWidth = getPXSizeForWindow(window->factor, BUTTON_HOME_SCREEN_W);
    SDL_Surface *surface = TTF_RenderText_Blended(window->font, string, color);
    while (((letter_w * len) > width) && letter_w > 10)
    {
        letter_w--;
    }

    r.w = len * letter_w;
    r.x = r.x + (fullWidth - r.w) / 2;

    SDL_RenderCopy( window->renderer,
                    SDL_CreateTextureFromSurface(window->renderer,
                    surface),
                    NULL,
                    &r);
    free(surface);
}

char *deleteLastChar(char * string)
{
    char *res;
    int i;
    int len;

    if (string == NULL) return (NULL);
    len = strlen(string);

    if (len == 0) return (string);
    i = 0;
    res = malloc(sizeof(char) * (len - 1));
    while (i < len - 1)
    {
        res[i] = string[i];
        i++;
    }
    res[i] = '\0';
    free(string);
    return (res);
}