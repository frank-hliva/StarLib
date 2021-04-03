#include <iostream>
#include <cstdio>
#include <errno.h>
#include "Star.h"
#include "CreatePipeEx.h"

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
            )
            {
                if (size == 0)
                {
                    size = 4096;
                }
                if ((readMode | writeMode) & (~FILE_FLAG_OVERLAPPED))
                {
                    SetLastError(ERROR_INVALID_PARAMETER);
                    return false;
                }
                auto readPipeHandle = CreateNamedPipeW(
                    pipeName.c_str(),
                    PIPE_ACCESS_INBOUND | readMode,
                    PIPE_TYPE_BYTE | PIPE_WAIT,
                    1,
                    size,
                    size,
                    120 * 1000,
                    securityAttributes
                );
                if (!readPipeHandle)
                {
                    return false;
                }
                HANDLE writePipeHandle = CreateFileW(
                    pipeName.c_str(),
                    GENERIC_WRITE,
                    0,
                    securityAttributes,
                    OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL | writeMode,
                    nullptr
                );
                if (writePipeHandle == INVALID_HANDLE_VALUE)
                {
                    unsigned long errorCode = GetLastError();
                    std::cout << errorCode << std::endl;
                    CloseHandle(readPipeHandle);
                    SetLastError(errorCode);
                    return false;
                }
                *readPipe = readPipeHandle;
                *writePipe = writePipeHandle;
                return true;
            }
        }
    }
}
