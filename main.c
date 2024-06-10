#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// clang-format off
#define CONSOLE_NO_WRAP            "\033[?7l"
#define CONSOLE_WRAP               "\033[?7h"
#define CLEAR_SCREEN_TO_BEGINNING  "\033[3J"
#define X_Y_CURSOR_POS_RESET       "\033[1;1H"
#define INVIS_CURSOR               "\033[?25l"
#define VIS_CURSOR                 "\033[?25h"
// #define CHAR_BUFF                  72
#define CHAR_BUFF                  67
#define COL_BUFF                   128
#define ROW_BUFF                   64
#define COL_SPACER                 " "
// clang-format on

// char *matrix[CHAR_BUFF] = {
//     "日", "二", "ﾊ", "ﾐ", "ﾋ", "ｰ", "ｳ", "ｼ", "ﾅ", "ﾓ",  "ﾆ", "ｻ",
//     "ﾜ",  "ﾂ",  "ｵ", "ﾘ", "ｱ", "ﾎ", "ﾃ", "ﾏ", "ｹ", "ﾒ",  "ｴ", "ｶ",
//     "ｷ",  "ﾑ",  "ﾕ", "ﾗ", "ｾ", "ﾈ", "ｽ", "ﾀ", "ﾇ", "ﾍ",  "ｦ", "ｲ",
//     "ｸ",  "ｺ",  "ｿ", "ﾁ", "ﾄ", "ﾉ", "ﾌ", "ﾔ", "ﾖ", "ﾙ",  "ﾚ", "ﾛ",
//     "ﾝ",  "0",  "1", "2", "3", "4", "5", "7", "8", "9",  ":", "・",
//     ".",  "\"", "=", "*", "+", "-", "<", ">", "¦", "｜", "ç", "_"};

char *matrix[CHAR_BUFF] = {
    "ﾊ", "ﾐ", "ﾋ", "ｰ", "ｳ", "ｼ", "ﾅ", "ﾓ", "ﾆ", "ｻ", "ﾜ", "ﾂ", "ｵ", "ﾘ",
    "ｱ", "ﾎ", "ﾃ", "ﾏ", "ｹ", "ﾒ", "ｴ", "ｶ", "ｷ", "ﾑ", "ﾕ", "ﾗ", "ｾ", "ﾈ",
    "ｽ", "ﾀ", "ﾇ", "ﾍ", "ｦ", "ｲ", "ｸ", "ｺ", "ｿ", "ﾁ", "ﾄ", "ﾉ", "ﾌ", "ﾔ",
    "ﾖ", "ﾙ", "ﾚ", "ﾛ", "ﾝ", "0", "1", "2", "3", "4", "5", "7", "8", "9",
    ":", ".", "=", "*", "+", "-", "<", ">", "¦", "ç", "_"};

int print_matrix[ROW_BUFF][COL_BUFF];
int shuffle_y_pos[COL_BUFF];
int reverse_scroll[COL_BUFF];

int m_random(int min, int max);
int matrix_random();
void print_grid(bool scroll);
void set_grid();
void scroll_grid(int pos_y);
void scroll_grid_r(int pos_y);
void shuffle_scroll(bool reverse);
static void finish(int sig);
static void clear_console();
void swap(int *a, int *b);

const int U_SECONDS = 62500;
const int MODULO_LIMITER = 5;

int main() {
  srand(time(NULL));
  (void)signal(SIGINT, finish);
  bool reverse_flow = true;
  bool should_scroll = true;

  printf(CONSOLE_NO_WRAP);
  printf(INVIS_CURSOR);
  clear_console();
  set_grid();

  for (int i = 0;; i++) {
    if (i % MODULO_LIMITER == 0) {
      i = 0;
      should_scroll = m_random(0, 1);
    }
    clear_console();
    print_grid(should_scroll);
    printf(X_Y_CURSOR_POS_RESET);
    shuffle_scroll(reverse_flow);
    fflush(stdout);
    should_scroll = false;
    usleep(U_SECONDS);
  }

  finish(0);

  return 0;
}

static void finish(int sig) {
  printf(CONSOLE_NO_WRAP);
  printf(VIS_CURSOR);
  clear_console();
  exit(0);
}

int m_random(int min, int max) {
  return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

int matrix_random() {
  int max = CHAR_BUFF - 1;
  int min = 0;
  int index = min + rand() / (RAND_MAX / (max - min + 1) + 1);
  return index;
}

void set_grid() {
  for (int i = 0; i < ROW_BUFF; i++) {
    for (int j = 0; j < COL_BUFF; j++) {
      print_matrix[i][j] = matrix_random();
    }
  }
}

void scroll_grid(int pos_y) {
  for (int i = 0; i < ROW_BUFF - 1; i++) {
    swap(&print_matrix[i][pos_y], &print_matrix[i + 1][pos_y]);
  }
  print_matrix[ROW_BUFF - 1][pos_y] = matrix_random();
}

void scroll_grid_r(int pos_y) {
  for (int i = ROW_BUFF - 1; i > 0; i--) {
    swap(&print_matrix[i][pos_y], &print_matrix[i - 1][pos_y]);
  }
  print_matrix[0][pos_y] = matrix_random();
}

void shuffle_scroll(bool reverse) {
  for (int i = 0; i < COL_BUFF; i++) {
    if (!shuffle_y_pos[i])
      continue;
    // if (reverse_scroll[i]) {
    if (reverse) {
      scroll_grid_r(i);
      continue;
    }
    scroll_grid(i);
  }
}

void print_grid(bool scroll) {
  for (int i = 0; i < ROW_BUFF; i++) {
    for (int j = 0; j < COL_BUFF; j++) {
      if (i == ROW_BUFF - 1 && scroll) {
        shuffle_y_pos[j] = m_random(0, 1);
        reverse_scroll[j] = m_random(0, 1);
      }
      printf(COL_SPACER "%s" COL_SPACER, matrix[print_matrix[i][j]]);
    }
    printf("\n");
  }
}

static void clear_console() {
  printf(CLEAR_SCREEN_TO_BEGINNING);
  printf(X_Y_CURSOR_POS_RESET);
  fflush(stdout);
};

void swap(int *a, int *b) {
  int tmp = *a;
  *a = *b;
  *b = tmp;
}
