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

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	// use GLESv2
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	// anti-
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	// activate V-Sync
	SDL_GL_SetSwapInterval(1);
}

SDL2::~SDL2(void) {
	// clean up
	SDL_GL_DeleteContext(this->Context);
	SDL_DestroyWindow(this->Window);
	SDL_Quit();
}

void SDL2::update(std::function<void(void)> draw)
{
    while (1)
	{
		draw();
		// glDrawElements(GL_TRIANGLES, 4, GL_UNSIGNED_INT, nullptr);
		SDL_GL_SwapWindow(this->Window);
	}
}

SDL_Window * SDL2::createWindow()
{
	this->Window = SDL_CreateWindow("Hello GLES!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
										  this->DisplayMode.w, this->DisplayMode.h, this->Flags);
	if (this->Window == NULL)
	{
		printf("Error while creating window: %s\n", SDL_GetError());
		SDL_Quit();
		exit(1);
	}

	return this ->Window;
}


SDL_GLContext SDL2::createGLContext()
{
	// create gl context
	printf("Creating OpenGL context...\n");
	this->Context = SDL_GL_CreateContext(this->Window);
	// if (context == NULL)
	// {
	// 	printf("Error while creating OpenGL Context: %s\n", SDL_GetError());
	// 	SDL_DestroyWindow(window);
	// 	SDL_Quit();
	// 	return 1;
	// }

	// hide mouse cursor
	SDL_ShowCursor(0);

	SDL_DisplayMode mode;
	SDL_GetDesktopDisplayMode(0, &mode);

	SDL_Log("Screen bpp: %d", SDL_BITSPERPIXEL(mode.format));
	SDL_Log("");
	SDL_Log("Vendor     : %s", glGetString(GL_VENDOR));
	SDL_Log("Renderer   : %s", glGetString(GL_RENDERER));
	SDL_Log("Version    : %s", glGetString(GL_VERSION));
	SDL_Log("Extensions : %s", glGetString(GL_EXTENSIONS));
	SDL_Log("");

	// set gl parameters
	glClearColor(1.f, 1.f, 1.f, 1.f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	return this->Context;
}