#include "Draw.hpp"
#include <string>

Config::Config() {
    foreground = Color{219, 219, 219, 128};
    background = Color{42, 42, 42, 255};
    outlines = Color{219, 219, 219, 16};
}

void Config::load(int argc, char *argv[]) {
    for (int i = 0; i < argc; i++) {
        std::string arg = std::string(argv[i]);
        if (arg == "--light" || arg == "-l") {
            foreground = Color{51, 51, 51, 128};
            background = Color{232, 232, 232, 255};
            outlines = Color{51, 51, 51, 16};
        }
    }
};
