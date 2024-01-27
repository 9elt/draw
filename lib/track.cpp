#include "Draw.hpp"
#define TRACK_CAPACITY 64
#define MIN_TRACK_LENGTH 4
#define MIN_POINT_DISTANCE 4.f

void Track::resize(u32 size) {
    u32 max = size > length ? length : size;

    Point *tmp = new Point[size];

    for (u32 i = 0; i < max; i++)
        tmp[i] = points[i];

    delete[] points;
    points = tmp;
    capacity = size;
}

void Track::fit() {
    if (length != capacity)
        resize(length);
}

void Track::push(Point point) {
    if (length < MIN_TRACK_LENGTH || point.dist(last()) > MIN_POINT_DISTANCE) {
        if (length == capacity)
            resize(capacity * 2);

        points[length++] = point;
    }
}

void Track::finalize() {
    finalized = true;

    if (is_empty()) {
        length = 0;
        fit();
        return;
    }

    fit();

    f32 lx, ty, rx, by;

    Point *init = get(0);

    ty = lx = init->y;
    by = rx = init->x;

    for (u32 i = 1; i < length; i++) {
        Point *p = get(i);

        if (p->x < lx)
            lx = p->x;
        if (p->x > rx)
            rx = p->x;
        if (p->y < ty)
            ty = p->y;
        if (p->y > by)
            by = p->y;
    }

    box = Box(lx, ty, rx - lx, by - ty);
}

Box *Track::bounding_box() { return &box; }

bool Track::is_finalized() { return finalized; }

bool Track::is_empty() { return length < MIN_TRACK_LENGTH; }

Point *Track::get(u32 i) { return &points[i]; }

Point *Track::last() { return &points[length - 1]; }

u32 Track::size() { return length; }

Track::Track() {
    capacity = TRACK_CAPACITY;
    length = 0;
    points = new Point[TRACK_CAPACITY];
    box = Box();
    finalized = false;
}

void Track::destroy() { delete[] points; };
