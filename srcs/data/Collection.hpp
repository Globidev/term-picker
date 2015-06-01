#pragma once

#include "Item.hpp"

struct Collection {

    using ItemMap = std::map<std::string, Item>;
    using Items = std::vector<std::reference_wrapper<Item>>;

    Collection();

    const Items & items() const;
    Items::size_type currentIndex() const;

    void prev();
    void next();
    void select();
    void expand();
    void expandRecursive();

    void show();
    void execute();

private:

    Item & current();

    void addItem(const std::string &);
    void flatten(Item &);

    void setVisible(Item &, bool);
    void expandFromNode(Item &, bool);

    ItemMap itemMap_;
    Items items_;
    Items::size_type currentIdx_;

};
