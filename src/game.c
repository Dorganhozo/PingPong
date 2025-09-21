#include "game.h"
#include "main.h"
#include "object.h"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>
#include <stdio.h>
#include <stdlib.h>

void set_positions(Game* game){
	SDL_Rect* ball_rect = obj_rect(game->world.ball);
	SDL_Rect* player_rect = obj_rect(game->world.player);
	SDL_Rect* enemy_rect = obj_rect(game->world.enemy);

	ball_rect->x = SCREEN_WIDTH/2 - ball_rect->w/2;
	ball_rect->y = SCREEN_HEIGHT/2 - ball_rect->h/2;

	player_rect->y = SCREEN_HEIGHT/2 - player_rect->h/2;

	enemy_rect->x = SCREEN_WIDTH - enemy_rect->w;
	enemy_rect->y = SCREEN_HEIGHT/2 - enemy_rect->h/2;

}

void game_update(Game* game, int delta_t){
	Obj *ball = game->world.ball;
	SDL_Rect* ball_rect = obj_rect(ball);


	Obj *enemy = game->world.enemy;
	SDL_Rect* enemy_rect = obj_rect(enemy);

	if (ball_rect->x > SCREEN_WIDTH/2) {
		game->y_accel = (enemy_rect->y - ball_rect->y ? 1 : -1) * 10;
	}


	if(enemy_rect->y + game->y_accel >= 0 && enemy_rect->y + enemy_rect->h + game->y_accel <= SCREEN_HEIGHT)
		enemy_rect->y += game->y_accel;

	if(is_collied(ball, game->world.player) || is_collied(ball, game->world.enemy)){
		game->x_vel_ball = -game->x_vel_ball;
	}
		
	if(ball_rect->y < 0 || ball_rect->y + ball_rect->h > SCREEN_HEIGHT)
		game->y_vel_ball = -game->y_vel_ball;


	if(ball_rect->x < 0){
		int score = atoi(GetTextValue(game->world.enemy_score));
		score ++;
		
		char new_score[3];
		sprintf(new_score, "%02d", score);

		SetTextValue(game->world.enemy_score, new_score);
		set_positions(game);
	}


	if(ball_rect->x > SCREEN_WIDTH){
		int score = atoi(GetTextValue(game->world.player_score));
		score ++;
		
		char new_score[3];
		sprintf(new_score, "%02d", score);

		SetTextValue(game->world.player_score, new_score);
		set_positions(game);
	}
	

	ball_rect->x += game->x_vel_ball;
	ball_rect->y += game->y_vel_ball;

}

void game_input(Game* game, const Uint8* keys){
	SDL_Rect* player_rect = obj_rect(game->world.player);	
	int diry = (keys[SDL_SCANCODE_DOWN] - keys[SDL_SCANCODE_UP]) * 5;

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

	RenderText(game->world.player_score, game->renderer);
	RenderText(game->world.enemy_score, game->renderer);

	SDL_RenderPresent(game->renderer);
}



int game_initialize(Game* game){
	*game = (Game){ NULL, NULL, NULL, .is_running = 1 };

	if(SDL_Init(SDL_INIT_EVERYTHING)){
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

	game->world.player_score = NewText(FONT, 24, color);
	SetTextValue(game->world.player_score, "00");
	SDL_Rect *player_counter_rect = GetTextRect(game->world.player_score);
	player_counter_rect->x = SCREEN_WIDTH/4 - player_counter_rect->w/2;
	
	if(!game->world.player_score){
		fprintf(stderr, "Error creating text:", TTF_GetError());
		return EXIT_FAILURE;
	}


	game->world.enemy_score = NewText(FONT, 24, color);
	SetTextValue(game->world.enemy_score, "00");
	SDL_Rect *enemy_counter_rect = GetTextRect(game->world.enemy_score);
	enemy_counter_rect->x = SCREEN_WIDTH/4 + SCREEN_WIDTH/2 - enemy_counter_rect->w/2;

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

	game->x_vel_ball = -1;
	game->y_vel_ball = 1;

	set_positions(game);


	return EXIT_SUCCESS;

}

void game_dispose(Game* game){
	SDL_DestroyRenderer(game->renderer);
	SDL_DestroyWindow(game->window);

	DestroyText(game->world.player_score);
	game->world.player_score = NULL;

	DestroyText(game->world.enemy_score);
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
