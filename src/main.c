#include "main.h"
#include "game.h"
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>

int main(){
	Game game;
	if (game_initialize(&game)) {
		game_dispose(&game);
		exit(EXIT_FAILURE);
	}

	const Uint8* keys = SDL_GetKeyboardState(NULL);

	while (game.is_running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_KEYDOWN:
					if(event.key.keysym.scancode != SDL_SCANCODE_Q && event.key.keysym.scancode != SDL_SCANCODE_ESCAPE)break;
				case SDL_QUIT:
					game.is_running = 0;

				default:
					break;
			}
		}
		game_input(&game, keys);
		game_update(&game, 0);
		game_render(&game);
		SDL_Delay(DELAY);

	}

	game_dispose(&game);
	return EXIT_SUCCESS;
}



