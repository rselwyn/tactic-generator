#include "display.h"
#include "gobjects.h"
#include "board.h"

display::display() : GWindow(kCellWidth * BOARD_CELLS + kNonBoardWidth, kCellWidth * BOARD_CELLS)
{
    setWindowTitle(kWindowTitle);
    DisplayCells();
    repaint();
}

void display::DisplayCells() {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            GRect rect(row * kCellWidth, col * kCellWidth, kCellWidth, kCellWidth);
            rect.setFilled(true);
            rect.setFillColor(((row %2 + col)%2 == 0 ? kLight : kDark)); // binary expression just makes it so that the grid shows light and dark
            draw(rect);
        }
    }
}

void display::ShowBoard(board *b) {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            board::piece p = b->get(row, 7-col);
            if (p.piece == board::EMPTY) continue;
            std::string path = kPiecePath + (p.iswhite ? "w" : "b") + b->typeToStr[p.piece] + kSuffix;
            GImage *img = new GImage(path);
            img->setSize(kCellWidth * kPercentOfCell, kCellWidth * kPercentOfCell);
            img->setX(row * kCellWidth + kCellWidth * (1-kPercentOfCell) / 2 );
            img->setY(col * kCellWidth + kCellWidth * (1-kPercentOfCell) / 2 );
            draw(img);
        }
    }
    repaint();
}
