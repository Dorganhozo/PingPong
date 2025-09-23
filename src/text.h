#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_ttf.h>

typedef struct Score Score;

void RenderScore(Score* score, SDL_Renderer* renderer);
void SetScoreValue(Score* score, int value);
const int GetScoreValue(Score* score);
SDL_Rect* GetScoreRect(Score* score);
void DestroyScore(Score* score);
Score* NewScore(char* font_path, int size, SDL_Color color);
