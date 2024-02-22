#pragma once
#include "button.h"
#include "gamestate.h"
#include "settings.h"

class Menu final : public GameState {
public:
    void HandleEvent(Engine *engine, SDL_Event& event) override;
    void Update(Engine *engine, double frame_delta) override;
    void Draw(Engine *engine) override;

private:
    Button b = Button("Start", WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 - 50, 200.0f, 100.0f);
};
