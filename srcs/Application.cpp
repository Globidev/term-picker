#include "Application.hpp"

#include "data/Collection.hpp"

#include "curses/Display.hpp"

#include "options/Options.hpp"

#include "tools/levenshtein.hpp"
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
    Collection filtered { Options::items, false };
    std::reference_wrapper<Collection> current = collection;

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

    int input { };
    bool queryMode { };
    std::string query;

    while (running) {

        if (input != ERR)
            curses::Display::show(current);

        input = getch();
        if (queryMode) {
            if (input != ERR) {
                switch (input) {
                    case 27:
                        query.clear();
                        current = collection;
                        queryMode = false;
                        break;
                    default:
                        query += input;
                        auto score = [&](const auto & s1) {
                            auto ld = levenshtein_distance(s1, query);
                            auto l = std::max(s1.size(), query.size());
                            return 1. - (double)ld / l;
                        };
                        auto distance = [&](const auto & s1, const auto & s2) {
                            return score(s1) > score(s2);
                        };
                        std::vector<std::string> items;
                        std::sort(
                            Options::items.begin(),
                            Options::items.end(),
                            distance
                        );
                        // for (auto item: Options::items) {
                        //
                        // }
                        // items.insert(items.begin(), query);
                        // for (auto & item: items)
                            // item += std::to_string(levenshtein_distance(item, query));
                        filtered = Collection { Options::items, false };
                        break;
                }
            }
        }
        else {
            switch (input) {
                case 27:
                case 'q':        quit();                            break;
                case ' ':
                case '\n':       select();                          break;
                case KEY_UP:     collection.previous();             break;
                case KEY_DOWN:   collection.next();                 break;
                case KEY_RIGHT:  collection.current().toggle();     break;
                case 'e':        collection.current().deepToggle(); break;
                case KEY_RESIZE: resize();                          break;
                case '/':        current = filtered; queryMode = true;  break;
                default:                                            break;
            }
        }
    }
}
