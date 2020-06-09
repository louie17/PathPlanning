#pragma once
#include"Model.h"
#include"Particle.h"
#include<assert.h>
#include<algorithm>
#include<fstream>
using namespace PSO;
typedef struct
{
	double xmin;
	double xmax;
	double ymin;
	double ymax;
	double zmin;
	double zmax;
}Var, Vel;

VPoint init(const Point& start, const Point& end, const VRada& vrada, const double& xmin, const double& xmax, const double& ymin, const double& ymax,
	const double& zmin, const double& zmax, const size_t& dim,
	const size_t& MaxIt, const size_t& NP, const double& w, const double& damp,
	const double& c1, const double& c2, const double& alpha
);

void create_pso(VParticlePtr &vparticle,const Model& model, Particle& GlobalBest);
VPoint linspace(const Point&, const Point&,const size_t& );
void cost_fun(ParticlePtr particle,const Model& model);
const double calc_dist(const Point&, const Point&);
void PSORun(VParticlePtr& vparticle, Model& model, Particle &GlobalBest,const Var &boundpos,const Vel& boundvel);
void writetext(  Particle& GlobalBest);
