#include <stdio.h>
#include <SDL.h>
#include "App.h"
#include "SpriteRenderer.h"
#include "Settings.h"

int getSpriteIndex(SpriteRenderer* renderer) {
	return (int)((renderer->lifetime / renderer->animationSpeed)) % renderer->spriteCount;
}

void initSprite(SpriteRenderer* renderer, SDL_Texture* spriteSheet, int spriteCount, float animationSpeed, SDL_Rect* transform, int flip, int tileWidth, int tileHeight) {
	renderer->spriteSheet = spriteSheet;
	renderer->spriteCount = spriteCount;
	renderer->animationSpeed = animationSpeed;
	renderer->lifetime = 0;
	renderer->transform = transform;
	renderer->flip = flip;
	renderer->tileWidth = tileWidth;
	renderer->tileHeight = tileHeight;
}

void updateSprite(SpriteRenderer* renderer, App* app) {
	renderer->lifetime += app->clock.deltaTime;

	int spriteIndex = getSpriteIndex(renderer);

	SDL_Rect clip;
	clip.x = 0;
	clip.y = renderer->tileHeight * spriteIndex;
	clip.w = renderer->tileWidth;
	clip.h = renderer->tileHeight;

	SDL_RenderCopyEx(app->renderer, renderer->spriteSheet, &clip, renderer->transform, 0, NULL,
		renderer->flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}