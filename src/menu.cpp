#include "menu.h"
#include "game.h"
#include <raylib.h>

void Menu::Update(Engine *engine, double frame_delta) {
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
  bstart.Draw();
  bquit.Draw();
}
