#include "Log.h"

namespace Star
{

	unsigned int Console::ToColor(LogTypes& logType)
	{
		switch (logType)
		{
		case LogTypes::PRIMARY: return 1;
		case LogTypes::SUCCESS: return 2;
		case LogTypes::INFO: return 3;
		case LogTypes::WARN: return 6;
		case LogTypes::DANGER: return 4;
		default: return 7;
		}
	}

	void Console::Colorize(LogTypes& logType, char margin)
	{
		const auto colorAttr = ToColor(logType);
		SetConsoleTextAttribute(hConsole, colorAttr + margin);
	}

	WString Console::ToLogTypeString(LogTypes logType)
	{
		switch (logType)
		{
		case LogTypes::PRIMARY: return L"Primary";
		case LogTypes::SUCCESS: return L"Success";
		case LogTypes::INFO: return L"Info";
		case LogTypes::WARN: return L"Warn";
		case LogTypes::DANGER: return L"Error";
		default: return L"Normal";
		}
	}

	void Console::RollbackColors()
	{
		SetConsoleTextAttribute(hConsole, consoleScreenInfo->wAttributes);
	}

	Console::Console(Options options) :
		options(options)
	{
		consoleScreenInfo->cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
		GetConsoleScreenBufferInfoEx(hConsole, consoleScreenInfo);

		consoleScreenInfo->ColorTable[1] = RGB(86, 156, 214);
		consoleScreenInfo->ColorTable[9] = RGB(156, 220, 254);

		consoleScreenInfo->ColorTable[2] = RGB(87, 166, 74);
		consoleScreenInfo->ColorTable[10] = RGB(181, 206, 168);

		consoleScreenInfo->ColorTable[3] = RGB(78, 201, 176);
		consoleScreenInfo->ColorTable[11] = RGB(168, 206, 204);

		consoleScreenInfo->ColorTable[6] = RGB(254, 239, 109);
		consoleScreenInfo->ColorTable[14] = RGB(220, 220, 170);

		SetConsoleScreenBufferInfoEx(hConsole, consoleScreenInfo);
	}

	Console::Console() : Console(Options())
	{

	}

	Console::~Console()
	{
		delete consoleScreenInfo;
	}

}
