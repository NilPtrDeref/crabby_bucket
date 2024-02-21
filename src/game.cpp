#include <cstdlib>
#include <string>
#include <SDL2/SDL_ttf.h>
#include "gamestate.h"
#include "settings.h"
#include "pause.h"

class Circle {
public:
    double x, y, radius;
};

class Rectangle {
public:
    double x, y, width, height;
};

class Player {
public:
    Circle circ;
    double speed;

    void Draw(SDL_Renderer* renderer) const {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        for (int w = -circ.radius; w < circ.radius; w++) {
            for (int h = -circ.radius; h < circ.radius; h++) {
                if (w*w + h*h <= circ.radius*circ.radius) {
                    SDL_RenderDrawPoint(renderer, static_cast<int>(circ.x) + w, static_cast<int>(circ.y) + h);
                }
            }
        }
    }
};

class Claw {
public:
    Rectangle rect;
    double speed;

    static Claw Random(const double vel) {
        double ypos = lerp(MIN_CLAW_Y + GAP_SIZE, WINDOW_HEIGHT - MIN_CLAW_Y - GAP_SIZE, static_cast<double>(rand()) / RAND_MAX);
        return Claw{
            static_cast<double>(WINDOW_WIDTH),
            ypos,
            CLAW_WIDTH,
            static_cast<double>(WINDOW_HEIGHT) - ypos,
            vel,
        };
    }

    void Draw(SDL_Renderer* renderer) const {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        const SDL_Rect r {static_cast<int>(rect.x), static_cast<int>(rect.y), static_cast<int>(rect.width), static_cast<int>(rect.height)};
        SDL_RenderFillRect(renderer, &r);
    }

    void Move(const double delta) {
        rect.x -= speed * delta;
    }

    [[nodiscard]] bool ShouldDestroy() const {
        return rect.x + rect.width < 0;
    }

    [[nodiscard]] bool Collides(const Circle c) const {
        // Calculate the closest point on the rectangle to the center of the circle
        const double closestX = std::max(rect.x, std::min(c.x, rect.x + rect.width));
        const double closestY = std::max(rect.y, std::min(c.y, rect.y + rect.height));

        // Calculate the distance between the closest point and the center of the circle
        const double distanceX = closestX - c.x;
        const double distanceY = closestY - c.y;
        const double distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);

        // Check if the distance is less than or equal to the radius of the circle
        return distanceSquared <= (c.radius * c.radius);
    }
};

class Game final : public GameState {
public:
    unsigned int score = 0;
    std::string score_text = "0";

    ~Game() override {
        TTF_CloseFont(font);
    }

    void HandleEvent(Engine *engine, SDL_Event& event) override {
        switch (event.type) {
        case SDL_KEYDOWN: {
            if (event.key.keysym.sym == SDLK_SPACE) player.speed = JUMP_SPEED;
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                GameState* pause = new Pause(this);
                engine->PushState(pause);
            }
            break;
        }
        default: break;
        }
    }

    void Update(Engine *engine, double frame_delta) override {
        claw_timer += frame_delta;
        if (claw_timer > CLAW_INTERVAL_MS) {
            const Claw r = Claw::Random(CLAW_SPEED);
            claws.push_back(r);
            claws.push_back(Claw{r.rect.x, 0.0f, r.rect.width, (WINDOW_HEIGHT-r.rect.height)-GAP_SIZE, CLAW_SPEED});
            claw_timer = 0.0f;
        }

        // Move y by velocity and check that velocity is not pushing you past the border
        player.circ.y += static_cast<int>(player.speed * frame_delta);
        if (player.circ.y - player.circ.radius < 0) {
            player.circ.y = 1.0f + player.circ.radius;
            player.speed = 0.0f;
        }
        if (player.speed < MAX_DOWNWARD_SPEED) player.speed += DOWNWARD_ACCELERATION * frame_delta;

        // You lose if you go below the bottom border
        if (player.circ.y - player.circ.radius > WINDOW_HEIGHT) {
            engine->Quit();
        }

        for (Claw& claw: claws) {
            // Check for collision
            if (claw.Collides(player.circ))
                engine->Quit();

            claw.Move(frame_delta);
        }

        // Remove the claws that are off the screen and increment the score
        if (claws.size() > 1) {
            if (claws[0].ShouldDestroy()) {
                claws.erase(claws.begin());
                claws.erase(claws.begin());
                score++;

            }
        }
    }

    void Draw(Engine *engine) override {
        // Clear the background to white and then draw the game
        SDL_SetRenderDrawColor(engine->renderer, 255, 255, 255, 255);
        SDL_RenderClear(engine->renderer);

        for (Claw& claw: claws) {
            claw.Draw(engine->renderer);
        }
        player.Draw(engine->renderer);

        // Recreate score texture to render only when the score changes
        score_text = std::to_string(score);
        SDL_Surface* score_surface = TTF_RenderText_Solid(font, score_text.c_str(), SCORE_COLOR);
        SDL_Texture* score_texture = SDL_CreateTextureFromSurface(engine->renderer, score_surface);
        SDL_Rect score_rect = {10, 10, score_surface->w, score_surface->h};
        SDL_RenderCopy(engine->renderer, score_texture, nullptr, &score_rect);
        SDL_FreeSurface(score_surface);
        SDL_DestroyTexture(score_texture);
    }

private:
    Player player = Player{ PLAYER_COLUMN, static_cast<double>(WINDOW_HEIGHT) / 2, RADIUS, INITIAL_SPEED};
    std::vector<Claw> claws;
    double claw_timer = 0.0f;

    TTF_Font* font = TTF_OpenFont("./assets/dpcomic.ttf", 36);
};