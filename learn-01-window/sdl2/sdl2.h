#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>

#include <functional>

class SDL2
{
    SDL_DisplayMode DisplayMode;
    unsigned int Flags;
    SDL_GLContext Context;
    SDL_Window *Window;

public:
    SDL2(bool fullScreen);
    ~SDL2();
    // void update(void (*draw)());
    // SDL_Window * getWindow() { return Window; }
    // SDL_GLContext getContext() { return Context; }
    SDL_DisplayMode getDisplayMode() { return DisplayMode; }

    SDL_Window * createWindow();
    void update(std::function<void(void)> draw);
};