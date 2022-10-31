#include "app.h"
#include "texturemanager.h"

SDL_Renderer* App::renderer = nullptr;
double App::dt = 0.01;
LagrangianDoublePendulum* Lpendulum = nullptr;
Vector2d testvec(0,0);
Object* origin = nullptr;
Object* control = nullptr;
ForceGenerator* controlgrav = nullptr;

int mousex=0;
int mousey=0;

App::App():
isRunning(false), window(nullptr)
{

}

App::~App() {

}

bool App::SDL_init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) > 0) {
        std::cout << "SDL_Init failed with error: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

bool App::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
    if(!SDL_init()) {
        cleanup();
        isRunning = false;
        return false; 
    }
    std::cout << "SDL initialized" << std::endl;
    window = SDL_CreateWindow(title, xpos, ypos, width, height, SDL_WINDOW_OPENGL);
    if (window == nullptr) {
        std::cout << "unable to create the main window with error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        isRunning = false;
        return false;
    }
    std::cout << "Window created" << std::endl;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    std::cout << "Renderer created" << std::endl;
    isRunning = true;

    // Lpendulum = new LagrangianDoublePendulum();

    // Sample Simulation: 
    // "The dependendence on initial conditions [of the double pendulum] was so sensitive that the gravitational pull
    // of a single raindrop a mile away mixed up the motion within two minutes"
    //          -from James Gleick in "Chaos: Making a New Science"
    system = new System();
    origin = new ImmovableObject();
    origin->pos = Vector2d(5,1);
    origin->mass = 0.1;

    Object* pend1m1 = new Object();
    pend1m1->pos = Vector2d(4,1);
    pend1m1->mass = 0.5;
    system->addObject(pend1m1);

    Object* pend1m2 = new Object();
    pend1m2->pos = Vector2d(3,1);
    pend1m2->mass = 0.5;
    system->addObject(pend1m2);

    Constraint* distconstraint1 = new DistanceConstraintFixed(pend1m1, origin->pos);
    Constraint* distconstraint2 = new DistanceConstraint(pend1m1, pend1m2);
    system->addConstraint(distconstraint1);
    system->addConstraint(distconstraint2);

    Object* pend2m1 = new Object();
    pend2m1->pos = Vector2d(4,1);
    pend2m1->mass = 0.5;
    system->addObject(pend2m1);

    Object* pend2m2 = new Object();
    pend2m2->pos = Vector2d(3,1);
    pend2m2->mass = 0.5;
    system->addObject(pend2m2);

    Constraint* distconstraint3 = new DistanceConstraintFixed(pend2m1, origin->pos);
    Constraint* distconstraint4 = new DistanceConstraint(pend2m1, pend2m2);
    system->addConstraint(distconstraint3);
    system->addConstraint(distconstraint4);

    // A single raindrop, a mile away, to pull on the second pendulum
    Object* raindrop = new ImmovableObject();
    raindrop->mass = 0.000034;
    raindrop->pos = Vector2d(1609,1);
    system->addObject(raindrop);

    ForceGenerator* grav1 = new GravAttract(raindrop, pend2m1);
    ForceGenerator* grav2 = new GravAttract(raindrop, pend2m2);
    system->addForceGenerator(grav1);
    system->addForceGenerator(grav2);

    for (auto& o: system->objects) {
        ForceGenerator* grav = new Gravity(o);
        system->generators.push_back(grav);
    }

    solver = new EulerSolver();
    solver->start(system, 1.0/30000);

    system->dump(std::cout);

    return true;
}

void App::handleEvents() {
    SDL_Event event;

    if (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_KEYDOWN: {
                isRunning = event.key.keysym.scancode != SDL_SCANCODE_ESCAPE;
                if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
                    SDL_GetMouseState(&mousex, &mousey);
                    isPaused = !isPaused;
                    if (isPaused) {
                        system->dump(std::cout);
                    }
                }
                else if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
                    if (!isPaused) break;
                    step();
                }
                break;
            }
            case SDL_QUIT: {
                isRunning = false;
                break;
            }
            default:
                break;
        }
    }
}

void App::update() {
    if (isPaused) return;
    for (int i = 0; i < iterationsPerFrame; i++) {
        step();
    }

    // std::cout << system->calculateEnergy() << std::endl;
}

void App::step() {
    // Clear before every iteration
    system->clearForces();

    // Apply F external (e.g. gravity)
    for (auto& g: system->generators) {
        g->update();
    }

    // Apply any extra forces here
    // system->objects[0]->applyForce(Vector2d(100,-100));

    // Adjust for constraints
    system->applyConstraintForces();

    // Step forward using new system state
    solver->iterate(system);

    if (Lpendulum != nullptr) {
        Lpendulum->update();
    }
}

void App::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    if (Lpendulum != nullptr) {
        Lpendulum->render(renderer);
    }
    origin->render(renderer);
    system->render(renderer);
    SDL_RenderPresent(renderer);
}

void App::cleanup() {
    delete solver;
    system->cleanup();
    delete system;

    if (window != nullptr) {
        SDL_DestroyWindow(window);
        window = nullptr;
        std::cout << "Window destroyed" << std::endl;
    }
    if (renderer != nullptr) {
        SDL_DestroyRenderer(renderer); 
        renderer = nullptr;
        std::cout << "Renderer destroyed" << std::endl;
    }
    
    SDL_Quit();
    std::cout << "SDL quit" << std::endl;
}

bool App::running() {
    return isRunning;
}