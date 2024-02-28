#include "menu.h"
#include "game.h"

void Menu::HandleEvent(Engine *engine, SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN && bstart.MouseIsOver()) {
        GameState* game = new Game(engine->renderer);
        engine->PushState(game);
    }
    if (event.type == SDL_MOUSEBUTTONDOWN && bquit.MouseIsOver()) {
        engine->Quit();
    }
}

void Menu::Update(Engine *engine, double frame_delta) {

}

void Menu::Draw(Engine *engine) {
    SDL_SetRenderDrawColor(engine->renderer, 255, 215, 0, 255);
    SDL_RenderFillRect(engine->renderer, nullptr);
    bstart.Draw(engine->renderer);
    bquit.Draw(engine->renderer);
}