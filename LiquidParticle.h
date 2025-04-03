#include <vector>
#include <SFML/Graphics.hpp>
#include "Particle.h"
#include "Common.h"
#include "TimeManager.h"

#pragma once

#ifndef _LIQUIDPARTICLE_H
#define _LIQUIDPARTICLE_H
const Vector2r GRAVITY = { 0.0, -9.81 };

class LiquidParticle : public Particle
{
public:
	// Attributes
	std::vector<std::shared_ptr<Particle>> neighborList;

	// Pressure Acceleration
	Vector2r pressureAcceleration;

	// Viscosity Acceleration
	Vector2r viscosityAcceleration;

	// Non-Pressure Acceleration
	Vector2r nonPressureAcceleration;

	//Constructor Destructor
	LiquidParticle(Real _restDensity, Real _radius, Vector2r _position);
	LiquidParticle(Real _restDensity, Real _radius, Vector2r _position, sf::Color _color);
	// ~LiquidParticle();

	// Copy Contructor
	LiquidParticle(const LiquidParticle& lp);

	void initParticle();
	// returns false if the particle is not within the support rad
	bool checkNeighbors(Particle p, Real supportRadius);
	void addNeighbor(shared_ptr<Particle> p);
	Real calculateKernel(Particle p);
	Vector2r calculateKernelGradient(Particle& p);

	/*Real calculateKernelPrev(Particle p);
	Vector2r calculateKernelGradientPrev(Particle& p);*/

	void calculateDensity();

	void calculatePressure(Real k);

	void calculatePressureAcceleration();

	void calculateViscosityAcceleration(Real mu);

	void calculateNonPressureAcceleration();

	void calculateTotalAcceleration();
	
	void calculateNextVelocity(Real timeStep);

	void calculateNextPosition(Real timeStep);

	bool isItOnMe(Vector2r point);

	/*void draw(sf::RenderWindow& window);

	void draw(sf::RenderWindow& window, sf::Color colour);*/

private:

};

#endif