#pragma once

struct Options {

    static std::vector<std::string> items;
    static bool                     tail;
    static bool                     single;
    static bool                     no_echo;
    static std::string              command;

    static void parseFromCommandLine(int, char **);

};
