#include "engine.h"
#include "evalconst.h"
#include "board.h"
#include <iostream>
#include <chrono>

#include "stdio.h"

// Citations: In constructing this class, I referenced a few sources on the implementation of a Chess AI and on the Minimax Algorithm (with A/B pruning)
// https://medium.freecodecamp.com/simple-chess-ai-step-by-step-1d55a9266977
// https://en.wikipedia.org/wiki/Alpha–beta_pruning#Core_idea
// https://en.wikipedia.org/wiki/Minimax

double engine::kConsidered = 0;

// Positive evaluation is an advantage for white
double engine::evalb(board *b) {

    double white = 0;
    double black = 0;

    for (int let = 0; let < 8; let++) {
        for (int num = 0; num < 8; num++) {
            board::piece p = b->get(let, num);
            if (p.iswhite) {
                if (p.piece == board::PAWN) white += PAWN_VALUE + PAWN_AT_POSITION_VALUES[num][let < 4 ? let : 7-let];
                if (p.piece == board::KNIGHT) white += KNIGHT_VALUE + KNIGHT_AT_POSITION_VALUES[num][let < 4 ? let : 7-let];
                if (p.piece == board::BISHOP) white += BISHOP_VALUE + BISHOP_AT_POSITION_VALUES[num][let < 4 ? let : 7-let];
                if (p.piece == board::ROOK) white += ROOK_VALUE + ROOK_AT_POSITION_VALUES[num][let < 4 ? let : 7-let];
                if (p.piece == board::QUEEN) white += QUEEN_VALUE + QUEEN_AT_POSITION_VALUES[num][let < 4 ? let : 7-let];
                if (p.piece == board::KING) white += KING_VALUE + KING_AT_POSITION_VALUES[num][let < 4 ? let : 7-let];
            }
            else {
                // The constant arrays are white-oriented, so we must convert the black coordinates so that we get the right value.
                if (p.piece == board::PAWN) black += PAWN_VALUE + PAWN_AT_POSITION_VALUES[7-num][let < 4 ? let : 7-let];
                if (p.piece == board::KNIGHT) black += KNIGHT_VALUE + KNIGHT_AT_POSITION_VALUES[7-num][let < 4 ? let : 7-let];
                if (p.piece == board::BISHOP) black += BISHOP_VALUE + BISHOP_AT_POSITION_VALUES[7-num][let < 4 ? let : 7-let];
                if (p.piece == board::ROOK) black += ROOK_VALUE + ROOK_AT_POSITION_VALUES[7-num][let < 4 ? let : 7-let];
                if (p.piece == board::QUEEN) black += QUEEN_VALUE + QUEEN_AT_POSITION_VALUES[7-num][let < 4 ? let : 7-let];
                if (p.piece == board::KING) black += KING_VALUE + KING_AT_POSITION_VALUES[7-num][let < 4 ? let : 7-let];
            }
        }
    }


    return white - black;
}

engine::moveoption engine::bestmove(board* b) {
    auto start = std::chrono::high_resolution_clock::now();

    std::set<board::move> moves = b->PossibleMoves();
    bool whiteMove = b->sideToMove;
    double topValue = whiteMove ? 10000000 : -10000000;
    board::move topMove;

    for (board::move cm : moves) {
        board *copy = new board;
        *copy = *b;
        copy->MakeMove(cm);
        double eval = minimax(copy, NOMINAL_MAX_DEPTH - 1, true, -1000000, 1000000);
        if ((eval < topValue && whiteMove)
                || (eval > topValue && !whiteMove)) {
            topValue = eval;
            topMove = cm;
        }
        delete copy; // memory management
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Time taken by function: "
             << duration.count() << " microseconds" << std::endl;

    return {topValue, topMove};
}

double engine::minimax(board* b, int depth, bool isMax, double alpha, double beta) {
    engine::kConsidered += 1.0;
    if (depth == 0) {
        return evalb(b);
    }

    if (isMax) {
        double bestValue = -10000000;
        std::set<board::move> moves = b->PossibleMoves();
        for (board::move option : moves) {
            b->MakeMove(option);
            bestValue = std::max(bestValue, minimax(b, depth-1, !isMax, alpha, beta));
            alpha = std::max(alpha, bestValue);
            b->UndoLast();
            if (alpha > beta) {
                break;
            }
        }
        return bestValue;
    }
    else {
        double bestValue = 10000000;
        std::set<board::move> moves = b->PossibleMoves();
        for (board::move option : moves) {
            b->MakeMove(option);
            bestValue = std::min(bestValue, minimax(b, depth-1, !isMax, alpha, beta));
            beta = std::min(beta, bestValue);
            b->UndoLast();
            if (beta < alpha) {
                break;
            }
        }
        return bestValue;
    }
}
