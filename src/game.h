#pragma once
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include "gamestate.h"
#include "settings.h"

class Circle {
public:
    double x = PLAYER_COLUMN;
    double y = static_cast<double>(WINDOW_HEIGHT) / 2;
    double radius = RADIUS;
};

class Rectangle {
public:
    double x, y, width, height;
};

class Player {
public:
    Circle circ;
    double speed = INITIAL_SPEED;
    SDL_Texture* img = nullptr;

    void Draw(SDL_Renderer* renderer) const;
};

class Claw {
public:
    Rectangle rect{};
    double speed;
    bool top = false;
    SDL_Texture* img = nullptr;

    Claw(Rectangle r, double s, bool t, SDL_Texture* i) {
        rect = r;
        speed = s;
        top = t;
        img = i;
    }

    static Claw Random(const double vel, SDL_Texture* i) {
        double ypos = lerp(MIN_CLAW_Y + GAP_SIZE, WINDOW_HEIGHT - MIN_CLAW_Y - GAP_SIZE, static_cast<double>(rand()) / RAND_MAX);
        return Claw(
            Rectangle{
                static_cast<double>(WINDOW_WIDTH),
                ypos,
                CLAW_WIDTH,
                static_cast<double>(WINDOW_HEIGHT) - ypos,
            },
            vel,
            false,
            i
        );
    }

    void Draw(SDL_Renderer* renderer) const;
    void Move(double delta);
    [[nodiscard]] bool ShouldDestroy() const;
    [[nodiscard]] bool Collides(Circle c) const;
};

class Game final : public GameState {
public:
    unsigned int score = 0;

    explicit Game(SDL_Renderer* renderer):
    crab_img(IMG_LoadTexture(renderer, "./assets/crab.png")),
    claw_img(IMG_LoadTexture(renderer, "./assets/claw.png")) {
        player.img = crab_img;
    }

    ~Game() override {
        TTF_CloseFont(font);
        SDL_DestroyTexture(crab_img);
        SDL_DestroyTexture(claw_img);
    }

    void HandleEvent(Engine *engine, SDL_Event& event) override;
    void Update(Engine *engine, double frame_delta) override;
    void Draw(Engine *engine) override;

private:
    Player player = Player();
    std::vector<Claw> claws;
    double claw_timer = 0.0f;

    TTF_Font* font = TTF_OpenFont("./assets/dpcomic.ttf", 36);
    SDL_Texture* crab_img;
    SDL_Texture* claw_img;
};
