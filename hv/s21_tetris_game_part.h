#include <curses.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
enum {
  Y = 22,
  X = 10,
};
enum Tetris_piece_type {
  PIECE_I,
  PIECE_O,
  PIECE_L,
  PIECE_J,
  PIECE_T,
  PIECE_S,
  PIECE_Z,
  NUMBER_OF_PIECES,
};

enum Tetris_rotation {
  ROTATION_NORMAL,
  ROTATION_LEFT,
  ROTATION_RIGHT,
  ROTATION_REVERSE,
};

typedef struct Tetris_coordinate {
  short x, y;
} Tetris_coordinate;

typedef struct Tetris_piece {
  enum Tetris_piece_type piece;
  enum Tetris_rotation rotation;
  struct Tetris_coordinate position;
} Tetris_piece;

extern int screen[Y][X];
extern int *ptr_screen[Y];
Tetris_piece get_random_piece(void);
void gameover_repeat_game(void);

enum { TET_GAMEOVER = 0, TET_PLAYING };
extern int delay;
extern int playing;
void move_down(void);
void move_bottom(void);
int move_left(void);
int move_right(void);
int rotate(void);
void start_new_game(void);
void get_next_piece(char next[4][4]);

#define PIECES_H
typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;
extern GameInfo_t game_info;
void userInput(UserAction_t action, bool hold);

GameInfo_t updateCurrentState();

extern const char tetris[NUMBER_OF_PIECES][4][4][4];

void print_matrix(void);

void print_score(void);

void refresh_screen(void);

void gameover_repeat_game(void);
void draw_windows(void);
int start_game(void);
void destroy_windows(void);

void init_graphics(void);
void init_game_start(void);

void main_loop(int delay);
