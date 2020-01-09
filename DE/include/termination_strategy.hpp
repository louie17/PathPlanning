#ifndef DE_TERMINATION_STRATEGY_HPP_INCLUDED
#define DE_TERMINATION_STRATEGY_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace de
{

	/**
	 * 定义了具体终结策略类所使用的接口的抽象基类。
	 *
	 * 终止策略定义了用于停止优化过程的逻辑。 可以很简单，例如达到
	 * 最大进化代数时退出的进化代数计数器，也可以实现更复杂的算法，如
	 * 通过确定何时达到合理的最佳值来尝试最小化执行步骤的数量。
	 *
	 * @author louiehan (11/15/2019)
	 */
	class termination_strategy
	{
	public:
		virtual ~termination_strategy() {}

		/**
		 * 优化进程终止判定事件
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @param best 当前最优个体
		 * @param genCount 进化代数
		 *
		 * @return bool return true to continue, or false to stop the
		 *  	   optimization process
		 */
		virtual bool event(individual_ptr best, size_t genCount) = 0;
	};

	/**
	 * A smart pointer to a TerminationStrategy
	 */
	typedef std::shared_ptr< termination_strategy > termination_strategy_ptr;

	/**
	 * 终止策略的基本实现：如果已进化到最大代数，则停止优化过程
	 *
	 * @author louiehan (11/15/2019)
	 */
	class max_gen_termination_strategy : public termination_strategy
	{
	private:
		const size_t m_maxGen;
	public:
		/**
		 * constructs a max_gen_termination_strategy object
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @param maxGen maximum number of generations after which the
		 *  			 optimization stops
		 */
		max_gen_termination_strategy(size_t maxGen)
			: m_maxGen(maxGen)
		{
		}

		virtual bool event(individual_ptr best, size_t genCount)
		{
			return genCount < m_maxGen;
		}
	};

}

#endif //DE_TERMINATION_STRATEGY_HPP_INCLUDED
