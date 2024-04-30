#pragma once
#include <raylib.h>
#include <string>

class Button {
public:
  Button(std::string t, float px, float py, float wi, float he) {
    text = t;
    posx = px;
    posy = py;
    w = wi;
    h = he;
  }

  ~Button() { UnloadFont(font); }

  std::string text;
  float posx, posy, w, h;

  [[nodiscard]] bool MouseIsOver() const;
  void Draw() const;

private:
  Font font = LoadFont("./assets/dpcomic.ttf");
  float fontsize = 36;
};
