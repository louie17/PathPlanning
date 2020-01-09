#ifndef DE_MULTITHREAD_HPP_INCLUDED
#define DE_MULTITHREAD_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <thread>
#include <mutex>
//#include <boost/thread.hpp>

namespace de
{
	typedef std::recursive_mutex mutex;
	typedef std::lock_guard< std::recursive_mutex > lock;

}

#endif //DE_MULTITHREAD_HPP_INCLUDED
