#include "app.h"
#include "texturemanager.h"

SDL_Renderer* App::renderer = nullptr;
double App::dt = 0.01;
LagrangianDoublePendulum* Lpend1 = nullptr;
LagrangianDoublePendulum* Lpend2 = nullptr;
std::vector<Object*> Cpend1;
std::vector<Object*> Cpend2;
double d0 = -1;
double dprev = 0;
double lyapunovTot = 0;
double lyapunovAvg = 0;
int angle = 0;
double massRatio = 1;
long updates = 0;
Vector2d testvec(0,0);
Object* origin = nullptr;

int trials = 1;
int totalTrials = 180;

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

    avg = std::ofstream("averageexponents.csv");
    curr = std::ofstream("output.csv");
    avg << std::string("Mass Ratio , Average Lyapunov Exponent\n");
    
            /* Two Lagrangian Pendulum Setup */

    // Lpend1 = new LagrangianDoublePendulum();
    // Lpend2 = new LagrangianDoublePendulum();
    // Lpend1->setAngles(0, 0);
    // Lpend1->setMassRatio(massRatio);
    // Lpend2->setAngles(0, 0);
    // Lpend2->setMassRatio(massRatio + 0.01);

    system = new System();
    origin = new ImmovableObject();
    origin->pos = Vector2d(5,1);
    origin->mass = 0.1;

    
    // Constraint Pendulum 1

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
    Cpend1.push_back(pend1m1);
    Cpend1.push_back(pend1m2);

    // Constraint Pendulum 2
    
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
    Cpend2.push_back(pend2m1);
    Cpend2.push_back(pend2m2);

    system->setAngles(origin, Cpend1, 0, 0, 1, 1);
    system->setMassRatio(Cpend1, massRatio);
    system->setAngles(origin, Cpend2, 0, 0, 1, 1);
    system->setMassRatio(Cpend2, massRatio+0.01);


    for (auto& o: system->objects) {
        ForceGenerator* grav = new Gravity(o);
        system->addForceGenerator(grav);
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
    for (int i = 0; i < iterationsPerFrame; ++i) {
        step();
        if (i%250 == 0) {
            calculateLE();
        }
    }
    if (updates == 3000) {
        if (massRatio >= 2) {
            isPaused = true;
            std::cout << "DONE" << std::endl;
            isRunning = false;
        }
        else {
            angle += 1;
            std::cout << "Testing case " << angle << std::endl;
            avg << angle << ", " << lyapunovAvg << "\n";
            // Lpend1->clear();
            // Lpend2->clear();
            // Lpend1->setAngles(angle,0);
            // Lpend2->setAngles(angle,0);
            // Lpend1->setMassRatio(massRatio);
            // Lpend2->setMassRatio(massRatio + 0.01);

            system->clear(Cpend1);
            system->setAngles(origin, Cpend1, angle, 0, 1,1);
            system->setMassRatio(Cpend1, massRatio);
            
            system->clear(Cpend2);
            system->setAngles(origin, Cpend2, angle, 0, 1,1);
            system->setMassRatio(Cpend2, massRatio+0.01);
            
            d0 = -1;
            dprev = 0;
            lyapunovTot = 0;
            lyapunovAvg = 0;
            updates = 0;
            ++trials;
            render();
        }
    }

    // std::cout << system->calculateEnergy() << std::endl;
}

void App::calculateLE() {
    std::vector<double> state1 = system->getState(Cpend1);
    std::vector<double> state2 = system->getState(Cpend2);
    // std::vector<double> state1 = Lpend1->getState();
    // std::vector<double> state2 = Lpend2->getState();
    double temp = 0;
    for (unsigned int i = 0; i < state1.size(); ++i) {
        temp += pow(state1[i] - state2[i], 2);
    }
    temp = sqrt(temp);
    if (d0 == -1) {
        d0 = temp;
        dprev = temp;
        // curr << "Iterations, Lyapunov Exponent\n";
    }
    ++updates;
    double di = temp;
    double lyapunovCurr = log(di/d0);
    lyapunovTot += lyapunovCurr;
    lyapunovAvg = lyapunovTot / (updates);
    // curr << updates << ", ";
    // curr << lyapunovCurr << "\n";
    // std::cout << lyapunovAvg << std::endl;
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

    if (Lpend1 != nullptr) {
        Lpend1->update();
    }
    if (Lpend2 != nullptr) {
        Lpend2->update();
    }

    // std::cout << system->calculateEnergy() << std::endl;
}

void App::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    if (Lpend1 != nullptr) {
        Lpend1->render(renderer);
    }
    if (Lpend2 != nullptr) {
        Lpend2->render(renderer);
    }
    if (origin != nullptr) {
        origin->render(renderer);
    }
    system->render(renderer);
    SDL_RenderPresent(renderer);
}

void App::cleanup() {
    avg.close();
    curr.close();

    delete solver;
    system->cleanup();
    delete system;
    
    if (origin != nullptr) {
        delete origin;
    }

    if (Lpend1 != nullptr) {
        delete Lpend1;
    }

    if (Lpend2 != nullptr) {
        delete Lpend2;
    }

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