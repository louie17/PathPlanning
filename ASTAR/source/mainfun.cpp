#include"mainAstar.h"
using namespace AS;


void main()
{
	Mission_G mg(1, 440, 370, 1, 0, 0);
	float survice_w1 = 1.0f;
	float start_w1 = 8000;
	float end_w1 = 10000;
	float e_w1 = 0.5;
	float hmin1 = 0.0f;
	float hmax1 = 3.0f;
	float horizontal_corner1 = 0.7854f;
	float verticality_corner1 = 0.5236f;
	float StepLength1 = 4.0f;
	Rada Rada1(1, 450, 350, 0, 100, 1);
	Rada Rada2(2, 450, 350, 0, 50, 1);
	Rada Rada3(3, 450, 350, 0, 70, 1);
	VRada Radav;
	Radav.push_back(Rada1);
	Radav.push_back(Rada2);
	Radav.push_back(Rada3);
	APoint sp(100, 100, 1, 0, 0, 0, 0, 0);
	APoint tp(440, 370, 1, 0, 0, 0, 0, 0);
	APoint ep(800, 500, 1, 0, 0, 0, 0, 0);
	auto route = Choose_Astar(sp, tp, ep, Radav, mg, e_w1, survice_w1, start_w1, end_w1, horizontal_corner1, verticality_corner1, hmin1, hmax1, StepLength1);

}
