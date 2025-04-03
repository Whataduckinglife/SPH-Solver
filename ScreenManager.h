#include <SFML/Graphics.hpp>
#include "LiquidParticle.h"
#include "Particle.h"
#include "BoundaryParticle.h"
#include "ParticleManager.h"

#pragma once

const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;

class ScreenManager {
public:
	// Attributes:
	// vector<Particle> particles;
	
	sf::RenderWindow* window;
	sf::Font font;
	double zoomScale = 50.0; // prev 50
	ScreenManager() = default;
	ScreenManager(sf::RenderWindow* _window) {
		window = _window;
	}

	ScreenManager(const ScreenManager& sm) {
		window = sm.window;
	}

	vector<double> getScreenCoordinates(Particle p);
	double getScreenLength(double len);
	Vector2r getAnalyticCoordinates(sf::Vector2i scrcoor);
	

	void initScreen();
	void updateScreen(vector<std::shared_ptr<Particle>> _particles);

	void printToHeader(std::string message);
};