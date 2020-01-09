#ifndef DE_MUTATION_STRATEGY_HPP_INCLUDED
#define DE_MUTATION_STRATEGY_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "population.hpp"

#define URN_DEPTH 5

#include <tuple>

namespace de
{

	/**
	 * 变异策略权重因子，交叉因子和抖动因子（根据前两个参数计算得出）所使用的参数。
	 *
	 * @author louiehan (11/15/2019)
	 */
	class mutation_strategy_arguments
	{
	private:
		const double m_weight;
		const double m_crossover;
		const double m_dither;

	public:
		/**
		 * 构造一个变异策略类对象。
		 *
		 * 除了调用代码提供的权重和交叉因子之外，该对象还包含一个变异因子，
		 * 该变异因子是在构造时计算出来的，并被某些突变策略使用。
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @param weight 权重因子，差分进化算法定义的介于0-2之间的double值。
		 * @param crossover 交叉因子 差分进化算法定义的介于0-1之间的double值。
		 */
		mutation_strategy_arguments(double weight, double crossover)
			: m_weight(weight), m_crossover(crossover), m_dither(weight + genrand() * (1.0 - weight))
		{
			// todo: test or assert that weight and crossover are within bounds (0-1, 0-2 or something)
		}

		/**
		 * 返回权重因子
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @return double
		 */
		double weight() const { return m_weight; }
		/**
		 * 返回交叉因子
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @return double
		 */
		double crossover() const { return m_crossover; }
		/**
		 * 返回变异因子
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @return double
		 */
		double dither() const { return m_dither; }
	};

	/**
	 * 变异策略的抽象基类
	 *
	 * 变异策略定义了在优化过程中如何调整变量值。
	 *
	 * @author louiehan (11/15/2019)
	 */
	class mutation_strategy
	{
	private:
		mutation_strategy_arguments m_args;
		size_t m_varCount;

	protected:
		/**
		 * 用变异策略生成一组4个随机大小的数作为索引。
		 *
		 * 这些数字必须全部不同，并且必须与外部提供的索引不同。
		 *
		 * @author louiehan (11/15/2019)
		 */
		class Urn
		{
			size_t m_urn[URN_DEPTH];

		public:
			/**
			 * Constructs an urn object
			 *
			 * @author louiehan (11/15/2019)
			 *
			 * @param NP 生成的随机数的上限（不包括）
			 * @param avoid 生成随机数时要避免的值
			 */
			Urn(size_t NP, size_t avoid)
			{
				do m_urn[0] = genintrand(0, NP, true); while (m_urn[0] == avoid);
				do m_urn[1] = genintrand(0, NP, true); while (m_urn[1] == m_urn[0] || m_urn[1] == avoid);
				do m_urn[2] = genintrand(0, NP, true); while (m_urn[2] == m_urn[1] || m_urn[2] == m_urn[0] || m_urn[2] == avoid);
				do m_urn[3] = genintrand(0, NP, true); while (m_urn[3] == m_urn[2] || m_urn[3] == m_urn[1] || m_urn[3] == m_urn[0] || m_urn[3] == avoid);
			}

			/**
			 * 返回生成的四个随机数之一
			 *
			 * @author louiehan (11/15/2019)
			 *
			 * @param index 要返回的随机数的索引，可以在0-3之间。
			 *
			 * @return size_t
			 */
			size_t operator[](size_t index) const { assert(index < 4); return m_urn[index]; }
		};


	public:
		virtual ~mutation_strategy()
		{
		}

		/**
		 * constructs a mutation strategy
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @param varCount 变量个数
		 * @param args 变异策略参数集
		 */
		mutation_strategy(size_t varCount, const mutation_strategy_arguments& args)
			: m_args(args), m_varCount(varCount)
		{
		}

		/**
		 * 运算符（）成员返回的元组的类型。
		 */
		typedef std::tuple< individual_ptr, de::NVectorPtr > mutation_info;

		/**
		 * 执行变异
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @param pop 当前种群的引用；
		 * @param bestIt 上一代最好的个体；
		 * @param i 当前个体的索引。
		 *
		 * @return mutation_info 元组（tuple），包含变异的个体和装有与变量数量相同大小的double值的vector容器，
		 *						 在它们超出相应约束所施加的限制的情况下，用作原值以生成新值。
		 */
		virtual mutation_info operator()(const population& pop, individual_ptr bestIt, size_t i) = 0;

		/**
		 * returns the number of variables
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @return size_t
		 */
		size_t varCount() const { return m_varCount; }

		/**
		 * 返回权重因子
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @return double
		 */
		double weight() const { return m_args.weight(); }

		/**
		 * 返回交叉因子
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @return double
		 */
		double crossover() const { return m_args.crossover(); }

		/**
		 * 返回变异因子
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @return double
		 */
		double dither() const { return m_args.dither(); }
	};

	typedef std::shared_ptr< mutation_strategy > mutation_strategy_ptr;

	/**
	 * 变异策略 DE/rand/1/bin #1
	 *
	 * @author louiehan (11/15/2019)
	 */
	class mutation_strategy_1 : public mutation_strategy
	{
	public:
		/**
		 * constructs a mutation strategy # 1
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @param varCount 变量个数
		 * @param args 变异策略参数集
		 */
		mutation_strategy_1(size_t varCount, const mutation_strategy_arguments& args)
			: mutation_strategy(varCount, args)
		{
		}

		/**
		* 执行变异
		*
		* @author louiehan (11/15/2019)
		*
		* @param pop 当前种群的引用；
		* @param bestIt 上一代最好的个体；
		* @param i 当前个体的索引。
		*
		* @return mutation_info 元组（tuple），包含变异的个体和装有与变量数量相同大小的double值的vector容器，
		*						在它们超出相应约束所施加的限制的情况下，用作原值以生成新值。
		*/
		mutation_info operator()(const population& pop, individual_ptr bestIt, size_t i)
		{
			assert(bestIt);

			de::NVectorPtr origin(std::make_shared< de::NVector >(varCount()));
			individual_ptr tmpInd(std::make_shared< individual >(*pop[i]->vars()));
			Urn urn(pop.size(), i);


			// make sure j is within bounds
			size_t j = genintrand(0, varCount(), true);
			size_t k = 0;

			do
			{
				(*tmpInd->vars())[j] = (*pop[urn[0]]->vars())[j] + weight() * ((*pop[urn[1]]->vars())[j] - (*pop[urn[2]]->vars())[j]);

				j = ++j % varCount();
				++k;
			} while (genrand() < crossover() && k < varCount());

			origin = pop[urn[0]]->vars();

			return mutation_info(tmpInd, origin);
		}

	};

	/**
	 * 变异策略2 De/rand-to-best/bin # 2
	 *
	 * @author louiehan (11/15/2019)
	 */
	class mutation_strategy_2 : public mutation_strategy
	{
	public:
		/**
		 * constructs a mutation strategy # 2
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @param varCount 变量个数
		 * @param args 变异策略参数集
		 */
		mutation_strategy_2(size_t varCount, const mutation_strategy_arguments& args)
			: mutation_strategy(varCount, args)
		{
		}

		/**
		* 执行变异
		*
		* @author louiehan (11/15/2019)
		*
		* @param pop 当前种群的引用；
		* @param bestIt 上一代最好的个体；
		* @param i 当前个体的索引。
		*
		* @return mutation_info 元组（tuple），包含变异的个体和装有与变量数量相同大小的double值的vector容器，
		*						在它们超出相应约束所施加的限制的情况下，用作原值以生成新值。
		*/
		mutation_info operator()(const population& pop, individual_ptr bestIt, size_t i)
		{
			assert(bestIt);

			de::NVectorPtr origin(std::make_shared< de::NVector >(varCount()));
			individual_ptr tmpInd(std::make_shared< individual >(*pop[i]->vars()));
			Urn urn(pop.size(), i);


			// make sure j is within bounds
			size_t j = genintrand(0, varCount(), true);
			size_t k = 0;

			do
			{
				(*tmpInd->vars())[j] = (*tmpInd->vars())[j] +
					weight() * ((*bestIt->vars())[j] - (*tmpInd->vars())[j]) +
					weight() * ((*pop[urn[1]]->vars())[j] - (*pop[urn[2]]->vars())[j]);

				j = ++j % varCount();
				++k;
			} while (genrand() < crossover() && k < varCount());

			origin = pop[urn[0]]->vars();

			return mutation_info(tmpInd, origin);
		}

	};

	/**
	 * 变异策略 DE/best/1/bin # 3
	 *
	 * @author louiehan (11/15/2019)
	 */
	class mutation_strategy_3 : public mutation_strategy
	{
	public:
		/**
		 * constructs a mutation strategy # 3
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @param varCount 变量个数
		 * @param args 变异策略参数集
		 */
		mutation_strategy_3(size_t varCount, const mutation_strategy_arguments& args)
			: mutation_strategy(varCount, args)
		{
		}

		/**
		* 执行变异
		*
		* @author louiehan (11/15/2019)
		*
		* @param pop 当前种群的引用；
		* @param bestIt 上一代最好的个体；
		* @param i 当前个体的索引。
		*
		* @return mutation_info 元组（tuple），包含变异的个体和装有与变量数量相同大小的double值的vector容器，
		*						在它们超出相应约束所施加的限制的情况下，用作原值以生成新值。
		*/
		mutation_info operator()(const population& pop, individual_ptr bestIt, size_t i)
		{
			assert(bestIt);

			de::NVectorPtr origin(std::make_shared< de::NVector >(varCount()));
			individual_ptr tmpInd(std::make_shared< individual >(*pop[i]->vars()));
			Urn urn(pop.size(), i);


			// make sure j is within bounds
			size_t j = genintrand(0, varCount(), true);
			size_t k = 0;

			do
			{
				double jitter = (0.0001* genrand() + weight());

				(*tmpInd->vars())[j] = (*bestIt->vars())[j] + jitter * ((*pop[urn[1]]->vars())[j] - (*pop[urn[2]]->vars())[j]);

				j = ++j % varCount();
				++k;
			} while (genrand() < crossover() && k < varCount());

			origin = pop[urn[0]]->vars();

			return mutation_info(tmpInd, origin);
		}
	};

	/**
	 * 变异策略 # 4
	 *
	 * @author louiehan (11/15/2019)
	 */
	class mutation_strategy_4 : public mutation_strategy
	{
	public:
		/**
		 * constructs a mutation strategy # 4
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @param varCount 变量个数
		 * @param args 变异策略参数集
		 */
		mutation_strategy_4(size_t varCount, const mutation_strategy_arguments& args)
			: mutation_strategy(varCount, args)
		{
		}

		/**
		* 执行变异
		*
		* @author louiehan (11/15/2019)
		*
		* @param pop 当前种群的引用；
		* @param bestIt 上一代最好的个体；
		* @param i 当前个体的索引。
		*
		* @return mutation_info 元组（tuple），包含变异的个体和装有与变量数量相同大小的double值的vector容器，
		*						在它们超出相应约束所施加的限制的情况下，用作原值以生成新值。
		*/
		mutation_info operator()(const population& pop, individual_ptr bestIt, size_t i)
		{
			assert(bestIt);

			de::NVectorPtr origin(std::make_shared< de::NVector >(varCount()));
			individual_ptr tmpInd(std::make_shared< individual >(*pop[i]->vars()));
			Urn urn(pop.size(), i);


			// make sure j is within bounds
			size_t j = genintrand(0, varCount(), true);
			size_t k = 0;

			double factor(weight() + genrand() * (1.0 - weight()));

			do
			{
				double jitter = (0.0001* genrand() + weight());

				(*tmpInd->vars())[j] = (*pop[urn[0]]->vars())[j] +
					factor * ((*pop[urn[1]]->vars())[j] - (*pop[urn[2]]->vars())[j]);

				j = ++j % varCount();
				++k;
			} while (genrand() < crossover() && k < varCount());

			origin = pop[urn[0]]->vars();

			return mutation_info(tmpInd, origin);
		}

	};



	/**
	 * 变异策略 # 5
	 *
	 * @author louiehan (11/15/2019)
	 */
	class mutation_strategy_5 : public mutation_strategy
	{

	public:
		/**
		 * constructs a mutation strategy # 5
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @param varCount 变量个数
		 * @param args 变异策略参数集
		 */
		mutation_strategy_5(size_t varCount, const mutation_strategy_arguments& args)
			: mutation_strategy(varCount, args)
		{
		}

		/**
		* 执行变异
		*
		* @author louiehan (11/15/2019)
		*
		* @param pop 当前种群的引用；
		* @param bestIt 上一代最好的个体；
		* @param i 当前个体的索引。
		*
		* @return mutation_info 元组（tuple），包含变异的个体和装有与变量数量相同大小的double值的vector容器，
		*						在它们超出相应约束所施加的限制的情况下，用作原值以生成新值。
		*/
		mutation_info operator()(const population& pop, individual_ptr bestIt, size_t i)
		{
			assert(bestIt);

			de::NVectorPtr origin(std::make_shared< de::NVector >(varCount()));
			individual_ptr tmpInd(std::make_shared< individual >(*pop[i]->vars()));
			Urn urn(pop.size(), i);

			// make sure j is within bounds
			size_t j = genintrand(0, varCount(), true);
			size_t k = 0;

			do
			{
				(*tmpInd->vars())[j] = (*pop[urn[0]]->vars())[j] + dither() * ((*pop[urn[1]]->vars())[j] - (*pop[urn[2]]->vars())[j]);

				j = ++j % varCount();
				++k;
			} while (genrand() < crossover() && k < varCount());

			origin = pop[urn[0]]->vars();
			return mutation_info(tmpInd, origin);
		}

	};

}

#endif //DE_MUTATION_STRATEGY_HPP_INCLUDED
