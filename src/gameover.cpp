#include "gameover.h"
#include <raylib.h>

void GameOver::Update(Engine *engine, double frame_delta) {
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    if (bback.MouseIsOver()) {
      engine->PopState();
      engine->PopState();
      return;
    }
    if (bquit.MouseIsOver()) {
      engine->Close = true;
    }
  }
}

void GameOver::Draw(Engine *engine) {
  DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, Color{255, 215, 0, 255});
  std::string score_text = std::to_string(score);
  Vector2 size = MeasureTextEx(font, score_text.c_str(), fontsize, 1);
  DrawTextEx(font, score_text.c_str(),
             Vector2{WINDOW_WIDTH / 2 - size.x / 2, WINDOW_HEIGHT / 2 - 200}, fontsize, 1,
             SCORE_COLOR);
  bback.Draw();
  bquit.Draw();
}
