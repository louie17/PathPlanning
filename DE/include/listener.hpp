// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef DE_LISTENER_HPP_INCLUDED
#define DE_LISTENER_HPP_INCLUDED

namespace de
{

	/**
	 * 用户定义的侦听器的抽象基类。
	 *
	 * 在优化会话期间发生某些重要事件时，侦听器类将接收通知。
	 *
	 * 这些事件可用于显示当前状态，诊断信息等。
	 *
	 * 用户将从此类中派生具体的侦听器类，这将对接收到的事件做一些有用的事情。
	 *
	 * @author louiehan (11/11/2019)
	 */
	class listener
	{
	public:
		virtual ~listener() {}

		/**
		 * 在优化过程开始时调用。
		 *
		 * @author louiehan (11/11/2019)
		 */
		virtual void start() = 0;
		/**
		 * 在优化过程结束时调用。
		 *
		 * 即使优化过程以异常结束，也会调用此函数
		 *
		 * @author louiehan (11/11/2019)
		 */
		virtual void end() = 0;
		/**
		 * 在优化过程中引发异常时调用，并发出错误信号
		 *
		 * @author louiehan (11/11/2019)
		 */
		virtual void error() = 0;
		/**
		 * 在每一代开始时调用。
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param genCount
		 */
		virtual void startGeneration(size_t genCount) = 0;
		/**
		 *在每一代结束时调用，除非在到达迭代结束之前引发异常。
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param genCount
		 * @param bestIndGen
		 * @param bestInd
		 */
		virtual void endGeneration(size_t genCount, individual_ptr bestIndGen, individual_ptr bestInd) = 0;
		/**
		 * 在选择开始之前调用。
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param genCount
		 */
		virtual void startSelection(size_t genCount) = 0;
		/**
		 * 执行选择后调用。
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param genCount
		 */
		virtual void endSelection(size_t genCount) = 0;
		/**
		 * 在开始目标函数处理之前调用。
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param genCount
		 */
		virtual void startProcessors(size_t genCount) = 0;
		/**
		 * 目标函数处理完成后调用。
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param genCount
		 */
		virtual void endProcessors(size_t genCount) = 0;
	};

	/**
	 * 指向侦听器的智能指针
	 */
	typedef std::shared_ptr< listener > listener_ptr;

	/**
	 * 忽略所有接收事件的具体侦听器
	 *
	 * @author louiehan (11/11/2019)
	 */
	class null_listener : public listener
	{
	public:
		virtual void start() {}
		virtual void end() {}
		virtual void error() {}
		virtual void startGeneration(size_t genCount) {}
		virtual void endGeneration(size_t genCount, individual_ptr bestIndGen, individual_ptr bestInd)
		{
			std::cout << "genCount:" << genCount << ", cost: " << bestInd->cost() << std::endl;
		}
		virtual void startSelection(size_t genCount) {}
		virtual void endSelection(size_t genCount) {}
		virtual void startProcessors(size_t genCount) {}
		virtual void endProcessors(size_t genCount) {}
	};

}

#endif //DE_LISTENER_HPP_INCLUDED
