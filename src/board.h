#ifndef BOARD_H
#define BOARD_H

#include "stdlib.h"
#include <string>
#include <map>
#include <stack>
#include <set>
#include <unordered_map>

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
            return ((char) ('a' + s1)) + std::to_string((s2+1)) + ((char) ('a' + d1)) + std::to_string(d2+1);
        }
    };

    struct minimax_args {
        board* b; int depth; bool isMax; double alpha; double beta; double* writeVal; std::unordered_map<long, std::pair<double, int>> *tt;

        ~minimax_args() {
            delete b;
        }
    };

    friend bool operator<(const move &a, const move &b) {
        return (1000*a.d1 + 100*a.d2+10*a.s1 + a.s2) < (1000*b.d1 + 100 * b.d2 + 10*b.s1 + b.s2);
    }

    friend bool operator<(const piece &a, const piece &b) {
        return (2*a.piece + a.iswhite) < (2*b.piece + b.iswhite);
    }

    board(); // construct a default chess board
    board(std::string fen, bool isWhiteToPlay); // construct board from FEN notation
    ~board();

    std::vector<move> PossibleMoves();

    piece MakeMove(move m);
    piece MakeMove(std::string m);
    void UndoLast();

    void ToString();
    piece get(int row, int col);
    std::map<ptype, std::string> typeToStr = {{PAWN, "p"}, {KNIGHT, "n"}, {BISHOP, "b"}, {KING, "k"}, {QUEEN, "q"}, {EMPTY, " "}, {ROOK, "r"}};

    bool canWhiteCastle = true;
    bool canBlackCastle = true;
    bool sideToMove = false; // False is white, True is black

    std::string ToHashString();

    static long ZOBRIST_TABLE[8][8][12]; // Contains bitstrings to hash with
    static long ZOBRIST_WHITE_MOVE;
    static long ZOBRIST_BLACK_MOVE;

    static void InitializeHashTable();
    static long generateRandomBitString();

    long ZobristKey();
    int movesMade();

private:

    piece _board[8][8]; // ["a-h"]["1-8"] relative to PGN

    std::vector<move> PossibleMovesWithColor(bool includeKingMoves);
    std::stack<std::pair<move, piece>> performedMovesStack; // keeps track of captured pieces so that old moves can be replayed back

    std::set<board::move> pawnMove(int let, int num, bool isWhite);
    std::set<board::move> knightMove(int let, int num, bool isWhite);
    std::set<board::move> bishopMove(int let, int num, bool isWhite);
    std::set<board::move> kingMove(int let, int num, bool isWhite, bool canCastle);
    std::set<board::move> queenMove(int let, int num, bool isWhite);
    std::set<board::move> rookMove(int let, int num, bool isWhite);

    bool confirmNoKingProblems(bool isWhite, board::move mo);

    bool inBounds(int let, int num);

};

#endif // BOARD_H
