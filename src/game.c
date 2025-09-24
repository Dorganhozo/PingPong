#include "game.h"
#include "main.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

bool is_out(int x, int y);

void set_positions(Game* game);

bool predict_path(const Game* game, int tolerance, int* y_position){
	SDL_Rect ball_rect = *obj_rect(game->world.ball);
	SDL_Rect* enemy_rect = obj_rect(game->world.enemy);


	int x_vel_ball = game->x_dir_ball * game->x_speed_ball;
	int y_vel_ball = game->y_dir_ball * game->y_speed_ball;

	float tolerance_percent = (float)tolerance/100;
	
	while(!is_out(ball_rect.x/tolerance_percent, ball_rect.y)){
		ball_rect.x += x_vel_ball;
		ball_rect.y += y_vel_ball;

		if(SDL_HasIntersection(&ball_rect, enemy_rect))
			return false;
	}
 	
	*y_position = ball_rect.y - y_vel_ball;

	return ball_rect.x > SCREEN_WIDTH * tolerance_percent;
}



void game_update(Game* game, int delta_t){
	Obj *ball = game->world.ball;
	SDL_Rect* ball_rect = obj_rect(ball);

	Obj *player = game->world.player;
	SDL_Rect* player_rect = obj_rect(player);

	Obj *enemy = game->world.enemy;
	SDL_Rect* enemy_rect = obj_rect(enemy);

	int player_score = GetScoreValue(game->world.player_score);
	int enemy_score = GetScoreValue(game->world.enemy_score);

	int y_position;

	int humility =  (enemy_score - player_score);

	int tolerance = 100 + humility;

	if(predict_path(game, tolerance, &y_position)){
		int y_vel = (y_position - enemy_rect->h/2 - enemy_rect->y >= 0 ? 1 : -1) * SPEED;

		if(!is_out(0, y_vel + enemy_rect->y) && !is_out(0, y_vel + enemy_rect->y + enemy_rect->h)){
			enemy_rect->y += y_vel;
		}
	}

	SDL_Rect* actor_rect = NULL;
	
	if(SDL_HasIntersection(ball_rect, enemy_rect) && (actor_rect = enemy_rect) ||
	   SDL_HasIntersection(ball_rect, player_rect) && (actor_rect = player_rect)){
		game->x_dir_ball = -game->x_dir_ball;
		game->x_speed_ball += 0.2f;

		int y_dis = (ball_rect->y + ball_rect->h/2) - (actor_rect->y + actor_rect->h/2);
		int y_dir = y_dis >=0 ? 1 : -1;

		game->y_dir_ball = -y_dir;
		game->y_speed_ball = game->x_speed_ball * abs(y_dis)/((float)actor_rect->h/2);
	}


	if(ball_rect->y < 0 || ball_rect->y + ball_rect->h > SCREEN_HEIGHT)
		game->y_dir_ball = -game->y_dir_ball;


	if(ball_rect->x < 0){
		int score = GetScoreValue(game->world.enemy_score);
		score ++;

		game->x_dir_ball = -game->x_dir_ball;


		SetScoreValue(game->world.enemy_score, score);
		set_positions(game);
	}


	if(ball_rect->x + ball_rect->w > SCREEN_WIDTH){
		int score = GetScoreValue(game->world.player_score);
		score ++;

		game->x_dir_ball = -game->x_dir_ball;


		SetScoreValue(game->world.player_score, score);
		set_positions(game);
	}


	SDL_Rect dest_rect = *ball_rect;
	float x_vel = game->x_dir_ball * game->x_speed_ball;
	float y_vel = game->y_dir_ball * game->y_speed_ball;

	dest_rect.x += x_vel;
	dest_rect.y += y_vel;

	if(!SDL_HasIntersection(actor_rect, &dest_rect)){
		ball_rect->x += x_vel;
		ball_rect->y += y_vel;
	}

}

void game_input(Game* game, const Uint8* keys){
	SDL_Rect* player_rect = obj_rect(game->world.player);	
	int diry = (keys[SDL_SCANCODE_DOWN] - keys[SDL_SCANCODE_UP]) * SPEED;

	if(player_rect->y + diry >= 0 && player_rect->y + player_rect->h + diry <= SCREEN_HEIGHT)
		player_rect->y += diry;
}

void game_render(const Game *game){
	SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
	SDL_RenderClear(game->renderer);

	//Draw
	obj_render(game->world.player, game->renderer);
	obj_render(game->world.enemy, game->renderer);
	obj_render(game->world.ball, game->renderer);

	RenderScore(game->world.player_score, game->renderer);
	RenderScore(game->world.enemy_score, game->renderer);

	SDL_RenderPresent(game->renderer);
}



int game_initialize(Game* game){
	srand(time(0));
	*game = (Game){ NULL, NULL, NULL, .is_running = 1 };

	if(SDL_Init(SDL_INIT_VIDEO)){
		fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	if(TTF_Init()){
		fprintf(stderr, "Erro initializing: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}


	game->window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);


	if (!game->window) {
		fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	game->renderer = SDL_CreateRenderer(game->window, -1, 0);

	if (!game->renderer) {
		fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}


	SDL_Color color = (SDL_Color){255, 255, 255};

	game->world.player_score = NewScore(FONT, 24, color);
	SetScoreValue(game->world.player_score, 0);
	SDL_Rect *player_counter_rect = GetScoreRect(game->world.player_score);
	player_counter_rect->x = SCREEN_WIDTH/4 - player_counter_rect->w/2;

	if(!game->world.player_score){
		fprintf(stderr, "Error creating text:", TTF_GetError());
		return EXIT_FAILURE;
	}


	game->world.enemy_score = NewScore(FONT, 24, color);
	SetScoreValue(game->world.enemy_score, 0);
	SDL_Rect *enemy_counter_rect = GetScoreRect(game->world.enemy_score);
	enemy_counter_rect->x =  SCREEN_WIDTH * 3/4 - enemy_counter_rect->w/2;

	if(!game->world.enemy_score){
		fprintf(stderr, "Error creating text:", TTF_GetError());
		return EXIT_FAILURE;
	}

	SDL_Rect ball_rect = (SDL_Rect){0, 0, 10, 10};
	game->world.ball = obj_create(ball_rect, color);

	if(!game->world.ball){
		fprintf(stderr, "Error creating ball.");
		return EXIT_FAILURE;
	}


	SDL_Rect player_rect = (SDL_Rect){0, 0, 10, 50};
	game->world.player = obj_create(player_rect, color);

	if(!game->world.player){
		fprintf(stderr, "Error creating player.");
		return EXIT_FAILURE;
	}

	SDL_Rect enemy_rect = (SDL_Rect){0, 0, 10, 50};
	game->world.enemy =  obj_create(enemy_rect, color);


	if(!game->world.enemy){
		fprintf(stderr, "Error creating enemy.");
		return EXIT_FAILURE;
	}


	game->x_dir_ball = 1;
	game->y_dir_ball = 1;

	set_positions(game);


	return EXIT_SUCCESS;

}

void game_dispose(Game* game){

	SDL_DestroyRenderer(game->renderer);
	SDL_DestroyWindow(game->window);

	DestroyScore(game->world.player_score);
	game->world.player_score = NULL;

	DestroyScore(game->world.enemy_score);
	game->world.enemy_score =  NULL;

	obj_destroy(game->world.player);
	game->world.player = NULL;

	obj_destroy(game->world.enemy);
	game->world.enemy = NULL;

	obj_destroy(game->world.ball);
	game->world.ball = NULL;


	game->window = NULL;
	game->renderer = NULL;

	TTF_Quit();
	SDL_Quit();
}

void set_positions(Game* game){
	SDL_Rect* ball_rect = obj_rect(game->world.ball);
	SDL_Rect* player_rect = obj_rect(game->world.player);
	SDL_Rect* enemy_rect = obj_rect(game->world.enemy);

	ball_rect->x = SCREEN_WIDTH/2 - ball_rect->w/2;
	ball_rect->y = SCREEN_HEIGHT/2 - ball_rect->h/2;

	game->x_speed_ball = 3;

	player_rect->y = SCREEN_HEIGHT/2 - player_rect->h/2;

	enemy_rect->x = SCREEN_WIDTH - enemy_rect->w;
	enemy_rect->y = SCREEN_HEIGHT/2 - enemy_rect->h/2;

	game->x_speed_ball = 3;
	game->y_speed_ball = 1;
}

bool is_out(int x, int y){
	if(x < 0 || x > SCREEN_WIDTH)
		return true;

	if(y < 0 || y > SCREEN_HEIGHT)
		return true;

	return false;
}
