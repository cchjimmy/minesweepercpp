#pragma once

#include <vector>

enum END_STATES { FAIL, SUCCESS, SAFE };
enum GAME_STATES { EMPTY, MINE, FLAGGED };

class MineSweeper {
public:
  MineSweeper(int width, int height);
  void restart();
  std::vector<int> getBoard();
  END_STATES progress(int index, bool flag);

private:
  int m_width, m_height, m_numMines, m_numFlags;
  std::vector<int> m_board;
  std::vector<int> m_numLayer;
};
