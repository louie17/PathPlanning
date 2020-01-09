#ifndef DE_PROCESSORS_HPP_INCLUDED
#define DE_PROCESSORS_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <queue>
#include <functional>   // std::ref

#include "boost_noncopyable.hpp"
#include "boost_threadgroup.hpp"
#include "individual.hpp"
#include "population.hpp"
#include "Scenario.hpp"

namespace de
{

	/**
	 * 基于抽象的类，用于从处理器接收事件的处理器侦听器。
	 *
	 * 由于其方法是从多个线程调用的，因此具体类必须使用线程同步对象以避免数据损坏。
	 *
	 * @author louiehan (11/11/2019)
	 */
	class processor_listener
	{
	public:
		virtual ~processor_listener() {}
		/**
		 * 在运行目标函数的处理器operator（）的开头调用
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param index 处理器索引
		 */
		virtual void start(size_t index) = 0;

		/**
		 * 在使用当前个体的变量运行目标函数之前调用
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param index 处理器索引
		 * @param individual 运行目标函数的当前个体
		 */
		virtual void start_of(size_t index, individual_ptr individual) = 0;

		/**
		 * 在使用当前个体的变量运行目标函数后调用。作为参数传递的个体还具有目标函数运行结果的成本集。
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param index 处理器索引
		 * @param individual 目标函数运行的包含代价的当前个体。
		 */
		virtual void end_of(size_t index, individual_ptr individual) = 0;

		/**
		 * 在运行目标函数的处理器运算符（）的末尾调用。
		 *
		 * 即使抛出异常，也会调用此函数。
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param index the processor index
		 */
		virtual void end(size_t index) = 0;

		/**
		 * 如果在目标函数运行期间引发异常并指示错误，则调用。
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param index 处理器索引
		 * @param message 描述错误信息的消息
		 */
		virtual void error(size_t index, const std::string& message) = 0;

	};

	/**
	 * 不执行任何操作的处理器侦听器的基本实现。
	 *
	 * 不需要同步，因为没有数据被读取或修改。
	 *
	 * @author louiehan (11/11/2019)
	 */
	class null_processor_listener : public processor_listener
	{
	public:
		/**
		 * 在运行目标函数的处理器operator（）的开头调用。
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param index 处理器索引
		 */
		virtual void start(size_t index)
		{
			std::cout << "Processor index:" << index << "start running!" << std::endl;
		}

		/**
		 * 在使用当前个体的变量运行目标函数之前调用。
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param index 处理器的索引
		 * @param individual 目标函数运行的当前个体
		 */
		virtual void start_of(size_t index, individual_ptr individual)
		{
			std::cout << "Processor index:" << index<<", Past best individual cost:"<<individual->cost()<<std::endl;
		}

		/**
		 * 使用当前个体的变量运行目标函数后调用。 作为参数传递的个体也将代价设定为目标函数运行的结果。
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param index 处理器索引
		 * @param individual 目标函数所依据的包含代价的当前个体。
		 */
		virtual void end_of(size_t index, individual_ptr individual)
		{
			std::cout << "Processor index:" << index << ", Current best individual cost:" << individual->cost() << std::endl;
		}

		/**
		 * 在运行目标函数的处理器operator（）的末尾调用。
		 *
		 * 即使有异常抛出也会被调用。
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param index 处理器索引
		 */
		virtual void end(size_t index)
		{
			std::cout << "Processor index:" << index << "exit!"<< std::endl;
		}

		/**
		 * 如果在目标函数运行期间引发异常并指示错误，则调用。
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param index 处理器索引
		 * @param message 描述错误信息的消息
		 */
		virtual void error(size_t index, const std::string& message)
		{
			std::cout << "Processor index:" << index << "; Error:" << message << std::endl;
		}
	};


	/**
	 *指向处理器侦听器的智能指针
	 */
	typedef std::shared_ptr< processor_listener > processor_listener_ptr;

	/**
	 * 如果目标函数出错，则抛出异常。
	 *
	 * @author louiehan (11/11/2019)
	 */
	class objective_function_exception : public exception
	{
	public:
		/**
		 * 构造一个objective_function_exception对象
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param message 描述引起异常的错误信息
		 */
		objective_function_exception(const std::string& message)
			: exception(message.c_str())
		{
		}
	};

	/**
	 * 目标函数工厂的接口。如果目标函数要求将不同的实例传递给每个处理器，
	 * 请创建一个派生自此类的具体的object_function_factory，并实现
	 * 虚make方法以创建目标函数的新实例。
	 *
	 * 使用对object_function_exception的引用或shared_ptr作为处理
	 * 器模板参数，并将相应的对象作为构造函数参数传递。
	 *
	 * 上面相应的处理器特征类将确保应用正确的行为。
	 *
	 * 模板参数T是目标函数的类型。
	 *
	 * @author louiehan (11/11/2019)
	 */
	template< typename T >class objective_function_factory
	{
	public:
		/**
		 * 定义指向目标函数的类型指针
		 */
		typedef std::shared_ptr< T > T_ptr;

		/**
		 * 虚析构函数
		 *
		 * @author louiehan (11/11/2019)
		 */
		virtual ~objective_function_factory() {}

		/**
		 *在派生类中实现的方法将创建目标函数的新实例。
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return T_ptr 目标函数的智能指针
		 */
		virtual T_ptr make() = 0;
	};

	/**
	 * 目标函数通过引用传递并复制的情况下的基本处理器特征类
	 *
	 * @author louiehan (11/11/2019)
	 */
	template< typename T >class processor_traits
	{
	public:
		// \cond
		typedef T  value_type;
		static double run(T t, de::NVectorPtr vars) { return t(vars); }
		static double run(T t, de::NVectorPtr vars, constraints_ptr constraints, sce::Site_WeaponRange_relation swRel) { return t(vars,constraints,swRel);}
		static T make(T t) { return t; }
		// \endcond
	};

	/**
	 * 目标函数作为指针传递的情况下的专用处理器特征类
	 *
	 * @author louiehan (11/11/2019)
	 */
	template< typename T > class processor_traits< T* >
	{
	public:
		// \cond
		typedef T*  value_type;
		static double run(value_type t, de::NVectorPtr vars) { return (*t)(vars); }
		static double run(value_type t, de::NVectorPtr vars, de::constraints_ptr constraints, sce::Site_WeaponRange_relation swRel) { return (*t)(vars, constraints, swRel); }
		static value_type make(value_type t) { return t; }
		// \endcond
	};

	/**
	 * 当目标函数作为共享指针传递时的特殊处理器特征类。
	 *
	 * @author louiehan (11/11/2019)
	 */
	template< typename T > class processor_traits< std::shared_ptr< T > >
	{
	public:
		// \cond
		typedef std::shared_ptr< T > value_type;
		static double run(value_type t, de::NVectorPtr vars) { return (*t)(vars); }
		static double run(value_type t, de::NVectorPtr vars, de::constraints_ptr constraints, sce::Site_WeaponRange_relation swRel) { return (*t)(vars, constraints, swRel); }
		static value_type make(value_type t) { return t; }
		// \endcond
	};

	/**
	 * 处理器接收到目标函数工厂的指针的情况下的专用处理器特征类。
	 *
	 * @author louiehan (11/11/2019)
	 */
	template< typename T > class processor_traits< objective_function_factory< T >* >
	{
	public:
		// \cond
		typedef std::shared_ptr< T > value_type;
		static double run(value_type t, de::NVectorPtr vars) { return (*t)(vars); }
		static double run(value_type t, de::NVectorPtr vars, de::constraints_ptr constraints, sce::Site_WeaponRange_relation swRel) { return (*t)(vars, constraints, swRel); }
		static value_type make(objective_function_factory< T >* off) { return off->make(); }
		// \endcond
	};
	/**
	 * 处理器接收到目标函数工厂的共享指针的情况下的专用处理器特征类。
	 *
	 * @author louiehan (11/11/2019)
	 */
	template< typename T > class processor_traits< std::shared_ptr< objective_function_factory< T > > >
	{
	public:
		// \cond
		typedef std::shared_ptr< T > value_type;
		static double run(value_type t, de::NVectorPtr vars) { return (*t)(vars); }
		static double run(value_type t, de::NVectorPtr vars, de::constraints_ptr constraints, sce::Site_WeaponRange_relation swRel) { return (*t)(vars, constraints, swRel); }
		static value_type make(std::shared_ptr< objective_function_factory< T > > off) { return off->make(); }
		// \endcond
	};

	/**
	 * 处理器接收到目标函数工厂的引用的情况下的专用处理器特征类。
	 *
	 * @author louiehan (11/11/2019)
	 */
	template< typename T > class processor_traits< objective_function_factory< T >& >
	{
	public:
		// \cond
		typedef std::shared_ptr< T > value_type;
		static double run(value_type t, de::NVectorPtr vars) { return (*t)(vars); }
		static double run(value_type t, de::NVectorPtr vars, de::constraints_ptr constraints, sce::Site_WeaponRange_relation swRel) { return (*t)(vars, constraints, swRel); }
		static value_type make(objective_function_factory< T >& off) { return off.make(); }
		// \endcond
	};

	/**
	 * 处理器在一个线程中运行目标函数。 可以有许多线程在任意多个线程中并行运行目标函数。
	 *
	 * 处理器类使用在相应的processor_traits中定义的目标函数的类型。
	 *
	 * @author louiehan (11/11/2019)
	 */
	template < typename T > class processor : boost::noncopyable
	{
	private:
		typename processor_traits< T >::value_type m_of;
		individual_queue& m_indQueue;
		processor_listener_ptr m_listener;
		size_t m_index;

		constraints_ptr m_constraints;
		sce::Site_WeaponRange_relation m_swRealation;


		bool m_result;

	public:
		/**
		 * 处理器对象构造函数
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param index 处理器索引
		 * @param of 目标函数，或目标函数工厂。接受指针、共享指针、引用
		 * @param indQueue 包含要处理的个体的队列
		 * @param listener 在处理目标函数期间将接收重要事件通知的侦听器。
		 */
		processor(size_t index, T of, individual_queue& indQueue, processor_listener_ptr listener)
			: m_of(processor_traits< T >::make(of)), m_indQueue(indQueue), m_result(false), m_listener(listener), m_index(index)
		{
			assert(listener);
		}

		/**
		* 处理器对象构造函数
		*
		* @author louiehan (12/11/2019)
		*
		* @param index 处理器索引
		* @param of 目标函数，或目标函数工厂。接受指针、共享指针、引用
		* @param constraints 各参数约束集合
		* @param swRealation 场景信息中的Site-Weapon Relation关系表
		* @param indQueue 包含要处理的个体的队列
		* @param listener 在处理目标函数期间将接收重要事件通知的侦听器。
		*/
		processor(size_t index, T of, individual_queue& indQueue, constraints_ptr constraints, sce::Site_WeaponRange_relation swRealation, processor_listener_ptr listener)
			:m_of(processor_traits< T >::make(of)), m_indQueue(indQueue), m_constraints(constraints), m_swRealation(swRealation), m_result(false), m_listener(listener), m_index(index)
		{
			assert(listener);
		}

		/**
		 * 对队列顶部的对象（如果有）运行目标函数
		 *
		 * @author louiehan (11/11/2019)
		 */
		void operator()()
		{
			m_listener->start(m_index);
			m_result = false;
			try
			{
				for (individual_ptr ind = m_indQueue.pop(); ind; ind = m_indQueue.pop())
				{
					m_listener->start_of(m_index, ind);
					//double result = processor_traits< T >::run(m_of, ind->vars());
					double result = processor_traits< T >::run(m_of, ind->vars(),m_constraints,m_swRealation);
					ind->setCost(result);
					m_listener->end_of(m_index, ind);
				}
				m_result = true;

				//BOOST_SCOPE_EXIT_TPL( (&m_index) (&m_listener) )
				//{
				m_listener->end(m_index);
				//} 
				//BOOST_SCOPE_EXIT_END
			}
			catch (const objective_function_exception& e)
			{
				m_result = false;
				m_listener->error(m_index, e.what());
			}
		}

		/**
		 * 指示线程退出时运行是否成功结束
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return bool
		 */
		bool success() const { return m_result; }
	};



	/**
	 * 发生处理器错误时抛出异常。
	 *
	 * @author louiehan (11/11/2019)
	 */
	class processors_exception : exception
	{
	public:
		/**
		 * 构造函数，以消息字符串作为参数
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @param message
		 */
		processors_exception(const std::string& message)
			: exception(message.c_str())
		{
		}
	};


	/**
	 * 处理器集合
	 *
	 * 此类在优化会话期间启动并协调各种处理器。
	 *
	 * 将目标函数或目标函数工厂的类型作为参数（引用，指针或shared_ptr）。
	 *
	 * @author louiehan (11/15/2019)
	 */
	template< typename T > class processors
	{
	private:
		typedef std::shared_ptr< boost::thread_group > thread_group_ptr;
		typedef std::shared_ptr< processor< T > > processor_ptr;
		typedef std::vector< processor_ptr > processor_vector;
		typedef std::shared_ptr< T > T_ptr;

	private:
		individual_queue m_indQueue;
		processor_vector m_processors;
		thread_group_ptr m_threads;

	public:
		/**
		 * 构造一个处理器对象，然后使用提供的目标函数构造“计数”处理器
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @param count 创建的处理器数量
		 * @param of 目标函数或目标函数工厂
		 * @param listener 传递给每个创建的处理器的侦听器
		 */
		processors(size_t count, T of, processor_listener_ptr listener)
		{
			assert(count > 0);
			assert(listener);

			for (size_t n = 0; n < count; ++n)
			{
				processor_ptr processor(std::make_shared< processor< T > >(n, of, std::ref(m_indQueue), listener));
				m_processors.push_back(processors< T >::processor_ptr(processor));
			}
		}

		/**
		* 构造一个处理器对象，然后使用提供的目标函数构造“计数”处理器
		*
		* @author louiehan (11/15/2019)
		*
		* @param count 创建的处理器数量
		* @param of 目标函数或目标函数工厂
		* @param constraints 各参数约束集合
		* @param swRealation 场景信息中的Site - Weapon Relation关系表
		* @param listener 传递给每个创建的处理器的侦听器
		*/		
		processors(size_t count, T of, constraints_ptr constraints, sce::Site_WeaponRange_relation swRealation, processor_listener_ptr listener)
		{
			assert(count > 0);
			assert(listener);

			for (size_t n = 0; n < count; ++n)
			{
				processor_ptr processor(std::make_shared< processor< T > >(n, of, std::ref(m_indQueue), constraints, swRealation, listener));
				m_processors.push_back(processors< T >::processor_ptr(processor));
			}
		}

		/**
		 * 将个体推入处理队列的底部。
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @param ind
		 */
		void push(individual_ptr ind) { m_indQueue.push(ind); }
		/**
		 * 异步启动所有处理器线程（它不会等待它们完成）。
		 *
		 * @author louiehan (11/15/2019)
		 */
		void start()
		{
			// 每次创建一个新线程组，不需要费心删除所有单独的线程
			m_threads = std::make_shared< boost::thread_group >();

			for (typename processor_vector::size_type n = 0; n < m_processors.size(); ++n)
			{
				processor_ptr p(m_processors[n]);
				std::thread* th(new std::thread(std::ref(*p)));//new thread()导致m_indiQuen减少一个
				m_threads->add_thread(th);

			}
		}

		/**
		 * 再返回之前等待所有处理器结束。
		 *
		 * 用于同步处理。
		 *
		 * @author louiehan (11/15/2019)
		 */
		void wait()
		{
			m_threads->join_all();

			if (!m_indQueue.empty())
				throw processors_exception("threads ended before emptying the queue");

			if (!success())
				throw processors_exception("objective function error");
		}

		/**
		 * 指示所有处理器是否成功结束。
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @return bool true if success, false if an error occured
		 */
		bool success()
		{
			for (typename processor_vector::size_type n = 0; n < m_processors.size(); ++n)
			{
				processor_ptr processor(m_processors[n]);
				if (!processor->success())
					return false;
			}

			return true;
		}

		/**
		 * 将群体中的所有个体推入处理队列。
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @param population
		 */
		void push(population_ptr population)
		{
			std::copy(population->begin(), population->end(), std::back_inserter(m_indQueue));
		}

		/**
		 * 指向处理器集合的智能指针。
		 */
		typedef std::shared_ptr< processors< T > > processors_ptr;

	};

}

#endif //DE_PROCESSORS_HPP_INCLUDED
