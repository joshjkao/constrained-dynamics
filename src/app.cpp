#include "app.h"
#include "texturemanager.h"
#include <string>

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
Object* origin1 = nullptr;
Object* origin2 = nullptr;
Object* drag = nullptr;

TTF_Font* font = nullptr;
SDL_Color White = {255,255,255};
SDL_Surface* surfaceMessage = nullptr;
SDL_Texture* message = nullptr;
SDL_Rect messageRect;

SDL_Surface* energySurface = nullptr;
SDL_Texture* energyTexture = nullptr;
SDL_Rect energyRect;

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
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "SDL_Init failed with error: " << SDL_GetError() << std::endl;
        return false;
    }
    if (TTF_Init() < 0) {
        std::cout << "SDL_ttf failed with error: " << SDL_GetError() << std::endl;
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
    avg << std::string("Angle , Average Lyapunov Exponent\n");

    font = TTF_OpenFont("assets/font.ttf", 24);
    if (font) {
        std::cout << "Font created\n";
    }
    else {
        std::cout << TTF_GetError() << std::endl;
    }
    surfaceMessage = TTF_RenderText_Solid(font, "energy: ", White);
    message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    SDL_FreeSurface(surfaceMessage);
    messageRect.x = 10;
    messageRect.y = 32*18;
    messageRect.w = 100;
    messageRect.h = 32;

    energyRect.x = 112;
    energyRect.y = 32*18;
    energyRect.w = 100;
    energyRect.h = 32;

    return systemSetup();
}

bool App::systemSetup() {
            /* Two Lagrangian Pendulum Setup */

    Lpend1 = new LagrangianDoublePendulum();
    Lpend2 = new LagrangianDoublePendulum();
    Lpend1->setAngles(90, 0);
    Lpend1->setColor(54, 68, 115,255);
    Lpend1->setMassRatio(1);
    Lpend2->setAngles(90.003, 0);
    Lpend2->setMassRatio(1);
    Lpend2->setColor(95,120,200,255);

    system = new System();
    origin1 = new ImmovableObject();
    origin1->pos = Vector2d(6.5,1.5);
    origin1->mass = 0.1;

    origin2 = new ImmovableObject();
    origin2->pos = Vector2d(3.5,1.5);
    origin2->mass = 0.1;
    
    // Constraint Pendulum 1

    Object* pend1m1 = new Object();
    pend1m1->pos = Vector2d(7.5,1.5);
    pend1m1->mass = 1;
    system->addObject(pend1m1);

    Object* pend1m2 = new Object();
    pend1m2->pos = Vector2d(7.5,2.5);
    pend1m2->mass = 1;
    system->addObject(pend1m2);

    Constraint* distconstraint1 = new DistanceConstraintFixed(pend1m1, origin1->pos);
    Constraint* distconstraint2 = new DistanceConstraint(pend1m1, pend1m2);
    system->addConstraint(distconstraint1);
    system->addConstraint(distconstraint2);
    Cpend1.push_back(pend1m1);
    Cpend1.push_back(pend1m2);

    // Mass for Triple Pendulum 1
    if (runTriple) {
        Object* pend1m3 = new Object();
        pend1m3->pos = Vector2d(7.5,3.5);
        pend1m3->mass = 1;
        system->addObject(pend1m3);
        Constraint* distconstraint5 = new DistanceConstraint(pend1m2, pend1m3);
        system->addConstraint(distconstraint5);
        Cpend1.push_back(pend1m3);
    }

    // // Constraint Pendulum 2
    
    // Object* pend2m1 = new Object();
    // pend2m1->pos = Vector2d(4.5,1);
    // pend2m1->mass = 0.5;
    // system->addObject(pend2m1);

    // Object* pend2m2 = new Object();
    // pend2m2->pos = Vector2d(5.5,1);
    // pend2m2->mass = 0.5;
    // system->addObject(pend2m2);

    // Constraint* distconstraint3 = new DistanceConstraintFixed(pend2m1, origin2->pos);
    // Constraint* distconstraint4 = new DistanceConstraint(pend2m1, pend2m2);
    // system->addConstraint(distconstraint3);
    // system->addConstraint(distconstraint4);
    // Cpend2.push_back(pend2m1);
    // Cpend2.push_back(pend2m2);

    // // Mass for Triple Pendulum 2
    // Object* pend2m3 = new Object();
    // pend2m3->pos = Vector2d(2,1);
    // pend2m3->mass = 0.5;
    // system->addObject(pend2m3);
    // Constraint* distconstraint6 = new DistanceConstraint(pend2m2, pend2m3);
    // system->addConstraint(distconstraint6);
    // Cpend2.push_back(pend2m3);

    // Object* raindrop = new ImmovableObject();
    // raindrop->mass = 0.000034;
    // raindrop->pos = Vector2d(1609,0);
    // ForceGenerator* grav1 = new GravAttract(raindrop, pend2m1);
    // ForceGenerator* grav2 = new GravAttract(raindrop, pend2m2);
    // system->addForceGenerator(grav1);
    // system->addForceGenerator(grav2);

    // system->setAngles(origin1, Cpend1, 0, 0, 1, 1);
    // system->setMassRatio(Cpend1, 1);
    // system->setColor(Cpend1, 99, 22, 23, 255);
    // system->setAngles(origin, Cpend2, 90.003, 0, 1, 1);
    // system->setMassRatio(Cpend2, 1);
    system->setColor(Cpend1, 191, 29, 31, 255);

    for (auto& o: system->objects) {
        ForceGenerator* grav = new Gravity(o);
        system->addForceGenerator(grav);
    }

    solver = new EulerSolver();
    solver->start(system, 1.0/24000);

    system->dump(std::cout);

    return true;
}

void App::handleEvents() {
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_MOUSEBUTTONDOWN: {
                SDL_GetMouseState(&mousex, &mousey);
                drag = system->checkMouseCollision(mousex, mousey);
                break;
            }
            case SDL_MOUSEBUTTONUP: {
                drag = nullptr;
                break;
            }
            case SDL_KEYDOWN: {
                isRunning = event.key.keysym.scancode != SDL_SCANCODE_ESCAPE;
                if (event.key.keysym.scancode == SDL_SCANCODE_T) {
                    clearSystem();
                    runTriple = !runTriple;
                    systemSetup();
                    isPaused = true;
                }
                else if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
                    isPaused = !isPaused;
                    if (isPaused) {
                        system->dump(std::cout);
                    }
                }
                else if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
                    if (!isPaused) break;
                    step();
                }
                else if (event.key.keysym.scancode == SDL_SCANCODE_R) {
                    clearSystem();
                    systemSetup();
                    isPaused = true;
                }
                break;
            }
            case SDL_QUIT: {
                isRunning = false;
                break;
            }
            case SDL_KEYUP: {
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
        if (i%200 == 0) {
            // calculateLE();
        }
    }
    /*
    if (updates == 2500) {
        if (angle >= 180) {
            avg << angle << ", " << lyapunovAvg << "\n";
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
            // system->setMassRatio(Cpend1, massRatio);
            
            system->clear(Cpend2);
            system->setAngles(origin, Cpend2, angle+0.003, 0, 1,1);
            // system->setMassRatio(Cpend2, massRatio);
            
            d0 = -1;
            dprev = 0;
            lyapunovTot = 0;
            lyapunovAvg = 0;
            updates = 0;
            ++trials;
            render();
        }
    }
    */

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
        // curr << "Iterations, LE\n";
    }
    ++updates;
    double di = temp;
    double lyapunovCurr = log(di/d0);
    lyapunovTot += lyapunovCurr;
    lyapunovAvg = lyapunovTot / (updates);
    curr << updates << ", ";
    curr << lyapunovCurr << "\n";
    // std::cout << d0 << std::endl;
}

void App::step() {
    // Clear before every iteration
    system->clearForces();

    // Apply F external (e.g. gravity)
    for (auto& g: system->generators) {
        g->update();
    }

    // Apply any extra forces here
    if (drag) {
        SDL_GetMouseState(&mousex, &mousey);
        Vector2d dragToMouse(mousex - drag->pos.x*100, mousey - drag->pos.y*100);
        double rDiff = mag(dragToMouse);
        double magnitude = rDiff * 0.5; // Damping factor to stop system from exploding
        Vector2d AF = hat(dragToMouse) * magnitude;
        drag->applyForce(AF);
    }

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
    if (drag) {
        int x0 = drag->pos.x*100, x1 = mousex;
        int y0 = drag->pos.y*100, y1 = mousey;
        int dx =  abs(x1-x0), sx = x0<x1 ? 1 : -1;
        int dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1;
        int err = dx+dy, e2;
        int count = 0;
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        while (1) {
            if (count < 10) {SDL_RenderDrawPoint(renderer,x0,y0);}
            if (x0==x1 && y0==y1) break;
            e2 = 2*err;
            if (e2 > dy) { err += dy; x0 += sx; }
            if (e2 < dx) { err += dx; y0 += sy; }
            count = (count + 1) % 20;
        }
    }
    if (Lpend1 != nullptr) {
        Lpend1->render(renderer);
    }
    if (Lpend2 != nullptr) {
        Lpend2->render(renderer);
    }
    if (origin1 != nullptr) {
        origin1->render(renderer);
    }
    if (origin2 != nullptr) {
        origin2->render(renderer);
    }
    SDL_RenderCopy(renderer, message, NULL, &messageRect);
    energySurface = TTF_RenderText_Solid(font, std::to_string(system->calculateEnergy()).substr(0,7).c_str(), White);
    if (surfaceMessage) {
        energyTexture = SDL_CreateTextureFromSurface(renderer, energySurface);
        SDL_FreeSurface(energySurface);
        SDL_RenderCopy(renderer, energyTexture, NULL, &energyRect);
        SDL_DestroyTexture(energyTexture);
    }
    else {
        std::cout << "failed to render text\n";
    }
    system->render(renderer);
    SDL_RenderPresent(renderer);
}

void App::clearSystem() {
    delete solver;
    system->cleanup();
    delete system;
    
    if (origin1 != nullptr) {
        delete origin1;
    }

    if (origin2 != nullptr) {
        delete origin2;
    }

    if (Lpend1 != nullptr) {
        delete Lpend1;
    }

    if (Lpend2 != nullptr) {
        delete Lpend2;
    }
}

void App::cleanup() {
    avg.close();
    curr.close();

    delete solver;
    system->cleanup();
    delete system;
    
    if (origin1 != nullptr) {
        delete origin1;
    }
    if (origin2 != nullptr) {
        delete origin2;
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