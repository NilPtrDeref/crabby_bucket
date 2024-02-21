#pragma once
#include <SDL_ttf.h>
#include <string>

class Button {
public:
    Button(std::string t, double px, double py, double wi, double he) {
        text = t;
        posx = px;
        posy = py;
        w = wi;
        h = he;
    }

    ~Button() {
        TTF_CloseFont(font);
    }

    std::string text;
    double posx, posy, w, h;

    [[nodiscard]] bool MouseIsOver() const;
    void Draw(SDL_Renderer *renderer) const;

private:
    TTF_Font* font = TTF_OpenFont("./assets/dpcomic.ttf", 36);
};
