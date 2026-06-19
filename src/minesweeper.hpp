#pragma once

#include "rng.hpp"
#include <vector>

enum END_STATES { FAIL, SUCCESS, SAFE };
enum GAME_STATES {
  EMPTY = 1 << 0,
  MINE = 1 << 1,
  FLAGGED = 1 << 2,
  MASKED = 1 << 3
};

class MineSweeper {
public:
  MineSweeper(int width = 20, int height = 20);
  std::vector<int> getBoard();
  std::vector<int> getNumbers();
  END_STATES progress(int index);
  void newGame();
  bool getFlagMode();
  void setFlagMode(bool b);
  int getWidth();
  void setWidth(int width);
  int getHeight();
  void setHeight(int height);
  long getSeed();
  void setSeed(long seed);
  int getNumMines();
  void setNumMines(int numMines);
  double getDensity();
  int getNumFlags();
  void resize();
  void reveal();

private:
  void populateNumLayer();
  void populateMines();
  void fill(int index);
  int m_width, m_height, m_numMines;
  bool m_flagMode = false;
  std::vector<int> m_board;
  std::vector<int> m_numLayer;
  RNG m_rng;
};
