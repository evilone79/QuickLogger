#pragma once

#include <mutex>
#include <deque>

namespace qlog
{
	template<class T>
	class ConcurrentQ
	{
	public:
		void enqueue(T&& item)
		{
			std::lock_guard<std::mutex> lock(m_mtx);
			m_q.push_back(std::forward<T>(item));
		}

		void enqueue(const T& item)
		{
			std::lock_guard<std::mutex> lock(m_mtx);
			m_q.push_back(item);
		}

		bool try_dequeue(T& item)
		{
			std::lock_guard<std::mutex> lock(m_mtx);
			if (!m_q.empty())
			{
				item = m_q.front();
				m_q.pop_front();
				return true;
			}
			return false;
		}

		auto begin() { return m_q.begin(); }
		auto end() { return m_q.end(); }

		void clear()
		{
			std::lock_guard<std::mutex> lock(m_mtx);
			m_q.clear();
		}
		auto size()
		{
			std::lock_guard<std::mutex> lock(m_mtx);
			return m_q.size();
		}
	protected:
		std::mutex m_mtx;
		std::deque<T> m_q;
	};
}