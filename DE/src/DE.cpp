// MTADE.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <memory>
#include <cassert>
#include <algorithm>
#include <unordered_map>

#include "differential_evolution.hpp"
#include "objective_function.hpp"

#include "Scenario.hpp"

using namespace de;

#define VARS_COUNT 20
#define POPULATION_SIZE 200

int main(int argc, char *argv[])
{
	sce::Scenario scenario;

	//根据威胁位置获取每个威胁的最大武器射程
	DVector wcrange(scenario.getAllSite().size(), 0.0);
	sce::Site_WeaponRange_relation swRelation;

	assert(scenario.getAllSite().size() > 0);
	assert(scenario.getAllPlatformSiteRelation().size() > 0);
	for (size_t i = 0; i < scenario.getAllSite().size(); i++)
	{
		auto iterS = scenario.getAllSite().at(i);
		std::vector<unsigned int> siteTmp;
		for (auto iterPSR : scenario.getAllPlatformSiteRelation())
		{
			if (iterPSR.getSiteName() == iterS->getName())
			{
				for (auto iterPWR : scenario.getAllPlatformWeaponRelation())
				{
					if (iterPSR.getPlatformName() == iterPWR.getPlatformName())
					{
						siteTmp.push_back(iterPWR.getWeapon()->getWeaponAreaCoverage());
					}
				}
			}
		}
		assert(siteTmp.size() > 0);
		wcrange[i] = siteTmp.size() > 0 ? *std::max_element(siteTmp.cbegin(), siteTmp.cend()) : 0.0;
		auto ret = swRelation.insert(std::make_pair(iterS, wcrange[i]));
		assert(ret.second);
	}

	try
	{
		/**
		* 创建与初始化constraints对象
		*
		* 首先创建一个默认constraints对象(double type, min -1.0e6, max 1.0e6)，
		* 然后将目标函数中的各个参数设置为实数类型：0<=y<=100.
		*/
		constraints_ptr constraints(std::make_shared< constraints >(VARS_COUNT, 1.0e-6, 1.0e6));
		(*constraints)[0] = std::make_shared< real_constraint >(0, 100);//width
		(*constraints)[1] = std::make_shared< real_constraint >(0, 100);//depth
		(*constraints)[2] = std::make_shared< real_constraint >(0, 20);//heigth

		(*constraints)[3] = std::make_shared< real_constraint >(0, 60);//yaw angle
		(*constraints)[4] = std::make_shared< real_constraint >(0, 60);//pitching angle
		(*constraints)[5] = std::make_shared< real_constraint >(0, 60);//rolling angle

		(*constraints)[6] = std::make_shared< real_constraint >(0, 100);//oil
		(*constraints)[7] = std::make_shared< real_constraint >(0, 10000);//speed


		/**
		* 实例化目标函数
		*
		* 球目标函数可以是任何以de :: DVectorPtr作为参数并返回双精度的函数或函子。 它可以通过引用，指针或共享指针传递。
		*/
		//sphere_function of;		
		evaluation_route of;

		/**
		* 实例化两个空侦听器，一个用于差分进化，另一个用于处理器
		*/
		listener_ptr listener(std::make_shared< null_listener >());
		processor_listener_ptr processor_listener(std::make_shared< null_processor_listener >());

		/**
		* 用并行处理器的数量（4），目标函数和侦听器实例化处理器的集合。
		*/
		//processors< sphere_function >::processors_ptr _processors(std::make_shared< processors< sphere_function > >(4, std::ref(of), processor_listener));
		processors< evaluation_route >::processors_ptr _processors(std::make_shared< processors< evaluation_route > >(4, std::ref(of), constraints, swRelation, processor_listener));
		/**
		* 实例化一个简单的终止策略，它将在1000代之后停止优化过程。
		*/
		termination_strategy_ptr terminationStrategy(std::make_shared< max_gen_termination_strategy >(1000));

		/**
		* 实例化选择策略-我们将使用最好的父/子策略
		*/
		selection_strategy_ptr selectionStrategy(std::make_shared< best_parent_child_selection_strategy >());

		/**
		* 实例化变异策略-我们将使用变异策略1，权重和交叉因子分别设置为0.5和0.9
		*/
		mutation_strategy_arguments mutation_arguments(0.5, 0.9);
		mutation_strategy_ptr mutationStrategy(std::make_shared< mutation_strategy_1 >(VARS_COUNT, mutation_arguments));

		/**
		* 使用先前定义的约束，处理器，侦听器和各种策略实例化差分进化。
		*/
		//differential_evolution< sphere_function > de(VARS_COUNT, POPULATION_SIZE, _processors, constraints, true, terminationStrategy, selectionStrategy, mutationStrategy, listener);
		differential_evolution< evaluation_route > de(VARS_COUNT, POPULATION_SIZE, _processors, constraints, true, terminationStrategy, selectionStrategy, mutationStrategy, listener);
		/**
		* 运行优化进程
		*/
		de.run();

		/**
		* 从优化过程中获得最佳个体
		*/
		individual_ptr best(de.best());

		/**
		* Print out the result
		*/
		//std::cout << "minimum value for the " << /*of->name() << */" is " << best->cost() << " for x=" << (*best->vars())[0] << ", y=" << (*best->vars())[1] << std::endl;
		std::cout << "minimum value for the " << /*of->name() << */" is " << best->cost()
			<< " for x=" << (*best->vars())[0].longitude()
			<< ", y=" << (*best->vars())[1].latitude()
			<< ", y=" << (*best->vars())[1].altitude() << std::endl;
	}
	catch (const de::exception& e)
	{
		/**
		* Print out any errors that happened during the initialization
		* or optimization phases
		*/
		std::cout << "an error occurred: " << e.what();
	}

	return 0;
}

