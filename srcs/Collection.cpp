#include "Collection.hpp"

#include "options/Options.hpp"

Collection::Collection() {
    // Build map
    for (const auto & itemName: Options::items)
        addItem(itemName);

    // Build vector
    for (auto & itemPair: itemMap_)
        flatten(itemPair.second);

    currentIt_ = items_.begin();
}

const Collection::Items & Collection::items() const {
    return items_;
}

bool Collection::isCurrent(const Item & item) const {
    return &item == &currentIt_->get();
}

void Collection::prev() {
    do {
        if (currentIt_ == items_.begin())
            currentIt_ = std::prev(items_.end());
        else
            --currentIt_;
    } while (!current().visible);
}

void Collection::next() {
    do {
        ++currentIt_;
        if (currentIt_ == items_.end())
            currentIt_ = items_.begin();
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

Item & Collection::current() {
    return *currentIt_;
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
