#include "Typedefs.hpp"
#include "raylib.h"

class Point {
  public:
    f32 x;

    f32 y;

    Point sub(Point *other);

    Point add(Point *other);

    bool eq(Point *other);

    f32 dist(Point *other);

    Vector2 raw();

    Point(f32 x, f32 y);

    Point(Vector2 raw);

    Point();
};
