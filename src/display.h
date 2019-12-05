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
static const double kCellWidth = 60;
static const int kNonBoardWidth = 300;

// Piece Image Loading

static const std::string kPiecePath = "images/pieces/";
static const std::string kSuffix = ".png";
static const double kPercentOfCell = .90;

// Text

static const std::string kTextColor = "#000000";
static const std::string kHeader = "Courier-BOLD-21";
static const std::string kPlain = "Courier-PLAIN-12";
static const std::string kBold  = "Courier-BOLD-12";

class display : private GWindow {

    public:
        display();
        ~display();
        void ShowBoard(board *b);
    private:
        std::map<board::piece, GImage*> imageStorage;
        void DisplayCells();
        void DisplaySidebar();
};

#endif // DISPLAY_H
