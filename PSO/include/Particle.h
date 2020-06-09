#pragma once
#include "Model.h"
namespace PSO
{
class Particle;
class Particle
{
public:
	Particle();
	Particle(const VPoint& vpos,const VVelocity& vvel, const Solution& sol,const double &cost,const double& best_cost,const Solution& best_sol,const VPoint& best_pos);
	void setcost(const double &);
	void setvpos(const VPoint&);
	void setvvel(const VVelocity&);
	void setsol(const Solution&);
	void setbestcost(const double&);
	void setbestpos(const VPoint&);
	void replacepoint(const Point&p,size_t pos);
	const double &getcost();
	const double& getbestcost();
	 VPoint& getvpos();
	 VPoint& getvbestpos();
	 VVelocity& getvvel();
	 Solution& getvsol();
	 Solution& getvbestsol();

private:
	double cost;

	VPoint vpos;
	VVelocity vvel;
	Solution solution;
	VPoint best_pos;
	double best_cost;
	Solution best_sol;


};
typedef shared_ptr<Particle> ParticlePtr;
typedef vector<ParticlePtr> VParticlePtr;
typedef vector<Particle> VParticle;
}