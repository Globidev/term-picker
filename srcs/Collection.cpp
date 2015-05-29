#include "Collection.hpp"

#include "options/Options.hpp"

Collection::Collection():
    currentIdx_ { 0 } {
    // Build map
    for (const auto & itemName: Options::items)
        addItem(itemName);

    // Build vector
    for (auto & itemPair: itemMap_)
        flatten(itemPair.second);
}

const Collection::Items & Collection::items() const {
    return items_;
}

Collection::Items::size_type Collection::currentIndex() const {
    return currentIdx_;
}

void Collection::prev() {
    do {
        if (currentIdx_ == 0)
            currentIdx_ = items_.size() - 1;
        else
            --currentIdx_;
    } while (!current().visible);
}

void Collection::next() {
    do {
        currentIdx_ = (currentIdx_ + 1) % items_.size();
    } while (!current().visible);
}

void Collection::select() {
    current().selected = !current().selected;
}

void Collection::expand() {
    current().expanded = !current().expanded;
    for (auto & childPair: current().childMap)
        setVisible(childPair.second, current().expanded);
}

void Collection::show() {
    for (const auto & item: items_)
        if (item.get().selected)
            std::cout << item << std::endl;
}

Item & Collection::current() {
    return items_.at(currentIdx_);
}

void Collection::addItem(const std::string & name) {
    auto first = name.begin();
    auto last = name.end();

    auto onParsed = [this](const auto & components) {
        auto inserted = itemMap_.emplace(
            components.front(),
            components.front()
        );

        auto item = std::ref(inserted.first->second);
        auto it = std::next(components.begin());
        for (; it != components.end(); ++it)
            item = item.get()[*it];
    };

    bool parseOK = qi::parse(
        first,
        last,
        (qi::as_string[*(qi::char_ - '/')] % '/')[onParsed]
    );
    (void)parseOK;
}

void Collection::flatten(Item & item) {
    items_.emplace_back(item);

    for (auto & childPair: item.childMap)
        flatten(childPair.second);
}

void Collection::setVisible(Item & item, bool visible) {
    item.visible = visible;
    for (auto & childPair: item.childMap)
        setVisible(childPair.second, item.expanded && visible);
}
