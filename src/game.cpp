#include "game.h"
#include <string>
#include <SDL2/SDL_ttf.h>
#include "gameover.h"
#include "gamestate.h"
#include "settings.h"
#include "pause.h"

void Player::Draw(SDL_Renderer* renderer) const {
    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    // for (int w = -circ.radius; w < circ.radius; w++) {
    //     for (int h = -circ.radius; h < circ.radius; h++) {
    //         if (w*w + h*h <= circ.radius*circ.radius) {
    //             SDL_RenderDrawPoint(renderer, static_cast<int>(circ.x) + w, static_cast<int>(circ.y) + h);
    //         }
    //     }
    // }

    int w, h;
    SDL_QueryTexture(img, nullptr, nullptr, &w, &h);
    SDL_Rect shape{ static_cast<int>(circ.x) - w / 2, static_cast<int>(circ.y) - h / 2, w, h };
    SDL_RenderCopy(renderer, img, nullptr, &shape);
}

void Claw::Draw(SDL_Renderer* renderer) const {
    int w, h;
    SDL_QueryTexture(img, nullptr, nullptr, &w, &h);

    SDL_SetRenderDrawColor(renderer, 223, 62, 35, 255);
    if (top) {
        const SDL_Rect r {static_cast<int>(rect.x), static_cast<int>(rect.y), static_cast<int>(rect.width), static_cast<int>(rect.height)-h};
        SDL_RenderFillRect(renderer, &r);
    } else {
        const SDL_Rect r {static_cast<int>(rect.x), static_cast<int>(rect.y)+h, static_cast<int>(rect.width), static_cast<int>(rect.height)-h};
        SDL_RenderFillRect(renderer, &r);
    }

    if (top) {
        SDL_Rect shape{ static_cast<int>(rect.x)-16, static_cast<int>(rect.height) - h, w, h };
        auto flip = static_cast<SDL_RendererFlip>(SDL_FLIP_HORIZONTAL|SDL_FLIP_VERTICAL);
        SDL_RenderCopyEx(renderer, img, nullptr, &shape, 0, nullptr, flip);
    } else {
        SDL_Rect shape{ static_cast<int>(rect.x)-16, static_cast<int>(rect.y), w, h };
        SDL_RenderCopyEx(renderer, img, nullptr, &shape, 0, nullptr, SDL_FLIP_HORIZONTAL);
    }
}

void Claw::Move(const double delta) {
    rect.x -= speed * delta;
}

bool Claw::ShouldDestroy() const {
    return rect.x + rect.width < 0;
}

bool Claw::Collides(const Circle c) const {
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

void Game::HandleEvent(Engine *engine, SDL_Event& event) {
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

void Game::Update(Engine *engine, double frame_delta) {
    claw_timer += frame_delta;
    if (claw_timer > CLAW_INTERVAL_MS) {
        auto t = Claw::Random(CLAW_SPEED, claw_img);
        auto b = Claw(Rectangle{t.rect.x, 0.0f, t.rect.width, (WINDOW_HEIGHT-t.rect.height)-GAP_SIZE}, CLAW_SPEED, true, claw_img);
        claws.push_back(b);
        claws.push_back(t);
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
        GameState* gameover = new GameOver(score);
        engine->PushState(gameover);
        return;
    }

    for (Claw& claw: claws) {
        // Check for collision
        if (claw.Collides(player.circ)) {
            GameState* gameover = new GameOver(score);
            engine->PushState(gameover);
            return;
        }

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

void Game::Draw(Engine *engine) {
    // Clear the background to white and then draw the game
    SDL_SetRenderDrawColor(engine->renderer, 255, 255, 255, 255);
    SDL_RenderClear(engine->renderer);

    int w, h;
    SDL_QueryTexture(bkg_img, nullptr, nullptr, &w, &h);
    SDL_Rect bgrect{0, 0, w, h};
    SDL_RenderCopy(engine->renderer, bkg_img, nullptr, &bgrect);

    for (Claw& claw: claws) {
        claw.Draw(engine->renderer);
    }
    player.Draw(engine->renderer);

    // Recreate score texture to render only when the score changes
    std::string score_text = std::to_string(score);
    SDL_Surface* score_surface = TTF_RenderText_Solid(font, score_text.c_str(), SCORE_COLOR);
    SDL_Texture* score_texture = SDL_CreateTextureFromSurface(engine->renderer, score_surface);
    SDL_Rect score_rect = {10, 10, score_surface->w, score_surface->h};
    SDL_RenderCopy(engine->renderer, score_texture, nullptr, &score_rect);
    SDL_FreeSurface(score_surface);
    SDL_DestroyTexture(score_texture);
}
