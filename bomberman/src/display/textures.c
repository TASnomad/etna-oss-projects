#include <window.h>
//load all the textures
void load_textures(t_window *window)
{
    load_grass_textures(window);
    load_water_textures(window);
	load_sand_textures(window);
	load_bush_textures(window);
    verify_textures(window);
}

/*
    Loading all grass images as textures
*/
void load_grass_textures(t_window *window)
{
    SDL_Surface *grassDark = IMG_Load("res/Sprites/grass_tile_1.png");
    SDL_Surface *grassLight = IMG_Load("res/Sprites/grass_tile_2.png");
    SDL_Surface *grassUltraLight = IMG_Load("res/Sprites/grass_tile_3.png");

    if (grassDark) {
        SDL_DestroyTexture(window->grassDarkTexture);
        window->grassDarkTexture = SDL_CreateTextureFromSurface(window->renderer, grassDark);
        SDL_FreeSurface(grassDark);
    }

    if (grassLight) {
        SDL_DestroyTexture(window->grassLightTexture);
        window->grassLightTexture = SDL_CreateTextureFromSurface(window->renderer, grassLight);
        SDL_FreeSurface(grassLight);
    }

    if (grassUltraLight) {
        SDL_DestroyTexture(window->grassUltraLightTexture);
        window->grassUltraLightTexture = SDL_CreateTextureFromSurface(window->renderer, grassUltraLight);
        SDL_FreeSurface(grassUltraLight);
    }
}

/*
    Loading all water textures
*/
void load_water_textures(t_window *window)
{
    SDL_Surface *water = IMG_Load("res/Sprites/WaterTiles/0.png");

     if (water) {
        SDL_DestroyTexture(window->waterTexture);
        window->waterTexture = SDL_CreateTextureFromSurface(window->renderer, water);
        SDL_FreeSurface(water);
    }
}

void load_sand_textures(t_window *window)
{
	SDL_Surface *sand = IMG_Load("res/Sprites/sand_tile.png");
	if (sand) {
		SDL_DestroyTexture(window->sandTexture);
		window->sandTexture = SDL_CreateTextureFromSurface(window->renderer, sand);
		SDL_FreeSurface(sand);
	}
}

void load_bush_textures(t_window *window)
{
	SDL_Surface *bush = IMG_Load("res/Sprites/bush_tile.png");
	if (bush) {
		SDL_DestroyTexture(window->bushTexture);
		window->bushTexture = SDL_CreateTextureFromSurface(window->renderer, bush);
		SDL_FreeSurface(bush);
	}
}


//verify if all the textures are loaded
void verify_textures(t_window *window)
{
    int error;

    error = 1;
    if (window->grassDarkTexture == NULL || window->grassLightTexture == NULL || window->grassUltraLightTexture == NULL || window->sandTexture == NULL || window->bushTexture == NULL)
    {
        error = 0;
    }
    if (window->waterTexture == NULL)
    {
        error = 0;
    }

    window->textureLoading = error;
}