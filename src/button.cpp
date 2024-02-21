#include "button.h"

#include "settings.h"

bool Button::MouseIsOver() const {
    int x;
    int y;
    SDL_GetMouseState(&x, &y);
    return x > posx && x < posx+w && y > posy && y < posy+h;
}

void Button::Draw(SDL_Renderer *renderer) const {
    if (MouseIsOver()) {
        SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    }
    const SDL_Rect r {static_cast<int>(posx), static_cast<int>(posy), static_cast<int>(w), static_cast<int>(h)};
    SDL_RenderFillRect(renderer, &r);

    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), SCORE_COLOR);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    int x = (posx + w/2) - (surface->w/2);
    int y = (posy + h/2) - (surface->h/2);
    SDL_Rect button_rect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, nullptr, &button_rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
