#pragma once
#include "gamestate.h"
#include "raylib.h"
#include "settings.h"
#include <cstdlib>

inline float lerp(float a, float b, float t) { return a + t * (b - a); }

class Player {
public:
  void Draw() const;

public:
  Vector2 center = {PLAYER_COLUMN, WINDOW_HEIGHT / 2};
  float radius;
  float speed = INITIAL_SPEED;
  Texture2D *img = nullptr;
};

class Claw {
public:
  Claw(Rectangle r, float s, bool t, Texture2D *i);
  Claw Random(const float vel, Texture2D *i);
  void Draw() const;
  void Move(float delta);
  [[nodiscard]] bool ShouldDestroy() const;
  [[nodiscard]] bool Collides(Vector2 center, float radius) const;

public:
  Rectangle rect;
  float speed;
  bool top = false;
  Texture2D *img = nullptr;
};

Claw RandomClaw(const float vel, Texture2D *i);

class Game final : public GameState {
public:
  Game();
  ~Game();
  void Update(Engine *engine, float frame_delta) override;
  void Draw(Engine *engine) override;

public:
  unsigned int score = 0;

private:
  Player player = Player();
  std::vector<Claw> claws;
  float claw_timer = 0.0f;
  Font font = LoadFont("./assets/dpcomic.ttf");
  float fontsize = 32;
  Texture2D bkg_img = LoadTexture("./assets/crabby_bkg.png");
  Texture2D crab_img = LoadTexture("./assets/crab.png");
  Texture2D claw_img = LoadTexture("./assets/claw.png");
};
