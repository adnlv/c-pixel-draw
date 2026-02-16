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

    SDL_Texture* canvtex = SDL_CreateTexture(renderer,
                                             SDL_PIXELFORMAT_RGBA8888,
                                             SDL_TEXTUREACCESS_STREAMING,
                                             canvsiz.x,
                                             canvsiz.y);
    if (canvtex == NULL)
    {
        SDL_Log("failed to create texture: %s\n", SDL_GetError());
        return 1;
    }

    {
        void* pixels;
        int pitch;
        if (!SDL_LockTexture(canvtex, NULL, &pixels, &pitch))
        {
            SDL_Log("failed to lock texture: %s\n", SDL_GetError());
            return 1;
        }

        uint32_t* dst = pixels;
        const int bpp = 4; // Bytes per pixel
        const int pixel_pitch = pitch / bpp; // Pitch in pixels
        const SDL_Palette* pal = SDL_GetTexturePalette(canvtex);

        const SDL_PixelFormatDetails* pfd = SDL_GetPixelFormatDetails(canvtex->format);
        if (pfd == NULL)
        {
            SDL_Log("failed to get pixel format details: %s\n", SDL_GetError());
            return 1;
        }

        for (int y = 0; y < canvsiz.y; y++)
        {
            for (int x = 0; x < canvsiz.x; x++)
            {
                dst[y * pixel_pitch + x] = SDL_MapRGBA(pfd, pal, 0, 0, 0, 0xFF);;
            }
        }

        SDL_UnlockTexture(canvtex);
    }

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
        SDL_SetRenderDrawColor(renderer, 0x77, 0x77, 0x77, 0xFF);
        SDL_RenderClear(renderer);

        SDL_RenderTexture(renderer, canvtex, NULL, &canvdst);
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderRect(renderer, &canvdst);

        SDL_RenderPresent(renderer);
    }

    /* === Cleaning Up === */
    SDL_DestroyTexture(canvtex);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
