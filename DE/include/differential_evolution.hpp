// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef DE_DIFFERENTIAL_EVOLUTION_HPP_INCLUDED
#define DE_DIFFERENTIAL_EVOLUTION_HPP_INCLUDED

#include <memory>
#include <tuple>

#include "random_generator.hpp"
#include "multithread.hpp"
#include "individual.hpp"
#include "processors.hpp"
#include "mutation_strategy.hpp"
#include "population.hpp"
#include "selection_strategy.hpp"
#include "termination_strategy.hpp"
#include "listener.hpp"

namespace de
{

	/**
	 * 优化过程期间发生错误时将抛出异常。
	 *
	 * @author louiehan (11/11/2019)
	 */
	class differential_evolution_exception
	{
	};

	/**
	 * 差分进化算法主类
	 *
	 * 根据各种输入参数或策略运行优化过程。
	 *
	 * @author louiehan (11/11/2019)
	 */
	template< typename T > class differential_evolution
	{
	private:
		const size_t m_varCount;
		const size_t m_popSize;

		population_ptr m_pop1;
		population_ptr m_pop2;
		individual_ptr m_bestInd;

		constraints_ptr m_constraints;
		typename processors< T >::processors_ptr m_processors;
		termination_strategy_ptr m_terminationStrategy;
		selection_strategy_ptr m_selectionStrategy;
		mutation_strategy_ptr m_mutationStrategy;
		listener_ptr m_listener;

		const bool m_minimize;
	public:
		/**
		 * 构造一个differential_evolution对象
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param varCount 变量总数。它包括目标函数所需的变量，但具有算法所需的更多元素；
		 * @param popSize 在种群的数量；
		 * @param processors 优化会话期间使用的并行处理器数；
		 * @param constraints 约束向量，其中包含目标函数使用的变量的约束以及算法内部使用的所有其他变量的约束；
		 * @param minimize 如果为true，将尝试最小化成本；如果为false，则将尝试最大化成本；
		 * @param terminationStrategy 终止策略；
		 * @param selectionStrategy 选择策略；
		 * @param mutationStrategy 变异策略；
		 * @param listener 侦听器。
		 */
		differential_evolution(size_t varCount, size_t popSize, typename processors< T >::processors_ptr processors, constraints_ptr constraints, bool minimize,
			termination_strategy_ptr terminationStrategy, selection_strategy_ptr selectionStrategy,
			mutation_strategy_ptr mutationStrategy, de::listener_ptr listener)
			try

			: m_varCount(varCount), m_popSize(popSize), m_pop1(std::make_shared< population >(popSize, varCount, constraints)),
			m_pop2(std::make_shared< population >(popSize, varCount)), m_bestInd(m_pop1->best(minimize)),
			m_constraints(constraints), m_processors(processors), m_minimize(minimize), m_terminationStrategy(terminationStrategy),
			m_listener(listener), m_selectionStrategy(selectionStrategy), m_mutationStrategy(mutationStrategy)
		{
			assert(processors);
			assert(constraints);
			assert(terminationStrategy);
			assert(selectionStrategy);
			assert(listener);
			assert(mutationStrategy);

			assert(popSize > 0);
			assert(varCount > 0);

			//通过使用初始随机参数运行所有目标函数来初始化总体1。
			processors->push(m_pop1);
			processors->start();
			processors->wait();

		}
		catch (const processors_exception&)
		{
			throw differential_evolution_exception();
		}

		/**
		* 构造differential_evolution对象
		*
		* @author louiehan (11/11/2019)
		*
		* @param varCount 变量总数。它包括目标函数所需的变量，但具有算法所需的更多元素；
		* @param popSize 在种群的数量；
		* @param processors 优化会话期间使用的并行处理器数；
		* @param startNode,endNode 路径的起始与终止点
		* @param constraints 约束向量，其中包含目标函数使用的变量的约束以及算法内部使用的所有其他变量的约束；
		* @param minimize 如果为true，将尝试最小化成本；如果为false，则将尝试最大化成本；
		* @param terminationStrategy 终止策略；
		* @param selectionStrategy 选择策略；
		* @param mutationStrategy 变异策略；
		* @param listener 侦听器。
		*/
		differential_evolution(size_t varCount, size_t popSize, typename processors< T >::processors_ptr processors,const Node &startNode, const Node &endNode, constraints_ptr constraints, bool minimize,termination_strategy_ptr terminationStrategy, selection_strategy_ptr selectionStrategy, mutation_strategy_ptr mutationStrategy, de::listener_ptr listener)
			try

			: m_varCount(varCount), m_popSize(popSize), m_pop1(std::make_shared< population >(popSize, varCount+2, startNode, endNode, constraints)),
			m_pop2(std::make_shared< population >(popSize, varCount+2)), m_bestInd(m_pop1->best(minimize)),
			m_constraints(constraints), m_processors(processors), m_minimize(minimize), m_terminationStrategy(terminationStrategy),
			m_listener(listener), m_selectionStrategy(selectionStrategy), m_mutationStrategy(mutationStrategy)
		{
			assert(processors);
			assert(constraints);
			assert(terminationStrategy);
			assert(selectionStrategy);
			assert(listener);
			assert(mutationStrategy);

			assert(popSize > 0);
			assert(varCount > 0);

			//通过使用初始随机参数运行所有目标函数来初始化总体1。
			processors->push(m_pop1);
			processors->start();
			processors->wait();

		}
		catch (const processors_exception&)
		{
			throw differential_evolution_exception();
		}

		virtual ~differential_evolution(void)
		{
		}

		/**
		 * 启动差分进化优化过程。
		 *
		 * 尽管处理是并行进行的，但此函数是同步的，并且在优化完成或错误触发异常之前不会返回。
		 *
		 * @author louiehan (11/11/2019)
		 */
		void run()
		{
			try
			{
				m_listener->start();
				individual_ptr bestIndIteration(m_bestInd);

				for (size_t genCount = 0; m_terminationStrategy->event(m_bestInd, genCount); ++genCount)
				{
					m_listener->startGeneration(genCount);
					for (size_t i = 0; i < m_popSize; ++i)
					{
						mutation_strategy::mutation_info mutationInfo((*m_mutationStrategy)(*m_pop1, bestIndIteration, i));

						individual_ptr tmpInd(std::get< 0 >(mutationInfo));

						tmpInd->ensureConstraints(m_constraints, std::get< 1 >(mutationInfo));

						// 填充队列
						m_processors->push(tmpInd);

						// 暂时将tempInd放在一个vector容器中（它们在处理之前是空的），在处理之后将被移动到正确的位置。
						(*m_pop2)[i] = tmpInd;
					}

					m_listener->startProcessors(genCount);
					m_processors->start();
					m_processors->wait();
					m_listener->endProcessors(genCount);

					//BestParentChildSelectionStrategy()( m_pop1, m_pop2, m_bestInd, m_minimize );
					m_listener->startSelection(genCount);
					(*m_selectionStrategy)(m_pop1, m_pop2, m_bestInd, m_minimize);
					bestIndIteration = m_bestInd;

					m_listener->endSelection(genCount);

					m_listener->endGeneration(genCount, bestIndIteration, m_bestInd);

				}

				//BOOST_SCOPE_EXIT_TPL( (m_listener) )
				//{
				m_listener->end();
				//} 
				//BOOST_SCOPE_EXIT_END
			}
			catch (const processors_exception&)
			{
				m_listener->error();
				throw differential_evolution_exception();
			}

		}

		/**
		 * 返回优化过程产生的最佳个体。
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return individual_ptr
		 */
		individual_ptr best() const { return m_bestInd; }
	};

}

#endif //DE_DIFFERENTIAL_EVOLUTION_HPP_INCLUDED
