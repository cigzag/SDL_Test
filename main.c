#include <stdio.h>
#include <SDL2/SDL.h>

int max(int array[], int n) {
    int ret = 0;

    for(int i = 0; i < n; i++) {
        if(array[i] > ret) {
            ret = array[i];
        }
    }

    return ret;
}

typedef struct
{
    int x, y;
    short life;
    char *name;
} Man;

void structs() {
    Man man;
    man.x = 50;
    man.y = 50;
    man.life = 100;
    man.name = "John";
}

int ProcessEvents(SDL_Window *w, Man *man) {
    SDL_Event e;

    while(SDL_PollEvent(&e) != 0) {
        if(e.type == SDL_QUIT) {
            return 1;
        }

        if(e.type == SDL_WINDOWEVENT_CLOSE) {
            if(w) {
                SDL_DestroyWindow(w);
                w = NULL;
                return 1;
            }
        }

        if(e.type == SDL_KEYDOWN) {
            switch(e.key.keysym.sym) {
                case SDLK_ESCAPE: {
                    return 1;
                }
                case SDLK_RIGHT: {
                    man->x += 10;
                    break;
                }
                case SDLK_LEFT: {
                    man->x -= 10;
                    break;
                }
                default:
                    break;
            }
        }
    }
}

void doRender(SDL_Renderer *renderer, Man *man) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

    // clear screen to blue
    SDL_RenderClear(renderer);

    // set draw color to white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // draw a rectangle
    SDL_Rect rect = { man->x, man->y, 200, 200 };
    SDL_RenderFillRect(renderer, &rect);

    // done drawing, present to screen
    SDL_RenderPresent(renderer);

    SDL_Delay(100);
}

int main(int argc, const char *argv[]) {
    /*int array[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    for(int i = 0; i < sizeof(array[i]); i++) {
        array[i] = i;
        printf("element at %d %d\n", i, array[i]);
    }

    printf("%d\n", array[0]);
    printf("Max: %d\n", max(array, 10));*/

    SDL_Window *window; // declare a window
    SDL_Renderer *renderer; // declare a renderer

    SDL_Init(SDL_INIT_VIDEO); // initialize sdl2

    Man man;
    man.x = 220;
    man.y = 140;

    // create window
    window = SDL_CreateWindow("Game Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    int done = 0;
    while(!done) {
        done = ProcessEvents(&window, &man);

        doRender(renderer, &man);
    }
}
