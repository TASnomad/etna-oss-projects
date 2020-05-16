#include	<window.h>
#include	<map.h>
#include	<utils.h>

void		display_game_screen(t_window *win)
{
	int		i;
	int		j;
	t_response_status st;

	st = get_map(win);
	SDL_Texture *cell = NULL;
	SDL_RenderClear(win->renderer);
	if (!win->textureLoading)
	{
		puts("SDL ressources are not loaded\n");
		load_textures(win);
		return ;
	}

	for (i = 0; i < MAP_SIZE; i++)
	{
		for (j = 0; j < MAP_SIZE; j++)
		{
			//printf("%d / %d: %d%d\n", i, j, GET_BIT(res[j * MAP_SIZE + i], 1), GET_BIT(res[j * MAP_SIZE + i], 2));
			// TODO: handle bomb in cell & fire
			if (st.map[j * MAP_SIZE + i] == SOLID_WALL)
				cell = win->waterTexture;
			else if(st.map[j * MAP_SIZE + i] == BRICK_WALL)
				cell = win->bushTexture;
			else
				cell = win->grassLightTexture;

			SDL_Rect d;

			d.x =  j * getPXSizeForWindow(win->factor, CELL_SIZE_W);
			d.y =  i * getPXSizeForWindow(win->factor, CELL_SIZE);
			d.w = getPXSizeForWindow(win->factor, CELL_SIZE_W);
			d.h = getPXSizeForWindow(win->factor, CELL_SIZE);
			SDL_RenderCopy(win->renderer, cell, NULL, &d);
			cell = NULL;
		}
	}
	SDL_RenderPresent(win->renderer);
	// pause();
	win->toDisplay = 0;
}