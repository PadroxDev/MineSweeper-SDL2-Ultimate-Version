#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include "Resources.h"
#include "App.h"

void initResources(App* app, SDL_Texture* resources[]) {
	printf("\nLoading resources...");

	resources[0] = IMG_LoadTexture(app->renderer, "res/InGameBackground.jpg");
	resources[1] = IMG_LoadTexture(app->renderer, "res/GroundTile.jpg");
	resources[2] = IMG_LoadTexture(app->renderer, "res/SheriffStars_SpriteSheet.jpg");
	resources[3] = IMG_LoadTexture(app->renderer, "res/Dynamite_SpriteSheet.jpg");
	resources[4] = IMG_LoadTexture(app->renderer, "res/BullSkull.png");
}