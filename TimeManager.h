#include "Common.h"
#pragma once

#ifndef _TIMEMANAGER_H
#define _TIMEMANAGER_H

class TimeManager {

public:
    Real courantFactor;
    Real particleSize;

    // Constructor
    TimeManager(Real courantFactor, Real particleSize)
        : courantFactor(courantFactor), particleSize(particleSize) {}

    Real calculateTimeStep(Real maxVelocity) const {
        if (maxVelocity <= 0) {
            throw std::invalid_argument("Maximum velocity must be greater than zero.");
        }
        return courantFactor * particleSize / (maxVelocity * sqrt(particleSize * 9.81) * 10);
    }



};


#endif // !_TIMEMANAGER_H
