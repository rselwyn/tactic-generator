#include "display.h"
#include "gobjects.h"

display::display() : GWindow(kCellWidth * BOARD_CELLS + kNonBoardWidth, kCellWidth * BOARD_CELLS)
{
    setWindowTitle(kWindowTitle);
}

void display::DisplayCells() {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            GRect rect(row * kCellWidth, col * kCellWidth, kCellWidth, kCellWidth);
        }
    }
}
