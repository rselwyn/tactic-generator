#ifndef BOARD_H
#define BOARD_H

#include "stdlib.h"
#include <string>
#include <map>
#include <set>


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

    // NOTE: CASTLING IS RECORDED AS A KING MOVE
    struct move {
        // s = start, d = dest
        int s1; int s2; int d1; int d2; piece p; bool isCastle;
    };

    friend bool operator<(const move &a, const move &b) {
        return a.d1 < b.d1;
    }

    board(); // construct a default chess board
    board(std::string fen); // construct board from FEN notation
    ~board();

    std::set<move> PossibleMoves();
    piece MakeMove(move m);
    void ToString();
    piece get(int row, int col);
    std::map<ptype, std::string> typeToStr = {{PAWN, "p"}, {KNIGHT, "n"}, {BISHOP, "b"}, {KING, "k"}, {QUEEN, "q"}, {EMPTY, " "}, {ROOK, "r"}};

private:
    piece _board[8][8]; // ["a-h"]["1-8"] relative to PGN
    bool canWhiteCastle = true;
    bool canBlackCastle = true;
    bool sideToMove = false; // False is white, True is black

    std::set<board::move> pawnMove(int let, int num, bool isWhite);
    std::set<board::move> knightMove(int let, int num, bool isWhite);
    std::set<board::move> bishopMove(int let, int num, bool isWhite);
    std::set<board::move> kingMove(int let, int num, bool isWhite, bool canCastle);
    std::set<board::move> queenMove(int let, int num, bool isWhite);
    std::set<board::move> rookMove(int let, int num, bool isWhite);

    bool inBounds(int let, int num);

};

#endif // BOARD_H
