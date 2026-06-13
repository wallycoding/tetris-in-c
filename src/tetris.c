#include "tetris.h"

static inline gpos_t rotate_piece(int32_t r, gpos_t gpos) {
  for (int32_t i = 0; i < r; i++) {
    int32_t t = gpos.x;
    gpos.x = -gpos.y;
    gpos.y = t;
  }
  return gpos;
}

static inline Vector2 get_grid_pos(gpos_t gpos) {
  return (Vector2) {
    GMARGIN + gpos.x * SQUARE_SIZE,
    GMARGIN + gpos.y * SQUARE_SIZE
  };
}

void draw_grid() {

  for (int i = 0; i <= GRID_ROWS; i++) {

    // render y axis
    DrawLineEx(
      (Vector2) { GMARGIN, GMARGIN + (i * SQUARE_SIZE) },
      (Vector2) { GMARGIN + GRID_W, + GMARGIN + (i * SQUARE_SIZE) },
      GLINE_SIZE,
      DARKGREEN
    );

    if (i > GRID_COLS) continue;

    // render x axis
    DrawLineEx(
      (Vector2) { GMARGIN + (i * SQUARE_SIZE), GMARGIN },
      (Vector2) { GMARGIN + (i * SQUARE_SIZE), GMARGIN + GRID_H },
      GLINE_SIZE,
      DARKGREEN
    );

  }

}


void draw_piece(tetromino_type_t type, gpos_t gpos, int32_t r) {
  const tetromino_t* t = &tetrominoes[type];
  for (int32_t i = 0; i < 4; i++) {
    gpos_t rpos = rotate_piece(r, t->shape[i]);
    Vector2 pos = get_grid_pos((gpos_t){ gpos.x + rpos.x, gpos.y + rpos.y });
    DrawRectangleV(
      pos,
      (Vector2) {SQUARE_SIZE, SQUARE_SIZE},
      t->color
    );
  }

}