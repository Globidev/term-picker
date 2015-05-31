#pragma once

struct Item;
struct Collection;

namespace curses {

    struct Display: boost::noncopyable {

        ~Display();

        static Display & instance();

        static void show(const Collection &);
        static void quit();

        void updateHeight();

    private:

        Display();

        void setupTerm();
        static void showItem(const Item &, bool current);

        SCREEN *screen_;
        FILE *outFile, *inFile;

        int lowerBound, height;

    };

}
