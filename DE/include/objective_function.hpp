#ifndef DE_OBJECTIVE_FUNCTION_HPP_INCLUDED
#define DE_OBJECTIVE_FUNCTION_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <memory>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <iterator>
#include <valarray>

#include "de_types.hpp"
#include "processors.hpp"
#include "de_constraints.hpp"
#include "Scenario.hpp"

#define PI 3.14159265

/**
* 具体目标函数的抽象基类。
*
* @author louiehan (11/18/2019)
*/
class objective_function
{
private:
	const std::string m_name;
public:
	/**
	* constructs an objective_function object
	*
	* @author louiehan (11/18/2019)
	*
	* @param name the objective function name
	*/
	objective_function(const std::string& name)
		: m_name(name)
	{
	}

	virtual ~objective_function() {}

	/**
	* 在派生类中实现-它包含要优化的目标函数。
	*
	* 目标函数接受一个double类型的参数向量并返回计算出的double值。
	*
	* args向量中的每个索引对应于约束向量中的相同索引。例如，如果目标函数需要两个变量，
	* 则将约束向量的前两个元素设置为两个变量的约束，并在目标函数operator（）中使用
	* args向量的前两个值作为两个变量。可以忽略此向量中的所有其他值。
	*
	* @author louiehan (11/18/2019)
	*
	* @param args 参数向量。向量通常比目标函数使用的变量数大得多，
	*			  因此目标函数of只取向量中的前n个值，而忽略其余的值。
	*
	* @return double 函数代价，这是需要优化的值。
	*/
	//virtual double operator()(de::DVectorPtr args) = 0;
	//virtual double operator()(de::NVectorPtr args) = 0;
	virtual double operator()(de::NVectorPtr args, de::constraints_ptr constraints, const sce::Site_WeaponRange_relation swRelation) = 0;

	/**
	* An objective function has a name
	*
	* @author louiehan (11/18/2019)
	*
	* @return const std::string&
	*/
	const std::string& name() const { return m_name; }
};

/**
* Smart pointer to an objective function
*/
typedef std::shared_ptr< objective_function > objective_function_ptr;

/**
* Objective function to optimize is the "sphere function":
*
* f(x,y) = x^2 + y^2
*/
class sphere_function :public objective_function
{
public:
	sphere_function()
		: objective_function("sphere function")
	{
	}

	virtual double operator()(de::DVectorPtr args)
	{
		/**
		* The two function arguments are the elements index 0 and 1 in
		* the argument vector, as defined by the constraints vector
		* below
		*/
		double x = (*args)[0];
		double y = (*args)[1];

		return x*x + y*y;
	}
};

class evaluation_route :public objective_function
{
public:
	evaluation_route()
		: objective_function("Route evaluation function")
	{
	}

	/**
	* 计算路径长度代价
	* 
	* @author louie (11/28/2019)
	*
	* @param args[input] 每个个体的节点集合，这里只用到每段搜索路径的起始点与终止点
	*		 norms[input] 个体各节点之间的长度值
	*		 length_cost[output] 待计算的长度代价值
	*/
	double evaluation_length_cost(de::NVectorPtr args, const std::valarray<double> &norms)
	{
		double route_length = 0.0;
		for (auto iter:norms)
		{
			route_length += iter;
		}
		//归一化
		//double rate = route_length/(*(args->end()) - *(args->begin())).norm();
		//return rate>=2 ? 1.0 : rate-1;
		return route_length;
	}

	/**
	* 计算路径长度标准差代价
	*
	* @author louie (11/28/2019)
	*
	* @param norms[input] 个体各节点之间的长度值
	*		 std_variance_cost[output] 待计算的标准差代价值
	*/
	double evaluation_std_variance_cost(const std::valarray<double> &norms)
	{
		double sum = norms.sum();
		double mean = sum / norms.size(); //均值  

		double accum = 0.0;
		for(auto d:norms) {
			accum += (d - mean)*(d - mean);
		}
		//归一化
		//return sqrt(accum / (norms.size() - 1))>1 ? 1-1/sqrt(accum / (norms.size() - 1)) : 0.0; //标准差
		return sqrt(accum / (norms.size() - 1));
	}

	/**
	* 计算偏转角度代价
	*
	* @author louie (11/28/2019)
	*
	* @param diff_vector[input] 个体各节点之间的差分向量
	*		 norms[input] 个体各节点之间的长度值
	*		 constraints[input] 含有偏航角、俯仰角、滚转角上限值的约束集合
	*		 angle_cost[output] 待计算的长度代价值
	*/
	double evalution_angle_cost(const de::NVector &diff_vector, const std::valarray<double> &norms, de::constraints_ptr constraints)
	{
		//向量夹角
		std::valarray<double> vetorial_angle(0.0,diff_vector.size()-1);
		//偏航角yaw angle
		std::valarray<double> yaw_angles(0.0, diff_vector.size() - 1);
		//俯仰角pitching angle
		std::valarray<double> pitching_angles(0.0, diff_vector.size() - 1);
		//滚转角rolling angle
		std::valarray<double> rolling_angles(0.0, diff_vector.size() - 1);


		for (size_t i = 0; i < diff_vector.size()-1; i++)
		{
			double tmp=diff_vector[i] * diff_vector[i + 1] / (norms[i] * norms[i + 1]);
			de::Node tmpNode = diff_vector[i + 1] - diff_vector[i];
			//以经度轴为参考轴
			//防止atan2(double y,double x)两个参数同为0时发生域错误。虽然在MS帮助文档中申明两个参数同为0时，结果也是0，但也要预防有些低版本编译功能不支持的问题
			if (tmpNode.altitude()==0.0 && tmpNode.longitude()==0.0) 
			{
				pitching_angles[i] = 0;
			}
			else
			{
				pitching_angles[i] = atan2(tmpNode.altitude(), tmpNode.longitude())*180.0 / PI;
			}

			if (tmpNode.latitude()==0.0 && tmpNode.altitude()==0.0)
			{
				rolling_angles[i] = 0;
			}
			else
			{
				rolling_angles[i] = atan2(tmpNode.latitude(), tmpNode.altitude())*180.0 / PI;
			}

			if (tmpNode.latitude()==0.0 && tmpNode.longitude()==0.0)
			{
				yaw_angles[i] = 0;
			}
			else
			{
				yaw_angles[i] = atan2(tmpNode.latitude(), tmpNode.longitude())*180.0 / PI;
			} 
						
			if ( tmp>=0 && tmp <= 1 )
			{
				vetorial_angle[i] = acos(tmp) * 180.0 / PI;
			}
			else
			{ 
				vetorial_angle[i] = (tmp>1) ? 0.0 : 180.0;
			}
		}
		 
		//这里最大转向角超出安全限制会施加惩罚,所有代价值均归一化
		double yaw_angle_cost = (abs(yaw_angles.max()) >= (*constraints)[3]->max()) ? 1.0 : 0.0;
		double pitching_angle_cost = (abs(pitching_angles.max()) >= (*constraints)[4]->max()) ? 1.0 : 0.0;
		double rolling_angle_cost = (abs(rolling_angles.max()) >= (*constraints)[5]->max()) ? 1.0 : 0.0;

		//angle_cost = vetorial_angle.max();
		//归一化
		//return (yaw_angle_cost>0 || pitching_angle_cost>0 || rolling_angle_cost>0) ? 1.0 : 0.0;

		return abs(yaw_angles.max()) + abs(pitching_angles.max()) + abs(rolling_angles.max());
	}

	/**
	* 计算地形代价
	*
	* @author louie (11/28/2019)
	*
	* @param diff_vector[input] 个体各节点之间的差分向量
	*		 norms[input] 个体各节点之间的长度值
	*		 constraints[input] 含有偏航角、俯仰角、滚转角上限值的约束集合
	*		 angle_cost[output] 待计算的长度代价值
	*/
	double evaluation_route_tabu_cost(de::NVectorPtr args, de::constraint_ptr high_constraint)
	{
		double tabu_cost(0.0);
		//由于暂时没有地形信息，这里只进行与最低安全飞行高度进行比较
		std::valarray<double> heights(0.0, args->size());
		for (size_t i=0; i<args->size();i++)
		{
			heights[i]=(*args)[i].altitude();

			tabu_cost += heights[i] - high_constraint->min();
		}
		//归一化
		//return heights.min() > high_constraint->min() ? 0.0 : 1.0;

		return tabu_cost;
	}

	double evaluation_route_mission_cost(de::NVectorPtr args)
	{
		double route_cost(0.0);
		std::vector<double> ermc(args->size()-1,0.0);
		for (size_t i = 0; i < args->size() - 1; ++i)
		{
			ermc[i] = (*(args->cend()-1) - (*args)[i]).norm();
		}

		for (size_t j = 0; j < ermc.size() - 1; ++j)
		{
			route_cost += ermc[j + 1] / ermc[j];
		}
		return route_cost;
	}

	double evaluation_threat_cost(const de::NVectorPtr args, const sce::Site_WeaponRange_relation swRelation)
	{
		constexpr double K_j = 1.0;//威胁系数
		double threat_cost = 0.0;

		for (size_t i = 0; i < args->size(); i++)
		{
			for (const auto &map_it:swRelation)
			{
				double dist = (args->at(i) - *map_it.first).norm();
				threat_cost += dist<map_it.second ? K_j / pow(dist, 4) : 0.0;
			}
		}		
		return threat_cost;
	}

	double evaluation_route_survival_cost(de::NVectorPtr args)
	{
		return 0.0;
	}

	virtual double operator()(de::NVectorPtr args,de::constraints_ptr constraints,const sce::Site_WeaponRange_relation swRelation)
	{
		//double tabu_cost(0.0), std_variance_cost(0.0),mission_cost(0.0),survival_cost(0.0), length_cost(0.0), angle_cost(0.0),threat_cost(0.0);

		de::NVector diff_vector(args->size() - 1, args->at(0));
		//DVector norms(args->size() - 1, 0);
		std::valarray<double> norms(0.0, args->size() - 1);
		

		for (size_t i = 0; i <args->size() - 1; i++)
		{
			diff_vector[i] = (*args)[i + 1] - (*args)[i];
			norms[i] = ((*args)[i + 1] - (*args)[i]).norm();
		}

		// 评估飞行高度和地形
		double tabu_cost = evaluation_route_tabu_cost(args,(*constraints)[2]);
		// 评估与任务点的距离
		double mission_cost = evaluation_route_mission_cost(args);
		//评估长度代价
		double length_cost = evaluation_length_cost(args, norms);
		//评估角度代价
		double angle_cost = evalution_angle_cost(diff_vector,norms,constraints);
		//评估方差代价
		double std_variance_cost = evaluation_std_variance_cost(norms);
		//评估威胁代价
		double threat_cost = evaluation_threat_cost(args, swRelation);

		// 评估生存代价,这在生存率评估环节做
		//double survival_cost = evaluation_route_survival_cost(args);

		// 适应度
		double cost = 0.2*length_cost + 0.2*angle_cost + 0.1*mission_cost + 0.1*tabu_cost+ 0.2*std_variance_cost+0.2*threat_cost;

		return cost;
	}

};

#endif //DE_OBJECTIVE_FUNCTION_HPP_INCLUDED