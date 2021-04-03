#pragma once

#include "Star.h"

namespace Star
{
	class DaysOfWeek : ICompatibleWithString
	{
	private:
		std::vector<WString> dayNames;
	public:
		DaysOfWeek(const std::vector<WString> dayNames) :
			dayNames(dayNames)
		{
		}

		DaysOfWeek() :
			dayNames({
				L"Sunday",
				L"Monday",
				L"Tuesday",
				L"Wednesday",
				L"Thursday",
				L"Friday",
				L"Saturday"
			})
		{
		}

		WString ToName(int wday) const
		{
			return dayNames.at(wday);
		}

		WString ToString() const override
		{
			return String::Join(L", ", dayNames);
		}
	};

	class DayOfWeek : ICompatibleWithString
	{
	private:
		char indexFromSunday;
		DaysOfWeek daysOfWeek;
		WString name;
	public:
		DayOfWeek(DaysOfWeek daysOfWeek, int indexFromSunday) :
			daysOfWeek(daysOfWeek),
			indexFromSunday(indexFromSunday),
			name(daysOfWeek.ToName(indexFromSunday))
		{
		}

		DayOfWeek(int indexFromSunday) :
			DayOfWeek(
				DaysOfWeek(),
				indexFromSunday
			)
		{
		}

		int IndexFromSunday() const
		{
			return (int)indexFromSunday;
		}

		int IndexFromMonday() const
		{
			int indexFromSunday = this->indexFromSunday;
			return indexFromSunday == 0 ? 6 : --indexFromSunday;
		}

		WString Name() const
		{
			return name;
		}

		WString ToString() const override
		{
			return Name();
		}
	};
}