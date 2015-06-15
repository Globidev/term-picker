#include "Item.hpp"

#include "options/Options.hpp"

Item::Item(const std::string & name, Parent parent):
    name_       { name },
    isSelected_ { false },
    isExpanded_ { false },
    isVisible_  { !parent },
    level_      { parent ? parent->level_ + 1 : 0 },
    parent_     { parent }
{ }

const std::string & Item::name() const {
    return name_;
}

bool Item::isSelected() const {
    return isSelected_;
}

bool Item::isExpanded() const {
    return isExpanded_;
}

bool Item::isVisible() const {
    return isVisible_;
}

std::size_t Item::level() const {
    return level_;
}

Item::Parent Item::parent() const {
    return parent_;
}

bool Item::isLeaf() const {
    return childMap_.empty();
}

Item::ChildMap::size_type Item::childCount() const {
    return childMap_.size();
}

bool Item::select() {
    if (!Options::leaf_selection || isLeaf()) {
        isSelected_ = !isSelected_;
        return true;
    }
    return false;
}

void Item::toggle() {
    isExpanded_ = !isExpanded_;

    forEachChild([=](auto child) {
        child->setVisible(isExpanded_);
    });
}

void Item::setVisible(bool visible) {
    isVisible_ = visible;

    forEachChild([=](auto child) {
        child->setVisible(visible && isExpanded_);
    });
}

void Item::deepToggle() {
    expand(!isExpanded_);

    forEachChild([=](auto child) {
        child->setVisible(isExpanded_);
    });
}

void Item::expand(bool expanded) {
    isExpanded_ = expanded;

    forEachChild([=](auto child) {
        child->expand(expanded);
    });
}

std::shared_ptr<Item> Item::operator[](const std::string & name) {
    auto it = childMap_.find(name);
    if (it != childMap_.end())
        return it->second;

    auto inserted = childMap_.emplace(
        name,
        std::make_shared<Item>(name, *this)
    );
    return inserted.first->second;
}

std::ostream & operator<<(std::ostream & os, const Item & item) {
    if (Options::show_tail)
        os << item.name();
    else {
        if (item.parent())
            os << *item.parent() << Options::separator;
        os << item.name();
    }
    return os;
}
