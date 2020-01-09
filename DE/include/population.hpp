#ifndef DE_POPULATION_HPP_INCLUDED
#define DE_POPULATION_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "individual.hpp"

namespace de
{

	typedef std::vector< individual_ptr > population_base;

	/**
	 * 种群类
	 *
	 * @author louiehan (11/15/2019)
	 */
	class population : public population_base
	{
	public:
		/**
		 * 构造包含未初始化个体的种群对象
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @param popSize 种群规模(个体数量)
		 * @param varCount 每个个体的变量个数
		 */
		population(size_t popSize, size_t varCount)
			: population_base(popSize)
		{
			assert(popSize > 0);
			assert(varCount > 0);

			init(popSize, varCount);
		}

		/**
		 * 构造一个种群对象并通过首先将所有变量设置为在相应约束所施加的限制内的
		 * 随机值来初始化其个体，然后为每个个体运行目标函数以计算相关代价。
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @param popSize 种群规模(个体数量)
		 * @param varCount 每个个体的变量个数
		 * @param constraints 初始化个体时使用的约束集合
		 */
		population(size_t popSize, size_t varCount, constraints_ptr constraints)
			: population_base(popSize)
		{
			assert(popSize > 0);
			assert(varCount > 0);

			init(popSize, varCount);

			for (population::size_type i = 0; i < size(); ++i)
				at(i)->init(constraints);
		}

		/**
		* 构造一个种群对象并通过首先将所有变量设置为在相应约束所施加的限制内的
		* 随机值来初始化其个体，然后为每个个体运行目标函数以计算相关代价。
		*
		* @author louiehan (11/15/2019)
		*
		* @param popSize 种群规模(个体数量)
		* @param varCount 每个个体的变量个数
		* @param startPoint,endPoint 路径起始与终止点
		* @param constraints 初始化个体时使用的约束集合
		*/
		population(size_t popSize, size_t varCount, const Node &startPoint, const Node &endPoint, constraints_ptr constraints)
			: population_base(popSize)
		{
			assert(popSize > 0);
			assert(varCount > 0);

			init(popSize, varCount);//增加了起始点与终止点

			for (population::size_type i = 0; i < size(); ++i)
				at(i)->init(startPoint,endPoint,constraints);
		}

		/**
		 * 返回一个种群中最好的个体
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @return individual_ptr
		 */
		individual_ptr best(bool minimize) const
		{
			population::size_type best(0);

			for (population::size_type i = 0; i < size(); ++i)
				best = at(i)->better_or_equal(at(best), minimize) ? i : best;

			return at(best);
		}

		/**
		 * 返回种群的字符串表示形式
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @return std::string string representation of the population
		 */
		std::string to_string()
		{
			std::string str;
			for (population::size_type i = 0; i < size(); ++i)
				str += at(i)->to_string();

			return str;
		}

	private:
		void init(size_t popSize, size_t varCount)
		{
			for (population_base::size_type i = 0; i < size(); ++i)
				operator[](i) = std::make_shared< individual >(varCount);
		}
	public:

	};

	typedef std::shared_ptr< population > population_ptr;

}

#endif //DE_POPULATION_HPP_INCLUDED
