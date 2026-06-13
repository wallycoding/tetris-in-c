#pragma once
#include <raylib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

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

static tetromino_t tetrominoes[] = {
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
                    {-1, 1},
                    {0, 1},
                    {0, 0},
                    {1, 1},
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