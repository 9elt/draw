#define DEBUG true

#include "raylib.h"
#include <math.h>
#include <string>
#include <vector>

const float MIN_DIST = 3.f;
const float INPUT_RATE = 1.f / 30.f;

bool eq(Vector2 *a, Vector2 *b) { return a->x == b->x && a->y == b->y; }

Vector2 sub(Vector2 *a, Vector2 *b) {
    return Vector2{a->x - b->x, a->y - b->y};
}

Vector2 add(Vector2 *a, Vector2 *b) {
    return Vector2{a->x + b->x, a->y + b->y};
}

float dist(Vector2 *a, Vector2 *b) {
    return sqrt(pow(a->x - b->x, 2)) + sqrt(pow(a->y - b->y, 2));
}

typedef std::vector<Vector2> Points;

class Box {
  public:
    Vector2 pin;

    float top;
    float bottom;
    float left;
    float right;

    void setpin(Vector2 point) {
        this->pin.x = this->left = point.x;
        this->pin.y = this->top = point.y;
    }

    bool overlap(Box *other) {
        return ((this->left >= other->left && this->left <= other->right) ||
                (other->left >= this->left && other->left <= this->right)) &&
               ((this->top >= other->top && this->top <= other->bottom) ||
                (other->top >= this->top && other->top <= this->bottom));
    }

    Box() {
        this->top = 0.f;
        this->left = 0.f;
        this->bottom = 0.f;
        this->right = 0.f;
        this->pin = Vector2{0.f, 0.f};
    }

    Box(Points *points) {
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
    bool ok;
    Points points;
    Box box;
    void finalize() {
        this->ok = true;
        this->box = Box{&this->points};
    }
    void push(Vector2 point) { this->points.push_back(point); }
    Track() { this->ok = false; }
};

typedef std::vector<Track> Tracks;

int main(int argc, char *argv[]) {
    bool DARK = true;

    for (int i = 0; i < argc; i++)
        if (argv[i]) {
            std::string arg = std::string(argv[i]);
            if (arg == "--light" || arg == "-l")
                DARK = false;
        }

    Box area;
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

#if defined(DEBUG)
    Color const REC = DARK ? Color{219, 219, 219, 16} : Color{51, 51, 51, 16};
#endif

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
            leave_area = area.pin;
            leave_drag = mouse_curr;
            grabbing = true;
        } else if (grabbing) {
            if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
                grabbing = false;
            } else {
                Vector2 _i = sub(&leave_drag, &mouse_curr);
                area.setpin(add(&leave_area, &_i));
            }
        }

        if (ftime > INPUT_RATE || (ctime += ftime) > INPUT_RATE) {
            ctime = 0;
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                Track *track = &tracks[tracks.size() - 1];
                int len = track->points.size();
                if (len < 5 ||
                    dist(&track->points[len - 1], &mouse_curr) > MIN_DIST) {
                    track->push(add(&mouse_curr, &area.pin));
                    mouse_last = mouse_curr;
                }
            }
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            tracks[tracks.size() - 1].finalize();
            tracks.push_back(Track());
        }

        area.right = area.left + (float)GetRenderWidth();
        area.bottom = area.top + (float)GetRenderHeight();

        BeginDrawing();

        ClearBackground(BG);

        int ntracks = 0;
        int npoints = 0;

        for (int i = 0; i < tracks.size(); i++) {
            Track *track = &tracks[i];
            Box *box = &tracks[i].box;
            Points *points = &tracks[i].points;
            int len = points->size();

            if ((!track->ok || box->overlap(&area)) && len > 3) {
#if defined(DEBUG)
                if (track->ok)
                    DrawRectangleLines(
                        box->left - area.left, box->top - area.top,
                        box->right - box->left, box->bottom - box->top, REC);
#endif
                Vector2 tpoints[len];
                for (int p = 0; p < len; p++)
                    tpoints[p] = sub(&(*points)[p], &area.pin);

                DrawSplineBasis(tpoints, len, 1.25f, FG);

                ntracks++;
                npoints += len;
            }
        }

#if defined(DEBUG)
        std::string debug_info = "fps " + std::to_string(GetFPS()) + "\nx " +
                                 std::to_string(area.left) + " " +
                                 std::to_string(area.right) + "\ny " +
                                 std::to_string(area.top) + " \n" +
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
