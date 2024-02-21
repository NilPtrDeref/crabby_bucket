#pragma once
#include "gamestate.h"

class Pause final : public GameState {
public:
    GameState* caller = nullptr;

    explicit Pause(GameState* c) {
        caller = c;
    }
    ~Pause() override = default;
    void HandleEvent(Engine *engine, SDL_Event& event) override;
    void Update(Engine *engine, double frame_delta) override;
    void Draw(Engine *engine) override;
};
