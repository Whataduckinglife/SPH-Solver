#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "particle.h"


#pragma once
#ifndef _BOUNDARYPARTICLE_H
#define _BOUNDARYPARTICLE_H

class BoundaryParticle : public Particle {

public:
	BoundaryParticle(Real _restDensity, Real _radius, Vector2r _position);
	BoundaryParticle(Real _restDensity, Real _radius, Vector2r _position, sf::Color _color);

	void initParticle();

};

#endif