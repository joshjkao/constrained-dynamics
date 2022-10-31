#include "app.h"

App *app = nullptr;

int main() {
    srand(time(0));
    app = new App();
    if (!app->init("Pendulums", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 32*32, 19*32, SDL_WINDOW_OPENGL)) return -1;

    const int fps = 60;
    const int frameDelay = 1000/fps;
    Uint32 frameStart;
    int frameTime;

    // Main loop
    while (app->running()) {
        frameStart = SDL_GetTicks();

        // Check for inputs
        app->handleEvents();

        // Update object states
        app->update();

        // Draw new frame
        app->render();

        // Frame rate limiter
        frameTime = SDL_GetTicks() - frameStart;
        // std::cout << frameDelay - frameTime << std::endl;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay-frameTime);
        }
    }

    app->cleanup();
    return 0;
}