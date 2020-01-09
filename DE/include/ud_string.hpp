#ifndef UD_STRING_HPP_INCLUDE
#define UD_STRING_HPP_INCLUDE

#include <string>
#include <iostream>
#include <vector>
#include <iterator>
#include <regex>
#include <algorithm>
#include <locale>

namespace udstr {
	/**
	* 使用正则表达式对多种格式的字符串进行切割，去除多余的字符
	*
	* @author louiehan (11/15/2019)
	*
	* @return std::vector<_str_type>
	*/
	template<typename E,
		typename TR = std::char_traits<E>,
		typename AL = std::allocator<E>,
		typename _str_type = std::basic_string<E, TR, AL>>
		std::vector<_str_type> bs_split(const std::basic_string<E, TR, AL>& in, const std::basic_string<E, TR, AL>& delim) {
		std::basic_regex<E> re{ delim };
		return std::vector<_str_type> {
			std::regex_token_iterator<typename _str_type::const_iterator>(in.begin(), in.end(), re, -1),
				std::regex_token_iterator<typename _str_type::const_iterator>()
		};
	}

	/**
	* 使用stl模板函数实现字符串大写转化
	*
	* @author louiehan (11/15/2019)
	*
	* @return std::string
	*/
	template<typename E,
		typename TR = std::char_traits<E>,
		typename AL = std::allocator<E>>
		inline std::basic_string<E, TR, AL> toupper(const std::basic_string<E, TR, AL>&src) {
		auto dst = src;
		static const std::locale loc("");
		transform(src.begin(), src.end(), dst.begin(), [&](E c)->E {return std::toupper(c, loc); });
		return dst;
	}

	/**
	* 使用stl模板函数实现字符串小写转化
	*
	* @author louiehan (11/15/2019)
	*
	* @return std::string
	*/
	template<typename E,
		typename TR = std::char_traits<E>,
		typename AL = std::allocator<E>>
		inline std::basic_string<E, TR, AL> tolower(const std::basic_string<E, TR, AL>&src) {
		auto dst = src;
		// 使用当前的locale设置
		static const std::locale loc("");
		// lambda表达式负责将字符串的每个字符元素转换为小写
		// std::string的元素类型为char,std::wstring的元素类型为wchar_t
		transform(src.begin(), src.end(), dst.begin(), [&](E c)->E {return std::tolower(c, loc); });
		return dst;
	}
}

#endif // !UD_STRING_HPP_INCLUDE
