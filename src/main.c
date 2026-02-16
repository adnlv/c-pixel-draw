#include <SDL3/SDL.h>

int main(void)
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
    {
        SDL_Log("failed to initialize SDL: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_Quit();
    return 0;
}
