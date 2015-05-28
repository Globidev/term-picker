#pragma once

struct Item;
struct Collection;

namespace curses {

    struct Display: boost::noncopyable {

        ~Display();

        static Display & instance();

        static void show(const Collection &);
        static void quit();

    private:

        Display();

        void setupTerm();
        void showItem(const Item &, bool current) const;

        SCREEN *screen_;
        FILE *outFile, *inFile;

    };

}
