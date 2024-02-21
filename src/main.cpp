#define SDL_MAIN_HANDLED
#include "engine.h"
#include "gamestate.h"
#include "game.cpp"

// TODO: Game loop? (Menu, Gameover screen, Pause menu?)
// TODO: Art? (Non-moving background. Yellow bucket with sky at the end.)
// TODO: Rotation for crab based on speed?
// TODO: Animations on claws?
int main() {
    Engine engine;
    engine.Init();

    GameState* game = new Game;
    engine.ChangeState(game);
    while (engine.Running()) {
        engine.HandleEvents();
        engine.Update();
        engine.Draw();
    }

    engine.Cleanup();
    return 0;
}