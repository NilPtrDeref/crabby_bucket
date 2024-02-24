#pragma once
#include "button.h"
#include "gamestate.h"
#include "settings.h"

class GameOver final : public GameState {
public:
    explicit GameOver(unsigned int s) {
        score = s;
    }

    ~GameOver() override {
        TTF_CloseFont(font);
    }

    void HandleEvent(Engine *engine, SDL_Event& event) override;
    void Update(Engine *engine, double frame_delta) override;
    void Draw(Engine *engine) override;

private:
    unsigned int score = 0;
    TTF_Font* font = TTF_OpenFont("./assets/dpcomic.ttf", 36);
    Button bback = Button("Back to Menu", WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 - 100, 200.0f, 100.0f);
    Button bquit = Button("Quit", WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 + 50, 200.0f, 100.0f);
};
