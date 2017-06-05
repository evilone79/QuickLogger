
#pragma once

#include <utility>
#include <algorithm>
#include "ConsumerQ.hpp"

namespace qlog
{
	template <typename Item, int Capacity>
	class ItemPool
	{
		static_assert(std::is_default_constructible<Item>::value, "Pool item must be default constructible");
	public:
		ItemPool() noexcept
		{
			std::generate(m_q.begin(), m_q.end(), [] {return new Item(); });
		}

		~ItemPool()
		{
			for (auto&& pItem : m_q) delete pItem;
		}

		void release_item(Item* item)
		{
			std::lock_guard<std::mutex> lock(m_mtx);
			m_q.push_back(item);
			m_waiter.notify_one();
		}

		Item* get_item()
		{
			std::lock_guard<std::mutex> lock(m_mtx);
			Item* item = nullptr;
			if (m_q.empty())
			{
				m_q.emplace_back(new Item());
				++m_itemCount;
			}
			item = m_q.front();
			m_q.pop_front();
			return item;
		}

		void synchronize()
		{
			std::unique_lock<std::mutex> lock(m_mtx);
			m_waiter.wait(lock, [this] {return m_q.size() == m_itemCount; });
		}

	private:
		long m_itemCount = Capacity;
		std::mutex m_mtx;
		std::condition_variable m_waiter;
		std::deque<Item*> m_q{ Capacity };
	};
}
