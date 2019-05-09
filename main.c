#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <SDL2/SDL.h>

void updateFrame(SDL_Renderer *renderer, SDL_Texture *texture, Uint32 *pixels,
                 SDL_PixelFormat *fmt, int w, int h, double t)
{
    int i, j;
    double om = 5.0;

    for(i=0; i<h; i++)
        for(j=0; j<w; j++)
        {
            double r1 = sqrt(i*i + j*j);
            double r2 = sqrt((i-h)*(i-h) + j*j);
            double r3 = sqrt(i*i + (j-w)*(j-w));
            double r4 = sqrt((i-h)*(i-h) + (j-w)*(j-w));
            Uint8 r = (Uint8) (256*(0.5*(sin(2*M_PI*r1/50 - om*t)+1)));
            Uint8 g = (Uint8) (256*(0.5*(sin(2*M_PI*r2/50 - om*t)+1)));
            Uint8 b = (Uint8) (256*(0.5*(sin(2*M_PI*r3/50 - om*t)+1)));
            double k = (0.5*(sin(2*M_PI*r4/50 - om*t)+1));
            pixels[w*i+j] = SDL_MapRGB(fmt, k*r, k*g, k*b);
        }

        SDL_UpdateTexture(texture, NULL, pixels, w * sizeof(Uint32));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[])
{
    int width = 800;
    int height = 600;

    SDL_Window *screen;
    SDL_Renderer *sdlRenderer;
    SDL_CreateWindowAndRenderer(width, height, 
                                SDL_WINDOW_BORDERLESS,
                                &screen, &sdlRenderer);

    SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);
    SDL_RenderClear(sdlRenderer);
    SDL_RenderPresent(sdlRenderer);

    Uint32 pixFormatCode = SDL_PIXELFORMAT_ARGB8888;

    SDL_Texture *sdlTexture = SDL_CreateTexture(sdlRenderer,
                                    pixFormatCode,
                                    SDL_TEXTUREACCESS_STREAMING,
                                    width, height);

    SDL_PixelFormat *pixFmt = SDL_AllocFormat(pixFormatCode);

    Uint32 c0 = 255;
    Uint32 c1 = 255 * 256;

    Uint32 *pixels = (Uint32 *)malloc(width*height*sizeof(Uint32));

    int frames = 0;
    double t = 0.0;

    int step = 20;
    int printstep = 20;

    Uint64 t0 = SDL_GetPerformanceCounter();
    Uint64 then = SDL_GetPerformanceCounter();
    Uint64 now = SDL_GetPerformanceCounter();

    while(frames < 500)
    {
        now = SDL_GetPerformanceCounter();
        t = (double)(now-t0) / SDL_GetPerformanceFrequency();
        updateFrame(sdlRenderer, sdlTexture, pixels, pixFmt, width, height, t);
        int i,j;
        for(i=0; i<height; i++)
            for(j=0; j<width; j++)
            {
            }

        frames++;

        if(frames % step == 0)
        {
            double elapsed = (double)(now-then)/SDL_GetPerformanceFrequency();
            if(frames % printstep == 0)
                printf("frame: %d elapsed = %.3e s fps = %.2f\n", frames,
                    elapsed, step/elapsed);
            then = now;
        }
    }


    free(pixels);
    SDL_FreeFormat(pixFmt);
    SDL_Quit();
    return 0;
}
