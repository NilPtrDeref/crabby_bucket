#include "menu.h"
#include "game.h"
#include <raylib.h>

void Menu::Update(Engine *engine, float frame_delta) {
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    if (bstart.MouseIsOver()) {
      GameState *game = new Game();
      engine->PushState(game);
    }
    if (bquit.MouseIsOver()) {
      engine->Close = true;
    }
  }
}

void Menu::Draw(Engine *engine) {
  DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, Color{255, 215, 0, 255});
  bstart.Draw();
  bquit.Draw();
}
