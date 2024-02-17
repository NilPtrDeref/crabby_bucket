#include <algorithm>
#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <cstdlib>

constexpr int WINDOW_WIDTH = 768;
constexpr int WINDOW_HEIGHT = 768;
constexpr double DOWNWARD_ACCELERATION = 0.002f;

class Rectangle {
public:
    double x, y;
    double width, height;
    double speed;

    static Rectangle Random() {
        const auto ypos = static_cast<double>(rand() % WINDOW_HEIGHT);
        return Rectangle{
            static_cast<double>(WINDOW_WIDTH),
            ypos,
            static_cast<double>(WINDOW_HEIGHT / 10),
            static_cast<double>(WINDOW_HEIGHT) - ypos,
            0.5f,
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

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Crabby Bucket", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    double x = 100;
    double y = static_cast<double>(WINDOW_HEIGHT) / 2;
    constexpr int radius = 20;

    Uint64 frame_start = SDL_GetPerformanceCounter();
    double speed = 0.5f; // Move this many pixels per millisecond

    std::vector<Rectangle> rectangles;
    rectangles.push_back(Rectangle::Random());

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
                if (event.key.keysym.sym == SDLK_SPACE) speed = -0.8f;
                break;
            }
            case SDL_MOUSEBUTTONDOWN: {
                // TODO: Move this out to something else. Automated system.
                rectangles.push_back(Rectangle::Random());
                break;
            }
            default: break;
            }
        }

        // Move y by velocity and check that velocity is not pushing you past the border
        y += static_cast<int>(speed * frame_delta);
        if (y - radius < 0) {
            y = 1.0f + radius;
            speed = 0.0f;
        }
        if (speed < 0.9f) speed += DOWNWARD_ACCELERATION * frame_delta;

        // You lose if you go below the bottom border
        if (y - radius > WINDOW_HEIGHT) {
            running = false;
        }

        // Clear the background to black
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (Rectangle& rect: rectangles) {
            rect.Draw(renderer);
            rect.Move(frame_delta);
        }
        rectangles.erase(std::remove_if(rectangles.begin(), rectangles.end(), [](const Rectangle& rect) {
            return rect.x + rect.width < 0;
        }), rectangles.end());

        // Draw the circle at the (x, y) coordinates
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (int w = -radius; w < radius; w++) {
            for (int h = -radius; h < radius; h++) {
                if (w*w + h*h <= radius*radius) {
                    SDL_RenderDrawPoint(renderer, static_cast<int>(x) + w, static_cast<int>(y) + h);
                }
            }
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}