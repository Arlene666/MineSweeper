#include "minesweeper.h"
#include <assert.h>
/////////////////////////////////////////////////////////////////////////////
// INTEGRITY INSTRUCTIONS

// Explicitly state the level of collaboration on this question
// Examples:
//   * I discussed ideas with classmate(s) [include name(s)]
//   * I worked together with classmate(s) in the lab [include name(s)]
//   * Classmate [include name] helped me debug my code
//   * I consulted website [include url]
//   * None
// A "None" indicates you completed this question entirely by yourself
// (or with assistance from course staff)
/////////////////////////////////////////////////////////////////////////////
// INTEGRITY STATEMENT:
// I received help from the following sources:

//   * None

// Name: Yu Zhang
// login ID: y2423zha
/////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////
// DO NOT CHANGE THESE CONSTANTS:
const char UNREVEALED = ' ' ;
const char REVEALED[9] = "012345678";
const char FLAG = 'F';
const char MINE = '*';
//////////////////////////////////////


bool flag(struct ms_board *b, int x, int y) {
  char *q = b->board;
  char w = b->width;
  char h = b->height;
  assert(*q);
  assert(w > 0);
  assert(h > 0);
  assert(x >= 1 && x <= h);
  assert(y >= 1 && y <= w);
  char *p = &(q[(y - 1) * w + x - 1]);
  if (*p == UNREVEALED) {
    *p = FLAG;
    return true;
  } else if (*p == FLAG) {
    *p = UNREVEALED;
    return true;
  } else {
    return false;
  }
}

// posn_member(p, posns, len) determines whether posn p
// is a member of posn array posns with length len
//requires: len >= 0
static bool posn_member(struct posn p, struct posn *posns, int len) {
  assert(len >= 0);
  if (len == 0) return false;
  for (int i = 0; i < len; ++i) {
    if ((posns[i]).x == p.x && (posns[i]).y == p.y) {
      return true;
    }
  }
  return false;
}

// mines_around(b, x, y) returns the number of mines 
// around (x, y) in ms_board b
// requires: *b is a valid ms_board
//           1 <= x <= b->width
//           1 <= y <= b->height
static int mines_around(struct ms_board *b, int x, int y) {
  int w = b->width;
  int h = b->height;
  int sum = 0;
  struct posn p = {0};
  for (int i = x - 1; i <= x + 1; ++i) {
    for (int j = y - 1; j <= y + 1; ++j) {
      if (i >= 1 && i <= h && j >= 1 && j <= w) {
        p.x = i;
        p.y = j;
        if (posn_member(p, (b->mines), (b->num_mines))) {
        ++sum;
        }
      }
    }
  }
  return sum;
}

bool reveal(struct ms_board *b, int x, int y) {
  char *q = b->board;
  int w = b->width;
  int h = b->height;
  assert(*q);
  assert(w > 0);
  assert(h > 0);
  assert(x >= 1 && x <= h);
  assert(y >= 1 && y <= w);
  struct posn *m = b->mines;
  char *p = &(q[(y - 1) * w + x - 1]);
  struct posn pn = {0};
  pn.x = x;
  pn.y = y;
  int n = mines_around(b, x, y);
  if (*p != UNREVEALED) {
    return false;
  } else if (posn_member(pn, m, (b->num_mines))) {
    *p = MINE;
    return true;
  } else if (n == 0) {
    *p = REVEALED[0];
    for (int i = x - 1; i <= x + 1; ++i) {
      for (int j = y - 1; j <= y + 1; ++j) {
        if (i >= 1 && i <= h && j >= 1 && j <= w) {
          reveal(b, i, j);
        }
      }
    }
    return true;
  } else {
    *p = REVEALED[n];
    return true;
  }
}

bool game_won(const struct ms_board *b) {
  int w = b->width;
  int h = b->height;
  char *q = b->board;
  assert(*q);
  assert(w > 0);
  assert(h > 0);
  struct posn p = {0};
  struct posn *m = b->mines;
  for (int i = 1; i <= h; ++i) {
    for (int j = 1; j <= w; ++j) {
      p.x = i;
      p.y = j;
      char *c = &(q[(j - 1) * w + i - 1]);
      if (posn_member(p, m, (b->num_mines))) {
        if(*c != FLAG) {
          return false;
        }
      } else {
        if (*c == UNREVEALED || *c == FLAG) {
          return false;
        }
      }
    }
  }
  return true;
}

bool game_lost(const struct ms_board *b) {
  int w = b->width;
  int h = b->height;
  char *q = b->board;
  assert(*q);
  assert(w > 0);
  assert(h > 0);
  int n = b->num_mines;
  struct posn *m = b->mines;
  for (int i = 0; i < n; ++i) {
    int s = (m[i]).x;
    int t = (m[i]).y;
    char *p = &(q[(t - 1) * w + s - 1]);
    if (*p == MINE) {
      return true;
    }
  }
  return false;
}

