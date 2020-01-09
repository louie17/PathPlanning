// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef DE_CONSTRAINTS_HPP_INCLUDED
#define DE_CONSTRAINTS_HPP_INCLUDED

#include <set>
#include <memory>
#include <cassert>
#include <cmath>
#include <iterator>
#include <string>
#include <sstream>
#include <cctype>
#include <algorithm>

#include "random_generator.hpp"
#include "de_types.hpp"
#include "ud_string.hpp"

namespace de
{

	/**
	 * 在约束错误的情况下抛出异常
	 *
	 * @author louiehan (11/11/2019)
	 */
	class constraints_exception : public exception
	{
	public:
		/**
		 * 将错误消息作为参数的构造函数
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param message
		 */
		constraints_exception(const std::string& message)
			: exception(message.c_str())
		{
		}
	};

	/**
	 * 具体约束类的抽象基类。
	 * 一个约束类描述了输入变量对目标函数的某些特性和限制。
	 *
	 * @author louiehan (11/11/2019)
	 */
	class constraint
	{
	public:
		virtual ~constraint() {}

		/**
		 * 返回一个限制在约束类型和范围内的随机值。
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return double
		 */
		virtual double get_rand_value() = 0;

		/**
		 * 根据先前的值和原点返回一个限制在约束的类型和范围内的随机值(参见派生类中的具体实现)
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param value
		 * @param origin
		 *
		 * @return double
		 */
		virtual double get_rand_value(double value, double origin) = 0;

		/**
		 * 返回范围的最小限制。
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return double
		 */
		virtual double min() const = 0;

		/**
		 * 返回范围的最大限制。
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return double
		 */
		virtual double max() const = 0;

		/**
		 * 获取在为约束设置的限制内的随机值，但进一步限制在由其原点和该原点周围区域的宽度（以总宽度的百分比）定义的范围内。
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param origin 进一步限制了约束区域的原点（中心）
		 * @param zonePct 围绕原点的区域宽度，以总宽度的百分比表示
		 *
		 * @return double
		 */
		virtual double get_rand_value_in_zone(double origin, double zonePct) const = 0;

		/**
		 * 获取介于最小值和最大值之间的点-仅适用于范围约束
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return double
		 */
		virtual double get_middle_point() = 0;
	};

	/**
	 * 指向某条约束的智能指针
	 */
	typedef std::shared_ptr< constraint > constraint_ptr;

	/**
	 * 基于范围的约束的基类。 每个这样的约束都有一个最小值和最大值
	 *
	 * @author louiehan (11/11/2019)
	 */
	class range_constraint : public constraint
	{
	private:
		double m_min;
		double m_max;

	public:

		/**
		 * 构造函数，范围最小和最大限制
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param min
		 * @param max
		 */
		range_constraint(double min, double max)
			: m_min(min), m_max(max)
		{
			assert(min <= max);
		}

		/**
		 * 返回范围的最小限制
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return double
		 */
		double min() const { return m_min; }

		/**
		 * 返回范围的最大限制
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return double
		 */
		double max() const { return m_max; }

	};

	/**
	 * 实数限制类。指定变量在指定的限制内可以有任何双精度值。
	 *
	 * @author louiehan (11/11/2019)
	 */
	class real_constraint : public range_constraint
	{
	public:

		/**
		 * 构造函数，范围最大最小值限定
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param min
		 * @param max
		 */
		real_constraint(double min, double max)
			: range_constraint(min, max)
		{
			assert(min <= max);
		}

		/**
		 * 返回一个受限于约束类型和范围的随机值
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return double
		 */
		double get_rand_value()
		{
			return genrand(range_constraint::min(), range_constraint::max());
		}

		/**
		 * 根据先前的值和原点，返回限制在约束类型和范围内的随机值
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param value
		 * @param origin
		 *
		 * @return double
		 */
		double get_rand_value(double value, double origin)
		{
			double ret = value;

			while (ret < range_constraint::min())
			{
				ret = range_constraint::min() + genrand() * (origin - range_constraint::min());
			}

			while (ret > range_constraint::max())
			{
				ret = range_constraint::max() + genrand() * (origin - range_constraint::max());
			}

			return ret;
		}

		virtual double get_rand_value_in_zone(double origin, double zonePct) const
		{
			if (origin > max())
				throw constraints_exception("origin coordinate > max");
			if (origin < min())
				throw constraints_exception("origin coordinate < min");

			if (zonePct > 100.0)
				throw constraints_exception("zonePct > 100%");

			if (zonePct < 0)
				throw constraints_exception("zonePct < 0%");

			if (zonePct == 0)
				throw constraints_exception("zonePct == 0%");

			double zoneSize = (max() - min()) * zonePct / 100.0;

			double _min = std::max(min(), origin - zoneSize / 2.0);
			double _max = std::min(max(), origin + zoneSize / 2.0);

			return genrand(_min, _max);
		}

		virtual double get_middle_point()
		{
			return (max() + min()) / 2.0;
		}

	};

	/**
	 * 整数约束。 指定变量可以具有指定限制内的任何整数值。
	 *
	 * @author louiehan (11/11/2019)
	 */
	class int_constraint : public range_constraint
	{
	public:
		/**
		 * 构造函数，采用整数约束的最小和最大限制
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param min
		 * @param max
		 */
		int_constraint(double min, double max)
			: range_constraint(min, max)
		{
			assert(min <= max);
		}

		/**
		 * 返回一个受限于约束类型和范围的随机值
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return double
		 */
		double get_rand_value()
		{
			return genintrand(range_constraint::min(), range_constraint::max());
		}

		/**
		 * 根据先前的值和原点，返回限制在约束类型和范围内的随机值
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param value
		 * @param origin
		 *
		 * @return double
		 */
		double get_rand_value(double value, double origin)
		{
			double ret = round(value);

			while (ret < range_constraint::min())
			{
				ret = range_constraint::min() + genrand() * (origin - range_constraint::min());
				ret = round(ret);
			}

			while (ret > range_constraint::max())
			{
				ret = range_constraint::max() + genrand() * (origin - range_constraint::max());
				ret = round(ret);
			}

			return ret;
		}

		virtual double get_rand_value_in_zone(double origin, double zonePct) const
		{
			if (origin > max())
				throw constraints_exception("origin coordinate > max");
			if (origin < min())
				throw constraints_exception("origin coordinate < min");

			if (zonePct > 100.0)
				throw constraints_exception("zonePct > 100%");

			if (zonePct < 0)
				throw constraints_exception("zonePct < 0%");

			if (zonePct == 0)
				throw constraints_exception("zonePct == 0%");

			double zoneSize = (max() - min()) * zonePct / 100.0;

			double _min = std::max(min(), origin - zoneSize / 2.0);
			double _max = std::min(max(), origin + zoneSize / 2.0);

			double val = round(genrand(_min, _max));

			for (; val < _min || val > _max; val = round(genrand(_min, _max)));

			return val;
		}

		virtual double get_middle_point()
		{
			return round((max() - min()) / 2.0);
		}


	};

	/**
	 * 约束集合。 指定变量可以采用预定义集合中的任何值。不需要最小或最大。
	 * 请注意，重复值将被删除。
	 *
	 * @author louiehan (11/11/2019)
	 */
	class set_constraint : public constraint
	{
	private:
		class unique : public std::unary_function < Double, bool >
		{
		public:
			bool operator()(Double d) const
			{
				return m_unique.insert(d).second;
			}

		public:
			double min() const
			{
				if (m_unique.size() > 0)
					return *m_unique.begin();
				else
					throw constraints_exception("could not get the min value of an empty set constraint");
			}

			double max() const
			{
				if (m_unique.size() > 0)
					return *m_unique.rbegin();
				else
					throw constraints_exception("could not get the max value of an empty set constraint");
			}

		private:
			mutable std::set< Double > m_unique;
		};
	private:
		unique m_unique;
		de::DVector m_values;

	public:
		/**
		 * 根据Double值的vector对象构造集合，并删除重复项以确保随机分配的均匀分布
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param values
		 */
		set_constraint(const de::DVector& values)
		{
			// 确保“集合”中的值是唯一的
			std::remove_copy_if(values.begin(), values.end(), std::back_inserter(m_values), m_unique);
		}

		/**
		 * 将一个单独的值添加到“集合”中，不会创建重复的值。
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param value
		 */
		void add_value(de::Double value)
		{
			if (m_unique(value))
				m_values.push_back(value);
		}

		/**
		 * 返回从可用值集合中随机选择的值
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return double
		 */
		virtual double get_rand_value()
		{
			de::DVector::size_type index(genintrand(0, m_values.size() - 1));

			return m_values[index];
		}

		/**
		 * 返回从可用值集中随机选择的值
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return double
		 */
		virtual double get_rand_value(double value, double origin)
		{
			return get_rand_value();
		}

		/**
		 * 返回范围的最小限制
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return double
		 */
		double min() const
		{
			return m_unique.min();
		}

		/**
		 * 返回范围的最大限制
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return double
		 */
		double max() const
		{
			return m_unique.max();
		}

		virtual double get_rand_value_in_zone(double origin, double zonePct) const
		{
			throw constraints_exception("get_rand_value_in_zone only supported for range constraints");
		}

		virtual double get_middle_point()
		{
			throw constraints_exception("get_middle_point not supported by set constraint");
		}

	};

	/**
	 * 布尔约束。 指定变量可以采用布尔值-true或false。
	 *
	 * @author louiehan (11/11/2019)
	 */
	class boolean_constraint : public constraint
	{
	public:
		/**
		 * 返回一个随机布尔值
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return double
		 */
		virtual double get_rand_value()
		{
			return genrand() < 0.5;
		}

		/**
		 * 返回一个随机布尔值
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return double
		 */
		virtual double get_rand_value(double value, double origin)
		{
			return get_rand_value();
		}

		/**
		 * 返回范围的最小限制
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return double
		 */
		double min() const
		{
			return 0;
		}

		/**
		 * 返回范围的最大限制
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return double
		 */
		double max() const
		{
			return 1;
		}

		virtual double get_rand_value_in_zone(double origin, double zonePct) const
		{
			throw constraints_exception("get_rand_value_in_zone only supported for range constraints");
		}

		virtual double get_middle_point()
		{
			throw constraints_exception("get_middle_point not supported by bool constraint");
		}

	};

	typedef std::vector< constraint_ptr > constraints_base;

	/**
	 * 约束条件的集合，用vector容器实现。用于定义优化进程期间使用的所有变量的约束。
	 *
	 * @author louiehan (11/11/2019)
	 */
	class constraints : public constraints_base
	{
	private:

	public:
		/**
		 * 使用默认值初始化一个约束集合，所有的约束是实数类型，且有相同的默认大小
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param varCount 约束的个数
		 * @param defMin 默认最小值
		 * @param defMax 默认最大值
		 */
		constraints(size_t varCount, double defMin, double defMax)
			: constraints_base(varCount, std::make_shared< real_constraint >(defMin, defMax))
		{
		}

		/**
		 * 从字符串描述中初始化一个约束集合，目前只适用于基于范围的约束；
		 * 一条约束可以被描述为："type;min;max" 其中type可以是实数或整数类型，min，max是上下限。
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param str 一个约束的描述字符串集合，每个字符串描述一个变量；
		 * @param var_count 变量的个数 (可以不等于字符串的个数).如果变量的个数输入的字符串多，
		 *  			   则多余的约束被设定为实数类型，并使用默认的大小值来限定范围；
		 * @param def_min 默认最小值，如果变量的数量大于指定为字符串的约束的数量；
		 * @param def_max 默认最小值，如果变量的数量大于指定为字符串的约束的数量。
		 */
		constraints(const std::vector< std::string >& str, size_t var_count, double def_min, double def_max)
			: constraints_base(var_count, std::make_shared< real_constraint >(def_min, def_max))
		{
			std::stringstream ess;

			for (std::vector< std::string >::size_type i = 0; i < str.size(); ++i)
			{
				std::string delim{ "[\\s;,]+" };//正则表达式通配符，用于匹配多余的字符
				auto s_result = udstr::bs_split(str[i], delim);

				std::string type;
				double _min;
				double _max;

				size_t count(0);

				for (std::vector< std::string >::size_type j = 0; j < s_result.size(); ++j, ++count)
				{
					switch (count)
					{
					case 0:
						type = s_result[count];
						break;
					case 1:
						_min = std::stod(s_result[count]);
					case 2:
						_max = std::stod(s_result[count]);
					default:
						// too many fields
						ess.clear();
						ess << "wrong variable format in \"" << str[i] << "\" - too many fields";
						throw constraints_exception(ess.str());
					}
				}

				// too few fields
				if (count < 3) {
					ess.clear();
					ess << "wrong variable format in \"" << str[i] << "\" - too few fields";
					throw constraints_exception(ess.str());
				}

				if (i < var_count)
					constraints_base::at(i) = str_to_constraint(type, _min, _max);
				else
					constraints_base::push_back(str_to_constraint(type, _min, _max));
			}
		}

		/**
		 * 返回一个限制在约束类型和范围内的随机值。
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param index 约束条件的索引
		 *
		 * @return double
		 */
		double get_rand_value(size_t index)
		{
			if (index < constraints_base::size())
				return (*this)[index]->get_rand_value();
			else
			{
				std::stringstream ess;
				ess << "invalid constraint index:" << index << ", higher than max number of constraints:" << constraints_base::size();
				throw constraints_exception(ess.str());
			}

		}

		/**
		 * 根据先前的值和原点，返回限制在约束类型和范围内的随机值。
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param index 约束条件的索引
		 * @param value 先前的值
		 * @param origin 原点
		 *
		 * @return double
		 */
		double get_rand_value(size_t index, double value, double origin)
		{
			if (index < constraints_base::size())
				return (*this)[index]->get_rand_value(value, origin);
			else
			{
				std::stringstream ess;
				ess << "invalid constraint index:" << index << ", higher than max number of constraints:" << constraints_base::size();
				throw constraints_exception(ess.str());
			}
		}

		/**
		 * 在由原点和表示为整个范围百分比的区域定义的“超立方体”区域内生成一组随机值。
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param origin 区域的原点坐标
		 * @param sidePct 超立方体的边占整个边的百分比
		 *
		 * @return DVectorPtr
		 */
		DVectorPtr get_square_zone_rand_values(const DVectorPtr origin, double sidePct) const
		{
			assert(origin);
			assert(sidePct > 0 && sidePct <= 100);

			if (origin->size() == constraints_base::size())
			{
				DVectorPtr square(std::make_shared< DVector >(origin->size()));

				for (constraints_base::size_type n = 0; n < constraints_base::size(); ++n)
					(*square)[n] = (*this)[n]->get_rand_value_in_zone((*origin)[n], sidePct);

				return square;

			}
			else
				throw constraints_exception("The origin vector must have the same number of elements as there are constraints");
		}

		DVectorPtr get_middle_point()
		{
			DVectorPtr r(std::make_shared< DVector >(constraints_base::size()));

			for (constraints_base::size_type n = 0; n < constraints_base::size(); ++n)
				(*r)[n] = (*this)[n]->get_middle_point();

			return r;

		}

		/**
		 * 在约束设置的限制范围内获取一组随机值。
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return DVectorPtr
		 */
		DVectorPtr get_rand_values() const
		{
			DVectorPtr r(std::make_shared< DVector >(constraints_base::size()));

			for (constraints_base::size_type n = 0; n < constraints_base::size(); ++n)
				(*r)[n] = (*this)[n]->get_rand_value();

			return r;
		}


	private:
		constraint_ptr str_to_constraint(const std::string& type, double min, double max)
		{
			if (udstr::tolower(type) == "real")
				return std::make_shared< real_constraint >(min, max);
			else if (udstr::tolower(type) == "int" || udstr::tolower(type) == "integer")
				return std::make_shared< int_constraint >(min, max);
			else
			{
				std::stringstream ess;
				ess << "invalid constraint type \"" << type << "\".";
				throw constraints_exception(ess.str());
			}
		}
	};

	typedef std::shared_ptr< constraints > constraints_ptr;

}

#endif //DE_CONSTRAINTS_HPP_INCLUDED
