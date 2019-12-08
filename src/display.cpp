#include "display.h"
#include "gobjects.h"
#include "ginteractors.h"
#include "board.h"

display::display() : GWindow(kCellWidth * BOARD_CELLS + kNonBoardWidth, kCellWidth * BOARD_CELLS)
{
    setWindowTitle(kWindowTitle);
    setAutoRepaint(false);
    DisplayCells();
    DisplaySidebar();
    repaint();
}

display::~display() {

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
    repaint();
}

void display::ShowBoard(board *b) {
    DisplayCells();
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            board::piece p = b->get(row, 7-col);
            if (p.piece == board::EMPTY) continue;
            std::string path = kPiecePath + (p.iswhite ? "w" : "b") + b->typeToStr[p.piece] + kSuffix;
            // Don't reallocate images if we don't need to
            GImage *img;
            if (imageStorage.count(p) != 0) {
                img = imageStorage[p];
            }
            else {
                img = new GImage(path);
                img->setSize(kCellWidth * kPercentOfCell, kCellWidth * kPercentOfCell);
                img->setX(0);
                img->setY(0);
                imageStorage[p] = img;
            }
            img->setX(row * kCellWidth + kCellWidth * (1-kPercentOfCell) / 2 );
            img->setY(col * kCellWidth + kCellWidth * (1-kPercentOfCell) / 2 );
            draw(img);
        }
    }
    repaint();
}

void display::DisplaySidebar() {
    GRect clear;
    clear.setX(8*kCellWidth);
    clear.setY(0);
    clear.setWidth(kNonBoardWidth);
    clear.setHeight(8*kCellWidth);
    clear.setFilled(true);
    clear.setLineWidth(0);
    clear.setFillColor("#ffffff");
    draw(clear);

    GText header("Chess Explorer");
    header.setFont(kHeader);
    header.setX(8 * kCellWidth + 30);
    header.setY(30);
    header.setColor(kTextColor);
    draw(header);

    GText mo(mode);
    mo.setFont(kPlain);
    mo.setX(8 * kCellWidth + 30);
    mo.setY(60);
    mo.setColor(kRedColor);
    draw(mo);

    GText user(userPrompt);
    user.setFont(kPlain);
    user.setX(8 * kCellWidth + 30);
    user.setY(90);
    user.setColor(kTextColor);
    draw(user);

    GText res(response);
    res.setFont(kPlain);
    res.setX(8 * kCellWidth + 30);
    res.setY(120);
    res.setColor(kTextColor);
    draw(res);

    repaint();
}

void display::ChangeModeText(std::string mode) {
    this->mode = mode;
}

void display::ChangePromptText(std::string text) {
    this->userPrompt = text;
}

void display::ChangeResponseMove(std::string response) {
    this->response = response;
}
