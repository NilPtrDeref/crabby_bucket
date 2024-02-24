#include "gameover.h"
#include "game.h"

void GameOver::HandleEvent(Engine *engine, SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN && bback.MouseIsOver()) {
        engine->PopState();
        engine->PopState();
    }
    if (event.type == SDL_MOUSEBUTTONDOWN && bquit.MouseIsOver()) {
        engine->Quit();
    }
}

void GameOver::Update(Engine *engine, double frame_delta) {
}

void GameOver::Draw(Engine *engine) {
    // Clear screen
    SDL_SetRenderDrawColor(engine->renderer, 255, 215, 0, 255);
    SDL_RenderFillRect(engine->renderer, nullptr);

    // Recreate score texture to render only when the score changes
    std::string score_text = std::to_string(score);
    SDL_Surface* score_surface = TTF_RenderText_Solid(font, score_text.c_str(), SCORE_COLOR);
    SDL_Texture* score_texture = SDL_CreateTextureFromSurface(engine->renderer, score_surface);
    int xoff = WINDOW_WIDTH/2 - score_surface->w/2;
    SDL_Rect score_rect = {xoff, WINDOW_HEIGHT/2 - 200, score_surface->w, score_surface->h};
    SDL_RenderCopy(engine->renderer, score_texture, nullptr, &score_rect);
    SDL_FreeSurface(score_surface);
    SDL_DestroyTexture(score_texture);

    // Draw buttons
    bback.Draw(engine->renderer);
    bquit.Draw(engine->renderer);
}