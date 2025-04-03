#include <fstream>
#include "ParticleManager.h"
#include "ScreenManager.h"

#pragma once




class SPH {
public:
	ScreenManager screenManager;

	ParticleManager particleManager;

	std::ofstream logFile;

	Real tsTotal;
	std::string filename;

	SPH(ScreenManager _screenManager, ParticleManager _particleManager);

	void SPH_Calculations(Real k, Real mu, Real timeStep);

	void SPH_Init(int row, int col);
	// File Related
	void openFile(std::string fileName);
	void printToFile(std::string outputString);
	void closeFile();

	void selectParticle(sf::Vector2i clickcoor);

private:
		const Real spacing = EPSILON;

};
