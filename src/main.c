#include <SDL3/SDL.h>

int main(void)
{
    /* === Initialization === */
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

    const SDL_Point canvsiz = {.x = 0xFF, .y = 0xAF};
    const uint8_t canvpad = 4;
    SDL_FRect canvdst = {.x = (float)canvpad, .y = (float)canvpad};
    SDL_Color canvclr = {.r = 0xFF, .g = 0xFF, .b = 0xFF, .a = 0xFF};

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

    if (!SDL_SetTextureScaleMode(canvtex, SDL_SCALEMODE_PIXELART))
    {
        SDL_Log("failed to set texture scale mode: %s\n", SDL_GetError());
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
        const int bpp = 4; /* Bytes per pixel */
        const int pixel_pitch = pitch / bpp; /* Pitch in pixels */
        const SDL_Palette* pal = SDL_GetTexturePalette(canvtex);

        const SDL_PixelFormatDetails* pfd = SDL_GetPixelFormatDetails(canvtex->format);
        if (pfd == NULL)
        {
            SDL_Log("failed to get pixel format details: %s\n", SDL_GetError());
            return 1;
        }

        SDL_memset4(dst, SDL_MapRGBA(pfd, pal, 0, 0, 0, 0xFF), canvsiz.y * pixel_pitch);
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

        /* available drawable area */
        const float avail_w = (float)ow - 2.0f * (float)canvpad;
        const float avail_h = (float)oh - 2.0f * (float)canvpad;
        const float canvpxs = SDL_floorf(SDL_min(avail_w / (float)canvsiz.x, avail_h / (float)canvsiz.y));
        canvdst.w = canvpxs * (float)canvsiz.x;
        canvdst.h = canvpxs * (float)canvsiz.y;
        canvdst.x = ((float)ow - canvdst.w) * 0.5f;
        canvdst.y = ((float)oh - canvdst.h) * 0.5f;

        /* === Events === */
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                is_running = false;
                break;
            }

            float mx;
            float my;
            const SDL_MouseButtonFlags mbtn = SDL_GetMouseState(&mx, &my);

            if ((event.type == SDL_EVENT_MOUSE_BUTTON_DOWN ||
                    event.type == SDL_EVENT_MOUSE_MOTION) &&
                event.button.button == SDL_BUTTON_LEFT)
            {
                if (mx < canvdst.x || mx > canvdst.x + canvdst.w ||
                    my < canvdst.y || my > canvdst.y + canvdst.h)
                {
                    break;
                }

                /* relative mouse position */
                const float rx = mx - canvdst.x;
                const float ry = my - canvdst.y;

                /* pixel index */
                const uint16_t ix = (int)(rx / canvpxs);
                const uint16_t iy = (int)(ry / canvpxs);
                const uint32_t i = ix + iy * canvsiz.x;

                SDL_Log("mx = %.2f | "
                        "my = %.2f | "
                        "rx = %.2f | "
                        "ry = %.2f | "
                        "ix = %d | "
                        "iy = %d | "
                        "i = %d\n",
                        mx, my, rx, ry, ix, iy, i);

                {
                    void* pixels;
                    int pitch;
                    if (!SDL_LockTexture(canvtex, NULL, &pixels, &pitch))
                    {
                        SDL_Log("failed to lock texture: %s\n", SDL_GetError());
                        return 1;
                    }

                    uint32_t* dst = pixels;
                    const SDL_Palette* pal = SDL_GetTexturePalette(canvtex);

                    const SDL_PixelFormatDetails* pfd = SDL_GetPixelFormatDetails(canvtex->format);
                    if (pfd == NULL)
                    {
                        SDL_Log("failed to get pixel format details: %s\n", SDL_GetError());
                        return 1;
                    }

                    dst[i] = SDL_MapRGBA(pfd, pal, canvclr.r, canvclr.g, canvclr.b, canvclr.a);

                    SDL_UnlockTexture(canvtex);
                }

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
