#pragma once
#include <cstdint>

namespace nu
{
	class Time
	{
	public:
		Time();

		void Reset();
		void Tick();

		float GetTime() { return TicksToSeconds(a_currentTicks - a_startTicks); }
		float GetDeltaTime() { return TicksToSeconds(a_deltaTicks); }

		float TicksToSeconds(uint64_t ticks) { return (float)ticks / 1'000'000'000; }

	private:
		uint64_t a_startTicks = 0;
		uint64_t a_currentTicks = 0;
		uint64_t a_frameTicks = 0;
		uint64_t a_deltaTicks = 0;
	};
}