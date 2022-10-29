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

    Lpendulum = new LagrangianDoublePendulum();

    // Test Code
    system = new System();
    origin = new ImmovableObject();
    origin->pos = Vector2d(5,1);
    origin->mass = 0.1;

    Object* object2 = new Object();
    object2->pos = Vector2d(4,1);
    object2->vel = Vector2d(0,0);
    object2->mass = 0.5;
    system->addObject(object2);
    

    Object* object3 = new Object();
    object3->pos = Vector2d(3,1);
    object3->mass = 0.5;
    system->addObject(object3);

    Object* object4 = new Object();
    object4->pos = Vector2d(2,1);
    object4->mass = 0.5;
    system->addObject(object4);

    Constraint* distconstraint1 = new DistanceConstraintFixed(object2, origin->pos);
    Constraint* distconstraint2 = new DistanceConstraint(object2, object3);
    Constraint* distconstraint3 = new DistanceConstraint(object4, object3);
    system->addConstraint(distconstraint1);
    system->addConstraint(distconstraint2);
    system->addConstraint(distconstraint3);

    for (auto& o: system->objects) {
        ForceGenerator* grav = new Gravity(o);
        system->generators.push_back(grav);
    }

    // ForceGenerator* s12 = new Spring(object1, object2, 500, 200);
    // ForceGenerator* s23 = new Spring(object2, object3, 1000, 2);

    // ForceGenerator* s12 = new ConstRadiusFixedPivot(control, origin);
    // ForceGenerator* s23 = new ConstRadius(object3, object2);
    // ForceGenerator* s32 = new ConstRadius(object2, object3);

    // system->generators.push_back(s34);
    // system->generators.push_back(s23);
    // system->generators.push_back(s32);
    // system->generators.push_back(s12);

    solver = new EulerSolver();
    solver->start(system, 0.0001/5);

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

    std::cout << system->calculateEnergy() << std::endl;
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

    Lpendulum->update();
}

void App::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    if (Lpendulum != nullptr) Lpendulum->render(renderer);
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