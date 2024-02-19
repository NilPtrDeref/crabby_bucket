#pragma once
#include <cstdlib>
#include "settings.h"

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

class Game {
public:
    bool running = true;
    unsigned int score = 0;

    ~Game() {
        TTF_CloseFont(font);
        if (score_surface != nullptr)
            SDL_FreeSurface(score_surface);
        if (score_texture != nullptr)
            SDL_DestroyTexture(score_texture);
    };

    void HandleEvent(const SDL_Event &event) {
            switch (event.type) {
            case SDL_KEYDOWN: {
                if (event.key.keysym.sym == SDLK_SPACE) player.speed = JUMP_SPEED;
                if (event.key.keysym.sym == SDLK_ESCAPE) paused = !paused;
                break;
            }
            default: break;
            }
    }
    void Update(const double frame_delta) {
        if (paused) return;

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
            running = false;
        }

        for (Claw& claw: claws) {
            // Check for collision
            if (claw.Collides(player.circ))
                running = false;

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
    void Draw(SDL_Renderer* renderer) {
        for (Claw& claw: claws) {
            claw.Draw(renderer);
        }

        // Draw the circle at the (x, y) coordinates
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (int w = -player.circ.radius; w < player.circ.radius; w++) {
            for (int h = -player.circ.radius; h < player.circ.radius; h++) {
                if (w*w + h*h <= player.circ.radius*player.circ.radius) {
                    SDL_RenderDrawPoint(renderer, static_cast<int>(player.circ.x) + w, static_cast<int>(player.circ.y) + h);
                }
            }
        }

        // Recreate score texture to render only when the score changes
        if (score_surface != nullptr)
            SDL_FreeSurface(score_surface);
        if (score_texture != nullptr)
            SDL_DestroyTexture(score_texture);
        score_text = std::to_string(score);
        score_surface = TTF_RenderText_Solid(font, score_text.c_str(), SCORE_COLOR);
        score_texture = SDL_CreateTextureFromSurface(renderer, score_surface);
        score_rect = {10, 10, score_surface->w, score_surface->h};
        SDL_RenderCopy(renderer, score_texture, nullptr, &score_rect);
    }

private:
    Player player = Player{ PLAYER_COLUMN, static_cast<double>(WINDOW_HEIGHT) / 2, RADIUS, INITIAL_SPEED};
    std::vector<Claw> claws;
    double claw_timer = 0.0f;
    bool paused = false;

    TTF_Font* font = TTF_OpenFont("./assets/dpcomic.ttf", 36);
    std::string score_text = "0";
    SDL_Surface* score_surface = nullptr;
    SDL_Texture* score_texture = nullptr;
    SDL_Rect score_rect = {};
};