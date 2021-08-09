#include "Star.h"
#ifdef _WIN32
#include <windows.h>
#endif
#include "StringSequence.h"

namespace Star
{
	Star::Guid::Guid(std::string guid) :
		guid(guid)
	{
	}

	Guid Guid::NewGuid()
	{
#ifdef _WIN32
		GUID guid = { 0 };
		auto guidString = std::string(40, ' ');
		CoCreateGuid(&guid);
		sprintf(
			(char*)guidString.c_str(),
			"%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
			guid.Data1,
			guid.Data2,
			guid.Data3,
			guid.Data4[0],
			guid.Data4[1],
			guid.Data4[2],
			guid.Data4[3],
			guid.Data4[4],
			guid.Data4[5],
			guid.Data4[6],
			guid.Data4[7]
		);
		return guidString;
#else
		auto hex = StringSequence::GenerateRandomHex;
		return Guid(
			hex(8) + '-' +
			hex(4) + '-' +
			hex(4) + '-' +
			hex(4) + '-' +
			hex(12)
		);
#endif
	}

	Guid Guid::Empty()
	{
		return Guid(
			std::string(8, '0') + '-' +
			std::string(4, '0') + '-' +
			std::string(4, '0') + '-' +
			std::string(4, '0') + '-' +
			std::string(12, '0')
		);
	}

	Star::binaries Guid::ToBinaries() const
	{
		return Binaries::OfString(guid);
	}

	Star::WString Guid::ToString() const
	{
		return ToString<wchar_t>();
	}
}


