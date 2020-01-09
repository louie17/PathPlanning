#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef DE_INTERFACE_HPP_INCLUDED
#define DE_INTERFACE_HPP_INCLUDED

#include <iostream>
#include <memory>
#include <cassert>
#include <algorithm>
#include <unordered_map>

#include "differential_evolution.hpp"
#include "objective_function.hpp"
#include "de_types.hpp"
#include "Scenario.hpp"

namespace de
{
	//typedef std::unordered_map<std::shared_ptr<sce::Site>, double> Site_WeaponRange_relation;
	/**
	* DE算法便捷统一调用接口函数
	*
	* @author louiehan 11/28/2019
	*
	* @param scenario 算法运行所需的场景参数类对象实体；
	* @param population_size 在种群的数量；
	* @param initial_Node_Number 除了起始、终止两点外，每个个体中所包含的节点数，对应于varCount；
	* @param evolution_Number 最大进化代数，终止策略；
	* @param factor 如果为true，将尝试最小化成本；如果为false，则将尝试最大化成本；
	* @param terminationStrategy 终止策略；
	* @param selectionStrategy 选择策略；
	* @param mutationStrategy 变异策略；
	* @param listener 侦听器。
	*/
	NVectorPtr De_alg(sce::Site_WeaponRange_relation &swRelation,sce::VertexsVector &vertexs,sce::Point &startPoint, sce::Point &endPoint, const size_t &population_size, const size_t &initial_Node_Number, const size_t &evolution_Number, const double &weight, const double &cross_Probability)
	{
		try
		{
			/**
			* 创建与初始化constraints对象
			*
			* 首先创建一个默认constraints对象(double type, min -1.0e6, max 1.0e6)，
			* 然后将目标函数中的各个参数设置为实数类型：0<=y<=100.
			*/
			constraints_ptr constraints(std::make_shared< constraints >(initial_Node_Number+2, 1.0e-6, 1.0e6));

			//TODO: 找出飞行区域内的上下限
			DVector v_Altitudes,v_Latitudes,v_Longitudes;
			for (size_t i=0;i<vertexs.size();++i)
			{
				auto iter= vertexs.at(i);
				v_Altitudes.push_back(iter->getAltitude());
				v_Latitudes.push_back(iter->getLatitude());
				v_Longitudes.push_back(iter->getLongitude());
			}
			
			(*constraints)[0] = std::make_shared< real_constraint >(*std::min_element(v_Longitudes.cbegin(),v_Longitudes.cend()), *std::max_element(v_Longitudes.cbegin(), v_Longitudes.cend()));//longitude
			(*constraints)[1] = std::make_shared< real_constraint >(*std::min_element(v_Latitudes.cbegin(), v_Latitudes.cend()), *std::max_element(v_Latitudes.cbegin(), v_Latitudes.cend()));//latitude
			(*constraints)[2] = std::make_shared< real_constraint >(*std::min_element(v_Altitudes.cbegin(), v_Altitudes.cend()), *std::max_element(v_Altitudes.cbegin(), v_Altitudes.cend()));//altitude

			(*constraints)[3] = std::make_shared< real_constraint >(-60, 60);//yaw angle
			(*constraints)[4] = std::make_shared< real_constraint >(-60, 60);//pitching angle
			(*constraints)[5] = std::make_shared< real_constraint >(-60, 60);//rolling angle

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
			termination_strategy_ptr terminationStrategy(std::make_shared< max_gen_termination_strategy >(evolution_Number));

			/**
			* 实例化选择策略-我们将使用最好的父/子策略
			*/
			selection_strategy_ptr selectionStrategy(std::make_shared< best_parent_child_selection_strategy >());

			/**
			* 实例化变异策略-我们将使用变异策略1，权重和交叉因子分别设置为0.5和0.9
			*/
			mutation_strategy_arguments mutation_arguments(weight, cross_Probability);
			mutation_strategy_ptr mutationStrategy(std::make_shared< mutation_strategy_1 >(initial_Node_Number, mutation_arguments));

			/**
			* 使用先前定义的约束，处理器，侦听器和各种策略实例化差分进化。
			*/
			//differential_evolution< sphere_function > de(VARS_COUNT, POPULATION_SIZE, _processors, constraints, true, terminationStrategy, selectionStrategy, mutationStrategy, listener);
			Node startNode,endNode;
			startNode(startPoint);
			endNode(startNode);
			differential_evolution< evaluation_route > de(initial_Node_Number, population_size, _processors,startNode,endNode,constraints, true, terminationStrategy, selectionStrategy, mutationStrategy, listener);
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
				<< " for x=" << (*best->vars())[1].longitude()
				<< ", y=" << (*best->vars())[1].latitude()
				<< ", y=" << (*best->vars())[1].altitude() << std::endl;

			return best->vars();
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
}

#endif