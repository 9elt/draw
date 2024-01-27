#include "raylib.h"

typedef unsigned int u32;

typedef signed int s32;

typedef float f32;

class Config {
  public:
    Color foreground;

    Color background;

    Color outlines;

    void load(int argc, char *argv[]);

    Config();
};

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

class Track {
    Point *points;

    u32 capacity;

    u32 length;

    Box box;

    bool finalized;

    void resize(u32 size);

    void fit();

  public:
    Box *bounding_box();

    bool is_empty();

    bool is_finalized();

    u32 size();

    Point *get(u32 i);

    Point *last();

    void push(Point point);

    void finalize();

    void destroy();

    Track();
};

class Canvas {
  private:
    Track *tracks;

    u32 capacity;

    u32 length;

    void resize(u32 size);

  public:
    u32 size();

    Track *get(u32 i);

    Track *last();

    void push(Track track);

    void pop();

    void clear();

    Canvas();

    ~Canvas();
};
