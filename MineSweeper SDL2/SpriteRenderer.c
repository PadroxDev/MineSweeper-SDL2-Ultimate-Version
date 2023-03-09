#include <stdio.h>
#include <SDL.h>
#include "App.h"
#include "SpriteRenderer.h"

int getSpriteIndex(SpriteRenderer* renderer) {
	return (int)((renderer->lifetime / renderer->animationSpeed)) % renderer->spriteCount;
}

void initSprite(SpriteRenderer* renderer, SDL_Texture* spriteSheet, int spriteCount, float animationSpeed, SDL_Rect* transform, int flip) {
	renderer->spriteSheet = spriteSheet;
	renderer->spriteCount = spriteCount;
	renderer->animationSpeed = animationSpeed;
	renderer->lifetime = 0;
	renderer->transform = transform;
	renderer->flip = flip;
}

void updateSprite(SpriteRenderer* renderer, App* app) {
	renderer->lifetime += app->clock.deltaTime;

	int spriteIndex = getSpriteIndex(renderer);

	SDL_Rect clip;
	clip.x = 0;
	clip.y = renderer->transform->h * spriteIndex;
	clip.w = renderer->transform->w;
	clip.h = renderer->transform->h;

	SDL_RenderCopyEx(app->renderer, renderer->spriteSheet, &clip, renderer->transform, 0, NULL,
		renderer->flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}