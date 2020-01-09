#ifndef BOOST_THREAD_GROUP_HPP
#define BOOST_THREAD_GROUP_HPP

#include <thread>
#include <mutex>
#include <list>
#include <memory>

namespace boost
{
	//兼容boost::thread_group
	//使用std::thread代替boost::thread,std::mutex代替boost::shared_mutex
	class thread_group
	{
	private:
		thread_group(thread_group const&);
		thread_group& operator=(thread_group const&);
	public:
		thread_group() {}
		~thread_group()
		{
			for (auto it = threads.begin(), end = threads.end(); it != end; ++it)
			{
				delete *it;
			}
		}

		bool is_this_thread_in()
		{
			std::thread::id id = std::this_thread::get_id();
			std::lock_guard<std::mutex> guard(m);
			for (std::list<std::thread*>::iterator it = threads.begin(), end = threads.end();
				it != end;
				++it)
			{
				if ((*it)->get_id() == id)
					return true;
			}
			return false;
		}

		bool is_thread_in(std::thread* thrd)
		{
			if (thrd)
			{
				std::thread::id id = thrd->get_id();
				std::lock_guard<std::mutex> guard(m);
				for (std::list<std::thread*>::iterator it = threads.begin(), end = threads.end();
					it != end;
					++it)
				{
					if ((*it)->get_id() == id)
						return true;
				}
				return false;
			}
			else
			{
				return false;
			}
		}

		template<typename F>
		std::thread* create_thread(F threadfunc)
		{
			std::lock_guard<std::mutex> guard(m);
			std::unique_ptr<std::thread> new_thread(new thread(threadfunc));
			threads.push_back(new_thread.get());
			return new_thread.release();
		}

		void add_thread(std::thread* thrd)
		{
			if (thrd)
			{
				assert(!is_thread_in(thrd));
				std::lock_guard<std::mutex> guard(m);
				threads.push_back(thrd);
			}
		}

		void remove_thread(std::thread* thrd)
		{
			std::lock_guard<std::mutex> guard(m);
			auto it = std::find(threads.begin(), threads.end(), thrd);
			if (it != threads.end())
			{
				threads.erase(it);
			}
		}

		void join_all()
		{
			assert(!is_this_thread_in());
			std::lock_guard<std::mutex> guard(m);
			for (auto it = threads.begin(), end = threads.end(); it != end; ++it)
			{
				if ((*it)->joinable())
					(*it)->join();
			}
		}

		size_t size() const
		{
			std::lock_guard<std::mutex> guard(m);
			return threads.size();
		}

	private:
		std::list<std::thread*> threads;
		mutable std::mutex m;
	};
}

#endif // !BOOST_THREAD_GROUP_HPP
