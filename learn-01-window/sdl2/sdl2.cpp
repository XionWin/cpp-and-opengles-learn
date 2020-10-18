#include "sdl2.h"

SDL2::SDL2(bool fullScreen)
{
	// initialize SDL
	printf("Initializing SDL...\n");
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("ERROR: Failed to initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	// setting SDL flags
	this->Flags = SDL_WINDOW_OPENGL;
	this->DisplayMode.w = 800;
	this->DisplayMode.h = 600;
	if (fullScreen)
	{
		this->Flags |= SDL_WINDOW_FULLSCREEN;
		// getting current display-resolution
		if (SDL_GetDesktopDisplayMode(0, &this->DisplayMode) != 0)
		{
			printf("Could not retrieve current display resolution: %s\n", SDL_GetError());
		}
	}
	else
	{
		this->Flags |= SDL_WINDOW_RESIZABLE;
	}
	const char *fullscreen_string = fullScreen ? "(fullscreen)" : "";
	printf("Creating window %dx%d%s...\n", this->DisplayMode.w, this->DisplayMode.h, fullscreen_string);
}

SDL2::~SDL2(void) {
	// clean up
	SDL_DestroyWindow(this->Window);
	SDL_Quit();
}

void SDL2::update(std::function<void(void)> draw)
{
    while (1)
	{
		draw();
	}
}

SDL_Window * SDL2::createWindow()
{
	this->Window = SDL_CreateWindow("Hello SDL2!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
										  this->DisplayMode.w, this->DisplayMode.h, this->Flags);
	if (this->Window == NULL)
	{
		printf("Error while creating window: %s\n", SDL_GetError());
		SDL_Quit();
		exit(1);
	}

	return this ->Window;
}
