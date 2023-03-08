#include <SDL.h> // Required for lastTimeTick
typedef struct SDL_Window SDLWindow;
typedef struct SDL_Surface SDL_Surface;
typedef struct SDL_Renderer SDL_Renderer;
typedef struct Board Board;

typedef struct Clock {
	uint32_t lastTimeTick;
	double deltaTime;
} Clock;

typedef struct App {
	SDL_Window* window;
	SDL_Surface* surface;
	SDL_Renderer* renderer;
	Clock clock;
	int running;
	float fpsCount;
} App;

void tick(Clock* clock);

int initApp(App* app);

void displayFPS(App* app);

void displayBoard(App* app, Board* board, SDL_Texture** resources);

void quitApp(App* app, SDL_Texture** resources);