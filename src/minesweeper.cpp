#include "minesweeper.hpp"
#include "rng.hpp"
#include <ctime>
#include <vector>

MineSweeper::MineSweeper(int width, int height)
    : m_width(width), m_height(height) {
  MineSweeper::resize();
  MineSweeper::setSeed(time(NULL));
}

void MineSweeper::resize() {
  m_board.resize(m_width * m_height);
  m_numLayer.resize(m_width * m_height);
}

void MineSweeper::newGame() {
  MineSweeper::populateMines();
  MineSweeper::populateNumLayer();
};

std::vector<int> MineSweeper::getBoard() { return m_board; }

std::vector<int> MineSweeper::getNumbers() { return m_numLayer; }

bool testFlag(long mask, long flags) { return (mask & flags) == flags; }

void MineSweeper::populateMines() {
  int count = 0;
  for (int i = 0, l = m_board.size(); i < l; i++) {
    (m_board[i] =
         m_rng() > (double)(m_numMines - count) / (l - i) ? EMPTY : MINE);
    count += m_board[i] & MINE ? 1 : 0;
    m_board[i] |= MASKED;
  }
}

void MineSweeper::populateNumLayer() {
  std::fill(m_numLayer.begin(), m_numLayer.end(), 0);
  for (int i = 0, l = m_board.size(); i < l; i++) {
    if (m_board[i] & EMPTY)
      continue;
    for (int j = -1; j < 2; j++) {
      if (i % m_width + j < 0 || i % m_width + j > m_width - 1)
        continue;
      for (int k = -1; k < 2; k++) {
        m_numLayer[i + j + k * m_width] += 1;
      }
    }
  }
}

void MineSweeper::fill(int index) {
  m_board[index] &= ~MASKED;
  if (m_numLayer[index])
    return;
  for (int i = -1; i < 2; i++) {
    for (int j = -1; j < 2; j++) {
      if (i == 0 && j == 0)
        continue;
      if ((index % m_width) + j >= 0 && (index % m_width) + j < m_width &&
          index + i * m_width + j >= 0 &&
          index + i * m_width + j < m_width * m_height &&
          testFlag(m_board[index + i * m_width + j], EMPTY | MASKED)) {
        fill(index + i * m_width + j);
      }
    }
  }
}

END_STATES MineSweeper::progress(int index) {
  if (m_flagMode) {
    m_board[index] ^= FLAGGED;
  } else {
    if (testFlag(m_board[index], FLAGGED))
      return SAFE;
    MineSweeper::fill(index);
    if (testFlag(m_board[index], MINE)) {
      return FAIL;
    }
  }

  int maskCount = std::count_if(m_board.begin(), m_board.end(),
                                [](auto v) { return testFlag(v, MASKED); });
  if (maskCount == m_numMines)
    return SUCCESS;

  return SAFE;
}

void MineSweeper::reveal() {
  for (auto &v : m_board) {
    v &= ~MASKED;
  }
}

int MineSweeper::getNumMines() { return m_numMines; }
void MineSweeper::setNumMines(int numMines) { m_numMines = numMines; }
int MineSweeper::getWidth() { return m_width; }
void MineSweeper::setWidth(int width) { m_width = width; }
int MineSweeper::getHeight() { return m_height; }
void MineSweeper::setHeight(int height) { m_height = height; }
long MineSweeper::getSeed() { return m_rng.getSeed(); }
void MineSweeper::setSeed(long seed) { m_rng.setSeed(seed); }
bool MineSweeper::getFlagMode() { return m_flagMode; }
void MineSweeper::setFlagMode(bool b) { m_flagMode = b; }
double MineSweeper::getDensity() {
  return (double)m_numMines / (m_width * m_height);
}
int MineSweeper::getNumFlags() {
  return std::count_if(m_board.begin(), m_board.end(),
                       [](auto v) { return testFlag(v, FLAGGED | MASKED); });
}
