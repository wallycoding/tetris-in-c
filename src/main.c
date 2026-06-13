#include "tetris.h"

int main(void) {

  InitWindow(GAME_W, GAME_H, "Tetris");
  InitAudioDevice();
  game_state_t game_state = init_game();
  PlayMusicStream(game_state.music);

  SetTargetFPS(60);
  while(!WindowShouldClose()) {
    UpdateMusicStream(game_state.music);

    BeginDrawing();
    ClearBackground(GAME_BG);

    game_control(&game_state);
    draw_menu(&game_state);
    draw_grid_cells(&game_state);
    draw_piece(&game_state.current);
    draw_grid();

    EndDrawing();
  }

  close_game(&game_state);
  CloseAudioDevice();
  CloseWindow();
  return 0;
}