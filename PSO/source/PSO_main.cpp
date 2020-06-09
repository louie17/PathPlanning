#include"CreateModel.h"
using namespace PSO;
 int main()
{
	Point start(20,20,0);
	Point end(180, 180, 0);
	Rada r1, r2, r3;
	r1.setPoint(Point(50, 120, 0));
	r1.setR(10);


	r2.setPoint(Point(80 ,80, 0));
	r2.setR(15);
	r3.setPoint(Point(150, 150, 0));
	r3.setR(20);
	VRada vrada;
	vrada.push_back(r1);
	vrada.push_back(r2);
	vrada.push_back(r3);
	double xmin = 0;
	double xmax = 200;
	double ymin = 0;
	double ymax = 200;
	double zmin = 0;
	double zmax = 200;
	size_t dim = 3;
	double w = 1;
	double damp = 0.98;
	double c1 = 1.5;
	double c2 = 1.5;
	double alpha = 0.1;
	size_t MaxIt = 100;
	size_t NP = 15;
	auto route = init( start,  end,  vrada, xmin, xmax,  ymin, ymax,zmin,zmax,  dim,
		MaxIt,  NP,  w, damp,c1,  c2,  alpha
	);
	
	return 0;
}



