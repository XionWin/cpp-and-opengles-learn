#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>
#include "sdl2/sdl2.h"

#define FULLSCREEN 1
#define TRIANGLE_SIZE 0.5f
#define INFO_LOG_BUFFER_SIZE 1024

int main()
{
	SDL2 sdl2 = SDL2(true);

	SDL_Window *window = sdl2.createWindow();
	SDL_GLContext context = sdl2.createGLContext();

	sdl2.update(
		[] () {
			int quit = 0;
			SDL_Event e;
			while (SDL_PollEvent(&e))
			{
				if (e.type == SDL_QUIT || e.type == SDL_KEYDOWN)
					quit = 1;
				else if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED)
				{

				}
			}
			if (quit)
			{
				printf("Program was quit by user!\n");
				exit(0);
			}
			glClearColor(.2f, .2f, .5f, 1);
			glClear(GL_COLOR_BUFFER_BIT);
		}
	);
	

	// clean up
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
