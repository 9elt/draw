#include "raylib.h"

class Config {
  public:
    Color foreground;

    Color background;

    Color outlines;

    void load(int argc, char *argv[]);

    Config();
};
