#pragma once

typedef struct App App;
typedef struct SDL_Texture SDL_Texture;

void initResources(App* app, SDL_Texture* resources[]);

void initSoundResources(App* app, Mix_Chunk* soundResources[]);