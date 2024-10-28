#include "minesweeper.hpp"
#include <vector>

MineSweeper::MineSweeper(int width, int height)
    : m_width(width), m_height(height) {
  m_board.resize(m_width * m_height, EMPTY);
  m_numLayer.resize(m_width * m_height);
}

void MineSweeper::restart() {
  m_board.clear();
  m_numLayer.clear();
}

std::vector<int> MineSweeper::getBoard() { return m_board; }

END_STATES MineSweeper::progress(int index, bool flag) {
  if (flag) {
    if (m_board[index] & FLAGGED) {
      m_board[index] &= ~FLAGGED;
      m_numFlags--;
    } else {
      m_board[index] |= FLAGGED;
      m_numFlags++;
    }
  } else {
    if (m_board[index] & MINE) {
      return FAIL;
    }
  }

  return SAFE;
}

bool allMinesFlagged(std::vector<int> board) { return false; }
