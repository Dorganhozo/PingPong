#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

typedef struct Obj Obj;

int is_collied(const Obj* a, const Obj* b);

void obj_render(const Obj* obj, SDL_Renderer* renderer);
Obj* obj_create(SDL_Rect rect, SDL_Color color);
void obj_destroy(Obj* obj);
SDL_Rect* obj_rect(Obj* obj);
