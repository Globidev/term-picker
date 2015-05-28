#include "Item.hpp"

Item::Item(const std::string & name, boost::optional<Parent> parent):
    name     { name },
    selected { false },
    expanded { false },
    visible  { !parent },
    level    { parent ? parent->level + 1 : 0 },
    parent   { parent }
{ }

Item & Item::operator[](const std::string & name) {
    auto it = childMap.find(name);
    if (it != childMap.end())
        return it->second;

    auto inserted = childMap.emplace(name, Item { name, *this });
    return inserted.first->second;
}
