#include <SFML/Graphics.hpp>
#include "BoundaryParticle.h"
#include "SPHLoop.h"
#include "ScreenManager.h"


extern const unsigned int SCR_WIDTH;
extern const unsigned int SCR_HEIGHT;

// Adjacent particles stick together for some reason
// Measure average density correctly
// Measure Density with different stiffness constants
// bigger k -> smaller density deviation
// Relative error should be around 1%
// Height is more important for breaking dam. 
// Average density of the whole particle approaches ->restDensity as k -> 0
// cite as many as possible
// Monaghan sph papers
// cite the surveys
// give frames
// Introduction -> Navier Stokes -> SPH Cpncept -> How frluid simulation step looks like -> then individual aspects -> analysis: you show what you are doing: density etc over time, average density over time average density changes due to k -> 

Real MAX_VELOCITY = 0.0;

void showInfo(ParticleManager* pm, Real ts, Real m, Real stiffk) {
    std::cout << "Average Density: " << pm->calculateAverageDensity() << std::endl;
    std::cout << /*"# of BPs: " << pm->numOfBoundaryParticles <<*/ ",  # of LPs : " << pm->numOfLiquidParticles << std::endl;
    std::cout << "TimeStep = " << ts << ", Mu = " << m << " ,k = " << stiffk << std::endl;
}

Real timeElapsed = 0;

int main() {
	sf::RenderWindow window(sf::VideoMode(SCR_WIDTH, SCR_HEIGHT), "Particles");
    TimeManager tm(0.1, 1.0);
    ScreenManager scrmgr(&window);
	ParticleManager prtmgr;
	SPH sph(scrmgr, prtmgr);
	Real timeStep = 0.005; // stable-ish version t = 0.005-0.0025, mu = 3, k = 200000
	Real mu = 1.5;
	Real k = 150000;
	int row = 1;
	int col = 1;
	sph.SPH_Init(row, col);
    bool selectionEnabled = false;
    while (window.isOpen()) {
        sf::Event event;
        // for simple adjustment of the vaariables use the keyboard. switch case etc
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                case sf::Keyboard::T:
                    std::cout << "Current timeStep: " << timeStep << ". Enter new time Step: ";
                    std::cin >> timeStep;
                    break;
                case sf::Keyboard::M:
                    std::cout << "Current viscosity constant: " << mu << ". Enter new mu: ";
                    std::cin >> mu;
                    break;
                case sf::Keyboard::K:
                    std::cout << "Current stiffness constant: " << k << ". Enter new k: ";
                    std::cin >> k;
                    break;
                case sf::Keyboard::R:
                    std::cout << "Reset Particles \n";
                    sph.particleManager.particles.clear();
                    sph.SPH_Init(row, col);
                    break;
                case sf::Keyboard::E:
                    std::cout << "Edit Particles \n";
                    std::cout << "Enter Row Count: ";

                    std::cin >> row;
                    std::cout << "Enter Col Count: ";

                    std::cin >> col;
                    sph.particleManager.particles.clear();
                    sph.SPH_Init(row, col);
                    break;
                case sf::Keyboard::Q:
                    sph.closeFile();
                    window.close();
                    break;
                case sf::Keyboard::I:
                    showInfo(&sph.particleManager, timeStep, mu, k);
                    break;
                case sf::Keyboard::S:
                    selectionEnabled = !selectionEnabled;
                    break;
                }
                
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (selectionEnabled) {
                    if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        sf::Vector2i mousePosition = { event.mouseButton.x, event.mouseButton.y };
                        sph.selectParticle(mousePosition);
                    }
                }
                
            }
            if (event.type == sf::Event::Closed)
                
                window.close();
        }
        window.clear();
        sph.screenManager.updateScreen(sph.particleManager.particles);
#ifndef _TEST_
        sph.SPH_Calculations(k, mu, timeStep);
#else 
        sph.particleManager.allKernelTests();
        window.close();
#endif

#ifdef _VAR_TIMESTEP_
        timeStep = tm.calculateTimeStep(MAX_VELOCITY);
#endif // _VAR_TIMESTEP_
        //std::cout << "Solver Step \n";
        if (sph.particleManager.selected != nullptr) {
            sph.screenManager.printToHeader("VisAcc: (" 
                            + std::to_string(sph.particleManager.selected->viscosityAcceleration[0])
                            + ", " 
                            + std::to_string(sph.particleManager.selected->viscosityAcceleration[1])
                            + ") ");
        }
        window.display();
        timeElapsed += timeStep;
        if (timeElapsed > 16.025000) {
            sph.closeFile();
            window.close();
        }
    }
    sph.closeFile();
	return 0;
}