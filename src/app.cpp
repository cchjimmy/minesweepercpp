#include "app.hpp"
#include "minesweeper.hpp"
#include <cstdlib>
#include <curses.h>
#include <string>

App::App(int width, int height)
    : m_width(width), m_height(height), m_window(initscr()) {}

App::~App() { endwin(); }

void printBoard(MineSweeper *ms, int width, int height, int x = 0, int y = 0,
                int marginX = 0, int marginY = 0) {
  auto board = ms->getBoard();
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      mvprintw(i + y + (i + 1) * marginY, j + x + (j + 1) * marginX,
               std::to_string(board[j + i * width]).c_str());
    }
  }
}

std::string getStatsString(MineSweeper *ms) {
  std::string stats;
  stats.append("Seed: ");
  stats.append(std::to_string(ms->getSeed()));
  stats.append("\n");
  stats.append("Num mines: ");
  stats.append(std::to_string(ms->numMines()));
  stats.append("\n");
  stats.append("Percentage mines: ");
  stats.append(std::to_string((float)ms->numMines() /
                              (ms->getWidth() * ms->getHeight())));
  return stats;
}

void App::run() {
  int width = 12;
  int height = 10;

  MineSweeper ms(width, height);
  ms.setSeed(100);

  while (true) {
    ms.newGame(0.1);
    auto board = ms.getBoard();
    printBoard(&ms, width, height);
    mvprintw(12, 0, getStatsString(&ms).c_str());

    refresh();
    getch();
  }
}
