#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <memory>
#include <vector>
#include <math.h>
#include "Particle.h"
#include "LiquidParticle.h"
#include "TimeManager.h"

#pragma once
#ifndef _PARTICLEMANAGER_H
#define _PARTICLEMANAGER_H


extern Real MAX_VELOCITY;

class ParticleManager {
public:
    // List of all particles

    unsigned int numOfLiquidParticles = 0;
    unsigned int numOfBoundaryParticles = 0;
    std::vector<std::shared_ptr<Particle>> particles;
    LiquidParticle* selected;

    // To make adaptive timestep
    //TimeManager timeManager;

    void setNumOfParticles(unsigned int num) {
        numOfLiquidParticles = num;
        numOfBoundaryParticles = particles.size() - num;
    }

    void calculateNumOfParticles();

    void setNeighbors(LiquidParticle* _particle, Real _multiplier);
    void findNeighbors(Real _multiplier);

    void calculateDensityAll();
    
    void calculatePressureAll(Real k);
    void calculatePressureAccelerationAll();
    
    void calculateViscosityAccelerationAll(Real mu);
    
    void calculateNonPressureAccelerationAll();
    
    void calculateTotalAccelerationAll();

    void calculateNextVelocityAll(Real timeStep);
    void calculateNextPositionAll(Real timeStep);

    Real calculateAverageDensity();

    vector<std::shared_ptr<Particle>> getParticles() {
        return particles;
    }


    // Test Related 
    void allKernelTests();
    void testKernel();
    void testKernelGradientSum();
    void testKernelGradientCross();

    // Drawing Related
    LiquidParticle* findParticle(Vector2r point);
    
};

#endif