#pragma once

struct Item {

    using ChildMap = std::map<std::string, std::shared_ptr<Item>>;
    using Parent = boost::optional<Item &>;

    Item(const std::string &, Parent parent = boost::none);
    Item(const Item &) = delete;
    Item & operator=(const Item &) = delete;

    const std::string & name() const;
    bool isSelected() const;
    bool isExpanded() const;
    bool isVisible() const;
    std::size_t level() const;
    Parent parent() const;

    bool isLeaf() const;
    ChildMap::size_type childCount() const;

    bool select();
    void toggle();
    void deepToggle();

    std::shared_ptr<Item> operator[](const std::string &);

    template <class F>
    void forEachChild(F f) {
        for (auto childPair: childMap_)
            f(childPair.second);
    }

    friend std::ostream & operator<<(std::ostream &, const Item &);

private:

    void setVisible(bool);
    void expand(bool);

    std::string name_;
    bool isSelected_;
    bool isExpanded_;
    bool isVisible_;
    std::size_t level_;
    Parent parent_;

    ChildMap childMap_;
};
