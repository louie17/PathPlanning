#include "APoint.h"



APoint::APoint() :X(0), Y(0), Z(0), G(0), F(0), TIME_COST(0), S_COST(0), TYPE(0)
{


}

 APoint::APoint(float x, float y, float z, float g, float f, int t, float time, float s):X(x),Y(y),Z(z),G(g),F(f),TIME_COST(time),S_COST(s),TYPE(t)
 {}


 APoint::~APoint()
{
}
