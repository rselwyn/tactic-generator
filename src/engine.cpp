#include "engine.h"
#include "evalconst.h"
#include "board.h"
#include <iostream>
#include <vector>
#include <chrono>
#include "hashmap.h"

#include "simpio.h"

// Multithreaded action
#include <pthread.h>


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

    std::vector<board::move> moves = b->PossibleMoves();
    bool whiteMove = b->sideToMove;
    double topValue = whiteMove ? 10000000 : -10000000;
    board::move topMove;

    pthread_t eval_threads[moves.size()];
    double extreme_value[moves.size()]; // max or min
    board::minimax_args* copies[moves.size()];

    for (int i = 0; i < moves.size(); i++) {
        board *copy = new board;
        *copy = *b;
        copy->MakeMove(moves[i]);
        board::minimax_args *args = new board::minimax_args;
        args->b = copy;
        args->depth = NOMINAL_MAX_DEPTH - 1;
        args->isMax = b->sideToMove;
        args->alpha = -1000000;
        args->beta = 1000000;
        args->writeVal = &extreme_value[i];
        copies[i] = args;
        pthread_create(&eval_threads[i], NULL, dispatch_minimax, (void*) args);
    }

    for (int i = 0; i < moves.size(); i++)
        pthread_join(eval_threads[i], NULL);

    std::cout << std::endl;
    for (int i = 0; i < moves.size(); i++) {
        double eval = extreme_value[i];
        if ((eval < topValue && whiteMove)
                || (eval > topValue && !whiteMove)) {
            topValue = eval;
            topMove = moves[i];
        }
        pthread_cancel(eval_threads[i]);
        delete copies[i];
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Time taken by function: "
             << duration.count() << " microseconds" << std::endl;

    return {topValue, topMove};
}

void* engine::dispatch_minimax(void *_i) {
    struct board::minimax_args *args = (struct board::minimax_args*) _i;
    double value = minimax(args->b, args->depth, args->isMax, args->alpha, args->beta);
//    std::cout << value << std::endl;
    std::cout << ".";
    *(args->writeVal) = value;

    return NULL;
}

double engine::minimax(board* b, int depth, bool isMax, double alpha, double beta) {

    engine::kConsidered += 1.0;
    if (depth == 0) {
        return evalb(b);
    }

    if (isMax) {
        double bestValue = -10000000;
        std::vector<board::move> moves = b->PossibleMoves();
        for (board::move option : moves) {
            b->MakeMove(option);
            double mm = minimax(b, depth-1, !isMax, alpha, beta);
//            std::cout << "After " << option.toString() << " Eval is " << mm << std::endl;
            bestValue = std::max(bestValue, mm);
            alpha = std::max(alpha, bestValue);
            b->UndoLast();
//            std::cout << "Undo" << std::endl;

            if (alpha > beta) {
                break;
            }
        }
        return bestValue;
    }
    else {
        double bestValue = 10000000;
        std::vector<board::move> moves = b->PossibleMoves();
        for (board::move option : moves) {
            b->MakeMove(option);
            double mm = minimax(b, depth-1, !isMax, alpha, beta);
            //            std::cout << "After " << option.toString() << " Eval is " << mm << std::endl;
            bestValue = std::min(bestValue, mm);
            beta = std::min(beta, bestValue);
            b->UndoLast();
//            std::cout << "Undo" << std::endl;
            if (beta < alpha) {
                break;
            }
        }
        return bestValue;
    }
}
