#include "text.h"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Score{
	TTF_Font* font;
	int value;
	SDL_Color color;
	SDL_Rect* rect;
};

void RenderScore(Score* score, SDL_Renderer* renderer){
	
	if(!score){
		fprintf(stderr, "Error drawing text: text is NULL.");
		return;
	}

	char text[3];

	sprintf(text, "%02d", score->value);

	SDL_Surface* surface = TTF_RenderText_Solid(score->font, text, score->color);


	if(!surface){
		fprintf(stderr, "Erro creating text: %s\n", TTF_GetError());
		SDL_FreeSurface(surface);
		surface = NULL;
		return;
	}

	SDL_Rect *rect = GetScoreRect(score);
	
	if(rect->w != surface->w)
		rect->w = surface->w;

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	SDL_RenderCopy(renderer, texture, NULL, rect);
	surface = NULL;

	SDL_DestroyTexture(texture);
	texture = NULL;
}

SDL_Rect* GetScoreRect(Score* score){
	if(!score->rect){
		score->rect = (SDL_Rect*)malloc(sizeof(SDL_Rect));
		if (!score) {
			fprintf(stderr, "Error creating rect.");
			return NULL;
		}

		score->rect->x = 0;
		score->rect->y = 0;

		char text[3];
		sprintf(text, "%02d", score->value);
	
		TTF_SizeUTF8(score->font, text, &score->rect->w, &score->rect->h);
	}

	return score->rect;
}

void DestroyScore(Score* score){
	TTF_CloseFont(score->font);
	free(score->rect);

	score->font = NULL;
	score->rect = NULL;

	free(score);
	score = NULL;
}

void SetScoreValue(Score *text, int value){
	text->value = value;
}

const int GetScoreValue(Score *text){
	return text->value;
}

Score* NewScore(char* font_path, int size, SDL_Color color){
	Score* text = (Score*)malloc(sizeof(Score));
	text->font = TTF_OpenFont(font_path, size);
	text->color = color;
	text->rect = NULL;

	return text;
}
