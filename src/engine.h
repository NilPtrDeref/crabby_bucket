#pragma once
#include <vector>
#include <SDL2/SDL.h>

class GameState;

class Engine {
public:
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    int Init();
    void Cleanup();

    void ChangeState(GameState* state);
    void PushState(GameState* state);
    void PopState();

    void HandleEvents();
    void Update();
    void Draw();

    [[nodiscard]] bool Running() const { return running; }
    void Quit() { running = false; }

private:
    std::vector<GameState*> states = {};
    bool running = true;
    Uint64 last_frame = SDL_GetPerformanceCounter();
};
