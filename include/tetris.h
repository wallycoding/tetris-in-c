#ifndef TETRIS_H
#define TETRIS_H

#include "tetrominoes.h"

#define GRID_ROWS 20
#define GRID_COLS 11
#define SQUARE_SIZE 30
#define GLINE_SIZE 2.0f
#define GMARGIN 30

#define GRID_W (GRID_COLS * SQUARE_SIZE)
#define GRID_H (GRID_ROWS * SQUARE_SIZE)
#define GRID_START_POSITION (gpos_t){(GRID_COLS/2), 1}

#define GAME_PANEL 300
#define GAME_W (GMARGIN * 2 + GRID_W + GAME_PANEL)
#define GAME_H (GMARGIN * 2 + GRID_H)

#define GAME_BG ((Color){24, 28, 25})

typedef struct PIECE_T {
  tetromino_type_t type;
  gpos_t position;
  int32_t rotation;
} piece_t;

typedef struct game_state_t {
  int32_t grid[GRID_ROWS][GRID_COLS];
  piece_t current;
  float fall_distance;
} game_state_t;

void draw_grid();
void draw_piece(piece_t* p);
void draw_grid_cells(game_state_t* gs);

game_state_t init_game();
bool has_locked(game_state_t* gs, piece_t* p);
bool has_collision(game_state_t* gs, piece_t* p);
void update_grid_cells(game_state_t* gs);
void next_piece(game_state_t* gs);
void game_control(game_state_t* gs);
void player_controls(game_state_t* gs);
void falling_control(game_state_t* gs);

#endif