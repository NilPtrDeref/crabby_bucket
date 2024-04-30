#include "button.h"
#include "settings.h"
#include <raylib.h>

bool Button::MouseIsOver() const {
  int x = GetMouseX();
  int y = GetMouseY();
  return x > posx && x < posx + w && y > posy && y < posy + h;
}

void Button::Draw() const {
  if (MouseIsOver()) {
    DrawRectangle(static_cast<int>(posx), static_cast<int>(posy), static_cast<int>(w),
                  static_cast<int>(h), Color{220, 220, 220, 255});
  } else {
    DrawRectangle(static_cast<int>(posx), static_cast<int>(posy), static_cast<int>(w),
                  static_cast<int>(h), Color{255, 255, 255, 255});
  }

  Vector2 size = MeasureTextEx(font, text.c_str(), fontsize, 1);
  DrawTextEx(font, text.c_str(),
             Vector2{(posx + w / 2) - (size.x / 2), (posy + h / 2) - (size.y / 2)}, fontsize, 1,
             SCORE_COLOR);
}
