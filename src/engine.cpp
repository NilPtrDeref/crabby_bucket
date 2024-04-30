#include "engine.h"
#include "gamestate.h"
#include "menu.h"
#include "settings.h"
#include <raylib.h>

Engine::Engine() {
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Crabby Bucket");
  SetTargetFPS(60);
  SetExitKey(0);

  GameState *menu = new Menu;
  ChangeState(menu);
  while (!WindowShouldClose() && !Close) {
    Update();

    BeginDrawing();
    Draw();
    EndDrawing();
  }
}

Engine::~Engine() {
  while (!states.empty())
    PopState();
  CloseWindow();
}

void Engine::ChangeState(GameState *state) {
  PopState();
  PushState(state);
}

void Engine::PushState(GameState *state) { states.push_back(state); }

void Engine::PopState() {
  if (states.empty())
    return;

  const GameState *current = states.back();
  delete current;
  states.erase(states.end());
}

void Engine::Update() {
  GameState *current = states.back();
  current->Update(this, GetFrameTime());
}

void Engine::Draw() {
  GameState *current = states.back();
  current->Draw(this);
}
