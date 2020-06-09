#pragma once

#include<vector>
#include<list>
#include<memory>
#include<stdexcept>
using namespace std;
namespace AS
{
	class APoint;
	class Rada;
	class Mission_G;
	typedef std::shared_ptr<APoint> APointPtr;
	typedef std::shared_ptr<Rada> RadaPtr;
	typedef std::vector<Rada> VRada;
	typedef std::vector<APointPtr> VAPointPtr;
	class APoint
	{
	public:
		APoint();
		APoint(const float &x, const float &y, const float &z, const float &g, const float &f,const size_t &t,const float &time,const float &s);
		bool operator ==(APointPtr p1);
		//~APoint();
	public:
		float X;
		float Y;
		float Z;
		float G;
		float F;
		float TIME_COST;
		float S_COST;
		size_t   TYPE;
	};

	class Rada
	{
	public:
		Rada();
		Rada(const size_t &type,const float &x,const float &y,const float &z,const float &r,const float &i);
		//~Rada();

	public:
		size_t type;
		float x;
		float y;
		float z;
		float r;
		float i;
	};

	class Mission_G
	{
	public:
		Mission_G();
		Mission_G(const size_t&,const float&,const float&,const float&,const float&,const float&);
		//~Mission_G();


	public:
		size_t type;
		float x;
		float y;
		float z;
		float step;
		float para;
	};

	class A_STAR
	{
	public:
		A_STAR(const APoint&, const APoint&, const APoint&, const VRada&,const Mission_G&,const float &e_w1,const float &survice_w1,const float &start_w1,const float &end_w1,const float &horizontal_corner1,const float &verticality_corner1,const float &hmin1,const float &hmax1,const float &StepLength1);

	public:
		float StepLength;
		VAPointPtr close;
		VAPointPtr open;
		VRada radas;
		VAPointPtr result_point;
		vector<pair<float, float>> result;
		vector<Mission_G> mission_g;
		Mission_G mg;
		APoint startP;
		APoint endP;
		APoint targetP;
		float e_w;
		float survice_w;
		float start_w;
		float end_w;
		float horizontal_corner;
		float verticality_corner;
		float h_min;
		float h_max;
		vector<vector<float>> zcubic;
		vector<float> width;
		vector<float> depth;
		VAPointPtr route;
	};
}
