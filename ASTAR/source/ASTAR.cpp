#include "ASTAR.h"

AS::A_STAR::A_STAR(const APoint& sp, const APoint& tp, const APoint& ep, const VRada& radav, const Mission_G& mg1, const float& e_w1, const float& survice_w1, const float& start_w1, const float& end_w1, const float& horizontal_corner1, const float& verticality_corner1, const float& hmin1, const float& hmax1, const float& StepLength1)
	:startP(sp), targetP(tp), endP(ep), radas(radav), mg(mg1), e_w(e_w1), survice_w(survice_w1),
	start_w(start_w1), end_w(end_w1), horizontal_corner(horizontal_corner1),
	verticality_corner(verticality_corner1), h_min(hmin1), h_max(hmax1), StepLength(StepLength1)
{
}

AS::APoint::APoint()
	:X(0), Y(0), Z(0), G(0), F(0), TIME_COST(0), S_COST(0), TYPE(0)
{
}
AS::APoint::APoint(const float& x, const float& y, const float& z, const float& g, const float& f, const size_t& t, const float& time, const float& s)
	: X(x), Y(y), Z(z), G(g), F(f), TIME_COST(time), S_COST(s), TYPE(t)
{
}
bool AS::APoint::operator==(APointPtr p1)
{
	if (X == p1->X && Y == p1->Y && Z == p1->Y)
	{
		return true;
	}

	return false;
}
AS::Rada::Rada()
	: type(0), x(0), y(0), z(0), r(0), i(0) {}

AS::Rada::Rada(const size_t& type, const float& x, const float& y, const float& z, const float& r, const float& i)
	: type(type), x(x), y(y), z(z), r(r), i(i) {}

AS::Mission_G::Mission_G() : type(0), x(0), y(0), z(0), step(0), para(0)
{
}

AS::Mission_G::Mission_G(const size_t& t, const float& x, const float& y, const float& z, const float& step, const float& para)
	: type(t), x(x), y(y), z(z), step(step), para(para)
{
}

