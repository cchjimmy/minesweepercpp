#include "minesweeper.hpp"
#include "rng.hpp"
#include <vector>

MineSweeper::MineSweeper(int width, int height)
    : m_width(width), m_height(height) {
  m_board.resize(m_width * m_height, EMPTY);
  m_numLayer.resize(m_width * m_height);
}

void MineSweeper::newGame(double density) {
  m_numMines = 0;
  for (int &space : m_board) {
    int chance = m_rng() > density ? 0 : 1;
    if (chance)
      m_numMines++;
    space = (GAME_STATES)chance;
  }
};

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

int MineSweeper::numMines() { return m_numMines; }
int MineSweeper::getWidth() { return m_width; }
int MineSweeper::getHeight() { return m_height; }
long MineSweeper::getSeed() { return m_rng.getSeed(); }
void MineSweeper::setSeed(long seed) { m_rng.setSeed(seed); }
