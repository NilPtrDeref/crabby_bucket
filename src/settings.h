#pragma once
#include <SDL2/SDL.h>

constexpr int WINDOW_WIDTH = 1024;
constexpr int WINDOW_HEIGHT = 768;
constexpr double DOWNWARD_ACCELERATION = 0.002f;
constexpr double JUMP_SPEED = -0.8f;
constexpr double INITIAL_SPEED = 0.3f;
constexpr double MAX_DOWNWARD_SPEED = 0.9f;
constexpr int RADIUS = 20;
constexpr double MIN_CLAW_Y = 150.0f;
constexpr double MAX_CLAW_Y = WINDOW_HEIGHT - 150.0f;
constexpr double CLAW_WIDTH = 50.0f;
constexpr double CLAW_SPEED = 0.5f;
constexpr double CLAW_INTERVAL_MS = 850.0f;
constexpr double GAP_SIZE = 130.0f;
constexpr double PLAYER_COLUMN = 100.0f;
constexpr SDL_Color SCORE_COLOR = {0, 0, 0, 255};
