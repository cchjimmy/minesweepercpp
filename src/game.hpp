#pragma once

#include "app.hpp"
#include "minesweeper.hpp"
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <curses.h>
#include <form.h>
#include <format>
#include <string>
#include <vector>

#define GREEN_PAIR 1
#define BOMB_PAIR 2
#define DEFAULT_PAIR 3
#define BLACK_PAIR 4
#define DARK_GREEN_PAIR 5
#define COLOR_DARK_GREEN 8
#define CURSOR_PAIR 6

template <typename T>
void print2D(std::vector<T> v, int width, int height, int x = 0, int y = 0,
             int marginX = 0, int marginY = 0) {
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      mvprintw(i + y + (i + 1) * marginY, j + x + (j + 1) * marginX,
               std::to_string(v[j + i * width]).c_str());
    }
  }
}

void printBoard(MineSweeper &ms, int cursor, int x, int y) {
  auto board = ms.getBoard();
  auto nums = ms.getNumbers();
  for (int i = 0, h = ms.getHeight(); i < h; i++) {
    for (int j = 0, w = ms.getWidth(); j < w; j++) {
      auto value = board[j + i * w];
      std::string str = "  ";
      if (value & MASKED) {
        attron(COLOR_PAIR((i + j) % 2 > 0 ? GREEN_PAIR : DARK_GREEN_PAIR));
        if (value & FLAGGED)
          str[1] = 'F';
      } else {
        if (value & MINE) {
          attron(COLOR_PAIR(BOMB_PAIR));
        }
        if (value & EMPTY) {
          attron(COLOR_PAIR(DEFAULT_PAIR));
          str[1] = nums[j + i * w] > 0 ? '0' + nums[j + i * w] : ' ';
        }
      }
      if (j + i * w == cursor) {
        attron(COLOR_PAIR(CURSOR_PAIR));
      }
      mvprintw(i + y, j * 2 + x, str.c_str());
    }
  }
}

void buildStatusString(MineSweeper &ms, std::string &str) {
  str.clear();
  str.append(
      "Keys: r - new game, c - configure,\n      f - flag cell, space - reveal "
      "cell\n");
  str.append("Movement: w/a/s/d, h/j/k/l, arrow keys\n");
  str.append("Seed: ");
  str.append(std::to_string(ms.getSeed()));
  str.append(", ");
  str.append("Size: ");
  str.append(std::to_string(ms.getWidth()) + "x" +
             std::to_string(ms.getHeight()));
  str.append(", ");
  str.append("Flag count: ");
  str.append(std::to_string(ms.getNumFlags()));
  str.append("\n");
  str.append("# Mines: ");
  str.append(std::to_string(ms.getNumMines()));
  str.append(", ");
  str.append("Percentage mines: ");
  str.append(std::to_string(ms.getDensity()));
  str.append("\n");
}

int countLines(std::string s) {
  return std::count(s.begin(), s.end(), '\n') + 1;
}

std::string hhmmss(time_t time) {
  int hours = time / (60 * 60);
  int minutes = time / (60) - hours * 60;
  int seconds = time % 60;
  return std::format("{:0>2}:{:0>2}:{:0>2}", hours, minutes, seconds);
}

class Game : public App {
public:
  Game() {
    initscr();
    curs_set(0);
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    start_color();
    init_color(COLOR_DARK_GREEN, (124 * 1000 / 256), (252 * 1000 / 256), 0);
    init_pair(GREEN_PAIR, COLOR_WHITE, COLOR_GREEN);
    init_pair(BOMB_PAIR, COLOR_RED, COLOR_RED);
    init_pair(DEFAULT_PAIR, COLOR_WHITE, COLOR_BLACK);
    init_pair(BLACK_PAIR, COLOR_BLACK, COLOR_BLACK);
    init_pair(DARK_GREEN_PAIR, COLOR_WHITE, COLOR_DARK_GREEN);
    init_pair(CURSOR_PAIR, COLOR_BLACK, COLOR_WHITE);
  }
  ~Game() { endwin(); }

  void configure(MineSweeper &ms) {
    const std::vector<std::string> prompts = {"Width:", "Height:", "# Mines:"};

    std::vector<FIELD *> field;
    field.resize(prompts.size() + 1);
    for (int i = 0, l = prompts.size(); i < l; i++) {
      field[i] = new_field(1, 10, i, prompts[i].size() + 1, 0, 0);
    };
    field[field.size() - 1] = NULL;

    set_field_type(field[0], TYPE_INTEGER, 0, 0, 1e10 - 1);
    set_field_type(field[1], TYPE_INTEGER, 0, 0, 1e10 - 1);
    set_field_type(field[2], TYPE_INTEGER, 0, 0, 1e10 - 1);

    set_field_back(field[0], A_UNDERLINE);
    set_field_back(field[1], A_UNDERLINE);
    set_field_back(field[2], A_UNDERLINE);
    field_opts_off(field[0], O_AUTOSKIP);
    field_opts_off(field[1], O_AUTOSKIP);
    field_opts_off(field[2], O_AUTOSKIP);

    // defaults
    set_field_buffer(field[0], 0, std::to_string(ms.getWidth()).c_str());
    set_field_buffer(field[1], 0, std::to_string(ms.getHeight()).c_str());
    set_field_buffer(field[2], 0, std::to_string(ms.getNumMines()).c_str());

    FORM *my_form = new_form(field.data());
    post_form(my_form);
    refresh();

    for (int i = 0, l = prompts.size(); i < l; i++) {
      mvprintw(i, 0, prompts[i].c_str());
    }
    mvprintw(prompts.size() + 1, 0, "Submit (F1)");
    refresh();

    int ch;
    bool submit = false;

    while (!submit) {
      ch = getch();
      switch (ch) {
      case KEY_F(1): {
        submit = true;
        for (int i = 0, l = prompts.size(); i < l; i++) {
          submit &= form_driver(my_form, REQ_NEXT_FIELD) == E_OK;
          if (!submit)
            break;
        }
        if (submit == true)
          break;
        mvprintw(
            prompts.size() + 2, 0,
            std::format("Invalid field {}", current_field(my_form)->index + 1)
                .c_str());
        refresh();
        break;
      }
      case '\n':
      case KEY_DOWN:
        /* Go to next field */
        form_driver(my_form, REQ_NEXT_FIELD);
        /* Go to the end of the present buffer */
        /* Leaves nicely at the last character */
        form_driver(my_form, REQ_END_LINE);
        break;
      case KEY_LEFT:
        form_driver(my_form, REQ_PREV_CHAR);
        break;
      case KEY_RIGHT:
        form_driver(my_form, REQ_NEXT_CHAR);
        break;
      case KEY_UP:
        /* Go to previous field */
        form_driver(my_form, REQ_PREV_FIELD);
        form_driver(my_form, REQ_END_LINE);
        break;
      case 127: // backspace
        form_driver(my_form, REQ_DEL_PREV);
        break;
      default:
        /* If this is a normal character, it gets */
        /* Printed				  */
        form_driver(my_form, ch);
        break;
      }
    }

    ms.setWidth(std::stoi(field_buffer(field[0], 0)));
    ms.setHeight(std::stoi(field_buffer(field[1], 0)));
    ms.setNumMines(std::stoi(field_buffer(field[2], 0)));

    unpost_form(my_form);
    free_form(my_form);
    for (int i = 0, l = prompts.size(); i < l; i++) {
      free_field(field[i]);
    }
  }

  void run() {
    int winWidth, winHeight;
    int width = 20, height = 20;

    MineSweeper ms(width, height);

    bool quit = false, gameEnded = false;
    END_STATES state = SAFE;
    int cursor = 0;
    std::string status;

    ms.setNumMines(width * height * 0.1);
    ms.newGame();
    buildStatusString(ms, status);

    time_t now = time(NULL);

    while (!quit) {
      if (state != SAFE && !gameEnded) {
        time_t diff = time(NULL) - now;
        now += diff;
        auto timeStr = hhmmss(diff);
        if (state == SUCCESS) {
          status.append(std::format("WIN, solve time: {}", timeStr));
        }
        if (state == FAIL) {
          status.append(std::format("GAME OVER, time wasted: {}", timeStr));
          ms.reveal();
        }
        cursor = -1;
        gameEnded = true;
      }

      erase();

      attron(COLOR_PAIR(DEFAULT_PAIR));
      mvprintw(0, 0, status.c_str());

      getmaxyx(stdscr, winHeight, winWidth);

      printBoard(ms, cursor, (winWidth) / 2 - width, countLines(status));

      refresh();

      const int input = getch();

      switch (input) {
      case 'q':
        quit = true;
        break;
      case 'r':
        ms.newGame();
        buildStatusString(ms, status);
        state = SAFE;
        gameEnded = false;
        cursor = 0;
        break;
      case 'c':
        curs_set(1);
        attron(COLOR_PAIR(DEFAULT_PAIR));
        Game::configure(ms);
        curs_set(0);
        width = ms.getWidth();
        height = ms.getHeight();
        ms.resize();
        ms.newGame();
        buildStatusString(ms, status);
        state = SAFE;
        gameEnded = false;
        cursor = 0;
        break;
      default:
        break;
      }

      if (state != SAFE)
        continue;

      switch (input) {
      case 'w':
      case 'k':
      case KEY_UP:
        if (cursor - width < 0)
          break;
        cursor -= width;
        break;
      case 's':
      case 'j':
      case KEY_DOWN:
        if (cursor + width >= width * height)
          break;
        cursor += width;
        break;
      case 'a':
      case 'h':
      case KEY_LEFT:
        if ((cursor % width) - 1 < 0)
          break;
        cursor -= 1;
        break;
      case 'd':
      case 'l':
      case KEY_RIGHT:
        if ((cursor % width) + 1 >= width)
          break;
        cursor += 1;
        break;
      case ' ': {
        state = ms.progress(cursor);
        break;
      }
      case 'f':
        ms.setFlagMode(true);
        state = ms.progress(cursor);
        ms.setFlagMode(false);
        break;
      }
    }
  }
};
