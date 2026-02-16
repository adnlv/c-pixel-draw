#include <SDL3/SDL.h>

int main(void)
{
    /* === SDL Initialization === */
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
    {
        SDL_Log("failed to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    /* === Window & Renderer === */
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

    uint8_t* canvbuf = NULL;
    const SDL_Point canvsiz = {.x = 0xFF, .y = 0xFF};
    const uint16_t canvlen = canvsiz.x * canvsiz.y;
    const uint8_t canvpad = 4;
    SDL_FRect canvdst = {.x = (float)canvpad, .y = (float)canvpad};

    /* === Main Loop === */
    SDL_Event event;
    bool is_running = true;
    while (is_running)
    {
        int ow;
        int oh;
        SDL_GetCurrentRenderOutputSize(renderer, &ow, &oh);

        canvdst.w = (float)ow - canvdst.x - (float)canvpad;
        canvdst.h = (float)oh - canvdst.y - (float)canvpad;

        /* === Events === */
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                is_running = false;
                break;
            }
        }

        /* === Rendering === */
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderFillRect(renderer, &canvdst);

        SDL_RenderPresent(renderer);
    }

    /* === Cleaning Up === */
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
