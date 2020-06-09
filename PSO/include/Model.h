#pragma once
#include<vector>
#include<memory>
#include<iostream>
using namespace std;
namespace PSO {

	class Point;
	class Point
	{
	public:
		Point();
		Point(const double&, const double&, const double&);
		void operator =(const Point& p);
		const double& getPointX() const;
		const double& getPointY() const;
		const double& getPointZ() const;
		const double& getPoint(const size_t& type) const;
		void setPoint(const Point&);
		void setX(const double &);
		void setY(const double&);
		void setZ(const double&);
		void set(const double&, const size_t &type);

	private:
		double x;
		double y;
		double z;

	};
	typedef shared_ptr<Point> PointPtr;
	typedef shared_ptr<PointPtr> VPointPtr;
	typedef vector<Point> VPoint;

	class Velocity : public Point
	{

	};
	typedef shared_ptr<Velocity> VelocityPtr;
	typedef shared_ptr<VelocityPtr> VVelocityPtr;
	typedef vector<Velocity> VVelocity;
	class Rada :public Point
	{
	public:
		const double& getR();
		void setR(const double&);
	private:

		double r;
	};
	typedef shared_ptr<Rada> RadaPtr;
	typedef shared_ptr<RadaPtr> VRadaPtr;
	typedef vector<Rada> VRada;


	class Solution
	{
	public:
		Solution();
		Solution(const VPoint&route, const double&violation, const double& length);
	public:
		VPoint route;
		double length;
		double violation;
	};

	class Model
	{
	public:
		Model();
		Model(const Point& start, const Point& end, const VRada vrada,
			const double& xmin, const double& xmax,
			const double& ymin, const double& ymax,
			const double& zmin, const double& zmax,
			const size_t& dim, const size_t MaxIt, const size_t& NP, const double& w, const double& damp,
			const double& c1, const double& c2, const double& alpha
		);
		const Point& getStart() const;
		const Point& getEnd()const;
		const VRada& getVRada()const;
	private:
		Point start;
		Point end;
		VRada vrada;
	public:
		double xmin;
		double xmax;
		double ymin;
		double ymax;
		double zmin;
		double zmax;
		size_t dim;
		size_t MaxIt;
		size_t NP;
		double w;
		double damp;
		double c1;
		double c2;
		double alpha;
	};

}