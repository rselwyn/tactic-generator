#ifndef DISPLAY_H
#define DISPLAY_H

#include "gobjects.h"
#include "gwindow.h"
#include "board.h"

#include "string.h"

#define BOARD_CELLS 8

// Graphics Constants

// Colors
static const std::string kDark = "#b48762";
static const std::string kLight = "#f0d8b5";

// Other
static const std::string kWindowTitle = "Chess Tactic Explorer";

// Dimensions
static const int kCellWidth = 40;
static const int kNonBoardWidth = 200;

class display : private GWindow {

    public:
        display();
    private:
        void DisplayCells();
        void ShowBoard(board *b);

};

#endif // DISPLAY_H
