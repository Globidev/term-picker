#include "Display.hpp"

#include "data/Collection.hpp"

#include "options/Options.hpp"

namespace curses {

    Display::Display():
        lowerBound { 0 } {

        setupTerm();
        raw();
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

        int currentIndex = collection.currentIndex();
        int displayedIndex = 0;
        for (int i = 0; i < currentIndex; ++i)
            displayedIndex += collection.at(i)->isVisible();
        if (displayedIndex >= self.lowerBound + self.height)
            self.lowerBound = displayedIndex - self.height + 1;
        else if (displayedIndex < self.lowerBound)
            self.lowerBound = displayedIndex;

        clear();

        int maxSize = collection.size();
        int printed = 0;
        for (int i = self.lowerBound; printed < self.height && i < maxSize; ++i) {
            auto item = collection.at(i);
            if (item->isVisible()) {
                showItem(*item, i == currentIndex);
                ++printed;
            }
        }

        refresh();
    }

    void Display::quit() {
        endwin();
        delscreen(instance().screen_);
    }

    void Display::updateHeight() {
        int _;
        getmaxyx(stdscr, height, _);
    }

    void Display::showItem(const Item & item, bool current) {
        std::string spacing(item.level() * 3, ' ');
        printw(spacing.c_str());

        if (current || item.isSelected())
            attron(A_REVERSE);
        printw("|--");
        attron(A_BOLD);
        printw(item.name().c_str());
        attroff(A_REVERSE | A_BOLD);

        if (!item.isLeaf()) {
            std::ostringstream childIndicator;
            childIndicator << "" << Options::separator << ".."
                           << " (" << item.childCount() << ")";
            printw(childIndicator.str().c_str());
        }
        printw("\n");
    }

    void Display::setupTerm() {
        outFile = isatty(STDOUT_FILENO) ? fopen(ttyname(STDOUT_FILENO), "w")
                                        : fopen("/dev/tty", "w");
        inFile = isatty(STDIN_FILENO) ? fopen(ttyname(STDIN_FILENO), "r")
                                      : fopen("/dev/tty", "r");
        screen_ = newterm(nullptr, outFile, inFile);
        updateHeight();
    }

}
