#include "BoundaryParticle.h"

BoundaryParticle::BoundaryParticle(Real _restDensity, Real _radius, Vector2r _position)
	: BoundaryParticle(_restDensity, _radius, _position, sf::Color::Blue) {}

BoundaryParticle::BoundaryParticle(Real _restDensity, Real _radius, Vector2r _position, sf::Color _color) {
	restDensity = _restDensity;
	radius = _radius;
	position = _position;
	defaultColor = sf::Color::Blue;
	color = _color;
	initParticle();
}

void BoundaryParticle::initParticle() {
	density = restDensity;
	particleSize = 2 * radius;
	volume = particleSize * particleSize;
	mass = restDensity * volume;
	velocity = { 0.0, 0.0 };
	acceleration = { 0.0, 0.0 };
}