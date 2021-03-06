#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <Windows.h>
#include <time.h>

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int ppc = 6; //pixels per cell side (square this number to get total cell area)

void paintCell(SDL_Renderer* renderer, int ppc, int c, int l) {
    SDL_Rect rect;
    rect.x = c * ppc;
    rect.y = l * ppc;
    rect.w = ppc;
    rect.h = ppc;
    SDL_RenderFillRect(renderer, &rect);    
}

int borderCheck(int v, int maxv) {
    if (v < 0) {
        return maxv;
    }
    if (v > maxv) {
        return 0;
    }
    return v;
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    int fieldHeight = SCREEN_HEIGHT / ppc;
    int fieldWidth = SCREEN_WIDTH / ppc;
    int* field = malloc(fieldHeight * fieldWidth * sizeof(int));
    int* fieldChanges = malloc(fieldHeight * fieldWidth * sizeof(int));
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
    for (int c = 0; c < fieldWidth; c++) {
        for (int l = 0; l < fieldHeight; l++) {
            //if (rand() % 11 == 0) {
            //if ((c == ((fieldWidth - 1) / 2) && l > 0 && l < fieldHeight - 1) || (l == ((fieldHeight - 1) / 2) && c > 0 && c < fieldWidth - 1)) {
            //if ((c == ((fieldWidth - 1) / 2) && l > 0 && l < fieldHeight - 1)) {
            if ((c == ((fieldWidth - 1) / 2) && l > 0 && l < fieldHeight - 1)) {
                field[l * fieldWidth + c] = 1;
                paintCell(renderer, ppc, c, l);
            }
            else {
                field[l * fieldWidth + c] = 0;
            }
        }
    }
    SDL_RenderPresent(renderer);
    int generations = 0;
    printf("%i\n", generations);
    Sleep(2000);
    //Sleep(x) -> wait x miliseconds before continuing
    while (1) {
        //int r = rand() % 255 + 10;
        //int g = rand() % 255 + 10;
        //int b = rand() % 255 + 10;
        int changes = 0;
        for (int c = 0; c < fieldWidth; c++) {
            for (int l = 0; l < fieldHeight; l++) {
                //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                int AliveNeighbors = 0;
                for (int nc = c - 1; nc <= c + 1; nc++) {
                    for (int nl = l - 1; nl <= l + 1; nl++) {
                        int ncaux = nc;
                        int nlaux = nl;
                        if (nc < 0) {
                            nc = fieldWidth - 1;
                        }
                        if (nc > fieldWidth - 1) {
                            nc = 0;
                        }
                        if (nl < 0) {
                            nl = fieldHeight - 1;
                        }
                        if (nl > fieldHeight - 1) {
                            nl = 0;
                        }
                        if (nl != l || nc != c){
                            if (field[nl * fieldWidth + nc] == 1) {
                                AliveNeighbors++;
                            }
                        }
                        nc = ncaux;
                        nl = nlaux;
                        if (AliveNeighbors > 3) {
                            nl = l + 1;
                            nc = c + 1;
                        }
                    }
                }
                if (AliveNeighbors == 3) {
                    if (field[l * fieldWidth + c] != 1) {
                        fieldChanges[changes] = l * fieldWidth + c;
                        changes++;
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                        paintCell(renderer, ppc, c, l);
                    }
                }
                else {
                    if (AliveNeighbors > 3 || AliveNeighbors < 2) {
                        if (field[l * fieldWidth + c] != 0) {
                            fieldChanges[changes] = -(l * fieldWidth + c);
                            changes++;
                            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                            paintCell(renderer, ppc, c, l);
                        }
                    }
                }
            }
        }
        for (int i = 0; i < changes; i++) {
            int coords = fieldChanges[i];
            if (coords < 0) {
                field[-coords] = 0;
            }
            else {
                field[coords] = 1;
            }
        }
        SDL_RenderPresent(renderer);
        generations++;
        printf("%i\n", generations);
        Sleep(50);
    }
    free(field);
    free(fieldChanges);
    return 0;
}