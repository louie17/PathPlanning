#pragma once
#include"ASTAR.h"
#include<iostream>

#define PI 3.1415926
#define MAX_DIST 10000000
namespace AS {
	using namespace std;
	VAPointPtr Choose_Astar(const APoint&, const APoint&, const APoint&, const VRada&, const Mission_G&, const float& e_w1, const float& survice_w1, const float& start_w1, const float& end_w1, const float& horizontal_corner1, const float& verticality_corner1, const float& hmin1, const float& hmax1, const float& StepLength1);

	VAPointPtr searchFirstPoints(A_STAR& A_STAR);
	float Calc_dist(const APoint&, const APoint&);
	float Calc_dist(APointPtr, APointPtr);
	float getcostG(A_STAR& A_STAR);
	float getcostH(APointPtr p, A_STAR& A_STAR, const size_t& flag);
	float getcostheight(APointPtr p, float e_w);
	vector<float> getCostS(APointPtr Par_Pos, APointPtr lastPB, A_STAR& A_STAR);
	void saveresult(A_STAR& A_STAR);
	APointPtr buble_poits(VAPointPtr p);
	bool removeone(VAPointPtr&, APointPtr);
	VAPointPtr searchPoints(APointPtr lastPB, size_t parentIndex, APointPtr endpos, size_t flag, A_STAR& A_STAR);

}