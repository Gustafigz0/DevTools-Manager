#pragma once
#include <string>

namespace Ansi {
    inline constexpr const char* reset = "\x1b[0m";
    inline constexpr const char* bold = "\x1b[1m";
    inline constexpr const char* underline = "\x1b[4m";

    namespace fg {
        inline constexpr const char* black   = "\x1b[30m";
        inline constexpr const char* red     = "\x1b[31m";
        inline constexpr const char* green   = "\x1b[32m";
        inline constexpr const char* yellow  = "\x1b[33m";
        inline constexpr const char* blue    = "\x1b[34m";
        inline constexpr const char* magenta = "\x1b[35m";
        inline constexpr const char* cyan    = "\x1b[36m";
        inline constexpr const char* white   = "\x1b[37m";
        inline constexpr const char* bright_black = "\x1b[90m";
        inline constexpr const char* bright_white = "\x1b[97m";
    }

    std::string colorize(const std::string& text, const char* style);
    void enableVirtualTerminalIfNeeded();
}
