#pragma once

#include <windows.h>
#include <sstream>

#ifdef _DEBUG
    #ifdef ERROR
        #undef ERROR
    #endif
    #define ERROR(msg) {\
            std::wstringstream ss; \
            ss << L"Error in " << __FILE__ << L" line " << __LINE__ << L": " << msg; \
            MessageBoxW(NULL, ss.str().c_str(), L"Error", MB_OK | MB_ICONERROR); \
        }
    #ifdef INFO
        #undef INFO
    #endif
    #define INFO(msg) {\
            std::wstringstream ss; \
            ss << L"Info in " << __FILE__ << L" line " << __LINE__ << L": " << msg; \
            MessageBoxW(NULL, ss.str().c_str(), L"Information", MB_OK | MB_ICONINFORMATION); \
        }
    #ifdef WARNING
        #undef WARNING
    #endif
    #define WARNING(msg) {\
            std::wstringstream ss; \
            ss << L"Warning in " << __FILE__ << L" line " << __LINE__ << L": " << msg; \
            MessageBoxW(NULL, ss.str().c_str(), L"Warning", MB_OK | MB_ICONWARNING); \
        }
#else
#define ERROR(msg) ((void)0)
#define INFO(msg) ((void)0)
#define WARNING(msg) ((void)0)
#endif
