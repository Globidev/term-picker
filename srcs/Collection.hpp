#pragma once

#include "Item.hpp"

struct Collection {

    using ItemMap = std::map<std::string, Item>;
    using Items = std::vector<std::reference_wrapper<Item>>;

    Collection();

    const Items & items() const;
    bool isCurrent(const Item &) const;

    void prev();
    void next();
    void select();
    void expand();

private:

    Item & current();

    void addItem(const std::string &);
    void flatten(Item &);

    void setVisible(Item &, bool);

    ItemMap itemMap_;
    Items items_;
    Items::iterator currentIt_;

};
