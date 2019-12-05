#include "engine.h"
#include "evalconst.h"
#include "board.h"
#include <iostream>
#include "stdio.h"

// Citations: In constructing this class, I referenced a few sources on the implementation of a Chess AI and on the Minimax Algorithm (with A/B pruning)
// https://medium.freecodecamp.com/simple-chess-ai-step-by-step-1d55a9266977
// https://en.wikipedia.org/wiki/Alpha–beta_pruning#Core_idea
// https://en.wikipedia.org/wiki/Minimax

engine::engine()
{

}

// Positive evaluation is an advantage for white
double engine::evalb(board *b) {
    double white = 0;
    double black = 0;

    for (int let = 0; let < 8; let++) {
        for (int num = 0; num < 8; num++) {
            board::piece p = b->get(let, num);
            if (p.iswhite) {
                if (p.piece == board::PAWN) white += PAWN_VALUE + PAWN_AT_POSITION_VALUES[num][let%4];
                if (p.piece == board::KNIGHT) white += KNIGHT_VALUE + KNIGHT_AT_POSITION_VALUES[num][let%4];
                if (p.piece == board::BISHOP) white += BISHOP_VALUE + BISHOP_AT_POSITION_VALUES[num][let%4];
                if (p.piece == board::ROOK) white += ROOK_VALUE + ROOK_AT_POSITION_VALUES[num][let%4];
                if (p.piece == board::QUEEN) white += QUEEN_VALUE + QUEEN_AT_POSITION_VALUES[num][let%4];
                if (p.piece == board::KING) white += KING_VALUE + KING_AT_POSITION_VALUES[num][let%4];
            }
            else {
                // The constant arrays are white-oriented, so we must convert the black coordinates so that we get the right value.
                if (p.piece == board::PAWN) black += PAWN_VALUE + PAWN_AT_POSITION_VALUES[7-num][let%4];
                if (p.piece == board::KNIGHT) black += KNIGHT_VALUE + KNIGHT_AT_POSITION_VALUES[7-num][let%4];
                if (p.piece == board::BISHOP) black += BISHOP_VALUE + BISHOP_AT_POSITION_VALUES[7-num][let%4];
                if (p.piece == board::ROOK) black += ROOK_VALUE + ROOK_AT_POSITION_VALUES[7-num][let%4];
                if (p.piece == board::QUEEN) black += QUEEN_VALUE + QUEEN_AT_POSITION_VALUES[7-num][let%4];
                if (p.piece == board::KING) black += KING_VALUE + KING_AT_POSITION_VALUES[7-num][let%4];
            }
        }
    }
    return white - black;
}

engine::moveoption engine::bestmove(board* b) {
    std::set<board::move> moves = b->PossibleMoves();
    double topValue = -10000000;
    board::move topMove;

    for (board::move cm : moves) {
        board *copy = new board;
        *copy = *b;
        copy->MakeMove(cm);
        double eval = minimax(copy, NOMINAL_MAX_DEPTH - 1, true, -1000000, 1000000);
        if (eval > topValue) {
            topValue = eval;
            topMove = cm;
        }
        delete copy; // memory management
    }

    return {topValue, topMove};
}

double engine::minimax(board* b, int depth, bool isMax, double alpha, double beta) {
    if (depth == 0) {
        return evalb(b);
    }

    if (isMax) {
        double bestValue = -10000000;
        std::set<board::move> moves = b->PossibleMoves();
        for (board::move option : moves) {
            board *copy = new board;
            *copy = *b;
            copy->MakeMove(option);
            bestValue = std::max(bestValue, minimax(copy, depth-1, alpha, beta, false));
            alpha = std::max(alpha, bestValue);
            delete copy;
            if (alpha > beta) break;
        }
        return bestValue;
    }
    else {
        double bestValue = 10000000;
        std::set<board::move> moves = b->PossibleMoves();
        for (board::move option : moves) {
            board *copy = new board;
            *copy = *b;
            copy->MakeMove(option);
            bestValue = std::min(bestValue, minimax(copy, depth-1, alpha, beta, false));
            beta = std::min(beta, bestValue);
            delete copy;
            if (beta > alpha) break;
        }
        return bestValue;
    }
}
