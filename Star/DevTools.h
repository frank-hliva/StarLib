#pragma once
#include <iostream>
#include <functional>
#include <chrono>

namespace Star::DevTools
{
	void TIME_TEST(std::string title, std::function<void()> action)
	{
		namespace chrono = std::chrono;
		using steadyClock = chrono::steady_clock;

		auto begin = steadyClock::now();
		action();
		auto end = steadyClock::now();
		if (title == "")
		{
			std::cout << "*** " << title << " ***" << std::endl;
		}
		std::cout << "Time in microseconds: " << chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << std::endl;
		std::cout << "Time in milliseconds: " << chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << std::endl;
		std::cout << "Time in seconds: " << chrono::duration_cast<std::chrono::seconds>(end - begin).count() << std::endl;
		std::cout << "Time in minutes: " << chrono::duration_cast<std::chrono::minutes>(end - begin).count() << std::endl;
	}

	void TIME_TEST(std::function<void()> action)
	{
		TIME_TEST("", action);
	}
}