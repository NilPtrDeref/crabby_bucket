#pragma once
#include "button.h"
#include "gamestate.h"
#include "settings.h"

class Pause final : public GameState {
public:
  Pause(GameState *c);
  void Update(Engine *engine, float frame_delta) override;
  void Draw(Engine *engine) override;

private:
  GameState *caller = nullptr;
  Button bresume =
      Button("Resume", WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 100, 200.0f, 100.0f);
  Button bback =
      Button("Back to Menu", WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 + 50, 200.0f, 100.0f);
};
