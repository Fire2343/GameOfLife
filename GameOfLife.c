#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <Windows.h>
#include <time.h>

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int ppc = 5; //pixels per cell side (square this number to get total cell area)

void paintCell(SDL_Renderer *renderer, int ppc, int c, int l) {

    SDL_Rect rect;
    rect.x = c * ppc;
    rect.y = l * ppc;
    rect.w = ppc;
    rect.h = ppc;

    SDL_RenderFillRect(renderer, &rect);
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    int fieldHeight = SCREEN_HEIGHT / ppc;
    int fieldWidth = SCREEN_WIDTH / ppc;
    int *field = malloc(fieldHeight * fieldWidth * sizeof(int));
    int *fieldCopy = malloc(fieldHeight * fieldWidth * sizeof(int));
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    else
    {
        //Create window
        window = SDL_CreateWindow("Game Of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, NULL);
        if (window == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        }
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
        if (renderer == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        //SDL RendererClear sets entire window to color specified in renderer draw color
    }
    for(int c = 0; c < fieldWidth; c++) {
        for(int l = 0; l < fieldHeight; l++) {
            if (rand() % 3 == 0) {
                field[l * fieldWidth + c] = 1;
                fieldCopy[l * fieldWidth + c] = 1;
                paintCell(renderer, ppc, c, l);
            }
            else {
                field[l * fieldWidth + c] = 0;
                fieldCopy[l * fieldWidth + c] = 0;
            }
        }
    }
    SDL_RenderPresent(renderer);
    int generations = 0;
    printf("%i\n", generations);
    while (1) {
        for (int c = 0; c < fieldWidth; c++) {
            for (int l = 0; l < fieldHeight; l++) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                int AliveNeighbors = 0;
                for (int nc = c - 1; nc <= c + 1; nc++) {
                    for (int nl = l - 1; nl <= l + 1; nl++) {
                        int nlav = nl;
                        int ncav = nc;
                        if (nl < 0) {
                            nl = fieldHeight - 1;
                        }
                        if (nl > fieldHeight - 1) {
                            nl = 0;
                        }
                        if (nc < 0) {
                            nc = fieldWidth - 1;
                        }
                        if (nc > fieldWidth - 1) {
                            nc = 0;
                        }
                        if (nl != l || nc != c) {
                            if (field[nl * fieldWidth + nc] == 1) {
                                AliveNeighbors++;
                            }
                        }
                        nl = nlav;
                        nc = ncav;
                    }
                }
                int *v = &fieldCopy[l * fieldWidth + c];
                if (AliveNeighbors < 2 || AliveNeighbors > 3) {
                    if (*v != 0) {
                        *v = 0;
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                        paintCell(renderer, ppc, c, l);
                    }
                }
                if (AliveNeighbors == 3) {
                    if (*v != 1) {
                        *v = 1;
                        paintCell(renderer, ppc, c, l);
                    }
                }
            }
        }
        for (int c = 0; c < fieldWidth; c++) {
            for (int l = 0; l < fieldHeight; l++) {
                field[l * fieldWidth + c] = fieldCopy[l * fieldWidth + c];
            }
        }
        SDL_RenderPresent(renderer);
        generations++;
        printf("%i\n", generations);
        Sleep(17);
        //Sleep(x) -> wait x miliseconds before continuing
    }
    free(field);
    free(fieldCopy);
    return 0;
}
    