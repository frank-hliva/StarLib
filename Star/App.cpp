#include "Windows.h"
#include <Shlwapi.h>
#include "App.h"

namespace Star
{
	WString App::Path()
	{
		wchar_t filePath[MAX_PATH] = {0};
		GetModuleFileNameW(nullptr, filePath, MAX_PATH);
		return WString(filePath);
	}

	WString App::Directory()
	{
		wchar_t path[MAX_PATH] = {0};
		GetModuleFileNameW(nullptr, path, MAX_PATH);
		PathRemoveFileSpecW(path);
		return WString(path);
	}
}