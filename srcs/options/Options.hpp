#pragma once

struct Options {

    static std::vector<std::string> items;
    static bool                     tail;
    static bool                     single;

    static void parseFromCommandLine(int, char **);

};
