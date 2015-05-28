#pragma once

struct Item;
struct Collection;

namespace curses {

    struct Display: boost::noncopyable {

        ~Display();

        static Display & instance();

        static void show(const Collection &);

    private:

        Display();

        void showItem(const Item &, bool current) const;

    };

}
