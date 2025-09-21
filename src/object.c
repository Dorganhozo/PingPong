#include "object.h"
#include <SDL2/SDL_render.h>

struct Obj{
	SDL_Rect rect;
	SDL_Color color;
};

void obj_render(const Obj *obj, SDL_Renderer *renderer){
	SDL_SetRenderDrawColor(renderer, obj->color.r, obj->color.g, obj->color.b, 255);
	SDL_RenderFillRect(renderer, &obj->rect);
}

int is_collied(const Obj *a, const Obj *b){
	return a->rect.x <= b->rect.x + b->rect.w && a->rect.x + a->rect.w >= b->rect.x &&
	       a->rect.y <= b->rect.y + b->rect.h && a->rect.y + a->rect.h >= b->rect.y;
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
