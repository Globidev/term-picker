#include "Display.hpp"

#include "Collection.hpp"

namespace curses {

    Display::Display() {
        setupTerm();
        noecho();
        keypad(stdscr, 1);
        curs_set(0);
    }

    Display::~Display() {
        if (!isendwin())
            quit();
    }

    Display & Display::instance() {
        static Display display;
        return display;
    }

    void Display::show(const Collection & collection) {
        auto & self = instance();

        int height,_;
        getmaxyx(stdscr, height, _);
        int currentIndex = collection.currentIndex();
        int displayedIndex = 0;
        for (int i = 0; i < currentIndex; ++i)
            displayedIndex += collection.items()[i].get().visible;

        clear();
        int start = std::max(0, displayedIndex - height + 1);
        int maxSize = collection.items().size();
        int printed = 0;
        for (int i = start; printed < height && i < maxSize; ++i) {
            auto item = collection.items()[i].get();
            if (item.visible) {
                self.showItem(item, i == currentIndex);
                ++printed;
            }
        }
        refresh();
    }

    void Display::quit() {
        auto & self = instance();

        endwin();
        delscreen(self.screen_);
    }

    void Display::showItem(const Item & item, bool current) const {
        auto line = std::string(item.level * 3, ' ') + "|--" + item.name + "\n";
        if (current || item.selected)
            attron(A_REVERSE);
        printw(line.c_str());
        if (current || item.selected)
            attroff(A_REVERSE);
    }

    void Display::setupTerm() {
        // outFile = stdout;
        outFile = fopen(ttyname(STDOUT_FILENO), "w");
        inFile = fopen(ttyname(STDIN_FILENO), "r");

        screen_ = newterm(getenv("TERM"), outFile, inFile);
    }

}
