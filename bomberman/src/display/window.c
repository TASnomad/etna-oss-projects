#include <window.h>
#include <response.h>

t_window    *create_bomberman_window()
{
    t_window *window = NULL;

    if (init_SDL() == 1)
    {
        window                          = (t_window *) malloc(sizeof(t_window));
		if (window) {
			window->serv_thr = (pthread_t *) malloc(sizeof(pthread_t));
		}
        window->screen_type             = WINDOW_INIT_LOADING_SCREEN;
        window->still_running           = 1;
        window->window                  = SDL_CreateWindow("Bomberman", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);
        window->grassDarkTexture        = NULL;
        window->grassLightTexture       = NULL;
        window->grassUltraLightTexture  = NULL;
        window->waterTexture            = NULL;
		window->sandTexture				= NULL;
		window->bushTexture				= NULL;
        window->textureLoading          = 0;
        window->cellXSize               = CELL_SIZE;
        window->cellYSize               = CELL_SIZE;
        window->renderer                = SDL_CreateRenderer(window->window, -1, SDL_RENDERER_ACCELERATED);
        window->font                    = NULL;
        window->toDisplay               = 0;
        window->refreshTime             = 0;
        window->serv                    = NULL;
        window->ipInput                 = malloc(sizeof(char));
        window->ipInput[0]              = '\0';
		window->sock					= -1;
		window->id						= -1;
        window->keyBoardLock            = 0;
    }

    getWindowFactor(window);
    return window;
}

void    free_t_window(t_window * window)
{
    if (window != NULL)
    {
        if (window->ipInput != NULL) free(window->ipInput);
        if (window->serv_thr != NULL) pthread_cancel(*window->serv_thr);
        if (window->serv != NULL) dtor_server(window->serv);
        if (window->font != NULL) TTF_CloseFont(window->font);
        if (window->grassDarkTexture != NULL) SDL_DestroyTexture(window->grassDarkTexture);
        if (window->grassLightTexture != NULL) SDL_DestroyTexture(window->grassLightTexture);
        if (window->grassUltraLightTexture != NULL) SDL_DestroyTexture(window->grassUltraLightTexture);
        if (window->waterTexture) SDL_DestroyTexture(window->waterTexture);
        if (window->sandTexture) SDL_DestroyTexture(window->sandTexture);
        if (window->bushTexture) SDL_DestroyTexture(window->bushTexture);
        if (window->renderer != NULL) SDL_DestroyRenderer(window->renderer);
        if (window->window) SDL_DestroyWindow(window->window);
        free(window);
    }
}

void getWindowFactor(t_window *window)
{
    int nw = -1;
	int nh = -1;
    int glw = -1;
	int glh = -1;
	SDL_GetWindowSize(window->window, &nw, &nh);
	SDL_GL_GetDrawableSize(window->window, &glw, &glh);

    if (nw < glw)
    {
        window->factor = ((float) glw / (float) nw);
    }
    else if (nw == glw)
    {
        window->factor = 1;
    }
    else
    {
        window->factor = ((float) nw / (float) glw);
    }
}

int getPXSizeForWindow(float factor, int size)
{
    return size * factor;
}
