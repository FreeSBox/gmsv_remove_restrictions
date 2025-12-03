#include "batch.hpp"

#include "range.hpp"

#include <tier0/dbg.h>
#include <future>//std::async

static std::mutex s_entry_mutex;
static std::vector<std::future<bool>> g_futures;

namespace memory
{
	void batch::add(std::string name, pattern pattern, std::function<void(handle)> callback)
	{
		m_entries.emplace_back(std::move(name), std::move(pattern), std::move(callback));
	}

	bool scan_pattern_and_execute_callback(range region, memory::batch::entry entry)
	{
		if (auto result = region.scan(entry.m_pattern))
		{
			if (entry.m_callback)
			{
				std::lock_guard<std::mutex> lock(s_entry_mutex);// Acquire a lock on the mutex to synchronize access.

				std::invoke(std::move(entry.m_callback), *result);
				Msg("Found '%s' at HL2.exe+'%p'\n", entry.m_name.c_str(), (void*)(result->as<char*>() - region.begin().as<char*>()));

				return true;
			}
		}

		Warning("Failed to find '%s'.\n", entry.m_name.c_str());

		return false;
	}

	bool batch::run(range region)
	{
		for (auto& entry_ : m_entries)
		{
			g_futures.emplace_back(std::async(&scan_pattern_and_execute_callback, region, entry_));
		}

		bool found_all_patterns = true;
		for (auto& future : g_futures)
		{
			future.wait();

			if (!future.get())
				found_all_patterns = false;
		}

		m_entries.clear();
		g_futures.clear();

		return found_all_patterns;
	}
}