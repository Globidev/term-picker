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

        clear();
        for (const auto & item: collection.items()) {
            if (item.get().visible)
                self.showItem(item, collection.isCurrent(item));
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
        attroff(A_REVERSE);
    }

    void Display::setupTerm() {
        outFile = stdout;
        inFile = stdin;
        if (!isatty(fileno(outFile)))
            outFile = fopen("/dev/tty", "w");
        if (!isatty(fileno(inFile)))
            inFile = fopen("/dev/tty", "r");

        screen_ = newterm(nullptr, outFile, inFile);
    }

}
