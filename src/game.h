#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include "object.h"
#include "text.h"

typedef struct {
	Obj* player;
	Obj* enemy;
	Obj* ball;
	Text* player_score;
	Text* enemy_score;
} World;


typedef struct{
	World world;
	SDL_Renderer* renderer;
	SDL_Window* window;
	int is_running;
	int x_vel_ball, y_vel_ball;
	int x_accel,  y_accel;
} Game;

int game_initialize(Game* game);

void game_input(Game* game, const Uint8* keys);

void game_render(const Game* game);

void game_update(Game* game, int delta_t);

void game_dispose(Game* game);
