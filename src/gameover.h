#pragma once
#include "button.h"
#include "gamestate.h"
#include "settings.h"
#include <raylib.h>

class GameOver final : public GameState {
public:
  GameOver(unsigned int s);
  ~GameOver();
  void Update(Engine *engine, float frame_delta) override;
  void Draw(Engine *engine) override;

private:
  unsigned int score = 0;
  Font font = LoadFont("./assets/dpcomic.ttf");
  float fontsize = 32;
  Button bback =
      Button("Back to Menu", WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 100, 200.0f, 100.0f);
  Button bquit = Button("Quit", WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 + 50, 200.0f, 100.0f);
};
