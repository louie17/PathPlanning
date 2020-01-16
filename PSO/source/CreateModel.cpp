#include "CreateModel.h"
#define DIMENSION 3
using namespace std;
using namespace PSO;

VPoint init(const Point& start, const Point& end, const VRada &vrada, const double& xmin, const double& xmax, const double& ymin, const double& ymax,
		const double& zmin, const double& zmax,const size_t& dim,
		const size_t &MaxIt, const size_t& NP, const double& w, const double& damp,
		const double& c1, const double& c2,const double & alpha
		)
{

	Model PSO_Model(start,end,vrada,xmin,xmax,ymin,ymax,zmin,zmax, dim,MaxIt,NP,w,damp,c1,c2,alpha);
	Var PPos{xmin,xmax,ymin,ymax,zmin,zmax};
	Vel PVel{-alpha *(xmax-xmin),alpha * (xmax - xmin) ,-alpha * (ymax - ymin), alpha * (ymax - ymin) 
			- alpha * (zmax - zmin), alpha* (zmax - zmin) };
	Particle GlobalBest;
	GlobalBest.setcost(INFINITY);
	VParticlePtr vparticle ;
	create_pso(vparticle, PSO_Model, GlobalBest);
	PSORun(vparticle, PSO_Model, GlobalBest, PPos, PVel);
	writetext(GlobalBest);
	return GlobalBest.getvsol().route;
}
//vparticle 使用引用而不是值传递
void create_pso(VParticlePtr &vparticle, const Model& model, Particle& GlobalBest)
{
	for (size_t i = 0; i < model.NP; i++)
	{
		ParticlePtr particle = make_shared<Particle>();
		if (i > 0)
		{
			for (size_t j = 0; j < model.dim; j++)
			{
				Point p;
				double rdnum = rand()% size_t((model.xmax - model.xmin+1))+model.xmin ;
				p.setX(rdnum);
				 rdnum = rand() % size_t((model.ymax - model.ymin + 1)) + model.ymin;
				p.setY(rdnum);
				 rdnum = rand() % size_t((model.zmax - model.zmin + 1)) + model.zmin;
				p.setZ(rdnum);
				
				particle->getvpos().push_back(p);
				
			}
		}
		else
		{
			
			auto vec = linspace(model.getStart(), model.getEnd(), model.dim + 2);
			assert(vec.size() > 2);
			//初始化坐标
			for (size_t i = 1; i < vec.size()-1; i++)
			{
				particle->getvpos().push_back(vec[i]);
				

			}
		
		
	
		}
		for (size_t i = 0; i < model.dim; i++)
		{
			//初始化速度,默认构造函数为0
			Velocity vel;
			particle->getvvel().push_back(vel);
		}
		cost_fun(particle,model);
		particle->getvbestsol() = particle->getvsol();
		particle->setbestcost(particle->getcost());
		particle->getvbestpos() = particle->getvpos();
		if (particle->getbestcost() < GlobalBest.getcost())
		{
			GlobalBest.getvsol() = particle->getvbestsol();
			GlobalBest.getvpos() = particle->getvpos();
			GlobalBest.setcost(particle->getbestcost());
			
		}

		vparticle.push_back(particle);
	}


}

VPoint linspace(const Point& start, const Point& end,const size_t &n)
{
	VPoint vec;
	double d1 = (end.getPointX() - start.getPointX()) / (n - 1);
	double d2 = (end.getPointY() - start.getPointY()) / (n - 1);
	double d3 = (end.getPointZ() - start.getPointZ()) / (n - 1);
	for (size_t i = 0; i < n; i++)
	{
		Point p;
		p.setX(i * d1 + start.getPointX());
		p.setY(i * d2 + start.getPointY());
		p.setZ(i * d3 + start.getPointZ());
		vec.push_back(p);

	}
	return vec;
}

void cost_fun(ParticlePtr  particle, const Model& model)
{
	auto vpos = particle->getvpos();
	auto xs = model.getStart();
	auto ys = model.getEnd();
	auto vrada = model.getVRada();
	
	VPoint route = vpos;
	route.insert(route.begin(), xs);
	route.insert(route.end(), ys);

	size_t k = route.size();
	size_t t = 20;
	assert(route.size()>2);
	VPoint arraypoint;
	for (size_t i = 0; i < route.size()-1; i++)
	{

		VPoint vec=	linspace(route[i], route[i+1],t);
		arraypoint.insert(arraypoint.end(),vec.begin(),vec.end());

	}
	double length = 0;
	for (size_t i = 1; i < arraypoint.size() ; i++)
	{
		length += calc_dist(arraypoint[i-1], arraypoint[i]);
	}
	auto radav = model.getVRada();
	size_t nobs =radav.size();
	
	double violation = 0;
	for (size_t i = 0; i < nobs; i++)
	{
		double sum = 0;
		for (size_t j = 0; j < arraypoint.size(); j++)
		{
			double d = calc_dist(radav[i], arraypoint[j]);
			double v = max(1 - d / radav[i].getR(), 0.0);
			sum += v;
			
		}
		double meanv = sum / arraypoint.size();
		violation += meanv;

	}
	double wcost = 100;
	double cost = length * (1 + wcost * violation);
	particle->getvsol().route = arraypoint;
	particle->getvsol().length = length;
	particle->getvsol().violation = violation;
	particle->setcost(cost);

}
const double calc_dist(const Point& p1, const Point& p2)
{
	return sqrt(pow(p1.getPointX() - p2.getPointX(), 2) + pow(p1.getPointY() - p2.getPointY(), 2) +
		pow(p1.getPointZ() - p2.getPointZ(), 2));

}

void PSORun(VParticlePtr& vparticle,  Model& model, Particle &GlobalBest, const Var& boundpos, const Vel& boundvel)
{
	for (size_t it = 0; it < model.MaxIt; it++)
	{
		for (size_t j = 0; j < model.NP; j++)
		{
			//更新粒子的速度
			
		
			for (size_t k = 0; k < model.dim; k++)
			{
				//获得当前vx，vy,vz
				for (size_t axis = 0; axis < DIMENSION; axis++)
				{
					auto vel = vparticle[j]->getvvel();
					double v0 = vel[k].getPoint(axis);//粒子当前vx
					double rdnum = double(rand() % (1000 - 0 + 1)) / 1000.0;//获得0-1
					double rdnum1 = double(rand() % (1000 - 0 + 1)) / 1000.0;//获得0-1
					//更新速度
					double v1 = model.w * v0
						+ model.c1 * rdnum * (vparticle[j]->getvbestpos()[k].getPoint(axis) - vparticle[j]->getvpos()[k].getPoint(axis))
						+ model.c2 * rdnum1 * (GlobalBest.getvpos()[k].getPoint(axis) - vparticle[j]->getvpos()[k].getPoint(axis));
					vparticle[j]->getvvel()[k].set(v1,axis);
					//约束速度
					
					vparticle[j]->getvvel()[k].set(max(v1, boundvel.xmin),axis);
					vparticle[j]->getvvel()[k].set(min(v1, boundvel.xmax),axis);
					//更新坐标
					double pos = vparticle[j]->getvpos()[k].getPoint(axis);
					vparticle[j]->getvpos()[k].set( (v1 + pos),axis);

					//约束坐标
					double newx = vparticle[j]->getvpos()[k].getPoint(axis);
					vparticle[j]->getvpos()[k].set(max(newx, boundpos.xmin),axis);
					vparticle[j]->getvpos()[k].set(min(newx, boundpos.xmax),axis);
				}



			}
			cost_fun(vparticle[j],model);
			if (vparticle[j]->getcost() < vparticle[j]->getbestcost())
			{
				vparticle[j]->setbestcost(vparticle[j]->getcost());
				vparticle[j]->getvbestsol() = vparticle[j]->getvsol();
				vparticle[j]->getvbestpos() = vparticle[j]->getvpos();
				if (vparticle[j]->getbestcost() < GlobalBest.getcost())
				{
					GlobalBest.getvsol() = vparticle[j]->getvbestsol();
					GlobalBest.setcost(vparticle[j]->getbestcost());
					GlobalBest.setvpos(vparticle[j]->getvbestpos());
				}
			}
	



		}
		if (GlobalBest.getvsol().violation == 0)
		{
			cout << "ok" << endl;
		}
		cout << "it:" << it << " cost: " << GlobalBest.getcost() << endl;;
		model.w = model.w * model.damp;
	}

}

void writetext( Particle & GlobalBest)
{
	ofstream ofs;

	ofs.open("d:/result.txt",ios::out | ios::trunc);
	auto route = GlobalBest.getvsol().route;
	for (size_t i = 0; i < route.size(); i++)
	{
		
		ofs << route[i].getPointX() <<" "<< route[i].getPointY() <<" "<< route[i].getPointZ() << endl;;
		
		

	}
	ofs.close();

}

