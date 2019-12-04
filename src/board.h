#ifndef BOARD_H
#define BOARD_H

#include "stdlib.h"
#include "string.h"

class board
{
public:

    enum ptype {
        PAWN,
        KNIGHT,
        BISHOP,
        KING,
        ROOK,
        QUEEN,
        EMPTY
    };

    struct piece {
        bool iswhite;
        ptype piece;
    };

    struct move {
        int s1; int s2; int d1; int d2; piece p;
    };

    board(); // construct a default chess board
    board(std::string fen); // construct board from FEN notation
    ~board();

    std::set<move> PossibleMoves();
    piece MakeMove(move m);
    void ToString();

private:
    piece _board[8][8]; // ["a-h"]["1-8"] relative to PGN
    bool canWhiteCastle = true;
    bool canBlackCastle = true;
    bool sideToMove = false; // False is white, True is black

    std::map<ptype, std::string> typeToStr = {{PAWN, "p"}, {KNIGHT, "n"}, {BISHOP, "b"}, {KING, "k"}};
};

#endif // BOARD_H
