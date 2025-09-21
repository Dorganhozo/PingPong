#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_ttf.h>

typedef struct Text Text;

void RenderText(Text* text, SDL_Renderer* renderer);
void SetTextValue(Text* text, const char* value);
const char* GetTextValue(Text* text);
SDL_Rect* GetTextRect(Text* text);
void DestroyText(Text* text);
Text* NewText(char* font_path, int size, SDL_Color color);
