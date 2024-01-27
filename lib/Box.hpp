#include "Point.hpp"

class Box {
  public:
    f32 x;

    f32 y;

    f32 width;

    f32 height;

    bool overlap(Box *other);

    Point pin();

    void pin(Point point);

    Box();

    Box(f32 x, f32 y, f32 width, f32 height);
};
