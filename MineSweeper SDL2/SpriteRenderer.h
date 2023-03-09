#pragma once
#include <SDL.h>

typedef struct SDL_Texture SDL_Texture;
typedef struct SDL_Rect SDL_Rect;
typedef struct App App;

typedef struct SpriteRenderer {
	SDL_Texture* spriteSheet;
	int spriteCount;
	float animationSpeed;
	float lifetime;
	SDL_Rect* transform;
	int flip;
} SpriteRenderer;

void initSprite(SpriteRenderer* renderer, SDL_Texture* spriteSheet, int spriteCount, float animationSpeed, SDL_Rect* transform, int flip);

void updateSprite(SpriteRenderer* renderer, App* app);