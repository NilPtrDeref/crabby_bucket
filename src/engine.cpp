#include "engine.h"
#include "gamestate.h"
#include "settings.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

int Engine::Init() {
    if (SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "There was an error initializing SDL_video: " << SDL_GetError() << std::endl;
        return 1;
    }
    if (TTF_Init() == -1) {
        std::cerr << "There was an error initializing SDL_ttf: " << TTF_GetError() << std::endl;
        return 1;
    }

    window = SDL_CreateWindow("Crabby Bucket", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "There was an error initializing SDL window: " << SDL_GetError() << std::endl;
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        std::cerr << "There was an error initializing SDL renderer: " << SDL_GetError() << std::endl;
        return 1;
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    return 0;
}

void Engine::Cleanup() {
    while (!states.empty())
        PopState();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

void Engine::ChangeState(GameState* state) {
    PopState();
    PushState(state);
}

void Engine::PushState(GameState* state) {
   states.push_back(state);
}

void Engine::PopState() {
    if (states.empty()) return;

    const GameState *current = states.back();
    delete current;
    states.erase(states.end());
}

void Engine::HandleEvents() {
    GameState *current = states.back();

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT: {
            Quit();
            break;
        }
        default: current->HandleEvent(this, event);
        }
    }
}

void Engine::Update() {
    const Uint64 frame_end = last_frame;
    last_frame = SDL_GetPerformanceCounter();
    const double frame_delta = static_cast<double>(last_frame - frame_end) * 1000 / static_cast<double>(SDL_GetPerformanceFrequency());

    GameState *current = states.back();
    current->Update(this, frame_delta);
}

void Engine::Draw() {
    GameState *current = states.back();
    current->Draw(this);
    SDL_RenderPresent(renderer);
}

