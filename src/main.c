#include "tetris.h"

int main(void) {

  InitWindow(GAME_W, GAME_H, "Tetris");

  int r = 0;

  while(!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(GAME_BG);

    if (IsKeyPressed(KEY_UP)) r = (r+1) & 3; // 011

    draw_piece(TETROMINO_T, (gpos_t){3, 5}, r);
    draw_grid();

    EndDrawing();
  }

  CloseWindow();

  return 0;
}