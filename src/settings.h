#pragma once
#include <raylib.h>

constexpr int WINDOW_WIDTH = 1024;
constexpr int WINDOW_HEIGHT = 768;
constexpr float DOWNWARD_ACCELERATION = 0.002f;
constexpr float JUMP_SPEED = -0.8f;
constexpr float INITIAL_SPEED = 0.3f;
constexpr float MAX_DOWNWARD_SPEED = 0.9f;
constexpr int RADIUS = 20;
constexpr float MIN_CLAW_Y = 150.0f;
constexpr float MAX_CLAW_Y = WINDOW_HEIGHT - 150.0f;
constexpr float CLAW_WIDTH = 32.0f;
constexpr float CLAW_SPEED = 0.5f;
constexpr float CLAW_INTERVAL_MS = 850.0f;
constexpr float GAP_SIZE = 130.0f;
constexpr float PLAYER_COLUMN = 100.0f;
constexpr Color SCORE_COLOR = {0, 0, 0, 255};
