#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

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

typedef struct {
    Man man;

    // Images
    SDL_Texture *texture;
} GameState;

void structs() {
    Man man;
    man.x = 50;
    man.y = 50;
    man.life = 100;
    man.name = "John";
}

int ProcessEvents(SDL_Window *w, GameState *game) {
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
                default:
                    break;
            }
        }

        const uint8_t *state = SDL_GetKeyboardState(NULL);
        if(state[SDL_SCANCODE_RETURN]) {
            printf("<RETURN> is pressed.\n");
        }

        if(state[SDL_SCANCODE_LEFT]) {
            game->man.x -= 10;
        }

        if(state[SDL_SCANCODE_RIGHT]) {
            game->man.x += 10;
        }

        if(state[SDL_SCANCODE_UP]) {
            game->man.y -= 10;
        }

        if(state[SDL_SCANCODE_DOWN]) {
            game->man.y += 10;
        }
    }
}

void doRender(SDL_Renderer *renderer, GameState* game) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

    // clear screen to blue
    SDL_RenderClear(renderer);

    // set draw color to white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // draw a rectangle
    SDL_Rect rect = { game->man.x, game->man.y, 100, 100 };
    SDL_RenderFillRect(renderer, &rect);

    // draw image
    SDL_Rect imageRect = { 50, 50, 128, 128 };
    SDL_RenderCopy(renderer, game->texture, NULL, &imageRect);

    // done drawing, present to screen
    SDL_RenderPresent(renderer);

    //SDL_Delay(10);
}

int main(int argc, const char *argv[]) {
    /*int array[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    for(int i = 0; i < sizeof(array[i]); i++) {
        array[i] = i;
        printf("element at %d %d\n", i, array[i]);
    }

    printf("%d\n", array[0]);
    printf("Max: %d\n", max(array, 10));*/

    GameState gameState;
    SDL_Window *window; // declare a window
    SDL_Renderer *renderer; // declare a renderer

    SDL_Surface *imageSurface = NULL; // src image in memory, texture is rendered image

    SDL_Init(SDL_INIT_VIDEO); // initialize sdl2

    Man man;
    man.x = 220;
    man.y = 140;

    // create window
    window = SDL_CreateWindow("Game Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    int imgFlags = IMG_INIT_PNG;

    if(!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize. Error %s\n", IMG_GetError());
    }

    imageSurface = IMG_Load("../triangle.png");

    if(imageSurface == NULL) {
        printf("SDL_Image could not intialize. Error %s\n", IMG_GetError());
        SDL_Delay(5000);
        return 1;
    }

    gameState.texture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    SDL_FreeSurface(imageSurface);

    int done = 0;
    while(!done) {
        done = ProcessEvents(window, &gameState);

        doRender(renderer, &gameState);
    }

    SDL_DestroyTexture(gameState.texture);

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    SDL_Quit();
}
