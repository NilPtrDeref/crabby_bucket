#pragma once
#include <vector>

class GameState;

class Engine {
public:
  int Init();
  Engine();
  ~Engine();

  void ChangeState(GameState *state);
  void PushState(GameState *state);
  void PopState();

  void Update();
  void Draw();

public:
  bool Close = false;

private:
  std::vector<GameState *> states = {};
};
