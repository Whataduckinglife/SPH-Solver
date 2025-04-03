#define _USE_MATH_DEFINES
#include "BoundaryParticle.h"
#include "LiquidParticle.h"

LiquidParticle::LiquidParticle(Real _restDensity, Real _radius, Vector2r _position)
	: LiquidParticle(_restDensity, _radius, _position, sf::Color::Red) {}

LiquidParticle::LiquidParticle(Real _restDensity, Real _radius, Vector2r _position, sf::Color _color)
{
	restDensity = _restDensity;
	radius = _radius;
	position = _position;
	defaultColor = sf::Color::Red;
	color = _color;
	initParticle();
}

void LiquidParticle::initParticle() {
	particleSize = 2 * radius;
	volume = particleSize * particleSize;
	mass = restDensity * volume;
	/*!!!!!for (int i = 0; i < 2; i++) {
		velocity.push_back(0.0);
		acceleration.push_back(0.0);
		viscosityAcceleration.push_back(0.0);
		pressureAcceleration.push_back(0.0);
		nonPressureAcceleration.push_back(0.0);
	}*/
}

bool  LiquidParticle::checkNeighbors(Particle p, Real supportRadius) {
	Real distance = (p.position - position).norm();
	if (distance <= (particleSize * supportRadius) + EPS_ERROR) {
		return true;
	}
	return false;
}

void  LiquidParticle::addNeighbor(shared_ptr<Particle> p) {
	neighborList.push_back(p);
}


//Real LiquidParticle::calculateKernel(Particle p) {
//	Real alpha = 5.0 / (14 * ROUND(M_PI) * pow(particleSize, 2));
//	Real distance = (p.position - position).norm();
//	Real q = (distance / particleSize);
//	Real res = 0.0;
//	const Real q = r / m_radius;
//	if (q <= 1.0)
//	{
//		if (q <= 0.5)
//		{
//			const Real q2 = q * q;
//			const Real q3 = q2 * q;
//			res = m_k * (static_cast<Real>(6.0) * q3 - static_cast<Real>(6.0) * q2 + static_cast<Real>(1.0));
//		}
//		else
//		{
//			res = m_k * (static_cast<Real>(2.0) * pow(static_cast<Real>(1.0) - q, static_cast<Real>(3.0)));
//		}
//	}
//	return res;
//} 
//}

const static Real pi = static_cast<Real>(M_PI);
// WARNING: IF NOT PRECISE ENOUGH INCREASE PRECISON IN ROUND
Real  LiquidParticle::calculateKernel(Particle p) {
	Real alpha = 5.0 / (14 * pi * particleSize * particleSize);
	Real distance = (p.position - position).norm();
	Real q =  (distance / particleSize);

	if (q < 1.0) {
		Real q1 = (1.0 - q);
		Real q2 = (2.0 - q);
		return alpha * (q2 * q2 * q2 - 4.0 * q1 * q1 * q1);
	} 
	else if (q < 2.0) {
		Real q2 = (2.0 - q);
		return alpha * (q2 * q2 * q2);
	}
	return 0.0;
}

//Real LiquidParticle::calculateKernel(Particle p) {
//	//Real alpha = 10.0 / (7.0 * pi * pow(particleSize, 2));
//	Real r = (p.position - position).norm();
//	Real h = particleSize;
//	// if (r > h) return 0.0;
//
//	double q = r / h;
//	if (neighborList.size() > 12) {
//		std::cout << q << std::endl;
//	}
//	
//	double coefficient = 10.0 / (7.0 * M_PI * h * h);
//
//	if (q <= 1.0) {
//		return coefficient * (1.0 - 6.0 * q * q + 6.0 * q * q * q);
//	}
//	else if (q <= 2.0) {
//		return coefficient * 2.0 * std::pow(1.0 - q, 3);
//	}
//	else {
//		return 0.0;
//	}
//
//
//}

Vector2r  LiquidParticle::calculateKernelGradient(Particle& p) {
	Real distance = (p.position - position).norm();
	
	if (distance < EPSILON) {
		return { 0.0, 0.0 };
	}

	// Vector2r ret = { 0.0 , 0.0 };
	Real ret = 0.0;
	Real alpha = 5.0 / (14 * pi * pow(particleSize, 3));
	Real q = distance / particleSize;
	Vector2r x_ij = p.position - position;
	/*if (0 <= q && q < 1) {
		ret[0] = alpha * ( x_ij[0] / distance) * ((-3) * (pow(MAX((2 - q), 0.0), 2.0)) + 12 * pow(MAX((1 - q), 0.0), 2.0));
		ret[1] = alpha * ( x_ij[1] / distance) * ((-3) * (pow(MAX((2 - q), 0.0), 2.0)) + 12 * pow(MAX((1 - q), 0.0), 2.0));
	}
	else if (1 <= q && q < 2) {
		ret[0] = alpha * ((p.position[0] - position[0]) / distance) * ((-3) * (pow(MAX((2 - q), 0.0), 2.0)));
		ret[1] = alpha * ((p.position[1] - position[1]) / distance) * ((-3) * (pow(MAX((2 - q), 0.0), 2.0)));
	}
	else {
		return { 0.0, 0.0 };
	}*/
	if (0 <= q && q < 1) {
		Real q1 = (2 - q);
		Real q2 = (1 - q);
		ret = (alpha * (-3 * q1 * q1 + 12 * q2 * q2)) / (distance * particleSize);
	}
	else if (1 <= q && q < 2) {
		Real q1 = (2 - q);
		ret = (alpha * (-3 * q1 * q1)) / (distance * particleSize);
	}
	else {
		ret = 0.0;
	}

#ifdef _TEST_
	if (neighborList.size() > 12) {
		
	}
#endif // _TEST_
	return ret * x_ij;
}

void  LiquidParticle::calculateDensity() {
	Real sum = 0.0;
	for (std::shared_ptr<Particle> p : neighborList) {
		sum += p->mass * calculateKernel(*p);
	}
	density = sum;
}

// k : stiffness coefficient
void  LiquidParticle::calculatePressure(Real k) {  // It shouldnt spike as such!!! 
	Real p = k * ((density / restDensity) - 1);
	if (p < EPSILON) {
		pressure = 0;
	}
	else {
		pressure = p;
	}
}

void  LiquidParticle::calculatePressureAcceleration() {
	Vector2r sum = { 0.0, 0.0 };
	for (std::shared_ptr<Particle> p : neighborList) {
		Vector2r s1 = calculateKernelGradient(*p);
		Real d2 = density * density;
		if (auto liquidParticle = std::dynamic_pointer_cast<LiquidParticle>(p)) {
			// Liquid particle interaction			
			Real l2 = liquidParticle->density * liquidParticle->density;
			sum += liquidParticle->mass * ((pressure / d2) + (liquidParticle->pressure / l2)) * s1;

		}
		else if (auto boundaryParticle = std::dynamic_pointer_cast<BoundaryParticle>(p)) {
			Real b2 = boundaryParticle->density * boundaryParticle->density;
			Real boundaryContribution = boundaryParticle->mass * ((pressure / d2) + (pressure / b2));
			sum += boundaryContribution * s1;
		}
	}
	pressureAcceleration = sum;
}

void  LiquidParticle::calculateViscosityAcceleration(Real mu) {
	Vector2r sum = { 0.0, 0.0 };
	Vector2r kernelGradient;
	
	for (std::shared_ptr<Particle> p : neighborList) {
		kernelGradient = calculateKernelGradient(*p);
		Vector2r x_ij = position - p->position;
		Vector2r v_ij = velocity - p->velocity;
		Real upperDotProduct = v_ij.dot(x_ij);
		Real lowerDotProduct = x_ij.dot(x_ij);
		Real h2 = particleSize * particleSize;
		sum += (2 * mu) * ((p->mass / p->density) * (upperDotProduct / (lowerDotProduct + 0.01 * h2))) * kernelGradient;
	}

	viscosityAcceleration = sum;
}

void  LiquidParticle::calculateNonPressureAcceleration() {
	/*if (pressureAcceleration[0] <= 0) {
		viscosityAcceleration[0] *= -1;
	}

	if (pressureAcceleration[1] <= 0) {
		viscosityAcceleration[1] *= -1;
	}*/
	nonPressureAcceleration = { VSIGNUM(velocity[0]) * viscosityAcceleration[0] + GRAVITY[0], VSIGNUM(velocity[1]) * viscosityAcceleration[1] + GRAVITY[1]};
}

void  LiquidParticle::calculateTotalAcceleration() {
	acceleration = {nonPressureAcceleration[0] + pressureAcceleration[0], nonPressureAcceleration[1] + pressureAcceleration[1] };
	

}

void  LiquidParticle::calculateNextVelocity(Real timeStep) {
	velocity = { velocity[0] + timeStep * acceleration[0], velocity[1] + timeStep * acceleration[1] };

}

Real t = 0.0;
void  LiquidParticle::calculateNextPosition(Real timeStep) {
	position = { position[0] + timeStep * velocity[0], position[1] + timeStep * velocity[1] };
	
	t += timeStep;
#ifdef _DEBUG_
	std::cout << t << ";" <<
		density << ";" <<
		restDensity << ";" <<
		pressure << ";" <<
		pressureAcceleration[1] << ";" <<
		viscosityAcceleration[1] << ";" <<
		acceleration[0] << ";" << acceleration[1] << ";" <<
		velocity[0] << ";" << velocity[1] << ";" <<
		position[0] << ";" << position[1] << "\n";
#endif // _DEBUG_


}

bool LiquidParticle::isItOnMe(Vector2r point) {
	Real distance = (position - point).norm();
	if (distance <= radius) {
		return true;
	}
	return false;
	// return ((position - point).norm() <= radius ? true : false);
}