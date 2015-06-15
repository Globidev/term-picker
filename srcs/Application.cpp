#include "Application.hpp"

#include "data/Collection.hpp"
#include "curses/Display.hpp"
#include "options/Options.hpp"
#include "tools/exec.hpp"

Application::Application(int argc, char **argv) {
    Options::parseFromCommandLine(argc, argv);

    if (Options::items.empty()) {
        std::cerr << "Nothing to pick" << std::endl;
        exit(1);
    }
}

static void echoSelected(const Collection & collection) {
    for (auto item: collection)
        if (item->isSelected())
            std::cout << *item << std::endl;
}

static void executeSelected(const Collection & collection) {
    for (auto item: collection)
        if (item->isSelected()) {
            std::ostringstream oss;
            oss << *item;
            tools::execute(Options::command, oss.str());
        }
}

void Application::run() {
    bool running = true;
    Collection collection { Options::items };

    auto quit = [&] {
        curses::Display::quit();
        if (!Options::noEcho)
            echoSelected(collection);
        if (!Options::command.empty())
            executeSelected(collection);
        running = false;
    };

    auto select = [&] {
        if (collection.current().select()) {
            if (!Options::multiple)
                quit();
        }
        else
            collection.current().toggle();
    };

    auto resize = [] {
        curses::Display::instance().updateHeight();
    };

    while (running) {
        curses::Display::show(collection);

        switch (getch()) {
            case 'q':        quit();                            break;
            case ' ':
            case '\n':       select();                          break;
            case KEY_UP:     collection.previous();             break;
            case KEY_DOWN:   collection.next();                 break;
            case KEY_RIGHT:  collection.current().toggle();     break;
            case 'e':        collection.current().deepToggle(); break;
            case KEY_RESIZE: resize();                          break;
            default:                                            break;
        }

    }
}
