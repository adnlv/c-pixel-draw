#include <SDL3/SDL.h>

int main(void)
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
    {
        SDL_Log("failed to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window;
    SDL_Renderer* renderer;

    if (!SDL_CreateWindowAndRenderer("Pixel Draw",
                                     640,
                                     480,
                                     SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE,
                                     &window, &renderer))
    {
        SDL_Log("failed to create window and renderer: %s\n", SDL_GetError());
        return 1;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
