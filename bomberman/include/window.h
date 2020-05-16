#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <map.h>
#include <proto.h>
#define WINDOW_INIT_LOADING_SCREEN      1
#define WINDOW_HOME_SCREEN              2
#define WINDOW_SERVER_WAITING_SCREEN    3
#define	WINDOW_GAME_SCREEN				4
#define WINDOW_CHOOSE_SERVER_SCREEN     5
#define WINDOW_PLAYER_WAITING_SCREEN    6
#define WINDOW_HEIGHT               600
#define WINDOW_WIDTH                800
#define CELL_SIZE                   WINDOW_HEIGHT / MAP_SIZE
#define CELL_SIZE_W                 WINDOW_WIDTH / MAP_SIZE
#define BUTTON_HOME_SCREEN_X        200
#define BUTTON_HOME_SCREEN_Y        180
#define BUTTON_HOME_SCREEN_W        400
#define BUTTON_HOME_SCREEN_H        100
#define BUTTON_SPACE                20
#define BUTTON_LETTER_W             32
#define SOLID_WALL 1
#define BRICK_WALL 2
#define EMPTY_CELL 3

typedef struct s_window {
    int             still_running;
    int             screen_type;
    int             cellXSize;
    int             cellYSize;
    int             textureLoading;
    int             toDisplay;
    float           factor;
    char            *ipInput;
	int				sock;
	int				id;
    int             refreshTime;
    int             keyBoardLock;
    pthread_t       *serv_thr;
    t_server        *serv;
    SDL_Window      *window;
    SDL_Texture     *grassDarkTexture;
    SDL_Texture     *grassLightTexture;
    SDL_Texture     *grassUltraLightTexture;
    SDL_Texture     *sandTexture;
	SDL_Texture		*bushTexture;
    SDL_Texture     *waterTexture;
    SDL_Renderer    *renderer;
    TTF_Font        *font;
} t_window;

typedef struct s_button {
    char *value;
    char *displayText;
    SDL_Rect rect;
    SDL_Color bgCol;
    SDL_Color fgCol;
} t_button;

t_button    *createButton();
t_window    *create_bomberman_window();
int         init_SDL();
int         getPXSizeForWindow(float factor, int size);

char        *deleteLastChar(char * string);

SDL_Color   getChooseScreenButtonBgColor();

SDL_Color   getHomeScreenbackgroundColor();
SDL_Color   getHomeScreenButtonBgColor();
SDL_Color   getHomeScreenButtonFgColor();

SDL_Color getServerWaitingScreenbackgroundColor();
SDL_Color getServerWaitingScreenButtonBgColor();
SDL_Color getServerWaitingScreenButtonFgColor();

void        initButton(t_button *button, char *value, char *displayText, SDL_Rect rect, SDL_Color bgCol, SDL_Color fgCol);
void        initButtonRect(t_button *button);
void        display_home_screen(t_window *window);
void        display_choose_server_screen(t_window *window);
void        display_loading_screen(t_window *window);
void        display_loading_party_screen(t_window *window);
void        display_server_waiting_screen(t_window *window);
void        display_server_waiting_buttons(t_window *window);
void 		display_game_screen(t_window *window);
void        displayHomeScreenButtons(t_window *window);
void        drawButton(t_button *button, t_window *window);
void        drawText(t_window *window, char *string, SDL_Rect r, SDL_Color color);
void        free_t_window(t_window *window);
void        free_t_button(t_button *button);
void        get_display_board_cell_size();
void        getWindowFactor(t_window *window);
void        load_textures(t_window *window);
void        load_grass_textures(t_window *window);
void		load_sand_textures(t_window *window);
void        load_water_textures(t_window *window);
void		load_bush_textures(t_window	*window);
void        verify_textures(t_window *window);
void        handle_display(t_window *window);
#endif /* !__WINDOW_H__ */