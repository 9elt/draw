#include "Track.hpp"

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
