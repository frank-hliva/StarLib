#pragma once

#include <iostream>
#include <chrono>
#include <iomanip>
#include <functional>
#include "Text.h"
#include "DaysOfWeek.h"
#include <assert.h>
#include <ctime>

namespace Star
{
	namespace Chrono = std::chrono;
	using Clock = Chrono::system_clock;
	using TimePoint = Chrono::system_clock::time_point;
	using timestamp = time_t;
	using namespace Star;

	enum class DateTimeKind
	{
		Unspecified = 0,
		Utc = 1,
		Local = 2
	};

	enum class DaylightSavingMode
	{
		Unknown = -1,
		StandardTime = 0,
		DayLightSavingTime = 1
	};

	using days = Chrono::duration<int, std::ratio<3600 * 24>>;
	using hours = Chrono::hours;
	using minutes = Chrono::minutes;
	using seconds = Chrono::seconds;
	using milliseconds = Chrono::milliseconds;
	using microseconds = Chrono::microseconds;
	using nanoseconds = Chrono::nanoseconds;

	template<class UnderSecondUnit>
	class datetime : ICompatibleWithString
	{
	public:
		struct Struct
		{
			int year = 0;
			int month = 0;
			int day = 0;
			int hours = 0;
			int minutes = 0;
			int seconds = 0;
			UnderSecondUnit underSecond{ 0 };
			DateTimeKind kind = DateTimeKind::Unspecified;
		};
	private:
		const tm time;
		const UnderSecondUnit underSecond = UnderSecondUnit{ 0 };
		const DateTimeKind kind;
		static constexpr int daysInMonths[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	protected:
		static tm TimestampToTm(const Star::timestamp& timestamp, const DateTimeKind& kind)
		{
			switch (kind)
			{
			case DateTimeKind::Utc:
				return *std::gmtime(&timestamp);
			default:
				return *std::localtime(&timestamp);
			}
		}

		static UnderSecondUnit TimeUnderSecond(const TimePoint& timePoint)
		{
			Clock::duration totalDuration = timePoint.time_since_epoch();
			auto allSecs = Chrono::duration_cast<Chrono::seconds>(totalDuration);
			return Chrono::duration_cast<UnderSecondUnit>(totalDuration - allSecs);
		}

		static tm TimePointToTm(const TimePoint& time, const DateTimeKind& kind)
		{
			return TimestampToTm(
				Clock::to_time_t(time),
				kind
			);
		}

		static __inline Star::timestamp makeTimeByKind(_Inout_ struct tm* const time, const DateTimeKind& kind)
		{
			switch (kind)
			{
			case DateTimeKind::Utc:
				return _mkgmtime(time);
			default:
				return mktime(time);
			}
		}

		static tm ToTimeInfo(const tm& time, const DateTimeKind& kind)
		{
			auto tm = time;
			makeTimeByKind(&tm, kind);
			return tm;
		}

		static tm StructToTm(const Struct& time, const DateTimeKind& kind)
		{
			auto newTm = tm{
				.tm_sec = time.seconds,
				.tm_min = time.minutes,
				.tm_hour = time.hours,
				.tm_mday = time.day,
				.tm_mon = time.month - 1,
				.tm_year = time.year - 1900,
			};
			makeTimeByKind(&newTm, kind);
			return newTm;
		}

		datetime<UnderSecondUnit> UpdateTm(std::function<tm(tm)> updater) const
		{
			return datetime<UnderSecondUnit>{
				updater(tm{ time }),
					underSecond,
					kind
			};
		}

		static bool IsLeapYear(int year)
		{
			if (year % 4 != 0) return false;
			if (year % 400 == 0) return true;
			if (year % 100 == 0) return false;
			return true;
		}

		static int GetDaysInMonth(int year, int month)
		{
			assert(month >= 0);
			assert(month < 12);
			return daysInMonths[month] + (month == 1 && IsLeapYear(year) ? 1 : 0);
		}
	public:
		static datetime<UnderSecondUnit> NowUtc()
		{
			return datetime<UnderSecondUnit>(
				Clock::now(),
				DateTimeKind::Utc
			);
		}

		static datetime<UnderSecondUnit> Now()
		{
			return datetime<UnderSecondUnit>(
				Clock::now(),
				DateTimeKind::Local
			);
		}

		static datetime<UnderSecondUnit> Today()
		{
			return datetime<UnderSecondUnit>::Now().Date();
		}

		datetime(
			const tm& time,
			const UnderSecondUnit& underSecond = UnderSecondUnit{ 0 },
			const DateTimeKind& kind = DateTimeKind::Unspecified
		) :
			time(ToTimeInfo(time, kind)),
			underSecond(underSecond),
			kind(kind)
		{
		}

		datetime(
			const tm& time,
			const DateTimeKind& kind = DateTimeKind::Unspecified
		) :
			datetime(
				time,
				UnderSecondUnit{ 0 },
				kind
			)
		{
		}

		datetime(
			const TimePoint& timePoint,
			const DateTimeKind& kind = DateTimeKind::Unspecified
		) :
			datetime(
				TimePointToTm(timePoint, kind),
				TimeUnderSecond(timePoint),
				kind
			)
		{
		}

		datetime(
			const Star::timestamp& timestamp,
			const DateTimeKind& kind = DateTimeKind::Unspecified
		) :
			datetime(
				Clock::from_time_t(timestamp),
				kind
			)
		{
		}

		datetime(
			const Struct& dateTimeStruct,
			const DateTimeKind& kind = DateTimeKind::Unspecified
		) :
			datetime(
				StructToTm(dateTimeStruct, kind),
				dateTimeStruct.underSecond,
				kind
			)
		{
		}

		int Year() const
		{
			return time.tm_year + 1900;
		}
		int Month() const
		{
			return time.tm_mon + 1;
		}
		int Day() const
		{
			return time.tm_mday;
		}
		int Hours() const
		{
			return time.tm_hour;
		}
		int Minutes() const
		{
			return time.tm_min;
		}
		int Seconds() const
		{
			return time.tm_sec;
		}
		UnderSecondUnit UnderSecond() const
		{
			return underSecond;
		}

		template <class Rep, class Period>
		datetime<UnderSecondUnit> operator+ (const Chrono::duration<Rep, Period>& duration) const
		{
			return datetime<UnderSecondUnit>{
				ToTimePoint() + duration,
				Kind()
			};
		}

		template <class Rep, class Period>
		datetime<UnderSecondUnit> operator- (const Chrono::duration<Rep, Period>& duration) const
		{
			return datetime<UnderSecondUnit>{
				ToTimePoint() - duration,
				Kind()
			};
		}

		datetime<UnderSecondUnit> AddYears(int years) const
		{
			return UpdateTm([years](tm time) -> tm
			{
				time.tm_year += years;
				return time;
			});
		}

		datetime<UnderSecondUnit> AddMonths(int months) const
		{
			return UpdateTm([months, this](tm time) -> tm
			{
				auto isLastDayInMonth = time.tm_mday == GetDaysInMonth(time.tm_year, time.tm_mon);
				auto year = time.tm_year + months / 12;
				auto month = time.tm_mon + months % 12;
				if (month > 11)
				{
					year += 1;
					month -= 12;
				}
				auto result = tm(time);
				result.tm_year = year;
				result.tm_mon = month;
				result.tm_mday = (
					isLastDayInMonth
					? GetDaysInMonth(year, month)
					: min(time.tm_mday, GetDaysInMonth(year, month))
					);
				return result;
			});
		}

		datetime<UnderSecondUnit> AddDays(int days) const
		{
			return *this + ::days(days);
		}

		datetime<UnderSecondUnit> AddHours(int hours) const
		{
			return *this + ::hours(hours);
		}

		datetime<UnderSecondUnit> AddMinutes(int minutes) const
		{
			return *this + ::minutes(minutes);
		}

		datetime<UnderSecondUnit> AddSeconds(int seconds) const
		{
			return *this + ::seconds(seconds);
		}

		datetime<UnderSecondUnit> AddMilliseconds(int milliseconds) const
		{
			return *this + ::milliseconds(milliseconds);
		}

		Star::DayOfWeek DayOfWeek() const
		{
			return ::DayOfWeek(time.tm_wday);
		}
		int DayOfYear() const
		{
			return time.tm_yday;
		}
		bool IsLeapYear() const
		{
			return IsLeapYear(time.tm_year);
		}
		int TotalDaysInMonth() const
		{
			return GetDaysInMonth(time.tm_year, time.tm_mon);
		}

		DaylightSavingMode IsDaylightSaving() const
		{
			return static_cast<DaylightSavingMode>(time.tm_isdst);
		}

		DateTimeKind Kind() const
		{
			return kind;
		}

		datetime<UnderSecondUnit> Date() const
		{
			return datetime<UnderSecondUnit>(Struct{
				.year = Year(),
				.month = Month(),
				.day = Day(),
				.hours = 0,
				.minutes = 0,
				.seconds = 0,
				.underSecond = UnderSecondUnit{0},
				.kind = Kind()
			});
		}

		datetime<UnderSecondUnit> ToUtc() const
		{
			return datetime<UnderSecondUnit>(
				ToTimePoint(),
				DateTimeKind::Utc
			);
		}

		datetime<UnderSecondUnit> ToLocal() const
		{
			return datetime<UnderSecondUnit>(
				ToTimePoint(),
				DateTimeKind::Local
			);
		}

		TimePoint ToTimePoint() const
		{
			return Clock::from_time_t(ToTimestamp()) + underSecond;
		}

		tm ToTm() const
		{
			return tm(this->time);
		}

		timestamp ToTimestamp() const
		{
			auto time = this->time;
			return makeTimeByKind(&time, this->kind);
		}

		datetime::Struct ToStruct() const
		{
			return {
				.year = Year(),
				.month = Month(),
				.day = Day(),
				.hours = Hours(),
				.minutes = Minutes(),
				.seconds = Seconds(),
				.underSecond = UnderSecond(),
				.kind = Kind()
			};
		}

		template<typename InputCharType = wchar_t, typename FormatCharType = wchar_t>
		static datetime<UnderSecondUnit> Parse(
			const InputCharType* input,
			const FormatCharType* format = L"%Y-%m-%d %H:%M:%S",
			const DateTimeKind kind = DateTimeKind::Unspecified,
			const DaylightSavingMode daylightSaving = DaylightSavingMode::Unknown
		)
		{
			auto time = tm{ .tm_isdst = static_cast<int>(daylightSaving) };
			auto stringStream = std::basic_istringstream<InputCharType>(input);
			auto formatCString = ConvertString::ToCString<InputCharType, FormatCharType>(format);
			stringStream >> std::get_time<InputCharType>(&time, formatCString);

			return datetime<UnderSecondUnit>(
				time,
				UnderSecondUnit{ 0 },
				kind
			);
		}

		template<typename InputCharType = wchar_t, typename FormatCharType = wchar_t>
		static datetime<UnderSecondUnit> Parse(
			const std::basic_string<InputCharType>& input,
			const FormatCharType* format = L"%Y-%m-%d %H:%M:%S",
			const DateTimeKind kind = DateTimeKind::Unspecified,
			const DaylightSavingMode daylightSaving = DaylightSavingMode::Unknown
		)
		{
			return Parse<InputCharType, FormatCharType>(
				input.c_str(),
				format,
				kind,
				daylightSaving
			);
		}

		template<typename CharType = wchar_t>
		std::basic_string<CharType> ToString(const CharType* format) const
		{
			wchar_t buffer[32];
			auto formatCString = ConvertString::ToCString<wchar_t, CharType>(format);
			std::wcsftime(
				buffer,
				32,
				formatCString,
				&this->time
			);
			return ConvertString::ToString<CharType, wchar_t>(buffer);
		}

		template<typename CharType = wchar_t>
		std::basic_string<CharType> ToString(std::basic_string<CharType> format) const
		{
			return ToString<CharType>(format.c_str());
		}

		template<typename CharType = wchar_t>
		std::basic_string<CharType> ToString() const
		{
			return ToString<CharType>(L"%Y-%m-%d %H:%M:%S");
		}

		WString ToString() const override
		{
			return ToString<wchar_t>(L"%Y-%m-%d %H:%M:%S");
		}
	};

	template<typename CharType = wchar_t, typename UnderSecondUnit>
	std::basic_ostream<CharType>& operator<<(std::basic_ostream<CharType>& ostream, const datetime<UnderSecondUnit>& datetime)
	{
		ostream << datetime.ToString();
		return ostream;
	}


	using DateTime = datetime<microseconds>;
}

