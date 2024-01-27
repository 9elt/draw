#include "Point.hpp"
#include "math.h"
#include <raylib.h>

Point Point::sub(Point *other) { return Point(x - other->x, y - other->y); }

Point Point::add(Point *other) { return Point(x + other->x, y + other->y); }

bool Point::eq(Point *other) { return x == other->x && y == other->y; }

float Point::dist(Point *other) {
    return sqrt(pow(x - other->x, 2)) + sqrt(pow(y - other->y, 2));
}

Vector2 Point::raw() { return Vector2{x, y}; }

Point::Point(float _x, float _y) {
    x = _x;
    y = _y;
}

Point::Point(Vector2 raw) {
    x = raw.x;
    y = raw.y;
}

Point::Point() {
    x = 0.f;
    y = 0.f;
}
