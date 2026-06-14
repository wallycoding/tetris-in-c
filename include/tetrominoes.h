#pragma once
#include <raylib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

typedef enum TETROMINO_TYPE_T {
  TETROMINO_I,
  TETROMINO_O,
  TETROMINO_T,
  TETROMINO_S,
  TETROMINO_Z,
  TETROMINO_J,
  TETROMINO_L,
} tetromino_type_t;

typedef struct INT32_VEC2_T {
  int32_t x;
  int32_t y;
} int32_vec2_t;

typedef int32_vec2_t tetromino_shape_t[4];

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
            .color = {255, 80, 250, 255},
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
            .color = {56, 149, 255, 200},
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