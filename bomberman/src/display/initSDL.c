#include <window.h>

int init_SDL()
{
    int flags=IMG_INIT_JPG|IMG_INIT_PNG;
    int initted = IMG_Init(flags);
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0)
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        return 0;
    }

    if((initted&flags) != flags) {
        printf("IMG_Init: Failed to init required jpg and png support!\n");
        printf("IMG_Init: %s\n", IMG_GetError());
        return 0;
    }

    if (TTF_Init() == -1)
    {
        printf("TTF_Init: %s\n", TTF_GetError());
		return 0;
    }
    return 1;
}