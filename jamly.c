#include "raylib.h"

#define NOB_IMPLEMENTATION
#include "nob.h"

#define ROSEVUWHITE CLITERAL(Color){ 0xFF, 0xD9, 0xDA, 0xFF }
#define ROSEVUBLACK CLITERAL(Color){ 0x1B, 0x20, 0x21, 0xFF }

int timer = 0;

int main(void) {
    const char *text_timer = "";
    Vector2 text_size;
    float timer_font_size;
    int text_spacing = 20;
    // size_t temp_mark;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "Jamly");
    SetTargetFPS(60);
    double seconds_since_start = GetTime();
    while(!WindowShouldClose()) {
        text_timer = nob_temp_sprintf("%d", timer);
        // Start
        // temp_mark = nob_temp_save();

        if ((int)seconds_since_start < (int)GetTime()) {
            seconds_since_start = GetTime();

            timer += 1;
        }


        // Logic


        timer_font_size = GetScreenWidth() / 6.0;
        text_size = MeasureTextEx(GetFontDefault(), text_timer, timer_font_size, text_spacing);
        // text_size = (Vector2){ 20, 30};

        // Draw
        BeginDrawing();
        ClearBackground(ROSEVUBLACK);

        // DrawText(hello, GetScreenWidth() / 2 - text_size.x / 2, GetScreenHeight() / 2 - text_size.y / 2, timer_font_size, ROSEVUWHITE);
        DrawTextEx(GetFontDefault(), text_timer, (Vector2){GetScreenWidth() / 2 - text_size.x / 2, GetScreenHeight() / 2 - text_size.y / 2}, timer_font_size, text_spacing, ROSEVUWHITE);
        EndDrawing();

        // Finish
        // nob_temp_rewind(temp_mark);
    }

    CloseWindow();
}
