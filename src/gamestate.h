#pragma once
#include "engine.h"
#include <SDL2/SDL.h>

class GameState {
public:
    virtual ~GameState() = default;
    virtual void HandleEvent(Engine *engine, SDL_Event& event) = 0;
    virtual void Update(Engine *engine, double frame_delta) = 0;
    virtual void Draw(Engine *engine) = 0;
};
