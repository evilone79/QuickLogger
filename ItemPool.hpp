
#pragma once

#include <utility>
#include <algorithm>
#include <vector>
#include "ConsumerQ.hpp"
#include <iostream>

namespace qlog
{
	template <typename ItemT>
	class pool_slot
	{		
	public:
		pool_slot(std::atomic_size_t& syncCount) : _syncCount(syncCount) {}
		
		ItemT * item() noexcept { return &_item; }
		bool available() const { return _refCnt.load() == 0; }
		void inc_ref()
		{			
			_refCnt.fetch_add(1); 
			_syncCount.fetch_add(1);			
		}
		void dec_ref()
		{
			_refCnt.fetch_sub(1); 
			_syncCount.fetch_sub(1);			
		}		
	private:
		ItemT _item;
		volatile std::atomic_size_t _refCnt = 0;
		std::atomic_size_t& _syncCount;
	};

	
	
	template <typename ItemT, size_t Cap>
	class sequential_pool
	{		
	public:
		sequential_pool()
		{
			std::generate(std::begin(_items), std::end(_items), 
				[&] {return std::make_unique<pool_slot<ItemT>>(s_syncCount); });
		}
		pool_slot<ItemT>* get_slot()
		{
			std::lock_guard<std::mutex> lock(_mtx);			
			while(!_items[_index]->available()){}
			auto slot = _items[_index].get();
			++_index %= _items.size();			
			return slot;
		}
		void synchronize()
		{			
			while(s_syncCount > 0){}			
		}
	private:
		size_t _index = 0;
		std::mutex _mtx;
		std::vector<std::unique_ptr<pool_slot<ItemT>>> _items{ Cap };
		static std::atomic_size_t s_syncCount;
	};

	template <typename ItemT, size_t Cap>
	std::atomic_size_t sequential_pool<ItemT, Cap>::s_syncCount = 0;
}
