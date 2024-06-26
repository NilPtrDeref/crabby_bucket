#pragma once
#include "button.h"
#include "gamestate.h"
#include "settings.h"

class Menu final : public GameState {
public:
  void Update(Engine *engine, float frame_delta) override;
  void Draw(Engine *engine) override;

private:
  Button bstart = Button("Start", WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 100, 200.0f, 100.0f);
  Button bquit = Button("Quit", WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 + 50, 200.0f, 100.0f);
};
