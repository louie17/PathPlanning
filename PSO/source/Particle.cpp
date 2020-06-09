#include "Particle.h"
using namespace PSO;
Particle::Particle():vpos(),vvel(), solution(),cost(0),best_cost(0),best_sol()
{
}

Particle::Particle(const VPoint& vpos, const VVelocity& vvel, const Solution& sol, const double& cost, const double& best_cost, const Solution& best_sol,const VPoint& best_pos)
	: vpos(vpos), vvel(vvel), solution(sol), cost(cost),best_cost(best_cost),best_sol(best_sol),best_pos(best_pos)
{
}



void Particle::setcost(const double& t_cost)
{
	cost = t_cost;
}

void Particle::setvpos(const VPoint& vec)
{
	for (size_t i = 0; i < vec.size(); i++)
	{
		vpos[i] = vec[i];
	}
}

void Particle::setvvel(const VVelocity& vec)
{
	for (size_t i = 0; i < vec.size(); i++)
	{
		vvel[i] = vec[i];
	}
}

void Particle::setsol(const Solution& sol)
{
	solution = sol;
}

void Particle::setbestcost(const double& cost)
{
	best_cost = cost;
}

void Particle::setbestpos(const VPoint& vp)
{
	best_pos = vp;
}

void Particle::replacepoint(const Point& p,size_t pos)
{
	for (size_t i = 0; i < vpos.size(); i++)
	{
		if (pos == i)
		{
			vpos[i] = p;
		}
	}
}

const double& Particle::getcost()
{
	// TODO: �ڴ˴����� return ���
	return cost;
}

const double& Particle::getbestcost()
{
	// TODO: �ڴ˴����� return ���
	return best_cost;
}

 VPoint& Particle::getvpos()
{
	// TODO: �ڴ˴����� return ���
	return vpos;
}

 VPoint& Particle::getvbestpos()
 {
	 // TODO: �ڴ˴����� return ���
	 return best_pos;
 }

 VVelocity& Particle::getvvel()
{
	// TODO: �ڴ˴����� return ���
	return vvel;
}

 Solution& Particle::getvsol()
{
	// TODO: �ڴ˴����� return ���
	return solution;
}

 Solution& Particle::getvbestsol()
 {
	 // TODO: �ڴ˴����� return ���
	 return best_sol;
 }


