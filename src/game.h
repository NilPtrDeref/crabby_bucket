#pragma once
#include "gamestate.h"
#include "settings.h"
#include <cstdlib>
#include <raylib.h>

inline float lerp(float a, float b, float t) { return a + t * (b - a); }

class Player {
public:
  Vector2 center = {PLAYER_COLUMN, WINDOW_HEIGHT / 2};
  float radius;
  float speed = INITIAL_SPEED;
  Texture2D *img = nullptr;

  void Draw() const;
};

class Claw {
public:
  Rectangle rect;
  float speed;
  bool top = false;
  Texture2D *img = nullptr;

  Claw(Rectangle r, float s, bool t, Texture2D *i) {
    rect = r;
    speed = s;
    top = t;
    img = i;
  }

  static Claw Random(const double vel, Texture2D *i) {
    float ypos = lerp(MIN_CLAW_Y + GAP_SIZE, WINDOW_HEIGHT - MIN_CLAW_Y - GAP_SIZE,
                      static_cast<float>(rand()) / RAND_MAX);
    return Claw(
        Rectangle{
            static_cast<float>(WINDOW_WIDTH),
            ypos,
            CLAW_WIDTH,
            static_cast<float>(WINDOW_HEIGHT) - ypos,
        },
        vel, false, i);
  }

  void Draw() const;
  void Move(float delta);
  [[nodiscard]] bool ShouldDestroy() const;
  [[nodiscard]] bool Collides(Vector2 center, float radius) const;
};

class Game final : public GameState {
public:
  unsigned int score = 0;

  explicit Game()
      : bkg_img(LoadTexture("./assets/crabby_bkg.png")), crab_img(LoadTexture("./assets/crab.png")),
        claw_img(LoadTexture("./assets/claw.png")) {
    player.img = &crab_img;
  }

  ~Game() override {
    UnloadFont(font);
    UnloadTexture(bkg_img);
    UnloadTexture(crab_img);
    UnloadTexture(claw_img);
  }

  void Update(Engine *engine, double frame_delta) override;
  void Draw(Engine *engine) override;

private:
  Player player = Player();
  std::vector<Claw> claws;
  float claw_timer = 0.0f;

  Font font = LoadFont("./assets/dpcomic.ttf");
  float fontsize = 36;
  Texture2D bkg_img;
  Texture2D crab_img;
  Texture2D claw_img;
};
