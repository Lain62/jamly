#include <format>
#include <string>

#include "raylib.h"

#define FLAG_IMPLEMENTATION
#include "flag.h"

const Color ROSEVUWHITE = { 0xFF, 0xD9, 0xDA, 0xFF };
const Color ROSEVUBLACK = { 0x1B, 0x20, 0x21, 0xFF };

const int TEXTSPACING = 20;

enum TimerHand {
    Hour,
    Minute,
    Second
};

class Timer {
public:
    int hour;
    int minute;
    int second;
    bool minus;
    Timer() {
        hour = 0;
        minute = 0;
        second = 0;
        minus = false;
    };

    std::string get_text() {
        std::string hour = to_string_clock(Hour);
        std::string minute = to_string_clock(Minute);
        std::string second = to_string_clock(Second);
        if (minus) return std::format("-{}:{}:{}", hour, minute, second);
        return std::format("{}:{}:{}", hour, minute, second);
    }
    bool is_zero(void) {
        if (hour > 0) return false;
        if (minute > 0) return false;
        if (second > 0) return false;
        return true;
    }

    void add_one(void) {
        second += 1;

        if (second >= 60) {
            second = 0;
            minute += 1;
        }

        if (minute >= 60) {
            minute = 0;
            hour += 1;
        }
    }

    void minus_one(void) {
        if (is_zero()) return;
        second -= 1;

        if (second < 0) {
            second = 59;
            if (minute >= 0) minute -= 1;
        }

        if (minute < 0) {
            minute = 59;
            if (hour >= 0) hour -= 1;
        }
    }

    void toggle_minus(void) {
        minus = !minus;
    }

    void set(int hour, int minute, int second) {
        this->hour = hour;
        this->minute = minute;
        this->second = second;
    }
private:
    std::string to_string_clock(TimerHand hand) {
        int num = 0;
        switch (hand) {
        case Hour: {
            num = hour;
        } break;
        case Minute: {
            num = minute;
        } break;
        case Second: {
            num = second;
        } break;
        }
        if (num < 10) {
            return std::format("0{}", num);
        } else {
            return std::format("{}", num);
        }
    }
};

Timer timer = Timer();

void usage(FILE *stream) {
    fprintf(stream, "./jamly [OPTIONS]\n");
    fprintf(stream, "OPTIONS:\n");
    flag_print_options(stream);
}

int main(int argc, char **argv) {
    // Setup Vars
    std::string text_timer;
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

    timer.set((int)*hour, (int)*minute, (int)*second);

    // Setup Raylib
    SetTraceLogLevel(LOG_NONE);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "Jamly");
    // SetTargetFPS(120);
    while(!WindowShouldClose()) {
        // Logic
        if ((int)seconds_since_start < (int)GetTime()) {
            seconds_since_start = GetTime();
            if (*countdown_mode) {
                if (timer.is_zero()) {
                    timer.toggle_minus();
                    *countdown_mode = false;
                    timer.add_one(); // sets to 0
                    timer.add_one(); // set to 1
                }
                timer.minus_one();
            }
            else timer.add_one();
        }

        text_timer = timer.get_text();
        timer_font_size = GetScreenWidth() / 6.0;
        text_size = MeasureTextEx(GetFontDefault(), text_timer.c_str(), timer_font_size, TEXTSPACING);

        // Draw
        BeginDrawing();
        ClearBackground(ROSEVUBLACK);

        timer_x = GetScreenWidth() / 2 - text_size.x / 2;
        timer_y = GetScreenHeight() / 2 - text_size.y / 2;
        // Todo: Make this its own function so you can have different colors
        DrawTextEx(GetFontDefault(), text_timer.c_str(), (Vector2){ (float)timer_x, (float)timer_y }, timer_font_size, TEXTSPACING, ROSEVUWHITE);
        EndDrawing();
    }

    CloseWindow();
}
