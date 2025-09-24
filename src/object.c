#include "object.h"
#include <SDL2/SDL_render.h>
#include <stdbool.h>

struct Obj{
	SDL_Rect rect;
	SDL_Color color;
};

void obj_render(const Obj *obj, SDL_Renderer *renderer){
	SDL_SetRenderDrawColor(renderer, obj->color.r, obj->color.g, obj->color.b, 255);
	SDL_RenderFillRect(renderer, &obj->rect);
}


Obj* obj_create(SDL_Rect rect, SDL_Color color){
	Obj* obj = (Obj*)malloc(sizeof(Obj));

	if(!obj){
		return NULL;
	}

	obj->rect = rect;

	obj->color = color;


	return obj;
}

SDL_Rect* obj_rect(Obj *obj){
	return &obj->rect;
}

void obj_destroy(Obj* obj){
	free(obj);
}
