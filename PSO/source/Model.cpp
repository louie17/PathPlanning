#include "Model.h"
using namespace PSO;
Point::Point():x(0),y(0),z(0)
{
}



Point::Point(const double& x1, const double& y1, const double& z1) : x(x1), y(y1), z(z1)
{
}

void Point::operator=(const Point& p)
{
	x = p.x;
	y = p.y;
	z = p.z;
}

const double& Point::getPointX() const
{
	// TODO: 在此处插入 return 语句
	return x;
}

const double& Point::getPointY() const
{
	// TODO: 在此处插入 return 语句
	return y;
}

const double& Point::getPointZ() const
{
	// TODO: 在此处插入 return 语句
	return z;
}

const double& Point::getPoint(const size_t& type) const
{
	// TODO: 在此处插入 return 语句
	if (type == 0)
	{
		return getPointX();
	}
	else if (type == 1)
	{
		return getPointY();
	}
	else
	{
		return getPointZ();
	}
}

void Point::setPoint(const Point& p)
{
	x = p.x;
	y = p.y;
	z = p.z;
}



void Point::setX(const double& posx)
{
	x = posx;
}

void Point::setY(const double& posy)
{
	y = posy;
}

void Point::setZ(const double& posz)
{
	z = posz;
}

void Point::set(const double& para, const size_t& type)
{
	if (type == 0)
	{
		setX(para);
	}
	else if (type == 1)
	{
		setY(para);
	}
	else 
	{
		setZ(para);
	}

}


const double& Rada::getR()
{
	// TODO: 在此处插入 return 语句
	return r;
}

void Rada::setR(const double& r1 )
{
	r = r1;
}

Model::Model()
{
}

Model::Model(const Point& start, const Point& end, const VRada vrada, const double& xmin, const double& xmax, const double& ymin, const double& ymax, const double& zmin, const double& zmax, const size_t& dim, const size_t MaxIt, const size_t& NP, const double& w, const double& damp, const double& c1, const double& c2, const double& alpha)
	:start(start), end(end), vrada(vrada), xmin(xmin), xmax(xmax), ymin(ymin), ymax(ymax), dim(dim),
	MaxIt(MaxIt), NP(NP), w(w), damp(damp), c1(c1), c2(c2), alpha(alpha), zmin(zmin), zmax(zmax)
{
}




const Point& Model::getStart() const
{
	// TODO: 在此处插入 return 语句
	return start;
}

const Point& Model::getEnd() const
{
	// TODO: 在此处插入 return 语句
	return end;
}

const VRada& Model::getVRada() const
{
	// TODO: 在此处插入 return 语句
	return vrada;
}

Solution::Solution()
{
}

Solution::Solution(const VPoint& route, const double& violation, const double& length):
	route(route),violation(violation),length(length)
{

}

