#define DEBUG true
#include "Canvas.hpp"
#include "Config.hpp"
#include <string>

Config config;

const f32 INPUT_RATE = 1.f / 30.f;

int main(int argc, char *argv[]) {
    config.load(argc, argv);

    Box area;
    Canvas canvas;

    bool grabbing = false;

    f32 ftime = 0.f;
    f32 ctime = 0.f;

    Point leave_area;
    Point leave_drag;

    Point mouse_last;
    Point mouse_curr;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetTargetFPS(144);

    InitWindow(960, 640, "draw");

    while (!WindowShouldClose()) {
        mouse_curr = Point(GetMousePosition());
        ftime = GetFrameTime();

        if (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) {
            if (IsKeyPressed(KEY_C))
                canvas.clear();

            if (IsKeyPressed(KEY_Z))
                canvas.pop();
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            leave_area = area.pin();
            leave_drag = mouse_curr;
            grabbing = true;
        } else if (grabbing) {
            if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
                grabbing = false;
            } else {
                Point _i = leave_drag.sub(&mouse_curr);
                area.pin(leave_area.add(&_i));
            }
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            canvas.last()->finalize();
            canvas.push(Track());
        }

        if (ftime > INPUT_RATE || (ctime += ftime) > INPUT_RATE) {
            ctime = 0;
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                Point pin = area.pin();
                canvas.last()->push(mouse_curr.add(&pin));
                mouse_last = mouse_curr;
            }
        }

        area.width = GetRenderWidth();
        area.height = GetRenderHeight();

        BeginDrawing();

        ClearBackground(config.background);

#if defined(DEBUG)
        u32 debug_track_size = 0;
        u32 debug_points_count = 0;
#endif

        for (u32 i = 0; i < canvas.size(); i++) {
            Track *track = canvas.get(i);

            if (!track->is_empty()) {
                Box *box = track->bounding_box();
                u32 size = track->size();

                if (!track->is_finalized() || box->overlap(&area)) {
#if defined(DEBUG)
                    if (track->is_finalized())
                        DrawRectangleLines(box->x - area.x, box->y - area.y,
                                           box->width, box->height,
                                           config.outlines);
                    debug_track_size++;
                    debug_points_count += size;
#endif
                    Vector2 tpoints[size];
                    track->cast(area.pin(), tpoints);

                    DrawSplineBasis(tpoints, size, 1.25f, config.foreground);
                }
            }
        }

#if defined(DEBUG)
        std::string debug_info =
            "fps " + std::to_string(GetFPS()) + "\nx " +
            std::to_string(area.x) + " " + std::to_string(area.y) +
            "\nrendering " + std::to_string(debug_track_size) + " tracks\n" +
            std::to_string(debug_points_count) + " points";

        DrawText(debug_info.c_str(), 4, 4, 10, config.foreground);
#endif

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
