#pragma once

struct Item {

    using ChildMap = std::map<std::string, Item>;
    using Parent = Item &;

    Item(const std::string &, boost::optional<Parent> = boost::none);

    Item & operator[](const std::string &);

    friend std::ostream & operator<<(std::ostream &, const Item &);

    std::string name;
    bool selected;
    bool expanded;
    bool visible;
    std::size_t level;
    boost::optional<Parent> parent;

    ChildMap childMap;
};
