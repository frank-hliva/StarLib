#pragma once

#include <Windows.h>
#include <string>
#include "Star.h"

namespace Star
{
    namespace IO
    {
        namespace Pipes
        {
            bool __stdcall CreatePipeW(
                OUT LPHANDLE readPipe,
                OUT LPHANDLE writePipe,
                IN LPSECURITY_ATTRIBUTES securityAttributes,
                IN unsigned long size,
                unsigned long readMode,
                unsigned long writeMode,
                WString pipeName
            );
        }
    }
}