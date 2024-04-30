#include "game.h"
#include "gameover.h"
#include "gamestate.h"
#include "pause.h"
#include "settings.h"
#include <cstdio>
#include <raylib.h>
#include <string>

void Player::Draw() const {
  DrawTexture(*img, static_cast<int>(center.x) - img->width / 2,
              static_cast<int>(center.y) - img->height / 2, WHITE);
}

void Claw::Draw() const {
  if (top) {
    DrawRectangle(static_cast<int>(rect.x), static_cast<int>(rect.y), static_cast<int>(rect.width),
                  static_cast<int>(rect.height) - img->height, Color{223, 62, 35, 255});
  } else {
    DrawRectangle(static_cast<int>(rect.x), static_cast<int>(rect.y) + img->height,
                  static_cast<int>(rect.width), static_cast<int>(rect.height) - img->height,
                  Color{223, 62, 35, 255});
  }

  if (top) {
    // DrawTextureEx(*img, Vector2{rect.x - 16, rect.height - img->height}, 180, 1, WHITE);
    DrawTextureEx(*img, Vector2{rect.x + 64 - 16, rect.height - 5}, 180, 1, WHITE);
  } else {
    DrawTextureV(*img, Vector2{rect.x - 16, rect.y}, WHITE);
  }
}

void Claw::Move(const float delta) { rect.x -= speed * delta; }

bool Claw::ShouldDestroy() const { return rect.x + rect.width < 0; }

bool Claw::Collides(Vector2 center, float radius) const {
  return CheckCollisionCircleRec(center, radius, rect);
}

void Game::Update(Engine *engine, double frame_delta) {
  // Handle key input
  if (IsKeyPressed(KEY_SPACE))
    player.speed = JUMP_SPEED;
  if (IsKeyPressed(KEY_ESCAPE)) {
    GameState *pause = new Pause(this);
    engine->PushState(pause);
    return;
  }

  claw_timer += frame_delta;
  if (claw_timer > CLAW_INTERVAL_MS) {
    auto t = Claw::Random(CLAW_SPEED, &claw_img);
    auto b = Claw(Rectangle{t.rect.x, 0.0f, t.rect.width,
                            static_cast<float>((WINDOW_HEIGHT - t.rect.height) - GAP_SIZE)},
                  CLAW_SPEED, true, &claw_img);
    claws.push_back(b);
    claws.push_back(t);
    claw_timer = 0.0f;
  }

  // Move y by velocity and check that velocity is not pushing you past the
  // border
  player.center.y += player.speed * frame_delta;
  if (player.center.y - player.radius < 0) {
    player.center.y = 1.0f + player.radius;
    player.speed = 0.0f;
  }
  if (player.speed < MAX_DOWNWARD_SPEED)
    player.speed += DOWNWARD_ACCELERATION * frame_delta;

  // You lose if you go below the bottom border
  if (player.center.y - player.radius > WINDOW_HEIGHT) {
    GameState *gameover = new GameOver(score);
    engine->PushState(gameover);
    return;
  }

  for (Claw &claw : claws) {
    // Check for collision
    if (claw.Collides(player.center, player.radius)) {
      GameState *gameover = new GameOver(score);
      engine->PushState(gameover);
      return;
    }

    claw.Move(frame_delta);
  }

  // Remove the claws that are off the screen and increment the score
  if (claws.size() > 1) {
    if (claws[0].ShouldDestroy()) {
      claws.erase(claws.begin());
      claws.erase(claws.begin());
      score++;
    }
  }
}

void Game::Draw(Engine *engine) {
  // Clear the background to white and then draw the game
  ClearBackground(WHITE);
  DrawTexture(bkg_img, 0, 0, WHITE);

  for (Claw &claw : claws) {
    claw.Draw();
  }
  player.Draw();

  // Recreate score texture to render only when the score changes
  std::string score_text = std::to_string(score);
  DrawTextEx(font, score_text.c_str(), Vector2{10, 10}, fontsize, 1, SCORE_COLOR);
}
