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
        std::string toString() {
            return ((char) ('a' + s1)) + std::to_string((s2+1)) + ((char) ('a' + d1)) + std::to_string(d2+1) + ")";
        }
    };

    friend bool operator<(const move &a, const move &b) {
        return (1000*a.d1 + 100*a.d2+10*a.s1 + a.s2) < (1000*b.d1 + 100 * b.d2 + 10*b.s1 + b.s2);
    }

    friend bool operator<(const piece &a, const piece &b) {
        return (2*a.piece + a.iswhite) < (2*b.piece + b.iswhite);
    }

    board(); // construct a default chess board
    board(std::string fen); // construct board from FEN notation
    ~board();

    std::set<move> PossibleMoves();

    piece MakeMove(move m);
    piece MakeMove(std::string m);

    void ToString();
    piece get(int row, int col);
    std::map<ptype, std::string> typeToStr = {{PAWN, "p"}, {KNIGHT, "n"}, {BISHOP, "b"}, {KING, "k"}, {QUEEN, "q"}, {EMPTY, " "}, {ROOK, "r"}};

    bool canWhiteCastle = true;
    bool canBlackCastle = true;
    bool sideToMove = false; // False is white, True is black

private:
    piece _board[8][8]; // ["a-h"]["1-8"] relative to PGN

    std::set<move> PossibleMovesWithColor();

    std::set<board::move> pawnMove(int let, int num, bool isWhite);
    std::set<board::move> knightMove(int let, int num, bool isWhite);
    std::set<board::move> bishopMove(int let, int num, bool isWhite);
    std::set<board::move> kingMove(int let, int num, bool isWhite, bool canCastle);
    std::set<board::move> queenMove(int let, int num, bool isWhite);
    std::set<board::move> rookMove(int let, int num, bool isWhite);

    bool confirmKingMove(bool isWhite, board::move mo);
    bool isKingChecked();

    bool inBounds(int let, int num);

};

#endif // BOARD_H
