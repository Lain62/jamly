#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "raylib.h"

#define NOB_IMPLEMENTATION
#include "nob.h"

#define FLAG_IMPLEMENTATION
#include "flag.h"

#define ROSEVUWHITE CLITERAL(Color){ 0xFF, 0xD9, 0xDA, 0xFF }
#define ROSEVUBLACK CLITERAL(Color){ 0x1B, 0x20, 0x21, 0xFF }

#define TEXTSPACING 20

typedef struct {
    int hour;
    int minute;
    int second;
    bool minus;
} Timer;

Timer timer = {0};

char *get_int_to_string_clock(int num) {
    if (num < 10) {
        return nob_temp_sprintf("0%d", num);
    } else {
        return nob_temp_sprintf("%d", num);
    }
}

char *get_timer_text(void) {
    char *hour = get_int_to_string_clock(timer.hour);
    char *minute = get_int_to_string_clock(timer.minute);
    char *second = get_int_to_string_clock(timer.second);

    if (timer.minus) return nob_temp_sprintf("-%s:%s:%s", hour, minute, second);
    return nob_temp_sprintf("%s:%s:%s", hour, minute, second);
}

bool is_timer_zero(void) {
    if (timer.hour > 0) return false;
    if (timer.minute > 0) return false;
    if (timer.second > 0) return false;
    return true;
}

void timer_add_one(void) {
    timer.second += 1;

    if (timer.second >= 60) {
        timer.second = 0;
        timer.minute += 1;
    }

    if (timer.minute >= 60) {
        timer.minute = 0;
        timer.hour += 1;
    }
}

void timer_minus_one(void) {
    if (is_timer_zero()) return;
    timer.second -= 1;

    if (timer.second < 0) {
        timer.second = 59;
        if (timer.minute >= 0) timer.minute -= 1;
    }

    if (timer.minute < 0) {
        timer.minute = 59;
        if (timer.hour >= 0) timer.hour -= 1;
    }
}

void toggle_minus(void) {
    timer.minus = !timer.minus;
}

void set_timer(int hour, int minute, int second) {
    timer.hour = hour;
    timer.minute = minute;
    timer.second = second;
}

void usage(FILE *stream) {
    fprintf(stream, "./jamly [OPTIONS]\n");
    fprintf(stream, "OPTIONS:\n");
    flag_print_options(stream);
}

int main(int argc, char **argv) {
    // Setup Vars
    const char *text_timer;
    Vector2 text_size;
    float timer_font_size;
    int timer_x, timer_y;
    double seconds_since_start = GetTime();

    // Setup Flags
    bool *help = flag_bool("h", false, "Prints this help");
    bool *countdown_mode = flag_bool("c", false, "Activate Countdown Mode");

    uint64_t *hour = flag_uint64("H", 0, "Sets the hour of the timer");
    uint64_t *minute = flag_uint64("M", 0, "Sets the minute of the timer");
    uint64_t *second = flag_uint64("S", 0, "Sets the second of the timer");

    // Parse Flags
    if (!flag_parse(argc, argv)) {
        usage(stderr);
        flag_print_error(stderr);
        exit(1);
    }

    if (*help) {
        usage(stdout);
        exit(0);
    }

    set_timer((int)*hour, (int)*minute, (int)*second);

    // Setup Raylib
    SetTraceLogLevel(LOG_NONE);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "Jamly");
    // SetTargetFPS(120);
    while(!WindowShouldClose()) {
        // Logic
        text_timer = get_timer_text();

        if ((int)seconds_since_start < (int)GetTime()) {
            seconds_since_start = GetTime();
            if (*countdown_mode) {
                if (is_timer_zero()) {
                    toggle_minus();
                    *countdown_mode = false;
                    timer_add_one(); // sets to 0
                    timer_add_one(); // set to 1
                }
                timer_minus_one();
            }
            else timer_add_one();
        }

        timer_font_size = GetScreenWidth() / 6.0;
        text_size = MeasureTextEx(GetFontDefault(), text_timer, timer_font_size, TEXTSPACING);

        // Draw
        BeginDrawing();
        ClearBackground(ROSEVUBLACK);

        timer_x = GetScreenWidth() / 2 - text_size.x / 2;
        timer_y = GetScreenHeight() / 2 - text_size.y / 2;
        DrawTextEx(GetFontDefault(), text_timer, (Vector2){ timer_x, timer_y }, timer_font_size, TEXTSPACING, ROSEVUWHITE);
        EndDrawing();
    }

    CloseWindow();
}
