#include "engine.h"

#include "vector.h"

#include <stdio.h>
#include <string.h>

double delta_time = 0;
double elapsed = 0;
double fps = 0;
double frame_time = 0;
bool is_fps_locked = false;

void UpdateGame() {
    double time = GetTime();

    if (is_fps_locked && frame_time > 0.0) {
        while (time - elapsed < frame_time) time = GetTime();
    }

    delta_time = fmax(time - elapsed, 0.0);
    elapsed = time;
    fps = 1.0 / delta_time;
}
double GetDeltaTime() {
    return delta_time;
}
double GetFPS() {
    return fps;
}
void LockFPS(double FPS) {
    is_fps_locked = true;
    frame_time = 1.0/FPS;
}
void UnlockFPS() {
    is_fps_locked = false;
    frame_time = 0.0;
}