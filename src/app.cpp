#include "app.hpp"
#include <curses.h>

App::App(int width, int height)
    : m_width(width), m_height(height), m_window(initscr()) {}

App::~App() { endwin(); }

void App::run() {
  move(10, 10);
  addstr("hello");
  refresh();
  getch();
}
