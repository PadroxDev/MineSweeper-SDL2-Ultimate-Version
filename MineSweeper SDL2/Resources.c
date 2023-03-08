#include <SDL.h>
#include <SDL_image.h>
#include "Resources.h"
#include "App.h"

void initResources(App* app, SDL_Texture** resources) {
	printf("\nLoading resources...");
	resources[0] = IMG_LoadTexture(app->renderer, "res/SheriffStars_SpriteSheet.jpg");
}