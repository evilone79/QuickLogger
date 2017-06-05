#pragma once

#include "ConcurrentQ.hpp"

namespace qlog
{
	template <typename T>
	class ConsumerQ
	{
	public:
		virtual ~ConsumerQ()
		{
			m_waiter.notify_all();
		}

		void put_item(T&& item)
		{
			if (m_teardown) return;
			m_conQ.enqueue(std::forward<T>(item));
			m_waiter.notify_one();
		}

		void put_item(const T& item)
		{
			if (m_teardown) return;
			m_conQ.enqueue(item);
			m_waiter.notify_one();
		}

		bool try_fetch_item(T& item)
		{
			if (m_teardown)
			{
				return try_fetch_item_nonblocking(item);
			}
			std::unique_lock<std::mutex> waitLock(m_waitMtx);
			m_waiter.wait(waitLock, [this, &item]()
			{
				return m_conQ.try_dequeue(item) || m_teardown;
			});
			return !m_teardown;
		}

		bool try_fetch_item_nonblocking(T& item)
		{
			return m_conQ.try_dequeue(item);
		}

		void initiate_teardown()
		{
			m_teardown = true;
			m_waiter.notify_one();
		}

	protected:
		std::mutex m_waitMtx;
		std::condition_variable m_waiter;
		ConcurrentQ<T> m_conQ;
		std::atomic_bool m_teardown = false;
	};
}