#pragma once

struct Options {

    static std::vector<std::string> items;
    static bool                     tail;
    static bool                     multiple;
    static bool                     noEcho;
    static std::string              command;
    static char                     separator;

    static void parseFromCommandLine(int, char **);

};
