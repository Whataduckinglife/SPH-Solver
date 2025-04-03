#include <vector>
#include <SFML/Graphics.hpp>
#include "Common.h"

#pragma once

#ifndef _PARTICLE_H
#define _PARTICLE_H

using namespace std;

class Particle
{
public:
	// Attributes
	Real restDensity;
	Real radius;
	Real particleSize;
	Real volume;
	Real mass;
	Vector2r position;
	
	Real density;

	Real pressure;
	Vector2r acceleration;
	Vector2r velocity;
	bool selected = false;

	~Particle() {}

	virtual void initParticle() {}

	////Drawing Related Attributes
	sf::Color color;
	sf::Color defaultColor;

	//Vector2r drawPosition;
	//Real drawRadius;
	//void draw(sf::RenderWindow& window, sf::Color cl) {
	//	sf::CircleShape circle(drawRadius);
	//	color = colour;
	//	circle.setFillColor(color);
	//	// Set position with spacing between circles
	//	circle.setPosition(drawX, drawY);
	//	window.draw(circle);
	//}
	//void draw(sf::RenderWindow& window) {
	//	draw(window, color);
	//}
	
	

};

#endif