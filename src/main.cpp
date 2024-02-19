#include <algorithm>
#include <iostream>
#include <vector>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "game.h"
#include "settings.h"

// TODO: Game loop? (Menu, Gameover screen, Pause menu?)
// TODO: Art? (Non-moving background. Yellow bucket with sky at the end.)
// TODO: Rotation for crab based on speed?
// TODO: Animations on claws?
int main() {
    if (SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "There was an error initializing SDL_video: " << SDL_GetError() << std::endl;
        return 1;
    }
    if (TTF_Init() == -1) {
        std::cerr << "There was an error initializing SDL_ttf: " << TTF_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Crabby Bucket", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "There was an error initializing SDL window: " << SDL_GetError() << std::endl;
        return 1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        std::cerr << "There was an error initializing SDL renderer: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Put main game loop in scope so that variables get deallocated properly
    {
        Game game;
        SDL_Event event;
        Uint64 frame_start = SDL_GetPerformanceCounter();
        bool running = true;
        while (running && game.running) {
            const Uint64 frame_end = frame_start;
            frame_start = SDL_GetPerformanceCounter();
            const double frame_delta = static_cast<double>(frame_start - frame_end) * 1000 / static_cast<double>(SDL_GetPerformanceFrequency());

            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                case SDL_QUIT: {
                    running = false;
                    break;
                }
                default: game.HandleEvent(event);
                }
            }

            game.Update(frame_delta);

            // Clear the background to black and then draw the game
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            game.Draw(renderer);
            SDL_RenderPresent(renderer);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}