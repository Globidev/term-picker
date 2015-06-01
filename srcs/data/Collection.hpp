#pragma once

#include "Item.hpp"

struct Collection: public std::vector<std::shared_ptr<Item>> {

    using ItemMap = std::map<std::string, std::shared_ptr<Item>>;

    Collection(const std::vector<std::string> &);

    size_type currentIndex() const;
    Item & current();

    void previous();
    void next();

private:

    void addItem(const std::string &);

    ItemMap topLevelItems_;
    size_type currentIdx_;

};
