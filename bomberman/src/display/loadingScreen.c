#include <window.h>

/**
 * Function which show the loading screen
 * 
 **/
void display_loading_screen(t_window *window)
{
    if (window->toDisplay == 0) return;
    load_textures(window);
    window->font = TTF_OpenFont("res/Fonts/GloriaHallelujah.ttf", 100);

    if (window->textureLoading == 1)
    {
        
        get_display_board_cell_size(window);
        SDL_Rect DestR;
        SDL_Rect TextR;
        SDL_Color col;
        int i;
        int j;

        DestR.x = 0;
        DestR.y = 0;
        DestR.w = window->cellXSize;
        DestR.h = window->cellYSize;

        TextR.x = 200;
        TextR.y = 400;
        TextR.w = 900;
        TextR.h = 300;

        col.r = 251;
        col.g = 182;
        col.b = 45;

        
        /* render background, whereas NULL for source and destination
        rectangles just means "use the default" */
        SDL_RenderClear(window->renderer);

        /* render the current animation step of our shape */
        for (i = 0; i < MAP_SIZE; i++)
        {
            DestR.y = i * window->cellYSize;
            for (j = 0; j < MAP_SIZE; j++)
            {
                DestR.x = j * window->cellXSize;
                if ((i + j) % 3 == 0) SDL_RenderCopy(window->renderer, window->grassDarkTexture, NULL, &DestR);
                else if ((i + j) % 3 == 1) SDL_RenderCopy(window->renderer, window->grassLightTexture, NULL, &DestR);
                else SDL_RenderCopy(window->renderer, window->grassUltraLightTexture, NULL, &DestR);
            }
        }

        drawText(window, "Chargement ...", TextR, col);
        SDL_RenderPresent(window->renderer);
        //SDL_UpdateWindowSurface(window->window);
        window->toDisplay = 1;
        window->screen_type = WINDOW_HOME_SCREEN;
    }
    else 
    {
        printf("error while loading image: %s\n", SDL_GetError());
        window->still_running = 0;
    }
}

/*
    function which set the image size depending on the window dimensions
*/
void get_display_board_cell_size(t_window *window)
{
    int x;
    int y;

    SDL_GetRendererOutputSize(window->renderer, &x, &y);

    window->cellXSize = x / MAP_SIZE;
    window->cellYSize = y / MAP_SIZE;
    //return cellImgSize;
}