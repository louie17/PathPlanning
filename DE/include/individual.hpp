// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef DE_INDIVIDUAL_HPP_INCLUDED
#define DE_INDIVIDUAL_HPP_INCLUDED

#include <queue>
#include <sstream>

#include "de_types.hpp"
#include "de_constraints.hpp"
#include "multithread.hpp"

namespace de
{
	class individual;
	typedef std::shared_ptr< individual > individual_ptr;

	/**
	 * 差分进化种群中的个体类。
	 *
	 * 每个个体有一组变量和与这些变量相关的代价。
	 *
	 * 每个个体是线程安全的。
	 *
	 * @author louiehan (11/11/2019)
	 */
	class individual
	{
	private:
		de::NVectorPtr m_vars;
		double m_cost;
		de::mutex m_mx;

	public:
		/**
		 * 个体对象构造函数
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param varCount 每个个体的变量的数量
		 */
		individual(size_t varCount)
			: m_vars(std::make_shared< de::NVector >(varCount)),m_cost(0.0)
		{
		}

		/**
		 * 个体对象构造函数
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param vars 将被复制到内部变量vector容器中的变量vector容器对象。
		 */
		individual(const de::NVector& vars)
			: m_vars(std::make_shared< de::NVector >(vars)), m_cost(0.0)
		{
		}

		/**
		 * 使用约束集合内的随机值初始化变量的内部vector容器对象。
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param constraints
		 */
		void init(constraints_ptr constraints)
		{
			assert(constraints);
			assert(m_vars);
			assert(m_vars->size() == constraints->size());

			for (de::NVector::size_type j = 0; j < m_vars->size(); ++j)
			{
				(*m_vars)[j].setAltitude(constraints->get_rand_value(0));
				(*m_vars)[j].setLatitude(constraints->get_rand_value(1));
				(*m_vars)[j].setLongitude(constraints->get_rand_value(2));
			}
				
		}

		/**
		* 使用约束集合内的随机值初始化变量的内部vector容器对象。
		*
		* @author louiehan (11/11/2019)
		*
		* @param constraints
		* @param startPoint,endPoint 路径起始与终止点
		*/
		void init(const Node &startPoint, const Node &endPoint, constraints_ptr constraints)
		{
			assert(constraints);
			assert(m_vars);
			assert(m_vars->size() == constraints->size());

			m_vars->at(0) = startPoint;
			for (de::NVector::size_type j = 1; j < m_vars->size()-1; ++j)
			{
				m_vars->at(j) = de::Node(constraints->get_rand_value(0), constraints->get_rand_value(1), constraints->get_rand_value(2));
			}
			//m_vars->insert(m_vars->begin(), de::Node(startPoint.getAltitude(), startPoint.getLatitude(), startPoint.getLongitude()));
			//m_vars->insert(m_vars->end(), de::Node(endPoint.getAltitude(), endPoint.getLatitude(), endPoint.getLongitude()));
			m_vars->at(m_vars->size() - 1) = endPoint;
		}

		/**
		 * 返回个体的代价值
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return double
		 */
		double cost() const { return m_cost; }

		/**
		 * 使用原点和旧值将变量设置为约束内的新随机值。
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param constraints
		 * @param origin
		 */
		void ensureConstraints(constraints_ptr constraints, de::NVectorPtr origin)
		{
			assert(constraints);
			assert(m_vars);
			assert(origin);
			//assert(m_vars->size() == constraints->size());

			for (de::NVector::size_type j = 0; j < m_vars->size(); ++j)
			{
				(*m_vars)[j].setAltitude(constraints->get_rand_value(2, (*m_vars)[j].altitude(), (*origin)[j].altitude()));
				(*m_vars)[j].setLatitude(constraints->get_rand_value(1, (*m_vars)[j].latitude(), (*origin)[j].latitude()));
				(*m_vars)[j].setLongitude(constraints->get_rand_value(0, (*m_vars)[j].longitude(), (*origin)[j].longitude()));
			}
		}

		/**
		 * returns the internal variable vector
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return de::NVectorPtr
		 */
		de::NVectorPtr vars() const { return m_vars; }

		/**
		 * 根据索引返回对变量值的非常量引用，该索引可用作左值。
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param index 要返回其引用的变量的索引
		 *
		 * @return de::Node&
		 */
		de::Node& operator[](size_t index) { return (*m_vars)[index]; }

		/**
		 * 根据索引返回对变量值的常量引用，该索引可用作左值。
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param index 要返回其引用的变量的索引
		 *
		 * @return de::Node&
		 */
		const de::Node& operator[](size_t index) const { return (*m_vars)[index]; }

		/**
		 * 设定代价
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param cost
		 */
		void setCost(double cost) { m_cost = cost; }

		/**
		 * 根据代价将当前个体与另一个个体进行比较。如果当前代价低于或等于另一个个体的代价，则返回true。
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param ind 要比较的个体
		 *
		 * @return bool
		 */
		bool operator<=(const individual& ind) const
		{
			assert(ind.size() == size());
			return cost() <= ind.cost();
		}

		/**
		 * 根据代价将当前个体与另一个个体进行比较。如果当前代价严格低于另一个个体的代价，则返回true。
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param ind 要比较的个体
		 *
		 * @return bool
		 */
		bool operator<(const individual& ind) const
		{
			assert(ind.size() == size());
			return cost() < ind.cost();
		}

		/**
		 * 根据代价将当前个体与另一个个体进行比较。如果当前代价优于或等于另一个个体的代价，
		 * 则返回true，优的标准是小于还是大于取决于minimize标志是更低还是更高。
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param ind 要比较的个体
		 * @param minimize <= if true, >= if false
		 *
		 * @return bool
		 */
		bool better_or_equal(const individual_ptr ind, bool minimize) const
		{
			assert(ind);
			return minimize ? *this <= *ind : *ind <= *this;
		}

		/**
		 * 根据代价将当前个体与另一个个体进行比较。如果当前代价严格优于另一个个体的代价，
		 * 则返回true，优的标准是小于还是大于取决于minimize标志是更低还是更高。
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param ind 要比较的个体
		 * @param minimize < if true, > if false
		 *
		 * @return bool
		 */
		bool better(const individual_ptr ind, bool minimize) const
		{
			assert(ind);
			return minimize ? *this < *ind : *ind < *this;
		}

		/**
		 * 返回vector容器内变量的个数
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return size_t
		 */
		size_t size() const { return m_vars->size(); }

		/**
		 * 以字符串的形式返回个体内部的（代价和节点列表）。
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return std::string
		 */
		std::string to_string() const
		{
			std::ostringstream os;

			os << "cost: " << cost() << ", Nodes: ";

			for (de::NVector::size_type j = 0; j < m_vars->size(); ++j)
			{
				os <<"["<< (*m_vars)[j].altitude() << ", "<< (*m_vars)[j].latitude()<<", "<< (*m_vars)[j].longitude()<< "],";
			}

			return os.str();

		}
	};

	typedef std::queue< individual_ptr > individual_queue_base;

	/**
	 * 线程安全的个体队列
	 *
	 * 用于将包含要由不同处理器传递给目标函数的参数的个体排队。
	 *
	 * @author louiehan (11/11/2019)
	 */
	class individual_queue : public individual_queue_base
	{
	private:
		de::mutex m_mx;

	public:
		/**
		 * 在队列底部添加新的个体。
		 *
		 * 数线程安全
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param ind 要插入队列中的个体
		 */
		void push_back(individual_ptr ind)
		{
			de::lock lock(m_mx);

			individual_queue_base::push(ind);
		}

		/**
		 * 从队列顶部删除个体（如果有）并返回。
		 *
		 * 线程安全
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return individual_ptr 队列顶部的个体；如果队列为空，则为null
		 */
		individual_ptr pop()
		{
			de::lock lock(m_mx);

			if (!individual_queue_base::empty())
			{
				individual_ptr p(individual_queue_base::front());

				individual_queue_base::pop();

				return p;
			}
			else
				return individual_ptr();
		}
	};

}

#endif //DE_INDIVIDUAL_HPP_INCLUDED
