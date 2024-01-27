#include "Box.hpp"

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

    void cast(Point pin, Vector2 arr[]);

    Track();
};
