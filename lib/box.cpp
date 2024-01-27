#include "Draw.hpp"

bool Box::overlap(Box *other) {
    return ((x >= other->x && x <= other->x + other->width) ||
            (other->x >= x && other->x <= x + width)) &&
           ((y >= other->y && y <= other->y + other->height) ||
            (other->y >= y && other->y <= y + height));
}

Point Box::pin() { return Point{x, y}; }

void Box::pin(Point pos) {
    x = pos.x;
    y = pos.y;
}

Box::Box() {
    x = 0.f;
    y = 0.f;
    width = 0.f;
    height = 0.f;
}

Box::Box(float _x, float _y, float w, float h) {
    x = _x;
    y = _y;
    width = w;
    height = h;
};
