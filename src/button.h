#pragma once
#include "raylib.h"
#include <string>

class Button {
public:
  Button(std::string t, float px, float py, float wi, float he);
  ~Button();
  [[nodiscard]] bool MouseIsOver() const;
  void Draw() const;

public:
  std::string text;
  float posx, posy, w, h;
  Color hover = Color{220, 220, 220, 255};
  Color regular = Color{255, 255, 255, 255};

private:
  Font font = LoadFont("./assets/dpcomic.ttf");
  float fontsize = 32;
};
