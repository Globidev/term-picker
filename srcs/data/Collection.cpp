#include "Collection.hpp"

#include "options/Options.hpp"

Collection::Collection(const std::vector<std::string> & items, bool tree):
    currentIdx_ { 0 } {

    if (tree) {
        for (const auto & itemName: items)
            addItem(itemName);

        using Push = std::function<void (std::shared_ptr<Item>)>;
        Push push = [&](auto item) {
            this->push_back(item);
            item->forEachChild(push);
        };

        for (auto itemPair: topLevelItems_)
            push(itemPair.second);
    }
    else {
        for (const auto & itemName: items) {
            auto inserted = topLevelItems_.emplace(itemName, std::make_shared<Item>(itemName));
            push_back(inserted.first->second);
        }
    }
}

Collection::size_type Collection::currentIndex() const {
    return currentIdx_;
}

Item & Collection::current() {
    return *at(currentIdx_);
}

void Collection::previous() {
    do {
        if (currentIdx_ == 0)
            currentIdx_ = size() - 1;
        else
            --currentIdx_;
    } while (!current().isVisible());
}

void Collection::next() {
    do {
        currentIdx_ = (currentIdx_ + 1) % size();
    } while (!current().isVisible());
}

void Collection::addItem(const std::string & name) {
    auto onParsed = [this](const auto & components) {
        auto inserted = topLevelItems_.emplace(
            components.front(),
            std::make_shared<Item>(components.front())
        );

        auto item = inserted.first->second;
        auto it = std::next(components.begin());
        for (; it != components.end(); ++it)
            item = (*item)[*it];
    };

    qi::parse(
        name.begin(),
        name.end(),
        (
                qi::as_string[
                    *(qi::char_ - Options::separator)
                ]
            %   Options::separator
        )[onParsed]
    );
}
