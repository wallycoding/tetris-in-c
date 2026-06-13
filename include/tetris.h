#ifndef TETRIS_H
#define TETRIS_H

#include <raylib.h>
#include <stdint.h>

#define GRID_ROWS 20
#define GRID_COLS 10
#define SQUARE_SIZE 30
#define GLINE_SIZE 2.0f
#define GMARGIN 30

#define GRID_W (GRID_COLS * SQUARE_SIZE)
#define GRID_H (GRID_ROWS * SQUARE_SIZE)

#define GAME_PANEL 300
#define GAME_W (GMARGIN * 2 + GRID_W + GAME_PANEL)
#define GAME_H (GMARGIN * 2 + GRID_H)

#define GAME_BG ((Color){24, 28, 25})

typedef enum TETROMINO_TYPE_T {
  TETROMINO_I,
  TETROMINO_O,
  TETROMINO_T,
  TETROMINO_S,
  TETROMINO_Z,
  TETROMINO_J,
  TETROMINO_L,
} tetromino_type_t;

typedef struct GPOS_T {
  int32_t x;
  int32_t y;
} gpos_t;

typedef gpos_t tetromino_shape_t[4];

typedef struct TETROMINO_T {
  tetromino_shape_t shape;
  Color color;
} tetromino_t;

static tetromino_t tetrominoes[] =
    {
        [TETROMINO_I] =
            {
                .shape =
                    {
                        {-1, 0},
                        {0, 0},
                        {1, 0},
                        {2, 0},
                    },
                .color = {0, 255, 255, 255},
            },
        [TETROMINO_O] =
            {
                .shape =
                    {
                        {-1, 0},
                        {0, 0},
                        {-1, 1},
                        {0, 1},
                    },
                .color = {255, 255, 0, 255},
            },
        [TETROMINO_T] =
            {
                .shape =
                    {
                        {-1, 0},
                        {0, 0},
                        {0, -1},
                        {1, 0},
                    },
                .color = {128, 0, 128, 255},
            },
        [TETROMINO_S] =
            {
                .shape =
                    {
                        {-1, 0},
                        {0, 0},
                        {0, 1},
                        {1, 1},
                    },
                .color = {0, 255, 0, 255},
            },
        [TETROMINO_Z] =
            {
                .shape =
                    {
                        {0, 0},
                        {1, 0},
                        {-1, 1},
                        {0, 1},
                    },
                .color = {255, 0, 0, 255},
            },
        [TETROMINO_J] =
            {
                .shape =
                    {
                        {-1, 0},
                        {0, 0},
                        {1, 0},
                        {-1, 1},
                    },
                .color = {0, 0, 255, 255},
            },
        [TETROMINO_L] =
            {
                .shape =
                    {
                        {-1, 0},
                        {0, 0},
                        {1, 0},
                        {1, 1},
                    },
                .color = {255, 165, 0, 255},
            },
};

void draw_grid();
void draw_piece(tetromino_type_t type, gpos_t gpos, int32_t r);

#endif