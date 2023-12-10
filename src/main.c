#include <stdio.h>

#include <raylib.h>

#define WINDOW_TITLE "Woki-Toki"

int main(void) {
  const int screenWidth  = 800;
  const int screenHeight = 600;

  InitWindow(screenWidth, screenHeight, WINDOW_TITLE);

  SetTargetFPS(60);

  SetExitKey(KEY_ESCAPE);

  while(!WindowShouldClose()) {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
