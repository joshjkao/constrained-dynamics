#ifndef APP_H
#define APP_H

#include <iostream>
#include <fstream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "system.h"
#include "ODEsolver.h"
#include "doublependulum.h"

class App {
    public:
        App();
        ~App();
        bool init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
        bool systemSetup();
        void handleEvents();
        void update();
        void calculateLE();
        void step();
        void render();
        void clearSystem();
        void cleanup();
        bool running();

        static SDL_Renderer* renderer;
        static double dt;
    private:
        bool SDL_init();

        bool isRunning;
        int iterationsPerFrame = 400;
        bool isPaused = true;
        bool runTriple = false;
        SDL_Window* window;

        System* system = nullptr;
        ODESolver* solver = nullptr;

        std::ofstream avg;
        std::ofstream curr;

        TTF_Font* font = nullptr;

};

#endif