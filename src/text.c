#include "text.h"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Text{
	TTF_Font* font;
	char* value;
	SDL_Color color;
	SDL_Rect* rect;
};

void RenderText(Text* text, SDL_Renderer* renderer){
	
	if(!text){
		fprintf(stderr, "Error drawing text: text is NULL.");
		return;
	}

	SDL_Surface* surface = TTF_RenderText_Solid(text->font, text->value, text->color);


	if(!surface){
		fprintf(stderr, "Erro creating text: %s\n", TTF_GetError());
		SDL_FreeSurface(surface);
		surface = NULL;
		return;
	}

	SDL_Rect *rect = GetTextRect(text);
	
	if(rect->w != surface->w)
		rect->w = surface->w;

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	SDL_RenderCopy(renderer, texture, NULL, rect);
	surface = NULL;

	SDL_DestroyTexture(texture);
	texture = NULL;
}

SDL_Rect* GetTextRect(Text* text){
	if(!text->rect){
		text->rect = (SDL_Rect*)malloc(sizeof(SDL_Rect));
		if (!text) {
			fprintf(stderr, "Error creating rect.");
			return NULL;
		}

		text->rect->x = 0;
		text->rect->y = 0;
		TTF_SizeUTF8(text->font, text->value, &text->rect->w, &text->rect->h);
	}

	return text->rect;
}

void DestroyText(Text* text){
	TTF_CloseFont(text->font);
	free(text->value);
	free(text->rect);

	text->font = NULL;
	text->value = NULL;
	text->rect = NULL;

	free(text);
	text = NULL;
}

void SetTextValue(Text *text, const char *value){
	int length = strlen(value);
	text->value = (char*)malloc(length);
	strcpy(text->value, value);
}

const char* GetTextValue(Text *text){
	return text->value;
}

Text* NewText(char* font_path, int size, SDL_Color color){
	Text* text = (Text*)malloc(sizeof(Text));
	text->font = TTF_OpenFont(font_path, size);
	text->color = color;
	text->rect = NULL;

	return text;
}
