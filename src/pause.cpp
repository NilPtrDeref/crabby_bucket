#include "pause.h"
#include "raylib.h"
#include "settings.h"

Pause::Pause(GameState *c) { caller = c; }

void Pause::Update(Engine *engine, float frame_delta) {
  if (IsKeyPressed(KEY_ESCAPE)) {
    engine->PopState();
    return;
  }

  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    if (bback.MouseIsOver()) {
      engine->PopState();
      engine->PopState();
      return;
    }
    if (bresume.MouseIsOver()) {
      engine->PopState();
      return;
    }
  }
};

void Pause::Draw(Engine *engine) {
  if (caller != nullptr) {
    caller->Draw(engine);
    DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, Color{0, 0, 0, 50});
    bresume.Draw();
    bback.Draw();
  }
};
