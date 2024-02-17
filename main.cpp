#include <algorithm>
#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <cstdlib>

constexpr int WINDOW_WIDTH = 768;
constexpr int WINDOW_HEIGHT = 768;
constexpr double DOWNWARD_ACCELERATION = 0.002f;
constexpr double JUMP_SPEED = -0.8f;
constexpr double INITIAL_SPEED = 0.5f;
constexpr double MAX_DOWNWARD_SPEED = 0.9f;
constexpr int RADIUS = 20;
constexpr double MIN_CLAW_Y = 150.0f;
constexpr double MAX_CLAW_Y = WINDOW_HEIGHT - 150.0f;
constexpr double CLAW_SPEED = 0.5f;
constexpr Uint32 CLAW_INTERVAL_MS = 850;
constexpr double GAP_SIZE = 130.0f;

class Claw {
public:
    double x, y;
    double width, height;
    double speed;

    static Claw Random(double vel) {
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
            static_cast<double>(WINDOW_HEIGHT / 10),
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
};

Uint32 CreateClaw(Uint32 interval, void* param) {
    auto* claws = static_cast<std::vector<Claw>*>(param);
    const Claw r = Claw::Random(CLAW_SPEED);
    claws->push_back(r);
    claws->push_back(Claw{r.x, 0.0f, r.width, (WINDOW_HEIGHT-r.height)-GAP_SIZE, CLAW_SPEED});
    return interval;
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Crabby Bucket", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    std::vector<Claw> claws;
    SDL_TimerID claw_timer_id = SDL_AddTimer(CLAW_INTERVAL_MS, CreateClaw, &claws);

    double x = 100;
    double y = static_cast<double>(WINDOW_HEIGHT) / 2;

    Uint64 frame_start = SDL_GetPerformanceCounter();
    double speed = INITIAL_SPEED;


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

        for (Claw& rect: claws) {
            rect.Draw(renderer);
            rect.Move(frame_delta);
        }
        claws.erase(std::remove_if(claws.begin(), claws.end(), [](const Claw& rect) {
            return rect.x + rect.width < 0;
        }), claws.end());

        // Draw the circle at the (x, y) coordinates
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (int w = -RADIUS; w < RADIUS; w++) {
            for (int h = -RADIUS; h < RADIUS; h++) {
                if (w*w + h*h <= RADIUS*RADIUS) {
                    SDL_RenderDrawPoint(renderer, static_cast<int>(x) + w, static_cast<int>(y) + h);
                }
            }
        }

        SDL_RenderPresent(renderer);
    }

    SDL_RemoveTimer(claw_timer_id);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}