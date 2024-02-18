#include <algorithm>
#include <iostream>
#include <vector>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cstdlib>

constexpr int WINDOW_WIDTH = 1024;
constexpr int WINDOW_HEIGHT = 768;
constexpr double DOWNWARD_ACCELERATION = 0.002f;
constexpr double JUMP_SPEED = -0.8f;
constexpr double INITIAL_SPEED = 0.5f;
constexpr double MAX_DOWNWARD_SPEED = 0.9f;
constexpr int RADIUS = 20;
constexpr double MIN_CLAW_Y = 150.0f;
constexpr double MAX_CLAW_Y = WINDOW_HEIGHT - 150.0f;
constexpr double CLAW_WIDTH = 50.0f;
constexpr double CLAW_SPEED = 0.5f;
constexpr Uint32 CLAW_INTERVAL_MS = 850;
constexpr double GAP_SIZE = 130.0f;
constexpr double PLAYER_COLUMN = 100.0f;
constexpr SDL_Color SCORE_COLOR = {255, 255, 255, 255};

class Claw {
public:
    double x, y;
    double width, height;
    double speed;

    static Claw Random(const double vel) {
        double ypos = rand() % WINDOW_HEIGHT;
        if (ypos < MIN_CLAW_Y) {
            ypos = MIN_CLAW_Y;
        }
        if (ypos > MAX_CLAW_Y) {
            ypos = MAX_CLAW_Y;
        }
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
        const SDL_Rect rect {static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height)};
        SDL_RenderFillRect(renderer, &rect);
    }

    void Move(const double delta) {
        x -= speed * delta;
    }

    [[nodiscard]] bool ShouldDestroy() const {
        return x + width < 0;
    }
};

Uint32 CreateClaw(const Uint32 interval, void* param) {
    auto* claws = static_cast<std::vector<Claw>*>(param);
    const Claw r = Claw::Random(CLAW_SPEED);
    claws->push_back(r);
    claws->push_back(Claw{r.x, 0.0f, r.width, (WINDOW_HEIGHT-r.height)-GAP_SIZE, CLAW_SPEED});
    return interval;
}

// TODO: Collision
// TODO: Game loop? (Menu, Gameover screen, Pause menu?)
// TODO: Art?
// TODO: Animations on claws?
// TODO: Rotation for crab based on speed?
int main() {
    if (SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "There was an error initializing SDL_video: " << SDL_GetError() << std::endl;
        return 1;
    }
    if (TTF_Init() == -1) {
        std::cerr << "There was an error initializing SDL_ttf: " << TTF_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Crabby Bucket", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "There was an error initializing SDL window: " << SDL_GetError() << std::endl;
        return 1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        std::cerr << "There was an error initializing SDL renderer: " << SDL_GetError() << std::endl;
        return 1;
    }

    TTF_Font* font = TTF_OpenFont("./assets/dpcomic.ttf", 36);
    if (font == nullptr) {
        std::cerr << "There was an error loading the font: " << TTF_GetError() << std::endl;
        return 1;
    }

    std::vector<Claw> claws;
    const SDL_TimerID claw_timer_id = SDL_AddTimer(CLAW_INTERVAL_MS, CreateClaw, &claws);

    const double x = PLAYER_COLUMN;
    double y = static_cast<double>(WINDOW_HEIGHT) / 2;

    Uint64 frame_start = SDL_GetPerformanceCounter();
    double speed = INITIAL_SPEED;

    unsigned int score = 0;
    std::string score_text = "0";
    SDL_Surface* score_surface = TTF_RenderText_Solid(font, score_text.c_str(), SCORE_COLOR);
    SDL_Texture* score_texture = SDL_CreateTextureFromSurface(renderer, score_surface);
    SDL_Rect score_rect = {10, 10, score_surface->w, score_surface->h};

    SDL_Event event;
    bool running = true;
    while (running) {
        const Uint64 frame_end = frame_start;
        frame_start = SDL_GetPerformanceCounter();
        const double frame_delta = static_cast<double>(frame_start - frame_end) * 1000 / static_cast<double>(SDL_GetPerformanceFrequency());

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT: {
                running = false;
                break;
            }
            case SDL_KEYDOWN: {
                if (event.key.keysym.sym == SDLK_SPACE) speed = JUMP_SPEED;
                break;
            }
            default: break;
            }
        }

        // Move y by velocity and check that velocity is not pushing you past the border
        y += static_cast<int>(speed * frame_delta);
        if (y - RADIUS < 0) {
            y = 1.0f + RADIUS;
            speed = 0.0f;
        }
        if (speed < MAX_DOWNWARD_SPEED) speed += DOWNWARD_ACCELERATION * frame_delta;

        // You lose if you go below the bottom border
        if (y - RADIUS > WINDOW_HEIGHT) {
            running = false;
        }

        // Clear the background to black
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (Claw& claw: claws) {
            claw.Draw(renderer);
            claw.Move(frame_delta);
        }
        if (claws.size() > 1) {
            if (claws[0].ShouldDestroy()) {
                claws.erase(claws.begin());
                claws.erase(claws.begin());
                score++;

                // Recreate score texture to render only when the score changes
                SDL_FreeSurface(score_surface);
                SDL_DestroyTexture(score_texture);
                score_text = std::to_string(score);
                score_surface = TTF_RenderText_Solid(font, score_text.c_str(), SCORE_COLOR);
                score_texture = SDL_CreateTextureFromSurface(renderer, score_surface);
                score_rect = {10, 10, score_surface->w, score_surface->h};
            }
        }

        // Draw the circle at the (x, y) coordinates
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (int w = -RADIUS; w < RADIUS; w++) {
            for (int h = -RADIUS; h < RADIUS; h++) {
                if (w*w + h*h <= RADIUS*RADIUS) {
                    SDL_RenderDrawPoint(renderer, static_cast<int>(x) + w, static_cast<int>(y) + h);
                }
            }
        }

        SDL_RenderCopy(renderer, score_texture, nullptr, &score_rect);
        SDL_RenderPresent(renderer);
    }

    SDL_RemoveTimer(claw_timer_id);
    TTF_CloseFont(font);
    SDL_FreeSurface(score_surface);
    SDL_DestroyTexture(score_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}