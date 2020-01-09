#ifndef BOOSTTRIM_HPP_INCLUDED
#define BOOSTTRIM_HPP_INCLUDED

#include <string>
using namespace std;

//此名称空间的函数说明
//trim：删除字符串两端的空白符
//trim_copy：返回字符串删除两端空白符之后的新字符串，不改变原字符串
//trim_copy_if：返回字符串删除两端符合条件判断的字符，返回删除之后的新字符串，不改变原来字符串
//right：只删除右端的
//left：只删除左端的
namespace BoostTrim
{
	//判断一个char是否为空白符
	inline  bool is_space(char t)
	{
		return (t == char(9) /*HT*/ || t == char(10) || t == char(11) || t == char(13) || t == char(32)/*space*/);
	}

	template<typename Fun>
	inline void trim_left_if(string& Input, Fun is_space)
	{
		for (string::iterator i = Input.begin(); i != Input.end();)
		{
			if (is_space(*i))
			{
				i = Input.erase(i);
				continue;
			}
			else
			{
				break;
			}
		}
	}

	template<typename Fun>
	inline string trim_left_copy_if(const string& Input, Fun is_space)
	{
		string str;
		for (string::const_iterator i = Input.begin(); i != Input.cend();)
		{
			if (is_space(*i))
			{
				++i;
			}
			else
			{
				return string(i, Input.cend());
			}
		}
		return string();
	}

	inline string trim_left_copy(const string& Input)
	{
		return trim_left_copy_if(Input, is_space);
	}

	inline  void trim_left(string& Input)
	{
		trim_left_if(Input, is_space);
	}

	template<typename Fun>
	inline void trim_right_if(string& Input, Fun is_space)
	{
		size_t i = Input.size() - 1;
		for (; i != -1; --i)
		{
			if (is_space(Input[i]))
			{
				continue;
			}
			else
			{
				break;
			}
		}
		Input.erase(i + 1);
	}

	template<typename Fun>
	inline string trim_right_copy_if(const string& Input, Fun is_space)
	{
		size_t i = Input.size() - 1;
		for (; i != -1; --i)
		{
			if (is_space(Input[i]))
			{
				continue;
			}
			else
			{
				break;
			}
		}

		string result1;
		for (; i != -1; --i)
		{
			result1.push_back(Input[i]);
		}
		return result1;
	}

	inline string trim_right_copy(const string& Input)
	{
		trim_right_copy_if(Input, is_space);
	}


	inline  void trim_right(string& Input)
	{
		trim_right_if(Input, is_space);
	}

	template<typename Fun>
	inline void trim_if(string& Input, Fun is_space)
	{
		trim_left_if(Input, is_space);
		trim_right_if(Input, is_space);
	}

	inline void trim(string& str)
	{
		trim_left(str);
		trim_right(str);
	}

	template<typename Fun>
	inline string trim_copy_if(const string& Input, Fun is_space)
	{
		return trim_right_copy_if(trim_left_copy_if(Input, is_space), is_space);
	}

	inline string trim_copy(const string& Input)
	{
		return trim_copy_if(Input, is_space);
	}

}

#endif