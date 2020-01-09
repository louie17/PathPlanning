#pragma once
#include <Eigen/Eigen>
#include <vector>
#include <iostream>
#include "Scenario.hpp"
using namespace std;
using namespace Eigen;

struct TransitionIntensity
{
	MatrixXd  weapon;
};
struct ProbBack {
	MatrixXd prob;
	double cost;

};

MatrixXd reRouteEvaluation(MatrixXd posRouteInterp, MatrixXd posSensor,
	MatrixXd posWeapon, MatrixXd rangeSensor, MatrixXd rangeWeapon,
	MatrixXd outsideTransitionIntensity, std::vector<TransitionIntensity> sensorTransitionIntensity,
	std::vector<TransitionIntensity> weaponTransitionIntensity, MatrixXd stateCost,
	MatrixXd transitionCost, int timeStep);
ProbBack reRecursionSolver(MatrixXd previousStateProbability, double previousExpectedCost,
	MatrixXd previousTransitionIntensity, MatrixXd stateCost, MatrixXd transitionCost, int deltaT);



MatrixXd markov_init(size_t timestep, sce::Route_ptr route, sce::Site_WeaponRange_relation swRelation, std::map<string, double> CofRada);
