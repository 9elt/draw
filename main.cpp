#define DEBUG true

#include "raylib.h"
#include <string>
#include <vector>

const float INPUT_RATE = 1.f / 30.f;

bool eq(Vector2 *a, Vector2 *b) { return a->x == b->x && a->y == b->y; }

Vector2 sub(Vector2 *a, Vector2 *b) {
    return Vector2{a->x - b->x, a->y - b->y};
}

Vector2 add(Vector2 *a, Vector2 *b) {
    return Vector2{a->x + b->x, a->y + b->y};
}

typedef std::vector<Vector2> Points;

class Box {
  public:
    float top;
    float bottom;
    float left;
    float right;
    bool init;
    Box() { this->init = false; }
    Box(Points *points) {
        this->init = true;

        int size = points->size();
        if (size == 0)
            return;

        Vector2 initial = (*points)[0];
        this->top = initial.y;
        this->bottom = initial.y;
        this->left = initial.x;
        this->right = initial.x;

        for (int i = 1; i < size; i++) {
            Vector2 *p = &(*points)[i];
            if (p->x < this->left)
                this->left = p->x;
            if (p->x > this->right)
                this->right = p->x;
            if (p->y < this->top)
                top = p->y;
            if (p->y > this->bottom)
                this->bottom = p->y;
        }
    }
};

class Track {
  public:
    Points points;
    Box box;
    void calcBox() {

        Box bb = Box{&this->points};

        this->box = bb;
    }
    void push(Vector2 point) { this->points.push_back(point); }
    Track() {}
};

typedef std::vector<Track> Tracks;

class Area {
  public:
    Vector2 pos;
    float right;
    float bottom;
    Area() {
        this->pos = {0.f, 0.f};
        this->right = 0.f;
        this->bottom = 0.f;
    }
};

bool overlap(Area *A, Box *B) {
    return ((A->pos.x >= B->left && A->pos.x <= B->right) ||
            (B->left >= A->pos.x && B->left <= A->right)) &&
           ((A->pos.y >= B->top && A->pos.y <= B->bottom) ||
            (B->top >= A->pos.y && B->top <= A->bottom));
}

int main(int argc, char *argv[]) {
    bool DARK = true;

    for (int i = 0; i < argc; i++) {
        if (argv[i]) {
            std::string arg = std::string(argv[i]);
            if (arg == "--light" || arg == "-l")
                DARK = false;
        }
    }

    Area area;
    Tracks tracks;
    tracks.push_back(Track());

    bool grabbing = false;

    float ftime = 0.f;
    float ctime = 0.f;

    Vector2 leave_area;
    Vector2 leave_drag;

    Vector2 mouse_last;
    Vector2 mouse_curr;

    Color const BG = DARK ? Color{42, 42, 42} : Color{232, 232, 232};
    Color const FG = DARK ? Color{219, 219, 219, 128} : Color{51, 51, 51, 128};
    Color const REC = DARK ? Color{255, 255, 255, 16} : Color{0, 0, 0, 16};

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetTargetFPS(144);

    InitWindow(960, 640, "draw");

    while (!WindowShouldClose()) {
        mouse_curr = GetMousePosition();
        ftime = GetFrameTime();

        if (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) {
            if (IsKeyPressed(KEY_C)) {
                tracks.clear();
                tracks.push_back(Track());
            }
            if (IsKeyPressed(KEY_Z)) {
                if (tracks[tracks.size() - 1].points.size() == 0)
                    tracks.pop_back();
                if (tracks.size() > 0)
                    tracks.pop_back();
                tracks.push_back(Track());
            }
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            leave_area = area.pos;
            leave_drag = GetMousePosition();
            grabbing = true;
        } else if (grabbing) {
            if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
                grabbing = false;
            } else {
                Vector2 _i = sub(&leave_drag, &mouse_curr);
                area.pos = add(&leave_area, &_i);
            }
        }

        if (ftime > INPUT_RATE || (ctime += ftime) > INPUT_RATE) {
            ctime = 0;
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                if (!eq(&mouse_last, &mouse_curr)) {
                    tracks[tracks.size() - 1].push(add(&mouse_curr, &area.pos));
                    mouse_last = mouse_curr;
                }
            }
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            tracks[tracks.size() - 1].calcBox();
            tracks.push_back(Track());
        }

        area.right = area.pos.x + (float)GetRenderWidth();
        area.bottom = area.pos.y + (float)GetRenderHeight();

        BeginDrawing();

        ClearBackground(BG);

        int ntracks = 0;
        int npoints = 0;

        for (int i = 0; i < tracks.size(); i++) {
            Box *box = &tracks[i].box;
            Points *points = &tracks[i].points;
            int len = points->size();

            if ((!box->init || overlap(&area, box)) && len > 3) {
#if defined(DEBUG)
                if (box->init)
                    DrawRectangleLines(
                        box->left - area.pos.x, box->top - area.pos.y,
                        box->right - box->left, box->bottom - box->top, REC);
#endif
                Vector2 tpoints[len];
                for (int p = 0; p < len; p++)
                    tpoints[p] = sub(&(*points)[p], &area.pos);

                DrawSplineBasis(tpoints, len, 1.25f, FG);

                ntracks++;
                npoints += len;
            }
        }

#if defined(DEBUG)
        std::string debug_info = "fps " + std::to_string(GetFPS()) + "\nx " +
                                 std::to_string(area.pos.x) + " " +
                                 std::to_string(area.right) + "\ny " +
                                 std::to_string(area.pos.y) + " \n" +
                                 std::to_string(area.bottom) + "\nrendering " +
                                 std::to_string(ntracks) + " tracks\n" +
                                 std::to_string(npoints) + " points";

        DrawText(debug_info.c_str(), 4, 4, 10, FG);
#endif

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
