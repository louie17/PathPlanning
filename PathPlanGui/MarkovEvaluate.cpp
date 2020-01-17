#include "MarkovEvaluate.h"

MatrixXd markov_init(size_t timestep, sce::Route_ptr route, sce::Site_WeaponRange_relation swRelation, std::map<string, double> CofRada)
{
	size_t timeStep = timestep;

	size_t nWayPoints = route->getAllWayPoints().size();

	//Postion of route
	MatrixXd posRoute(3, nWayPoints);
	size_t index = 0;
	auto wpoints = route->getAllWayPoints();
	for (size_t i = 0; i < nWayPoints; i++)
	{

		posRoute(index++) = wpoints[i].getLongitude();
		posRoute(index++) = wpoints[i].getLatitude();
		posRoute(index++) = wpoints[i].getAltitude();
	}

	size_t nWeapon = swRelation.size();
	MatrixXd posSensor(3, nWeapon), posWeapon(3, nWeapon);
	MatrixXd rangeSensor(1, nWeapon), rangeWeapon(1, nWeapon);

	index = 0;

	for (auto& x : swRelation)
	{
		rangeSensor(index / 3) = x.second;
		posSensor(index++) = x.first->getLongitude();
		posSensor(index++) = x.first->getLatitude();
		posSensor(index++) = x.first->getAltitude();

	}
	rangeWeapon = rangeSensor;
	posWeapon = posSensor;
	//index = 0;
	//for (auto &x : swRelation)
	//{
	//	rangeWeapon(index) = x.second;
	//	posWeapon(index++) = x.first->getLongitude();
	//	posWeapon(index++) = x.first->getLatitude();
	//	posWeapon(index++) = x.first->getAltitude();

	//}


	double Lud = CofRada["Lud_o"];
	double Ldu = CofRada["Ldu_o"];
	double Ldt = CofRada["Ldt_o"];
	double Ltd = CofRada["Ltd_o"];
	double Lte = CofRada["Lte_o"];
	double Let = CofRada["Let_o"];
	double Leh = CofRada["Leh_o"];
	MatrixXd outsideTransitionIntensity(5, 5);
	outsideTransitionIntensity <<
		-Lud, Lud, 0.0, 0.0, 0.0,
		Ldu, -(Ldu + Ldt), Ldt, 0.0, 0.0,
		0.0, Ltd, -(Ltd + Lte), Lte, 0.0,
		0.0, 0.0, Let, -(Let + Leh), Leh,
		0.0, 0.0, 0.0, 0.0, 0.0;
	//cout << outsideTransitionIntensity << endl;

	//TransitionIntensity sensorTransitionIntensity[10];
	std::vector<TransitionIntensity> sensorTransitionIntensity(nWeapon);

	Lud = CofRada["Lud_r"];
	Ldu = CofRada["Ldu_r"];
	Ldt = CofRada["Ldt_r"];
	Ltd = CofRada["Ltd_r"];
	Lte = CofRada["Lte_r"];
	Let = CofRada["Let_r"];
	Leh = CofRada["Leh_r"];

	for (int i = 0; i < nWeapon; i++)
	{
		sensorTransitionIntensity[i].weapon.setIdentity(5, 5);

		sensorTransitionIntensity[i].weapon <<
			-Lud, Lud, 0.0, 0.0, 0.0,
			Ldu, -(Ldu + Ldt), Ldt, 0.0, 0.0,
			0.0, Ltd, -(Ltd + Lte), Lte, 0.0,
			0.0, 0.0, Let, -(Let + Leh), Leh,
			0.0, 0.0, 0.0, 0.0, 0.0;

		//-0.4, 0.4, 0.0, 0.0, 0.0,
		//0.1, -0.4, 0.3, 0.0, 0.0,
		//0.0, 0.1, -0.1, 0.0, 0.0,
		//0.0, 0.0, 1.0, -1.0, 0,
		//0.0, 0.0, 0.0, 0.0, 0;
	}

	//1 - (Ldu), Lud, 0.0, 0.0, 0.0,
	//	Ldu, 1 - Lud - Ltd, Ldt, 0.0, 0.0,
	//	0.0, Ltd, 1 - Lte - Ldt, Lte, 0.0,
	//	0.0, 0.0, Let, 1 - Lte, Leh,
	//	0.0, 0.0, 0.0, 0.0, 1;


	//for (int i = 0; i < 3; i++)
	//{
	//	cout << sensorTransitionIntensity[i].weapon<<endl;
	//}
	Lud = CofRada["Lud_w"];
	Ldu = CofRada["Ldu_w"];
	Ldt = CofRada["Ldt_w"];
	Ltd = CofRada["Ltd_w"];
	Lte = CofRada["Lte_w"];
	Let = CofRada["Let_w"];
	Leh = CofRada["Leh_w"];
	std::vector<TransitionIntensity> weaponTransitionIntensity(nWeapon);

	for (int i = 0; i < nWeapon; i++)
	{
		weaponTransitionIntensity[i].weapon.setIdentity(5, 5);
		weaponTransitionIntensity[i].weapon <<
			-Lud, Lud, 0.0, 0.0, 0.0,
			Ldu, -(Ldu + Ldt), Ldt, 0.0, 0.0,
			0.0, Ltd, -(Ltd + Lte), Lte, 0.0,
			0.0, 0.0, Let, -(Let + Leh), Leh,
			0.0, 0.0, 0.0, 0.0, 0.0;

	}

	MatrixXd stateCost(1, 5);
	stateCost <<
		0, 1, 10, 100, 0;

	MatrixXd transitionCost(5, 5);
	transitionCost <<
		0.0, 0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0, 1000.0,
		0.0, 0.0, 0.0, 0.0, 0.0;


	return	reRouteEvaluation(posRoute, posSensor, posWeapon, rangeSensor, rangeWeapon,
		outsideTransitionIntensity, sensorTransitionIntensity, weaponTransitionIntensity, stateCost, transitionCost, timeStep);
}

ProbBack reRecursionSolver(MatrixXd previousStateProbability, double previousExpectedCost, MatrixXd previousTransitionIntensity, MatrixXd stateCost, MatrixXd transitionCost, int deltaT)
{
	MatrixXd X_previous(1, 6);
	X_previous << previousStateProbability, previousExpectedCost;
	//cout << X_previous << endl;
	double XiU_previous, XiD_previous, XiT_previous, XiE_previous, XiH_previous;
	XiU_previous = stateCost(0) + previousTransitionIntensity.row(0) * transitionCost.row(0).transpose() - previousTransitionIntensity(0, 0) * transitionCost(0, 0);
	XiD_previous = stateCost(1) + previousTransitionIntensity.row(1) * transitionCost.row(1).transpose() - previousTransitionIntensity(1, 1) * transitionCost(1, 1);
	XiT_previous = stateCost(2) + previousTransitionIntensity.row(2) * transitionCost.row(2).transpose() - previousTransitionIntensity(2, 2) * transitionCost(2, 2);
	XiE_previous = stateCost(3) + previousTransitionIntensity.row(3) * transitionCost.row(3).transpose() - previousTransitionIntensity(3, 3) * transitionCost(3, 3);
	XiH_previous = stateCost(4) + previousTransitionIntensity.row(4) * transitionCost.row(4).transpose() - previousTransitionIntensity(4, 4) * transitionCost(4, 4);

	MatrixXd	B_previous(5, 6), A_previous(6, 6);
	B_previous << previousTransitionIntensity.transpose(), MatrixXd::Zero(5, 1);
	//cout << B_previous<<endl;
	A_previous << B_previous, XiU_previous, XiD_previous, XiT_previous, XiE_previous, XiH_previous, 0;
	//cout << A_previous<<endl;
	MatrixXd expA_previous(6, 6);
	expA_previous = MatrixXd::Zero(6, 6);


	//cout << "a*a" << endl;
	//cout << A_previous*A_previous/1 << endl;

	for (int ii = 0; ii < 16; ii++)
	{
		MatrixXd mat = A_previous;
		double result = 1;
		if (ii > 0)
		{
			for (int jj = 1; jj <= ii; jj++)
			{
				result = result * jj;
			}

			for (int kk = 1; kk < ii; kk++)
			{
				//	cout << "mat:" << mat<<endl;
				mat = mat * A_previous;
				//	cout << "mat:" << mat<<endl;
			}
			//cout << "mat:" << endl;
			//cout << mat << endl;
			expA_previous = expA_previous + (mat) / result;
		}
		else
		{
			expA_previous = MatrixXd::Identity(6, 6);
		}

		//cout << expA_previous << endl;
	}
	//cout << "expA_previous:" << endl;
	//	cout << expA_previous<<endl;
	X_previous = expA_previous * X_previous.transpose();
	MatrixXd currentStateProbability = X_previous.topRows(5);
	double currentExpectedCost = X_previous(5);
	MatrixXd global_stateProbability;
	double global_expectedCost;
	global_stateProbability = currentStateProbability;
	global_expectedCost = currentExpectedCost;

	ProbBack prob;
	prob.prob = global_stateProbability;
	prob.cost = global_expectedCost;

	return prob;

}

MatrixXd reRouteEvaluation(MatrixXd posRouteInterp, MatrixXd posSensor, MatrixXd posWeapon, MatrixXd rangeSensor, MatrixXd rangeWeapon, MatrixXd outsideTransitionIntensity, std::vector<TransitionIntensity> sensorTransitionIntensity,
	std::vector<TransitionIntensity> weaponTransitionIntensity, MatrixXd stateCost, MatrixXd transitionCost, int timeStep)
{
	int nWayPoint = posRouteInterp.cols();
	int nSensor = posSensor.cols();
	int nWeapon = posWeapon.cols();

	MatrixXd transitionIntensity(5, 5);

	MatrixXd stateProbabilityProcess = MatrixXd::Zero(5, nWayPoint);
	MatrixXd expectedCostProcess = MatrixXd::Zero(1, nWayPoint);

	MatrixXd stateProbability(1, 5);
	stateProbability << 1, 0, 0, 0, 0;

	int expectedCost = 0;

	for (int ii = 0; ii < nWayPoint; ii++)
	{
		//% Calculate Transision Intensity Matrix for the way point
		//	% Check if platform is inside sensor operation range

		MatrixXd flagInsideSensor = MatrixXd::Zero(nSensor, 1);
		for (int jj = 0; jj < nSensor; jj++)
		{
			//cout << posRouteInterp.col(ii);
			double distPlatformSensor = (posRouteInterp.col(ii) - posSensor.col(jj)).norm();
			if (distPlatformSensor < rangeSensor(jj))
			{
				flagInsideSensor(jj) = 1;
			}
		}
		//% Check if platform is inside weapon operation range
		MatrixXd flagInsideWeapon = MatrixXd::Zero(nWeapon, 1);

		for (int jj = 0; jj < nWeapon; jj++)
		{
			double distPlatformWeapon = (posRouteInterp.col(ii) - posWeapon.col(jj)).norm();

			if (distPlatformWeapon < rangeWeapon(jj))
			{
				flagInsideWeapon(jj) = 1;
			}

		}
		//  % Platform is outside sensor and weapon operation range
		if (flagInsideWeapon.sum() == 0 && flagInsideSensor.sum() == 0)
		{
			transitionIntensity = outsideTransitionIntensity;
		}
		/*	% Platform is inside weapon operation range
		% For weapons whose operation range is within sensor operation range,
		% if the platform is inside weapon operation range, it is not
		% considered in the sensor operation range.
		*/
		else if (flagInsideWeapon.sum() > 0)
		{
			/*find flagInsideWeapon coeff >0 */
			//VectorXd insideWeapon(flagInsideWeapon.sum());
			vector<int> insideWeapon;
			for (int k = 0; k < flagInsideWeapon.size(); k++)
			{
				if (flagInsideWeapon(k) > 0)
					insideWeapon.push_back(k);
			}
			//% Overlapping weapon area
			if (insideWeapon.size() > 1)
			{
				int cnt = int(insideWeapon[0]);
				double lambdaUD, lambdaDT, lambdaDU, lambdaTD, lambdaTE, lambdaET, lambdaEH;
				lambdaUD = weaponTransitionIntensity[cnt].weapon(0, 1);
				lambdaDT = weaponTransitionIntensity[cnt].weapon(1, 2);
				lambdaDU = weaponTransitionIntensity[cnt].weapon(1, 0);
				lambdaTD = weaponTransitionIntensity[cnt].weapon(2, 1);
				lambdaTE = weaponTransitionIntensity[cnt].weapon(2, 3);
				lambdaET = weaponTransitionIntensity[cnt].weapon(3, 2);
				lambdaEH = weaponTransitionIntensity[cnt].weapon(3, 4);

				for (int jj = 1; jj < insideWeapon.size(); jj++)
				{
					int cnt = insideWeapon[jj];
					lambdaUD = lambdaUD + weaponTransitionIntensity[cnt].weapon(0, 1);
					lambdaDT = lambdaDT + weaponTransitionIntensity[cnt].weapon(1, 2);
					lambdaDU = 1 / (1 / lambdaDU + 1 / weaponTransitionIntensity[cnt].weapon(1, 0) - 1 / (lambdaDU + weaponTransitionIntensity[cnt].weapon(1, 0)));
					lambdaTD = 1 / (1 / lambdaTD + 1 / weaponTransitionIntensity[cnt].weapon(2, 1) - 1 / (lambdaTD + weaponTransitionIntensity[cnt].weapon(2, 1)));
					lambdaTE = max(lambdaTE, weaponTransitionIntensity[cnt].weapon(2, 3));
					lambdaET = min(lambdaET, weaponTransitionIntensity[cnt].weapon(2, 3));
					lambdaEH = max(lambdaEH, weaponTransitionIntensity[cnt].weapon(3, 4));
				}
				transitionIntensity.setIdentity(5, 5);
				transitionIntensity <<
					-lambdaUD, lambdaUD, 0, 0, 0,
					lambdaDU, -(lambdaDU + lambdaDT), lambdaDT, 0, 0,
					0, lambdaTD, -(lambdaTD + lambdaTE), lambdaTE, 0,
					0, 0, lambdaET, -(lambdaET + lambdaEH), lambdaEH,
					0, 0, 0, 0, 0;

			}
			// % Single weapon area
			transitionIntensity = weaponTransitionIntensity[int(insideWeapon[0])].weapon;
		}
		//% Platform is inside sensor operation range
		else
		{
			/*find flagInsideWeapon coeff >0 */
			//VectorXd insideSensor(flagInsideSensor.sum());
			vector<int> insideSensor;
			for (int k = 0; k < flagInsideSensor.size(); k++)
			{
				if (flagInsideSensor(k) > 0)
					insideSensor.push_back(k);
			}
			//% Overlapping weapon area
			if (insideSensor.size() > 1)
			{
				int cnt = int(insideSensor[0]);
				double lambdaUD, lambdaDT, lambdaDU, lambdaTD, lambdaTE, lambdaET, lambdaEH;
				lambdaUD = sensorTransitionIntensity[cnt].weapon(0, 1);
				lambdaDT = sensorTransitionIntensity[cnt].weapon(1, 2);
				lambdaDU = sensorTransitionIntensity[cnt].weapon(1, 0);
				lambdaTD = sensorTransitionIntensity[cnt].weapon(2, 1);
				lambdaTE = sensorTransitionIntensity[cnt].weapon(2, 3);
				lambdaET = sensorTransitionIntensity[cnt].weapon(3, 2);
				lambdaEH = sensorTransitionIntensity[cnt].weapon(3, 4);

				for (int jj = 1; jj < insideSensor.size(); jj++)
				{
					int cnt = insideSensor[jj];
					lambdaUD = lambdaUD + sensorTransitionIntensity[cnt].weapon(0, 1);
					lambdaDT = lambdaDT + sensorTransitionIntensity[cnt].weapon(1, 2);
					lambdaDU = 1 / (1 / lambdaDU + 1 / sensorTransitionIntensity[cnt].weapon(1, 0) - 1 / (lambdaDU + sensorTransitionIntensity[cnt].weapon(1, 0)));
					lambdaTD = 1 / (1 / lambdaTD + 1 / sensorTransitionIntensity[cnt].weapon(2, 1) - 1 / (lambdaTD + sensorTransitionIntensity[cnt].weapon(2, 1)));
					lambdaTE = max(lambdaTE, sensorTransitionIntensity[cnt].weapon(2, 3));
					lambdaET = min(lambdaET, sensorTransitionIntensity[cnt].weapon(2, 3));
					lambdaEH = max(lambdaEH, sensorTransitionIntensity[cnt].weapon(3, 4));
				}

				transitionIntensity.setIdentity(5, 5);
				transitionIntensity <<
					-lambdaUD, lambdaUD, 0, 0, 0,
					lambdaDU, -(lambdaDU + lambdaDT), lambdaDT, 0, 0,
					0, lambdaTD, -(lambdaTD + lambdaTE), lambdaTE, 0,
					0, 0, lambdaET, -(lambdaET + lambdaEH), lambdaEH,
					0, 0, 0, 0, 0;

			}
			else
			{
				transitionIntensity = sensorTransitionIntensity[int(insideSensor[0])].weapon;
			}

		}
		//cout << transitionIntensity << endl;
		// % Calculate state probability and expected cost of current way point
		ProbBack& Probability = reRecursionSolver(stateProbability, expectedCost, transitionIntensity, stateCost, transitionCost, timeStep);

		stateProbability = Probability.prob.transpose();
		expectedCost = Probability.cost;
		stateProbabilityProcess.col(ii) = Probability.prob;
		expectedCostProcess(ii) = Probability.cost;
		//cout << endl;
		cout << "state_probability:" << endl << stateProbability << endl;
		cout << "i:" << ii << "expect_cost:" << Probability.cost << endl;

	}
	return stateProbabilityProcess;
}

