#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct {
    int x, y;
    short life;
    char *name;
} Man;

typedef struct {
    int x,y;
} Triangle;

typedef struct {
    Man man;

    Triangle triangles[100];

    // Images
    SDL_Texture *texture;

    SDL_Renderer* renderer;
} GameState;

void loadGame(GameState* game, SDL_Renderer *renderer) {
    SDL_Surface *imageSurface = NULL; // src image in memory, texture is rendered image

    int imgFlags = IMG_INIT_PNG;

    if(!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize. Error %s\n", IMG_GetError());
    }

    imageSurface = IMG_Load("../triangle.png");

    if(imageSurface == NULL) {
        printf("SDL_Image could not intialize. Error %s\n", IMG_GetError());
        SDL_Delay(5000);
        exit(1);
    }

    game->texture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    SDL_FreeSurface(imageSurface);

    game->man.x = 320-40;
    game->man.y = 240-40;

    // init triangles
    for(int i = 0; i < 100; i++) {
        game->triangles[i].x = i*64;
        game->triangles[i].y = i*32;
    }
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

void doRender(GameState* game) {
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 255, 255);

    // clear screen to blue
    SDL_RenderClear(game->renderer);

    // set draw color to white
    SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);

    // draw a rectangle
    SDL_Rect rect = { game->man.x, game->man.y, 100, 100 };
    SDL_RenderFillRect(game->renderer, &rect);

    // draw image
    for(int i = 0; i < 100; i++) {
        SDL_Rect imageRect = { game->triangles[i].x, game->triangles[i].y, 128, 128 };
        SDL_RenderCopy(game->renderer, game->texture, NULL, &imageRect);
    }

    // done drawing, present to screen
    SDL_RenderPresent(game->renderer);

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

    SDL_Init(SDL_INIT_VIDEO); // initialize sdl2

    Man man;
    man.x = 220;
    man.y = 140;

    // create window
    window = SDL_CreateWindow("Game Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    gameState.renderer = renderer;

    loadGame(&gameState, gameState.renderer);

    int done = 0;
    while(!done) {
        done = ProcessEvents(window, &gameState);

        doRender(&gameState);
    }

    SDL_DestroyTexture(gameState.texture);

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(gameState.renderer);

    SDL_Quit();
}
