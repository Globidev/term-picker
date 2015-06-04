#include "Options.hpp"

std::vector<std::string>    Options::items      { };
bool                        Options::multiple   { false };
bool                        Options::noEcho     { false };
std::string                 Options::command    { "" };
char                        Options::separator  { '/' };
bool                        Options::show_tail      { false };

static auto getUsage(void) {
    po::options_description usage { "Available options" };

    usage.add_options()
        ("help,h",      "Show this help message")
        ("items",       po::value(&Options::items),
                        "Items to pick")
        ("multiple,m",  po::bool_switch(&Options::multiple),
                        "Ends the program on the first selection")
        ("no-echo",     po::bool_switch(&Options::noEcho),
                        "Do not output selected items")
        ("command,c",   po::value(&Options::command),
                        "Command to execute for each selected item")
        ("separator,s", po::value(&Options::separator),
                        "The character used to split items into a tree")
        ("show-tail,t",         po::bool_switch(&Options::show_tail),
                                "Only echoes the tail of selected item")
    ;

    return usage;
}

static auto getPositional(void) {
    po::positional_options_description usage;

    usage.add("items", -1);

    return usage;
}

void Options::parseFromCommandLine(int argc, char **argv) {
    po::variables_map options;
    po::command_line_parser parser { argc, argv };

    auto usage = getUsage();
    auto positional = getPositional();
    try {
        po::store(
            parser.options(usage)
                  .positional(positional)
                  .run(),
            options
        );
        if (options.count("help")) {
            std::cerr << usage << std::endl;
            exit(0);
        }
        po::notify(options);
    }
    catch (const po::error & exception) {
        std::cerr << exception.what() << std::endl << std::endl;
        std::cerr << usage << std::endl;
        exit(1);
    }
}
