#pragma once

#include "rng.hpp"
#include <vector>

enum END_STATES { FAIL, SUCCESS, SAFE };
enum GAME_STATES { EMPTY, MINE, FLAGGED };

class MineSweeper {
public:
  MineSweeper(int width, int height);
  std::vector<int> getBoard();
  END_STATES progress(int index, bool flag);
  void newGame(double density = 0.5);
  int numMines();
  int getWidth();
  int getHeight();
  long getSeed();
  void setSeed(long seed);

private:
  int m_width, m_height, m_numMines, m_numFlags;
  std::vector<int> m_board;
  std::vector<int> m_numLayer;
  std::vector<int> m_count;
  RNG m_rng;
};
