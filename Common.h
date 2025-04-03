#define _USE_MATH_DEFINES
#include <cmath>
#include <limits>
#include <Eigen/Dense>

#pragma once


#ifndef _COMMON_H
#define _COMMON_H

// #define _TEST_
// #define _DEBUG_
// #define _VAR_TIMESTEP_
#define _BREAKING_DAM_


const unsigned int PRECISION = 5;


#define MAX(a, b) (( a > b ? a : b))

#define ROUND(x) (std::ceil(x * std::pow(10.0, PRECISION)) / std::pow(10.0, PRECISION))

#define VSIGNUM(x) ((x >= 0) ? -1 : -1)

#ifdef USE_DOUBLE
typedef double Real;

#define REAL_MAX DBL_MAX
#define REAL_MIN DBL_MIN
#define RealParameter DoubleParameter
#define RealParameterType ParameterBase::DOUBLE
#define RealVectorParameter DoubleVectorParameter
#define RealVectorParameterType ParameterBase::VEC_DOUBLE
// double eps = 1.0e-5;
#else
typedef float Real;

#define REAL_MAX FLT_MAX
#define REAL_MIN FLT_MIN
#define RealParameter FloatParameter
#define RealParameterType ParameterBase::FLOAT
#define RealVectorParameter FloatVectorParameter
#define RealVectorParameterType ParameterBase::VEC_FLOAT
// float eps = 1.0e-4f;
#endif

const Real EPS_ERROR = 1.0e-5;





static constexpr double EPSILON = std::numeric_limits<double>::epsilon();

using Vector2r = Eigen::Matrix<Real, 2, 1, Eigen::DontAlign>;
using Vector3r = Eigen::Matrix<Real, 3, 1, Eigen::DontAlign>;
using Vector4r = Eigen::Matrix<Real, 4, 1, Eigen::DontAlign>;
using Vector5r = Eigen::Matrix<Real, 5, 1, Eigen::DontAlign>;
using Vector6r = Eigen::Matrix<Real, 6, 1, Eigen::DontAlign>;
using Matrix2r = Eigen::Matrix<Real, 2, 2, Eigen::DontAlign>;
using Matrix3r = Eigen::Matrix<Real, 3, 3, Eigen::DontAlign>;
using Matrix4r = Eigen::Matrix<Real, 4, 4, Eigen::DontAlign>;
using Matrix5r = Eigen::Matrix<Real, 5, 5, Eigen::DontAlign>;
using Matrix6r = Eigen::Matrix<Real, 6, 6, Eigen::DontAlign>;
using Vector2i = Eigen::Matrix<int, 2, 1, Eigen::DontAlign>;
using Vector3f = Eigen::Matrix<float, 3, 1, Eigen::DontAlign>;
using Vector4f = Eigen::Matrix<float, 4, 1, Eigen::DontAlign>;
using Matrix3f = Eigen::Matrix<float, 3, 3, Eigen::DontAlign>;
using AlignedBox2r = Eigen::AlignedBox<Real, 2>;
using AlignedBox3r = Eigen::AlignedBox<Real, 3>;
using AngleAxisr = Eigen::AngleAxis<Real>;
using Quaternionr = Eigen::Quaternion<Real, Eigen::DontAlign>;
using VectorXr = Eigen::Matrix<Real, -1, 1, 0, -1, 1>;
using MatrixXr = Eigen::Matrix<Real, -1, -1, 0, -1, -1>;

#endif