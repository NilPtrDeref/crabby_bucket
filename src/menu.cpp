#include "menu.h"
#include "game.h"

void Menu::HandleEvent(Engine *engine, SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN && b.MouseIsOver()) {
        GameState* game = new Game;
        engine->PushState(game);
    }
}

void Menu::Update(Engine *engine, double frame_delta) {

}

void Menu::Draw(Engine *engine) {
    SDL_SetRenderDrawColor(engine->renderer, 255, 215, 0, 255);
    SDL_RenderFillRect(engine->renderer, nullptr);
    b.Draw(engine->renderer);
}