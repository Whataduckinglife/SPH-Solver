#include "SPHLoop.h"


SPH::SPH(ScreenManager _screenManager, ParticleManager _particleManager) {
    particleManager = _particleManager;
    screenManager = _screenManager;
    
    //SPH_Init(1, 1);
}
void SPH::SPH_Init(int row, int col) {
    Real _radius = .2; // prev .575
    Real _restDensity = 100; // prev 115
    Real startX = - 5 * _radius;
    Real startY = _radius;
    int bc = 30;
    tsTotal = 0.0;
    closeFile();
    screenManager.initScreen();

#ifndef _TEST_
    unsigned int boundaryHeight = 30;
    // Add Boundary Particles- Left Wall
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < boundaryHeight; ++j) {
            Real x = startX + i * (2 * _radius + spacing);
            Real y = startY + j * (2 * _radius + spacing);
            BoundaryParticle* p = new BoundaryParticle(_restDensity, _radius, {x, y});
            particleManager.particles.push_back(((std::shared_ptr<Particle>) p));

        }
    }

    // Add Boundary Particles- Bottom Wall
    startX = _radius;
    startY = _radius;
    for (int i = 0; i < bc; ++i) {
        for (int j = 0; j < 3; ++j) {
            Real x = startX + i * (2 * _radius + spacing);
            Real y = startY + j * (2 * _radius + spacing);
            BoundaryParticle* p = new BoundaryParticle(_restDensity, _radius, { x, y });

            particleManager.particles.push_back(((std::shared_ptr<Particle>) p));

        }
    }
    
    // Add Boundary Particles - Right Wall
    startX = (1 + bc * 2) * _radius;
    startY = _radius;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < boundaryHeight; j++) {
            Real x = startX + i * (2 * _radius + spacing);
            Real y = startY + j * (2 * _radius + spacing);
            BoundaryParticle* p = new BoundaryParticle(_restDensity, _radius, { x, y });

            particleManager.particles.push_back(((std::shared_ptr<Particle>) p));
        }
    }

    //Add liquid Particles
    startX = 10.0 * _radius;
    startY = 16.0 * _radius;
   
#ifndef _BREAKING_DAM_   
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            Real x = startX + i * (2 * _radius + spacing);
            Real y = startY + j * (2 * _radius + spacing);
            LiquidParticle* p = new LiquidParticle(_restDensity, _radius, {x, y});

            particleManager.particles.push_back(((std::shared_ptr<Particle>) p));
            //std::cout << "Particle is at: " << p->centerX << ", " << p->centerY << "\n";
        }
    }
#else
    startX = _radius * 10.0;
    startY = _radius * 7;
    row = 60;
    col = 4;
    // Breaking Dam
    for (int i = 0; i < col; i++) {
        for (int j = 0; j < row; j++) {
            Real x = startX + i * (2 * _radius + spacing);
            Real y = startY + j * (2 * _radius + spacing);
            LiquidParticle* p = new LiquidParticle(_restDensity, _radius, { x, y });

            particleManager.particles.push_back(((std::shared_ptr<Particle>) p));
        }
    }
#endif // )BREAKING_DAM_

   
#else
     //Test Case
    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 20; ++j) {
            Real x = startX + i * (2 * _radius + spacing);
            Real y = startY + j * (2 * _radius + spacing);
            LiquidParticle* p = new LiquidParticle(_restDensity, _radius, {x, y});

            particleManager.particles.push_back(((std::shared_ptr<Particle>) p));
            //std::cout << "Particle is at: " << p->centerX << ", " << p->centerY << "\n";
        }
    }
#endif
    filename = "averageDensity_" + std::to_string(row) + "x" + std::to_string(col) + ".csv";
    particleManager.calculateNumOfParticles();
    openFile(filename);

}

void SPH::SPH_Calculations(Real k, Real mu, Real timeStep) {
	particleManager.findNeighbors(2.0);
	particleManager.calculateDensityAll();

    particleManager.calculatePressureAll(k);

    particleManager.calculatePressureAccelerationAll();
    particleManager.calculateViscosityAccelerationAll(mu);
    particleManager.calculateNonPressureAccelerationAll();
    particleManager.calculateTotalAccelerationAll();

    particleManager.calculateNextVelocityAll(timeStep);
    particleManager.calculateNextPositionAll(timeStep);
    particleManager.calculateAverageDensity();
    printToFile(std::to_string(tsTotal) + ";" + std::to_string(particleManager.calculateAverageDensity()));
    tsTotal += timeStep;
}



void SPH::openFile(std::string fl) {
    logFile.open(fl, std::ios::out | std::ios::trunc);
    
    // Check if the file is open
    if (!logFile.is_open()) {
        std::cerr << "Failed to open the log file." << std::endl;
        exit(1);
    }
}

void SPH::printToFile(std::string outputString) {
    if (!logFile.is_open()) {
        std::cerr << "File is not open for writing!" << std::endl;
        return;
    }
    logFile << outputString << std::endl;
    
}

void SPH::closeFile() {
    // Check if the file is open
    if (logFile.is_open()) {
        logFile.close();
    }
    // Close the file
    
}

void SPH::selectParticle(sf::Vector2i clickcoor) {
    //Vector2r click = screenManager.getAnalyticCoordinates(clickcoor);
    // LiquidParticle* p = particleManager.findParticle(click);
    Vector2r click = {clickcoor.x, clickcoor.y};
    Real scrRad = screenManager.getScreenLength(particleManager.particles[0]->radius);
    bool pclicked = false;
    for (auto& p : particleManager.particles) {
        if (dynamic_cast<LiquidParticle*> (p.get()) != nullptr) {
            LiquidParticle* ptr = dynamic_cast<LiquidParticle*> (p.get());
            std::vector<Real> scrcoor = screenManager.getScreenCoordinates(*ptr);
            Vector2r scrcoorvec = { scrcoor[0], scrcoor[1] };
            Real distance = (scrcoorvec - click).norm();
            if (distance <= scrRad) {
                pclicked = true;
                if (particleManager.selected != nullptr) {
                    particleManager.selected->color = particleManager.selected->defaultColor;
                }
                particleManager.selected = ptr;
                if (ptr != nullptr) {
                    ptr->color = sf::Color::Yellow;
                }
            }
        }
    }

    if (!pclicked) {
        if (particleManager.selected != nullptr) {
            particleManager.selected->color = particleManager.selected->defaultColor;
        }
        particleManager.selected = nullptr;
    }
    
    

    
    
    

}