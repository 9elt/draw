#include "Draw.hpp"
#define CANVAS_CAPACITY 8

void Canvas::resize(u32 size) {
    u32 max = size > length ? length : size;

    Track *tmp = new Track[size];

    for (u32 i = 0; i < max; i++)
        tmp[i] = tracks[i];

    delete[] tracks;
    tracks = tmp;
    capacity = size;
}

Track *Canvas::last() { return &tracks[length - 1]; }

void Canvas::push(Track track) {
    if (length == capacity) {
        capacity *= 2;
        resize(capacity);
    }

    tracks[length++] = track;
}

u32 Canvas::size() { return length; }

Track *Canvas::get(u32 i) { return &tracks[i]; }

void Canvas::pop() {
    if (length > 0) {
        if (last()->is_finalized())
            tracks[length - 1] = Track();
        else if (length > 1) {
            tracks[length--].destroy();
        }
    }
}

void Canvas::clear() {
    for (u32 i = 0; i < length; i++)
        tracks[i].destroy();

    delete[] tracks;

    capacity = CANVAS_CAPACITY;
    length = 1;
    tracks = new Track[CANVAS_CAPACITY];
    tracks[0] = Track();
}

Canvas::Canvas() {
    capacity = CANVAS_CAPACITY;
    length = 1;
    tracks = new Track[CANVAS_CAPACITY];
    tracks[0] = Track();
}

Canvas::~Canvas() {
    for (u32 i = 0; i < length; i++)
        tracks[i].destroy();

    delete[] tracks;
}
