#ifndef DE_RANDOM_GENERATOR_HPP_INCLUDED
#define DE_RANDOM_GENERATOR_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <cassert>
#include <cmath>
#include <random>

namespace de
{

	inline double genrand(double min = 0, double max = 1)
	{
		static std::mt19937 gen;
		std::uniform_real_distribution<> dist(min, max);

		return dist(gen);
	}

	inline int genintrand(double min, double max, bool upperexclusive = false)
	{
		assert(min < max);
		int ret = 0;
		do ret = std::round(genrand(min, max)); while (ret < min || ret > max || upperexclusive && ret == max);
		return ret;
	}

}

#endif //DE_RANDOM_GENERATOR_HPP_INCLUDED
