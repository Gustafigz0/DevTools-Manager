#include "Ansi.hpp"

#ifdef _WIN32
  #define NOMINMAX
  #include <windows.h>
#endif

namespace Ansi {
    std::string colorize(const std::string& text, const char* style) {
        return std::string(style) + text + reset;
    }

    void enableVirtualTerminalIfNeeded() {
    #ifdef _WIN32
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut == INVALID_HANDLE_VALUE) return;
        DWORD outMode = 0;
        if (!GetConsoleMode(hOut, &outMode)) return;

        DWORD requested = outMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        if (!SetConsoleMode(hOut, requested)) {
            // Tenta novamente com apenas ENABLE_VIRTUAL_TERMINAL_PROCESSING (fallback)
            SetConsoleMode(hOut, outMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        }
    #endif
    }
}
