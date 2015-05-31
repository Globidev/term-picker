#include "Options.hpp"

std::vector<std::string>    Options::items   { };
bool                        Options::tail    { false };
bool                        Options::single  { false };
bool                        Options::no_echo { false };
std::string                 Options::command { "" };

static auto getUsage(void) {
    po::options_description usage { "Available options" };

    usage.add_options()
        ("help,h",      "Show this help message")
        ("items",       po::value(&Options::items),
                        "Items to pick")
        ("tail,t",      po::bool_switch(&Options::tail),
                        "Only display the tail of the item")
        ("single,s",    po::bool_switch(&Options::single),
                        "Ends the program on the first selection")
        ("no-echo",     po::bool_switch(&Options::no_echo),
                        "Do not output selected items")
        ("command,c",   po::value(&Options::command),
                        "Command to execute for each selected item")
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
