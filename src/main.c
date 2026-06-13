#include "tetris.h"

int main(void) {

  InitWindow(GAME_W, GAME_H, "Tetris");
  game_state_t game_state = init_game();

  while(!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(GAME_BG);

    game_control(&game_state);
    draw_menu(&game_state);
    draw_grid_cells(&game_state);
    draw_piece(&game_state.current);
    draw_grid();

    EndDrawing();
  }

  CloseWindow();

  return 0;
}