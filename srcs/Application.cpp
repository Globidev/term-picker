#include "Application.hpp"

#include "options/Options.hpp"

#include "curses/Display.hpp"

#include "Collection.hpp"

Application::Application(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Nothing to pick" << std::endl;
        exit(1);
    }

    Options::parseFromCommandLine(argc, argv);
}

void Application::run() {
    Collection collection;
    bool running = true;

    auto quit = [&] {
        curses::Display::quit();
        collection.show();
        running = false;
    };

    auto select = [&] {
        collection.select();
        if (Options::single)
            quit();
    };

    while (running) {
        curses::Display::show(collection);

        switch (getch()) {
            case 'q':       quit();                 break;
            case ' ':
            case '\n':      select();               break;
            case KEY_UP:    collection.prev();      break;
            case KEY_DOWN:  collection.next();      break;
            case KEY_RIGHT: collection.expand();    break;
            default:                                break;
        }

    }
}
