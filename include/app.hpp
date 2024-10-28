#pragma once

#include "curses.h"

class App {
    public:
        App(int width, int height);
        ~App();
        void run();
    private:
        int m_width, m_height;
        WINDOW *m_window;
};
