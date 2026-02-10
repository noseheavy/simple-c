//
// minimal example
//
// standard template
// for sdl2
//

/* * * * * *
 * MACROS  *
 * * * * * */

#define WIDTH  800
#define HEIGHT 600

#define PIXEL(a, b) pixels[a + surf->w * b]

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// #include <pthread.h>
#include <SDL2/SDL.h>

/* * * * * * *
 * VARIABLES *
 * * * * * * */

SDL_bool running = SDL_TRUE;

SDL_Window  *win;
SDL_Surface *surf;
Uint32      *pixels;

/* * * * * * *
 * FUNCTIONS *
 * * * * * * */

// todo: float rgb => int
// Uint32 color
// todo: bresenham, pixel at end pos...
void drawLine(int x1, int y1,
              int x2, int y2,
              Uint32 color);
// todo: ui
// void drawButton

// run at beginning
int init(void) {
    if (SDL_Init(SDL_INIT_VIDEO)) return 1;

    // mac: working area is centered, border irrelevant
    // first create splash screen
    win = SDL_CreateWindow("Minimal Example",
                           SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                           640, 480,
                           SDL_WINDOW_BORDERLESS);
    surf   = SDL_GetWindowSurface(win);
    pixels = surf->pixels;

    // graphics
    for (int j = 0; j < 480; j++) {
        for (int i = 0; i < 640; i++) {
            PIXEL(i, j) = 0xFFFFFF;
        }
    }
    drawLine(0, 0, 639, 479, 0x000000);
    SDL_UpdateWindowSurface(win);

    // mac
    SDL_PumpEvents(); // summon window
    SDL_PumpEvents(); // bring focus

    // test
    for (int i = 0; i < 10000000; i++) {
        printf("%d\n", i);
    }

    // setup normal window
    SDL_SetWindowSize(win, WIDTH, HEIGHT);
    SDL_SetWindowPosition(win, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_SetWindowBordered(win, SDL_TRUE);
    surf   = SDL_GetWindowSurface(win);
    pixels = surf->pixels;

    return 0;
}

// run continuously
int run(void) {
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = SDL_FALSE;
                break;
            default:
                // return 1;
                break;
        }
    }

    // graphics
    float t = SDL_GetTicks()/1000.0;
    drawLine(799.0/2, 599.0/2,
             799.0/2 + 100 * cos(t), 599.0/2 + 100 * sin(t),
             0xFFFFFF);
    drawLine(799.0/2, 599.0/2,
             799.0/2 + 100 * cos(t - 1), 599.0/2 + 100 * sin(t - 1),
             0x000000);
    // drawLine(0, 0, 799, 599, 0x000000);
    SDL_UpdateWindowSurface(win);

    return 0;
}

int main(void) {
    if (init()) {
        printf("error: initialization\n");

        return 1;
    }

    while (running) {
        if (run()) {
            running = SDL_FALSE;
            printf("error: unhandled event\n");
        }
    }

    return 0;
}

/* * * * * * * *
 * DEFINITION  *
 * * * * * * * */

// void color
void drawLine(int x1, int y1,
              int x2, int y2,
              Uint32 color) {
    int x = x1;
    int y = y1;

    char stepX = (x2 > x1)? 1 : -1;
    char stepY = (y2 > y1)? 1 : -1;
    float accum = 0;
    do { // even if r1 = r2
        PIXEL(x, y) = color;

        if (stepX * (x2 - x1) > stepY * (y2 - y1)) {
            x += stepX;

            accum += 1.0 * (y2 - y1)/(x2 - x1) * stepX;
            if (accum > 0.5) {
                accum -= 1;
                y++;
            } else if (accum < -0.5) {
                accum += 1;
                y--;
            }
        } else {
            y += stepY;

            accum += 1.0 * (x2 - x1)/(y2 - y1) * stepY;
            if (accum > 0.5) {
                accum -= 1;
                x++;
            } else if (accum < -0.5) {
                accum += 1;
                x--;
            }
        }

        PIXEL(x, y) = color;
    } while (x != x2 || y != y2);
}
