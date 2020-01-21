#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <Windows.h>
#include <time.h>

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int ppc = 10; //pixels per cell
int field[108][192];
int fieldCopy[108][192];

void paintCell(SDL_Renderer* renderer, int ppc, int c, int l) {

    for (int pc = c * ppc; pc < c * ppc + ppc; pc++) {
        for (int pl = l * ppc; pl < l * ppc + ppc; pl++) {
            SDL_RenderDrawPoint(renderer, pc, pl);
        }
    }
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    else
    {
        //Create window
        window = SDL_CreateWindow("Game Of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
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
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        //SDL RendererClear sets entire window to color specified in renderer draw color
    }
    for(int c = 0; c < SCREEN_WIDTH / ppc; c++) {
        for(int l = 0; l < SCREEN_HEIGHT / ppc; l++) {
            if (rand() % 2 == 0) {
                field[l][c] = 1;
                fieldCopy[l][c] = 1;
                paintCell(renderer, ppc, c, l);
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
        for (int c = 0; c < SCREEN_WIDTH / ppc; c++) {
            for (int l = 0; l < SCREEN_HEIGHT / ppc; l++) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                int AliveNeighbors = 0;
                for (int nc = c - 1; nc <= c + 1; nc++) {
                    for (int nl = l - 1; nl <= l + 1; nl++) {
                        int nlav = nl;
                        int ncav = nc;
                        if (nl < 0) {
                            nl = SCREEN_HEIGHT / ppc - 1;
                        }
                        if (nl > SCREEN_HEIGHT / ppc - 1) {
                            nl = 0;
                        }
                        if (nc < 0) {
                            nc = SCREEN_WIDTH / ppc - 1;
                        }
                        if (nc > SCREEN_WIDTH / ppc - 1) {
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
                    paintCell(renderer, ppc, c, l);
                }
                if (AliveNeighbors == 3) {
                    fieldCopy[l][c] = 1;
                    paintCell(renderer, ppc, c, l);
                }
            }
        }
        for (int c = 0; c < SCREEN_WIDTH / ppc; c++) {
            for (int l = 0; l < SCREEN_HEIGHT / ppc; l++) {
                field[l][c] = fieldCopy[l][c];
            }
        }
        SDL_RenderPresent(renderer);
        generations++;
        printf("%i\n", generations);
        //Sleep(500);
        //Sleep(x) -> wait x miliseconds before continuing
    }
    return 0;
}
    