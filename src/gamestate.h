#pragma once
#include "engine.h"

class GameState {
public:
  virtual ~GameState() = default;
  virtual void Update(Engine *engine, float frame_delta) = 0;
  virtual void Draw(Engine *engine) = 0;
};
