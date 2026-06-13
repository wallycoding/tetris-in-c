#include "tetris.h"

// Private helpers

static inline tetromino_type_t rand_tetromino() {
  return xs32_rand() % 7;
}

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

// Draw functions 

void draw_grid() {
  for (int i = 0; i <= GRID_ROWS; i++) {
    // render y axis
    DrawLineEx(
      (Vector2) { GMARGIN, GMARGIN + (i * SQUARE_SIZE) },
      (Vector2) { GMARGIN + GRID_W, + GMARGIN + (i * SQUARE_SIZE) },
      GLINE_SIZE * (i == 0 || i == GRID_ROWS) ? 2 : 1,
      DARKGRAY
    );
    if (i > GRID_COLS) continue;
    // render x axis
    DrawLineEx(
      (Vector2) { GMARGIN + (i * SQUARE_SIZE), GMARGIN },
      (Vector2) { GMARGIN + (i * SQUARE_SIZE), GMARGIN + GRID_H },
      GLINE_SIZE * (i == 0 || i == GRID_COLS) ? 2 : 1,
      DARKGRAY
    );
  }
}

static void draw_square(Vector2 spos, Vector2 epos, Color color) {

  const int32_t border = GLINE_SIZE*2;

  DrawRectangleV(
    spos,
    epos,
    color
  );

  DrawRectangleV(
    (Vector2){ spos.x+border, spos.y },
    (Vector2){ epos.x-border, epos.y },
    (Color){
      color.r >> (unsigned char)1,
      color.g >> (unsigned char)1,
      color.b >> (unsigned char)1,
      color.a/2
    }
  );

  DrawRectangleV(
    (Vector2){ spos.x+(SQUARE_SIZE-(border * 3)- border * 3), spos.y+border+(SQUARE_SIZE-(border * 3)- border * 3) },
    (Vector2){ border * 4, border * border },
    (Color){
      color.r >> (unsigned char)1,
      color.g >> (unsigned char)1,
      color.b >> (unsigned char)1,
      color.a/2
    }
  );

}

void draw_piece(piece_t* p) {
  const tetromino_t* t = &tetrominoes[p->type];
  for (int32_t i = 0; i < 4; i++) {
    gpos_t rpos = rotate_piece(p->rotation, t->shape[i]);
    Vector2 pos = get_grid_pos((gpos_t){ p->position.x + rpos.x, p->position.y + rpos.y });
    draw_square(
      pos,
      (Vector2) {SQUARE_SIZE, SQUARE_SIZE},
      t->color
    );
  }
}

void draw_grid_cells(game_state_t* gs) {
  for (int32_t y = 0; y < GRID_ROWS; y++) {
    for (int32_t x = 0; x < GRID_COLS; x++) {
      if (!gs->grid[y][x]) continue;
      draw_square(
        get_grid_pos((gpos_t) {x, y}),
        (Vector2) {SQUARE_SIZE, SQUARE_SIZE},
        tetrominoes[gs->grid[y][x] - 1].color
      );
    }
  }
}

void draw_menu(game_state_t* gs) {

  const int32_t start_panel = GMARGIN * 2 + GRID_W;
  piece_t p = {.rotation=0};
  DrawText("Fun Tetris", start_panel, GMARGIN, 32, WHITE);
  for (int32_t i = 0; i < 4; i++) {
    p.type = gs->nexts[i];
    p.position = (gpos_t){GRID_COLS + 2, (i+1) * 3};
    draw_piece(&p);
  }

}

// Game functions 

game_state_t init_game() {
  game_state_t gs = {
    .grid={0},
    .current={
      .type=rand_tetromino(),
      .position=GRID_START_POSITION,
      .rotation=0
    },
    .nexts={0},
    .fall_distance=.0,
    .game_over=false,

    .sound_drop=LoadSound("./assets/drop.wav"),
    .sound_lineclear=LoadSound("./assets/lineclear.wav"),
    .sound_move=LoadSound("./assets/move.wav"),
    .sound_rotate=LoadSound("./assets/rotate.wav"),

    .music=LoadMusicStream("./assets/music.wav")

  };
  memset(&gs.grid, 0, sizeof(gs.grid) / sizeof(gs.grid[0][0]));
  for (int i = 0; i < 4; i++) gs.nexts[i] = rand_tetromino();
  return gs;
}

bool has_locked(game_state_t* gs, piece_t* p) {
  const tetromino_t* t = &tetrominoes[p->type];

  for (int32_t i = 0; i < 4; i++) {
    gpos_t rpos = rotate_piece(p->rotation, t->shape[i]);
    gpos_t pos = { p->position.x + rpos.x, p->position.y + rpos.y };
    if (pos.y > (GRID_ROWS-1) || gs->grid[pos.y + 1][pos.x] != 0) return true;
  }

  return false;
}

bool has_collision(game_state_t* gs, piece_t* p) {
  const tetromino_t* t = &tetrominoes[p->type];

  for (int32_t i = 0; i < 4; i++) {
    gpos_t rpos = rotate_piece(p->rotation, t->shape[i]);
    gpos_t pos = { p->position.x + rpos.x, p->position.y + rpos.y };

    if (pos.x < 0 || pos.y < 0) return true;
    if (pos.x > (GRID_COLS-1) || pos.y > (GRID_ROWS-1)) return true;
    if (gs->grid[pos.y][pos.x] != 0) return true;

  }

  return false;
}

void update_grid_cells(game_state_t* gs) {
  const tetromino_t* t = &tetrominoes[gs->current.type];

  for (int32_t i = 0; i < 4; i++) {
    gpos_t rpos = rotate_piece(gs->current.rotation, t->shape[i]);
    gpos_t pos = { gs->current.position.x + rpos.x, gs->current.position.y + rpos.y };
    gs->grid[pos.y][pos.x] = gs->current.type + 1;
  }

  int count_clear_lines = 0;
  for (int32_t y = GRID_ROWS-1; y >= 0; y--) {
    bool line_full = true;
    for (int32_t x = 0; x < GRID_COLS; x++) {
      if (!gs->grid[y][x]) {
        line_full = false;
        break;
      }
    }
    if (!line_full) continue;
    count_clear_lines++;
    memset(&gs->grid[y], 0, sizeof(gs->grid[0][0]) * GRID_COLS);
    for (int32_t yy = y; yy > 0; yy--) {
      for (int32_t x = 0; x < GRID_COLS; x++) {
        gs->grid[yy][x] = gs->grid[yy-1][x];
        gs->grid[yy-1][x] = 0;
      }
    }
    y++;
  }

  if (count_clear_lines) PlaySound(gs->sound_lineclear);
}

void next_piece(game_state_t* gs) {
  const tetromino_t* t = &tetrominoes[gs->current.type];
  gs->current = (piece_t){
    .type=gs->nexts[0],
    .position=GRID_START_POSITION,
    .rotation=0
  };
  if (check_game_over(gs)) return;
  for (int i = 1; i < 4; i++) gs->nexts[i - 1] = gs->nexts[i];
  gs->nexts[3] = rand_tetromino();
  gs->fall_distance = .0;
}

void game_control(game_state_t* gs) {
  if (gs->game_over) return;
  player_controls(gs);
  falling_control(gs);
}

void player_controls(game_state_t* gs) {

  piece_t pcopy = gs->current;
  switch(GetKeyPressed()) {
    case KEY_UP:
      if (gs->current.type == TETROMINO_O) break;
      pcopy.rotation = (gs->current.rotation+1) & 3;
      static const int32_t gaps[] = {0, 1, -2, 3, -4};
      for (int32_t i = 0; i < sizeof(gaps)/sizeof(gaps[0]); i++) {
        pcopy.position.x += gaps[i];
        if (!has_collision(gs, &pcopy)) break;
      }
      if (!has_collision(gs, &pcopy)) gs->current = pcopy;
      PlaySound(gs->sound_rotate);
      break;
    case KEY_LEFT:
      pcopy.position.x--;
      if (has_collision(gs, &pcopy)) break;
      gs->current.position = pcopy.position;
      PlaySound(gs->sound_move);
      break;
      case KEY_RIGHT:
      pcopy.position.x++;
      if (has_collision(gs, &pcopy)) break;
      gs->current.position = pcopy.position;
      PlaySound(gs->sound_move);
      break;
    case KEY_TAB:
      gs->current.type = (gs->current.type+1) % 7;
  }
}

void falling_control(game_state_t* gs) {
  float dt = GetFrameTime();
  gs->fall_distance += dt * 4.0f;

  float fd = gs->fall_distance;
  piece_t pcopy = gs->current;
  if (fd > (IsKeyDown(KEY_DOWN) ? 0.4f : 2.0f)) {
    pcopy.position.y++;
    fd = .0;
  }
  if (has_collision(gs, &pcopy)) {
    if (!has_locked(gs, &pcopy)) return;
    update_grid_cells(gs);
    next_piece(gs);
    PlaySound(gs->sound_drop);
    return;
  };
  gs->fall_distance = fd;
  gs->current = pcopy;
}

bool check_game_over(game_state_t* gs) {
  const tetromino_t* t = &tetrominoes[gs->current.type];
  if (
    has_collision(gs, &gs->current) && 
    has_locked(gs, &gs->current)
  ) gs->game_over = true;
  return gs->game_over;
}

void close_game(game_state_t* gs) {
  UnloadSound(gs->sound_drop);
  UnloadSound(gs->sound_lineclear);
  UnloadSound(gs->sound_move);
  UnloadSound(gs->sound_rotate);
  UnloadMusicStream(gs->music);
}