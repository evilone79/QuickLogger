#ifndef __GlobalSingleton_H__
#define __GlobalSingleton_H__

#include <Windows.h>
#include <mutex>


template<typename T, bool allowLocal = false>
class GlobalSingleton
{
	using U = decltype(std::declval<T>());
	static_assert(!std::is_nothrow_constructible<U>::value, "Singleton type must be nothrow constructible");
	static T* _inst;

public:
	GlobalSingleton(const GlobalSingleton& other) = delete;
	GlobalSingleton(GlobalSingleton&& other) noexcept = delete;
	GlobalSingleton& operator=(const GlobalSingleton& other) = delete;
	GlobalSingleton& operator=(GlobalSingleton&& other) noexcept = delete;

	static T* instance(const char* ptrId)
	{
		static std::mutex s_mutex;
		if (!_inst)
		{
			{
				std::lock_guard<std::mutex> lock(s_mutex);
				if (!_inst)
					_inst = get_create_instance(ptrId);
			}
		}
		return _inst;
	}
	
	static void cleanup() { if (_inst) delete _inst; }

protected:
	GlobalSingleton() noexcept {};
	static T* get_create_instance(const char* ptrId)
	{
		auto hModule = GetModuleHandle(nullptr);
		auto pInst = GetProcAddress(hModule, ptrId);
		if (!pInst)
		{
			if (!allowLocal)
			{
				char msg[128] = { 0 };
				std::sprintf(msg, "Global pointer %s could not be exported\n", ptrId);
				throw std::exception(msg);
			}
			return new T();
		}
		auto ppInst = reinterpret_cast<T**>(pInst);
		if (!*ppInst)
		{
			*ppInst = new T();
		}
		return *ppInst;
	}

};
template<typename T, bool allowLocal = false>
T* GlobalSingleton<T, allowLocal>::_inst = nullptr;

#endif
