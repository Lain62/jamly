import raylib;
import std.string;
import std.conv;
import std.stdio;

const Color VU_BLACK = {0x1B, 0x20, 0x21, 0xFF};
const Color VU_ROSE = {0xEA, 0x63, 0x8C, 0xFF};
const Color VU_WHITE = {0xFF, 0xD9, 0xDA, 0xFF};

const int HOUR_IDX = 0;
const int MINUTE_IDX = 2;
const int SECOND_IDX = 4;

void add_one(string[] time, int idx) {
    int tim = to!int(time[idx]);
    tim += 1;
    if (tim < 10) {
        time[idx] = join(["0", to!string(tim)]);
    } else {
        time[idx] = to!string(tim);
    }
}

void main() {
    string[] time = ["00", ":", "00", ":", "00", "\0"];
    
    InitWindow(800, 600, "Timer");
    
    SetTargetFPS(60);
    
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    double seconds_since_start = GetTime();
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(VU_BLACK);
        if (cast(int)seconds_since_start < cast(int)GetTime()) {
            seconds_since_start = GetTime();            
            add_one(time, SECOND_IDX);
            if (to!int(time[SECOND_IDX]) >= 60) {
                time[SECOND_IDX] = "00";
                add_one(time, MINUTE_IDX);
            }
            if (to!int(time[MINUTE_IDX]) >= 60) {
                time[MINUTE_IDX] = "00";
                add_one(time, HOUR_IDX);
            }            
        }
        int timer_font_size = GetScreenWidth() / 6;
        int timer_x = (GetScreenWidth() / 2) - (MeasureText(join(time).ptr, timer_font_size) / 2);
        int timer_y = (GetScreenHeight() / 2) - ((GetScreenHeight() / 6) / 2);
        DrawText(join(time).ptr, timer_x, timer_y, timer_font_size, VU_WHITE);
        EndDrawing();
    }
    
    CloseWindow();
}
