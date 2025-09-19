module raylib;

extern (C) {
    const uint FLAG_WINDOW_RESIZABLE = 0x00000004;
    
    struct Color {
        char r;
        char g;
        char b;
        char a;
    }
    
    void InitWindow(int width, int height, const char *title);
    void CloseWindow();
    bool WindowShouldClose();
    void SetTargetFPS(int fps);
    
    void SetWindowState(uint flags);

    double GetTime();
    float GetFrameTime();
    
    void BeginDrawing();
    void EndDrawing();
    
    void ClearBackground(Color color);
    Color GetColor(uint hexValue);
    
    void DrawText(const char *text, int posX, int posY, int fontSize, Color color);

    int MeasureText(const char *text, int fontSize);
    int GetScreenWidth();
    int GetScreenHeight();
}
