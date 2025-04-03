#include <iostream>     
#include <iomanip> 
#include "ParticleManager.h"



void ParticleManager::setNeighbors(LiquidParticle* _p, Real _multiplier) {
    
    for (auto& p : particles) {
        if (_p->checkNeighbors(*p, _multiplier)) {
            //p->color = sf::Color::Yellow;
            _p->addNeighbor(p);
            
        }
        

    }
    
}
void ParticleManager::findNeighbors(Real _multiplier) {
    for (auto& p : particles) {
        // p->color = p->defaultColor;
        if (dynamic_cast<LiquidParticle*> (p.get()) != nullptr) {
            LiquidParticle* k = dynamic_cast<LiquidParticle*> (p.get());
            k->neighborList.clear();

            setNeighbors(k, _multiplier);

            // std::cout << "Number of Neighbor Particles for particle at (" << k->position[0] << ", " << k->position[1] << "): " << k->neighborList.size() << "\n";
        }
        else {
            // std::cout << "Number of Neighbor Particles for boundary particle at (" << p.centerX << ", " << p.centerY << "): " << " is zero." << "\n";
        }
    }
}

void ParticleManager::calculateDensityAll() {
    for (auto& p : particles) {
        if (dynamic_cast<LiquidParticle*> (p.get()) != nullptr) {
            LiquidParticle* k = dynamic_cast<LiquidParticle*> (p.get());
            k->calculateDensity();
        }
    }
}

void ParticleManager::calculatePressureAll(Real k) {
    for (auto& p : particles) {
        if (dynamic_cast<LiquidParticle*> (p.get()) != nullptr) {
            LiquidParticle* ptr = dynamic_cast<LiquidParticle*> (p.get());
            ptr->calculatePressure(k);

        }
    }
}
void ParticleManager::calculatePressureAccelerationAll() {
    for (auto& p : particles) {
        if (dynamic_cast<LiquidParticle*> (p.get()) != nullptr) {
            LiquidParticle* ptr = dynamic_cast<LiquidParticle*> (p.get());
            ptr->calculatePressureAcceleration();
        }
    }
}

void ParticleManager::calculateViscosityAccelerationAll(Real mu) {
    for (auto& p : particles) {
        if (dynamic_cast<LiquidParticle*> (p.get()) != nullptr) {
            LiquidParticle* ptr = dynamic_cast<LiquidParticle*> (p.get());
            ptr->calculateViscosityAcceleration(mu);
        }
    }
}

void ParticleManager::calculateNonPressureAccelerationAll() {
    for (auto& p : particles) {
        if (dynamic_cast<LiquidParticle*> (p.get()) != nullptr) {
            LiquidParticle* ptr = dynamic_cast<LiquidParticle*> (p.get());
            ptr->calculateNonPressureAcceleration();
        }
    }
}

void ParticleManager::calculateTotalAccelerationAll() {
    for (auto& p : particles) {
        if (dynamic_cast<LiquidParticle*> (p.get()) != nullptr) {
            LiquidParticle* ptr = dynamic_cast<LiquidParticle*> (p.get());
            ptr->calculateTotalAcceleration();
        }
    }
}

void ParticleManager::calculateNextVelocityAll(Real timeStep) {
    for (auto& p : particles) {
        if (dynamic_cast<LiquidParticle*> (p.get()) != nullptr) {
            LiquidParticle* ptr = dynamic_cast<LiquidParticle*> (p.get());
            ptr->calculateNextVelocity(timeStep);
        }
        if (p->velocity.norm() > MAX_VELOCITY) {
            MAX_VELOCITY = p->velocity.norm();
        }
    }
}
void ParticleManager::calculateNextPositionAll(Real timeStep) {
    for (auto& p : particles) {
        if (dynamic_cast<LiquidParticle*> (p.get()) != nullptr) {
            LiquidParticle* ptr = dynamic_cast<LiquidParticle*> (p.get());
            ptr->calculateNextPosition(timeStep);
        }
    }
}


Real ParticleManager::calculateAverageDensity() {
    Real sum = 0.0;
    for (auto& p : particles) {
        if (dynamic_cast<LiquidParticle*> (p.get()) != nullptr) {
            LiquidParticle* ptr = dynamic_cast<LiquidParticle*> (p.get());
            sum += ptr->density;
        }
    }
    return sum / numOfLiquidParticles;
}

void ParticleManager::calculateNumOfParticles() {
    unsigned int n = 0;
    for (auto& p : particles) {
        if (dynamic_cast<LiquidParticle*> (p.get()) != nullptr) {
            n++;
        }
        
    }
    setNumOfParticles(n);
}


// Test Related 
void ParticleManager::allKernelTests() {
    findNeighbors(2.0);
    testKernel();
    testKernelGradientSum();
}
void ParticleManager::testKernel() {
    for (auto& p : particles) {
        Real sum = 0.0;
        if (dynamic_cast<LiquidParticle*> (p.get()) != nullptr) {
            LiquidParticle* k = dynamic_cast<LiquidParticle*> (p.get());
            for(auto& p1 : particles) {
                sum += k->calculateKernel(*p1);
                if (k->neighborList.size() > 12) {
                    std::cout << fixed << std::setprecision(numeric_limits<Real>::max_digits10)
                              << (k->position - p1->position).norm() << ", " 
                              << k->calculateKernel(*p1) << "; " << std::endl;
                }
            }

            if (abs(sum - 1 / k->volume) < EPS_ERROR) {
                std::cout << "No Problem";
            }
            else {
                Real dif = ROUND(sum) - ROUND(1 / k->volume);
                
                std::cout << "Particle at: " << k->position[0] << ", " << k->position[1] << "\n"
                    << "NumNeigh: " << k->neighborList.size() << "\n"
                    << "Sum: " << sum << "\n"
                    << "Vol: " << 1 / k->volume << "\n"
                    << "Dif: " << dif << "\n \n";
            }
            
            
        }

    }
    std::cout << "\n +++ End of Kernel Test +++\n";
}

void ParticleManager::testKernelGradientSum() {
    for (auto& p : particles) {
        Vector2r sum = { 0.0 , 0.0};
        if (dynamic_cast<LiquidParticle*> (p.get()) != nullptr) {
            LiquidParticle* k = dynamic_cast<LiquidParticle*> (p.get());
            for (auto& p1 : particles) {
                Vector2r s1 = k->calculateKernelGradient(*p1);
                sum[0] += s1[0];
                sum[1] += s1[1];
            }
            if (k->neighborList.size() > 12) {
                std::cout << "Sum: (" << sum[0] << ", " << sum[1] << ")\n";
            }
            

        }
    }
    std::cout << "End of Kernel Gradient Sum Test";
}


LiquidParticle* ParticleManager::findParticle(Vector2r point) {
    for (shared_ptr<Particle> p : particles) {
        if (dynamic_cast<LiquidParticle*> (p.get()) != nullptr) {
            LiquidParticle* ptr = dynamic_cast<LiquidParticle*> (p.get());
            if (ptr->isItOnMe(point)) {
                return ptr;
            }
        }
    }
    return nullptr;

}