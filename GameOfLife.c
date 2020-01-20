#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <Windows.h>

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int field[1080][1920];
int fieldCopy[1080][1920];

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    else
    {
        //Create window
        window = SDL_CreateWindow("Game Of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_MAXIMIZED);
        if (window == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        }
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        //SDL RendererClear sets entire window to color specified in renderer draw color
    }
    for(int c = 0; c < 1920; c++) {
        for(int l = 0; l < 1080; l++) {
            if (rand() % 13 == 0) {
                field[l][c] = 1;
                fieldCopy[l][c] = 1;
                SDL_RenderDrawPoint(renderer, c, l);
            }
            else {
                field[l][c] = 0;
                fieldCopy[l][c] = 0;
            }
        }
    }
    SDL_RenderPresent(renderer);
    int generations = 0;
    printf("%i\n", generations);
    while (1) {
        for (int c = 0; c < 1920; c++) {
            for (int l = 0; l < 1080; l++) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                int AliveNeighbors = 0;
                for (int nc = c - 1; nc <= c + 1; nc++) {
                    for (int nl = l - 1; nl <= l + 1; nl++) {
                        int nlav = nl;
                        int ncav = nc;
                        if (nl < 0) {
                            nl = 1079;
                        }
                        if (nl > 1079) {
                            nl = 0;
                        }
                        if (nc < 0) {
                            nc = 1919;
                        }
                        if (nc > 1919) {
                            nc = 0;
                        }
                        if (nl != l || nc != c) {
                            if (field[nl][nc] == 1) {
                                AliveNeighbors++;
                            }
                        }
                        nl = nlav;
                        nc = ncav;
                    }
                }
                if (AliveNeighbors < 2 || AliveNeighbors > 3) {
                    fieldCopy[l][c] = 0;
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderDrawPoint(renderer, c, l);
                }
                if (AliveNeighbors == 3) {
                    fieldCopy[l][c] = 1;
                    SDL_RenderDrawPoint(renderer, c, l);
                }
            }
        }
        for (int c = 0; c < 1920; c++) {
            for (int l = 0; l < 1080; l++) {
                field[l][c] = fieldCopy[l][c];
            }
        }
        SDL_RenderPresent(renderer);
        generations++;
        printf("%i\n", generations);
        //Sleep(250);
        //Sleep(x) -> wait x miliseconds before continuing
    }
    return 0;
}
    