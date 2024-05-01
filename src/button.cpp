#include "button.h"
#include "raylib.h"
#include "settings.h"

Button::Button(std::string t, float px, float py, float wi, float he) {
  text = t;
  posx = px;
  posy = py;
  w = wi;
  h = he;
}

Button::~Button() { UnloadFont(font); }

bool Button::MouseIsOver() const {
  int x = GetMouseX();
  int y = GetMouseY();
  return x > posx && x < posx + w && y > posy && y < posy + h;
}

void Button::Draw() const {
  Color btn_color = regular;
  if (MouseIsOver()) {
    btn_color = hover;
  }
  DrawRectangle(static_cast<int>(posx), static_cast<int>(posy), static_cast<int>(w),
                static_cast<int>(h), btn_color);

  // Draw the text in the middle of the button
  Vector2 size = MeasureTextEx(font, text.c_str(), fontsize, 1);
  DrawTextEx(font, text.c_str(),
             Vector2{(posx + w / 2) - (size.x / 2), (posy + h / 2) - (size.y / 2)}, fontsize, 1,
             SCORE_COLOR);
}
