#pragma once

struct Options {

    static std::vector<std::string> items;
    static bool                     leaf_selection;
    static bool                     show_tail;
    static bool                     multiple;
    static bool                     noEcho;
    static std::string              command;
    static char                     separator;

    static void parseFromCommandLine(int, char **);

};
