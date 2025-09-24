#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include "object.h"
#include "text.h"

typedef struct {
	Obj* player;
	Obj* enemy;
	Obj* ball;
	Score* player_score;
	Score* enemy_score;
} World;


typedef struct{
	World world;
	SDL_Renderer* renderer;
	SDL_Window* window;
	bool is_running;
	float x_speed_ball, y_speed_ball;
	float x_dir_ball, y_dir_ball;
} Game;

int game_initialize(Game* game);

void game_input(Game* game, const Uint8* keys);

void game_render(const Game* game);

void game_update(Game* game, int delta_t);

void game_dispose(Game* game);
